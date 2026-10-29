// Host build: the first of the game's objects in the link (tools/configure_host.py). Its two variables are the
// first of the game's variables in the program's .data and .bss: linkers place the input sections of an output
// section in the order of the input files, so the game's data lies between these and game_end.c's (include/
// host_mem.h: HOST_GAME_DATA / HOST_GAME_BSS; for the debug dumps and HostMem_IsGameRam).
char gHostGameDataBegin[1] = {1};
char gHostGameBssBegin[1];
// (Mach-O puts the zero-initialised variables of file scope (static) in a section of their own, __bss, and the
// external ones in __common: this one marks the start of the game's static ones there; elsewhere it's in .bss with
// the others)
static char sGameStaticBssBegin[1] __attribute__((used));
char *host_game_static_bss_begin(void) { return sGameStaticBssBegin; }
