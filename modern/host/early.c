// Host build: the start-up work that has to come before the game's static constructors (which may touch DS memory):
// the DS memory blocks allocated and mapped first.
//
// ELF (Linux): the constructor's priority (101) puts it before every other initialiser. Mach-O (macOS, iOS) has no
// priorities across object files: its initialisers run in the order of the objects in the link, so
// tools/configure_host.py links this object first of all there.

#include "host.h"
#include "host_mem.h"
#include "guards.h"

void host_vram_init(void);

__attribute__((constructor(101))) static void early_init(void) {
    HostMem_Init();
    host_map_memory();
    host_vram_init();
    host_guards_init();   // (the guard variant: configure_host.py --guards)
    host_highmem_init();  // ACWW_HIGHMEM: the low 4 GB reserved from here on
}
