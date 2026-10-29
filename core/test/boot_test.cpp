// Boots an X68000 floppy or SASI hard disk on the host and checks what the app relies on.
//
//   boot_test <rom dir> <work dir> <image> <seconds> [<second floppy> [warp:<x>,<y> | type:<line>]]
//
// Runs the floppy for <seconds> of guest time, writing <work dir>/boot.ppm of the last picture
// and boot.wav of the sound (with <second floppy> in drive 1, when given), then presses the pad's A button, Space and Return and writes pressed.ppm. It
// presses Caps, whose lamp the reported lock keys must show for an <image> of "" (the ROM's keyboard
// handling). Then it saves a state, runs on, loads the state, and checks that the lock keys are as
// saved and the second after the load draws and plays the same as the second after the save. Last it
// starts a second machine in the same process, as the app does for its next game.

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "x68k_machine.h"

using kairo68::Config;
using kairo68::X68kMachine;

namespace {

// X68000 key scan codes.
constexpr int kSpace = 0x35;
constexpr int kReturn = 0x1d;
constexpr int kCaps = 0x5d;

// A run of this many seconds runs one frame.
constexpr double kOneFrame = 1e-6;

int fail(const std::string &message) {
    std::fprintf(stderr, "FAIL: %s\n", message.c_str());
    return 1;
}

struct Run {
    std::vector<uint8_t> picture;
    uint32_t width = 0, height = 0;
    uint64_t audio_frames = 0, loud_frames = 0;
    double seconds = 0;
    std::vector<int16_t> sound;
    // Frames the host would run flat out (X68kMachine::loading).
    uint64_t loading_frames = 0;
    // Of those, frames of the boot (X68kMachine::booting).
    uint64_t booting_frames = 0;
};

bool run(X68kMachine &machine, double seconds, Run &result, std::string &error) {
    std::vector<int16_t> audio(4096 * 2);
    const double start = result.seconds;
    while (result.seconds - start < seconds) {
        if (!machine.run_frame(error)) return false;
        result.seconds += machine.frame_seconds();
        if (machine.loading()) ++result.loading_frames;
        if (machine.booting()) ++result.booting_frames;
        uint32_t got;
        while ((got = machine.read_audio(audio.data(), 4096)) > 0) {
            result.audio_frames += got;
            result.sound.insert(result.sound.end(), audio.begin(), audio.begin() + got * 2);
            for (uint32_t f = 0; f < got; ++f)
                if (std::abs(audio[f * 2]) > 64 || std::abs(audio[f * 2 + 1]) > 64) ++result.loud_frames;
        }
    }
    const uint8_t *pixels = machine.frame(result.width, result.height);
    if (pixels) result.picture.assign(pixels, pixels + result.width * result.height * 4);
    return true;
}

// A hash of each state entry, and their names when wanted: the machine's state, compactly.
std::vector<size_t> state_hashes(X68kMachine &machine, std::vector<std::string> *names) {
    std::vector<size_t> hashes;
    for (const auto &[name, bytes] : machine.state_entries()) {
        hashes.push_back(std::hash<std::string_view>()(
            std::string_view(reinterpret_cast<const char *>(bytes.data()), bytes.size())));
        if (names) names->push_back(name);
    }
    return hashes;
}

void write_ppm(const std::string &file, const Run &run) {
    FILE *out = std::fopen(file.c_str(), "wb");
    if (!out) return;
    std::fprintf(out, "P6\n%u %u\n255\n", run.width, run.height);
    for (size_t i = 0; i < run.picture.size(); i += 4) std::fwrite(&run.picture[i], 1, 3, out);
    std::fclose(out);
}

void write_wav(const std::string &file, const std::vector<int16_t> &sound, uint32_t rate) {
    FILE *out = std::fopen(file.c_str(), "wb");
    if (!out) return;
    const uint32_t bytes = static_cast<uint32_t>(sound.size() * 2);
    auto u32 = [out](uint32_t v) { std::fwrite(&v, 4, 1, out); };
    auto u16 = [out](uint16_t v) { std::fwrite(&v, 2, 1, out); };
    std::fwrite("RIFF", 1, 4, out); u32(36 + bytes); std::fwrite("WAVEfmt ", 1, 8, out);
    u32(16); u16(1); u16(2); u32(rate); u32(rate * 4); u16(4); u16(16);
    std::fwrite("data", 1, 4, out); u32(bytes);
    std::fwrite(sound.data(), 2, sound.size(), out);
    std::fclose(out);
}

} // namespace

int main(int argc, char **argv) {
    if (argc < 5 || argc > 7)
        return fail("usage: boot_test <rom dir> <work dir> <image> <seconds> [<second floppy> [warp:<x>,<y> | type:<line>]]");
    const std::string work = argv[2];
    Config config;
    config.rom_dir = argv[1];
    config.home = work + "/home";
    // A .hdf image is a SASI hard disk; anything else goes in floppy drive 0.
    const std::string image = argv[3];
    const bool hard_disk = image.size() > 4 && (image.substr(image.size() - 4) == ".hdf" || image.substr(image.size() - 4) == ".HDF");
    const kairo68::Drive drive = hard_disk ? kairo68::kHardDisk : kairo68::kFloppy0;
    config.media[drive] = image;
    if (argc >= 6 && *argv[5]) config.media[kairo68::kFloppy1] = argv[5];
    // A direct-touch warp to this frame point after the boot, with warp.ppm of where it ends.
    int warp_x = -1, warp_y = -1;
    // A startup command typed after the boot, as the app's typer types it, with typed.ppm of the result.
    std::string typed;
    if (argc == 7 && std::string_view(argv[6]).starts_with("type:")) typed = argv[6] + 5;
    else if (argc == 7 && std::sscanf(argv[6], "warp:%d,%d", &warp_x, &warp_y) != 2)
        return fail(std::string("not warp:<x>,<y> or type:<line>: ") + argv[6]);
    const double seconds = std::atof(argv[4]);

    std::string error;
    auto machine = X68kMachine::create(config, error);
    if (!machine) return fail("create: " + error);
    Run boot;
    if (!run(*machine, seconds, boot, error)) return fail("run: " + error);
    write_ppm(work + "/boot.ppm", boot);
    write_wav(work + "/boot.wav", boot.sound, static_cast<uint32_t>(machine->sample_rate()));
    std::printf("boot: %ux%u picture, %llu audio frames (%llu audible), %.2f guest seconds, %.3f s frames\n",
                boot.width, boot.height, (unsigned long long) boot.audio_frames, (unsigned long long) boot.loud_frames,
                boot.seconds, machine->frame_seconds());
    if (boot.picture.empty()) return fail("no picture");
    std::printf("loading in %llu frames of the boot run\n", (unsigned long long) boot.loading_frames);
    uint64_t keyboard_waits = 0, keyboard_polls = 0, mouse_reads = 0;
    bool mouse_set_up = false;
    machine->guest_input(keyboard_waits, keyboard_polls, mouse_reads, mouse_set_up);
    std::printf("guest input in the boot run: %llu key waits, %llu keyboard polls, %llu mouse reads, mouse %s\n",
                (unsigned long long) keyboard_waits, (unsigned long long) keyboard_polls,
                (unsigned long long) mouse_reads, mouse_set_up ? "set up" : "not set up");
    std::printf("booting in %llu frames of the boot run\n", (unsigned long long) boot.booting_frames);
    std::printf("drive after boot: %s\n", machine->media_path(drive).c_str());
    if (!typed.empty()) {
        // The machine as kairo::StartupTyper types into it, through X68kMachine's prompt hooks.
        struct Typing {
            X68kMachine &machine;
            kairo::Prompt prompt() { return machine.prompt(); }
            bool typed_key(char c, int &code, bool &shift) { return X68kMachine::typed_key(c, code, shift); }
            int special_key(kairo::SpecialKey key) { return X68kMachine::special_key(key); }
            bool expand_command(std::string &) { return true; }
            void key(int code, bool down) { machine.key(code, down); }
        } typing{*machine};
        std::printf("before typing: the prompt %s\n", machine->prompt() == kairo::Prompt::Line ? "waits" : "does not wait");
        kairo::StartupTyper typer;
        typer.start(typed);
        Run typed_run;
        int frames = 0;
        while (typer.active() && frames < 600) {
            if (!run(*machine, kOneFrame, typed_run, error)) return fail("run while typing: " + error);
            if (!typer.frame(typing, error)) return fail("typing: " + error);
            ++frames;
        }
        if (!run(*machine, 3, typed_run, error)) return fail("run after typing: " + error);
        write_ppm(work + "/typed.ppm", typed_run);
        std::printf("typed \"%s\" %s after %d frames; the prompt %s again\n", typed.c_str(),
                    typer.active() ? "not finished" : "finished", frames,
                    machine->prompt() == kairo::Prompt::Line ? "waits" : "does not wait");
    }
    if (warp_x >= 0) {
        machine->mouse_warp(warp_x, warp_y);
        Run warped;
        int warp_frames = 0;
        while (!machine->mouse_warp_done() && warp_frames < 600) {
            if (!run(*machine, kOneFrame, warped, error)) return fail("run while warping: " + error);
            ++warp_frames;
        }
        if (!run(*machine, 0.1, warped, error)) return fail("run after the warp: " + error);
        write_ppm(work + "/warp.ppm", warped);
        std::printf("warp to %d,%d: %s after %d frames\n", warp_x, warp_y,
                    machine->mouse_warp_done() ? "done" : "not done", warp_frames);
    }
    const double expected = boot.seconds * machine->sample_rate();
    if (boot.audio_frames < expected * 0.95 || boot.audio_frames > expected * 1.05)
        return fail("audio does not follow guest time");

    Run pressed;
    machine->joystick(4, true);
    machine->key(kSpace, true);
    machine->key(kReturn, true);
    if (!run(*machine, 0.5, pressed, error)) return fail("run while pressed: " + error);
    machine->joystick(4, false);
    machine->key(kSpace, false);
    machine->key(kReturn, false);
    if (!run(*machine, 4, pressed, error)) return fail("run after press: " + error);
    write_ppm(work + "/pressed.ppm", pressed);
    // A press ends the boot: from then on only a running floppy motor runs the host flat out.
    std::printf("after the press: loading in %llu frames, booting in %llu\n",
                (unsigned long long) pressed.loading_frames, (unsigned long long) pressed.booting_frames);
    std::printf("press: the picture %s\n", pressed.picture == boot.picture ? "did not change" : "changed");

    // Caps: the ROM's keyboard handling lights its lamp, which locks() reports (bit 0), and a state
    // keeps it. A game may take the keyboard over and leave the lamp alone, so only a boot without a
    // disk must light it.
    Run caps;
    machine->key(kCaps, true);
    if (!run(*machine, 0.2, caps, error)) return fail("run while Caps is pressed: " + error);
    machine->key(kCaps, false);
    if (!run(*machine, 0.3, caps, error)) return fail("run after Caps: " + error);
    std::printf("locks after Caps: %d\n", machine->locks());
    const int locks = machine->locks();
    if (image.empty() && !(locks & 1)) return fail("Caps does not light");

    const std::string state = work + "/state.sta";
    if (!machine->save_state(state)) return fail("save state");
    // The second after the save and the second after the load, frame by frame. State a saved state
    // misses shows up as saved state that drifts apart; the first frame it does names the culprit.
    const int frames = static_cast<int>(1 / machine->frame_seconds() + 0.5);
    Run after_save, after_load, ignored;
    std::vector<std::string> names;
    std::vector<std::vector<size_t>> saved{state_hashes(*machine, &names)};
    for (int frame = 0; frame < frames; ++frame) {
        if (!run(*machine, kOneFrame, after_save, error)) return fail("run after save: " + error);
        saved.push_back(state_hashes(*machine, nullptr));
    }
    if (!run(*machine, 2, ignored, error)) return fail("run on: " + error);
    if (!machine->load_state(state)) return fail("load state");
    std::printf("locks after load: %d\n", machine->locks());
    if (machine->locks() != locks) return fail("a loaded state does not keep its lock keys");
    bool drifted = false;
    for (int frame = 0; frame <= frames; ++frame) {
        if (frame > 0 && !run(*machine, kOneFrame, after_load, error)) return fail("run after load: " + error);
        const std::vector<size_t> loaded = state_hashes(*machine, nullptr);
        if (loaded == saved[frame]) continue;
        drifted = true;
        std::printf("state: drifts %d frames after the load:", frame);
        for (size_t i = 0; i < loaded.size(); ++i)
            if (loaded[i] != saved[frame][i]) std::printf("\n  %s", names[i].c_str());
        std::printf("\n");
        if (!run(*machine, (frames - frame) * machine->frame_seconds() - kOneFrame, after_load, error))
            return fail("run after load: " + error);
        break;
    }
    write_ppm(work + "/after_save.ppm", after_save);
    write_ppm(work + "/after_load.ppm", after_load);
    write_wav(work + "/after_save.wav", after_save.sound, static_cast<uint32_t>(machine->sample_rate()));
    write_wav(work + "/after_load.wav", after_load.sound, static_cast<uint32_t>(machine->sample_rate()));
    std::printf("state: the picture of the second after load %s, its sound %s\n",
                after_save.picture == after_load.picture ? "matches" : "differs",
                after_save.sound == after_load.sound ? "matches" : "differs");
    if (drifted || after_save.picture != after_load.picture || after_save.sound != after_load.sound)
        return fail("state round trip");
    std::printf("drive: %s\n", machine->media_path(drive).c_str());

    machine.reset();
    auto second = X68kMachine::create(config, error);
    if (!second) return fail("second machine: " + error);
    Run again;
    if (!run(*second, seconds, again, error)) return fail("second run: " + error);
    write_ppm(work + "/second.ppm", again);
    std::printf("second machine: %ux%u picture, %llu audio frames\n", again.width, again.height,
                (unsigned long long) again.audio_frames);
    std::printf("PASS\n");
    return 0;
}
