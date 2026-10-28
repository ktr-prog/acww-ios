#!/usr/bin/env python3
"""Compile one game unit for Apple arm64 (macOS, iOS) without trusting the callers and callees of other units to
extend narrow integers: tools/configure_host.py runs the game's compile commands through this on Apple targets.

    python3 tools/apple_abi_cc.py <the compiler command: ... -c <source> -o <object>>

Why: Apple's arm64 ABI has the caller extend char/short arguments to 32 bits and the callee extend char/short return
values, and each side relies on the other (clang marks them zeroext/signext and leaves the extension out). AAPCS64
(Linux) and GCC on x86-64 make no such promise, so the callee extends what it receives and the caller what it gets
back. The decompiled game has calls whose declaration doesn't match the definition (an int passed where the
definition takes a u8 or s16, a u8 return read through a declaration that says int, function pointers cast to other
types, the generated shared-call dispatchers that pass everything as int): on the other hosts the value is cut to
its type where it is received, as on the DS; on Apple it arrives uncut. That breaks the game in places (paths,
angles, coordinates).

What this does: the unit is compiled to LLVM IR (the front end only, no optimisation), the promises that the other
side relies on are taken out, and the IR is optimised and compiled as usual:
- a function's own parameters lose zeroext/signext (it extends them itself where it uses them),
- calls and declarations lose zeroext/signext on the value returned (the caller extends what it gets back).
What the unit gives stays as Apple's ABI wants it (arguments it passes and values it returns are still extended),
so calls between the game and Apple's libraries or the host runtime are unaffected. Variadic calls through fixed
declarations (and the reverse) can't be helped this way: those declarations are fixed in the sources.
"""
import os
import re
import subprocess
import sys

EXT = re.compile(r"\s(?:zeroext|signext)\b")
CALLEE = re.compile(r'([@%](?:"[^"]*"|[-\w.$]+))\(')
CALL_KW = re.compile(r"\b(call|invoke|callbr)\b")


def matching_paren(s, i):
    depth, j = 0, i
    while j < len(s):
        c = s[j]
        if c == '"':  # (quoted names may hold parentheses)
            j = s.find('"', j + 1)
            if j < 0:
                return -1
        elif c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                return j
        j += 1
    return -1


def fix_line(line):
    if line.startswith("define "):
        m = CALLEE.search(line)
        if not m or m.group(1).startswith("@llvm."):
            return line
        po = m.end() - 1
        pc = matching_paren(line, po)
        if pc < 0:
            return line
        return line[:po] + EXT.sub("", line[po:pc]) + line[pc:]
    if line.startswith("declare "):
        m = CALLEE.search(line)
        if not m or m.group(1).startswith("@llvm."):
            return line
        return EXT.sub("", line[:m.start()]) + line[m.start():]
    s = line.lstrip()
    if not (s.startswith("%") or s.startswith("call") or s.startswith("tail ") or s.startswith("musttail ")
            or s.startswith("notail ") or s.startswith("invoke")):
        return line
    k = CALL_KW.search(line)
    if not k:
        return line
    m = CALLEE.search(line, k.end())
    if not m or m.group(1).startswith("@llvm."):
        return line
    head = line[:k.end()] + EXT.sub("", line[k.end():m.start()])
    rest = line[m.start():]
    if re.search(r"\bmusttail\s+$", line[:k.start()]):
        # (a musttail call must match its caller's parameters, which have lost theirs)
        po = m.end() - 1 - m.start()
        pc = matching_paren(rest, po)
        if pc > 0:
            rest = rest[:po] + EXT.sub("", rest[po:pc]) + rest[pc:]
    return head + rest


def main():
    cmd = sys.argv[1:]
    if "-c" not in cmd or "-o" not in cmd:
        sys.exit("apple_abi_cc.py: needs a compile command with -c <source> -o <object>")
    out = cmd[cmd.index("-o") + 1]
    src = cmd[cmd.index("-c") + 1]
    ll = out + ".abi.ll"
    # 1. the front end only: the IR as the compiler would optimise it (the dependency file as usual)
    front = list(cmd)
    front[front.index("-c")] = "-S"
    front[front.index("-o") + 1] = ll
    front += ["-emit-llvm", "-Xclang", "-disable-llvm-passes"]
    r = subprocess.run(front)
    if r.returncode:
        sys.exit(r.returncode)
    # 2. the promises taken out
    with open(ll, encoding="utf-8", errors="surrogateescape") as f:
        lines = f.readlines()
    with open(ll, "w", encoding="utf-8", errors="surrogateescape") as f:
        f.writelines(fix_line(l) for l in lines)
    # 3. optimised and compiled as usual (the source's language and preprocessor flags dropped: the input is IR)
    back, skip = [], 0
    drop_with_value = {"-x", "-MF", "-include", "-I", "-D", "-U", "-isystem", "-idirafter"}
    for i, a in enumerate(cmd):
        if skip:
            skip -= 1
            continue
        if a in drop_with_value:
            skip = 1
            continue
        if a in ("-MMD", "-MD", "-nostdinc") or re.match(r"-(I|D|U)\S", a) or a.startswith("-std="):
            continue
        if a == src:
            back += ["-x", "ir", ll]
            continue
        back.append(a)
    back.append("-Wno-unused-command-line-argument")
    r = subprocess.run(back)
    if r.returncode:
        sys.exit(r.returncode)
    os.remove(ll)


if __name__ == "__main__":
    main()
