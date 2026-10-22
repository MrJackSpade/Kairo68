// A Sharp X68000 on MAME's x68k driver (third_party/mame), run one video frame at a time by its
// host instead of MAME's own real-time loop. MAME runs on its own thread and stops at the end of
// every frame until the host asks for the next one; the host supplies video, sound and input
// through Kairo68's OSD in place of MAME's.

#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace kairo68 {

// What a machine starts with.
struct Config {
    // The folder holding the ROM set as MAME looks for it: <rom_dir>/x68000/iplrom.dat and
    // cgrom.dat, or <rom_dir>/x68000.zip.
    std::string rom_dir;
    // A private folder for MAME's NVRAM and configuration.
    std::string home;
    // Floppy images for drives 0 and 1; "" for an empty drive.
    std::string fd[2];
    // The audio sample rate.
    int sample_rate = 48000;
};

// The joystick in port 1, in JoystickInputRouter's order: up, down, left, right, trigger A, B.
constexpr int kJoystickControls = 6;

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

    // A key by MAME's keyboard item ID (input_item_id, osd/interface/inputcode.h).
    void key(int item, bool down);
    void joystick(int control, bool down);
    // The mouse: motion in mickeys, and button 1 (left) or 2 (right).
    void mouse_move(int dx, int dy);
    void mouse_button(int button, bool down);

    void reset();
    // Run on the machine's own thread at the end of the frame; false when MAME refused.
    bool save_state(const std::string &file);
    bool load_state(const std::string &file);

    // Inserts an image in floppy drive 0 or 1, or ejects it for ""; true when it is in.
    bool insert_fd(int drive, const std::string &path);
    std::string fd_path(int drive);

private:
    struct Impl;
    explicit X68kMachine(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

} // namespace kairo68
