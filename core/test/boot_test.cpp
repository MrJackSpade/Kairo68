// Boots an X68000 floppy on the host and checks what the app relies on.
//
//   boot_test <rom dir> <work dir> <floppy image> <seconds>
//
// Runs the floppy for <seconds> of guest time, writing <work dir>/boot.ppm of the last picture
// and boot.wav of the sound. Then it saves a state, runs on, loads the state, and checks that the
// frames after the load draw the same picture as those after the save. Last it starts a second
// machine in the same process, as the app does for its next game.

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "x68k_machine.h"

using kairo68::Config;
using kairo68::X68kMachine;

namespace {

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
    if (argc != 5) return fail("usage: boot_test <rom dir> <work dir> <floppy image> <seconds>");
    const std::string work = argv[2];
    Config config;
    config.rom_dir = argv[1];
    config.home = work + "/home";
    config.fd[0] = argv[3];
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
    const double expected = boot.seconds * machine->sample_rate();
    if (boot.audio_frames < expected * 0.95 || boot.audio_frames > expected * 1.05)
        return fail("audio does not follow guest time");

    const std::string state = work + "/state.sta";
    if (!machine->save_state(state)) return fail("save state");
    Run after_save, after_load, ignored;
    if (!run(*machine, 1, after_save, error)) return fail("run after save: " + error);
    if (!run(*machine, 2, ignored, error)) return fail("run on: " + error);
    if (!machine->load_state(state)) return fail("load state");
    if (!run(*machine, 1, after_load, error)) return fail("run after load: " + error);
    write_ppm(work + "/after_save.ppm", after_save);
    write_ppm(work + "/after_load.ppm", after_load);
    std::printf("state: the second after load %s the second after save\n",
                after_save.picture == after_load.picture ? "matches" : "differs from");
    if (after_save.picture != after_load.picture) return fail("state round trip");
    std::printf("drive 0: %s\n", machine->fd_path(0).c_str());

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
