#!/usr/bin/env python3
"""Generate the sources MAME's build makes for the X68000 subset into core/generated.

Usage: generate_mame.py <mame-source-root>

The root is the pinned MAME archive (docs/source-import.md) extracted whole. Outputs, as MAME's
makefile and scripts/src/*.lua make them:
  version.cpp            the build version strings
  drivlist.cpp           the driver list, filtered to sharp/x68k.cpp (makedep.py driverlist)
  <name>.lh              every layout the subset includes (complay.py)
  uismall.fh, ui/uicmd14.fh   the UI fonts (file2str.py, png2bdc.py)
  emu/cpu/.../*.hxx      the CPU cores' generated instruction and disassembler sources the subset
                         includes, from scripts/src/cpu.lua's build tasks
  sources.cmake          the C and C++ sources of each library, as MAME's projects list them
                         (scripts/src/*.lua), and the devices, formats and driver the import selected
"""
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SUBSET = ROOT / 'third_party/mame'
OUT = ROOT / 'core/generated'
VERSION = '0.289'
DRIVERS = ['sharp/x68k.cpp']
LAYOUT_DIRS = ['src/emu/layout', 'src/mame/layout']
LH_INCLUDE = re.compile(r'#\s*include\s*"([^"]+)\.lh"')
HXX_INCLUDE = re.compile(r'#\s*include\s*"([^"]+\.hxx)"')
# One cpu.lua build task: input, output, the scripts it uses, and the command after "$(1)".
BUILD_TASK = re.compile(
    r'\{\s*MAME_DIR\s*\.\.\s*"([^"]+)"\s*,\s*GEN_DIR\s*\.\.\s*"([^"]+)"\s*,\s*\{([^}]*)\}\s*,'
    r'\s*\{\s*"[^"]*"\s*,\s*PYTHON\s*\.\.\s*"([^"]*)"\s*\}\s*\}')
MAME_FILE = re.compile(r'MAME_DIR \.\. "([^"]+\.(?:c|cpp))"')
# The targets the core builds for, by MAME's targetos names (the Android app, and the host test),
# and the CMake test for each.
TARGETS = ('android', 'windows', 'linux')
CMAKE_TARGET = {'android': 'ANDROID', 'windows': 'WIN32', 'linux': 'CMAKE_SYSTEM_NAME STREQUAL "Linux"'}
# Library projects and the GENie scripts that list their sources.
PROJECTS = {'emu': 'emu.lua', 'utils': 'lib.lua', 'expat': '3rdparty.lua', 'zlib': '3rdparty.lua',
            'zstd': '3rdparty.lua', 'softfloat3': '3rdparty.lua', 'flac': '3rdparty.lua',
            'utf8proc': '3rdparty.lua', 'ymfm': '3rdparty.lua', 'jpeg': '3rdparty.lua',
            '7z': '3rdparty.lua'}

sys.path.insert(0, str(Path(__file__).resolve().parent))
import import_mame  # noqa: E402


def run(mame, *args):
    # Not isolated: png2bdc.py imports png.py from its own folder.
    subprocess.run([sys.executable, *args], cwd=mame, check=True)


def os_condition(condition, target):
    """A Lua `if` condition on _OPTIONS["targetos"] alone, evaluated for target; None for one that
    tests anything else."""
    expr = condition.replace('_OPTIONS["targetos"]', repr(target)).replace('~=', '!=')
    if '_OPTIONS' in expr or re.search(r'[A-Za-z_][A-Za-z_0-9]*\s*\(', expr):
        return None
    if re.sub(r"'[^']*'|\"[^\"]*\"|\b(?:and|or|not)\b|[()=!\s]", '', expr):
        return None
    return bool(eval(expr, {'__builtins__': {}}))  # noqa: S307 - only quoted strings and operators remain


def project_files(mame, project, script):
    """The sources a GENie project block names: those every target builds, and per target in
    TARGETS those its `if _OPTIONS["targetos"]...` blocks add. Blocks guarded by anything else
    are taken as built."""
    text = (mame / 'scripts/src' / script).read_text(encoding='utf-8')
    start = re.search(r'^\s*project\s*\(?\s*"' + re.escape(project) + r'"', text, re.M)
    if not start:
        raise SystemExit(f'no project {project} in {script}')
    following = re.search(r'^\s*project\s*\(?\s*"', text[start.end():], re.M)
    block = text[start.end():start.end() + following.start()] if following else text[start.end():]
    common, per_target = set(), {target: set() for target in TARGETS}
    # Each open if: for every target, whether the current branch is built, and whether an
    # earlier branch of the chain already was (or None for a non-OS condition).
    stack = []
    for line in block.splitlines():
        stripped = line.strip()
        branch = re.match(r'^(if|elseif)\s+(.*)\s+then$', stripped)
        if branch:
            built = {target: os_condition(branch.group(2), target) for target in TARGETS}
            if branch.group(1) == 'if':
                stack.append({'built': built, 'taken': dict(built)})
            else:
                frame = stack[-1]
                frame['built'] = {t: None if built[t] is None or frame['taken'][t] is None
                                  else built[t] and not frame['taken'][t] for t in TARGETS}
                frame['taken'] = {t: None if built[t] is None or frame['taken'][t] is None
                                  else built[t] or frame['taken'][t] for t in TARGETS}
            continue
        if stripped == 'else' and stack:
            frame = stack[-1]
            frame['built'] = {t: None if frame['taken'][t] is None else not frame['taken'][t] for t in TARGETS}
            continue
        if stripped == 'end' and stack:
            stack.pop()
            continue
        for name in MAME_FILE.findall(line):
            built = {t: all(f['built'][t] is not False for f in stack) for t in TARGETS}
            if all(built.values()):
                common.add(name)
            else:
                for target, yes in built.items():
                    if yes:
                        per_target[target].add(name)
    return sorted(common), {target: sorted(files) for target, files in per_target.items()}


def generate_cpu_sources(mame):
    """Runs every cpu.lua build task whose output a subset source includes."""
    wanted = set()
    for dirpath, _, files in os.walk(SUBSET / 'src'):
        for name in files:
            if name.endswith(('.cpp', '.h', '.ipp', '.hxx')):
                text = (Path(dirpath) / name).read_text(encoding='utf-8', errors='replace')
                for inc in HXX_INCLUDE.findall(text):
                    present = [Path(dirpath) / inc, SUBSET / 'src/devices' / inc, SUBSET / 'src/emu' / inc]
                    if not any(p.is_file() for p in present):
                        wanted.add(inc)
    text = (mame / 'scripts/src/cpu.lua').read_text(encoding='utf-8')
    made = set()
    for source, output, deps, command in BUILD_TASK.findall(text):
        target = output.removeprefix('emu/')
        if target not in wanted or target in made:
            continue
        scripts = re.findall(r'MAME_DIR\s*\.\.\s*"([^"]+)"', deps)
        args = command.split()
        if args[0] != '$(1)':
            raise SystemExit(f'unexpected build command for {output}: {command}')
        out = OUT / 'emu' / target
        out.parent.mkdir(parents=True, exist_ok=True)
        resolved = {'$(<)': str(mame / source), '$(@)': str(out)}
        resolved.update({f'$({i + 1})': str(mame / s) for i, s in enumerate(scripts)})
        run(mame, *[resolved.get(a, a) for a in args])
        made.add(target)
    missing = wanted - made
    if missing:
        raise SystemExit(f'no cpu.lua build task makes {sorted(missing)}')
    return made


def cmake_list(name, files):
    return f'set({name}\n' + ''.join(f'    "${{MAME_ROOT}}/{f}"\n' for f in files) + ')\n'


def write_sources(mame):
    parts = ['# Generated by tools/generate_mame.py from MAME 0.289; do not edit.\n']
    for project, script in PROJECTS.items():
        common, per_target = project_files(mame, project, script)
        name = f'MAME_{project.upper()}_SOURCES'
        files = [f for f in common if (SUBSET / f).is_file()]
        if not files:
            raise SystemExit(f'project {project} has no sources in the subset')
        parts.append(cmake_list(name, files))
        branches = []
        for target in TARGETS:
            extra = [f for f in per_target[target] if (SUBSET / f).is_file()]
            if extra:
                keyword = 'if' if not branches else 'elseif'
                branches.append(f'{keyword}({CMAKE_TARGET[target]})\n    list(APPEND {name}\n' +
                                ''.join(f'        "${{MAME_ROOT}}/{f}"\n' for f in extra) + '    )\n')
        if branches:
            parts.append(''.join(branches) + 'endif()\n')
    _, options, drivers = import_mame.run_makedep(str(mame), [f'src/mame/{d}' for d in DRIVERS])
    devices = drivers | import_mame.lua_option_files(str(mame), options) | import_mame.unconditional_formats(str(mame))
    devices = sorted(f for f in devices if f.endswith(('.c', '.cpp')) and (SUBSET / f).is_file())
    parts.append(cmake_list('MAME_DEVICE_SOURCES', devices))
    (OUT / 'sources.cmake').write_text('\n'.join(parts), encoding='utf-8')
    write_has_headers(options)


def write_has_headers(options):
    """has_cpus.h and the others, as genie.lua's generate_has_header writes them."""
    for kind in ('CPUS', 'SOUNDS', 'MACHINES', 'VIDEOS', 'BUSES', 'FORMATS'):
        names = sorted(name for k, name in options if k == kind)
        (OUT / f'has_{kind.lower()}.h').write_text(
            '// Generated file, edition is futile\n\n'
            f'#ifndef GENERATED_HAS_{kind}_H\n#define GENERATED_HAS_{kind}_H\n\n'
            + ''.join(f'#define HAS_{kind}_{name}\n' for name in names) + '\n#endif\n', encoding='utf-8')


def main():
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    mame = Path(sys.argv[1]).resolve()
    scripts = mame / 'scripts/build'
    (OUT / 'ui').mkdir(parents=True, exist_ok=True)

    (OUT / 'version.cpp').write_text(
        f'#define BARE_BUILD_VERSION "{VERSION}"\n'
        '#define BARE_VCS_REVISION "Kairo68"\n'
        'extern const char bare_build_version[];\n'
        'extern const char bare_vcs_revision[];\n'
        'extern const char build_version[];\n'
        'const char bare_build_version[] = BARE_BUILD_VERSION;\n'
        'const char bare_vcs_revision[] = BARE_VCS_REVISION;\n'
        'const char build_version[] = BARE_BUILD_VERSION " (" BARE_VCS_REVISION ")";\n', encoding='utf-8')

    # makedep reads the driver sources, so it runs against the subset with the full driver list.
    with tempfile.TemporaryDirectory() as temp:
        flt = Path(temp) / 'kairo68.flt'
        flt.write_text(''.join(f'{d}\n' for d in DRIVERS), encoding='utf-8')
        listing = subprocess.run(
            [sys.executable, '-I', str(scripts / 'makedep.py'), '-r', str(SUBSET), 'driverlist',
             str(mame / 'src/mame/mame.lst'), '-f', str(flt)],
            check=True, capture_output=True, text=True).stdout
    (OUT / 'drivlist.cpp').write_text(listing, encoding='utf-8')

    names = set()
    for dirpath, _, files in os.walk(SUBSET / 'src'):
        for name in files:
            if name.endswith(('.cpp', '.h', '.ipp')):
                text = (Path(dirpath) / name).read_text(encoding='utf-8', errors='replace')
                names.update(Path(n).name for n in LH_INCLUDE.findall(text))
    for name in sorted(names):
        source = next((SUBSET / d / f'{name}.lay' for d in LAYOUT_DIRS if (SUBSET / d / f'{name}.lay').is_file()), None)
        if source is None:
            raise SystemExit(f'no layout source for {name}.lh')
        run(mame, str(scripts / 'complay.py'), str(source), str(OUT / f'{name}.lh'), f'layout_{name}')

    run(mame, str(scripts / 'file2str.py'), str(mame / 'scripts/font/NotoSans-Bold.bdc'),
        str(OUT / 'uismall.fh'), 'font_uismall', 'uint8_t')
    with tempfile.TemporaryDirectory() as temp:
        bdc = Path(temp) / 'uicmd14.bdc'
        run(mame, str(scripts / 'png2bdc.py'), str(mame / 'src/frontend/mame/ui/uicmd14.png'), str(bdc))
        run(mame, str(scripts / 'file2str.py'), str(bdc), str(OUT / 'ui/uicmd14.fh'), 'font_uicmd14', 'uint8_t')
    cpu = generate_cpu_sources(mame)
    write_sources(mame)
    print(f'generated {len(cpu)} CPU sources, {len(names)} layouts, the driver list, version, fonts and sources.cmake in {OUT}')


if __name__ == '__main__':
    main()
