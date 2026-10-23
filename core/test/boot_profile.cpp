// Measures an X68000 boot on the host: how long until the first picture that is not black, and
// until the boot ends (X68kMachine::booting), in guest and host seconds.
//
//   boot_profile <rom dir> <work dir> <image> <cpu MHz> <memory MB> <max guest seconds>
//
// Writes <work dir>/first.ppm, the first picture that is not black, and <work dir>/booted.ppm,
// the picture when the boot ended or the limit came.

#include <chrono>
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

bool black(const uint8_t *pixels, uint32_t width, uint32_t height) {
    for (size_t i = 0; i < size_t(width) * height * 4; i += 4)
        if (pixels[i] > 16 || pixels[i + 1] > 16 || pixels[i + 2] > 16) return false;
    return true;
}

void write_ppm(const std::string &file, const uint8_t *pixels, uint32_t width, uint32_t height) {
    FILE *out = std::fopen(file.c_str(), "wb");
    if (!out) return;
    std::fprintf(out, "P6\n%u %u\n255\n", width, height);
    for (size_t i = 0; i < size_t(width) * height * 4; i += 4) std::fwrite(&pixels[i], 1, 3, out);
    std::fclose(out);
}

} // namespace

int main(int argc, char **argv) {
    if (argc != 7) return fail("usage: boot_profile <rom dir> <work dir> <image> <cpu MHz> <memory MB> <max guest seconds>");
    const std::string work = argv[2];
    Config config;
    config.rom_dir = argv[1];
    config.home = work;
    config.media[kairo68::kFloppy0] = argv[3];
    config.cpu_mhz = std::atoi(argv[4]);
    config.ram_mb = std::atoi(argv[5]);
    const double limit = std::atof(argv[6]);

    const auto start = std::chrono::steady_clock::now();
    auto wall = [&start] { return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count(); };
    std::string error;
    auto machine = X68kMachine::create(config, error);
    if (!machine) return fail("create: " + error);
    const double created = wall();
    std::vector<int16_t> audio(4096 * 2);
    double guest = 0, first_guest = -1, first_wall = -1;
    while (machine->booting() && guest < limit) {
        if (!machine->run_frame(error)) return fail("run: " + error);
        guest += machine->frame_seconds();
        while (machine->read_audio(audio.data(), 4096) > 0) {}
        uint32_t width = 0, height = 0;
        const uint8_t *pixels = machine->frame(width, height);
        if (first_guest < 0 && pixels && !black(pixels, width, height)) {
            first_guest = guest;
            first_wall = wall();
            write_ppm(work + "/first.ppm", pixels, width, height);
        }
    }
    uint32_t width = 0, height = 0;
    if (const uint8_t *pixels = machine->frame(width, height)) write_ppm(work + "/booted.ppm", pixels, width, height);
    std::printf("%d MHz, %d MB: machine created in %.2f s; first picture at %.2f guest s (%.2f s); "
                "%s %.2f guest s (%.2f s)\n", config.cpu_mhz, config.ram_mb, created, first_guest, first_wall,
                machine->booting() ? "still booting at" : "booted at", guest, wall());
    return 0;
}
