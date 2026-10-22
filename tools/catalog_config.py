"""Kairo68's catalog settings: X68000 floppy and hard disk media, artwork references and the
platform's slug."""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "shared/tools"))
import content_ids
from launchbox_artwork import References

APP = "kairo68"
PLATFORMS = ("x68000",)
ARTWORK_CODE = 68
ARTWORK = References("Kairo68", (ARTWORK_CODE,))

FLOPPY = {".xdf", ".dim", ".d88", ".88d", ".hdm", ".2hd", ".dup"}
HARD_DISK = {".hdf", ".hds"}
LIMITS = {"fd": 64 * 1024**2, "hd": 4 * 1024**3}
KINDS = set(LIMITS)


def kind(name):
    suffix = Path(name).suffix.lower()
    return "fd" if suffix in FLOPPY else "hd" if suffix in HARD_DISK else None


MEDIA = content_ids.Media(kind, content_ids.same_file, LIMITS.__getitem__, 1, seven_zip=True)

MOBYGAMES = "x68000"
SCANS = {"x68000": (".downloads/scan/x68000.json", ("x68000",))}
