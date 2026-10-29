// Host build: save states (modern/host/savestate.c): the whole state of the game and of the runtime's DS in memory,
// taken and put back while the game is held between two frames. Each save also goes to a file (acww-state.bin in the
// working directory: on iOS the app's folder), so it loads after the app was closed too.
#ifndef HOST_SAVESTATE_H
#define HOST_SAVESTATE_H

#include <stddef.h>

// memory of the game's state outside the program's data and bss: the page blocks and the game's stacks
// (host_plat_pages_alloc, host_plat_stack_alloc call it); never freed
void host_state_track(void *p, size_t n);
// a block host_state_track was told about that isn't the game's (the frontend's own stack): not copied
void host_state_untrack(void *p);
// a variable inside the state's range that a load leaves as it is: one that holds this launch's own things (the save
// memory's file and buffer - the game's save on disk isn't rolled back -, the frontend's block, the ROM image)
void host_state_keep(void *p, size_t n);
// memory the state may point into that isn't part of it (the ROM image): pointers into it follow it to another launch
void host_state_foreign(const void *p, size_t n);

// (a file-scope variable kept, registered at start-up)
#define HOST_STATE_KEEP(var) \
    static void __attribute__((constructor)) host_state_keep_##var(void) { host_state_keep(&(var), sizeof(var)); }

// Called on the frontend's side while the game is held (between frames): *gameSp is where the game's coroutine
// stopped. Save copies the state (and *gameSp) and writes the file; load puts it back (from the file if this launch
// has none) and sets *gameSp. 1 if done; host_state_message says what happened, for the player (capitals).
int host_state_save(void *const *gameSp);
int host_state_load(void **gameSp);
int host_state_have(void);  // a state was saved (in this launch, or its file is there)
const char *host_state_message(void);

#endif
