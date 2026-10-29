// Host build: where the mixed sound goes: the frontend's audio ring (frontend.h) and, with ACWW_WAV=path, a WAV file
// (for headless runs).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../frontend.h"
#include "snd7.h"
#include "../savestate.h"

AcwwShm *frontend_shm(void);

static FILE *sWav;
static u32 sWavFrames;
static int sWavInit;
HOST_STATE_KEEP(sWav)
HOST_STATE_KEEP(sWavFrames)
HOST_STATE_KEEP(sWavInit)

static void wav_header(void) {
    u32 data = sWavFrames * 4;
    u8 h[44];
    memcpy(h, "RIFF", 4);
    *(u32 *)(h + 4) = 36 + data;
    memcpy(h + 8, "WAVEfmt ", 8);
    *(u32 *)(h + 16) = 16;
    *(u16 *)(h + 20) = 1;  // PCM
    *(u16 *)(h + 22) = 2;
    *(u32 *)(h + 24) = ACWW_AUDIO_RATE;
    *(u32 *)(h + 28) = ACWW_AUDIO_RATE * 4;
    *(u16 *)(h + 32) = 4;
    *(u16 *)(h + 34) = 16;
    memcpy(h + 36, "data", 4);
    *(u32 *)(h + 40) = data;
    fseek(sWav, 0, SEEK_SET);
    fwrite(h, 1, 44, sWav);
    fseek(sWav, 0, SEEK_END);
}

void snd7_output_close(void);
static void wav_close(void) { snd7_output_close(); }
void snd7_output_close(void) {
    if (!sWav) return;
    wav_header();
    fclose(sWav);
    sWav = 0;
}

void snd7_output(const s16 *lr, u32 frames) {
    if (!sWavInit) {
        sWavInit = 1;
        const char *p = getenv("ACWW_WAV");
        if (p && (sWav = fopen(p, "wb"))) {
            wav_header();
            atexit(wav_close);  // (and from the interrupt handler, which leaves with _exit or _Exit)
        }
    }
    if (sWav) {
        fwrite(lr, 4, frames, sWav);
        sWavFrames += frames;
        if ((sWavFrames & 0x7fff) < frames) wav_header();  // keep the file valid if the run is cut short
    }
    AcwwShm *shm = frontend_shm();
    if (!shm) return;
    u32 w = shm->audio_write;
    for (u32 i = 0; i < frames; i++, w++) {
        shm->audio[(w % ACWW_AUDIO_FRAMES) * 2] = lr[i * 2];
        shm->audio[(w % ACWW_AUDIO_FRAMES) * 2 + 1] = lr[i * 2 + 1];
    }
    __atomic_store_n(&shm->audio_write, w, __ATOMIC_RELEASE);  // (the frontend's audio callback reads it)
}
