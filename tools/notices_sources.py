"""Kairo68's third-party notices: the sources shared/tools/notices.py writes
kairo68/src/main/assets/THIRD_PARTY_NOTICES.txt from, the license files in the MAME import (MAME, and
each 3rdparty library the core compiles in or includes). Run it again after importing a new MAME."""
from notices import text

OUT = 'kairo68/src/main/assets/THIRD_PARTY_NOTICES.txt'

HEADER = '''Kairo68 third-party notices
These notices apply to the emulator code and libraries in this package. Kairo68 emulates the
X68000 with MAME 0.289; Kairo68 is not affiliated with MAMEdev.'''

MAME_TERMS = '''MAME is distributed as a whole under the GNU General Public License, version 2 or later. Its
individual source files carry their own licenses; the files in this package are under the
BSD-3-Clause, GPL-2.0-or-later, LGPL-2.1-or-later and MIT licenses, or are in the public domain.
Copyright (C) MAMEdev and contributors. The license texts follow.'''


def mame(name):
    # MAME's BSD-3-Clause and MIT texts are templates; MAME's files are MAMEdev's.
    return text(f'mame/{name}').replace('Copyright <YEAR> <COPYRIGHT HOLDER>', 'Copyright (C) MAMEdev and contributors')


# (title, source, [text]) in the order shown.
SECTIONS = [
    ('MAME 0.289', 'https://www.mamedev.org/, third_party/mame',
     [MAME_TERMS] + [mame(name) for name in ('docs/legal/BSD-3-Clause', 'docs/legal/LGPL-2.1', 'docs/legal/MIT',
                                             'docs/legal/GPL-2.0')]),
    ('ymfm', 'third_party/mame/3rdparty/ymfm', [mame('3rdparty/ymfm/LICENSE')]),
    ('SoftFloat 3', 'third_party/mame/3rdparty/softfloat3', [mame('3rdparty/softfloat3/COPYING.txt')]),
    ('zlib', 'third_party/mame/3rdparty/zlib', [mame('3rdparty/zlib/LICENSE')]),
    ('Zstandard', 'third_party/mame/3rdparty/zstd',
     ['Zstandard is dual-licensed under the BSD license below and GPL-2.0; Kairo68 uses the BSD license.',
      mame('3rdparty/zstd/LICENSE')]),
    ('libFLAC', 'third_party/mame/3rdparty/flac', [mame('3rdparty/flac/COPYING.Xiph')]),
    ('Expat', 'third_party/mame/3rdparty/expat', [mame('3rdparty/expat/COPYING')]),
    ('utf8proc', 'third_party/mame/3rdparty/utf8proc', [mame('3rdparty/utf8proc/LICENSE.md')]),
    ('LZMA SDK', 'third_party/mame/3rdparty/lzma', ['The LZMA SDK is placed in the public domain by Igor Pavlov.']),
    ('Independent JPEG Group libjpeg', 'third_party/mame/3rdparty/libjpeg',
     ['This software is based in part on the work of the Independent JPEG Group.']),
    ('pugixml', 'third_party/mame/3rdparty/pugixml', [mame('3rdparty/pugixml/readme.txt')]),
    ('NanoSVG', 'third_party/mame/3rdparty/nanosvg', [mame('3rdparty/nanosvg/LICENSE.txt')]),
    ('AES-256-CBC', 'third_party/mame/3rdparty/aes256cbc', [mame('3rdparty/aes256cbc/LICENSE')]),
    ('Asio', 'third_party/mame/3rdparty/asio', [mame('3rdparty/asio/LICENSE_1_0.txt')]),
    ('RapidJSON', 'third_party/mame/3rdparty/rapidjson', [mame('3rdparty/rapidjson/license.txt')]),
]
