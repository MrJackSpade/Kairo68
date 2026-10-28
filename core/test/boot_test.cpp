// Boots an X68000 floppy or SASI hard disk on the host and checks what the app relies on.
//
//   boot_test <rom dir> <work dir> <image> <seconds>
//
// Runs the floppy for <seconds> of guest time, writing <work dir>/boot.ppm of the last picture
// and boot.wav of the sound, then presses the pad's A button, Space and Return and writes pressed.ppm. It
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
};

bool run(X68kMachine &machine, double seconds, Run &result, std::string &error) {
    std::vector<int16_t> audio(4096 * 2);
    const double start = result.seconds;
    while (result.seconds - start < seconds) {
        if (!machine.run_frame(error)) return false;
        result.seconds += machine.frame_seconds();
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
    if (argc != 5) return fail("usage: boot_test <rom dir> <work dir> <image> <seconds>");
    const std::string work = argv[2];
    Config config;
    config.rom_dir = argv[1];
    config.home = work + "/home";
    // A .hdf image is a SASI hard disk; anything else goes in floppy drive 0.
    const std::string image = argv[3];
    const bool hard_disk = image.size() > 4 && (image.substr(image.size() - 4) == ".hdf" || image.substr(image.size() - 4) == ".HDF");
    const kairo68::Drive drive = hard_disk ? kairo68::kHardDisk : kairo68::kFloppy0;
    config.media[drive] = image;
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
    std::printf("drive after boot: %s\n", machine->media_path(drive).c_str());
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
