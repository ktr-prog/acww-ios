// Host build: save states for testing (modern/host/savestate.c): the whole state of the game and of the runtime's
// DS in memory, taken and put back while the game is held between two frames. They live only as long as the
// process (the program is loaded at a different address every launch: the state's pointers would be wrong).
#ifndef HOST_SAVESTATE_H
#define HOST_SAVESTATE_H

#include <stddef.h>

// memory of the game's state outside the program's data and bss: the page blocks and the game's stacks
// (host_plat_pages_alloc, host_plat_stack_alloc call it); never freed
void host_state_track(void *p, size_t n);
// a block host_state_track was told about that isn't the game's (the frontend's own stack): not copied
void host_state_untrack(void *p);
// a variable inside the state's range that a load leaves as it is (the save memory's file and buffer: the game's
// save on disk isn't rolled back)
void host_state_keep(void *p, size_t n);

// Called on the frontend's side while the game is held (between frames): *gameSp is where the game's coroutine
// stopped. Save copies the state (and *gameSp); load puts it back (and sets *gameSp). 1 if done.
int host_state_save(void *const *gameSp);
int host_state_load(void **gameSp);
int host_state_have(void);  // a state was saved

#endif
