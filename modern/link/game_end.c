// Host build: the last of the game's objects in the link (tools/configure_host.py; see game_begin.c). Its two
// variables follow the last of the game's variables in the program's .data and .bss.
char gHostGameDataEnd[1] = {1};
char gHostGameBssEnd[1];
// (the end of the game's static zero-initialised variables: game_begin.c)
static char sGameStaticBssEnd[1] __attribute__((used));
char *host_game_static_bss_end(void) { return sGameStaticBssEnd; }
