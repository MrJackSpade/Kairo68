// A Sharp X68000 on MAME's x68k driver (third_party/mame), run one video frame at a time by its
// host instead of MAME's own real-time loop. MAME runs on its own thread and stops at the end of
// every frame until the host asks for the next one; the host supplies video, sound and input
// through Kairo68's OSD in place of MAME's.

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace kairo68 {

// The X68000's drives: floppy drives 0 and 1, and the SASI hard disk.
enum Drive { kFloppy0, kFloppy1, kHardDisk, kDrives };

// What a machine starts with.
struct Config {
    // The folder holding the ROM set as MAME looks for it: <rom_dir>/x68000/iplrom.dat and
    // cgrom.dat, or <rom_dir>/x68000.zip.
    std::string rom_dir;
    // The IPL-ROM, by MAME's BIOS name for it: ipl10 is iplrom.dat (IPL-ROM 1.0).
    std::string bios = "ipl10";
    // Main memory in megabytes, 1 to 12.
    int ram_mb = 4;
    // The 68000's clock in MHz: 10 (X68000), 16 (XVI) or 25 (X68030). The machine is always the
    // X68000, whose SASI hard disk and drivers every game knows; only its CPU runs faster.
    int cpu_mhz = 10;
    // A private folder for MAME's NVRAM and configuration.
    std::string home;
    // The image in each Drive (floppy images, a SASI hard disk image); "" for an empty drive.
    std::string media[kDrives];
    // The audio sample rate.
    int sample_rate = 48000;
    // Skip memory check: the IPL cannot skip its memory test, so the boot (booting()) runs as fast
    // as the host can, with MAME drawing one frame in 12, until it ends.
    bool skip_memory_check = true;
    // A measurement build (KAIRO68_PERF) also counts the guest's accesses by address range. The
    // taps that count them slow every access, so stage times measured with them are not real.
    bool count_accesses = false;
    // The real-time clock's start, as YYYYMMDDhhmmss; "" for the host's time. A fixed start makes
    // two runs of a game identical, for checking that a change keeps the guest's behavior.
    std::string rtc;
    // Called first on the thread MAME runs on, which does all the emulation: the host's place to
    // give it the priority it gives its own emulation thread (Kairo68#26).
    std::function<void()> on_emulation_thread;
};

// X68000 key scan codes: 0x01 (ESC) to 0x74 (OPT.2), as the keyboard sends them.
constexpr int kKeyCodes = 0x80;
// The pad in joystick port 1, in its bit order: up, down, left, right, A, B, Run, Select.
constexpr int kJoystickControls = 8;

class X68kMachine {
public:
    // A machine booting config's floppies; null with error set when it cannot start.
    static std::unique_ptr<X68kMachine> create(const Config &config, std::string &error);
    ~X68kMachine();

    // Runs one guest video frame; false with error set when the machine stops.
    bool run_frame(std::string &error);
    // The guest time a frame lasts.
    double frame_seconds() const;
    // The latest picture, RGBA rows of width pixels; null before the first one.
    const uint8_t *frame(uint32_t &width, uint32_t &height) const;
    int sample_rate() const;
    // Takes up to max_frames interleaved stereo frames produced so far.
    uint32_t read_audio(int16_t *stereo, uint32_t max_frames);

    // A key by X68000 scan code.
    void key(int code, bool down);
    void joystick(int control, bool down);
    // The mouse: motion in mickeys, and button 1 (left) or 2 (right).
    void mouse_move(int dx, int dy);
    void mouse_button(int button, bool down);

    void reset();
    // With Config::skip_memory_check, true from power-on or reset until the boot has loaded: until
    // a floppy drive that has run stops, a hard disk has been read, or the guest polls the joystick
    // ports. A key, pad or mouse press ends it at once. Always false without skip_memory_check.
    bool booting() const;
    // Run on the machine's own thread at the end of the frame; false when MAME refused.
    bool save_state(const std::string &file);
    bool load_state(const std::string &file);

    // A measurement build's counters (core/kairo68_perf.h) since the last call, by name, then
    // cleared; empty in any other build. Between frames only.
    std::vector<std::pair<std::string, uint64_t>> take_perf();

    // Every entry a saved state holds, by name, as it would hold it now; for tests that check a
    // loaded state restores the machine exactly.
    std::vector<std::pair<std::string, std::vector<uint8_t>>> state_entries();

    // Inserts an image in a drive, or ejects it for ""; true when it is in.
    bool insert(Drive drive, const std::string &path);
    // The image in a drive; "" when it is empty.
    std::string media_path(Drive drive);

private:
    struct Impl;
    explicit X68kMachine(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

} // namespace kairo68
