// See x68k_machine.h. MAME's front end (src/frontend) is not part of the subset: this file is the
// whole host side, a machine manager that runs one machine and an OSD that hands each frame,
// its sound, and the player's input across to the host thread.

#include "x68k_machine.h"
#include "kairo68_perf.h"

#include <array>
#include <atomic>
#include <bit>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "emu.h"
#include "config.h"
#include "emuopts.h"
#include "fileio.h"
#include "input.h"
#include "inputdev.h"
#include "main.h"
#include "drivenum.h"
#include "render.h"
#include "screen.h"
#include "cpu/m68000/m68000.h"
#include "imagedev/floppy.h"
#include "sound/ymopm.h"
#include "sharp/x68k_hdc.h"
#include "audio_effects/compressor.h"
#include "audio_effects/eq.h"
#include "audio_effects/filter.h"
#include "audio_effects/reverb.h"
#include "ui/uimain.h"
#include "osdepend.h"
#include "modules/lib/osdlib.h"

extern const char bare_build_version[];
extern const char build_version[];

// MAME's front end defines these; Kairo68 runs MAME without one.
const char *emulator_info::get_appname() { return "kairo68"; }
const char *emulator_info::get_appname_lower() { return "kairo68"; }
const char *emulator_info::get_configname() { return "kairo68"; }
const char *emulator_info::get_copyright() { return "Copyright Nicola Salmoria and the MAME team"; }
const char *emulator_info::get_copyright_info() { return get_copyright(); }
const char *emulator_info::get_bare_build_version() { return bare_build_version; }
const char *emulator_info::get_build_version() { return build_version; }
void emulator_info::display_ui_chooser(running_machine &) {}
int emulator_info::start_frontend(emu_options &, osd_interface &, std::vector<std::string> &) { return 0; }
int emulator_info::start_frontend(emu_options &, osd_interface &, int, char *[]) { return 0; }
bool emulator_info::draw_user_interface(running_machine &) { return false; }
void emulator_info::periodic_check() {}
bool emulator_info::frame_hook() { return false; }
void emulator_info::sound_hook(const std::map<std::string, std::vector<std::pair<const float *, int>>> &) {}
void emulator_info::layout_script_cb(layout_file &, const char *) {}
bool emulator_info::standalone() { return false; }

namespace kairo68 {
namespace {

constexpr const char *kSystem = "x68000";
// Each Drive's image device by its command-line (brief instance) name.
constexpr const char *kDriveNames[kDrives] = {"flop1", "flop2", "sasi"};
// A guest frame's sound is read by the host after every frame; more than a second waiting means
// the host stopped reading, and the machine stops instead of growing the buffer.
constexpr size_t kMaxQueuedSeconds = 1;

// Errors MAME reports through osd_printf_error, for the message the host shows.
class ErrorCapture : public osd_output {
public:
    void output_callback(osd_output_channel channel, util::format_argument_pack<char> const &args) override {
        if (channel == OSD_OUTPUT_CHANNEL_ERROR) {
            std::ostringstream text;
            util::stream_format(text, args);
            std::lock_guard<std::mutex> lock(mutex_);
            errors_ += text.str();
        }
        chain_output(channel, args);
    }
    std::string text() {
        std::lock_guard<std::mutex> lock(mutex_);
        return errors_;
    }

private:
    std::mutex mutex_;
    std::string errors_;
};

class Manager : public machine_manager {
public:
    Manager(emu_options &options, osd_interface &osd) : machine_manager(options, osd) {}
    ui_manager *create_ui(running_machine &machine) override {
        ui_ = std::make_unique<ui_manager>(machine);
        return ui_.get();
    }

private:
    std::unique_ptr<ui_manager> ui_;
};

} // namespace

struct X68kMachine::Impl : public osd_interface {
    Config config;

    // The frame handshake: MAME stops at the end of each frame until the host asks for the next.
    std::mutex mutex;
    std::condition_variable changed;
    bool frame_ready = false;
    bool go = false;
    bool exiting = false;
    bool finished = false;
    std::string error;
    std::thread thread;
    running_machine *machine = nullptr;
    // Work the host asks for, run on MAME's thread between frames. The frame ends inside the
    // screen's VBLANK timer callback, where loading a state or an image would be undone by the
    // scheduler re-arming its timers afterwards; so when work arrives, the machine pauses and
    // leaves the callback, and MAME's run loop calls update() again from its paused branch,
    // between timeslices, where the work runs.
    std::vector<std::function<void(running_machine &)>> tasks;
    uint64_t tasks_done = 0;
    bool paused_for_tasks = false;

    // Video.
    render_target *target = nullptr;
    // The latest picture as 0xAARRGGBB words (alpha undefined): the screen's own bitmap, or owned
    // when it had to be dimmed or must outlive frames MAME skips (render_frame).
    const uint32_t *picture = nullptr;
    uint32_t picture_pitch = 0; // words from one row to the next
    std::vector<uint32_t> owned;
    // frame()'s RGBA copy of the picture, for the tools.
    mutable std::vector<uint32_t> rgba;
    uint32_t width = 0, height = 0;
    double frame_period = 1.0 / 55.46;

    // Sound: interleaved stereo frames MAME has produced and the host has not read.
    std::deque<int16_t> sound;
    uint32_t stream_id = 0;

    // Input. Keys and the pad drive the guest's own fields (ioport_field::set_value, as MAME's
    // Lua input does), so every X68000 key works, including those MAME gives no host key, and no
    // host key reaches MAME's UI. The host sets the wanted states; MAME's thread applies them when
    // it polls input. The mouse moves relatively, so it is an OSD device.
    std::array<std::atomic<bool>, kKeyCodes> keys{};
    std::array<std::atomic<bool>, kJoystickControls> joystick{};
    std::array<ioport_field *, kKeyCodes> key_fields{};
    std::array<ioport_field *, kJoystickControls> pad_fields{};
    std::array<bool, kKeyCodes> keys_applied{};
    std::array<bool, kJoystickControls> pad_applied{};
    bool fields_found = false;
    // The keyboard's Caps and かな lights (x68k_kbd.cpp outputs), lit at 0; found with the fields.
    osd::output_item const *led_caps = nullptr;
    osd::output_item const *led_kana = nullptr;
    std::atomic<int> mouse_dx{0}, mouse_dy{0};
    int mouse_frame_dx = 0, mouse_frame_dy = 0;
    std::array<std::atomic<bool>, 2> mouse_buttons{};

    // The end of the first frame sets what needs the running machine: the CPU clock, and no throttle.
    bool started = false;

    // The boot (X68kMachine::booting), read at the end of each frame on MAME's thread.
    std::atomic<bool> booting{true};
    // Whether a floppy drive's motor ran at the end of the last frame (X68kMachine::loading).
    std::atomic<bool> floppy_running{false};
    bool drive_ran = false;
    // Whether the guest read a joystick port in this frame and in the one before. A game polls the
    // joystick every frame once it runs, which ends a boot whose drive motor stays on (Kairo68#2);
    // a startup driver that reads the ports does so within one frame, so one frame is not enough.
    bool joystick_read = false;
    bool joystick_read_before = false;
    memory_passthrough_handler joystick_tap;
    void update_booting();
#ifdef KAIRO68_PERF
    // Config::count_accesses: one read and one write tap per counted address range.
    std::vector<memory_passthrough_handler> access_taps;
    void count_accesses(address_space &space);
#endif

    ErrorCapture errors;

    void run_mame();

    // osd_interface
    void init(running_machine &machine) override;
    void free_target();
    void config_load(config_type type, config_level level, util::xml::data_node const *node);
    void config_save(config_type, util::xml::data_node *) {}
    void update(bool skip_redraw) override;
    void render_frame();
    void own_picture();
    void run_tasks(std::unique_lock<std::mutex> &lock);
    void input_update(bool relative_reset) override;
    void find_fields();
    void check_osd_inputs() override {}
    void set_verbose(bool) override {}
    void init_debugger() override {}
    void wait_for_debugger(device_t &, bool) override {}
    bool no_sound() override { return false; }
    bool sound_external_per_channel_volume() override { return false; }
    bool sound_split_streams_per_source() override { return false; }
    uint32_t sound_get_generation() override { return 1; }
    osd::audio_info sound_get_information() override;
    uint32_t sound_stream_sink_open(uint32_t, std::string, uint32_t) override { return stream_id = 1; }
    uint32_t sound_stream_source_open(uint32_t, std::string, uint32_t) override { return 0; }
    void sound_stream_close(uint32_t id) override { if (id == stream_id) stream_id = 0; }
    void sound_stream_sink_update(uint32_t id, const int16_t *buffer, int samples_this_frame) override;
    void sound_stream_source_update(uint32_t, int16_t *, int) override {}
    void sound_stream_set_volumes(uint32_t, const std::vector<float> &) override {}
    void sound_begin_update() override {}
    void sound_end_update() override {}
    void customize_input_type_list(std::vector<input_type_entry> &) override {}
    void add_audio_to_recording(const int16_t *, int) override {}
    std::vector<ui::menu_item> get_slider_list() override { return {}; }
    osd_font::ptr font_alloc() override { return nullptr; }
    bool get_font_families(std::string const &, std::vector<std::pair<std::string, std::string>> &) override { return false; }
    bool execute_command(const char *) override { return false; }
    std::unique_ptr<osd::midi_input_port> create_midi_input(std::string_view) override { return nullptr; }
    std::unique_ptr<osd::midi_output_port> create_midi_output(std::string_view) override { return nullptr; }
    std::vector<osd::midi_port_info> list_midi_ports() override { return {}; }
    std::unique_ptr<osd::network_device> open_network_device(int, osd::network_handler &) override { return nullptr; }
    std::vector<osd::network_device_info> list_network_devices() override { return {}; }

    // Runs task on MAME's thread, which is waiting at the end of a frame, and waits for it. The
    // machine does not advance.
    template <typename Result>
    Result between_frames(std::function<Result(running_machine &)> task) {
        Result result{};
        std::unique_lock<std::mutex> lock(mutex);
        if (!machine || finished) return result;
        tasks.push_back([&result, task](running_machine &m) { result = task(m); });
        const uint64_t wanted = tasks_done + tasks.size();
        changed.notify_all();
        changed.wait(lock, [&] { return tasks_done >= wanted || finished; });
        return result;
    }
};

X68kMachine::X68kMachine(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}

void X68kMachine::Impl::run_mame() {
    if (config.on_emulation_thread) config.on_emulation_thread();
    osd_output::push(&errors);
    int result = EMU_ERR_NONE;
    try {
        emu_options options;
        options.set_value(OPTION_READCONFIG, "0", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_WRITECONFIG, "0", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_SKIP_GAMEINFO, "1", OPTION_PRIORITY_CMDLINE);
        // The host paces frames.
        options.set_value(OPTION_THROTTLE, "0", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_SLEEP, "0", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_FRAMESKIP, "0", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_MOUSE, "1", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_JOYSTICK, "1", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_SAMPLERATE, std::to_string(config.sample_rate), OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_MEDIAPATH, config.rom_dir, OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_CFG_DIRECTORY, config.home + "/cfg", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_NVRAM_DIRECTORY, config.home + "/nvram", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_STATE_DIRECTORY, config.home + "/sta", OPTION_PRIORITY_CMDLINE);
        options.set_value(OPTION_BIOS, config.bios, OPTION_PRIORITY_CMDLINE);
        if (!config.rtc.empty()) options.set_value(OPTION_RTC_TIME, config.rtc, OPTION_PRIORITY_CMDLINE);
        options.set_system_name(kSystem);
        options.set_value(OPTION_RAMSIZE, std::to_string(config.ram_mb) + "M", OPTION_PRIORITY_CMDLINE);
        for (int drive = 0; drive < kDrives; ++drive)
            if (!config.media[drive].empty())
                // By the drive's command-line name; image_option() wants its full instance name.
                options.set_value(kDriveNames[drive], config.media[drive], OPTION_PRIORITY_CMDLINE);
        Manager manager(options, *this);
        // running_machine requires the manager's HTTP manager; with OPTION_HTTP off it serves nothing.
        manager.start_http_server();
        machine_config machine_config(*options.system(), options);
        running_machine running(machine_config, manager);
        manager.set_machine(&running);
        result = running.run(false);
        manager.set_machine(nullptr);
    } catch (emu_fatalerror &failure) {
        std::lock_guard<std::mutex> lock(mutex);
        error = failure.what();
    } catch (std::exception &failure) {
        std::lock_guard<std::mutex> lock(mutex);
        error = failure.what();
    }
    osd_output::pop(&errors);
    std::lock_guard<std::mutex> lock(mutex);
    if (error.empty() && !exiting) {
        error = errors.text();
        if (error.empty()) error = "The X68000 stopped (MAME error " + std::to_string(result) + ")";
    }
    machine = nullptr;
    finished = true;
    changed.notify_all();
}

// The machine's sound is the core's sound: MAME's output effects, by default a 20 Hz high-pass
// filter and an EQ, are off. They are a listener's setting rather than the machine's, and their
// filter state is not part of a saved state.
void X68kMachine::Impl::config_load(config_type type, config_level, util::xml::data_node const *) {
    if (type != config_type::FINAL) return;
    sound_manager &sound = machine->sound();
    const std::vector<audio_effect *> chain = sound.default_effect_chain();
    auto &filter = dynamic_cast<audio_effect_filter &>(*chain.at(audio_effect::FILTER));
    filter.set_highpass_active(false);
    filter.set_lowpass_active(false);
    dynamic_cast<audio_effect_compressor &>(*chain.at(audio_effect::COMPRESSOR)).set_mode(0);
    dynamic_cast<audio_effect_reverb &>(*chain.at(audio_effect::REVERB)).set_mode(0);
    dynamic_cast<audio_effect_eq &>(*chain.at(audio_effect::EQ)).set_mode(0);
    for (u32 entry = 0; entry < chain.size(); ++entry) sound.default_effect_changed(entry);
}

void X68kMachine::Impl::free_target() {
    machine->render().target_free(target);
    target = nullptr;
}

void X68kMachine::Impl::init(running_machine &running) {
    machine = &running;
    // A target showing only the screen (no views with artwork, such as the driver's floppy lamps),
    // at its native pixels. Nothing draws it: MAME updates a screen only while a target shows it,
    // and render_frame copies the screen's own bitmap.
    target = running.render().target_alloc(nullptr, RENDER_CREATE_NO_ART);
    target->set_keepaspect(false);
    // Freed while the machine exits, as MAME's windows free theirs: a target outliving that frees
    // its layouts' textures through a render manager that is already gone.
    running.add_notifier(MACHINE_NOTIFY_EXIT, machine_notify_delegate(&Impl::free_target, this));
    running.configuration().config_register("kairo68", configuration_manager::load_delegate(&Impl::config_load, this),
        configuration_manager::save_delegate(&Impl::config_save, this));
    auto &input = running.input();

    // The mouse: motion since the last poll, and two buttons.
    auto &mouse = input.add_device(DEVICE_CLASS_MOUSE, "Kairo68 mouse", "kairo68-mouse", this);
    mouse.add_item("X", "", ITEM_ID_XAXIS, [](void *device, void *) -> s32 {
        return static_cast<Impl *>(device)->mouse_frame_dx * osd::input_device::RELATIVE_PER_PIXEL;
    }, nullptr);
    mouse.add_item("Y", "", ITEM_ID_YAXIS, [](void *device, void *) -> s32 {
        return static_cast<Impl *>(device)->mouse_frame_dy * osd::input_device::RELATIVE_PER_PIXEL;
    }, nullptr);
    for (int button = 0; button < 2; ++button) {
        mouse.add_item("Button " + std::to_string(button + 1), "", input_item_id(ITEM_ID_BUTTON1 + button),
            [](void *device, void *index) -> s32 {
                return static_cast<Impl *>(device)->mouse_buttons[reinterpret_cast<intptr_t>(index)].load() ? 1 : 0;
            }, reinterpret_cast<void *>(intptr_t(button)));
    }
}

void X68kMachine::Impl::input_update(bool) {
    mouse_frame_dx = mouse_dx.exchange(0);
    mouse_frame_dy = mouse_dy.exchange(0);
    // The ports are complete once the machine runs.
    if (machine->phase() != machine_phase::RUNNING) return;
    if (!fields_found) find_fields();
    for (int code = 0; code < kKeyCodes; ++code) {
        const bool down = keys[code];
        if (key_fields[code] && down != keys_applied[code]) {
            key_fields[code]->set_value(down);
            keys_applied[code] = down;
        }
    }
    for (int control = 0; control < kJoystickControls; ++control) {
        const bool down = joystick[control];
        if (down != pad_applied[control]) {
            pad_fields[control]->set_value(down);
            pad_applied[control] = down;
        }
    }
}

// The keyboard's LINE0 to LINEE ports hold eight keys each, bit b of LINEn being scan code
// n * 8 + b; the pad is joystick port 1's FM Towns pad.
void X68kMachine::Impl::find_fields() {
    fields_found = true;
    int mouse_fields = 0;
    for (auto &[tag, port] : machine->ioport().ports()) {
        const auto line = tag.rfind(":LINE");
        if (tag.starts_with(":keyboard:") && line != std::string::npos && line + 6 == tag.size()) {
            const int row = std::stoi(tag.substr(line + 5), nullptr, 16);
            for (ioport_field &field : port->fields())
                if (field.type() == IPT_KEYBOARD) key_fields[row * 8 + std::countr_zero(field.mask())] = &field;
        } else if (tag.starts_with(":mouse_port:")) {
            // The X68000 mouse's axes and buttons read Kairo68's mouse (init), the first mouse.
            // MAME maps them to the third mouse by default, which no host has, so the guest's mouse
            // never moved (Kairo68#32).
            for (ioport_field &field : port->fields()) {
                input_code code;
                switch (field.type()) {
                case IPT_MOUSE_X: code = MOUSECODE_X_INDEXED(0); break;
                case IPT_MOUSE_Y: code = MOUSECODE_Y_INDEXED(0); break;
                case IPT_BUTTON1: code = MOUSECODE_BUTTON1_INDEXED(0); break;
                case IPT_BUTTON2: code = MOUSECODE_BUTTON2_INDEXED(0); break;
                default: continue;
                }
                ioport_field::user_settings settings;
                field.get_user_settings(settings);
                settings.seq[SEQ_TYPE_STANDARD] = input_seq(code);
                field.set_user_settings(settings);
                ++mouse_fields;
            }
        } else if (tag == ":joy1:townspad:PAD") {
            for (ioport_field &field : port->fields()) {
                switch (field.type()) {
                case IPT_JOYSTICK_UP: pad_fields[0] = &field; break;
                case IPT_JOYSTICK_DOWN: pad_fields[1] = &field; break;
                case IPT_JOYSTICK_LEFT: pad_fields[2] = &field; break;
                case IPT_JOYSTICK_RIGHT: pad_fields[3] = &field; break;
                case IPT_BUTTON1: pad_fields[4] = &field; break;
                case IPT_BUTTON2: pad_fields[5] = &field; break;
                case IPT_START: pad_fields[6] = &field; break;
                case IPT_SELECT: pad_fields[7] = &field; break;
                default: break;
                }
            }
        }
    }
    machine->output().notify_all([this](osd::output_item const &item) {
        if (item.name() == "key_led_caps") led_caps = &item;
        if (item.name() == "key_led_kana") led_kana = &item;
    });
    if (!led_caps || !led_kana) throw emu_fatalerror("The X68000 keyboard's Caps and Kana lights were not found");
    if (!key_fields[0x01]) throw emu_fatalerror("The X68000 keyboard's ports were not found");
    if (mouse_fields != 4) throw emu_fatalerror("The X68000 mouse's axes and buttons were not found");
    for (ioport_field *field : pad_fields)
        if (!field) throw emu_fatalerror("Joystick port 1's FM Towns pad was not found");
}

osd::audio_info X68kMachine::Impl::sound_get_information() {
    // One stereo output at the configured rate, as MAME's own single-device modules describe theirs.
    osd::audio_info result;
    result.m_generation = 1;
    result.m_default_sink = 1;
    result.m_default_source = 0;
    result.m_nodes.resize(1);
    auto &node = result.m_nodes[0];
    node.m_name = "kairo68";
    node.m_display_name = "Kairo68";
    node.m_id = 1;
    node.m_rate.m_default_rate = 0;
    node.m_rate.m_min_rate = 0;
    node.m_rate.m_max_rate = 0;
    node.m_sinks = 2;
    node.m_sources = 0;
    node.m_port_names = {"L", "R"};
    node.m_port_positions = {osd::channel_position::FL(), osd::channel_position::FR()};
    if (stream_id) result.m_streams.emplace_back(stream_id, 1);
    return result;
}

void X68kMachine::Impl::sound_stream_sink_update(uint32_t id, const int16_t *buffer, int samples_this_frame) {
    if (id != stream_id || samples_this_frame <= 0) return;
    std::lock_guard<std::mutex> lock(mutex);
    if (sound.size() / 2 + size_t(samples_this_frame) > size_t(config.sample_rate) * kMaxQueuedSeconds) {
        error = "The X68000's sound was not read (" + std::to_string(sound.size() / 2) + " frames queued, " +
                std::to_string(samples_this_frame) + " more)";
        exiting = true;
        return;
    }
    sound.insert(sound.end(), buffer, buffer + samples_this_frame * 2);
}

void X68kMachine::Impl::update(bool skip_redraw) {
    std::unique_lock<std::mutex> lock(mutex);
    if (paused_for_tasks) {
        // From the run loop's paused branch: between timeslices, outside any timer.
        run_tasks(lock);
        paused_for_tasks = false;
        machine->resume();
    } else {
        // The end of a frame, inside the VBLANK callback.
        lock.unlock();
        if (!started) {
            started = true;
            // The host paces frames. MAME's own OSDs apply its throttle option; without this the
            // video manager holds the machine to real time, so a fast-forwarded boot cannot run faster.
            machine->video().set_throttled(false);
            device_t *cpu = machine->root_device().subdevice("maincpu");
            if (!cpu) throw emu_fatalerror("The X68000's CPU was not found");
            cpu->set_unscaled_clock(u32(config.cpu_mhz) * 1000000);
            // Only the CPU changes main RAM while it runs; everything else may have a side effect.
            if (config.idle_skip)
                downcast<m68000_device &>(*cpu).enable_idle_skip(offs_t(config.ram_mb) * 0x100000);
            if (config.direct_memory) {
                // The access counters tap main RAM, which direct accesses would go past.
                if (config.count_accesses) throw emu_fatalerror("count_accesses needs direct_memory off");
                downcast<m68000_device &>(*cpu).enable_direct_memory();
            }
            if (config.synthesis_worker) {
                device_t *fm = machine->root_device().subdevice("ym2151");
                if (!fm) throw emu_fatalerror("The X68000's YM2151 was not found");
                downcast<ym2151_device &>(*fm).enable_synthesis_worker();
            }
            // The PPI's ports A and B are the two joystick ports.
            joystick_tap = cpu->memory().space(AS_PROGRAM).install_read_tap(
                    0xe9a000, 0xe9a003, "kairo68_joystick",
                    [this](offs_t, u16 &, u16) { joystick_read = true; });
#ifdef KAIRO68_PERF
            if (config.count_accesses) count_accesses(cpu->memory().space(AS_PROGRAM));
            kairo68::perf::count_pcs() = config.count_pcs;
#endif
        }
        if (!skip_redraw) render_frame();
        update_booting();
        // A boot runs as fast as the host can, and its frames go by unseen: drawing every one
        // (x68k_state::screen_update and the renderer) took half the device's time (Kairo68#2).
        // MAME's fast-forward skips 11 frames in 12, with their screen updates, until the boot ends.
        machine->video().set_fastforward(config.skip_memory_check && booting.load());
        // Fast-forwarded frames may go undrawn and leave the picture as it is, while MAME draws
        // on (or reallocates) the screen's bitmaps: keep a copy of it meanwhile.
        if (machine->video().fastforward()) own_picture();
        lock.lock();
        if (!exiting) {
            frame_ready = true;
            changed.notify_all();
            changed.wait(lock, [this] { return go || exiting || !tasks.empty(); });
            if (!tasks.empty() && !exiting) {
                paused_for_tasks = true;
                machine->pause();
                return;
            }
        }
    }
    go = false;
    if (exiting) machine->schedule_exit();
}

// Runs the host's work until the host asks for the next frame.
void X68kMachine::Impl::run_tasks(std::unique_lock<std::mutex> &lock) {
    while (true) {
        changed.wait(lock, [this] { return go || exiting || !tasks.empty(); });
        if (tasks.empty()) return;
        auto pending = std::move(tasks);
        tasks.clear();
        lock.unlock();
        for (auto &task : pending) task(*machine);
        lock.lock();
        tasks_done += pending.size();
        changed.notify_all();
    }
}

#ifdef KAIRO68_PERF
void X68kMachine::Impl::count_accesses(address_space &space) {
    using namespace kairo68::perf;
    const offs_t ram_end = offs_t(config.ram_mb) * 0x100000 - 1;
    const std::pair<offs_t, offs_t> ranges[] = {
        {0x000000, ram_end}, {0xc00000, 0xdfffff}, {0xe00000, 0xe7ffff}, {0xe80000, 0xeaffff}, {0xeb0000, 0xebffff}};
    const Region regions[] = {kRam, kGraphicsVram, kTextVram, kIo, kSpriteRam};
    for (int i = 0; i < 5; ++i) {
        const Region region = regions[i];
        access_taps.push_back(space.install_readwrite_tap(ranges[i].first, ranges[i].second, "kairo68_count",
            [region](offs_t, u16 &, u16) { ++counters().reads[region]; },
            [region](offs_t, u16 &, u16) { ++counters().writes[region]; }));
    }
    // Everything else: the unused space after RAM, and the user I/O, SRAM, CG-ROM and IPL-ROM.
    for (const auto &[start, end] : {std::pair<offs_t, offs_t>{ram_end + 1, 0xbfffff}, {0xec0000, 0xffffff}})
        access_taps.push_back(space.install_readwrite_tap(start, end, "kairo68_count",
            [](offs_t, u16 &, u16) { ++counters().reads[kOther]; },
            [](offs_t, u16 &, u16) { ++counters().writes[kOther]; }));
}
#endif

void X68kMachine::Impl::update_booting() {
    if (!machine) return;
    bool running = false;
    for (floppy_image_device &drive : device_type_enumerator<floppy_image_device>(machine->root_device()))
        if (drive.exists() && !drive.mon_r()) running = true;
    floppy_running = running;
    if (!booting.load()) return;
    if (joystick_read && joystick_read_before) booting = false;
    joystick_read_before = joystick_read;
    joystick_read = false;
    for (x68k_hdc_image_device &disk : device_type_enumerator<x68k_hdc_image_device>(machine->root_device()))
        if (disk.exists() && disk.busy()) booting = false;
    if (running) drive_ran = true;
    else if (drive_ran) booting = false;
}

// The finished picture straight from the screen's bitmap (Kairo68#20). The target shows only the
// screen, at its native size, as one unblended quad at full brightness, so MAME's renderer only
// copied the visible area. At full brightness the picture is that area of the bitmap itself, its
// 0xRRGGBB words handed to the presenter as B, G, R, A bytes (Kairo#50); the host reads it before
// the next frame runs. The target stays allocated: MAME updates a screen only while a target
// shows it.
void X68kMachine::Impl::render_frame() {
    KAIRO68_PERF_TIME(render_ns);
    screen_device *screen = screen_device_enumerator(machine->root_device()).first();
    if (!screen) throw emu_fatalerror("The X68000's screen was not found");
    frame_period = screen->frame_period().as_double();
    screen_bitmap &finished = screen->curbitmap();
    if (!finished.valid()) return;
    if (finished.format() != BITMAP_FORMAT_RGB32) throw emu_fatalerror("The X68000's screen is not RGB32");
    const bitmap_rgb32 &bitmap = finished.as_rgb32();
    const rectangle &visible = screen->visible_area();
    const uint32_t w = uint32_t(visible.width()), h = uint32_t(visible.height());
    // The quad's colour, as screen_device::update_quads sets it: the screen's colour darkened by its
    // brightness, which the driver sets from the monitor contrast for fades. MAME's renderer
    // scales each channel by trunc(256 * c / 255) and shifts it down by 8 (rendersw.hxx,
    // draw_quad_rgb32); the integer form gives the same factor for every c, since 256c/255 is a
    // whole number only at 0 and 255.
    const rgb_t quad = screen->color() - rgb_t(0, 0xff - screen->brightness(), 0xff - screen->brightness(),
                                               0xff - screen->brightness());
    width = w;
    height = h;
    if (quad.r() == 0xff && quad.g() == 0xff && quad.b() == 0xff) {
        picture = &bitmap.pix(visible.top(), visible.left());
        picture_pitch = uint32_t(bitmap.rowpixels());
        return;
    }
    const uint32_t sr = uint32_t(quad.r()) * 256 / 255, sg = uint32_t(quad.g()) * 256 / 255,
                   sb = uint32_t(quad.b()) * 256 / 255;
    owned.resize(size_t(w) * h);
    uint32_t *out = owned.data();
    for (int32_t y = visible.top(); y <= visible.bottom(); ++y) {
        const uint32_t *row = &bitmap.pix(y, visible.left());
        for (uint32_t x = 0; x < w; ++x) {
            const uint32_t rgb = row[x];
            const uint32_t r = (((rgb >> 16) & 0xff) * sr) >> 8, g = (((rgb >> 8) & 0xff) * sg) >> 8,
                           b = ((rgb & 0xff) * sb) >> 8;
            *out++ = 0xff000000u | (r << 16) | (g << 8) | b;
        }
    }
    picture = owned.data();
    picture_pitch = w;
}

// Copies a picture that is the screen's bitmap into owned.
void X68kMachine::Impl::own_picture() {
    if (!picture || picture == owned.data()) return;
    std::vector<uint32_t> copy(size_t(width) * height);
    for (uint32_t y = 0; y < height; ++y)
        std::copy_n(picture + size_t(y) * picture_pitch, width, copy.data() + size_t(y) * width);
    owned = std::move(copy);
    picture = owned.data();
    picture_pitch = width;
}

std::unique_ptr<X68kMachine> X68kMachine::create(const Config &config, std::string &error) {
    auto impl = std::make_unique<Impl>();
    impl->config = config;
    Impl *state = impl.get();
    impl->thread = std::thread([state] { state->run_mame(); });
    {
        std::unique_lock<std::mutex> lock(state->mutex);
        state->changed.wait(lock, [state] { return state->frame_ready || state->finished; });
        if (state->finished) {
            error = state->error;
            lock.unlock();
            state->thread.join();
            return nullptr;
        }
    }
    return std::unique_ptr<X68kMachine>(new X68kMachine(std::move(impl)));
}

X68kMachine::~X68kMachine() {
    {
        std::lock_guard<std::mutex> lock(impl_->mutex);
        impl_->exiting = true;
        impl_->changed.notify_all();
    }
    impl_->thread.join();
}

bool X68kMachine::run_frame(std::string &error) {
    std::unique_lock<std::mutex> lock(impl_->mutex);
    if (impl_->finished || impl_->exiting) {
        error = impl_->error.empty() ? "The X68000 stopped" : impl_->error;
        return false;
    }
    impl_->frame_ready = false;
    impl_->go = true;
    impl_->changed.notify_all();
    impl_->changed.wait(lock, [this] { return impl_->frame_ready || impl_->finished; });
    if (impl_->finished || impl_->exiting) {
        error = impl_->error.empty() ? "The X68000 stopped" : impl_->error;
        return false;
    }
    return true;
}

double X68kMachine::frame_seconds() const { return impl_->frame_period; }

const uint8_t *X68kMachine::frame(uint32_t &width, uint32_t &height) const {
    if (!impl_->picture) return nullptr;
    width = impl_->width;
    height = impl_->height;
    // 0xAARRGGBB to the RGBA bytes: red in the low byte, opaque.
    impl_->rgba.resize(size_t(width) * height);
    uint32_t *out = impl_->rgba.data();
    for (uint32_t y = 0; y < height; ++y) {
        const uint32_t *row = impl_->picture + size_t(y) * impl_->picture_pitch;
        for (uint32_t x = 0; x < width; ++x) {
            const uint32_t rgb = row[x];
            *out++ = 0xff000000u | ((rgb & 0xff) << 16) | (rgb & 0xff00) | ((rgb >> 16) & 0xff);
        }
    }
    return reinterpret_cast<const uint8_t *>(impl_->rgba.data());
}

const uint8_t *X68kMachine::picture(uint32_t &width, uint32_t &height, uint32_t &stride) const {
    if (!impl_->picture) return nullptr;
    width = impl_->width;
    height = impl_->height;
    stride = impl_->picture_pitch * 4;
    return reinterpret_cast<const uint8_t *>(impl_->picture);
}

int X68kMachine::sample_rate() const { return impl_->config.sample_rate; }

uint32_t X68kMachine::read_audio(int16_t *stereo, uint32_t max_frames) {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    const uint32_t frames = std::min<uint32_t>(max_frames, uint32_t(impl_->sound.size() / 2));
    std::copy_n(impl_->sound.begin(), frames * 2, stereo);
    impl_->sound.erase(impl_->sound.begin(), impl_->sound.begin() + frames * 2);
    return frames;
}

void X68kMachine::key(int code, bool down) {
    if (code > 0 && code < kKeyCodes) impl_->keys[code] = down;
    if (down) impl_->booting = false;
}

void X68kMachine::joystick(int control, bool down) {
    if (control >= 0 && control < kJoystickControls) impl_->joystick[control] = down;
    if (down) impl_->booting = false;
}

void X68kMachine::mouse_move(int dx, int dy) {
    impl_->mouse_dx += dx;
    impl_->mouse_dy += dy;
}

void X68kMachine::mouse_button(int button, bool down) {
    if (button == 1 || button == 2) impl_->mouse_buttons[button - 1] = down;
    if (down) impl_->booting = false;
}

void X68kMachine::reset() {
    impl_->between_frames<bool>([this](running_machine &m) {
        m.schedule_soft_reset();
        impl_->drive_ran = false;
        impl_->joystick_read = false;
        impl_->joystick_read_before = false;
        impl_->booting = true;
        return true;
    });
}

bool X68kMachine::booting() const { return impl_->config.skip_memory_check && impl_->booting.load(); }

bool X68kMachine::loading() const { return booting() || impl_->floppy_running.load(); }

int X68kMachine::locks() {
    return impl_->between_frames<int>([this](running_machine &) {
        if (!impl_->fields_found) impl_->find_fields();
        return (impl_->led_caps->value() == 0 ? 1 : 0) | (impl_->led_kana->value() == 0 ? 2 : 0);
    });
}

bool X68kMachine::save_state(const std::string &file) {
    return impl_->between_frames<bool>([file](running_machine &m) {
        // As running_machine::handle_saveload: anonymous timers cannot be saved or restored.
        if (!m.scheduler().can_save()) return false;
        emu_file out(OPEN_FLAG_WRITE | OPEN_FLAG_CREATE | OPEN_FLAG_CREATE_PATHS);
        if (out.open(file)) return false;
        return m.save().write_file(out) == STATERR_NONE;
    });
}

bool X68kMachine::load_state(const std::string &file) {
    // A saved state is past the boot.
    impl_->booting = false;
    return impl_->between_frames<bool>([file](running_machine &m) {
        // As running_machine::handle_saveload: anonymous timers cannot be saved or restored.
        if (!m.scheduler().can_save()) return false;
        emu_file in(OPEN_FLAG_READ);
        if (in.open(file)) return false;
        return m.save().read_file(in) == STATERR_NONE;
    });
}

std::vector<std::pair<std::string, uint64_t>> X68kMachine::take_perf() {
    std::vector<std::pair<std::string, uint64_t>> taken;
#ifdef KAIRO68_PERF
    using namespace kairo68::perf;
    // MAME's thread wrote them before the frame handshake and waits for the next frame.
    std::lock_guard<std::mutex> lock(impl_->mutex);
    Counters &c = counters();
    taken = {{"instructions", c.instructions}, {"slices", c.slices}, {"cpu_ns", c.cpu_ns},
             {"timer_fires", c.timer_fires}, {"timer_ns", c.timer_ns}, {"screen_updates", c.screen_updates},
             {"screen_lines", c.screen_lines}, {"screen_ns", c.screen_ns}, {"sound_ns", c.sound_ns},
             {"render_ns", c.render_ns}, {"screen_under_ns", c.screen_under_ns}, {"screen_gfx_ns", c.screen_gfx_ns},
             {"screen_pcg_ns", c.screen_pcg_ns}, {"screen_mix_ns", c.screen_mix_ns},
             {"idle_skips", c.idle_skips}, {"idle_cycles", c.idle_cycles},
             {"fm_ns", c.fm_ns}, {"fm_tail_ns", c.fm_tail_ns}};
    static const char *const kRegionNames[kRegions] = {"ram", "gvram", "tvram", "io", "sprite", "other"};
    if (impl_->config.count_accesses)
        for (int region = 0; region < kRegions; ++region) {
            taken.emplace_back(std::string("reads.") + kRegionNames[region], c.reads[region]);
            taken.emplace_back(std::string("writes.") + kRegionNames[region], c.writes[region]);
        }
    for (const auto &[name, timer] : c.timers) {
        taken.emplace_back(std::string("timer.") + name + ".fires", timer.first);
        taken.emplace_back(std::string("timer.") + name + ".ns", timer.second);
    }
    for (const auto &[pc, count] : c.pcs) {
        char name[16];
        std::snprintf(name, sizeof name, "pc.%06x", unsigned(pc));
        taken.emplace_back(name, count);
    }
    c = Counters();
#endif
    return taken;
}

std::vector<uint16_t> X68kMachine::read_words(uint32_t address, size_t count) {
    return impl_->between_frames<std::vector<uint16_t>>([address, count](running_machine &m) {
        device_t *cpu = m.root_device().subdevice("maincpu");
        if (!cpu) throw emu_fatalerror("The X68000's CPU was not found");
        address_space &space = cpu->memory().space(AS_PROGRAM);
        auto quiet = m.disable_side_effects();
        std::vector<uint16_t> words;
        for (size_t i = 0; i < count; ++i) words.push_back(space.read_word((address + 2 * i) & 0xfffffe));
        return words;
    });
}

std::vector<std::pair<std::string, std::vector<uint8_t>>> X68kMachine::state_entries() {
    using Entries = std::vector<std::pair<std::string, std::vector<uint8_t>>>;
    return impl_->between_frames<Entries>([](running_machine &m) {
        // As save_manager::do_write lays them out, after the same pre-save callbacks.
        save_manager &save = m.save();
        save.dispatch_presave();
        Entries entries;
        for (int index = 0; index < save.registration_count(); ++index) {
            void *base;
            u32 size, count, blocks, stride;
            const char *name = save.indexed_item(index, base, size, count, blocks, stride);
            std::vector<uint8_t> bytes;
            const uint8_t *data = static_cast<const uint8_t *>(base);
            for (u32 block = 0; block < blocks; ++block, data += stride)
                bytes.insert(bytes.end(), data, data + size * count);
            entries.emplace_back(name, std::move(bytes));
        }
        return entries;
    });
}

namespace {

device_image_interface &drive_image(running_machine &machine, Drive drive) {
    for (device_image_interface &image : image_interface_enumerator(machine.root_device()))
        if (image.brief_instance_name() == kDriveNames[drive]) return image;
    throw emu_fatalerror("The X68000 has no %s drive", kDriveNames[drive]);
}

} // namespace

bool X68kMachine::insert(Drive drive, const std::string &path) {
    return impl_->between_frames<bool>([drive, path](running_machine &m) {
        device_image_interface &image = drive_image(m, drive);
        if (path.empty()) {
            image.unload();
            return true;
        }
        return !image.load(path).first;
    });
}

std::string X68kMachine::media_path(Drive drive) {
    return impl_->between_frames<std::string>([drive](running_machine &m) {
        device_image_interface &image = drive_image(m, drive);
        return image.exists() ? std::string(image.filename()) : std::string();
    });
}

} // namespace kairo68
