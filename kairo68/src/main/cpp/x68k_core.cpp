// The X68000 machine (core/x68k_machine.h) as the MachineCore the shared frame host runs
// (shared/frontend/src/main/cpp/kairo/machine_core.h).

#include <array>
#include <memory>
#include <string>
#include <vector>

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
    bool booting() override { return machine_->booting(); }
    // Bit 0 Caps, bit 1 Kana, as NativeMachine lists them (MainActivity).
    int locks() override { return machine_->locks(); }

    int save_state(const std::string &directory) override {
        return machine_->save_state(directory + kStateFile) ? 0 : kFailed;
    }

    // The emulator's state holds no disk image: the host records and restores them (machine_jni.cpp).
    bool media_in_state() override { return false; }
    int load_state(const std::string &directory) override {
        return machine_->load_state(directory + kStateFile) ? 0 : kFailed;
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
