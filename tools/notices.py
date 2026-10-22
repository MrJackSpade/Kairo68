"""Writes kairo68/src/main/assets/THIRD_PARTY_NOTICES.txt from the license files in the MAME import:
MAME, and each 3rdparty library the core compiles in or includes. The shared frontend's own
notices (its Java libraries, font, NDK runtime, shader runtime and AndroidX) follow these in
About -> Licenses. Run it again after importing a new MAME."""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAME = ROOT / 'third_party/mame'
OUT = ROOT / 'kairo68/src/main/assets/THIRD_PARTY_NOTICES.txt'
RULE = '=' * 72

HEADER = '''Kairo68 third-party notices
These notices apply to the emulator code and libraries in this package. Kairo68 emulates the
X68000 with MAME 0.289; Kairo68 is not affiliated with MAMEdev.'''

MAME_TERMS = '''MAME is distributed as a whole under the GNU General Public License, version 2 or later. Its
individual source files carry their own licenses; the files in this package are under the
BSD-3-Clause, GPL-2.0-or-later, LGPL-2.1-or-later and MIT licenses, or are in the public domain.
Copyright (C) MAMEdev and contributors. The license texts follow.'''

# (title, source, license files relative to third_party/mame, preface)
SECTIONS = [
    ('MAME 0.289', 'https://www.mamedev.org/, third_party/mame',
     ['docs/legal/BSD-3-Clause', 'docs/legal/LGPL-2.1', 'docs/legal/MIT', 'docs/legal/GPL-2.0'], MAME_TERMS),
    ('ymfm', 'third_party/mame/3rdparty/ymfm', ['3rdparty/ymfm/LICENSE'], None),
    ('SoftFloat 3', 'third_party/mame/3rdparty/softfloat3', ['3rdparty/softfloat3/COPYING.txt'], None),
    ('zlib', 'third_party/mame/3rdparty/zlib', ['3rdparty/zlib/LICENSE'], None),
    ('Zstandard', 'third_party/mame/3rdparty/zstd', ['3rdparty/zstd/LICENSE'],
     'Zstandard is dual-licensed under the BSD license below and GPL-2.0; Kairo68 uses the BSD license.'),
    ('libFLAC', 'third_party/mame/3rdparty/flac', ['3rdparty/flac/COPYING.Xiph'], None),
    ('Expat', 'third_party/mame/3rdparty/expat', ['3rdparty/expat/COPYING'], None),
    ('utf8proc', 'third_party/mame/3rdparty/utf8proc', ['3rdparty/utf8proc/LICENSE.md'], None),
    ('LZMA SDK', 'third_party/mame/3rdparty/lzma', [], 'The LZMA SDK is placed in the public domain by Igor Pavlov.'),
    ('Independent JPEG Group libjpeg', 'third_party/mame/3rdparty/libjpeg', [],
     'This software is based in part on the work of the Independent JPEG Group.'),
    ('pugixml', 'third_party/mame/3rdparty/pugixml', ['3rdparty/pugixml/readme.txt'], None),
    ('NanoSVG', 'third_party/mame/3rdparty/nanosvg', ['3rdparty/nanosvg/LICENSE.txt'], None),
    ('AES-256-CBC', 'third_party/mame/3rdparty/aes256cbc', ['3rdparty/aes256cbc/LICENSE'], None),
    ('Asio', 'third_party/mame/3rdparty/asio', ['3rdparty/asio/LICENSE_1_0.txt'], None),
    ('RapidJSON', 'third_party/mame/3rdparty/rapidjson', ['3rdparty/rapidjson/license.txt'], None),
]


def section(title, source, files, preface):
    parts = [f'{RULE}\n{title}\nSource: {source}\n{RULE}']
    if preface:
        parts.append(preface)
    for name in files:
        text = (MAME / name).read_text(encoding='utf-8').replace('\r\n', '\n').strip('\n')
        # MAME's BSD-3-Clause and MIT texts are templates; MAME's files are MAMEdev's.
        if name.startswith('docs/legal/'):
            text = text.replace('Copyright <YEAR> <COPYRIGHT HOLDER>', 'Copyright (C) MAMEdev and contributors')
        parts.append(text)
    return '\n\n'.join(parts)


def main():
    text = HEADER + '\n\n' + '\n\n'.join(section(*s) for s in SECTIONS) + '\n'
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(text, encoding='utf-8', newline='\n')
    print(f'{OUT.relative_to(ROOT)}: {len(text.splitlines())} lines')


if __name__ == '__main__':
    main()
