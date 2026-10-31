// The X68000 machine (core/x68k_machine.h) as the MachineCore the shared frame host runs
// (shared/frontend/src/main/cpp/kairo/machine_core.h).

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "kairo/blank_image.h"
#include "kairo/machine_core.h"
#include "kairo/thread_priority.h"
#include "x68k_machine.h"

namespace {

using kairo68::X68kMachine;

// Media slots, as MainActivity numbers them: the core's drives.
static_assert(kairo68::kFloppy0 == 0 && kairo68::kFloppy1 == 1 && kairo68::kHardDisk == 2);
constexpr const char *kStateFile = "/state.sta";
// MainActivity.stateError reads this.

class X68kCore : public kairo::MachineCore {
public:
    explicit X68kCore(std::unique_ptr<X68kMachine> machine) : machine_(std::move(machine)) {}

    bool run_frame() override { return machine_->run_frame(error_); }
    std::string error() override { return error_; }

    // MAME's screen bitmap itself, B, G, R, A bytes inside wider rows (Kairo#50).
    const uint8_t *frame(uint32_t &width, uint32_t &height) override {
        const uint8_t *pixels = machine_->picture(width, height, stride_);
        if (pixels) return pixels;
        // Black until the machine draws its first picture.
        width = 768;
        height = 512;
        stride_ = 0;
        blank_.resize(width * height * 4);
        for (size_t i = 3; i < blank_.size(); i += 4) blank_[i] = 0xff;
        return blank_.data();
    }
    bool frame_bgra() override { return true; }
    uint32_t frame_stride() override { return stride_; }

    double frame_seconds() override { return machine_->frame_seconds(); }
    int32_t audio_rate() override { return machine_->sample_rate(); }
    uint32_t read_audio(int16_t *stereo, uint32_t max_frames) override { return machine_->read_audio(stereo, max_frames); }

    void release_inputs() override {
        for (int code = 0; code < kairo68::kKeyCodes; ++code)
            if (keys_[code]) key(code, false);
        for (int control = 0; control < kairo68::kJoystickControls; ++control) machine_->joystick(control, false);
        machine_->mouse_button(1, false);
        machine_->mouse_button(2, false);
    }

    // X68000 scan codes, as X68kKeys names them.
    void key(int code, bool down) override {
        if (code <= 0 || code >= kairo68::kKeyCodes || keys_[code] == down) return;
        keys_[code] = down;
        machine_->key(code, down);
    }

    // Controls in X68kKeys.joystick order, which is the core's pad order.
    void joystick(int control, bool down) override { machine_->joystick(control, down); }
    void mouse_move(int dx, int dy) override { machine_->mouse_move(dx, dy); }
    void mouse_button(int button, bool down) override { machine_->mouse_button(button, down); }

    void reset() override { machine_->reset(); }
    // FrameMachine fast-forwards while the core boots (Config::skip_memory_check).
    bool loading() override { return machine_->loading(); }
    // Bit 0 Caps, bit 1 Kana, as NativeMachine lists them (MainActivity).
    int locks() override { return machine_->locks(); }
    void mouse_warp(int x, int y) override { machine_->mouse_warp(x, y); }

    // Startup commands (Kairo68#70), typed at Human68k's command prompt.
    kairo::Prompt prompt() override { return machine_->prompt(); }
    bool typed_key(char c, int &code, bool &shift) override { return X68kMachine::typed_key(c, code, shift); }
    int special_key(kairo::SpecialKey key) override { return X68kMachine::special_key(key); }
    bool mouse_warp_done() override { return machine_->mouse_warp_done(); }
    // Auto touch mode (Kairo#78): the guest's IOCS keyboard and mouse calls.
    bool guest_input(GuestInput &input) override {
        machine_->guest_input(input.keyboard_waits, input.keyboard_polls, input.mouse_reads, input.mouse_enabled);
        return true;
    }

    int save_state(const std::string &directory) override {
        return machine_->save_state(directory + kStateFile, state_error_) ? 0 : kFailed;
    }
    // MAME's reason, or why the machine could not get to a point it can save at.
    std::string state_error() override { return state_error_; }

    // The emulator's state holds no disk image: the host records and restores them (machine_jni.cpp).
    bool media_in_state() override { return false; }
    int load_state(const std::string &directory) override {
        return machine_->load_state(directory + kStateFile, state_error_) ? 0 : kFailed;
    }

    int media(int slot, const std::string &path) override {
        if (slot < 0 || slot >= kairo68::kDrives) return kUnsupported;
        return machine_->insert(kairo68::Drive(slot), path) ? 0 : kFailed;
    }

    std::vector<std::string> media_paths() override {
        std::vector<std::string> paths;
        for (int drive = 0; drive < kairo68::kDrives; ++drive) paths.push_back(machine_->media_path(kairo68::Drive(drive)));
        return paths;
    }

private:
    std::unique_ptr<X68kMachine> machine_;
    std::string error_;
    std::string state_error_;
    std::vector<uint8_t> blank_;
    uint32_t stride_ = 0;
    std::array<bool, kairo68::kKeyCodes> keys_{};
};

} // namespace

std::unique_ptr<kairo::MachineCore> kairo::create_core(const kairo::Launch &launch, std::string &error) {
    kairo68::Config config;
    // MAME's thread does the emulation; FrameMachine's worker only waits for its frames.
    config.on_emulation_thread = [] {
        kairo::name_thread("kairo68-mame");
        kairo::raise_emulation_priority("MAME");
    };
    config.rom_dir = launch.firmware;
    config.home = launch.home;
    for (int drive = 0; drive < kairo68::kDrives; ++drive)
        if (int(launch.media.size()) > drive) config.media[drive] = launch.media[drive];
    // The setting X68kCatalog's options name; every launch passes it.
    config.ram_mb = std::stoi(launch.settings.at("memory"));
    config.cpu_mhz = std::stoi(launch.settings.at("cpu"));
    if (config.cpu_mhz != 10 && config.cpu_mhz != 16 && config.cpu_mhz != 25) {
        error = "The CPU clock is " + std::to_string(config.cpu_mhz) + " MHz, not 10, 16 or 25";
        return nullptr;
    }
    const std::string &skip = launch.settings.at("skipMemoryCheck");
    if (skip != "on" && skip != "off") {
        error = "Skip memory check is \"" + skip + "\", not on or off";
        return nullptr;
    }
    config.skip_memory_check = skip == "on";
    auto machine = X68kMachine::create(config, error);
    if (!machine) return nullptr;
    return std::make_unique<X68kCore>(std::move(machine));
}

// The blank disks MainActivity.blankDisks lists, as the X68000's drives took them; Human68k's
// FORMAT.X prepares each. A 2HD floppy is an XDF, its 1,232 KB of sectors. SASI hard disks of
// 10, 20 and 40 MB are zeroed 256-byte blocks, the layout MAME's own SASI image creator writes
// (x68k_hdc_image_device::call_create, 0x13c98 blocks for 20 MB).
std::string kairo::create_blank_disk(const std::string &kind, const std::string &path) {
    if (kind == "floppy-2hd") return kairo::write_zeroed_image(path, 77 * 2 * 8 * 1024);
    uint64_t blocks;
    if (kind == "sasi-10") blocks = 0x9f54;
    else if (kind == "sasi-20") blocks = 0x13c98;
    else if (kind == "sasi-40") blocks = 0x279c8;
    else return "Kairo68 makes no blank disk " + kind;
    return kairo::write_zeroed_image(path, blocks * 256);
}
