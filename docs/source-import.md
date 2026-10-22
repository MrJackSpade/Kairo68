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

- `x68k-save-state` (`src/mame/sharp/x68k.cpp`, `x68k.h`, `x68k_v.cpp`): MAME does not mark the x68000 driver as supporting save states, and the driver registers only its text and graphics VRAM and sprite registers. A loaded state kept the video controller, system port, I/O controller, ADPCM, PPI and FDC control state, SRAM, and the PCG and BG RAM from before the load, so the picture could stay black or show wrong patterns. The change registers that state and, after a load, redecodes the PCG patterns and redraws both BG layers.

The core also builds MAME with `SOUND_DISABLE_THREADING`, as MAME's emscripten build does, and turns off MAME's output audio effects (high-pass filter and EQ by default) through its configuration callbacks; neither changes MAME's sources.

## Generated sources

MAME's build generates some sources from its tree; `tools/generate_mame.py <extracted-mame-root>` writes them to `core/generated/` from the same pinned archive: `version.cpp`, `drivlist.cpp` (MAME's `makedep.py driverlist`, filtered to `sharp/x68k.cpp`), the layouts the subset includes (`complay.py`), the `has_*.h` option headers (as `scripts/genie.lua`'s `generate_has_header`), the CPU sources `cpu.lua` builds with Python (`core/generated/emu/cpu/*/*.hxx`), the per-project source lists `sources.cmake`, and the UI fonts `uismall.fh` (from `scripts/font/NotoSans-Bold.bdc`, SIL Open Font License) and `ui/uicmd14.fh` (from `src/frontend/mame/ui/uicmd14.png`).
