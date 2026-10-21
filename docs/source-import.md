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
5. Every 3rdparty library that a selected file includes, plus the libraries the core always links (expat, zlib, lzma, zstd, utf8proc, flac, softfloat3, rapidjson, asio), is copied whole so its license files stay with it.
6. MAME's `COPYING`, `README.md` and `docs/legal/` are copied.

The import produced 5,928 files. `third_party/mame/KAIRO_IMPORT.txt` records the driver, the 3rdparty libraries, and the full `makedep.py` output. To repeat the import, run `tools/import_mame.py <extracted-mame-root> <new-destination> src/mame/sharp/x68k.cpp` and compare with the committed tree.

After copying, nested Git metadata (`.gitignore`, `.gitattributes`, `.gitmodules`, `.github/`) was removed so that every copied file is tracked. One symbolic link in zstd's CLI tests (`tests/cli-tests/bin/unzstd`) could not be created on Windows and is absent. The pre-generated 68000 decoder sources (`src/devices/cpu/m68000/m68000-*.cpp`) are part of MAME's tree and are kept as shipped.

There are no local changes to the MAME sources.
