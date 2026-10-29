// Host build: the DS memory the game reaches by address, as host objects (include/host_mem.h; docs/portability.md
// 3.2). Each block is aligned so that its addresses keep the low bits of the DS's (the block's size or more): code
// that looks at an address's low bits sees the DS's values.

#include <stddef.h>
#include <stdio.h>

#include "host.h"
#include "host_mem.h"
#include "plat.h"

HostSysWork gHostSysWork __attribute__((aligned(0x1000)));
unsigned char gHostPltt[0x800] __attribute__((aligned(0x800)));
unsigned char gHostOam[0x800] __attribute__((aligned(0x800)));
unsigned char gHostWramArena[32] __attribute__((aligned(32)));

// size bytes aligned to align (a power of two), zeroed, never freed
void *HostMem_AlignedBlock(size_t size, size_t align) {
    unsigned char *p = host_plat_pages_alloc(size + align);
    if (!p) host_fatal("no memory for a host memory block");
    return p + (-(uintptr_t)p & (align - 1));
}

#if HOST_MEM_RUNTIME_ALIGN
unsigned char (*gHostDtcmBlock)[0x4000], (*gHostItcmBlock)[0x8000];
unsigned char (*gHostMainRamBlock)[HOST_MAIN_RAM_HI - HOST_MAIN_RAM_LO];
void HostMem_Init(void) {
    gHostDtcmBlock = HostMem_AlignedBlock(sizeof gHostDtcm, 0x4000);
    gHostItcmBlock = HostMem_AlignedBlock(sizeof gHostItcm, 0x8000);
    gHostMainRamBlock = HostMem_AlignedBlock(sizeof gHostMainRam, 0x200000);
}
#else
unsigned char gHostDtcm[0x4000] __attribute__((aligned(0x4000)));
unsigned char gHostItcm[0x8000] __attribute__((aligned(0x8000)));
unsigned char gHostMainRam[HOST_MAIN_RAM_HI - HOST_MAIN_RAM_LO] __attribute__((aligned(0x200000)));
void HostMem_Init(void) {}
#endif

#if __SIZEOF_POINTER__ == 4
// the DS's layout of the system work area (32-bit hosts: the same member sizes)
#define AT(m, a) _Static_assert(offsetof(HostSysWork, m) == (a) - 0x027ff000u, #m)
AT(cardRomHeader, 0x027ffa80);
AT(bootCheckInfo, 0x027ffc00);
AT(resetParameter, 0x027ffc20);
AT(romBaseOffset, 0x027ffc2c);
AT(ctrdgModuleInfo, 0x027ffc30);
AT(vblankCount, 0x027ffc3c);
AT(wmBootBuf, 0x027ffc40);
AT(nvramUserInfo, 0x027ffc80);
AT(excpVector, 0x027ffd9c);
AT(arenaLo, 0x027ffda0);
AT(arenaHi, 0x027ffdc4);
AT(realTimeClock, 0x027ffde8);
AT(romHeader, 0x027ffe00);
AT(pxiHandleChecker, 0x027fff88);
AT(micLastAddress, 0x027fff90);
AT(threadInfoMain, 0x027fffa0);
AT(buttonXY, 0x027fffa8);
AT(touchPanel, 0x027fffaa);
AT(lockIdFlagMain, 0x027fffb0);
AT(vramCLock, 0x027fffc0);
AT(cardLock, 0x027fffe0);
AT(ctrdgLock, 0x027fffe8);
AT(initLock, 0x027ffff0);
AT(mmemCheckerMain, 0x027ffff8);
_Static_assert(sizeof(HostSysWork) == 0x1000, "HostSysWork");
#undef AT
#endif

void *HostMainRam_Ptr(unsigned int a) {
    if (a < HOST_MAIN_RAM_LO || a > HOST_MAIN_RAM_HI) {  // (the end itself: an arena bound)
        fprintf(stderr, "host: main RAM address %08x outside the host's block (%08x-%08x)\n", a, HOST_MAIN_RAM_LO,
                HOST_MAIN_RAM_HI);
        host_fatal("main RAM address");
    }
    return gHostMainRam + (a - HOST_MAIN_RAM_LO);
}

void host_root_heap_bounds(uintptr_t *lo, uintptr_t *hi);  // main.c

static int inside(uintptr_t a, uintptr_t n, const void *lo, size_t size) {
    uintptr_t b = (uintptr_t)lo;
    return a >= b && a - b <= size && n <= size - (a - b);
}

int HostMem_IsGameRam(const void *p, unsigned int n) {
    uintptr_t a = (uintptr_t)p, rlo, rhi;
    host_root_heap_bounds(&rlo, &rhi);
    const char *sblo = host_game_static_bss_begin(), *sbhi = host_game_static_bss_end();
    return inside(a, n, gHostMainRam, sizeof gHostMainRam) || inside(a, n, &gHostSysWork, sizeof gHostSysWork) ||
           inside(a, n, gHostDtcm, sizeof gHostDtcm) || inside(a, n, gHostItcm, sizeof gHostItcm) ||
           inside(a, n, gHostWramArena, sizeof gHostWramArena) ||
           inside(a, n, gHostGameDataBegin, (size_t)(gHostGameDataEnd + 1 - gHostGameDataBegin)) ||
           inside(a, n, gHostGameBssBegin, (size_t)(gHostGameBssEnd + 1 - gHostGameBssBegin)) ||
           (sbhi > sblo && inside(a, n, sblo, (size_t)(sbhi + 1 - sblo))) ||
           (rhi > rlo && inside(a, n, (const void *)rlo, rhi - rlo));
}

// at start-up (hw.c's host_map_memory): the game's variables in the order the link was meant to give them, and the
// host blocks aligned (the loader honours their alignment: a PIE build is loaded at an address aligned to its
// segments' largest alignment)
void HostMem_Check(void) {
    if (!((uintptr_t)gHostGameDataBegin < (uintptr_t)gHostGameDataEnd &&
          (uintptr_t)gHostGameBssBegin < (uintptr_t)gHostGameBssEnd))
        host_fatal("the game's data and bss aren't between modern/link/game_begin.c's and game_end.c's variables");
    if (((uintptr_t)gHostMainRam & 0x1fffff) || ((uintptr_t)gHostItcm & 0x7fff) || ((uintptr_t)gHostDtcm & 0x3fff) ||
        ((uintptr_t)&gHostSysWork & 0xfff) || ((uintptr_t)gHostPltt & 0x7ff) || ((uintptr_t)gHostOam & 0x7ff))
        host_fatal("a host memory block isn't aligned");
}
