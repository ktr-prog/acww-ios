// Host build: the last object of the game's state in the link (tools/configure_host.py): the game's objects (from
// game_begin.c), then the runtime's that hold the DS's state (the memory, the hardware, the OS, the sound, the
// renderers), then this. Its two variables follow all of those variables in the program's data and bss, so the
// program's state the save states copy (modern/host/savestate.c) is [gHostGameDataBegin, gHostStateDataEnd) and
// [gHostGameBssBegin, gHostStateBssEnd). The frontend, SDL, the C library and the rest come after (or before).
char gHostStateDataEnd[1] = {1};
char gHostStateBssEnd[1];
// (the end of the static zero-initialised ones: Mach-O's __bss, game_begin.c)
static char sStateStaticBssEnd[1] __attribute__((used));
char *host_state_static_bss_end(void) { return sStateStaticBssEnd; }
