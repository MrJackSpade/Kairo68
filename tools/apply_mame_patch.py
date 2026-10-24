"""Applies one group of tools/mame_patches.py to the imported tree in third_party/mame, as
tools/import_mame.py applies every group on an import. For adding a new group without re-importing:

    python tools/apply_mame_patch.py <group>

Each change must replace text that occurs exactly once; nothing is written unless every change in
the group applies."""
import sys
from pathlib import Path

import mame_patches

root = Path(__file__).resolve().parent.parent / 'third_party' / 'mame'
name = sys.argv[1]
if name not in mame_patches.PATCHES:
    raise SystemExit(f'no patch group {name}; groups: {", ".join(mame_patches.PATCHES)}')
texts = {}
for rel, old, new in mame_patches.PATCHES[name]:
    path = root / rel
    if path not in texts:
        with open(path, encoding='utf-8', newline='') as f:
            texts[path] = f.read()
    if texts[path].count(old) != 1:
        raise SystemExit(f'patch {name}: {rel} does not contain the text to replace exactly once')
    texts[path] = texts[path].replace(old, new)
for path, text in texts.items():
    with open(path, 'w', encoding='utf-8', newline='') as f:
        f.write(text)
print(f'{name}: {len(mame_patches.PATCHES[name])} changes in {len(texts)} files')
