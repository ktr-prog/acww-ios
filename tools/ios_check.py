#!/usr/bin/env python3
"""Check the iOS build's program (tools/configure_host.py --os ios) before it is packaged:

    python3 tools/ios_check.py build/hostios_usa/acww

- a 64-bit arm64 Mach-O executable for iOS (LC_BUILD_VERSION platform iOS), with an entry point;
- modern/host/early.c's early_init is the first static initialiser: it maps the DS memory, which the game's static
  constructors may touch, and Mach-O runs initialisers in the link's object order (configure_host.py puts early.o
  first). Read from __mod_init_func (plain pointers or chained fixups) or __init_offsets, whichever the linker wrote;
- the save states' ranges (modern/host/savestate.c), from the link map next to the program (acww.map, which
  configure_host.py has the linker write) and objects.txt: in each writable section, the variables of the objects
  from modern/link/game_begin.o to state_end.o (the game and the runtime's DS) lie between the section's two markers,
  and nobody else's (the frontend's, SDL's, the C library's) do. If not, a warning, and the file savestates-off next
  to the program: tools/ios_package.py puts it in the app, which then has no save states.
Exit status 0 if all is well. Standard library only.
"""
import os
import re
import struct
import sys

MH_MAGIC_64 = 0xFEEDFACF
CPU_TYPE_ARM64 = 0x0100000C
MH_EXECUTE = 2
LC_SEGMENT_64 = 0x19
LC_SYMTAB = 0x2
LC_BUILD_VERSION = 0x32
LC_MAIN = 0x80000028
PLATFORM_IOS = 2


GITHUB = os.environ.get("GITHUB_ACTIONS") == "true"


def note(msg):
    """a detail of a failure (on GitHub Actions also an annotation of the run: readable without the log)"""
    print(f"ios_check.py:   {msg}")
    if GITHUB:
        print(f"::warning title=ios_check detail::{msg}")


def fail(msg):
    print(f"ios_check.py: FAIL: {msg}")
    if GITHUB:
        print(f"::error title=ios_check::{msg}")
    sys.exit(1)


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    data = open(sys.argv[1], "rb").read()
    magic, cputype, _, filetype, ncmds, _, _, _ = struct.unpack_from("<IiiIIIII", data, 0)
    if magic != MH_MAGIC_64:
        fail("not a 64-bit Mach-O file (a fat file or another format?)")
    if cputype != CPU_TYPE_ARM64:
        fail(f"CPU type {cputype:#x}, not arm64")
    if filetype != MH_EXECUTE:
        fail("not an executable")
    off = 32
    sections, symtab, platform, minos, has_main, text_vmaddr = {}, None, None, None, False, None
    for _ in range(ncmds):
        cmd, size = struct.unpack_from("<II", data, off)
        if cmd == LC_SEGMENT_64:
            segname = data[off + 8:off + 24].rstrip(b"\0").decode()
            vmaddr, vmsize, fileoff, filesize, _, _, nsects, _ = struct.unpack_from("<QQQQiiII", data, off + 24)
            if segname == "__TEXT":
                text_vmaddr = vmaddr
            s = off + 72
            for _ in range(nsects):
                sectname = data[s:s + 16].rstrip(b"\0").decode()
                addr, ssize, soff = struct.unpack_from("<QQI", data, s + 32)
                sections[(segname, sectname)] = (addr, ssize, soff)
                s += 80
        elif cmd == LC_SYMTAB:
            symtab = struct.unpack_from("<IIII", data, off + 8)
        elif cmd == LC_BUILD_VERSION:
            platform, minos = struct.unpack_from("<II", data, off + 8)
        elif cmd == LC_MAIN:
            has_main = True
        off += size
    if platform != PLATFORM_IOS:
        fail(f"built for platform {platform}, not iOS (2)")
    print(f"ios_check.py: arm64 iOS executable, minimum iOS {minos >> 16}.{minos >> 8 & 255}")
    if not has_main:
        fail("no entry point (LC_MAIN)")
    if not symtab:
        fail("no symbol table")
    symoff, nsyms, stroff, _ = symtab
    early = None
    for i in range(nsyms):
        strx, ntype, nsect, _, value = struct.unpack_from("<IBBhQ", data, symoff + i * 16)
        if ntype & 0xE0:  # (debugging entries)
            continue
        end = data.index(b"\0", stroff + strx)
        if data[stroff + strx:end] == b"_early_init" and nsect:
            early = value
            break
    if early is None:
        fail("no _early_init symbol (modern/host/early.c not linked, or the program stripped)")
    base = text_vmaddr or 0
    if ("__TEXT", "__init_offsets") in sections:
        addr, ssize, soff = sections[("__TEXT", "__init_offsets")]
        first = base + struct.unpack_from("<I", data, soff)[0]
        where = "__init_offsets"
    else:
        sect = sections.get(("__DATA_CONST", "__mod_init_func")) or sections.get(("__DATA", "__mod_init_func"))
        if not sect:
            fail("no initialisers at all (no __mod_init_func or __init_offsets)")
        addr, ssize, soff = sect
        raw = struct.unpack_from("<Q", data, soff)[0]
        # a plain pointer, or a chained fixup's rebase (DYLD_CHAINED_PTR_64: the target's address,
        # DYLD_CHAINED_PTR_64_OFFSET: its offset from the image's start - in the low 36 bits either way)
        mask = (1 << 36) - 1
        cands = {raw, raw & mask, base + (raw & mask)}
        first = early if early in cands else raw & mask
        where = "__mod_init_func"
    if first != early:
        fail(f"the first initialiser ({where}) is at {first:#x}, not early_init ({early:#x}): the DS memory would be "
             "mapped after the game's static constructors run")
    print(f"ios_check.py: early_init ({early:#x}) is the first initialiser ({where})")
    # (the save states only: a wrong range turns them off in this build - a file the packaging puts in the app,
    # modern/host/ios/ios_glue.m - instead of stopping it)
    out = os.path.dirname(sys.argv[1])
    off = os.path.join(out, "savestates-off")
    if os.path.exists(off):
        os.remove(off)
    try:
        check_state(os.path.join(out, "acww.map"), os.path.join(out, "objects.txt"))
    except StateProblem as e:
        print(f"ios_check.py: WARNING: save states off in this build: {e}")
        if GITHUB:
            print(f"::warning title=ios_check::save states off in this build: {e}")
        with open(off, "w") as f:
            f.write(f"{e}\n")
    print("ios_check.py: all good")


class StateProblem(Exception):
    pass


def state_fail(msg):
    raise StateProblem(msg)


# the save states' markers: the first and the last variable of the state in the section each lands in
STATE_MARKERS = [("_gHostGameDataBegin", "_gHostStateDataEnd"), ("_gHostGameBssBegin", "_gHostStateBssEnd"),
                 ("_sGameStaticBssBegin", "_sStateStaticBssEnd")]


def check_state(map_path, objects_path):
    if not os.path.exists(map_path) or not os.path.exists(objects_path):
        state_fail(f"no link map ({map_path}) or object list ({objects_path}): the save states' ranges can't be checked")
    objs = [l.strip() for l in open(objects_path) if l.strip()]
    first = [i for i, o in enumerate(objs) if o.endswith("modern/link/game_begin.o")]
    last = [i for i, o in enumerate(objs) if o.endswith("modern/link/state_end.o")]
    if not first or not last or first[0] > last[0]:
        state_fail("objects.txt: no game_begin.o ... state_end.o run")
    # (paths compared resolved: the map may give them absolute, objects.txt gives them from the repository's root)
    state_objs = {os.path.realpath(o) for o in objs[first[0]:last[0] + 1]}
    files, sects, syms, mode = {}, [], [], None
    for line in open(map_path, errors="replace"):
        line = line.rstrip("\n")
        if line.startswith("# Object files:"):
            mode = "obj"
        elif line.startswith("# Sections:"):
            mode = "sec"
        elif line.startswith("# Symbols:"):
            mode = "sym"
        elif line.startswith("# Dead Stripped"):
            mode = None
        elif line.startswith("#") or not line.strip():
            continue
        elif mode == "obj":
            m = re.match(r"\[\s*(\d+)\]\s+(.*)$", line)
            if m:
                files[int(m.group(1))] = os.path.realpath(m.group(2))
        elif mode == "sec":
            p = line.split()
            if len(p) >= 4:
                sects.append((int(p[0], 16), int(p[1], 16), p[2], p[3]))
        elif mode == "sym":
            m = re.match(r"(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)\s+\[\s*(\d+)\]\s+(.*)$", line)
            if m:
                syms.append((int(m.group(1), 16), int(m.group(2), 16), int(m.group(3)), m.group(4)))
    if not files or not sects or not syms:
        with open(map_path, errors="replace") as m:
            head = [next(m, "").rstrip() for _ in range(12)]
        note(" | ".join(head))
        state_fail(f"{map_path}: not a link map in the expected format ({len(files)} objects, {len(sects)} sections, "
             f"{len(syms)} symbols)")

    def section_of(a):
        for addr, size, seg, sect in sects:
            if addr <= a < addr + max(size, 1):
                return seg, sect
        return None

    by_name = {n: a for a, _, _, n in syms}
    ranges = {}  # section -> (lo, hi)
    for lo_name, hi_name in STATE_MARKERS:
        if lo_name not in by_name or hi_name not in by_name:
            state_fail(f"{map_path}: no {lo_name} / {hi_name}")
        lo, hi = by_name[lo_name], by_name[hi_name]
        sl, sh = section_of(lo), section_of(hi)
        if sl != sh or not sl:
            state_fail(f"{lo_name} ({sl}) and {hi_name} ({sh}) aren't in one section")
        if not lo < hi:
            state_fail(f"{lo_name} {lo:#x} isn't below {hi_name} {hi:#x}")
        if sl in ranges:  # (two pairs in one section: the union, if they meet)
            plo, phi = ranges[sl]
            if hi < plo or lo > phi:
                state_fail(f"two separate state ranges in {sl}")
            lo, hi = min(lo, plo), max(hi, phi)
        ranges[sl] = (lo, hi)
    bad, counts = [], {}
    for a, size, idx, name in syms:
        sec = section_of(a)
        if not sec or sec[0] != "__DATA" or sec[1] in ("__la_symbol_ptr", "__got", "__thread_ptrs") or \
                sec[1].startswith("__objc_"):
            continue
        mine = files.get(idx) in state_objs
        if sec not in ranges:
            if mine:
                bad.append(f"{name} ({files.get(idx)}) in {sec[0]},{sec[1]}, which has no state range")
            continue
        lo, hi = ranges[sec]
        inside = lo <= a <= hi
        counts[sec] = counts.get(sec, 0) + mine
        if mine and not inside:
            bad.append(f"{name} ({files.get(idx)}) at {a:#x}, outside the state range of {sec[1]} ({lo:#x}-{hi:#x})")
        elif not mine and inside:
            bad.append(f"{name} ({files.get(idx)}) at {a:#x}, inside the state range of {sec[1]} ({lo:#x}-{hi:#x})")
    if bad:
        for b in bad[:30]:
            note(b)
        state_fail(f"{len(bad)} variables on the wrong side of the save states' ranges")
    for sec, (lo, hi) in sorted(ranges.items(), key=lambda kv: kv[1]):
        print(f"ios_check.py: save state range {sec[0]},{sec[1]} {lo:#x}-{hi:#x} ({(hi - lo) >> 10} KB, "
              f"{counts.get(sec, 0)} variables): only the game's and the runtime's DS's")


if __name__ == "__main__":
    main()
