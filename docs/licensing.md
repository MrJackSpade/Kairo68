# Licensing and credits

[← Kairo68](../README.md)

Kairo68 first-party code is [GPL-2.0-or-later](../LICENSE.md). The pinned [Kairo frontend](https://github.com/MrJackSpade/Kairo) is also GPL-2.0-or-later. Third-party components keep their own notices.

## MAME

The X68000 emulation comes from [MAME](https://www.mamedev.org/) 0.289. MAME as a whole is distributed under the GPL; individual files carry their own license in a `// license:` header. In the imported subset those headers are BSD-3-Clause, GPL-2.0+, LGPL-2.1+, MIT, or public domain. The import contains no GPL-2.0-only file. The X68000 driver files (`src/mame/sharp/x68k*`) are BSD-3-Clause. MAME's license texts are in `third_party/mame/COPYING` and `third_party/mame/docs/legal/`.

The bundled MAME 3rdparty libraries keep their own licenses:

| Library | License |
| --- | --- |
| aes256cbc | MIT |
| asio | Boost Software License 1.0 |
| asmjit | Zlib |
| expat | MIT |
| libjpeg (Independent JPEG Group) | IJG license: this software is based in part on the work of the Independent JPEG Group. Only the library is built. |
| flac | libFLAC is BSD-3-Clause (Xiph). The directory also holds the `flac`/`metaflac` programs (GPL) and documentation (GFDL), which are not built. |
| lzma (LZMA SDK) | Public domain |
| nanosvg | Zlib |
| pugixml | MIT |
| rapidjson | MIT (the JSON-licensed `bin/jsonchecker/` is not included) |
| softfloat3 | BSD-3-Clause |
| utf8proc | MIT, with Unicode data under the Unicode license |
| ymfm | BSD-3-Clause |
| zlib | Zlib |
| zstd | BSD-3-Clause (dual-licensed with GPL-2.0) |

FM sound uses ymfm through MAME. fmgen is not used.

## Shared frontend

The shared RetroArch Slang shader runtime uses librashader under its GPL-3.0-only option, so a shader-enabled Kairo68 build is distributed under GPLv3 terms using first-party "or later" permission. See [shared licensing](../shared/docs/licensing.md).

## Firmware and games

Kairo68 does not include X68000 ROMs, operating systems, or games. Users supply any firmware and games they need.
