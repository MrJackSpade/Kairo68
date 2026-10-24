// Measures a game run (Kairo68#28): boots an image, then runs a fixed number of frames as fast as
// the host can with scripted input, and reports host time per guest frame. A measurement build
// (KAIRO68_PERF) adds what the guest did and where the emulation thread's time went per frame.
// The same run on every build, on the host or on the device, makes builds comparable.
//
//   game_profile <rom dir> <work dir> <image> <cpu MHz> <memory MB> <frames> <script> [options]
//
// <script> is a comma-separated list of <frame>:<+|-><key|pad><code> events, by frame after the
// boot ends: "+" presses, "-" lets go. key codes are X68000 scan codes (0x39 is Space, 0x1d is
// Return); pad codes are the pad bits (4 is A, 6 is Run). "-" alone is no input.
//
// Options:
//   --boot-frames <n> run exactly n frames as the boot instead of until the boot ends (for a game
//                     that waits at a prompt with its drive running, whose boot never ends)
//   --warmup <n>      run n frames after the boot before measuring (script frames count from the boot)
//   --rtc <time>      start the real-time clock at YYYYMMDDhhmmss (default 19930101000000), so
//                     runs repeat exactly; "host" for the host's time
//   --nice <n>        run the emulation thread at this priority (the app uses -16; root on Android)
//   --no-idle-skip    run every pass of the guest's busy-wait loops (Config::idle_skip)
//   --accesses        count accesses by address range (slows every access; stage times are not real)
//   --pcs <n>         count instructions by address and list the <n> busiest with their code (slower still)
//   --shots <n>       write the picture every n frames as <work dir>/frame-<frame>.ppm
//   --csv <file>      write each frame's host time and counters
//   --sound <file>    write the measured frames' sound, 16-bit stereo at the machine's rate
//   --hashes <file>   write a hash of each frame's picture and sound, to show that two builds run
//                     the guest identically (a fast path must not change a single frame)
//
// Prints one summary: frames per second the host reached, the guest's real rate, the frame time
// distribution, and the counters per frame.

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#ifndef _WIN32
#include <atomic>
#include <cerrno>
#include <cstring>
#include <sys/resource.h>
#include <unistd.h>
#endif

#include "x68k_machine.h"

using kairo68::Config;
using kairo68::X68kMachine;

namespace {

int fail(const std::string &message) {
    std::fprintf(stderr, "FAIL: %s\n", message.c_str());
    return 1;
}

struct Event {
    long frame;
    bool down;
    bool pad;
    int code;
};

// Parses the script; false with error set when an event is malformed.
bool parse_script(const std::string &script, std::vector<Event> &events, std::string &error) {
    if (script == "-") return true;
    size_t start = 0;
    while (start <= script.size()) {
        const size_t end = std::min(script.find(',', start), script.size());
        const std::string item = script.substr(start, end - start);
        const size_t colon = item.find(':');
        if (colon == std::string::npos || colon + 5 > item.size() || (item[colon + 1] != '+' && item[colon + 1] != '-')) {
            error = "bad script event \"" + item + "\"";
            return false;
        }
        Event event{};
        event.frame = std::strtol(item.substr(0, colon).c_str(), nullptr, 10);
        event.down = item[colon + 1] == '+';
        const std::string device = item.substr(colon + 2, 3);
        if (device != "key" && device != "pad") {
            error = "bad script device in \"" + item + "\"";
            return false;
        }
        event.pad = device == "pad";
        event.code = int(std::strtol(item.substr(colon + 5).c_str(), nullptr, 0));
        events.push_back(event);
        start = end + 1;
    }
    std::stable_sort(events.begin(), events.end(), [](const Event &a, const Event &b) { return a.frame < b.frame; });
    return true;
}

void write_ppm(const std::string &file, const uint8_t *pixels, uint32_t width, uint32_t height) {
    FILE *out = std::fopen(file.c_str(), "wb");
    if (!out) return;
    std::fprintf(out, "P6\n%u %u\n255\n", width, height);
    for (size_t i = 0; i < size_t(width) * height * 4; i += 4) std::fwrite(&pixels[i], 1, 3, out);
    std::fclose(out);
}

#ifndef _WIN32
// The emulation thread, for its scheduler statistics.
std::atomic<long> emulation_tid{0};
#endif

// The calling thread's priority; fails the run when the system refuses it.
void set_priority(int nice) {
#ifdef _WIN32
    std::fprintf(stderr, "FAIL: --nice is for Android and Linux\n");
    std::exit(1);
#else
    emulation_tid = long(gettid());
    if (setpriority(PRIO_PROCESS, 0, nice) != 0) {
        std::fprintf(stderr, "FAIL: setpriority(%d): %s\n", nice, std::strerror(errno));
        std::exit(1);
    }
#endif
}

double percentile(std::vector<double> values, double fraction) {
    if (values.empty()) return 0;
    std::sort(values.begin(), values.end());
    return values[std::min(values.size() - 1, size_t(fraction * double(values.size())))];
}

} // namespace

#ifdef GAME_PROFILE_LIBRARY
// In the shared library (KAIRO68_TOOL_SHARED), started by tool_core.cpp's main.
__attribute__((visibility("default"))) int kairo68_game_profile_main(int argc, char **argv) {
#else
int main(int argc, char **argv) {
#endif
    if (argc < 8)
        return fail("usage: game_profile <rom dir> <work dir> <image> <cpu MHz> <memory MB> <frames> <script> "
                    "[--nice <n>] [--boot-frames <n>] [--warmup <n>] [--rtc <time>] [--no-idle-skip] [--accesses] [--pcs <n>] [--shots <n>] [--csv <file>] [--hashes <file>] [--sound <file>]");
    const std::string work = argv[2];
    Config config;
    config.rom_dir = argv[1];
    config.home = work;
    config.media[kairo68::kFloppy0] = argv[3];
    config.cpu_mhz = std::atoi(argv[4]);
    config.ram_mb = std::atoi(argv[5]);
    config.rtc = "19930101000000";
    const long frames = std::strtol(argv[6], nullptr, 10);
    std::vector<Event> events;
    std::string error;
    if (!parse_script(argv[7], events, error)) return fail(error);
    long shots = 0;
    long warmup = 0;
    long top_pcs = 0;
    long fixed_boot = -1;
    std::string csv_file;
    std::string hash_file;
    std::string sound_file;
    for (int i = 8; i < argc; ++i) {
        const std::string option = argv[i];
        if (option == "--accesses") config.count_accesses = true;
        else if (option == "--no-idle-skip") config.idle_skip = false;
        else if (option == "--pcs" && i + 1 < argc) { config.count_pcs = true; top_pcs = std::strtol(argv[++i], nullptr, 10); }
        else if (option == "--rtc" && i + 1 < argc) { config.rtc = argv[++i]; if (config.rtc == "host") config.rtc.clear(); }
        else if (option == "--warmup" && i + 1 < argc) warmup = std::strtol(argv[++i], nullptr, 10);
        else if (option == "--boot-frames" && i + 1 < argc) fixed_boot = std::strtol(argv[++i], nullptr, 10);
        else if (option == "--nice" && i + 1 < argc) {
            const int nice = std::atoi(argv[++i]);
            config.on_emulation_thread = [nice] { set_priority(nice); };
        }
        else if (option == "--shots" && i + 1 < argc) shots = std::strtol(argv[++i], nullptr, 10);
        else if (option == "--csv" && i + 1 < argc) csv_file = argv[++i];
        else if (option == "--hashes" && i + 1 < argc) hash_file = argv[++i];
        else if (option == "--sound" && i + 1 < argc) sound_file = argv[++i];
        else return fail("unknown option " + option);
    }
    if (frames <= 0) return fail("frames must be positive");

    auto machine = X68kMachine::create(config, error);
    if (!machine) return fail("create: " + error);
    std::vector<int16_t> audio(4096 * 2);
    // FNV-1a over each frame's sound as it is read, and over its picture.
    uint64_t sound_hash = 0;
    FILE *sound_out = nullptr;
    auto hash = [](uint64_t h, const void *data, size_t bytes) {
        const uint8_t *p = static_cast<const uint8_t *>(data);
        for (size_t i = 0; i < bytes; ++i) h = (h ^ p[i]) * 0x100000001b3ull;
        return h;
    };
    auto drain = [&] {
        while (const uint32_t got = machine->read_audio(audio.data(), 4096))
        {
            sound_hash = hash(sound_hash, audio.data(), size_t(got) * 2 * sizeof(int16_t));
            if (sound_out) std::fwrite(audio.data(), sizeof(int16_t), size_t(got) * 2, sound_out);
        }
    };

    // The boot runs flat out, with MAME skipping most of its frames as the app does (Config::
    // skip_memory_check); its rate is reported apart from the game's.
    long boot_frames = 0;
    double boot_guest = 0;
    const auto boot_start = std::chrono::steady_clock::now();
    while (fixed_boot >= 0 ? boot_frames < fixed_boot : machine->booting()) {
        if (!machine->run_frame(error)) return fail("boot: " + error);
        boot_guest += machine->frame_seconds();
        drain();
        if (++boot_frames > 60 * 120 && fixed_boot < 0)
            return fail("the boot did not end in 120 guest seconds (--boot-frames runs a fixed number)");
    }
    const double boot_host = std::chrono::duration<double>(std::chrono::steady_clock::now() - boot_start).count();
    machine->take_perf();

    FILE *csv = csv_file.empty() ? nullptr : std::fopen(csv_file.c_str(), "w");
    FILE *hashes = hash_file.empty() ? nullptr : std::fopen(hash_file.c_str(), "w");
    if (!hash_file.empty() && !hashes) return fail("cannot write " + hash_file);
    if (!sound_file.empty() && !(sound_out = std::fopen(sound_file.c_str(), "wb")))
        return fail("cannot write " + sound_file);
    if (!csv_file.empty() && !csv) return fail("cannot write " + csv_file);
    std::vector<double> frame_ms;
    std::map<std::string, uint64_t> totals;
    std::vector<std::string> csv_columns;
    double guest = 0;
    size_t next_event = 0;
    auto start = std::chrono::steady_clock::now();
    for (long frame = -warmup; frame < frames; ++frame) {
        // Script frames count from the boot.
        for (; next_event < events.size() && events[next_event].frame <= frame + warmup; ++next_event) {
            const Event &event = events[next_event];
            if (event.pad) machine->joystick(event.code, event.down);
            else machine->key(event.code, event.down);
        }
        if (frame < 0) {
            if (!machine->run_frame(error)) return fail("warmup: " + error);
            drain();
            machine->take_perf();
            start = std::chrono::steady_clock::now();
            continue;
        }
        const auto before = std::chrono::steady_clock::now();
        if (!machine->run_frame(error)) return fail("frame " + std::to_string(frame) + ": " + error);
        const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - before).count();
        frame_ms.push_back(ms);
        guest += machine->frame_seconds();
        sound_hash = 0xcbf29ce484222325ull;
        drain();
        if (hashes) {
            uint32_t width = 0, height = 0;
            const uint8_t *pixels = machine->frame(width, height);
            const uint64_t picture = pixels ? hash(0xcbf29ce484222325ull, pixels, size_t(width) * height * 4) : 0;
            std::fprintf(hashes, "%ld %ux%u %016llx %016llx\n", frame, width, height,
                         (unsigned long long)picture, (unsigned long long)sound_hash);
        }
        const auto counters = machine->take_perf();
        for (const auto &[name, value] : counters) totals[name] += value;
        if (csv) {
            if (frame == 0) {
                std::fprintf(csv, "frame,ms");
                for (const auto &[name, value] : counters)
                    if (name.rfind("timer.", 0) != 0) {
                        csv_columns.push_back(name);
                        std::fprintf(csv, ",%s", name.c_str());
                    }
                std::fprintf(csv, "\n");
            }
            std::map<std::string, uint64_t> row(counters.begin(), counters.end());
            std::fprintf(csv, "%ld,%.3f", frame, ms);
            for (const auto &name : csv_columns) std::fprintf(csv, ",%llu", (unsigned long long)row[name]);
            std::fprintf(csv, "\n");
        }
        if (shots > 0 && frame % shots == 0) {
            uint32_t width = 0, height = 0;
            if (const uint8_t *pixels = machine->frame(width, height))
                write_ppm(work + "/frame-" + std::to_string(frame) + ".ppm", pixels, width, height);
        }
    }
    const double host = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
    if (csv) std::fclose(csv);
    if (hashes) std::fclose(hashes);
    if (sound_out) std::fclose(sound_out);

    const double guest_rate = double(frames) / guest;
    std::printf("%d MHz, %d MB, %ld frames after a %ld-frame boot and %ld warmup frames\n", config.cpu_mhz,
                config.ram_mb, frames, boot_frames, warmup);
    std::printf("boot: %.3f host s for %.3f guest s: %.2f frames/s\n", boot_host, boot_guest,
                double(boot_frames) / boot_host);
    std::printf("host %.3f s for %.3f guest s: %.2f frames/s against the guest's %.2f (%.1f%% of real time)\n",
                host, guest, double(frames) / host, guest_rate, 100.0 * guest / host);
    std::printf("frame ms: mean %.3f, median %.3f, p95 %.3f, max %.3f\n", 1000.0 * host / double(frames),
                percentile(frame_ms, 0.5), percentile(frame_ms, 0.95), *std::max_element(frame_ms.begin(), frame_ms.end()));
#ifndef _WIN32
    // /proc's schedstat: time on a CPU, then time runnable but waiting for one (Kairo68#26).
    if (const long tid = emulation_tid.load()) {
        const std::string path = "/proc/self/task/" + std::to_string(tid) + "/schedstat";
        FILE *stat = std::fopen(path.c_str(), "r");
        if (!stat) return fail("cannot read " + path);
        unsigned long long running = 0, waiting = 0;
        const int got = std::fscanf(stat, "%llu %llu", &running, &waiting);
        std::fclose(stat);
        if (got != 2) return fail("cannot parse " + path);
        std::printf("emulation thread since its start: %.3f s on a CPU, %.3f s waiting for one (%.1f%%)\n",
                    double(running) / 1e9, double(waiting) / 1e9, 100.0 * double(waiting) / double(running + waiting));
    }
#endif
    if (!totals.empty()) {
        std::printf("per frame:\n");
        std::vector<std::pair<std::string, uint64_t>> timers;
        for (const auto &[name, value] : totals) {
            if (name.rfind("pc.", 0) == 0) continue;
            if (name.rfind("timer.", 0) == 0) {
                if (name.size() > 3 && name.compare(name.size() - 3, 3, ".ns") == 0) timers.emplace_back(name, value);
                continue;
            }
            const double per_frame = double(value) / double(frames);
            if (name.size() > 3 && name.compare(name.size() - 3, 3, "_ns") == 0)
                std::printf("  %-16s %10.3f ms (%.1f%%)\n", name.c_str(), per_frame / 1e6, 100.0 * double(value) / 1e9 / host);
            else std::printf("  %-16s %12.1f\n", name.c_str(), per_frame);
        }
        std::sort(timers.begin(), timers.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
        std::printf("timers by host time:\n");
        for (const auto &[name, ns] : timers) {
            const std::string base = name.substr(6, name.size() - 9);
            std::printf("  %-52s %8.1f fires %9.3f ms\n", base.c_str(),
                        double(totals["timer." + base + ".fires"]) / double(frames), double(ns) / double(frames) / 1e6);
        }
    }
    if (top_pcs > 0) {
        // The addresses the guest spent most instructions at, with the code there.
        std::vector<std::pair<uint32_t, uint64_t>> pcs;
        for (const auto &[name, count] : totals)
            if (name.rfind("pc.", 0) == 0) pcs.emplace_back(uint32_t(std::stoul(name.substr(3), nullptr, 16)), count);
        std::sort(pcs.begin(), pcs.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
        if (pcs.size() > size_t(top_pcs)) pcs.resize(size_t(top_pcs));
        const double instructions = double(totals["instructions"]);
        std::printf("instructions by address (of %.0f a frame):\n", instructions / double(frames));
        for (const auto &[pc, count] : pcs) {
            std::printf("  %06x %10.1f a frame %5.2f%%  ", unsigned(pc), double(count) / double(frames),
                        100.0 * double(count) / instructions);
            for (const uint16_t word : machine->read_words(pc, 4)) std::printf(" %04x", word);
            std::printf("\n");
        }
    }
    return 0;
}
