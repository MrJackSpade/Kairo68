// Measurement counters for performance work (Kairo68#28): what the guest did in each frame and
// where the MAME thread's time went. Only a measurement build (KAIRO68_PERF) keeps them; in every
// other build the macros below are empty and nothing is counted.
//
// Every counter is written on MAME's thread. The host reads them between frames, after the frame
// handshake, through X68kMachine::take_perf().

#pragma once

#ifdef KAIRO68_PERF

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace kairo68::perf {

// The X68000's address ranges whose accesses are counted (Config::count_accesses).
enum Region { kRam, kGraphicsVram, kTextVram, kIo, kSpriteRam, kOther, kRegions };

struct Counters {
    // The 68000's instructions, and the scheduler's runs of it (timeslices).
    uint64_t instructions = 0;
    uint64_t slices = 0;
    // Host time inside the CPU's runs, timer callbacks, x68k_state::screen_update, the sound
    // manager's update and Kairo68's render_frame. Timer callbacks include the partial screen
    // updates they start (the CRTC's hsync timer calls update_partial).
    uint64_t cpu_ns = 0;
    uint64_t timer_ns = 0;
    uint64_t screen_ns = 0;
    uint64_t sound_ns = 0;
    uint64_t render_ns = 0;
    // screen_update's stages: the layer under BG, the graphics pages, BG and sprites, the mix.
    uint64_t screen_under_ns = 0;
    uint64_t screen_gfx_ns = 0;
    uint64_t screen_pcg_ns = 0;
    uint64_t screen_mix_ns = 0;
    // Timer callbacks, and screen_update's calls and the lines they drew.
    uint64_t timer_fires = 0;
    uint64_t screen_updates = 0;
    uint64_t screen_lines = 0;
    // Each timer callback by its name (a static string, so its address keys it): fires and host time.
    std::unordered_map<const char *, std::pair<uint64_t, uint64_t>> timers;
    // Guest accesses by region, with Config::count_accesses.
    uint64_t reads[kRegions] = {};
    uint64_t writes[kRegions] = {};
    // The 68000's skipped loop runs (Config::idle_skip), and the cycles they would have taken.
    uint64_t idle_skips = 0;
    uint64_t idle_cycles = 0;
    // Instructions by their address, with Config::count_pcs.
    std::unordered_map<uint32_t, uint64_t> pcs;
};

// Whether instructions are counted by address (Config::count_pcs); kept apart from the counters,
// which take_perf clears every frame.
inline bool &count_pcs() {
    static bool enabled = false;
    return enabled;
}

// MAME's thread's counters.
inline Counters &counters() {
    static Counters instance;
    return instance;
}

inline uint64_t now_ns() {
    return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count());
}

// Adds the time from its construction to its end to a counter.
class Scope {
public:
    explicit Scope(uint64_t &into) : into_(into), start_(now_ns()) {}
    ~Scope() { into_ += now_ns() - start_; }
    Scope(const Scope &) = delete;
    Scope &operator=(const Scope &) = delete;

private:
    uint64_t &into_;
    uint64_t start_;
};

} // namespace kairo68::perf

#define KAIRO68_PERF_COUNT(field, amount) (kairo68::perf::counters().field += (amount))
#define KAIRO68_PERF_TIME(field) kairo68::perf::Scope kairo68_perf_scope_##field(kairo68::perf::counters().field)
#define KAIRO68_PERF_PC(pc) (kairo68::perf::count_pcs() ? void(++kairo68::perf::counters().pcs[(pc)]) : void())

#else

#define KAIRO68_PERF_COUNT(field, amount) ((void)0)
#define KAIRO68_PERF_TIME(field) ((void)0)
#define KAIRO68_PERF_PC(pc) ((void)0)

#endif
