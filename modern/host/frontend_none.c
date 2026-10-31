// Host build: no frontend backend (frontend.h; HOST_FRONTEND_NONE: a 64-bit build without SDL2, e.g. the aarch64
// cross build): every run is headless, as with ACWW_HEADLESS (ACWW_SHOT, ACWW_WAV, ACWW_KEYS and ACWW_TOUCH work as
// in every build).
#if HOST_FRONTEND_NONE

#include <stdio.h>

#include "frontend.h"

AcwwShm *frontend_backend_open(const char *argv0) {
    (void)argv0;
    fprintf(stderr, "host: built without a frontend (no SDL2): headless\n");
    return 0;
}

void frontend_backend_frame(AcwwShm *shm) { (void)shm; }

void frontend_backend_close(void) {}

void frontend_backend_turn(void) {}
#endif
