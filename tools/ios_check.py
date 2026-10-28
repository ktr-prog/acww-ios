#!/usr/bin/env python3
"""Check the iOS build's program (tools/configure_host.py --os ios) before it is packaged:

    python3 tools/ios_check.py build/hostios_usa/acww

- a 64-bit arm64 Mach-O executable for iOS (LC_BUILD_VERSION platform iOS), with an entry point;
- modern/host/early.c's early_init is the first static initialiser: it maps the DS memory, which the game's static
  constructors may touch, and Mach-O runs initialisers in the link's object order (configure_host.py puts early.o
  first). Read from __mod_init_func (plain pointers or chained fixups) or __init_offsets, whichever the linker wrote.
Exit status 0 if all is well. Standard library only.
"""
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


def fail(msg):
    print(f"ios_check.py: FAIL: {msg}")
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
    print("ios_check.py: all good")


if __name__ == "__main__":
    main()
