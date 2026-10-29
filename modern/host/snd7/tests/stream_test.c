// The sound path of a stream (NitroSystem's NNSSndStrm, as the title music uses it) against the same channel playing the
// whole stream from one buffer: snd7's driver and spu.c, the stream's ring of 4 blocks per channel refilled at each of
// the stream's alarms (the ARM9's part, done here when the alarm's message arrives). Any difference is a sample the
// channel took from the ring before or after its refill: a click per block (docs/host.md). Also: a channel faster than
// the output is low-pass filtered (spu.c), a slower one held. No game data: a made-up signal.
//
//   cc -O1 -Imodern/host/snd7 -Imodern/host -Iinclude modern/host/snd7/tests/stream_test.c modern/host/snd7/spu.c
//      modern/host/snd7/driver.c modern/host/snd7/channel.c modern/host/snd7/exchannel.c modern/host/snd7/seq.c -lm
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snd7.h"

void snd7_receive(struct SNDCommand *list);
u64 snd7_next_due(void);
void snd7_run(u64 now);

// ---- what the driver needs of the rest of the host
static u64 sNow;
u64 host_cycles(void) { return sNow; }
u64 host_frame(void) { return sNow / 560190; }
const u8 data_0213a0b4[724];
unsigned int HostPtr32_Handle(const void *p) { (void)p; return 0; }
void *HostPtr32_Lookup(unsigned int h) { (void)h; return 0; }
static int sAlarms;
void pxi7_send(u32 w) { if ((w & 0x1f) == 7) sAlarms++; }
void host_fatal(const char *m) { fprintf(stderr, "%s\n", m); exit(1); }
static s16 *sOut;
static u32 sOutN, sOutCap;
void snd7_output(const s16 *lr, u32 n) {
    if (sOutN + n > sOutCap) {
        sOutCap = (sOutN + n) * 2;
        sOut = realloc(sOut, (size_t)sOutCap * 4);
    }
    memcpy(sOut + (size_t)sOutN * 2, lr, (size_t)n * 4);
    sOutN += n;
}

#define RING 1024  // samples per channel (2 channels x 2 KB: the stream players' buffers), 4 blocks
static s16 sRing[2][RING];

// one run: the stream (n stereo samples) through the ring (ring = 1) or from one buffer; the output's frames
static s16 *run(const s16 *pcm, long n, int timer, int ring, u32 *frames) {
    static int first = 1;
    if (first) { first = 0; sNow = 0x10000ull * 64 + 12345; snd7_receive(NULL); }  // (after the driver's start delay)
    s16 *lin[2] = {malloc((size_t)n * 2), malloc((size_t)n * 2)};
    for (long i = 0; i < n; i++) lin[0][i] = pcm[i * 2], lin[1][i] = pcm[i * 2 + 1];
    long pos = 0;
    if (ring) {
        for (int i = 0; i < RING; i++) sRing[0][i] = pcm[i * 2], sRing[1][i] = pcm[i * 2 + 1];
        pos = RING;
    }
    struct SNDCommand c[5];
    memset(c, 0, sizeof c);
    for (int ch = 0; ch < 2; ch++) {
        c[ch].id = 0x0e;  // SETUP_CHANNEL_PCM
        c[ch].arg[0] = (uintptr_t)ch | (uintptr_t)(timer << 5) << 16;
        c[ch].arg[1] = (uintptr_t)(ring ? sRing[ch] : lin[ch]);
        c[ch].arg[2] = (uintptr_t)(ring ? RING / 2 : n / 2) | 127u << 24;  // length in words, volume 127
        c[ch].arg[3] = 1u << 24 | (ring ? 1u : 2u) << 26 | (ch ? 127u : 0u) << 16;  // PCM16, loop / one shot, pan
        c[ch].llNext = &c[ch + 1];
    }
    u32 period = (u32)timer * RING / 4;
    c[2].id = 0x12, c[2].arg[1] = period, c[2].arg[2] = period, c[2].llNext = &c[3];  // SETUP_ALARM 0
    c[3].id = 0x0c, c[3].arg[0] = 3, c[3].arg[2] = ring ? 1 : 0, c[3].llNext = &c[4];  // START_TIMER
    c[4].id = 0x0b;  // (a harmless one: the list's end)
    snd7_run(sNow);
    sOutN = 0;
    snd7_receive(c);
    int blk = 0;
    u64 end = sNow + (u64)((double)(n - 4 * RING) * timer * 64);
    u64 vblank = sNow + 560190;
    sAlarms = 0;
    while (sNow < end) {
        u64 next = snd7_next_due();
        if (vblank < next) next = vblank;
        if (next > sNow) sNow = next;
        snd7_run(sNow);
        for (; sAlarms; sAlarms--) {  // the stream thread: the block just played, filled with the next samples
            for (int i = 0; i < RING / 4; i++, pos++) {
                sRing[0][blk * RING / 4 + i] = pos < n ? pcm[pos * 2] : 0;
                sRing[1][blk * RING / 4 + i] = pos < n ? pcm[pos * 2 + 1] : 0;
            }
            blk = (blk + 1) & 3;
        }
        if (sNow >= vblank) vblank += 560190;
    }
    struct SNDCommand stop = {0};
    stop.id = 0x0d, stop.arg[0] = 3, stop.arg[2] = 1;  // STOP_TIMER
    snd7_receive(&stop);
    free(lin[0]), free(lin[1]);
    *frames = sOutN;
    s16 *copy = malloc((size_t)sOutN * 4);
    memcpy(copy, sOut, (size_t)sOutN * 4);
    return copy;
}

int main(void) {
    int fails = 0;
    const struct { int timer; const char *what; } cases[] = {{11, "47.6 kHz (the title music's rate)"}, {32, "16.4 kHz"}};
    for (int k = 0; k < 2; k++) {
        int timer = cases[k].timer;
        double rate = 33513982.0 / 64 / timer;
        long n = (long)(rate * 3);
        s16 *pcm = malloc((size_t)n * 4);
        unsigned seed = 1;
        for (long i = 0; i < n; i++) {  // tones and a little noise
            seed = seed * 1103515245u + 12345u;
            double t = i / rate, v = 9000 * sin(2 * M_PI * 440 * t) + 7000 * sin(2 * M_PI * 3100 * t) +
                                     (k ? 0 : 3000 * sin(2 * M_PI * 19000 * t)) + (double)((seed >> 16) & 1023) - 512;
            pcm[i * 2] = (s16)v, pcm[i * 2 + 1] = (s16)(-v);
        }
        u32 fr, fl;
        s16 *ring = run(pcm, n, timer, 1, &fr), *lin = run(pcm, n, timer, 0, &fl);
        u32 m = fr < fl ? fr : fl, diff = 0, first = 0;
        for (u32 i = 0; i < m * 2; i++)
            if (ring[i] != lin[i] && !diff++) first = i / 2;
        printf("%s: %u frames, %u samples differ%s", cases[k].what, m, diff, diff ? "" : "\n");
        if (diff) printf(" (the first at frame %u)\n", first), fails++;
        free(ring), free(lin), free(pcm);
    }
    // a 19 kHz tone at 47.6 kHz: above the output's band, it must not come back as a tone below it (32728 - 19000 =
    // 13728 Hz) the way held samples would bring it
    {
        int timer = 11;
        double rate = 33513982.0 / 64 / timer;
        long n = (long)(rate * 1.5);
        s16 *pcm = malloc((size_t)n * 4);
        for (long i = 0; i < n; i++) pcm[i * 2] = pcm[i * 2 + 1] = (s16)(12000 * sin(2 * M_PI * 19000 * i / rate));
        u32 fr;
        s16 *o = run(pcm, n, timer, 0, &fr);
        u32 len = 8192;
        double peak = 0;
        for (u32 i = fr - len - 100; i < fr - 100; i++) peak = fmax(peak, fabs((double)o[i * 2]));
        printf("a 19 kHz tone at 47.6 kHz (amplitude 12000): what comes out peaks at %.0f\n", peak);
        if (peak > 200) printf("  the alias is too loud\n"), fails++;
        free(o), free(pcm);
    }
    printf(fails ? "FAILED\n" : "ok\n");
    return fails != 0;
}
