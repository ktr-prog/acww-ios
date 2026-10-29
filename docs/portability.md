# ACWW host port: Linux/x86-only dependencies and the plan to remove them

This is an audit of branch `modern` at 8bcdf7f4 (2026-10-08). The goal is a native port for macOS arm64 and Windows x86-64, and later possibly the web. Every count below was measured; the method is given next to each number. File:line references are to the files as they are at that commit.

How the numbers were measured:
- **Literal scan.** A Python scan of `src/` with comments and strings stripped. Each hex literal in a DS address range is classified as a cast to a pointer, a comparison, or something else. Script: `port_audit/dsaddr.py`.
- **libclang sweep.** Every one of the 865 game units was parsed with libclang 21 (`port_audit/sweep.py`). Targets: x86_64-linux, i386-linux, arm64-apple-macos13, aarch64-linux, x86_64-w64-windows-gnu (with and without `-mno-ms-bitfields`) and x86_64-pc-windows-msvc. Each run used the build's game flags. The x86_64-linux run is the baseline, so the "extra" diagnostics of a target are the ones only that target has.
- **GCC flag experiment.** All 865 units were compiled with gcc in several flag variants, and the disassembly of each unit and then of each function was compared (`port_audit/flagexp.sh`).
- **Object inspection.** `readelf`, `nm` and `objdump` on the existing `build/host64_usa` objects.
- **Two headless runs of the x86-64 build:**
  - the oracle's `play` script to frame 25001;
  - the attract demo to frame 11043, stopped with SIGINT so that the `ACWW_IOHIST` histogram was written.

No worktree was needed. All compiles read the main checkout and wrote only to the scratchpad.

---

## 0. Summary of findings

| # | Dependency | Size (measured) | Blocks | Portable replacement |
|---|---|---|---|---|
| A | Game data and memory at fixed DS addresses (gdata 0x02001000, gbss 0x02040000, .dtcm 0x027e0000, main RAM memfd and its mirrors, ITCM, WRAM, palette, VRAM, OAM, I/O pages) | 7 fixed mappings in hw.c, 3 `--section-start` options, `-Ttext-segment`, `MAP_32BIT`, root heap at 0x50000000 | macOS (`__PAGEZERO` covers the low 4 GB), Windows (no fixed low mappings in practice), PIE everywhere | Ordinary data/bss and heap blocks; host arrays for the system work area, DTCM, palette, OAM and VRAM banks. **Done (3.2, 3.3):** nothing is mapped at a DS address; the game's variables are ordinary data/bss, the x86-64 build a PIE (i386, the frozen reference, stays a fixed-address executable at the default 0x08048000) |
| B | Pointers kept in 32-bit integers (works only below 4 GB) | C: 121 pointer-to-int + 96 int-to-pointer casts (55 files); C++: 186 int-to-pointer casts (67 files); explicit `(u32)(uptr)` 92 and `(T*)(uptr)` 31; PXI 26-bit pointers; snd7 38 conversions; DMA source/destination registers; resource offsets relocated in place | macOS arm64 and every build with memory above 4 GB | Real pointer types; offsets resolved when used; ARM7 services called directly. A low-4 GB poisoning harness finds the remaining sites on Linux. **Steps 3.0/3.1 done:** the harness (`ACWW_HIGHMEM`) runs every script; 0 C and 0 C++ int/pointer casts left (`--strict-c` clean); PXI, snd7 and DMA with real pointers |
| C | DS addresses used as numbers in the source | Casts of DS constants to pointers (I/O excluded): 229 (system work area/DTCM 160 in 56 files, VRAM 32, palette 18, OAM 5, main RAM 10, WRAM 2, GBA slot 2). Address range checks: about 25 in 8 files. VRAM address tables: 2. I/O register casts: 830 in 121 files (harmless: only used as numbers by `IO_*`) | A, B | Named host objects through macros (`HW_SYSWORK`, `HW_PLTT`, `HW_OAM`, `HostVram_CpuPtr`). **Done (3.2):** 0 casts of DS constants to pointers outside I/O |
| D | Signals and CPU tricks | SIGSEGV I/O emulation and single-stepping (x86 only), interrupt injection under SP (`-mno-red-zone`, irq_entry.S), SIGFPE division emulation, SIGVTALRM hang guard, SIGPROF, `ucontext` gregs | macOS (different ucontext, no x86), Windows (no signals), arm64 (no division trap) | Already almost unused: 0 trapped I/O accesses in 11043 attract-demo frames (`ACWW_IOHIST`); hang guard silent over 25001 play frames; 1 division-by-zero site (`spl_scl_in_out`). Replace with explicit code. **Done 2026-10-08 (phase 1): only debug aids remain, behind `HOST_DEBUG_POSIX`** |
| E | ELF / GNU-ld-only build steps | `ld -r -T` (2 partial links), 291 `--wrap` thunks in x86 asm, `--defsym`, `objcopy --localize-symbols` (76 names), provides.ld (7784 PROVIDE, of which 99 labels + 133 aliases are referenced), `.init_array` per-overlay ranges, `__start_/__stop_` symbols, gap data as ELF inline asm (113 objects), `.set` absolute symbols, `--allow-multiple-definition` | ld64 (Mac), lld-link / ld.bfd for PE (Windows) | Generated C instead of linker features; source-level fixes. **Done** (2.4-2.11, 0.2, 3.2, 3.3): the game links with a plain `g++ -pie -o acww *.o <libs>` (i386: `-no-pie` and the sysroot's options); only the guard variant (a GNU-ld debugging build) keeps its partial links |
| F | GCC-only leniency and flags | Clang (Apple's only compiler) rejects 47 units with 2370 errors (1735 distinct) that GCC accepts. **0 after step 2.1.** GCC-only flags: `-malign-data=abi` (**dropped, 2.8**), `-fpcc-struct-return` (unsupported by clang off i386; changes 39 functions in 17 units; dropped on x86-64, 2.2) | macOS | Fix the declarations (the PROTO_LOCAL backlog); drop the flags after testing them |
| G | LLP64 (Windows) | **4.4: the Windows build runs (llvm-mingw, Wine) with the oracle identical.** 831 errors (mostly `IO_ADDR_`'s `(unsigned long)` cast), 8 `__LP64__` checks, `unsigned long` pointer words in modern/ds/overlays.c. **0 beyond the baseline with `-mno-ms-bitfields` after step 2.3.** MS bitfield layout breaks the save `static_assert`s (696 + 54 + others); the MSVC ABI also breaks member-pointer casts (133) and the MWCC_LAYOUT tail-padding trick | Windows | One-line `IO_ADDR_` fix; MinGW-w64 (clang or gcc, Itanium C++ ABI) with `-mno-ms-bitfields`, **not MSVC**. With these, only 11 extra diagnostics remain |
| H | Display and frontend | melonDS (GPL-3.0, patched, 32-bit core for i386) behind 8 `melon_*` functions; a separate SDL2 viewer process through `shm_open` + `fork`/`execl` | Licence; process model on Windows | In-process SDL frontend; own renderer behind the same 8-function interface |
| I | Compiler assumptions | char signedness changes 21 functions in 11 units; `wchar_t` 72 uses / `L""` 11; `long` 196 uses (7 casts); `s64`/`u64` `aligned(4)` typedef; arm64 member-function-pointer (PMF) encoding differs from x86 (12 hand-encoded PMFs in gap data) | – | Keep `-fsigned-char` (gcc and clang support it on every target); `char16_t`; PMFs as typed C |

The good news:
- **macOS arm64 is ABI-compatible already.** Beyond the clang baseline, the libclang sweep for arm64-apple-macos13 (and for aarch64-linux) gives **0 extra diagnostics**. All `static_assert`s of the save layout pass.
- **The signal machinery is idle in normal play.** It can be deleted rather than ported.

---

## 1. Fixed addresses

### 1.1 What is mapped or linked where today

| Region | DS address | How the host provides it | Evidence |
|---|---|---|---|
| ~~Game initialised data `gdata`~~ | – | **3.3:** the program's ordinary `.data` (between `gHostGameDataBegin`/`End`, modern/link/) | tools/configure_host.py |
| ~~Game bss `gbss`~~ | – | **3.3:** the program's ordinary `.bss` (`gHostGameBssBegin`/`End`; x86-64: 0x11f6c8 bytes) | – |
| DTCM module variables `.dtcm` | 0x027e0000 (16 KB) | **3.2:** ordinary variables; DTCM is a 16 KB host block (`gHostDtcm`, include/host_mem.h) | was configure_host.py:160-167, :65 |
| Main RAM, 4 MB, mirrored to 0x02ffffff | 0x02000000 | `memfd_create` + 5 `MAP_FIXED` mappings; gdata/gbss copied into it via `__stop_gdata`, `__start_gbss`, `__stop_gbss`. **3.2:** one mapping, no mirrors; the arena past the data is a host block (`gHostMainRam`, DS 0x02200000-0x023fffff). **3.3:** not mapped (no memfd) | – |
| ITCM | 0x01ff8000 | **3.2:** a 32 KB host block (`gHostItcm`: only an arena on the host) | modern/host/mem.c |
| Shared WRAM | 0x03000000 | **3.2:** not mapped (unused); the empty WRAM arena (0x037f8000) is `gHostWramArena` | mem.c |
| Palette, OAM | 0x05000000, 0x07000000 | **3.2:** host arrays `gHostPltt`, `gHostOam` (`HW_PLTT`, `HW_OAM`) | mem.c |
| VRAM windows | 0x06000000-0x068a3fff | **3.2:** 9 banks and 5 CPU window arrays (`HostVram_CpuPtr`), kept in step on every VRAMCNT change and before the renderer reads the banks | modern/host/vram.c |
| I/O pages | 0x04000000, 0x04001000, 0x04100000 | memfd mapped twice (`PROT_NONE` for the game, read/write for the runtime) | hw.c:885-892, :902-904 |
| x86-64 program image | anywhere | **3.3:** a PIE (`-fPIE -pie`), loaded where the kernel puts it (ASLR) | tools/configure_host.py |
| x86-64 root heap | **3.3:** anywhere (an 8 MB anonymous mapping) | `host_root_heap_region` | modern/host/main.c |
| x86-64 thread stacks | **3.3:** anywhere (anonymous mappings, no `MAP_32BIT`) | `host_stack_alloc` | modern/host/os.c |
| Arena constants | 0x0229bdc0 etc. | `.set` absolute symbols `SDK_MAIN_ARENA_LO`, `SDK_SECTION_ARENA_*`, `OVERLAY_n_ID` | main.c:58-68 (68 references in 15 files) |
| ROM header / system work area writes | 0x027ffe00, 0x027ffc00, 0x027ffc40, 0x027fff8c, 0x027ffde8, 0x027fffaa | **3.2:** the host runtime writes `gHostSysWork`'s members (`HostSysWork`, include/host_mem.h) | main.c, pxi7.c, frontend.c, os.c |

On macOS arm64 none of these can be mapped: the 4 GB `__PAGEZERO` covers all of them, and binaries are always PIE. On Windows fixed low mappings are not reliable and there is no `memfd`, `MAP_FIXED` or `MAP_32BIT`. The plan therefore makes everything ordinary, relocatable memory, and DS addresses survive only as numbers where a hardware register needs one.

### 1.2 Code that still uses DS addresses as numbers

Measured with the literal scan (comments and strings stripped). "Cast" means a literal directly cast to a pointer.

| Kind | Count | Files | Examples | Replacement |
|---|---|---|---|---|
| System work area 0x027ff000-0x027fffff (and DTCM) casts | 160 (about 45 distinct addresses; 0x027fffa8 alone 27 times) | 56 | `*(volatile u16 *)0x27fffa8` (ov001/unk_ov001_02219064.cpp:39), RTC 0x027ffde8, ROM header 0x027ffe00 | `HostSysWork` struct (OSSystemWork) at a pointer; `HW_SYSWORK(T, 0x027fffa8)` macro, generated by a rewrite tool |
| DTCM casts | 2 | 2 | `(u32 *)0x027e3000` (autoload_2/unk_02114cd8.c:38), `(u32 *)0x027e0000` (autoload_2/unk_02114ef4.c:190); also os.c:196 (`HW_INTR_CHECK_BUF` 0x027e3ff8), sdk.c:29 (`OS_GetDTCMAddress`) | 16 KB aligned host array; `OS_GetDTCMAddress` returns it |
| VRAM casts and computed addresses | 32 casts, 23 of them in `GX_GetBG*Ptr` | 5 | autoload_2/unk_021104ac.c:85-222, unk_02111c0c.c:23,27, ov001/unk_ov001_0222516c.cpp:154 | `HostVram_CpuPtr(dsaddr)`, resolved through the current VRAMCNT bank mapping |
| VRAM address tables (address >> 12 as u16) | 2 tables | 2 | autoload_2/unk_0211220c.c:23-26, `data_02139f44` (autoload_2/unk_02111ccc.c:108) | Translate when used |
| Palette casts | 18 | 8 | `*(vu16 *)0x5000000 = 0x7c00` (main/unk_02000c2c.cpp:250), main/unk_020b8d9c.cpp:6296-6391 | `gHostPltt[0x800]` through `HW_PLTT` |
| OAM casts | 5 | 3 | autoload_2/unk_02111ccc.c:99,104 | `gHostOam[0x800]` through `HW_OAM` |
| Main RAM casts | 10 (6 `OSi_ReferSymbol` no-ops, 3 arena bounds) | 6 | autoload_2/unk_02114810.c:25-29 | Host arena blocks |
| WRAM, GBA slot casts | 2 + 2 | 2 | unk_02114810.c:31 (0x037f8000), autoload_2/unk_02113f04.c:31 (`*(u32*)0x08000000`, unmapped today) | Arena block; a host stub for the cartridge check |
| Address range checks | about 25 | 8 | WM/FS buffer checks in autoload_2/unk_02122e24.c:143-179 and unk_02123444.c:299-321; unk_02114cd8.c:32; main/unk_02000c2c.cpp:102-103; `OS_SetArenaHi(0, 0x23ff000)` (main/unk_020b0a80.cpp:235); CTRDG PXI offset `((uptr)&data_02200080 - 0x02000000) >> 5` (autoload_2/unk_0212703c.c:608) | Range checks against host block bounds, or delete (the DS-only DMA-ability rules) |
| I/O register casts | 830 | 121 | `IO_R(&R16(0x04001008))` | **None needed**: `IO_*` use the address only as a number (include/nitro/io_access.h) |
| Absolute linker symbols (`.set`) | 68 references | 15 | `OVERLAY_65_ID` as an address (main/unk_020720f8.cpp:1586), `SDK_SECTION_ARENA_DTCM_START` | Constants in a header |
| Gap data with DS numerals | **Resolved (2.7)**: 6 in the unused `BuildInfo` (not emitted), 13 packed bytes of the building table (no pointers); the I/O/VRAM ones in gap code (not emitted) | – | – | – |
| ROM-format data with DS pointers | Overlay table records (`sinit` pointers: autoload_2/unk_0211a49c.c:140-141); NFTR font unpacking adds the base to 5 fields in place (`u32 base = (u32)h`, autoload_2/unk_021031c4.c:69-97); sound archive `e->ptr = (u32)base + e->off` (autoload_2/unk_0210b448.c:365); snd7 treats a value below 0x02000000 as an offset (snd7/driver.c:122) | – | – | Keep offsets in the 32-bit file slots and resolve base + offset when used (or a 64-bit side table) |
| Overlay shared-address thunks | **Resolved (2.10)**: 163 typed C dispatchers (`Shared_<name>`, include/host_shared_calls.h), the rest direct calls; the 275 data references reviewed | – | – | – |
| provides.ld | **Resolved (2.11)**: deleted. The 126 referenced aliases are fixed in the sources, the 91 referenced labels are C pointers (include/host_labels.h) | – | – | – |

### 1.3 Pointers kept in 32 bits

These are the things that made "the game's memory anywhere" hard (the audit's counts; **steps 3.0 and 3.1 removed them, see
there**). They worked on x86-64 only because everything was kept below 4 GB.

- **C units.** gcc `-Wpointer-to-int-cast -Wint-to-pointer-cast`, x86-64: 246 warnings in 55 of 218 C files (121 pointer to int, 124 int to pointer). libclang: 121 + 128, of which 32 are DS register constants, which are fine. These are mostly NitroSDK/NNS C code in `autoload_2`, for example `u32 base = (u32)h;` in the font unpacker.
- **C++ units.** -fpermissive is already gone, so a bare cast is an error, but clang still flags 186 int-to-pointer casts from 32-bit integers (in 67 files). There are also 92 explicit `(u32)(uptr)` / `(s32)(uptr)` and 31 `(T *)(uptr)` conversions (grep).
- **Code relocations.** In `build/host64_usa/game.o`, `.text` has 8382 `R_X86_64_32` and 7189 `R_X86_64_32S` relocations (non-PIE absolute addressing). gaps.o has 42 `R_X86_64_32` data relocations: pointer slots of 4 bytes in gap data.
- **ARM7 interface (PXI).**
  - `ptr26()` rebuilds 26-bit pointers by OR-ing 0x08000000. That is wrong for the x86-64 image at 0x10000000; it is only harmless because the data concerned lives in gdata/gbss. (pxi7.c:72-75)
  - NVRAM pointers travel as two 16-bit halves (pxi7.c:89, :95, :103).
  - The card request block (pxi7.c:223) and the sound command lists (pxi7.c:367) go through `ptr26` as well.
  - `snd7` has 38 `uintptr_t`/`dsptr` conversions, sample addresses masked with `& 0x7fffffc` (snd7/channel.c:40, :101), and `readable()` limited to 0x02000000-0x03000000 (snd7/spu.c:49).
- **DMA.** DMA works on 32-bit register addresses dereferenced as host pointers (hw.c:339-368), and so does card DMA.
- **Host helpers that truncate.** `is_reg()` (sdk.c:35) truncates pointers to 32 bits before the 0x04000000-0x05000000 test. Above 4 GB a heap pointer whose low half happens to fall in that range would be taken for a register. It must compare the full `uintptr_t`.

---

## 2. Signals and CPU tricks

| Mechanism | Where | Used in normal play? (measured) | Platform problem | Replacement |
|---|---|---|---|---|
| SIGSEGV on the `PROT_NONE` I/O pages: x86 instruction decoding (`emulate_access`, i386 only), otherwise single-stepping with the trap flag (EFLAGS 0x100, `REG_ERR`, `REG_EFL`) | hw.c:525-685, SIGTRAP handler :687-708 | **No.** `ACWW_IOHIST`: 0 trapped accesses in 11043 attract-demo frames. Every access goes through `IO_*` → `HostIo_Read`/`HostIo_Write` (1374 uses in 113 files) | x86 encodings, glibc `gregs`, no Windows equivalent | Delete. The I/O pages become plain host arrays (`host_io()`) |
| Interrupt injection: push the interrupted PC below the game's stack pointer and redirect to `host_irq_stub` | hw.c:735-744, modern/host/irq_entry.S (x86 only, ELF `.type`/`.size`), `-mno-red-zone` (configure_host.py:59), `host_game_text_start/end` markers (configure_host.py:170-179) | Only called from the three signal handlers. Real delivery happens at safe points: `HostIo_*` (hw.c:569-597), `OS_Enable/RestoreInterrupts`, `OS_Halt`, `VBlankIntrWait` (os.c:19-24, :194-205) | arm64 has no red-zone convention to disable, no Windows equivalent | Delete together with the I/O trap and the hang guard |
| SIGFPE division by zero (x86 `div`/`idiv` given the DS result: quotient = dividend, remainder 0) | hw.c:814-852 | Rarely. One site in the attract demo (`spl_scl_in_out`, autoload_2/unk_020fc984.cpp:273, frame 8127); none in 25001 play frames | arm64 does not trap and gives a/0 = 0, a%0 = a, which differs from the DS | `DS_DIV`/`DS_MOD` inline helpers with the DS semantics (checked against `_s32_div_f`: src/autoload_2/unk_02133150.s:15-27, :142-147) at all **116** integer `/` and `%` sites with a non-constant divisor (libclang: 79 `/`, 35 `%`, 2 `/=` in 51 files; objdump agrees: 117 hardware div/idiv in game.o). Then make SIGFPE fatal |
| SIGVTALRM hang guard every 2 ms of CPU time (`setitimer(ITIMER_VIRTUAL)`) | hw.c:759-782, :919-928; `EINTR` loop at hw.c:115 | Never fired in 25001 play frames | No Windows equivalent; reads ucontext | A report-only watchdog (thread or per-frame counter); explicit `host_poll()` at any spin site it reports |
| SIGPROF sampler (`ACWW_PROFILE`), SIGINT dump, `ACWW_WATCH` (mprotect + trap flag), `ACWW_NOEMULATE` | hw.c:710-728, :784-808, :491-501, :647-655 | Debugging only | Linux/x86 only | Keep behind `#if HOST_DEBUG_POSIX`; elsewhere use the platform debugger |
| Fault reporting with frame-pointer walk and `execinfo` `backtrace` | hw.c:480-489, :643-666, :842-852 | Crash reporting | glibc | Optional per-platform crash handler |
| Stack switching for game threads | coro_switch.S (i386, x86-64 System V, x86-64 Windows, arm64 incl. Apple `_` prefix), coro.c | Yes | ~~Windows x64 missing~~ **written (4.3)**: saves rdi, rsi, xmm6-15 too and switches the TIB's stack bounds; tested on Linux through `ms_abi` (`coro-win64-test`); **runs (4.4)** the Windows build's threads under Wine | arm64: **runs (4.4)**, the aarch64 build under qemu, and `coro-a64-test` |
| `ucontext` | hw.c:21, :31-43, :541-553 | Only in the handlers above | glibc register names | Goes away with the handlers |
| ~~`-fno-pie`/`-no-pie`, `-Ttext-segment`, `--section-start`~~ | – | – | – | **Done (3.3)** on x86-64: `-fPIE -pie`; i386 keeps `-fno-pie`/`-no-pie` (default fixed address, no options) |

---

## 3. ELF / GNU toolchain features the build relies on

| Feature | Where | What for | Replacement |
|---|---|---|---|
| ~~`ld -r -T dtcm.ld`~~ | – | **Gone (3.2)**: DTCM is a host block (`gHostDtcm`), its variables ordinary variables | – |
| ~~`ld -r -T order.ld` with `-fdata-sections` and `-malign-data=abi`~~ | – | **Gone (2.8)**: the overruns fixed (guard variant, `configure_host.py --guards`), no DS order; `game.ld` gone too (3.3; only the guard variant links the game with `ld -r`) | – |
| ~~`.init_array` → `host_ovinit` ranges~~ | – | **Gone (2.9)**: each overlay unit lists its variables (`HOST_OVERLAY_VARS`), explicit `StaticInit_<unit>`; modern/gen/overlay_table.c | – |
| ~~`--wrap` (291) and x86 asm thunks~~ | – | **Gone (2.10)**: typed C dispatchers, call sites renamed | – |
| ~~`--defsym=_Znwj=_Znwm`, `_Znaj=_Znam`~~ | – | **Gone (2.5)**: the global operators are the host's (modern/host/newdelete.cpp), the game allocates by name | – |
| ~~`objcopy --localize-symbols`~~ (77/76 names) | – | **Gone (2.4)**: `include/host_libc_rename.h` (forced include) renames the game's libc names to `ds_*` | – |
| ~~`PROVIDE` (provides.ld, provides64.ld via host_relayout.py with pyelftools on DWARF)~~ | – | **Gone (2.11)**: the aliases fixed in the sources, the labels C pointers; the build reads no DWARF | – |
| ~~Gap data as inline asm~~ | – | **Gone (2.7)**: `modern/gen/gaps.c`, typed C for every architecture (14 objects); bss labels defined in the sources; PMFs the compiler's | – |
| ~~`__start_gdata` / `__stop_gdata` / `__start_gbss` / `__stop_gbss`~~ | – | **Gone (3.3)**: the debug dumps and `HostMem_IsGameRam` use the variables of modern/link/game_begin.c and game_end.c, linked first and last of the game's objects | – |
| ~~`.set` absolute symbols~~ | – | **Gone (2.6)**: `include/sdk_linker_symbols.h` | – |
| `constructor(101)` priorities | main.c:73, overlay.c:53 (and `constructor` overlay.c:43) | Map memory and snapshot overlays before the game's static constructors | Explicit init calls from `main()`. Unnecessary once no constructor touches fixed memory. Priorities aren't guaranteed across units on Mach-O/COFF |
| `--allow-multiple-definition` | configure_host.py:65 | It hides a **bug**: the 9 `src/dtcm` objects are linked twice (12 symbols). They go into dtcm.o inside game.o and also stay in `host_objs`, because `host_objs = objs - game_objs` is computed after `dtcm_objs` were removed (configure_host.py:160-169; `objects.txt` lists them) | Exclude `dtcm_objs` from `host_objs`, then drop the option |
| `.note.GNU-stack` sections | configure_host.py:176; irq_entry.S:66; coro_switch.S:177-179 (guarded) | ELF | Guard or remove |

---

## 4. melonDS and SDL: the graphics and sound interface

**What the game side uses.**
- Hardware registers through `HostIo_Read`/`HostIo_Write`. `forward_display` (hw.c:415-433) passes display registers on to the renderer:
  - the changed halves of the display range: < 0x04000070 except 0x04000004, engine B 0x04001000-0x0400106f, POWCNT 0x04000304, 0x04000320-0x040006a3;
  - every write to the 3D ports 0x04000400-0x04000600;
  - VRAMCNT 0x04000240-0x0400024b byte by byte.
- 3D read-backs (GXSTAT, matrix/vector test results and RAM counts, 0x04000600-0x040006a3, 0x04000320) come back synchronously through `melon_read32` (hw.c:405). The renderer must answer them during the frame.
- **The renderer interface (melon.cpp) is 8 functions:** `melon_init`, `melon_write8/16/32`, `melon_read32`, `melon_frame` (VRAM/palette/OAM sync plus `RunFrame`), `melon_screen(i)` (256×192 0x00RRGGBB), `melon_gx_fifo_irq_level` (IRQ 21 level). `melon_debug` exists for diagnostics only. **5.1:** the runtime now calls `render_*` (modern/host/render.h, the same 8 functions), which dispatch to melonDS and the own 2D renderer (modern/host/gpu2d).
- **Inputs to the renderer:**
  - 9 VRAM banks from vram.c (`host_vram_bank`, `host_vram_bank_size`), compared with `memcmp` per frame in `VRAMDirtyGranularity` blocks (melon.cpp:64-80);
  - palette 0x05000000 and OAM 0x07000000, copied each frame (melon.cpp:77-78).
- **Not emulated today:** HBlank per-line effects (`Sky_HBlankMain`/`Sub` are stubs, gaps_code.c:8-9); display capture works only as far as melonDS does it.
- **melonDS build.** Patched by tools/host/melonds.patch (192 lines: GPU3D `HostDrain`, `HostMarkPaletteOAMDirty`, CPU halt hooks). i386 links a 32-bit core (`build-core32`), x86-64 links `build-core` with JIT and GDB stub compiled in but unused (configure_host.py:73-75), aarch64 links `build-core-a64` (cross-built without JIT and GDB stub, 4.4). melonDS itself builds on macOS arm64 and Windows, so it can stay as the temporary renderer on those hosts. Its only blocker there is the GPL licence.
- **Frontend.**
  - ~~frontend.c (`shm_open`, `fork`, `execl` of the 64-bit SDL2 viewer) exists only because the i386 game process can't load the 64-bit system SDL.~~ **4.1:** the 64-bit builds have SDL2 in their own process (frontend_sdl.c); the viewer link (frontend_viewer.c) is built for i386 only.
  - The `AcwwShm` struct (frontend.h) carries keys, touch, 2 screens and a 16384-frame stereo audio ring at 32728 Hz.
  - The viewer uses 17 SDL2 calls (window, renderer, streaming texture, events, audio device).
- **Sound** is already native: snd7 (driver, sequencer, SPU mixer) writes to the ring and to WAV (snd7/output.c).

**Replacement:** an in-process SDL frontend (SDL2, or SDL3 later) for the 64-bit builds, keeping the `AcwwShm` struct as a plain in-memory interface. An own renderer then implements the 8 functions plus the VRAM, palette and OAM inputs. The frozen oracle captures (rendered by melonDS) are its reference frames.

---

## 5. Compiler assumptions

| Item | Measured | Mac arm64 | Windows | Action |
|---|---|---|---|---|
| GCC leniency: per-namespace `extern "C"` redeclarations with different types (the "views" trick: `namespace R1 { extern EffectManager gEffectManager; }`) | Clang errors in **47 units, 2370 errors** (1644 conflicting types, 536 redeclarations, 163 C-linkage conflicts, 20 redefinitions, 4 `void *` arithmetic, 2 ambiguous calls, 1 no matching call); g++ accepts them (notes only) | **Blocks Apple clang** | Blocks clang-mingw | Fix as part of the PROTO_LOCAL/views backlog. The libclang sweep is the check; the goal is 0 |
| `-fpcc-struct-return` | **Dropped on x86-64 (2.2)**, oracle identical; on i386 kept but a no-op (866 units: same code without it) | clash gone | – | – |
| ~~`-malign-data=abi`~~ | **Dropped (2.8)**, oracle identical | – | – | – |
| char signedness | `-funsigned-char` changes 21 functions in 11 units (`OS_VSNPrintf`, `StringExpander`, `DebugText_PutString`, GameSpy ov065) | Apple arm64 char is signed anyway; `-fsigned-char` works | MinGW char signed | Keep `-fsigned-char` (Linux arm64 needs it). Optionally make those 21 functions use `s8` |
| `-fshort-wchar` | `wchar_t` 72 uses in 4 files, `L""` 11 in 2 files | Supported by clang | wchar_t is 16-bit natively | Optional: `char16_t`/`u""`, then drop the flag |
| `-fno-short-enums`, `-fno-strict-aliasing`, `-ffreestanding -nostdinc -fno-builtin`, C++ flags | – | Supported | Supported (gcc/clang) | Keep |
| `long` | 196 uses, 7 casts; `IO_ADDR_` `(unsigned int)(unsigned long)(p)` (include/nitro/io_access.h:34) is the source of about 1530 of the 1537 LLP64 truncation diagnostics; `unsigned long` pointer words in modern/ds/overlays.c:44-54 (`Modern_SharedResolve`) | LP64: fine | **LLP64: breaks** | `(unsigned int)(uptr)(p)`; `uptr` in overlays.c; review the 7 casts |
| `__LP64__` used as "64-bit" | 8 size checks in ov103/unk_ov103_02294c40.cpp:237-260 | fine | wrong on LLP64 (7 failing checks) | `HOST64` |
| Bitfield and class layout | win64-gnu: the save `static_assert`s fail (Pattern 134, PlayerData 130, VillagerData ×58 each...) because of MS bitfield layout; with `-mno-ms-bitfields` they pass. MSVC ABI: still 696 VillagerData/VillagerMemory and 54 SaveData failures, plus 133 member-pointer size errors; `MWCC_LAYOUT` relies on Itanium `[[no_unique_address]]` tail reuse (12 uses in 7 files) | Passes | **Use MinGW-w64 (Itanium ABI) + `-mno-ms-bitfields`; MSVC/clang-cl is not viable** | – |
| `s64`/`u64` `aligned(4)` (include/types64.h:6-7), save data in DS layout | – | Unaligned 8-byte loads are fine on arm64 | MinGW clang/gcc: fine; MSVC can't lower alignment | – |
| Member-function-pointer encoding | 470 PMF types, 66 PMF casts in source (compiler-generated: fine); 12 hand-encoded in gap data | arm64 uses the ARM encoding | x86 encoding | Generate the gap PMFs from C (section 3) |
| Varargs | `DS_VA_ARG` is the compiler's `va_arg` (include/ds_stdarg.h): portable. `host_panic` reinterprets a `void *` as a `va_list` (main.c:34-38, src/main/unk_02000c2c.cpp:74-79): only valid where `va_list` is a pointer | Apple: pointer (works); Linux arm64: struct (4.4: prints the format only; `Fatal_PanicV` has no caller) | char* (works) | Debug path; pass a real `va_list` |
| Division by zero | See section 2. Also `sdk.c:199-200` returns 0 for b == 0, but the DS routine returns the dividend (9 named calls, all with constant divisors, so harmless) | arm64 differs from the DS | – | `DS_DIV`/`DS_MOD` |
| Mangled C++ names declared as C | **0 (2.5)** (was 47 in 17 files + 24 in proto headers) | – | – | Done: member functions, C functions in the defining unit |
| Endianness | Little-endian everywhere | – | – | Nothing |
| Platform macros in game code | `__x86_64__`/`__i386__`/`__linux__`: 0 in src/include (only `__LP64__` ×8) | – | – | Clean |
| GNU extensions in the runtime | ~~`typeof`, `localtime_r`, `clock_nanosleep`, `shm_open`/`fork` in the 64-bit builds, `mmap` of the ROM, `execinfo`~~ (4.2: plat.h); ~~`memfd_create`, `MAP_32BIT`~~ (gone, 3.3); left: `MAP_FIXED_NOREPLACE` (Linux only: the `ACWW_HIGHMEM` harness) | – | – | Platform layer (`modern/host/plat_posix.c`, `plat_win32.c`, `plat_common.c`) or SDL |

---

## 6. The plan, in order

Each step can be done and checked on this Linux x86-64 machine. "Oracle" means `oracle.sh <build> quick|play`. The x86-64 build must stay identical to its own pre-change captures, and the i386 build stays identical while it is kept as the reference.

Two cheap tools keep the work honest before any Mac or Windows hardware exists:
- **libclang target sweep.** This audit's `sweep.py`, made into `tools/host_portcheck.py`. It parses all 865 units for arm64-apple-macos, x86_64-w64-windows-gnu (`-mno-ms-bitfields`) and x86_64-linux in about a minute, and reports only the diagnostics beyond the baseline.
- **Low-4 GB poisoning harness.** Introduced in step 3.0. Since 3.3 it reserves the whole low 4 GB.

### Phase 0: guard rails (1-2 days)
1. **0.1** Add `tools/host_portcheck.py` (the sweep). Today it reports clang-only errors in 47 units, 2568 extra diagnostics on win64-gnu and 0 on macos-arm64. Track the counts.
   **Done** (`tools/host_portcheck.py`; usage in its docstring and docs/host.md). It parses the 865 game units plus modern/ds (867) for x86_64-linux (the baseline: what clang rejects) and reports per target only the diagnostics beyond it; default targets macos-arm64 and win64-gnu-nomsbf (also win64-gnu, win64-msvc, linux-arm64, linux32). Re-measured at 0a7496cd: baseline 1638 clang errors in 45 units, macos-arm64 0 beyond, win64-gnu-nomsbf 1550 beyond (838 errors, 712 warnings), win64-gnu 2602 beyond. After 2.1 and 2.3: baseline **0**, macos-arm64 **0**, win64-gnu-nomsbf **0**; win64-gnu (MS bitfields, not a target) 1055 errors, all save-layout `static_assert`s. Exit status 0 only when everything is 0, so it can gate CI.
2. **0.2** Fix the double-linked DTCM objects (configure_host.py:169), then drop `--allow-multiple-definition`. Check: link, oracle.
   **Done (2026-10-08).** Both builds link without the option (no other duplicate surfaced); oracle identical (i386 against the captures, x86-64 against its own pre-change run).
3. **0.3** Add `-Werror=pointer-to-int-cast -Werror=int-to-pointer-cast` to the C units of the x86-64 build behind a `--strict-c` switch. It lists the 246 C sites; burn them down in phase 3.
   **Done (2026-10-08):** `configure_host.py --arch x86_64 --strict-c --suffix _strict` (the switch also drops `-w` for the C units, which would hide the casts). Count on 2026-10-08 (after the pointer-width work already on `modern`): **245 diagnostics, 226 distinct sites (111 pointer to int, 115 int to pointer) in 54 files** (53 sources and `include/ds_stdarg.h`), no other errors. Count: `ninja -f build_host64_strict.ninja -k 0 <the C objects>`, then the distinct `file:line:col` of `error: cast`.

### Phase 1: remove the signal-based machinery (Linux, about 1 week)
4. **1.1** Make the I/O page fault fatal behind `ACWW_STRICTIO` and run the play script plus the save/continue script. Expected: no fault (measured 0 in the attract demo). Then delete `emulate_access`, the SIGTRAP single-step path, `map_io`'s `PROT_NONE` view and the memfds. `host_io()` returns plain arrays (`static u8 sIo0[0x1000]...`). Check: oracle identical.
5. **1.2** Hang guard: switch it to report-only first (no injection; run the long scripts; it never fired), then remove the SIGVTALRM timer. If a spin site is ever reported, put an explicit `host_poll()` (advance time and deliver interrupts) in that loop. Delete `host_irq_inject`, irq_entry.S, the `game_text` markers and `-mno-red-zone`. Check: oracle identical (no injection happened in these runs, so the code shape changes but the behaviour doesn't).
6. **1.3** Division: write `tools/host_div_rewrite.py` (libclang) to turn the 116 non-constant-divisor `/`, `%`, `/=` into `DS_DIV`/`DS_MOD`/`DS_UDIV`/`DS_UMOD` (DS semantics: a/0 = a, a%0 = 0). Fix `_s32_div_f`/`_u32_div_f` in sdk.c to match. Then make SIGFPE fatal. Check: oracle identical (`spl_scl_in_out` still gets the dividend); `objdump` shows no `idiv` without a zero test in game code.
7. **1.4** Move the debug-only signal users (SIGINT dump, SIGPROF, `ACWW_WATCH`, `ACWW_NOEMULATE`) behind `HOST_DEBUG_POSIX`. Make the pacing sleep a platform function (`clock_nanosleep` here; `nanosleep`/`mach_wait_until` on macOS; a waitable timer on Windows).

After phase 1 the runtime has no ucontext, no x86 decoding and no `-mno-red-zone`, so interrupts are taken only at the explicit safe points.

**Phase 1 done (2026-10-08).** What was done and measured:
- **1.1** Trial first: with `ACWW_STRICTIO=1` a trapped I/O access (and a hang-guard action) ended the run. The oracle's quick and play scripts on both builds, and the new-game save script plus the continue run (docs/host.md, i386, to frame 36501 and 6501), ran through: **0 trapped accesses, 0 hang-guard actions**. Then deleted: `emulate_access`, the SIGSEGV/SIGTRAP handlers, the I/O memfds and their `PROT_NONE` views, `ACWW_NOEMULATE`. The registers are three static arrays in hw.c. Nothing is mapped at 0x04000000 any more, so a stray access is a crash.
- **1.2** Deleted: the SIGVTALRM timer and handler, `host_irq_inject`, `host_async_tick`, `host_irq_entry`, irq_entry.S, the `host_game_text_start/end` markers, `-mno-red-zone`. No explicit safe point was needed (the guard never acted). A watchdog thread (portable: `plat.h`) reports when virtual time stands still for 2 s of real time (`ACWW_WATCHDOG=<ms>`, 0 = off); it never touches the game's thread. It reported nothing in any run.
- **1.3** `tools/host_div_rewrite.py` (libclang; types as on the DS, i.e. parsed for i386) rewrote **116 sites in 51 files** (DS_DIV 64, DS_MOD 17, DS_UDIV 10, DS_UMOD 16, DS_UDIV64 7, DS_UMOD64 2; none needed a manual fix, none in headers or modern/ds); `--check` now reports 0. The helpers (`include/ds_div.h`) follow the ARM runtime: 32-bit a / 0 = a, a % 0 = 0, INT_MIN / -1 = INT_MIN; 64-bit (`_ll_sdiv`/`_ll_mod`...) a / 0 = a **and a % 0 = a** (hw.c's old emulation gave 0 there). `_s32_div_f`/`_u32_div_f` in sdk.c give a / 0 = a. SIGFPE is fatal (crash report). objdump of the x86-64 game.o: 113 hardware `div`/`idiv` (was 117); 93 have the zero test right before them, the other 20 sampled are tested earlier or proven non-zero by the compiler (e.g. `keyrand` returns early for n == 0). The one known division by zero (`spl_scl_in_out`) gets the dividend as before: oracle identical.
- **1.4** `modern/host/debug_posix.c` (`HOST_DEBUG_POSIX`, on by default except on Windows): crash report (SIGSEGV/SIGBUS/SIGILL/SIGFPE), the SIGINT report, `ACWW_PROFILE`, `ACWW_WATCH` (x86 Linux only; ucontext only there). The pacing sleep and the clock are `host_plat_sleep_until_ns` / `host_plat_time_ns` (`plat_posix.c`: `clock_nanosleep` on Linux, `nanosleep` on macOS; `plat_win32.c`: a high-resolution waitable timer, not built yet). hw.c has no signal, ucontext or x86-specific code left.
- Checks: oracle quick and play identical on i386 (against the captures) and on x86-64 (against the same tree's pre-change run, bit for bit); the save/continue script's screenshots identical to the pre-change i386 build. The save file written by that script differed in 24 bytes, the Wi-Fi ID: a build dependence found on the way and fixed (section 7).

### Phase 2: toolchain neutrality (Linux, 2-3 weeks; overlaps phase 3)
8. **2.1** Clang-clean the game. Fix the 47 units / 1735 distinct clang errors (namespace views → real types; this is the existing PROTO_LOCAL backlog). The portcheck reaching 0 is the check. Then get an LLVM toolchain into `~/acww/toolchains` (prebuilt tarball, no sudo needed) and add `--cc clang` to configure_host.py. Check: the clang x86-64 build passes the oracle. This catches most Apple-clang problems on Linux.
   **Source part done**: portcheck baseline 1638 errors / 45 units → **0** (each unit's own per-namespace declarations replaced by the definitions' types, call sites and data tables retyped; per-object code comparison against the previous tree, i386 oracle quick and play identical; x86-64 play and quick sound identical to the same tree's pre-change run, the two quick screenshots differ because of the `Insect_TickFrame` fix below - with only that fix undone the x86-64 run is identical bit for bit). Real bugs found on the way (mostly 64-bit only: DS offsets into `Insect` (its frame counter: the attract demo's insects animate differently on x86-64 now), `BlendAnimModel`, `Actor::position`, `MapBlock` stride, `FtrBed` rows, a pointer's upper half incremented in `gpiProfileListAdd`; Wi-Fi tables reached through PROVIDE labels past their object; lost arguments in the server browser) are fixed in separate commits. `configure_host.py --cc clang` exists (separate build `build_host64_clang.ninja`, `build/host64_clang_usa`; it drops `-malign-data=abi` and, on x86-64, `-fpcc-struct-return`, and uses `-fstandalone-debug`). **Clang build done (2026-10-08)**: clang 23.1.3 (llvm-mingw 20261006, a full clang that also targets Linux and uses the system's GCC 15 headers and libraries for the host runtime) in `~/acww/toolchains/llvm-mingw-*/bin`, found by `configure_host.py --cc clang` (or `--cc <path>`; the compiler is given `--target=x86_64-linux-gnu`). The game units, modern/ds, the generated C and the host runtime's C are compiled by clang; melonDS and the runtime's C++ glue stay g++-built, the link is g++. What clang rejected that the libclang sweep did not report: 2 units, 10 errors - 8 narrowing conversions in braced initialisers (src/main/unk_020571f4.cpp, `u8 x[4] = { a[1] - a[0], ... }` of `const u8` tables: int to u8, an error in clang even with `-w`; now `(u8)(...)`, the same values) that the sweep missed because `-Wno-everything` also silences clang's default-error warnings (the portcheck now turns them back on: `-Werror=c++11-narrowing`, `int-conversion`, `incompatible-function-pointer-types`, `implicit-function-declaration`, `implicit-int`; still 0 everywhere), and 2 redeclarations of `ModelSlotHandle_Init`/`_Destroy` with `void *` in ov003 (unused, came in after the last sweep; removed). The link failed on 9 complete-object destructors (`D1`) that modern/gen/shared_table.c names as shared-address candidates: GCC emits D1 with D2 (an alias, the same address) for every inline destructor, clang only the variants a unit uses. tools/host_shared_calls.py now names the base-object variant (C2/D2; the same code, the game has no virtual bases): 15 candidates renamed, the GCC binaries' addresses unchanged. Checks: `tools/host_oracle.sh build/host64_clang_usa/acww quick` and `play` identical to captures_v2 (both screenshots and the sound; all 7 play frames), and so are the GCC i386 and x86-64 builds'. The town, save and continue scripts: the clang build under `ACWW_HIGHMEM=1` and the GCC x86-64 build give the same screenshots, save files and `ACWW_IOTRACE` register traffic, every frame - after two fixes. Before them the pictures and save files were already the same but the register traffic was not, from frame 3425 of the continue script and 24245 of the save script (one sound fewer, one sequence started more). Both were real bugs that the GCC build got away with:
   - **The player action requests' payload read at the DS offset**, in 15 more handlers (the rest of the bc50ae1f fix): on 64-bit hosts `PlayerActionRequest`'s payload union is at 0x10, not 0xc (it holds a pointer), and `PlayerActor_SetupLieInBed` read `p + 0xc`, the padding. With GCC that byte happened to be 0; with clang it was 0x55, so waking up in bed skipped the get-out-of-bed sound (`PlayerActor_EndLieInBed` wants `actionWorkRaw[0] == 0`). The same mistake: `PlayerActor_SetupGetOutOfBed` (side byte `q[1]`), `PlayerActor::setupAct77`, `setupErrorMessage` (main) and 11 ov003 handlers (`ReleaseCreature`, `Act11`, `PluckReach`, `FishReelIn`, `FishShowCatch`, `FishStore`, `BugNetSwing`, `Act66`, `Act67`, `TreeShake`, `PitfallFall`); all now `unk_0c_b` / `unk_0c_h`. Found with `ACWW_IOTRACE_FRAMES` (the first differing access: a sqrt for a sound's distance), then gdb at that cycle in both builds.
   - **`Bgm_GetSceneBgmId` (src/main/unk_02034438.cpp) ended its table search at `data_020c8b9c`**, the object after `sSceneBgmTable` on the DS (an unrelated constant in another unit): on the host the loop's end is wherever the linker puts that object. With clang it lay past the table, and going into the house matched a garbage "scene" and requested BGM 13339 (`BgmTempoTracker` then restarted sequence 248). It ends at the table's own end now. (Other loops ending at another object's address may remain: none found by grep, but no tool checks for them.)
   Also fixed, from clang's warnings over the game (`-Wunsequenced -Wreturn-type -Wuninitialized ...`, not part of the build): `NNS_G3dGlbFlushP`'s `NNS_G3dGeBufferOP_N(*p++, p, ...)` (unsequenced; both compilers happened to pass p + 1, as the ROM does), `__wStringWrite` without a return (the ROM returns `wmemcpy`'s result, which both compilers happened to leave in the return register) and an empty `gti2HandlePing` (in the ROM a branch to `gti2SendPong`; in C++ falling off the end is undefined). Left: 23 `-W(sometimes-)uninitialized` warnings (bitfield words built from uninitialised variables in the Wi-Fi sync code, switch defaults the original also leaves unset), all on paths the test scripts don't reach. Counts: clang errors 10 in 2 units -> 0; link errors 9 -> 0; register-traffic differences from the GCC build in the town/save/continue scripts 2 -> 0.
   **Sanitizers and initialisation with clang** (after the merge of the sanitizer sweep): `configure_host.py --arch x86_64 --cc clang --sanitize address,undefined --suffix _clang_asan` builds (llvm-mingw has no Linux compiler-rt, so the g++ link uses GCC's libasan/libubsan; clang 23's instrumentation works with them). Quick and play: pictures and sound identical to captures_v2, **0 ASan reports**; clang's UBSan adds checks GCC lacks: 61 calls through function pointers of another type (`-fsanitize=function`: SDK callbacks typed `void (*)(void *)` called with a struct pointer's type, the 11 shared-address dispatchers of modern/gen/shared_calls.c by design, the "views" casts; the arguments' ABI classes match, so x86-64, arm64 and wasm call them correctly), 11 loads of enum values outside the enumerators (src/main/unk_020891bc.cpp, unk_0208d9d4.cpp, src/autoload_2/unk_020fa39c.cpp: DS enums used as plain integers) and 2 null-pointer `offsetof` idioms (`G3D_ANMOBJ_SIZE`, now `__builtin_offsetof`); the two `Str_SPrintf` calls through a variadic cast are direct calls now. Not fixed: the other function-pointer casts and the enums (a typing clean-up, not behaviour). Builds with `--extra-cflags=-ftrivial-auto-var-init=pattern` and `=zero` (every local variable of the game filled with 0xAA... or 0 before use): quick and play identical to captures_v2 and the town, save and continue scripts identical to the GCC x86-64 build (screenshots, save files, register traffic) in both - no uninitialised local is read on these paths any more.
9. **2.2** Drop `-fpcc-struct-return` on x86-64 (39 functions / 17 units change); oracle. `-malign-data=abi` is dropped in step 2.8.
   **Done (2026-10-08)** for x86-64: the 64-bit build's quick and play captures (screens and sound) are identical to the
   build's own captures from before. The 32-bit build keeps the flag while it is the reference, but there it changes
   nothing: all 866 units compile to the same code with and without it (`objdump -d` of each), since returning structs
   in memory is the i386 Linux ABI's default. Dropping it on i386 too is safe; it is kept only so that the reference
   build's command line is unchanged.
10. **2.3** LLP64 readiness:
    - `IO_ADDR_` → `(unsigned int)(uptr)(p)`;
    - overlays.c `unsigned long` → `uptr`;
    - `__LP64__` → `HOST64`;
    - the 7 `(unsigned long)` casts.

    Check: portcheck win64-gnu-nomsbf reports 0 beyond the baseline (11 today, apart from the casts).

    **Done.** `IO_ADDR_` is `(__UINTPTR_TYPE__)(p)` (first `(unsigned int)(__UINTPTR_TYPE__)(p)`, then made pointer-sized with the `is_reg` fix, section 7; io_access.h stays self-contained), `Modern_SharedResolve` reads `uptr` words, the 8 size checks in ov103 use `HOST64`, the overlay header's `.sinit` words and `__fill_mem`'s alignment use `uptr`. (The other `(unsigned long)` casts were already gone; `TA_AFC` in ov003 is an offset difference, fine.) Portcheck win64-gnu-nomsbf: 1550 beyond the baseline (838 errors, 712 warnings) → **0**. Code is unchanged on Linux (same types there; oracle identical).
11. **2.4** Game libc names: a forced-include `include/host_libc_rename.h` mapping the 76 clashing names to `ds_*`. Delete host_localize.py and the objcopy step. Check: `nm` shows no clash; oracle.
    **Done (2026-10-08).** Re-measured: 77 names localised (i386), 76 (x86-64). Now: 66 C library names and the 5
    `__cxa_vec_*` helpers renamed by macros in `include/host_libc_rename.h` (`-include` for every game unit); `atexit`
    also through an asm label on its declaration, so that GCC's own registration of function-local static destructors
    reaches the game's no-op (clang ignores the label for its emitted calls: use `-fno-c++-static-destructors` there);
    the MSL's `std::terminate`/`dthandler` moved to namespace `msl`; `__cxa_pure_virtual` is the host's; the global
    `operator new`/`delete` are the host's (`modern/host/newdelete.cpp`, see 2.5). `nm` of `game_all.o` against the
    host's libc, libm, libgcc and libstdc++: 0 names defined by both; the game's only references into them are
    `operator delete`/`delete[]`, `__cxa_pure_virtual`, libgcc's `__udivdi3`/`__umoddi3` (i386) and one
    compiler-emitted `memset` (x86-64: the same result as the game's). host_localize.py and the
    objcopy step are deleted (the link uses `game_all.o`). Oracle: identical (32-bit with the captures, 64-bit with its
    own).
12. **2.5** `operator new`/`delete` with `size_t` in the source; drop `--defsym`. Replace the 60 mangled-name C declarations with member calls.
    **Done (2026-10-08)**, differently for operator new/delete: once the game's names are no longer made local (2.4),
    a game-defined global `operator new` would serve melonDS and the C++ library too. So the program's global
    operators are the host's (`modern/host/newdelete.cpp`, malloc), and `::operator delete` gives a block of the
    game's heaps back to the game (`Mem_FreeForDelete`, the DS's operator delete; `host_is_game_heap` in main.c: DS
    memory or the 64-bit root heap block). The game's 3 uses of the global operator new (a scene macro in
    unk_020039ec.cpp, `new TalkWindowState[2]`, `__cxa_vec_new`) allocate by name: `Mem_AllocForNew`, or
    `new (kGameHeap) T[n]` (include/sys/new.h). `ProcBase::operator new` takes `size_t`. No `--defsym`.
    Mangled names: re-measured 47 occurrences in 17 files of src/ (20 `#define X _ZN...` aliases, the rest
    `extern "C"` definitions and calls) plus 24 declarations in 4 generated proto headers; now **0**. ProcBase is a real
    class (its members, constructor and destructor defined as members; its vtable is the compiler's, not the ROM's
    from gap data); 11 aliases were unused (deleted); the calls into classes defined in another unit go through C
    functions there (`FtrShirt_SyncAct0` ..., `SpNpcRoostGuest_GetGuest`, `PlayerData_GetNookPoints`: direct,
    non-virtual calls as the DS's); `FxVec3::~FxVec3` is a member, with `FxVec3_Destruct` for the `__cxa_vec_*`
    helpers. Oracle: identical.
13. **2.6** Replace the `.set` absolute symbols (main.c:58-68; 68 references) with constants in a header.
    **Done (2026-10-08):** 11 `.set` symbols, 67 references in 16 files (one an unbuilt `.s`) re-measured;
    now `include/sdk_linker_symbols.h` (`#define` constants; the exception table bounds, both 0, are NULL in
    `__FindExceptionTable`), 0 `.set` left in main.c. Oracle: identical.
14. **2.7** Gap data as typed C for every architecture (113 objects; let the compiler encode the 12 PMFs; resolve or prove unused the 19 stale DS numerals). Drop the `--x86-64` asm emitter.
    **Done (2026-10-08).** Re-measured: 56 gap sections, 84 labels (`.set` into the arrays), 12 hand-encoded PMFs, 7
    labels unused. Now one file for every architecture, `modern/gen/gaps.c` (tools/host_gaps.py without `--x86-64`
    and without the own-slot list `tools/host_gap_pointers.txt`, both deleted): 14 objects, each label its own C
    object (the bytes, or a table of pointers: `data_01ff8ab4`), no asm. The 12 PMFs are the compiler's
    (`&ProcBase::onCreate` ...); ProcBase's ROM vtable and next id went to its unit. The 52 zero-initialised labels (49 used)
    are defined in the sources with their real types (pointers 8 bytes, `MBWork`, `WMMsg`, `PMRegInfo[4]` with their
    64-bit sizes: before, they had their DS sizes on x86-64 and `MBWork`/`WMMsg` ran into the next label); labels
    inside a larger object became macros over it (the WFC-ID block, the multiboot beacon, the cartridge buffer); the
    ones whose users are only in overlays are in src/autoload_2/gap_vars.c (an overlay's variables are reset on load,
    these must not be). They are now game variables in `gbss` (then in DS order, order.ld; since 2.8 where the linker puts them), not host bss. Stale DS
    numerals: of the 19 in the DS RAM range, 6 are in `BuildInfo` (the SDK's build strings, unused: no `nm -u`
    reference in either build; not emitted) and 13 in `data_020d0a7c`, the building table (`BuildingInfo[0x22]`, 10
    bytes of u8/u16 fields, no pointer: packed bytes that look like addresses); the I/O/VRAM-range ones are in the gap
    *code* (ARM instructions of functions written in C, not emitted). Oracle: identical.
15. **2.8** Variable order:
    - an order.ld variant that puts 64 poisoned bytes after every variable;
    - a per-frame check (`ACWW_GUARDS`) over the long scripts to list the overruns;
    - fix each one;
    - then drop order.ld's ordering, `-fdata-sections` and `-malign-data`.

    Check: oracle with and without the guard variant.

    **Done (2026-10-08).** The guard variant: `configure_host.py --guards [--arch x86_64]` (build_host[64]_guards.ninja,
    build/host[64]_guards_<v>). It links the game once as usual with a map (`ld -r -Map`), then `tools/host_guards.py`
    writes a linker script that places every input section of gdata, .rodata and gbss where that link put it, each
    followed by 64 guard bytes, and the guards' table (`{guard, kind, size, name, object}`); merged strings and
    constants get none. `modern/host/guards.c` fills the guards after the memory is mapped (`ACWW_GUARDS_FILL`, default
    0xa5; read-only ones through `mprotect`), and with `ACWW_GUARDS=1` compares them every frame and reports a changed
    one (variable, object, frame, the first changed byte's address for a gdb hardware watchpoint), then restores it.
    `--only LO-HI` gives only some variables guards (bisecting a layout dependence). 9063 guards on i386 in the DS
    order (4665 data, 2822 read-only, 1576 bss), 9070 after the drop and the merge of 3.2 (DTCM's variables are
    ordinary ones now; the same on x86-64). (Since 3.3 the variant's gdata/gbss are ordinary sections of the
    program, placed by the final link, no longer at DS addresses.)

    Found and fixed (each a separate commit; the writers from gdb watchpoints):
    - **Gfx2d state** (main, written past the end on every host): sGfx2dBgScroll, sGfx2dSubBg3Mtx, sGfx2dDispCtrl,
      sGfx2dWindowBlend are one object on the DS; `Gfx2d_Init` cleared the sub BG3 affine block (0x20 bytes) through
      the 0x10-byte sGfx2dSubBg3Mtx and window 0's rectangle across sGfx2dDispCtrl's end. Now one `Gfx2dState` with
      typed members (`Gfx2dOffset`, `Gfx2dAffine`, `Gfx2dWindowRect`).
    - **ov069's topic table copy** (`VillagerTalkTopics_CopyTables`, every host; 64-bit also lost data): the 15 tables
      were copied with DS byte counts (24 bytes per entry of three member function pointers; 48 on x86-64, so every
      table got only its first half), and the request topics A and B (26 + 47 entries) as one block into A, running
      into B. Now the tables' sizes with `static_assert`s, A and B apart.
    - **Sound archive objects** (64-bit only): data_021f5a1c/data_021f5aac were byte arrays sized by adding the
      members' sizes, 4 bytes short of the struct on x86-64 (alignment of the FSFile member): the `info` pointer was
      stored past the end. Now `NNSSndArc` (include/nnsys/snd_arc.h), also the type of unk_0210b448.c's and
      unk_0210b1b0.c's `Arc`.
    - **Reads past an end** (the guards don't see reads; these showed up as differences once the DS order was gone,
      and were found by bisecting the guard variant with `--only` and by gdb watchpoints):
      - `TvSound_GetMaxSize` looped over sTvSoundSizes until the address of data_020c6190, the next variable in the
        DS layout: the TV sound heap got another size, and the new-game save script's sound changed from frame ~27870
        (the guard after sTvSoundSizes alone changed it; 6 rounds of 4 runs). It uses the table's bounds now.
      - The house rooms' exits: `sRoomSceneEntryList = {&data_ov004_0225893c, 3}`, but the three SceneWarp entries
        were three variables (symbols.txt names each) with sRoomCommonProfileCount between them. Leaving the house in
        the continue run read exit 2 from whatever followed: a fade type of 39, an index past
        sTransitionTypeTable, a jump into the middle of `TownEval_EvaluateAcre` at frame 4290. Now
        `SceneWarp sRoomSceneEntries[3]`; the 32 other scene warp lists were checked: arrays of their counts.
      - (found later, 2026-10-08) **The field villagers' state table** (ov068, `FieldVillagerAi_ChangeState` /
        `_Update`): states 0..0x17 index `sFieldVillagerAiTable`, but the 24 `{enter, exec}` entries were three
        8-entry variables (`sFieldVillagerAiTable`, `...Table2`, `...Table3`, symbols.txt's names). Once the DS order
        was gone the linker put Table2 and Table3 *before* Table, so states 8..0x17 (face the talker, chat, take out
        / put away an item, pitfall, hit by the net, pushed, ...) read whatever followed: sTalkSteps, sChatSteps,
        sPushTalkSteps ... as member function pointers (wrong step functions, null entries skipped or called). On
        x86-64 a `{ptr, adj}` read from other data had an odd `ptr` and was called as a virtual function through a
        null vtable: the attract demo's crash at frames ~1430-1560 in
        `FieldVillagerAi_ChangeState` from `FieldVillagerAi_CheckHeldItemChange` (a villager taking out or putting
        away an umbrella / item, which depends on the date). Now one `sFieldVillagerAiTable[0x18]`; state 0's enter
        function, a free function's address in a `void *[2]` blob (`data_ov068_0226f878`), is a member
        (`FieldVillagerAiStates::enterWander`). Sweep of 205 pinned dates/times to frame 3000 (every hour of
        2026-10-08, -10-10, -12-24; the 1st and 15th of each month at 6, 12, 18, 23 h; 15 holidays / event days;
        every weekday): x86-64 crashed on **13** before (12 this bug, all from frame 1433 on - the one host-clock
        crash at 1434 was this; and 2026-03-01 in `Heap::lock` at frame 654, a null `gBgHeap`, gone with 3.3's
        `NNS_FndResizeForMBlockExpHeap` fix), **0** after, also with `ACWW_HIGHMEM=1` (and 0 of 200 more x86-64 runs on 2026-10-07..09 at random minutes and
        seconds: the time is the random seed). The i386 build crashed on 13 of the same dates too (the same 12 and
        2026-12-24 21:30; in `FieldVillagerAi_Update`, the `exec` call through the wrong entry), 0 after: not
        64-bit only, only the default clock (2026-04-01 12:00) never reaches it, so the captures are unchanged. The guard variant could not see it (a read, not a write). In the same unit, DS offsets that were wrong
        on x86-64: `FieldVillager_CanSimulateCatch` (fishing tourney / bug-off days) read the player's position at
        `(u8 *)character + 0x5c` (now `Character::position`), `FieldVillagerAiPutAway::enterPutAwayItem` the held
        tool model's frame counter at `+ 0x9c` / `+ 0xa0` (now the model's `AnimFrameCtrl` base and `numFrames`);
        the villager state flags at `+ 0x1d` are `VillagerState::flags`. ov068 has no other DS offsets, blobs or
        pointers in integers left in its FieldVillager code.

    **Sanitizer sweep (2026-10-08)**, for the reads the guards can't see: the sanitizer variant
    (`configure_host.py --arch x86_64 --sanitize address,bounds,undefined`, docs/host.md: ASan with the game's NNS
    heaps poisoned where unallocated and the coroutine switches annotated, UBSan's bounds and the rest of UBSan but
    alignment and shift-base, every check recovering) over the oracle's quick and play scripts, the town, save and
    continue scripts (tools/host_play_script.py) and dates: 205 pinned `ACWW_RTC` values in the attract demo to frame
    3000 (every hour of 2026-10-08, -10-10, -12-24, the 1st and 15th of each month at 6, 12, 18, 23 h, holidays,
    every weekday) and 29 of them through the title and the intro to frame 8000 (the play script's input), before
    (8) and after (29) the merge of step 3.4 (its virtual time changes what each date reaches; the demo sweep both
    times). Static passes: GCC's `-O2 -Warray-bounds=2 -Wplacement-new -Wstringop-*` over
    every unit of both builds (`placement new` into DS-sized storage, array subscripts outside an object), and the
    data symbols of config/usa/*/symbols.txt against the host's declared sizes (156 variables whose DS spacing
    exceeds their i386 size by more than alignment; the large ones checked by hand: types that changed size, strings
    and padding in between, the DS's bytes from the extracted ROM) and against references (194 variables only their
    definition names: none a continuation of an indexed table). Found and fixed (builds: both = i386 and x86-64):
    - **Reads past an end / split tables** (both): `Bgm_GetSceneBgmId` looped over sSceneBgmTable until the address
      of data_020c8b9c, the next variable on the DS (a scene without an entry read whatever followed); it uses the
      table's own end now. `NameLabelBalloon::applyKindAnim` indexed `data_020d467c + seqIndex * 8` with seqIndex
      10..13, 0x28 - the sprite sequence table that starts there on the DS is split into seven arrays (only two
      records in data_020d467c): now `gSpriteSeqTable[seqIndex]`; on x86-64 (16-byte records) the stride was wrong as
      well, and the continue script crashed in the resident select (a string's bytes taken as a cell pointer) once
      the layout changed. `Date_GetDaysInMonth` is asked for month 13 in December (`EventRule_ResolveDay`, the month
      after): the DS read past its 12 entries into the next table (sDaysInMonthLeap and sDaysInMonth are adjacent
      there, leap year first, then sWeatherPeriodEnds): the two tables are one `sDaysInMonth[2][12]` now and months
      0 and 13 give the DS's bytes (a common year's month 13: 4).
    - **A read of the wrong stack slot** (both): `FieldVillagerAi_TryFallInPitfall` passed `&a` of two locals `a, b`
      as a `Vec2` (adjacent only on mwcc's stack): the pitfall check read y from elsewhere; now one `Vec2`.
    - **Result types** (both): the particle position callback (`PosCb`, unk_020fa488 / unk_020f92d4 / unk_020fa39c)
      was declared returning s32 for a callback that returns u16 (`SplPosFn`): `spl_draw_pol` indexed the sine table
      with the undefined upper bits (ASan: past `FX_SinCosTable_`, in 26 of the 205 dates). `gti2HandlePing` had an
      empty body (no return value; the DS code is a tail jump to `gti2SendPong`).
    - **ARM shift semantics** (both): `ObjShadowStrip::build` shifts by a depth (0x119a..0x2000; UBSan
      shift-exponent): Thumb's ASR by register gives 0 for a count of 32 or more, x86 shifts by the count modulo 32,
      so tree stage 3's shadow got cullExtent 0xb33 instead of 0x1666. `DS_ASR` / `DS_LSR` / `DS_LSL`
      (include/ds_shift.h).
    - **A 4x3 load of a 3x3 matrix** (both): `NNSi_G3dFuncSbc_NODEMIX` sent its 9-word normal matrix with
      `G3_LoadMtx43` (12 words: 3 read past it on the stack); now 12 words, the translation 0.
    - **64-bit storage and layouts**: objects built in DS-sized storage - `MsgString9BStorage` (0x1c, the object 0x28),
      `TouchPickSphere` members of VillagerBoard and RoomBoardSign (0x1c of 0x20: RoomBoardSign's `index` after it
      was overwritten), ItemName / MsgString9B in word buffers (7 sites), the collision wall edges (`WallEdgeListWriter`,
      fixed the same way in 3.4's 1ce5cf3c, merged); the action requests' payload read at the DS offset 0xc by
      `PlayerActor_SetupLieInBed` and `PlayerActor::setupAct77` (0x10 on 64-bit hosts: the padding before the
      payload, stack contents: whether the player made the wake-up sound in the continue script depended on them).
    - **Uninitialised data in the save file** (both): the Able Sisters' default patterns copy an author record whose
      gender and padding byte are never set (`PatternPresetInfo_Apply`): stack contents went into the save (0 in the
      plain builds, other bytes with ASan's stack); zeroed now.
    - **Save-data blocks declared shorter than their records** (both; layout unchanged): `house` + `songSet` is the
      HouseData, `bbsBoard` + `unk_12000` the BbsBoard, `nookShop` + `unk_15e00` the NookShop, `reddShop` +
      `unk_15f80` the ReddShop (whose `sold[6]` made it 0x18 bytes, 4 into `ableShop`: 4 bytes, one used), and
      TownExchangeRecord's `unk_0f4` + `unk_0f5` its ConstellationRecord: one block each, with size `static_assert`s.
    - Smaller ones: `TownEval_EvaluateAndClean` formed `&acres[-1][-1]` (not read); a matching-era enum copy of
      `prevAction` (UBSan: invalid enum load); the runtime's global `operator new` lacked the nothrow forms.

    Before -> after: run-time findings 6 ASan places (sSceneBgmTable, the pitfall Vec2, the wall edge buffer, the
    NODEMIX matrix, the sine table, sDaysInMonth), 3 UBSan places (shift count, `acres[-1]`, enum load), a crash
    (the name label table, continue script, frame 3568) and 2 build-dependent results (save file bytes, continue
    sound from 57 s) -> 0; date sweep, 205 runs: 48 with reports (36 sDaysInMonth, 26 the sine table) -> 0, after
    the merge of 3.4 again 0 (crashes 0 throughout). The tooling's own false positives (the heap allocators' header
    walks and fills, word-at-a-time string functions, an overlay reset over a heap's buffer, the ARM7's sequence
    reads) are handled in the variant, not in the game. The sanitizer build's output equals the plain x86-64 build's in all five scripts (screenshots, WAV,
    save files) once the uninitialised reads were fixed. Oracle (tools/host_oracle.sh, captures_v2): quick and play
    identical on i386 and x86-64 with every fix; i386 and x86-64 save and continue identical to each other (and the
    save file to the pre-fix x86-64 build's). Before the merge of 3.4 the WallEdge fix alone changed the x86-64
    output (proved by reverting only it: identical to the pre-change build), and made its quick screenshots equal
    the i386 captures.
    `tools/host_overlay_vars.py --check` on a fresh build reported 332 files out of date and 231 function-local
    statics "can't be reset" (ov147 TitleTalk's `tbl` ...): a tool problem since 3.3 - in the x86-64 PIE objects
    constant data with relocations (member function pointer tables, the generated lists themselves) is in
    `.data.rel.ro*`, which nm also calls data. It reads `objdump -t` and skips those sections now: 0 out of date,
    0 problems (2956 variables).

    Checks (after merging modern 7291e665, steps 3.0-3.2). The guard variant over quick, play, the town script, the
    save script and the continue run (i386) and quick, play, town (x86-64) reports **0 overruns** (before the fixes:
    3 variables on i386, 4 on x86-64); with fills 0xa5 and 0x5a the long scripts give the same output as modern's
    build, and quick and play the same with 0xa5, 0x00, 0x5a and 0xff. With the drop (no order.ld ordering, no
    `-fdata-sections`, no `-malign-data=abi`; tools/host_order.py deleted; the partial link keeps a 3-line `game.ld`
    that names `gdata`/`gbss` until 3.3): oracle quick and play identical (i386 with the captures, x86-64 with
    modern's own run), the save, continue and town scripts identical to modern's build (screenshots, sound, save
    files; i386, and town on x86-64), x86-64 also with `ACWW_HIGHMEM=1` and `=2`. Before the two read fixes the
    unordered build differed in the save script's sound and crashed in the continue run: the layout changed
    behaviour only through these reads. Two things seen on the way, not layout bugs: the x86-64 guard variant hung
    once in the attract demo because the per-frame check restored a guard the 4-byte-short sound archive had stored
    its pointer in (gone with that fix); and it crashed in `HappyRoom_Evaluate` in the town script - the room score
    sources' struct-return bug fixed on modern in 549c0c7c (stack layout decides whether it shows).
16. **2.9** Overlay reset without the linker:
    - generate `overlay_table.c` with each overlay's variables (`{&var, sizeof var}`, from config symbols.txt and the overlay units), the pristine copy taken per variable;
    - make the 98 overlay constructors explicit (a generated `OvNNN_StaticInit()` per overlay calling the units' init functions, or the objects made constant-initialised).

    Check: `ACWW_OVLOG` output (bytes reset, constructors run) is the same as today; oracle.

    **Done (2026-10-08).** Re-measured: 91 overlay units (in 54 overlays) had a compiler-generated static initialiser
    (`.init_array`), in both builds; 83 function-local statics of overlay units (33 constant data, 50 bss, 57 guard
    variables) were data no table could name. Now:
    - Every overlay unit ends with the list of its variables (`HOST_OVERLAY_VARS(<unit>) { HOST_OVERLAY_VAR(x), ... }`,
      include/host_overlay_vars.h): 2961 variables in 332 units, generated by `tools/host_overlay_vars.py` from both
      builds' objects (names qualified by libclang where a unit defines them in a namespace; `--check` lists what is
      out of date, function-local statics and unexpected static initialisers). modern/gen/overlay_table.c (generated,
      committed) collects the lists and the units' `StaticInit_<unit>` per overlay. `modern/host/overlay.c` copies the
      variables in `HostOverlay_Init()`, called first in `main()` (no constructor priority), restores them when the
      overlay is loaded and runs its StaticInits in link order. order.ld kept only the variable order (gone in 2.8): no
      `host_ovNNN_*` symbols, no `host_ovinit`.
    - The initialisers became constant data where they are constant: constexpr constructors (`DebugColor`,
      `VecFx32Ctor`/`VecFx32Copy`, `FxVec3`, `SceneWarp`, `ItemId(u16)` and local value types), trivial destructors
      where the DS's were empty (`FxVec3`, `Vec3Z`, `SceneWarp`), the spawn lists' counts as constants
      (`ROOM_COMMON_PROFILE_COUNT`, `FIELD_SCENE_PROFILE_COUNT`: on the DS copied from ov004's / ov003's variable),
      main's const values written as literals. That leaves 15 `StaticInit_<unit>` functions: member-function-pointer
      tables (cast between types or copied from other variables: `HOST_STATIC_INIT_ARRAY`), the museum's information
      points (`MuseumInfoPoint_InitFromDef`, which uses the divider: virtual time passes at the load as before),
      `gSceneBlockMap` read at the load, ov069's copy of its tables into main's, and the objects of classes with
      default constructors (`HOST_STATIC_CONSTRUCT`: these are constructed at start-up too - their constructors only
      set their members - and again at every load).
    - The function-local statics moved out of their functions (`<Class>_<function>_<name>`); the 37 initialised at
      run time keep their lazy initialisation with an explicit guard variable that is reset with the overlay.
    - `ACWW_OVLOG`: the same bytes reset at every load of the quick and play scripts (0 changed, as before);
      constructors 1/1/4/8/1 for overlays 69/68/3/4/9 instead of 1/5/13/22/1, 0 instead of 1 for 6 and 44 (constant
      data now). Oracle identical (32-bit with the captures, 64-bit with its own pre-change run).
17. **2.10** Shared-address calls:
    - typed C dispatchers (`Shared_<Name>(args)` → `Modern_SharedResolve`) generated from the proto headers;
    - rename the call sites at the source;
    - delete `--wrap` and the asm;
    - review the 275 data references at shared overlay addresses.

    **Done (2026-10-08).** `tools/host_shared_calls.py` (the overlays' symbols.txt and both builds' objects) writes
    include/host_shared_calls.h, modern/gen/shared_calls.c (the dispatchers) and modern/gen/shared_table.c (each
    dispatcher's candidates by symbol: plain C declarations of the names, also of C++ ones - the mangled name is a C
    identifier and gets the same prefix on Mach-O); `--rename` renamed the references outside the named function's
    overlay (575 in 52 units, libclang; one inline constructor in room/RoomObjTexAuto.h by hand). Of the 291 thunks:
    - 163 C functions with another function at their address got a dispatcher. Its parameters are pointers as
      `void *` and integers as they are, as many as the callers pass: main's player actions call ov003's functions
      with the arguments of the ov004 functions at the same addresses (`FishShadow_RunAi(this, 0, 6, -1)` for a
      one-parameter function); 13 calls whose results are typed pointers got casts. A candidate must be defined in
      both builds (a non-virtual thunk's name differs: `_ZThn236_` / `_ZThn328_`).
    - 62 had no other function defined at their address (the thunk always returned the named one) and 66 are member
      functions, constructors and destructors: called on an object of their class, whose code is in the named
      overlay, which is therefore loaded. They are direct calls. Logged over the play script before the change: every
      resolution was the named function (and once, a constructor at start-up, no overlay loaded: the named one too).
    - The 275 data references (193 symbols, `--report`): main's tables of the overlays' process profiles (145) and
      of the scene and menu overlays' records (44), used once that overlay is loaded; main's read of ov004's
      `sHouseRoachActiveCount`, behind `Scene_InHouseRoom()`; the room overlays' references to ov004's
      `sRoomCommonProfiles` / `sRoomSceneEntryList` (loaded with ov004); one ov009 vtable entry read by ov003 (loaded
      together). Each means the named overlay's data: no dispatch needed.
    `Modern_SharedResolve` takes a `HostSharedCall`; configure_host.py has no shared rule and no `--wrap`; modern_shared.py
    keeps only the DS (ARM) thunks for configure_modern.py. Oracle identical.
18. **2.11** provides.ld: fix the 133 alias users' declarations; turn the 99 interior labels into committed C accessors (one last run of host_relayout.py). The build no longer reads DWARF and needs no linker script.

    **Done (2026-10-08).** Re-measured after 2.9: 126 aliases and 91 labels referenced (`nm -u` of both builds'
    game_all.o ∩ provides.ld; 8 labels fewer since the 2.1 merge). modern/gen/provides.ld, provides64.ld, the
    relayout step and tools/host_provides.py are gone.
    - Aliases (126 → 0): members declared only, whose symbols symbols.txt gave to other functions, have bodies:
      constructors and destructors calling the C functions (`AnimSlotRef() { AnimSlotRef_Init(this); }`, ~45
      classes, the same signatures as the definitions, ABI unchanged: user-provided as before), methods calling the
      methods they were labels of (`BgmRequest() { initEmpty(); }`, `~NpcLookAt() { clearTargetActor(); }`);
      `CollisionTriangleX` is a typedef of `CollisionTriangle`; `CollisionCylinderX`'s two clip functions forward to
      `CollisionCylinder`'s; `TrendScores::pickTop` / `resetAndHalve` are the functions symbols.txt filed under
      `VillagerPlan`; the `VillagerActor` partner methods and `SkySprite`'s, `SkySprites`', `WeatherManager`'s members
      are defined in their units; `ItemPickSpec_Destruct(p)` calls are `p->~ItemPickSpec()`; empty DS destructors
      are `{}`.
    - Labels (91 → C): `tools/host_labels.py` (one run, while the labels were still provided) put a pointer per label
      at the end of the base variable's unit (`HOST_LABEL_DEFINE(gSavePlayers, (char *)&gSaveData.players[0])`, 14
      units): a member path from the 32-bit DWARF where the base has a type, so the 7 labels that host_relayout.py
      moved on 64-bit (`gSndBgmHandle`, `sHouseVisitTsuTopicTable` + 0x30, ...) are right by construction, base +
      offset where it is bytes; include/host_labels.h declares them. The 257 declarations of labels (111 files) are
      macros with the unit's own type: `#define gSavePlayers HOST_LABEL(u8 (*)[], gSavePlayers)`.
    Oracle identical; host_strict_errors 0.

After phase 2 the game links with a plain `cc *.o`: no `ld -r`, linker script, `--wrap`, `objcopy` or PROVIDE.
**Status (2026-10-08, after 3.3):** done. No `ld -r`, linker script, `--wrap`, `objcopy`, PROVIDE, `--defsym`,
`--section-start`, `-Ttext-segment` or DWARF reading: x86-64 links with `g++ -m64 -pie -o acww <objects> <melonDS
libraries> -lstdc++ -lpthread -lm`. The i386 build adds only its sysroot options and `-no-pie` (it stays a
fixed-address executable, as its captures were taken). The guard variant (`--guards`) still links the game with
`ld -r` and a generated linker script, as a GNU-ld debugging tool.

### Phase 3: memory anywhere (Linux x86-64 above 4 GB, 3-6 weeks)
19. **3.0** Low-4 GB poisoning harness (`ACWW_HIGHMEM=1` on x86-64):
    - root heap via a plain `mmap` above 4 GB;
    - stacks without `MAP_32BIT`;
    - every *unused* range below 4 GB reserved `PROT_NONE` (an emulation of `__PAGEZERO`), keeping only the DS mappings still in use.

    A truncated pointer then faults at a low address with a backtrace. Run quick, play, the town script and save/continue.

    **Done (2026-10-08).** `ACWW_HIGHMEM=1` (x86-64 Linux; modern/host/hw.c `highmem_poison`, called from main.c's
    `early_init` once the DS mappings and the VRAM windows exist): every gap below 4 GB in /proc/self/maps is
    reserved `PROT_NONE | MAP_NORESERVE` (6-7 ranges, 3934 of the 4095 MB). What stays: the DS mappings (ITCM, main
    RAM and its mirrors with DTCM, WRAM, palette, the VRAM windows, OAM), the program image at 0x10000000 and the C
    library's first brk heap (it can't grow any more: malloc maps above 4 GB then). The root heap is an 8 MB block at
    0x5a0000000 and the thread stacks are mapped from 0x6b0000000 up (`host_highmem_alloc`, `MAP_FIXED_NOREPLACE`):
    the low words of their addresses (0xa..., 0xb...) lie in poisoned space, so a pointer cut to 32 bits faults
    (sign-extended: a non-canonical address, also a fault). The crash report (debug_posix.c) is installed again after
    `melon_init`: melonDS's JIT (compiled in, unused) installs a SIGSEGV handler of its own, and before this a
    64-bit crash ended without a report. Scripts run (all without a fault): `oracle.sh quick-raw` and `play-raw`;
    the x86-64 town script (docs/host.md: A every 60 frames 2400-26500, Start 7650-21050, the Confirm touch every
    600 frames 7630-21050, B 26520-27000, Down 27100-27500, then Right 28200, Down 28700, Left 29200, Up 29700,
    Right 30200, Down 30900, 400-600 frames each; screenshots to frame 32000: out of the town hall and around
    town); the save/continue script (i386 timing; the x86-64 build doesn't reach the save with it) and the continue
    run from a save the i386 build wrote (wakes in the house, saves its start-of-session state, out in front of the
    house at frame 8000). The pictures and sound are the same with and without the harness (no behaviour depends
    on the addresses).
20. **3.1** Fix the pointer-in-32-bit sites:
    - the 246 C warnings, the 186 C++ int-to-pointer casts, and the 92 + 31 explicit `uptr` round trips (each one an integer that should be a pointer, or an offset);
    - in-place resource relocation (NFTR unpack, SndArc `e->ptr`, ...): resolve the offset when used;
    - PXI: call pxi7/snd7 directly with pointers (the ARM7 is the same process): a host mailbox carries `void *` instead of 26-bit words; snd7 `dsptr` becomes `void *`, and `readable()` checks the real buffer bounds;
    - DMA: implement the SDK DMA entry points (`MI_Dma*`, GX/GXS loads) in sdk.c with real pointers, as the CPU copies already are; register DMA only for the remaining GX FIFO/HBlank cases with resolvable addresses.

    Check: the harness runs every script without a fault, the oracle is identical, and `--strict-c` is clean.

    **Done (2026-10-08).** Re-measured first (after phases 1 and 2): `--strict-c` 220 distinct sites (105 pointer to
    int, 115 int to pointer) in 54 files; libclang (x86_64-linux, every unit) 269 int/pointer cast warnings, 34 of
    them C++ int-to-pointer casts in 4 files (the rest were fixed during phase 2); explicit round trips of a pointer
    through a 32-bit integer by grep: 15 `(u32)(uptr)`-style and 15 `(T *)(uptr)`. Now: `--strict-c` **0**,
    libclang casts **0** (C and C++), `tools/host_strict_errors.sh` 0, `tools/host_portcheck.py` 0 on every target.
    The remaining `(u32)(uptr)` / `(T *)(uptr)` are DS addresses used as numbers (VRAM, the system work area's arena
    bounds and thread-info pointer, overlay load addresses, I/O register addresses: step 3.2), ids made from
    addresses (an echo id, an aid bit) and the download-play / wireless code (DS-layout records; not reached on the
    host, which has no WM service). What was done:
    - **NNS heaps** (expheap/frmheap, 11 units with copies of the same structures): `Region`, `HeapHead.start/end`,
      `FrmHead/FrmState.head/tail` are `uptr`; the creation, free and resize functions take `void *` (as the SDK's)
      instead of u32 - the callers declared them with pointers, so a 64-bit host passed whole pointers and the
      definitions used the low halves. The sound heap the same (`NNS_SndHeapCreate(void *, u32)`).
    - **In-place relocation resolved when used.** The sound archive's FAT keeps a loaded file's address in the
      entry's pointer word and the reserved word after it (8 bytes on 64-bit hosts: `FatEnt_Ptr`). The NFTR font
      links (`NNSi_G2dUnpackNFT`), a sound bank's wave archive links and a wave archive's banks
      (`SND_AssignWaveArc` ...), a wave loaded on its own (`SND_SetWaveDataAddress`) and two DS-layout callback slots
      of the wireless code keep their 32-bit file slots: they hold a `ptr32` (include/host_ptr32.h, modern/host/
      ptr32.c): the pointer itself while it fits (always on 32-bit hosts and for DS memory: the i386 build is
      unchanged), else a handle into a host table, resolved by the readers (`HostPtr32_Get`; snd7's `dsptr`
      helpers are gone). The FS tables loaded into memory (`FS_LoadArchiveTables`) are offsets into `load_mem`; a
      memory archive's base is pointer-sized and its positions are signed offsets (the NARC's tables lie before the
      image).
    - **PXI.** `ptr26` is gone. A message that carries an address hands the pointer to the ARM7 side first
      (`PXI_PointerWord(tag, p)`, include/nitro/pxi_ptr.h; pxi7.c `HostPxi_PutPointer` / `take_pointer`) and is then
      sent unchanged, so the register traffic - and virtual time - is the DS's: the sound command list (snd7_receive
      takes the list), the card request block (`CARDCmd`, include/sys/CardCmd.h, now shared by the ARM9 units and
      pxi7.c, buffers pointer-sized), the NVRAM buffer, the WM request.
    - **snd7.** The SND commands' arguments are pointer-sized on both sides (`SndCommand.arg`, `PushCommand_impl`,
      `SND_PrepareSeq` / `SND_SetupChannelPcm` / `SND_SetupCapture` / the invalidations / `SND_ReadDriverInfo`);
      no 26-bit masks (`& 0x7fffffc`); `SOUNDxSAD` and the capture destinations are `uintptr_t`; a channel reads its
      samples only within its own buffer (`SOUNDxPNT + SOUNDxLEN` words; the ADPCM header at least) and a capture
      writes only within `SNDCAPxLEN` words, instead of "anywhere in 0x02000000-0x03000000". The stream library's
      channel buffers (`NNSSndStrmBufAddr`) and the capture library's buffers are pointers.
    - **DMA.** The functions that set a channel (`MIi_DmaSetParams` and its three variants) take `uptr` addresses
      and give them whole to the DMA model first (`HostDma_Addresses`, include/nitro/dma_host.h); hw.c's `dma_run`
      uses them while the registers still hold their low words (a register written otherwise - an I/O address, 0 -
      is taken as it is). The entry points (`MI_DmaCopy32/16`, `MI_DmaFill32`, the async ones,
      `MI_SendGXCommandAsyncFast`, `MIi_CardDmaCopy32`, the GX command DMA state `MIGXDmaParams`, now one header)
      take pointer-sized addresses. This keeps the DMA register sequence (the DS's), unlike a CPU copy, and so the
      i386 oracle.
    - **Declarations that disagreed with their definitions across a pointer and an integer** (tools/host_decls.py;
      a 64-bit host passes the whole register and the other side uses half of it): fixed where a pointer flowed -
      RTC dates (`RTC_GetDate(void *)`...), the thread parameter (`OS_SetThreadParameter`: the per-thread current
      heap, cut at the first thread switch under the harness), `OS_InitContext` / `OSContext.r[]` (a thread's
      argument), `OSi_EnterTimerCallback`, the backup reads/writes (`Backup_Read(..., void *)`), `CARDi_ReadRom`,
      the driver info reads, `SND_RecvCommandReply`, `NookShop_InitNew` (the shop through an `int`), `WfcUtil_Run`
      (the WFC work area through a u32), the G3D render object callbacks, `Character_FindInteractionTarget` (a
      pointer result read as `BOOL`), a view of `gSceneBlockMap` as u32 (ov068) and the crash screen's view of the
      thread info. 179 mismatching declarations of that kind are left (94 functions; 112 of them in the network and
      wireless code); outside it, the calls through them pass constants (0, a DS register address) or ids.
    - **Real 64-bit bugs found on the way**: the SND alarm records (`SlotEnt`) had the DS stride in the u32 view
      (12 bytes, 24 in the definition), so the ARM9 side registered alarm callbacks where the PXI handler didn't
      look and the stream's refills never ran: from ~24 s of the attract demo (the title music) the 64-bit build
      played a stuck buffer; now its sound follows the i386 reference (the RMS level per 2 s matches; the samples
      part at 24.6 s with the build's usual timing drift). The capture messages (`CapMsg[8]`, 160 bytes on the DS,
      192 needed) ran into the capture thread's `OSThread`, and the capture message array (`OSMessage[8]` as
      `u32[8]`) into the capture state. The FS memory archive read from `base + (u32)offset` with negative offsets
      (the NNS file system's mount of a NARC). NNS fonts on 64-bit read their glyph/width/map links as 8-byte
      pointers from 4-byte file slots (only the Wi-Fi menus use them). `Insect_NetClaim` read through the packed id
      as an address when neither flag was set (the DS read low memory there; now "no insect"). The room score
      sources (`VillagerRoomScoreSource` / `HouseRoomScoreSource`, src/main/unk_020594dc.cpp: the happy-room
      evaluation when a new town is set up and when the game saves) had their struct-returning virtuals written in
      ARM's shape (the result slot as `this`; i386 with `-fpcc-struct-return` shares it): on x86-64 they wrote the
      item into the caller's object - its vtable pointer. Whether the next virtual call then crashed depended on
      the vtable's address: after the merge of steps 2.9-2.11 it did, at frame 12228 of the town script and 4344
      of the continue run, in both 64-bit builds of refs/heads/modern d0eb4763 (with and without the harness). Now
      real `RoomSurfaceItem` results; the i386 save/continue run is unchanged (pictures and save file).
    - Check: the i386 oracle (quick: pictures and sound; play) identical after every step; the 64-bit build's quick,
      play and town runs identical to the same tree without these changes (refs/heads/modern 1700271b), except the
      quick sound from 24.34 s - and that is the alarm fix alone: with only `SlotEnt` put back, quick (pictures and
      sound) is identical to modern's; `--strict-c`, `host_strict_errors.sh`, `host_portcheck.py` 0.
21. **3.2** System work area, DTCM, palette, OAM, VRAM and arenas become host objects:
    - a rewrite tool turns the 160 system-work/DTCM casts into `HW_SYSWORK(...)` over a `HostSysWork` block, and the 18 + 5 palette/OAM casts into `HW_PLTT`/`HW_OAM`;
    - VRAM: 32 casts plus 2 tables through `HostVram_CpuPtr(dsaddr)`. This replaces vram.c's mmap aliasing: each bank appears in at most one CPU window at a time, so a lookup suffices. Assert that no pointer is kept across a VRAMCNT change;
    - DTCM is a 16 KB host array (the `.dtcm` partial link goes away);
    - ITCM, WRAM and the main RAM arenas are host blocks (unk_02114724.c, unk_02114810.c);
    - the about 25 range checks become checks against the host blocks;
    - the host runtime stops writing DS addresses (main.c:91-93, pxi7.c:159/282/396, frontend.c:86, os.c:196);
    - the debug tools (`RAMHASH`, `RAMDUMP`, `HEAPCHECK`, `VRAMDUMP`) walk the host objects instead.

    Check: the harness with *all* DS mappings removed; oracle.

    **Done (2026-10-08).** Re-measured first (refs/heads/modern 549c0c7c, `tools/portcheck/dsaddr.py`): casts of DS
    constants to pointers outside I/O 207 in 70 files - system work area/DTCM 161 (57 files), palette 18, OAM 5, VRAM
    8, main RAM 10 (7 `OSi_ReferSymbol` no-ops, 3 arena bounds), WRAM 2, GBA slot 2 - plus VRAM addresses made
    pointers at run time (24 in `G2_GetBG*Ptr` / `G2S_*`, 6 from the LCDC tables in `GX_LoadTex` / `GX_LoadTexPltt`),
    4 palette writes through a `REG16(0x05000000)` macro and 1 computed palette address; DS addresses used by the host
    runtime: main.c 3, pxi7.c 4, frontend.c 1, os.c 1, sdk.c 1 (`OS_GetDTCMAddress`), melon.cpp 2, hw.c's debug dumps
    5 and the heap check. Now **0** casts of DS constants to pointers outside I/O, and the runtime writes no DS address
    except main RAM's mapping itself:
    - **The host objects** (include/host_mem.h, modern/host/mem.c): `HostSysWork gHostSysWork` (0x027ff000-0x027fffff:
      the SDK's members by name - the shared arena, the ROM headers, boot info, vblank count, NVRAM user info, arena
      bounds, RTC, PXI handle checkers, thread info, buttons, touch panel, the lock words; the DS's layout on 32-bit
      hosts, checked by `_Static_assert`s; the arena bounds, the main thread info and the exception vector are pointers,
      8 bytes on 64-bit hosts), `gHostDtcm[0x4000]`, `gHostItcm[0x8000]`, `gHostPltt`/`gHostOam` (0x800 each),
      `gHostMainRam` (DS 0x02200000-0x023fffff: the main arena past the program's data, `HostMainRam_Ptr(dsaddr)`) and
      the empty WRAM arena. Each block is aligned so that its addresses keep the DS's low bits.
    - **The rewrite**: `tools/host_hwaddr_rewrite.py` (`--check` lists what is left) turned 182 literal casts in 70
      files into `HW_SYSWORK(T, member)` / `HW_SYSWORK_AT(T, member, offset)`, `(T *)(HW_DTCM_BLOCK + o)`,
      `(T *)(HW_PLTT + o)`, `(T *)(HW_OAM + o)`, `HostVram_CpuPtr(dsaddr)`; it leaves the casts to pointer members for
      a hand edit (11: `OS_SetArenaLo/Hi` and `OS_GetArenaLo/Hi` keep `void *` now, `OS_InitThread` stores the thread
      info pointer, `OS_InitException` the vector). By hand: the two units with their own partial `OSSystemWork`
      (`OS_GetLowEntropyData`, the PXI FIFO init: at DS offsets past the pointer members, which move on 64-bit hosts)
      use `HostSysWork`'s members; the users of the DTCM variable `data_027e0000` as DTCM's start (`HW_INTR_CHECK_BUF`,
      the IRQ stack checks, the arena, the Wi-Fi VBlank) use `HW_DTCM_BLOCK`; the VRAM addresses made at run time go
      through `HostVram_CpuPtr` where the number becomes a pointer (the tables keep their DS numbers); the 7
      `OSi_ReferSymbol` arguments are 0; the GBA slot check reads the open bus of an empty slot (unreachable, as before).
    - **VRAM** (modern/host/vram.c): the banks are host memory and the CPU sees VRAM through five window arrays (the
      engines' BG and OBJ windows, LCDC) in 16 KB granules mapped to banks by VRAMCNT. At a VRAMCNT change the windows
      are written back to their banks and filled from the banks' new places (the parts without a bank read 0, as with
      the mmap remapping); before the renderer reads the banks (and the dumps) the windows are written back. This is a
      lookup by address, but into windows rather than banks: a pointer kept across a VRAMCNT change sees what the DS
      would show at that address, and a copy that runs from one bank into the next in a window works as on the DS, so
      there is nothing to assert (a lookup straight into the banks would have needed the assertion and a size at every
      use). An address outside the windows is reported once and gets a scratch area. Cost: the mapped banks copied
      once per frame (at most 1.6 MB).
    - **DTCM**: the 9 `src/dtcm` units are ordinary game objects; `ld -r -T dtcm.ld` and `--section-start=.dtcm` are
      gone. **Arenas**: `OS_GetInitArenaLo/Hi` return the host blocks (main: `HostMainRam_Ptr(SDK_MAIN_ARENA_LO)` to
      0x023e0000, then `OS_SetArenaHi(0, HostMainRam_Ptr(0x023ff000))`; ITCM, DTCM, the shared arena in the system
      work area, the empty WRAM arena; no extended arena, as on a retail DS).
    - **Range checks** (the comparisons with DS addresses, in 9 functions, plus host code): against the host
      blocks where they test the game's memory - `CrashScreen_IsValidAddress` and `::operator delete`'s
      `host_is_game_heap` (`HostMem_IsGameRam`), `CARDi_IsInTcm` (the ITCM and DTCM blocks), hw.c's DMA check (a null
      or small address); `MIi_CheckDma0SourceAddress` keeps the I/O rule only (the GBA slot range is host memory on
      32-bit hosts); the debugger's exception buffer range in `OS_InitException` is never true on the host. Left as
      they are, because they test DS addresses as data, not host memory: the download-play checks of a child
      program's segment addresses (`MBi_IsAbleToRecv`, `IsAbleToLoad`, `MBi_SetSegmentInfo`: 17 comparisons), the
      sound file slots' "offset below 0x02000000" (`SND_GetWaveDataAddress`, snd7's driver), and the CTRDG module's
      main-RAM offset in a PXI word (only with POSTFLG bit 0, never on the host).
    - **The host runtime** writes `gHostSysWork` (main.c: ROM header, card id, boot type; pxi7.c: RTC, touch panel,
      the ARM7 handle checker; frontend.c: the button word), os.c waits on `HW_INTR_CHECK_BUF`, sdk.c's
      `OS_GetDTCMAddress` returns the DTCM block, melon.cpp copies the palettes and OAM from the host arrays.
    - **Debug dumps**: `ACWW_RAMDUMP` writes a 4 MB main RAM image assembled from gdata/gbss, `gHostMainRam` and the
      system work area at their DS offsets (on 32-bit builds the same layout as before: comparable with older dumps);
      `ACWW_RAMHASH` hashes that image, DTCM and the system work area; `ACWW_HEAPCHECK` walks the NNS heaps in the main
      RAM block and the 64-bit root heap block with the pointer-sized header; `ACWW_VRAMDUMP` / `ACWW_STATS` write the
      windows back first and read the host palette and OAM. Checked: the i386 VRAM dump at frame 3000 equals the
      pre-change build's except DMA3's address registers (low words of host pointers).
    - **The harness**: `ACWW_HIGHMEM=2` doesn't map main RAM either; of the DS's address space only the loader's pages
      of gdata and gbss remain (and nothing else is mapped by default any more: ITCM, WRAM, palette, VRAM windows,
      OAM, main RAM's mirrors are gone; main RAM itself stays mapped by default until 3.3).

    Checks: i386 oracle quick (pictures and sound) and play identical to the captures; i386 save/continue script:
    all screenshots and the save file (after the save and after the continue) identical to the pre-change build; the
    64-bit build's quick and play identical to its pre-change run, and under `ACWW_HIGHMEM=2` quick, play, the town
    script (to frame 32000) and the continue run from the i386 save (to frame 8000, save file included) without a
    fault and identical to the pre-change 64-bit runs without the harness; `--strict-c` 0, `host_strict_errors.sh` 0,
    `host_portcheck.py` 0 beyond the baseline on macos-arm64 and win64-gnu-nomsbf.
22. **3.3** gdata/gbss become ordinary `.data`/`.bss`: drop the `--section-start` options, the main RAM memfd (its mirrors went in 3.2) and `__start_/__stop_`. Then `-fPIE -pie`, dropping `-Ttext-segment`, `-no-pie` and `MAP_32BIT`. Check: x86-64 PIE build under ASLR, oracle, and the harness poisoning the whole low 4 GB.

    **Done (2026-10-08).** Before: `ld -r -T game.ld` (the game's data and bss as `gdata`/`gbss`), 2 `--section-start`
    options, `-Ttext-segment=0x10000000`, `-fno-pie`/`-no-pie` on x86-64, main RAM as a memfd mapped at 0x02000000
    (the loaded gdata/gbss pages copied into it), 4 `__start_/__stop_` symbols (hw.c, mem.c, guards.c), the root heap
    at 0x50000000, `MAP_32BIT` stacks; the harness reserved 5 ranges, 3947 of the 4095 MB below 4 GB. Now:
    - **The link** (tools/configure_host.py): the game's objects and the host's are linked directly, x86-64 with
      `g++ -m64 -pie -o acww <objects> <melonDS libraries> -lstdc++ -lpthread -lm` (game, runtime and assembly
      compiled `-fPIE`; melonDS's libraries were PIC already). The i386 build stays a fixed-address executable
      (`-fno-pie`/`-no-pie`, the linker's default address) as the frozen reference; its data is ordinary too, and
      its oracle is identical. No partial link, linker script, `--section-start` or `-Ttext-segment` is left
      (the guard variant keeps its own `ld -r` with generated scripts: a GNU-ld debugging build; checked: it builds
      as a PIE and runs).
    - **Where the game's variables are** (the debug dumps, `HostMem_IsGameRam`: a heap can live in a static buffer,
      e.g. `sFieldObjectModelHeapBuf`, and `::operator delete` must give its blocks back to it): between the
      variables of modern/link/game_begin.c and game_end.c, linked first and last of the game's objects (linkers
      keep the input order within an output section). Checked on the link map: all 673 .data and 397 .bss input
      sections of game objects lie between them, nothing else does (x86-64: 0x30951 bytes of data, 0x11f6c8 of
      bss); `HostMem_Check` checks the order and the host blocks' alignment at start-up (the kernel loads a PIE at
      its segments' largest alignment, 2 MB for `gHostMainRam`). The game's constant data with pointers is in
      `.data.rel.ro` in the PIE (read-only after relocation, as `.rodata` was).
    - **The runtime**: no memfd, no mapping at a DS address; the root heap and the thread stacks are anonymous
      mappings anywhere (no fixed address, no `MAP_32BIT`; `host_highmem_alloc` is gone). `ACWW_RAMDUMP` /
      `ACWW_RAMHASH` keep the 4 MB image's shape (data from 0x1000, bss from 0x40000, the arena at 0x200000, the
      system work area at 0x3ff000). The crash report prints each code address also less the load bias (the
      address `addr2line -e build/host64_usa/acww` takes). The DWARF tools (host_view_layout.py,
      host_var_sizes.py, ...) take the executable instead of `game_all.o`.
    - **The harness** (`ACWW_HIGHMEM=1`; `=2` is the same now): the whole low 4 GB from 64 KB up, one range, 4095 MB,
      is reserved `PROT_NONE`; nothing is mapped below 4 GB any more (anything that is gets reported).
    - **A real 64-bit bug found**: `NNS_FndResizeForMBlockExpHeap` (src/autoload_2/unk_02100708.c) found a
      block's header at the DS's 16 bytes before the block (`sizeof(MBlock)` is 24 on 64-bit hosts) and read its size
      from the high half of the header's `prev` pointer. With the heap below 4 GB that was 0: a "grow" that found no
      free block, so `FrameHeap::doAdjust` (shrinking a frame heap's block in its parent) failed silently; at a
      PIE's addresses (0x7f...) it took the shrink path on the wrong header and broke the root heap's free list
      (a crash at frame 526 of the attract demo, `PlayerBodyWorkPool_Create` -> `Heap::adjust`). Fixed separately
      first; the fix alone changes no output (below).

    Checks: i386 oracle quick (pictures and sound) and play identical to the captures. x86-64, against the build of
    refs/heads/modern c792395e (non-PIE): with the heap fix alone, quick (pictures and sound), play, and under
    `ACWW_HIGHMEM=1` the town script (32 screenshots to frame 32000) and the continue run from the i386 save (8
    screenshots and the save file) identical; the PIE build with the fix identical to both, in runs at different
    load addresses (ASLR on). `host_strict_errors.sh` 0. All of it again after merging refs/heads/modern 83890c6a
    (4.1-4.3): the same results.
23. **3.4** Retire the i386 build as the reference: re-record the oracle from the x86-64 build. First make virtual time independent of code shape, so arm64 matches too. Today the 64-bit timing drifts by a frame or two after scene changes.

    **Done (2026-10-08).** The i386 and x86-64 builds now give the same output - every screenshot, the sound, the
    save files and the register traffic frame for frame - in the quick, play, town, save and continue scripts, and
    the x86-64 build is the reference (captures in `~/acww/ref/oracle/captures_v2`). What was found and done:
    - **The drift was not timing.** `ACWW_IOTRACE` (new: per frame the reads, writes, a hash of every access's
      address and value, the clock; `ACWW_IOTRACE_FRAMES` lists each access with its caller) showed the two builds'
      register traffic identical, read values and clock included, up to frame 1467 of the play script. There the
      first differing access led, through gdb at that cycle in both builds, to a 64-bit bug; fixed, the next
      difference to the next one. Seven 64-bit bugs, all DS layouts read on 64-bit hosts (commits 1ce5cf3c,
      bc50ae1f): the collision query's wall list written through a DS-layout view (no wall was ever found: NPCs
      walked through cliffs), the joint-blend callbacks reading the render state as a model (the NPCs' blended
      poses), `func_01ffcc10` reading `recJntAnm` at the DS offset, the talk request reaching the NPC's voice
      (Kapp'n had no voice) and action control at DS offsets, `HandOverItem` / the bed check / a villager's put-away
      reading `profile` and a frame control at DS offsets, the particle resource view `ResB` (a child emitter's
      texture size: the taxi's exhaust), and ov004's `PlayerActor_Setup*` reading the action request's payload at
      the DS offset 0xc (the walk out of a building aimed elsewhere). With the first five fixed, the builds'
      traffic was identical through all 25000 frames of the play script under the old timing already; the access
      counting only turned every behaviour difference into a time shift that moved everything after it. Each fix
      leaves the i386 build unchanged (oracle quick and play identical to the old captures under
      `ACWW_TIMING=legacy`).
    - **Virtual time from what the game observes** (hw.c `io_read_cost`, docs/host.md "Virtual time"): measured
      first what the game uses time for (`ACWW_IOHIST`, play script, legacy time: 6.68 million register reads, of
      them 0.6 % clock reads - VCOUNT 41,296 for the HBlank handler and the VRAM upload queues, DISPSTAT 4,
      `OS_GetTick` 77 for the 40 ms scene-creation budget, two loaders, a blink and a timeout; the interrupt
      master enable alone 29 %). Now a clock read (DISPSTAT / VCOUNT, a timer's counter) takes 64 cycles, a polling
      read (the fifth and later read of one register with no other access between) 4096, `OS_SpinWait` /
      `SVC_WaitByLoop` their length (they took none before; `SND_RecvCommandReply`'s wait needs it), every other
      access nothing; halts and interrupt waits jump to the next event as before. Before -> after on the play
      script: every one of 6.68 million reads timed -> 43,425 clock reads and 1,595 polling reads (in 15 frames, all
      `G3X_ClearFifo` waiting a frame for melonDS's geometry engine at a scene start), no spin without time
      (a million timeless accesses would be reported and skipped to the next event: never happened). The game now
      reaches its VBlank wait in every frame, loads instantly (the title at frame 250, legacy 1550, the DS ~900)
      and, its random seeds coming from the clock, makes a different town in the new-game scripts.
      `ACWW_TIMING=legacy` keeps the old model for comparing with the old captures.
    - **Evidence**: the i386 and x86-64 `ACWW_IOTRACE` files are byte-identical for play (25002 frames), save (35000)
      and continue (8000); quick and play screenshots and the whole quick WAV identical; the save script's 70 and the
      continue's 32 screenshots identical, and the save file identical after the save and after the continue. arm64
      is not built yet; nothing in the model depends on the architecture (no host time, no access counts).
    - **Oracle**: `tools/host_oracle.sh <binary> quick|play|record` (CAPDIR, default
      `~/acww/ref/oracle/captures_v2`, recorded from the x86-64 build of this step; `record` never writes
      `captures/`). The old captures and `~/acww/ref/oracle/oracle.sh` are untouched; they still match the i386 and
      x86-64 builds under `ACWW_TIMING=legacy`. To switch every check to the new reference: run
      `tools/host_oracle.sh` instead of `oracle.sh` (or point oracle.sh at the new directory:
      `mv ~/acww/ref/oracle/captures ~/acww/ref/oracle/captures_i386_legacy && ln -s captures_v2
      ~/acww/ref/oracle/captures`).
    - **One script per scenario** for both builds: `tools/host_play_script.py play|town|save|continue` (prints
      `ACWW_KEYS` / `ACWW_TOUCH`); the separate x86-64 town script is gone. The new-game town differs from the
      legacy one, so the walk to the house was found anew (`ACWW_POSLOG`, the town map, `Town_FindPlayerHouse`
      from gdb; docs/host.md). The clock stays pinned (the headless default).

### Phase 4: platform layer and frontend (Linux first, about 1-2 weeks)
24. **4.1** In-process SDL frontend (window, input, audio callback on the ring) for 64-bit builds; drop `shm_open`/`fork`/`execl`. The viewer stays only for i386, if i386 is kept at all.
25. **4.2** `modern/host/plat.h` with POSIX and Win32 implementations: monotonic time, sleep-until, read the ROM file into memory (instead of `mmap`), local time, stack allocation, optional backtrace. `typeof` → C23 `__typeof__` or an explicit type.
26. **4.3** Coroutines on Windows x64 (an asm variant or Fibers). The arm64 path of coro_switch.S has never run.

**Steps 4.1-4.3 done (2026-10-08).** What was done and measured:
- **4.1** `modern/host/frontend_sdl.c` (`HOST_FRONTEND_SDL`, the x86-64 build; `configure_host.py` links the system's
  `libSDL2-2.0.so.0`, headers from `--sdl2`): window with both screens (letterboxed, F11 1x/2x/3x, Alt+Enter full
  screen), keyboard (scancodes), game controllers (positional face buttons, D-pad / left stick, shoulders and
  triggers), mouse and finger on the bottom screen as the touch panel, sound by an SDL audio callback that reads the
  AcwwShm ring (the viewer's rate control, with acquire/release on the ring's indices). The SDL calls run on an 8 MB
  stack of their own (a coroutine the VBlank switches to and back), not on the game thread that happens to take the
  VBlank. `frontend.c` keeps the common part (the frame published, ACWW_KEYS, the keys and touch read back);
  `frontend_viewer.c` (shm_open / fork / execl) is compiled only without `HOST_FRONTEND_SDL`, i.e. for i386, which
  also is the only build that makes `acww-viewer`. `ACWW_HEADLESS` opens no backend at all, as before. Checked with
  `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy` (and `disk`): `ACWW_WINSHOT` (the window's drawable read back before the
  present) at frames 2500 and 3000 is the `ACWW_SHOT` screenshot scaled 2x pixel for pixel (512x768, 0 differing
  pixels); with `ACWW_RTC` pinned to the headless default, a window run's screenshots and WAV are identical to a
  headless run's (window runs otherwise use the host's clock, so they differ from headless ones by design).
  The same with `SDL_VIDEODRIVER=offscreen`. Sound: a paced window run to frame 1800 with `SDL_AUDIODRIVER=disk`: the
  device's output has the WAV's loudness envelope (logo jingle, then the title music), about 1 s later (the device
  plays from start-up on), and 0.1 s windows of it correlate with the WAV at 0.98-0.998 where the music doesn't
  repeat. Not checked here (no display, no controller): real keyboard, mouse, finger and controller input, F11 /
  Alt+Enter, a GL renderer.
  One window run (host clock, the first of 31) crashed at frame 1434 in `FieldVillagerAi_ChangeState` (ov068, a
  villager's state change in the attract demo, null address); 30 more host-clock runs (12 headless with the pre-change
  build, 18 in a window) and every run with a pinned clock went through. **Explained and fixed (2026-10-08)**: a
  read past the end of the field villagers' state table (split into three variables), date-dependent; see 2.8's
  reads past an end.
- **4.2** `plat.h` grew: `host_plat_localtime`, `host_plat_stack_alloc` / `_free` (guard page below the stack;
  `HOST_PLAT_LOW4G` = MAP_32BIT on x86-64 Linux, gone with 3.3), `host_plat_read_file` (plat_common.c, stdio: the ROM is read into
  memory instead of `mmap`), `host_plat_backtrace` / `host_plat_print_backtrace` (execinfo on glibc/macOS,
  `CaptureStackBackTrace` on Windows). POSIX and Win32 implementations (plat_win32.c still uncompiled: no Windows
  toolchain here). Users switched: card.c (no mmap/open/fstat), pxi7.c (`localtime_r` → `host_plat_localtime`,
  `_exit` → C99 `_Exit`, `typeof` → named structs), os.c (stacks), debug_posix.c (backtrace), hw.c (`_Exit`).
  Counts: `typeof` in modern/host 2 → **0**; files in modern/host including POSIX headers (`unistd.h`, `sys/mman.h`,
  `fcntl.h`, `execinfo.h`, `pthread.h`...) 9 → 6, and those 6 are the expected ones: plat_posix.c, debug_posix.c and
  guards.c (POSIX-only debugging aids), frontend_viewer.c (i386 only), hw.c and main.c (the memory setup: memfd and
  fixed mappings, which 3.3 removes).
- **4.3** coro_switch.S has an x86-64 Windows variant: rbx rbp rdi rsi r12-r15 and xmm6-15 saved, and the TIB's
  StackBase / StackLimit / DeallocationStack (gs:0x08, 0x10, 0x1478) saved and switched with the stack (the system
  checks the stack pointer against them in exception dispatch, unwinding and `__chkstk`); a new stack's first entry
  leaves the 32-byte shadow space and the 16-byte alignment the convention wants. coro.c builds its 256-byte frame.
  Chosen over Fibers because it keeps one mechanism (and `host_dump_threads`' frame walk) on every host and needs no
  fiber per DS thread. Test: `modern/host/tests/coro_win64_test.c` (`ninja -f build_host64.ninja coro-win64-test`)
  assembles the same code on Linux with `HOST_CORO_WIN64_TEST` (the TIB through a pointer instead of gs) and calls it
  with GCC's `ms_abi`: every callee-saved register of the Microsoft convention survives a switch while the other side
  overwrites them all, the coroutines get their argument, an aligned stack and their shadow space inside their stack,
  the TIB fields follow the running stack and come back for the thread's own, 2001 switches with doubles live. Each of
  five deliberate breakages (no xmm15 restore, no StackBase restore, 16 instead of 32 bytes of shadow space, rdi or
  rsi not saved) makes it fail. Not checked: the real gs-relative TIB access and Windows' own use of the fields.
- Checks: i386 oracle quick (pictures and sound) and play identical to the captures; x86-64 quick-raw and play-raw
  identical to the pre-change build's run (every screenshot and the WAV byte for byte); `host_strict_errors.sh` 0;
  `coro-win64-test` passes.
27. **4.4** Cross builds checked on this machine before any hardware:
    - **aarch64-linux** with a cross gcc/clang plus `qemu-aarch64` user mode (downloadable without sudo). This exercises arm64 division, PMF encoding, the coroutine switch, memory above 4 GB and unsigned-char defaults, and runs the oracle.
    - **x86_64-w64-mingw32** with llvm-mingw plus Wine (portable tarball). This exercises LLP64, MS bitfields (with `-mno-ms-bitfields`), the Win32 platform layer and the coroutine switch.
    - **macOS:** the remaining differences from aarch64-linux are Mach-O/ld64, the `_` symbol prefix in coro_switch.S (already handled), Apple's signed char and `va_list`, and constructor ordering. A CI runner (GitHub Actions macos-14 is arm64) can build and run the headless oracle before anyone owns a Mac.

**Step 4.4, aarch64-linux part: done (2026-10-08).** What was done and measured:
- **Build**: `configure_host.py --arch aarch64` (`build_host_a64.ninja`, `build/hosta64_usa`): ARM's GCC 14.3
  cross toolchain with its glibc sysroot (`--cross`), the game with `-fsigned-char` (kept) and `-ffp-contract=off`
  (GCC contracts a * b + c into FMAs on arm64 by default; x86-64 has no FMA without -mfma), the runtime likewise,
  PIE. melonDS's core cross-built in `~/acww/ref/melonDS/build-core-a64` with `tools/host/aarch64-linux.toolchain.cmake`
  (JIT and GDB stub off, `-ffp-contract=off`; the command in docs/host.md). No SDL2 for arm64: a 64-bit build without
  SDL2 headers gets `modern/host/frontend_none.c` (`HOST_FRONTEND_NONE`, headless). Every unit compiles without
  errors with GCC 14 (the x86 builds use GCC 15); `--cc clang` is refused for aarch64 for now (llvm-mingw's clang with
  `--target=aarch64-linux-gnu` and the toolchain's sysroot would be the way).
- **Running**: qemu-aarch64 user mode, `-L <toolchain>/aarch64-none-linux-gnu/libc`; `tools/host_oracle.sh` runs the
  binary through `$ACWW_RUNNER` (new), with `QUICK_TIMEOUT` / `PLAY_TIMEOUT` raised. 15-35 frames a second: quick
  ~10 minutes, play ~28-33, save + continue ~40-55.
- **What arm64 exposed in the sources: nothing that changes behaviour.** The coroutine switch's arm64 path (never run
  before) was right as written: the game's threads run on it, and `coro-a64-test` (`modern/host/tests/coro_a64_test.c`,
  `ninja -f build_host_a64.ninja coro-a64-test`, under qemu) checks it alone: x19-x29 and d8-d15 survive a switch
  while the other side overwrites them all, new coroutines get their argument and an aligned stack inside their
  memory, 2001 switches with doubles live; each of four deliberate breakages (no d14/d15 restore, no x27/x28 restore,
  x19 not saved, d9 not restored) makes it fail. Division (`DS_DIV`/`DS_MOD` since phase 1), PMFs (the compiler's
  since 2.7: the ARM encoding just works), struct returns (typed calls since 2.10: x8 as the result register makes no
  difference) and char signedness (`-fsigned-char`; the runtime compiled for arm64 with `-Wtype-limits
  -Wchar-subscripts` shows no char compared as signed) needed nothing. Changed for arm64 only: debug_posix.c reads
  the arm64 registers (pc, sp, x29) for its crash report and keeps `ACWW_WATCH` (the trap flag) x86-only;
  `host_panic` prints only the format on Linux arm64 (a `void *` can't be a `va_list` there; `Fatal_PanicV` has no
  caller).
- **Evidence** (after merging modern at f96f0c6c): `tools/host_oracle.sh build/hosta64_usa/acww quick` and `play`
  under qemu: every line "identical" against captures_v2 (both screenshots and the 17.5 MB WAV; the 7 play
  screenshots). The save and continue scripts (`tools/host_play_script.py`): the 70 + 16 screenshots (every 500
  frames), the save file after the save and after the continue, and the `ACWW_IOTRACE` files (35002 and 8002
  lines: register traffic and values per frame) byte-identical to the x86-64 build's.
- **A difference on the way, explained**: before modern's sanitizer sweep (f92e867d) was merged, the arm64 save and
  continue runs had the same screenshots and save files as x86-64 but register traffic that parted at frame 24245
  (save: going into the house; extra PXI sound commands) and 3425 (continue: one more `FX_Sqrt` from
  `Snd_CalcListenerDistance` on x86-64, a sound effect's emitter); quick and play were identical. With the sweep's
  fixes (reads past ends, uninitialised and DS-sized storage, the payloads read at DS offsets - layout-dependent
  reads that give different garbage on each architecture) both builds agree. Not narrowed to the single fix: not
  `Bgm_GetSceneBgmId`'s table end and not the PlayerActor files (each reverted alone on arm64: still equal to x86-64);
  the rest of that commit's source fixes depend on its header changes.

**Step 4.4, Windows part: done (2026-10-09).** What was done and measured:
- **Build**: `configure_host.py --arch x86_64 --os windows` (`build_host_win64.ninja`, `build/hostwin64_usa/acww.exe`):
  llvm-mingw 20261006 (clang 23.1.3, UCRT, libc++, lld; `--mingw`), linked `-static`, `-Wl,--stack,8388608`; the game
  and the runtime's C with `-mno-ms-bitfields`, no PIE flag (lld's PE default: relocatable, high-entropy ASLR, so the
  image is at 0x140000000 and above 4 GB). melonDS's core cross-built in `~/acww/ref/melonDS/build-core-win64`
  (`tools/host/win64-mingw.toolchain.cmake`; JIT and GDB stub off). Headless only (modern's `frontend_none.c`). All
  865 game units and the runtime compile with no errors; `plat_win32.c` compiled for the first time with no change
  needed.
- **Running**: Wine 10.0, `tools/host_wine.sh` (new: no display, `WINEDEBUG=-all`, its own prefix) as
  `ACWW_RUNNER` of `tools/host_oracle.sh`. Speed as on Linux.
- **What Windows exposed** (each fixed in the source):
  - *Link*: the game's MSL `__strtold` clashed with libmingwex's (renamed `ds___strtold` in
    include/host_libc_rename.h); a scan of every global the program defines against the toolchain's libraries found
    two more the game defines and Windows' libraries have (`_strlwr`, `closesocket`: renamed too). `Proc_Create`'s
    vtable check used the ELF linker symbols `__executable_start` / `_end`: now `host_in_program_image()` (plat.h:
    `host_plat_image_bounds`, ELF symbols on POSIX, the PE header's `SizeOfImage` on Windows).
  - *COFF alignment*: static objects can't be aligned beyond 8 KB (clang: "requested alignment must be 8192 bytes or
    smaller" for DTCM 16 KB, ITCM 32 KB, main RAM 2 MB, the VRAM banks and windows up to 512 KB). With
    `HOST_MEM_RUNTIME_ALIGN` (Windows) those blocks are allocated aligned at start-up (`HostMem_Init`,
    `HostMem_AlignedBlock`) and their names stand for the allocations (`#define gHostDtcm (*gHostDtcmBlock)`: `sizeof`
    still gives the size); elsewhere they stay static arrays.
  - *Arguments in memory*: the Microsoft x64 convention passes the 5th and later arguments in stack slots whose bytes
    beyond the argument's own size are undefined, where System V passes up to 6 in registers (extended by the
    caller). So a call through a declaration with a narrower or 32-bit parameter where the definition reads a wider
    one gets garbage there. Found: `InsectSpawn_FindUnitInBlock` (ov003, declared `u8 flag` 6th, defined `s32 mode`:
    the attract demo's insects never spawned, the first visible divergence - a villager turned towards a missing
    insect at frame 366 - found with the `ACWW_IOTRACE` frame hashes, then the arguments of the diverging `Vec_DistXZ`
    call and a watch on the insect table between register accesses); `G3i_PerspectiveW_` (src/main/unk_020027b4.cpp,
    `u32` for the 8th, an `s32 *` matrix: crashed at start-up writing through 0x100000000); `TP_CalcCalibrateParam`
    (u8 for three u16 screen points, 5th, 8th, 9th); `CARDi_RequestStreamCommand`, `ItemPick_FromRange` and
    `gpGetInfo` (s32 for pointers in slots 5-9). The check (a scratch libclang script over every call: the callee's
    declaration as the calling unit sees it against the definition's parameter types from `tools/host_decls.py`,
    flagging slots 5+ where the declared type is narrower than the defined one) now reports 0. Calls through function
    pointers are not covered by it.
  - *LLP64*: `__ptmf_null` (src/autoload_2/unk_0213a740.c) was two `long`s, 8 bytes on Windows, read as a 16-byte
    member pointer: now two pointer-sized words. The runtime's `%lx` prints of addresses take `unsigned long long`.
    The game's own `long`s (196 uses) are 32-bit on Windows as on the DS; the DWARF layouts of every named struct in
    every unit are the same in the Linux x86-64 and the Windows objects (objdump --dwarf, compared per unit).
  - *Found on the way, every build*: the MSL `scalbn` (src/autoload_2/unk_02128030.c) was decompiled as `s64
    scalbn(s64, s32)` (a double and an s64 travel in the same registers on the DS) while its caller, `pow`, passes a
    double: now `double scalbn(double, s32)` (only `pow`'s subnormal results reach it).
  - *Text files*: the runtime's `ACWW_IOTRACE`, `ACWW_RAMHASH` and `ACWW_IOHIST` files are written in binary mode (the
    UCRT's text mode wrote CR LF).
  - *Not needed*: wchar_t is 16-bit on Windows anyway (`-fshort-wchar` kept, harmless); printf's formats (UCRT
    supports `%llu`, `%zu`); paths (Unix paths reach the program as they are and Wine maps them; the program never
    splits them); stacks (`host_plat_stack_alloc` commits the whole stack with a no-access page below and the switch
    points the TIB's StackBase / StackLimit / DeallocationStack at it, so `__chkstk`'s probes and exception dispatch
    see the right bounds; TLS is reached through the TEB, which a stack switch doesn't touch).
- **Evidence** (after merging modern at 69bab3b0): `ACWW_RUNNER=tools/host_wine.sh tools/host_oracle.sh
  build/hostwin64_usa/acww.exe quick` and `play`: every line "identical" against captures_v2 (both screenshots and
  the 17.5 MB WAV; the 7 play screenshots). The save and continue scripts: the 70 + 16 screenshots (every 500
  frames), the save file after the save and after the continue, and the `ACWW_IOTRACE` files (35002 and 8002 lines)
  byte-identical to the x86-64 build's. The Linux builds with the same sources: i386 and x86-64 `host_oracle.sh` quick
  and play identical against captures_v2 (and the i386 build's quick under `ACWW_TIMING=legacy` against the old
  captures), `host_strict_errors.sh` 0, `coro-win64-test` passes.
- **Left for Windows**: a frontend (SDL2 for Windows, or SDL3), a run on real Windows (Wine's TEB and exception
  handling are close to Windows' but not the same), unwind information (`.pdata`) for coro_switch.S so a debugger's
  stack walk crosses a coroutine's entry, and crash reports (`HOST_DEBUG_POSIX` is off: no handler yet, a fault ends
  the process).

### Phase 5: renderer (independent, can start any time)
28. **5.1** Own 2D compositor behind the 8-function interface (BG text/affine/extended modes, OBJ, windows, blending, master brightness), compared per frame with melonDS in the same process (both fed the same register/VRAM stream).
    **Done (2026-10-09).** modern/host/gpu2d/ (C, about 1100 lines with comments, written from the hardware's
    documented behaviour, no melonDS code), behind modern/host/render.h: `ACWW_RENDERER=own|melon` (default still
    melon), `ACWW_RENDER_COMPARE=1`, `ACWW_RENDER_SELFTEST=<n>` (modern/host/render.c, render_selftest.c; docs/host.md).
    melonDS now runs only the 3D engine for the own renderer (its 2D still runs, unused); its 3D picture is copied
    before its frame (`melon_3d_lines`: melonDS renders the next frame's 3D during the frame) and composited as BG0
    with BG0HOFS's scroll. What the renderer does: BG modes 0-6 (text 16/256 colours with extended palettes, affine,
    extended affine with 16-bit tiles, 256-colour and direct-colour bitmaps, the large bitmap), BG mosaic, OBJs
    (normal, affine, double size, 1D/2D tile and bitmap mapping, 256 colours, extended palettes, semi-transparent,
    bitmap alpha, OBJ window, OBJ mosaic with its line counter), windows 0/1/OBJ (the per-line vertical and
    horizontal "inside" flags, which carry over lines and frames), alpha blending, brightness up/down, the 3D layer's
    own alpha, display modes off/normal/VRAM, master brightness, POWCNT1's engine B switch and screen swap, VRAM
    banking with the small banks' mirrors and OR-ed overlaps. Timing as melonDS has it in this host: OBJs are
    prepared a line ahead (line 0's at the previous frame's end, under the previous frame's OAM and registers; their
    palette colours are looked up when the line is shown).
    **Checks (x86-64):** with `ACWW_RENDERER=own`, `tools/host_oracle.sh build/host64_usa/acww quick` (both
    screenshots and the WAV) and `play` (all 7) identical to captures_v2; `ACWW_RENDER_COMPARE=1` finds **no differing
    pixel in any frame**: quick 8002 frames, play 25002, the save script 35002 (covers the town script, whose inputs
    are its prefix; 70 screenshots and the save file identical to the melonDS build's), continue 8002 (16
    screenshots identical), the town script on its own 23002. The i386 build with `ACWW_RENDERER=own`: quick and play
    identical to captures_v2, 8002 and 25002 frames compared, 0 differ. With the default (melon) both builds' quick
    and play are identical as before; `tools/host_strict_errors.sh` 0. The self-test (random
    VRAM, palettes, OAM, VRAMCNT and registers for both engines; 1000 scenes, 2 frames each) matches melonDS
    everywhere. The game itself uses: BG modes 0 and 1, text BGs (16 colours), engine B's affine BG, the 3D layer,
    tiled OBJs (2D mapping, double size), window 0 and 1, the OBJ window, alpha blending, brightness up/down, display
    off; everything else is checked by the self-test only.
    **Where it differs from melonDS (the self-test leaves these out; the game never does them):** 16-bit writes to
    BGxX/BGxY's high half with bits 12-15 not a copy of bit 11 (melonDS keeps them, the DS has 28 bits); alpha
    blending of the backdrop as 1st target where nothing is above it (melonDS blends with a stale 2nd layer); engine
    A's extended OBJ palette in both F and G (melonDS shows something else than the OR); OBJ shape 3 (undefined;
    melonDS draws 8x8); melonDS's first two frames after start show engine B's BGs wrong (its caches). **Not done:**
    display capture (not used: `ACWW_GPU2D_FEATURES` reports it), main memory display mode (reported, black), the
    OBJ rendering-time limit per line (melonDS has none either), HBlank per-line effects (step 5.3). melonDS stays the
    default until the renderer is wanted for 5.2/5.3: switching the default changes no picture.
29. **5.2** Own 3D geometry engine (matrix stacks, lighting, polygon setup, the synchronous read-backs: GXSTAT, position/vector tests, RAM counts), then a software rasteriser matching melonDS's output; later a GPU (WebGPU-class) backend.
    **In progress (branch port-p52-gpu3d, 2026-10-10).** `modern/host/gpu3d/` behind `ACWW_RENDERER3D=own|melon`
    (gpu3d_geom.c the geometry engine, gpu3d_raster.c the rasteriser, gpu3d_tex.c texture slots and the 7 formats).
    With `ACWW_RENDERER3D=own` the game renders without melonDS (town, Kapp'n's taxi: visually identical). Fitted to
    melonDS's output by experiments (the rules are in the code's comments): clipped vertex colours (5 bits, from the larger colour
    by a factor rounded up to 16 bits, the result rounded up; an edge crossing both planes of an axis gets its second crossing from its far end); the viewport transform
    shifts W right once when it has more than 16 bits; lighting (diffuse: per-component products of the reversed light and the normal, 9-bit level, no upper
    limit; all terms summed with 14 fractional bits); texture coordinates from the normal (>> 24 with the 12-bit
    normal); spans between the edges' x at the edge's outermost line (x-major edges rounded, y-major left rounded
    down, right the next integer above); colours / texture coordinates / W exact between equal W with bits 0-6
    clear, else factors of 9 bits along edges (W's bit 0 dropped) and 8 along spans; the W-buffer depth is the scaled
    W (z) interpolated the same way; equal depth lets a facing-1 polygon over a facing-0 pixel; GBATEK's "Polygon
    Size" (right edges and x-major edge runs drawn for translucent polygons with blending and for opaque ones with
    edge marking or anti-aliasing); textures (modulation, decal), alpha test, translucent blending once per polygon
    ID (the larger alpha kept); fog (GBATEK's formula, density from depth >> 9); anti-aliasing (coverage per edge pixel:
    y-major edges by the pixel's area inside the edge, x-major edges by the exact edge's height at the pixel centre,
    the left edge first; the opaque top pixel keeps its coverage and the pixel below it and is blended with it at
    the end; translucent pixels in front resolve the edge first, behind ones go onto the layer below). DISP3DCNT writes now replace its
    bits (they were ORed in). **Checks:** `ACWW_RENDER_SELFTEST3D=<n>` with `ACWW_RENDER_SELFTEST3D_MASK`
    (render_selftest3d.c; new: 256 fog, 4096 lighting, 8192 a W per vertex, 32 a texture naming its texels) and
    `_POLYS`; at 300 scenes: masks 0, 1, 4, 8, 16, 17, 24, 32, 264 (fog, W-buffer) match, 2 / 6 about 1%, multi-polygon
    Z-buffer and 8192 + 2 / 32 differ by ±1 precision; ACWW_RENDER_COMPARE=1 on the play script to frame 8000: every
    3D read and every unclipped polygon's colours and texture coordinates equal, polygon lists differ in 6 of 25002 frames
    (12 clipped colours next to a saturated colour); a strip polygon hidden by the far plane leaves a 2-vertex
    polygon of its shared vertices (as melonDS). With `ACWW_RENDERER3D=own` the save script's save file is identical
    to the melonDS run's, every 3D read equal in the town, save and continue scripts; the five builds (x86-64, i386,
    clang, aarch64, Windows) give identical pictures and sound with the own renderers. Features the scripts use: no
    toon / highlight, shadows, edge marking, wireframe, 1-dot polygons or rear bitmap, pictures differ along polygon seams (texture
    coordinates / W mid-span ±1 in the 8-bit span factor). Tools: `ACWW_RENDER_SELFTEST3D_CMDS` (raw command scenes;
    `_DISP`, `_CLEAR`), `_QUAD`, `_W`, `_DUMP` (both pictures, the polygon per pixel, both depth buffers, the
    rendering registers), `ACWW_RENDER_DUMP3D=<frame>:<prefix>` (the same for a game frame), `ACWW_RENDER_POLYDUMP`,
    `ACWW_RENDER_FITDUMP` (+ `ACWW_GPU3D_FEATURES=2` for lighting records), `ACWW_RENDER_COMPARE=2` (lights,
    material, normal and vertex colour after every command), `ACWW_GPU3D_NOFOG`; melon_3d_buffers reads melonDS's
    depth and attribute buffers (an oracle only). Anti-aliasing: self-test mask 64 (100 scenes) 15400 differing
    pixels to 681; play script median per frame 2618 to 1286. **Open:** anti-aliasing residuals (±1 coverage on long
    x-major runs, some vertex rows, slivers), span-factor precision at seams, Z-buffer depth (15-bit steps along edges; the game uses
    W-buffering), specular lighting, sliver left/right choice, toon /
    highlight, shadows, edge marking, wireframe, the rear plane bitmap. **Feature audit (2026-10-10):** the game's
    models and code use none of toon / highlight, shadows, edge marking, wireframe or the rear bitmap, so the own
    engine is feature complete for ACWW; the open items above are precision refinements or unused features.
    Polygons without area are drawn (as melonDS): one-line polygons from the leftmost x to the rightmost (excluded,
    one pixel at least), points, lines with two equal vertices (full-size runs). Speed: `ACWW_GPU3D_TIMING=1`
    (rasteriser time per frame, printed every 1000 frames): about 6 ms a frame in the quick script (exact spans step
    instead of dividing, edges set up once per polygon, texture slots flattened once a frame); the remaining cost is
    the per-pixel perspective division (interp_setup). `ACWW_PROFILE`'s file now starts with a function's address
    (`# host_debug_exit_dumps <addr>`), so position-independent samples map through nm.
30. **5.3** Drop melonDS (and the GPL obligation), then HBlank effects (`Sky_HBlank*` stubs). **melonDS dropped
    (2026-10-10):** the own 2D renderer and 3D engine are the default (`ACWW_RENDERER` / `ACWW_RENDERER3D`, `own`;
    the 3D choice follows the 2D one unless set). melonDS (melon.cpp, melon_platform.cpp, its libraries) is linked
    only into the oracle build (`tools/configure_host.py --oracle`: suffix `_oracle`, any --arch), the reference for
    `ACWW_RENDERER=melon`, `ACWW_RENDER_COMPARE` and the render self-tests; every other build compiles
    `melon_none.c` (asking for melonDS stops with a message) and links no GPL code (no melonDS symbol in the
    binary). The bar, chosen with the user: feature complete, not pixel-exact - the own engine's ±1 rounding is
    accepted for now. New captures `~/acww/ref/oracle/captures_v3` recorded from the x86-64 build with the own
    renderers (the quick screenshots equal the own-renderer runs before the switch); `tools/host_oracle.sh` compares
    with them by default (timeouts raised to 300 / 900 s: the own engine is slower than melonDS's). **Checks:**
    i386, clang, aarch64 (quick, qemu), Windows (Wine) and the oracle build identical to captures_v3 (quick and play);
    the oracle build with `ACWW_RENDERER=melon` identical to captures_v2; render self-tests run in the oracle build.
    **H-blank effects (2026-10-10):** the 2D renderer draws both engines line by line with an H-blank hook after
    each visible line, where the host runs the game's H-blank interrupt handler (hw.c `host_hblank`: when DISPSTAT,
    IE and IME enable it; VCOUNT reads as the line; the handler's accesses stay out of the polling state, the
    virtual time and `ACWW_IOTRACE`). BGxX/BGxY writes reload the affine reference point (stepped by PB/PD per line).
    `Sky_HBlankMain` / `Sky_HBlankSub` (ITCM gap code) written in C from the DS code (unk_020b8d9c.cpp): the sky's
    gradient, the clouds' perspective, the blend, the horizon's layers; `MenuScreen_WipeHBlank` (the menus' sawtooth
    edge) and `IrisWipe_HBlank` (the circle of scene changes) now act per line. The once-per-frame HBlank interrupt
    stays (delivering it wakes the threads waiting for an interrupt; without it the sound shifts slightly). Game
    behaviour unchanged: quick sound, save script's save file and register traffic identical; captures_v3
    re-recorded (the previous set: `captures_v3.20261010-220357`). **Open:** the ±1 precision items of 29; OBJs
    are still prepared from the registers at the start of their line (the DS: during the line before).
31. **5.4** WebGPU rendering (wgpu-native; the user's request, 2026-10-10: more efficient, and the way to macOS and
    the web). **Phase 1 done:** `ACWW_RENDERER3D=wgpu` keeps the own geometry engine and draws its polygon list on the
    GPU (modern/host/gpu3d/gpu3d_wgpu.c) into the same frame buffer, read back for the 2D renderer: texture image and
    palette slots as storage buffers, the 7 formats decoded in the fragment shader; modulation / decal, alpha test,
    translucency (premultiplied colour, alpha by max: blending only over non-transparent pixels, as the DS), the
    translucent polygon ID rule (stencil), translucent depth update, depth "equal" (less or equal), Z-buffer (linear
    in screen space) and W-buffer (per pixel), the fog flag (min blend) and a resolve pass with fog and the frame
    buffer's packing. Not done: edge anti-aliasing, lines / points, toon, shadows, edge marking, rear bitmap.
    `configure_host.py --wgpu <dir>` (default for x86-64 Linux: `~/acww/toolchains/wgpu-native-v29.0.1.1/
    linux-x86_64`, the release's static library; `none`: without; other builds compile a stub and fall back to the
    software rasteriser with a message). `ACWW_WGPU_ADAPTER=cpu`: the software adapter (llvmpipe). **Checks:** 2.4 ms
    a frame on the RTX 3060 (the software rasteriser: about 6) with a synchronous readback and the texture slots
    uploaded every frame; not pixel-exact (12% of the 3D pixels differ, 5% visibly: texel boundaries from the GPU's
    float interpolation, edges without anti-aliasing), the pictures look the same; the save script's save file is
    identical (the 3D picture doesn't reach the game logic); the default renderer's output unchanged.
    **Phase 2 done (2026-10-11):** the screens composited and shown by the GPU (modern/host/wgpu_display.c, which
    owns the device gpu3d_wgpu.c draws with). The 2D renderer still draws its layers on the CPU, line by line (the
    H-blank effects stay exact), but leaves the 3D layer out (`Gpu2dFrameInput.defer3d`): per pixel it gives the
    colour where the 3D pixel is transparent, and how the pixel comes out where it is opaque - a colour in front of
    it, a blend with a layer in front of it, or the 3D layer on top with its effect (blend with the 2nd target,
    brightness up / down) - plus per line the master brightness and BG0HOFS (gpu2d.h `gpu2d_deferred`). A fragment
    shader applies that to the 3D picture, which never leaves the GPU (two textures, as the software rasteriser's
    two frame buffers), at the window's resolution, straight into its surface; the SDL frontend hands over the
    window's X11 or Wayland handles (SDL_syswm.h; modern/host/x11shim stands in for the X11 headers) and maps the
    mouse through the letterbox, falling back to an SDL renderer and read-back screens if the GPU can't draw into
    the window. Screenshots (`ACWW_SHOT`, `ACWW_WINSHOT`) read the composited picture back. `ACWW_SCALE3D=<n>`: the 3D
    picture at n times the DS's resolution (the polygons' positions have 4 fractional bits). **Checks:** the GPU's
    compositing gives the same screenshots, bit for bit, as the CPU's compositing of the same 3D picture
    (`ACWW_WGPU_READBACK=1`, frames 2500 and 8000); presented into a real window (X11 through XWayland, the RTX 3060)
    at scales 1 and 2; the 3D engine's time a frame (the CPU's side: texture slots, vertices, the commands) 0.82 ms
    against 2.0 with the read-back, a fast headless run of 6000 frames 39.6 s against 43.5. **Then (2026-10-11):** edge anti-aliasing as 4x MSAA (the
    resolve applies fog per sample and averages the samples weighted by alpha, when DISP3DCNT bit 4 is set;
    `ACWW_MSAA=1`: off); only the changed 4 KB pieces of the texture slots uploaded; `ACWW_SCALE3D` defaults to 2,
    and where a window pixel covers several 3D pixels (a smaller window, the 256x384 screenshots) the compositor
    averages a grid of points in it. The GPU's compositing still equals the CPU's at scale 1 without MSAA; MSAA
    smooths more edges than the DS's anti-aliasing (which leaves edges between polygons alone), so the 1x pictures
    are no closer to the software references (12539 pixels differ at frame 2500, against 12393). **Then
    (2026-10-11): the 2D layers on the GPU** (default with wgpu, `ACWW_GPU2D=0`: off; modern/host/gpu2d.wgsl, embedded by
    tools/host_embed.py). The 2D renderer took 3.8 ms of CPU time a frame (60 a second; the 3D engine 0.65): about
    a quarter of a core, mostly compositing (2.7), BGs (1.15) and OBJs (0.8), the game's H-blank handler 0.28. Now
    the CPU keeps only the state that goes from line to line - the H-blank, the windows' flags, the affine
    reference points, the OBJ mosaic counter, line 0's OBJs (prepared at the end of the previous frame, from its
    VRAM) - and records per line the registers and those values, a new version of the palettes or OAM when an
    H-blank changed them, and the VRAM mapping as offsets into one buffer (banks A-I, a zero granule, the composites
    of overlapping banks; only changed 4 KB pieces uploaded) (gpu2d.h `Gpu2dGpuFrame`). A fragment shader follows
    gpu2d.c / gpu2d_bg.c / gpu2d_obj.c rule for rule per pixel (every BG kind, the 128 OBJs, windows, both
    mosaics - a mosaic OBJ pixel walks back over its run -, effects, display modes, master brightness) into the
    same two words per pixel the CPU makes for the compositor. **Checks:** `ACWW_GPU2D_CHECK=1` draws with both and
    compares every word every frame: the quick script's 8002 frames and the play script's 25002 identical;
    `ACWW_GPU2D_SELFTEST=<n>` does it for render_selftest.c's random scenes (all BG modes, bitmap and affine OBJs,
    mosaic, windows, effects, display modes): 2100 scenes on three seeds identical (and a deliberately broken
    shader is caught). The 2D renderer's CPU time a frame 0.275 ms (3.26 drawing on the CPU); a fast headless run
    of 6000 frames 9.5 s (28.5). **wgpu the default (2026-10-11):** `ACWW_RENDERER3D` defaults to `wgpu` where
    the build has wgpu-native and the machine a hardware adapter (a software one only when asked for: llvmpipe is
    slower than the CPU's renderers), else to `own` without a message; `own` still when comparing with melonDS;
    tools/host_oracle.sh pins `own` (the captures are the CPU renderers', which the GPU's differ from slightly).
32. **Key bindings (2026-10-11, the user's request: "a keybinding ui that can be fully hidden"):**
    modern/host/frontend_ui.c, in the SDL frontend. Nothing shows until F1 (or a controller's Back + Start); then a
    screen over both screens (drawn on the CPU into a 256x384 overlay with a 5x7 font of its own, blended in by
    wgpu_display.c's compositor or the SDL renderer) lists the 12 DS buttons with a key and a controller button
    each; Enter and the next key or button pressed rebinds (a key or button does one thing: taken from another),
    Delete clears, a line resets to the defaults; the game waits while it is open; closing saves `bindings.txt`
    (SDL's preference directory, or `ACWW_BINDINGS`). The left stick and triggers stay D-pad and L/R besides; F1 and
    F11 can't be bound; M mutes only while no DS button has it. **Checks:** scripted (`ACWW_UI_SCRIPT`) in a real
    window (GPU path) and with SDL's offscreen driver (SDL renderer path): rebinding by key and by controller
    button, clearing, the file written and read back, nothing left on the screen after closing.
33. **Local wireless (2026-10-11, the user's request: "can we get multiplayer working?"):** the gate's DS-to-DS play
    between copies of the program. The game's whole wireless stack is the decomp's (the SDK's WM library, ov066's
    session and reliable MP layer, the network layer and CommManager above it); what the DS's ARM7 does - the radio -
    is modern/host/wm7.c, PXI tag 10: it answers the WM requests (power, parent parameters, beacons, scans, the
    connection, the MP sequences, the per-port data) and carries the radio's traffic in UDP datagrams between copies:
    a parent's beacons to every copy on the machine and the LAN (ports 31470-31477) and to the addresses that asked
    for them, the connection handshake, and per MP sequence (mpFreq per frame) the parent's frame to its children and
    each child's answer, as the DS hardware sends them. ov066 keeps its own frame numbers, acknowledgements and resends,
    so late or lost datagrams only slow it. Over the internet the visitor names the host (`ACWW_NET_PEER=host[:port]`)
    and asks it for its beacons; only the host's port has to be reachable. Options in docs/host.md.
    What it took besides wm7.c:
    - **The MAC address and the channels.** The game refuses wireless with the all-zero MAC address the host had in the
      user settings' copy (`func_0211f7e4`), and reads the channels allowed from there. wm7_init puts a MAC address
      (Nintendo's prefix; per copy: made from the machine's name and the save file's path, `ACWW_MAC`, a fixed one for
      headless runs) and channels 1, 7 and 13 there. Nothing else reads them in the scripted runs: captures_v3 stay
      identical.
    - **Pointers to the ARM7 side.** `WMi_SendCommand` passes its arguments as 32-bit words, and four requests put
      addresses into the request (scan buffers, the parent's description): those are ptr32 words now
      (`WM_ARM7_ADDR`, include/host_ptr32.h), which wm7.c resolves. The ARM7's answers carry their message's address
      in the 26-bit PXI word on the DS; wm7.c hands the pointer over with the word (`HostPxi_TakeArm7Pointer`), and
      `WmReceiveFifo` takes it.
    - **64-bit layouts.** The free-buffer message queue of WM_Init was sized for the DS (0x20 + 0x28 bytes): on 64-bit
      hosts it overran `WMArm9Buf *` and the init flag beside it (every request then failed). ov066 used DS offsets
      into host structures in three places (a beacon's description at +0x20, a ring frame's payload at +0x20, the
      member table at +0x28) and passed a beacon pointer through a u32; `WmReceiveFifo` read the connect events'
      fields through WMMsg's view, whose fields after 0x0c move on 64-bit hosts. The callback structures the ARM7 fills
      are the SDK's (WMStartConnectCallback, WMStartMPCallback, the full WMPortRecvCallback), laid out for the host.
    - **32-bit hosts' heap overheads.** The NitroSystem header overheads in unk_020e8558.cpp were doubled on every
      host; on i386, where the heaps keep the DS's sizes, a parent sized with the DS's 0x48 per child heap then held
      one child less: the second player's body heap in a visit was NULL (a crash on arrival). The DS's values on
      32-bit hosts now (64-bit hosts: doubled, with heaps twice the size).
    - **The system reset.** A broken connection makes the game reset the system (`OS_ResetSystem`: back to the title
      on the DS), which spun on the host: `OSi_DoResetSystem` calls `host_reset_system`, which starts the program
      again in place (`host_plat_restart`: exec with the same arguments; headless runs end there).
    **Checks:** two copies on this machine (`tools/host_wireless_test.sh <binary> <save>`: the scripts gate-host and
    gate-visit, `ACWW_TEST_JOB_DONE` for the gate, `ACWW_FAST=<frame>` then real time; both from the save script's
    town): the host opens the gate, the visitor finds the town, travels,
    arrives ("has come to play!"), walks around (seen on the host's screen), goes home through the gate ("is going
    home!"), both carry on - three runs in a row the same, on x86-64 and on i386; the same over `ACWW_NET_PEER` with
    `ACWW_NET_LAN=0`; the host quitting during the visit: the visitor's game reports the broken connection, resets,
    and (a window run) starts again; quick/play identical to captures_v3 (x86-64, i386); the save script's save
    differs only in the Wi-Fi user data made from the MAC address. Not done: Nintendo WFC (ov065: an access point,
    the game's own TCP/IP and SSL, a server - servers that no longer exist; Wiimmfi is a community one), a real DS,
    latency over the internet. A 32-bit and a 64-bit build don't play together (the visitor's game reports a broken
    connection on arrival: something the copies exchange differs between them; not looked into).
34. **The title music (2026-10-29, the user's report: "the title music still is fucked"):** the title screen's music
    is a stream (NNS_SndStrm: IMA-ADPCM, 47.6 kHz, two channels), refilled a block at a time from the stream thread
    when the ARM7's alarm comes at the end of each block. The host fired the alarm at the block's very end, and the
    stream thread refilled the block just played at once (in virtual time); a channel plays 3 samples behind its
    position (the hardware's pipeline, modern/host/snd7/spu.c), so the refill overwrote the block's last 3 samples
    before they were played: a click every block, a 186 Hz buzz over the whole piece. On the DS the alarm reaches the
    ARM9 (PXI, IRQ, thread, decode) far later than 3 samples. snd7/driver.c now delivers an alarm a quarter of its
    period after it is due. **Checks:** offline, the game's own decoder (MakeWaveData, the build's object) fed the
    title stream (from the user's EUR sound_data.sdat; the same stream as USA) block by block through spu.c as the
    game drives it: identical to the whole stream decoded at once and played straight (before: 2 wrong output
    samples of every 176).
35. **Save states across launches (2026-10-29: "save states still dont stay after a restart", "opening save state
    crashes game"):** (a) the program was built with the stack protector (Apple's clang and Ubuntu's GCC default):
    its canary is random per process, and a state from an earlier launch brings back the game threads' stacks with
    that launch's canaries in their frames, so the first such frame to return aborted the app. Built without it now
    (tools/configure_host.py). (b) The arena's fixed address (0x400000000) is beyond what iOS gives an app without
    the extended addressing entitlement (0x3d8000000 at most): it was somewhere else every launch, and every word of
    the state that looked like a pointer into it was moved - a pair of 32-bit values, the second 1 or 2, could be.
    Now the first free of 0x380000000, 0x2b0000000, 0x400000000; the ROM image is at its top (the same place each
    launch; nothing in the state points into it). (c) Pointers into the program still move by its ASLR slide: on
    Mach-O a word in data that points into the code but not at a function's start (LC_FUNCTION_STARTS) is left
    alone (stacks: return addresses, moved). **Checks:** the cross-process test (a coroutine "game" with data, bss,
    an arena list, a function pointer and a ROM pointer saved in one process and loaded in others, the program
    elsewhere each time; the arena at its fixed place and, with all three places taken, elsewhere): all passed.

---

## 7. Notes and side findings

- **Latent `ptr26` bug.** `ptr26` (pxi7.c:72-75) assumes the program image is at 0x08000000. On x86-64 it is at 0x10000000. It only works because everything sent over PXI lives in gdata/gbss/DS RAM. **Gone (3.1):** the pointers are handed over whole (`PXI_PointerWord`).
- **Truncation in `is_reg`.** `is_reg` (sdk.c:35) and `io_check_addr` truncate pointers to 32 bits; this must be fixed before memory goes above 4 GB. **Fixed (2026-10-08):** `HostIo_Read`/`HostIo_Write` take a pointer-sized address (`IO_ADDR_` is `(__UINTPTR_TYPE__)(p)`, which also removes the `unsigned long` cast that step 2.3 lists), hw.c checks the whole value, sdk.c's `is_reg` and `units` keep `uintptr_t`.
- **`_s32_div_f` disagrees with the DS.** sdk.c:199-200 returns 0 for a zero divisor, while the DS routine and hw.c's emulation give the dividend. **Fixed (2026-10-08)** with `DS_DIV`/`DS_UDIV`.
- **Unreachable on today's host.** The `*(u32 *)0x08000000` cartridge check (autoload_2/unk_02113f04.c:31) and the WRAM arena 0x037f8000 point at unmapped memory and must be unreachable today.
- **Exit paths skip `atexit`.** `ACWW_EXIT_FRAME` exits with `_exit(0)` (hw.c:206), so `atexit` handlers such as the `ACWW_IOHIST` dump (hw.c:937) don't run. Use SIGINT or flush explicitly when measuring. **Fixed (2026-10-08):** `host_exit_dumps()` (`ACWW_IOHIST`, `ACWW_PROFILE`) runs at `ACWW_EXIT_FRAME`, on SIGINT and at exit. `ACWW_IOHIST` now counts the `HostIo_*` calls by register and calling address (there are no trapped accesses to count).
- **The Wi-Fi ID depended on the build (fixed 2026-10-08).** With the same inputs, the save files written by the new-game save script differed between builds in 24 bytes of `PlayerData::wifiUserData` (both copies of the town data, 0x1e20-0x1e69 and 0x17e00-0x17e49), while every screenshot was identical and two runs of one build gave identical files. Both builds reached `DWCi_Acc_CreateTempLoginId` at the same virtual cycle (gdb); the difference is the WFC-ID it copies: `DWCi_AUTH_GetNewWiFiInfo` (src/autoload_2/unk_020fedcc.c) adds the *address* of `OS_GetTick` to its seed (a bug in the original SDK code), and on the host that address moves with every build. It now adds the DS's address (0x01ffa6b4), which gives the DS's value in every build. Similar uses of a function's or variable's host address as a number would be found by the low-4 GB harness only if they truncate; this one didn't.
- **Scratch material**: `tools/portcheck/` (dsaddr.py, divs.py; sweep.py became tools/host_portcheck.py)
  - `sweep.py`, `diag_*.json`: libclang per-target diagnostics;
  - `dsaddr.py`: DS literal classifier;
  - `divs.py`, `divsites.txt`: the 116 division sites;
  - `flagexp.sh`, `fx/*.hash`: flag experiments;
  - `cwarn.txt`: the 246 C cast warnings.
