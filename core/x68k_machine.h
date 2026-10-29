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

#include "kairo/startup_typer.h"

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
    // The 68000 skips the passes of a busy-wait loop on main RAM, which the guest cannot tell from
    // running them (Kairo68#12). Off only to compare against.
    bool idle_skip = true;
    // The YM2151 synthesizes on a worker thread, in parallel with the emulation, with the same
    // samples (Kairo68#22). Off only to compare against.
    bool synthesis_worker = true;
    // The 68000 reads and writes plain memory (main RAM, ROM) through host pointers instead of
    // MAME's dispatch (Kairo68#13, #14). Off only to compare against; count_accesses needs it off.
    bool direct_memory = true;
    // A measurement build (KAIRO68_PERF) also counts the guest's accesses by address range. The
    // taps that count them slow every access, so stage times measured with them are not real.
    bool count_accesses = false;
    // A measurement build also counts the guest's instructions by address (take_perf's "pc."
    // entries), to find where the guest spends them; slower still.
    bool count_pcs = false;
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
    // The latest picture, RGBA rows of width pixels; null before the first one. It converts
    // picture() each call: for the tools, which compare it between builds.
    const uint8_t *frame(uint32_t &width, uint32_t &height) const;
    // The latest picture as rows of B, G, R and an undefined alpha byte, stride bytes apart; null
    // before the first one. Valid until the next run_frame.
    const uint8_t *picture(uint32_t &width, uint32_t &height, uint32_t &stride) const;
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
    // Whether the guest boots or a floppy drive's motor runs, which the host runs flat out (Kairo#82).
    bool loading() const;
    // The lock keys on, as the keyboard's lights show them: bit 0 Caps, bit 1 Kana (かな).
    int locks();
    // Startup commands (kairo::StartupTyper): whether Human68k's command prompt waits for a line, the
    // JIS key that types a character, and the X68000's own Return, Shift, Ctrl and function keys.
    kairo::Prompt prompt() const;
    static bool typed_key(char c, int &code, bool &shift);
    static int special_key(kairo::SpecialKey key);
    // Direct touch: steers the IOCS mouse pointer to frame point (x, y) over the next frames. A guest
    // that never set the IOCS mouse up has no pointer to steer, and the warp never finishes.
    void mouse_warp(int x, int y);
    // Whether the pointer reached the last warp's point, or stopped where the guest clamps it.
    bool mouse_warp_done() const;
    // How the guest reads its input, as counts since the machine started: its IOCS calls waiting for
    // a key (_B_KEYINP) and polling the keyboard (_B_KEYSNS); and its mouse reads, IOCS calls
    // (_MS_STAT, _MS_GETDT, _MS_CURGT) and a program's own accesses to the mouse's SCC channel; and
    // whether it set the mouse up with any IOCS mouse call. Patch group m68000-trap-hook.
    void guest_input(uint64_t &keyboard_waits, uint64_t &keyboard_polls, uint64_t &mouse_reads,
                     bool &mouse_set_up) const;
    // Run on the machine's own thread at the end of the frame; false when MAME refused.
    bool save_state(const std::string &file);
    bool load_state(const std::string &file);

    // A measurement build's counters (core/kairo68_perf.h) since the last call, by name, then
    // cleared; empty in any other build. Between frames only.
    std::vector<std::pair<std::string, uint64_t>> take_perf();
    // count words of the guest's memory from address, read as the debugger reads them (without
    // side effects); empty when the machine is not running. Between frames only.
    std::vector<uint16_t> read_words(uint32_t address, size_t count);

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
