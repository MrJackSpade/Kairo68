# Source import policy

[← Kairo68](../README.md)

This repository is independent. Its only Git remote is `origin`, pointing to this project's repository. The first-party Kairo frontend is pinned at `shared/` as a submodule. Do not add an `upstream` remote, fork relationship, other submodule, subtree sync, or automated upstream merge. Third-party source is copied in as pinned snapshots under `third_party/`.

## MAME snapshot

`third_party/mame/` holds the part of MAME 0.289 that the X68000 driver needs.

| | |
| --- | --- |
| Release | `mame0289`, tag commit `f34f02505e32c1993c6a782b6814232cbfc74e36` (2026-07-29) |
| Archive | <https://codeload.github.com/mamedev/mame/tar.gz/refs/tags/mame0289> |
| Archive SHA-256 | `0929cc749afabcef892900e10dd90bd8b05f94a7dde8f367ac6a5d2082589f84` |
| Driver | `src/mame/sharp/x68k.cpp` |
| Imported | 2026-10-21 |

[`tools/import_mame.py`](../tools/import_mame.py) chose the files from the extracted archive:

1. MAME's own `scripts/build/makedep.py sourcesproject` (the logic behind `make SOURCES=...`) lists the driver's sources and the CPU, sound, machine, video, bus and format options it uses.
2. Those options are resolved to files through MAME's `scripts/src/*.lua` file lists, and the always-present shared floppy format code is added.
3. `src/emu`, `src/lib/util` and the top level of `src/osd` are copied whole.
4. The set is closed over `#include`: each included header is added, along with the source file next to any device, driver or format header, until nothing new is reached. Generated layout and font inputs are added from their source files.
5. Every 3rdparty library that a selected file includes, plus the libraries the core always links (expat, zlib, lzma, zstd, utf8proc, flac, softfloat3, rapidjson, asio, and pugixml, libjpeg, nanosvg and aes256cbc, which MAME includes through their own include directories), is copied whole so its license files stay with it.
6. MAME's `COPYING`, `README.md` and `docs/legal/` are copied.

The import produced 6,294 files: the 5,928 of the first import (2026-10-21) and, added 2026-10-22: the OSD platform services the core runs on (`src/osd/modules/file/` for POSIX and Windows files, directories, ptty and sockets, `src/osd/modules/lib/osdlib_unix.cpp` and `osdlib_win32.cpp`, the Windows helpers `src/osd/windows/winutil.*` and `winutf8.*` for the host test, and `src/osd/interface/audio.cpp`, `inputseq.cpp` and `nethandler.cpp`); the four include-directory libraries above; the devices MAME's device scripts list outside any option block (NVRAM, RAM, floppy drives, volume filters and the like at the top of `machine.lua`, `sound.lua` and `video.lua`); and the files of option blocks written as `opt_tool(CPUS, "...")` or guarded by a local set from one (the CPU disassemblers). Kairo68 supplies its own video, sound and input in place of MAME's OSD modules. `third_party/mame/KAIRO_IMPORT.txt` records the driver, the 3rdparty libraries, and the full `makedep.py` output. To repeat the import, run `tools/import_mame.py <extracted-mame-root> <new-destination> src/mame/sharp/x68k.cpp` and compare with the committed tree.

After copying, nested Git metadata (`.gitignore`, `.gitattributes`, `.gitmodules`, `.github/`) was removed so that every copied file is tracked. One symbolic link in zstd's CLI tests (`tests/cli-tests/bin/unzstd`) could not be created on Windows and is absent. The pre-generated 68000 decoder sources (`src/devices/cpu/m68000/m68000-*.cpp`) are part of MAME's tree and are kept as shipped.

## Local changes

`tools/import_mame.py` applies Kairo68's changes to the copied sources from `tools/mame_patches.py`; each replaces text that must occur exactly once, so an import against a MAME that moved the code stops instead of patching the wrong place.

MAME does not mark the x68000 driver as supporting save states. These changes make a loaded state restore the machine exactly; `core/test/boot_test.cpp` checks it by comparing every saved entry, frame by frame, for a second after a save and after a load.

- `x68k-save-state` (`src/mame/sharp/x68k.cpp`, `x68k.h`, `x68k_v.cpp`): the driver registered only its text and graphics VRAM and sprite registers. A loaded state kept the video controller, system port, I/O controller, ADPCM, PPI and FDC control state, SRAM, and the PCG and BG RAM from before the load, so the picture could stay black or show wrong patterns. The change registers that state and, after a load, reconnects the FDC to the selected drive, redecodes the PCG patterns and redraws both BG layers.
- `upd765-save-state` (`src/devices/machine/upd765.cpp`, `upd765.h`): the uPD765 family's live read/write state is not saved (MAME has those registrations commented out), nor is the data separator PLL inside it or the drive it reads. A read in progress at the save continued after the load from the state of before the load, lost its sector, and the X68000 saw End of Cylinder. The change saves both live states, their PLLs and drives.
- `floppy-save-state` (`src/devices/imagedev/floppy.cpp`): the drive saves its flux cache but not where the cache's weak zone starts or whether the track has unwritten changes.
- `rtc-save-state` (`src/emu/dirtc.h`, `dirtc.cpp`, `src/devices/machine/rp5c15.cpp`): `device_rtc_interface` counts the date and time in registers no state held, so after a load the clock ticked on from the time before it; the RP5C15 also left out its RAM.
- `hd63450-save-state` (`src/devices/machine/hd63450.cpp`): the DMA controller leaves out its latched bus-error code and its DTACK line.

- `osdlib-without-sdl` (`src/osd/modules/lib/osdlib_unix.cpp`): the POSIX OSD library reaches the clipboard through SDL, which only MAME's SDL OSD links. Kairo68 supplies its own OSD and has no SDL, so without `OSD_SDL` the file uses the clipboard stubs MAME already has for Android.

MAME's x68000 SASI hard disk controller was not usable for booting:

- `x68k-sasi-count` (`src/mame/sharp/x68k.cpp`, `x68k.h`): the IPL boots from SASI only when SRAM byte 0x5a counts the connected drives, which the owner sets with SWITCH.X; MAME's SRAM starts blank. The driver reports the attached drive in that byte, as it already reports the configured RAM size at 0x08.
- `x68k-sasi-dma` (`src/mame/sharp/x68k_hdc.cpp`, `x68k_hdc.h`, `x68k.cpp`): the IPL and Human68k move SASI data through DMA channel 1 on the controller's REQ line, which MAME never connected, so the first read never finished. The controller drives DRQ from REQ during its data phases, connected to the HD63450's DRQ1, and requests the first byte of a read or write after the average rotational latency of a 3600 rpm disk (1/120 s) rather than 450 ns: the HD63450 takes the request on its edge, and a request made before the IPL starts its DMA channel is never served.

Speed (Kairo68#5). Each change keeps the guest's behavior exactly: `tools/perf/verify-all.ps1` runs games before and after it with `core/test/game_profile` and every frame's picture and sound must match.

- `perf-counters` (`src/emu/schedule.cpp`, `sound.cpp`, `src/devices/cpu/m68000/m68000.cpp`, `src/mame/sharp/x68k_v.cpp`): a measurement build (`KAIRO68_PERF`, `core/kairo68_perf.h`) counts the 68000's instructions and timeslices, timer callbacks by timer, `screen_update` calls and lines, and their host time. Without `KAIRO68_PERF` the hooks are empty.
- `mfp-timer-elapse` (`src/devices/machine/mc68901.cpp`, `mc68901.h`): MAME's MC68901 fired a scheduler timer at every prescaled count, so a timer in delay mode cut the 68000's timeslices into a few hundred cycles. A delay-mode timer now fires only at the count where it elapses; a data register read, a control register write or a save computes the counter from the counts since then.
- `dma-drq-poll` (`src/devices/machine/hd63450.cpp`): an external-request channel's timer kept firing every bus clock (2 microseconds on the floppy channel) while DRQ was low, doing nothing but ending timeslices. It now stops at the first firing that finds DRQ low; DRQ's rising edge already restarts it.
- `x68k-deferred-partial` (`src/mame/sharp/x68k_crtc.cpp`, `x68k_crtc.h`, `x68k.cpp`, `x68k_v.cpp`): the CRTC drew each visible line at its hsync, about 460 `screen_update` calls a frame. The hsync now records the line the beam passed, and the lines waiting are drawn before anything the picture shows changes (video RAM, CRTC and video controller registers, palettes, sprites and BG, the system port, the raster copy), so each line is drawn from the state it had at its own hsync.

The core also builds MAME with `SOUND_DISABLE_THREADING`, as MAME's emscripten build does, and turns off MAME's output audio effects (high-pass filter and EQ by default) through its configuration callbacks; neither changes MAME's sources.

## Generated sources

MAME's build generates some sources from its tree; `tools/generate_mame.py <extracted-mame-root>` writes them to `core/generated/` from the same pinned archive: `version.cpp`, `drivlist.cpp` (MAME's `makedep.py driverlist`, filtered to `sharp/x68k.cpp`), the layouts the subset includes (`complay.py`), the `has_*.h` option headers (as `scripts/genie.lua`'s `generate_has_header`), the CPU sources `cpu.lua` builds with Python (`core/generated/emu/cpu/*/*.hxx`), the per-project source lists `sources.cmake`, and the UI fonts `uismall.fh` (from `scripts/font/NotoSans-Bold.bdc`, SIL Open Font License) and `ui/uicmd14.fh` (from `src/frontend/mame/ui/uicmd14.png`).
