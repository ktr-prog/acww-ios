#ifndef HOST_NOTE_H
#define HOST_NOTE_H

// Host build: a line in the log from the game's code (modern/host/hw.c): "game: <what> a b c (frame n)". At most
// 400 lines a run, so a note in a per-frame path can't flood the log. No-op on the DS.
#ifdef __cplusplus
extern "C" {
#endif
#ifdef HOST
void host_note(const char *what, int a, int b, int c);
#else
#define host_note(what, a, b, c) ((void)0)
#endif
#ifdef __cplusplus
}
#endif

#endif
