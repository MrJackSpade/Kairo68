# Kairo68's ARM64 core profile

The Android build of the core (MAME and `core/`) is optimized with this profile of the device
playing games ([#6](https://github.com/MrJackSpade/Kairo68/issues/6)). It is on by default;
`-Ppgo=false` (Gradle) or `-DKAIRO_PGO=OFF` (CMake) builds the unprofiled core to compare against.
Host builds don't use it.

`arm64.proftext` holds LLVM function names, CFG hashes and execution counts. It contains no game
data. `profile.json` records the compiler that made it, the source revision it was trained on, the
text's SHA-256 and the raw profiles merged into it. `shared/cmake/KairoPgo.cmake` checks both,
restores the checkout root in function names (`@KAIRO_SOURCE@/`) and indexes the text with the
NDK's `llvm-profdata` at configure time.

## When the core changes

A function whose code no longer matches its profile is a build error
(`-Werror=profile-instr-out-of-date`), so the profile never silently stops applying. After a change
to the core, train the profile again on the device, from the committed tree:

```text
tools\perf\build-variant.ps1 -Name pgogen -Out F:\kairo68-perf -CMakeArgs '-DKAIRO_PGO=OFF','-DKAIRO68_PGO_GENERATE=ON'
tools\perf\pgo-train.ps1 -Device <adb address> -Tool F:\kairo68-perf\game_profile-pgogen.stripped -Games <games> -Out F:\kairo68-perf\pgo
```

`pgo-train.ps1` runs the instrumented measurement tool through the boot and 1,800 frames of each
training game, merges the runs with equal weight and writes this folder with
`shared/tools/pgo_freeze.py`. The text is sparse: functions that never ran are left out, and LLVM
optimizes them as it would without a profile. Measurement builds (`-DKAIRO68_PERF=ON`) and
experiments on the core use `-DKAIRO_PGO=OFF`.

## Training and evaluation

Training: Alien Syndrome at 25 and 10 MHz, Akumajou Dracula, Afterburner II and Arkanoid (25 MHz).
A-Jax, Aquales and R+R are held out, so the evaluation includes games the profile has never seen.

RG DS, `tools/perf/ab-run.ps1`, interleaved, the profile-use build against `-DKAIRO_PGO=OFF` from
the same source (frames a second, mean and range):

| Game | Without | With | |
| --- | --- | --- | --- |
| Alien Syndrome (trained, 4 rounds) | 58.50 (58.40-58.63) | 61.02 (60.83-61.39) | +4.3% |
| A-Jax (held out, 3 rounds) | 122.70 (120.47-123.86) | 134.73 (133.54-135.60) | +9.8% |
| Aquales (held out, 3 rounds) | 83.59 (83.29-83.76) | 90.21 (89.03-91.09) | +7.9% |
