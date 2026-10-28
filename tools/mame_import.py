"""What Kairo68's MAME subset is made of, for shared/tools/import_mame.py:

    python shared/tools/import_mame.py . <extracted MAME release>

Its changes to MAME are tools/patches.py (BASE "mame"), applied to the copy."""

DRIVERS = ['src/mame/sharp/x68k.cpp']

# The OSD's platform services (files, directories, timing, threads) for Android and the Windows host
# test; Kairo68 supplies its own video, sound and input in place of MAME's modules.
OSD_FILES = [f'src/osd/modules/file/{n}' for n in (
    'posixdir.cpp', 'posixfile.cpp', 'posixfile.h', 'posixptty.cpp', 'posixsocket.cpp',
    'windir.cpp', 'winfile.cpp', 'winfile.h', 'winptty.cpp', 'winsocket.cpp')] + [
    'src/osd/modules/lib/osdlib_unix.cpp', 'src/osd/modules/lib/osdlib_win32.cpp'] + [
    f'src/osd/windows/{n}' for n in ('winutil.cpp', 'winutil.h', 'winutf8.cpp', 'winutf8.h')] + [
    f'src/osd/interface/{n}' for n in ('audio.cpp', 'inputseq.cpp', 'nethandler.cpp')]

# Included through their own include directories, which the include closure does not search.
EXTRA_3RDPARTY = ['pugixml', 'libjpeg', 'nanosvg', 'aes256cbc']
