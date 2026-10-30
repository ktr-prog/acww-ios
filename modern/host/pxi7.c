// Host build: the ARM7 side of the PXI (inter-processor) registers and FIFO. On the DS the ARM7 runs its own
// program (sound, touch screen, RTC, NVRAM, wireless...) and talks to the ARM9 through a 16-word FIFO of
// (tag:5, err:1, data:26) messages. Here each service is answered by the host; messages to a service the host
// doesn't implement yet are logged.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef _WIN32
#include <dirent.h>
#include <strings.h>
#endif

#include "host.h"
#include "host_mem.h"
#include "plat.h"
#include "savestate.h"
#include "sys/CardCmd.h"

#define FIFOCNT 0x04000184u
#define RECV 0x04100000u

static u32 sQueue[64];
static int sHead, sCount;
static u32 sArm7Sync;

// replies that take time on the DS (the card's SPI transfers): sent when virtual time reaches `due`. The SDK's
// asynchronous calls rely on not completing before the caller looks at their status.
struct PxiLater { u64 due; u32 word; };
static struct PxiLater sLater[64];
static int sLaterCount;
void pxi7_send_after(u32 word, u32 cycles) {
    if (sLaterCount < 64) sLater[sLaterCount++] = (struct PxiLater){host_cycles() + cycles, word};
}
u64 pxi7_next_due(void) {
    u64 d = ~0ull;
    for (int i = 0; i < sLaterCount; i++) if (sLater[i].due < d) d = sLater[i].due;
    return d;
}
void pxi7_run_due(void) {
    for (int i = 0; i < sLaterCount;) {
        if (sLater[i].due <= host_cycles()) { u32 w = sLater[i].word; sLater[i] = sLater[--sLaterCount]; pxi7_send(w); }
        else i++;
    }
}

void pxi7_send(u32 word) {
    if (sCount == 64) { fprintf(stderr, "pxi7: receive FIFO overflow\n"); return; }
    sQueue[(sHead + sCount++) % 64] = word;
    if (*(u16 *)host_io(FIFOCNT) & 0x400) host_irq_raise(1u << 18);  // receive FIFO not empty
}

void pxi7_prepare_read(u32 a) {
    if (a == 0x04000180) {
        u16 v = *(u16 *)host_io(a);
        *(u16 *)host_io(a) = (u16)((v & ~0xfu) | (sArm7Sync & 0xf));
    } else if (a == FIFOCNT) {
        u16 v = *(u16 *)host_io(a) & ~0x430bu;  // bit 14 error and bit 3 send-clear read as 0
        v |= 1;                                   // send FIFO empty: the host takes words at once
        if (sCount == 0) v |= 0x100;              // receive FIFO empty
        if (sCount >= 16) v |= 0x200;
        *(u16 *)host_io(a) = v;
    } else if (a == RECV) {
        u32 w = 0;
        if (sCount) { w = sQueue[sHead]; sHead = (sHead + 1) % 64; sCount--; }
        *(u32 *)host_io(a) = w;
        if (sCount && (*(u16 *)host_io(FIFOCNT) & 0x400)) host_irq_raise(1u << 18);
    }
}

void pxi7_sync_written(u32 value) {
    // the start-up handshake (PXI_InitFifo): the ARM7 answers each value with the next one
    sArm7Sync = ((value >> 8) + 1) & 0xf;
}

// Messages that carry an address: the ARM9 side hands the pointer over first (include/nitro/pxi_ptr.h,
// PXI_PointerWord); the message's data bits are the DS's (a truncated address) and aren't used for it.
static const void *sPointer[32];
void HostPxi_PutPointer(unsigned int tag, const void *p) { sPointer[tag & 31] = p; }
static void *take_pointer(u32 tag) {
    void *p = (void *)sPointer[tag];
    sPointer[tag] = 0;
    return p;
}

// The other way (the WM's replies, wm7.c): the word goes out with the pointer, which the ARM9's receive callback takes
// (HostPxi_TakeArm7Pointer) in the order the words arrive. The word's data bits are the DS's (the address's low bits).
static struct { u32 tag; void *p; } sArm7Pointer[64];
static int sArm7PointerHead, sArm7PointerCount;
int pxi7_send_pointer(u32 tag, void *p) {
    if (sCount == 64 || sArm7PointerCount == 64) return 0;
    sArm7Pointer[(sArm7PointerHead + sArm7PointerCount++) % 64].tag = tag;
    sArm7Pointer[(sArm7PointerHead + sArm7PointerCount - 1) % 64].p = p;
    pxi7_send(tag | ((u32)(uintptr_t)p & 0x3ffffff) << 6);
    return 1;
}
void *HostPxi_TakeArm7Pointer(unsigned int tag) {
    if (!sArm7PointerCount || sArm7Pointer[sArm7PointerHead].tag != tag)
        host_fatal("pxi7: a reply without its pointer");
    void *p = sArm7Pointer[sArm7PointerHead].p;
    sArm7PointerHead = (sArm7PointerHead + 1) % 64;
    sArm7PointerCount--;
    return p;
}

// ---- NVRAM (tag 4): the firmware flash. Messages: bit 25 start, bit 24 end, bits 16-23 index, bits 0-15 payload;
// the first carries the command in bits 8-15. The reply's low byte is the result (0: success).
static u8 sNvram[0x40000];
static u32 sNvramSize;
static u32 sNvWords[8];
static int sNvCount;
static u8 sNvStatus;  // bit 1: write enable latch

// the firmware image when none is given (ACWW_FIRMWARE): a 128 KB flash, erased (0xff) but for what GBATEK ("DS
// Firmware Header", "DS Firmware User Settings") says is read - the user settings' place in the header, and the user
// settings twice (as after a first start: owner "PLAYER", English, a birthday of 1 January, the touch screen
// calibration of a typical unit), each with its CRC. The game reads the header and the Wi-Fi settings (left erased:
// no access point set up); the runtime the calibration (tp_raw).
static u16 fw_crc16(u16 crc, const u8 *p, u32 n) {
    for (u32 i = 0; i < n; i++) {
        crc ^= p[i];
        for (int b = 0; b < 8; b++) crc = crc & 1 ? (u16)(crc >> 1 ^ 0xa001) : (u16)(crc >> 1);
    }
    return crc;
}
static void default_firmware(void) {
    enum { SIZE = 0x20000, USER = SIZE - 0x200 };
    memset(sNvram, 0xff, SIZE);
    sNvram[0x20] = (u8)(USER / 8), sNvram[0x21] = (u8)(USER / 8 >> 8);
    for (int copy = 0; copy < 2; copy++) {
        u8 *u = sNvram + USER + copy * 0x100;
        memset(u, 0, 0x100);
        u[0x00] = 5;                      // version
        u[0x02] = 0;                      // favourite colour
        u[0x03] = 1, u[0x04] = 1;         // birthday: month, day
        static const char kName[] = "PLAYER";
        for (int i = 0; kName[i]; i++) u[0x06 + 2 * i] = (u8)kName[i];  // nickname (UTF-16)
        u[0x1a] = (u8)(sizeof kName - 1); // its length
        static const u16 kCal[6] = {0x02df, 0x032c, 0x0d3b, 0x0ce7, 0, 0};  // ADC x1, y1, x2, y2
        u[0x58] = (u8)kCal[0], u[0x59] = (u8)(kCal[0] >> 8), u[0x5a] = (u8)kCal[1], u[0x5b] = (u8)(kCal[1] >> 8);
        u[0x5c] = 0x20, u[0x5d] = 0x20;   // their screen points
        u[0x5e] = (u8)kCal[2], u[0x5f] = (u8)(kCal[2] >> 8), u[0x60] = (u8)kCal[3], u[0x61] = (u8)(kCal[3] >> 8);
        u[0x62] = 0xe0, u[0x63] = 0xa0;
        u[0x64] = 0x01;                   // language: English
        u[0x66] = 0;                      // year (2000 +)
        u[0x70] = (u8)copy;               // update counter
        u16 crc = fw_crc16(0xffff, u, 0x70);
        u[0x72] = (u8)crc, u[0x73] = (u8)(crc >> 8);
    }
    sNvramSize = SIZE;
}

// The DS's boot puts the firmware's user settings (the valid copy with the newer update counter, 0x74 bytes with the
// counter and the CRC) at HW_NVRAM_USER_INFO (0x027ffc80), where the SDK reads them: the owner's name and birthday,
// the language, and the touch screen's calibration (TP_GetUserInfo). Without them the calibration was all zero and
// TP_GetCalibratedPoint put every touch at (0,0): touches that need a place (buttons, keys, the inventory) did
// nothing, only "touch anywhere" did.
static void firmware_user_settings(void) {
    if (!sNvramSize) return;
    u32 base = (u32)(sNvram[0x20] | sNvram[0x21] << 8) * 8;
    const u8 *best = 0;
    int bestCount = -1;
    for (int copy = 0; copy < 2; copy++) {
        u32 at = base + (u32)copy * 0x100;
        if (at + 0x74 > sNvramSize) continue;
        const u8 *u = sNvram + at;
        u16 crc = (u16)(u[0x72] | u[0x73] << 8);
        if (fw_crc16(0xffff, u, 0x70) != crc) continue;
        int count = u[0x70] & 0x7f;
        // (the newer one: the counters wrap at 0x80)
        if (!best || ((count - bestCount) & 0x7f) == 1) { best = u; bestCount = count; }
    }
    if (!best) {
        fprintf(stderr, "pxi7: the firmware has no valid user settings: none for the game\n");
        return;
    }
    memcpy(gHostSysWork.nvramUserInfo, best, 0x74);
}

// ---- the Wi-Fi settings: DWC's backup area, four pages of 0x100 bytes below the user settings (each with a CRC16 at
// 0xfe): the three connections (access points) and the WFC user ID (0xf0, in pages 0 and 1). Kept in a file next to
// the save file (<save>.wfc), so the WFC ID - Wiimmfi's account - stays the same. Connection 1 is the emulated access
// point (wm7.c, net_nat.c) with Wiimmfi's DNS server; with ACWW_WIIMMFI=0 (tests) it is taken out again (only if it
// is that one).
#include "host_features.h"
#include "net_nat.h"
static char sWfcPath[1024];
static u32 wfc_base(void) { return (u32)(sNvram[0x20] | sNvram[0x21] << 8) * 8 - 0x400; }
static int wfc_page_ok(const u8 *p) { return fw_crc16(0, p, 0xfe) == (u16)(p[0xfe] | p[0xff] << 8); }
static void wfc_page_crc(u8 *p) {
    u16 c = fw_crc16(0, p, 0xfe);
    p[0xfe] = (u8)c, p[0xff] = (u8)(c >> 8);
}
static void wfc_save(void) {
    if (!sWfcPath[0] || !sNvramSize) return;
    char tmp[1100];
    snprintf(tmp, sizeof tmp, "%s.tmp", sWfcPath);
    FILE *f = fopen(tmp, "wb");
    if (!f) return;
    int ok = fwrite(sNvram + wfc_base(), 1, 0x400, f) == 0x400;
    if (fclose(f) != 0 || !ok || rename(tmp, sWfcPath) != 0) fprintf(stderr, "pxi7: can't write %s\n", sWfcPath);
}
static void wfc_load(void) {
    if (!sWfcPath[0] || !sNvramSize) return;
    FILE *f = fopen(sWfcPath, "rb");
    if (!f) return;
    u8 b[0x400];
    if (fread(b, 1, 0x400, f) == 0x400) memcpy(sNvram + wfc_base(), b, 0x400);
    fclose(f);
}
static void wfc_sync(void) {
    if (!sNvramSize || wfc_base() + 0x400 > sNvramSize) return;
    u8 *w = sNvram + wfc_base();
    int on = host_wiimmfi_on() && nat_supported();
    int ours = wfc_page_ok(w) && w[0xe7] == 0 && !memcmp(w + 0x40, NAT_AP_SSID, sizeof NAT_AP_SSID);
    if (on == ours) return;
    if (on) {
        if (!wfc_page_ok(w) || !wfc_page_ok(w + 0x100) || !wfc_page_ok(w + 0x200) || !wfc_page_ok(w + 0x300)) {
            // (a fresh area, or a damaged one: as DWC would set it up - no connections, no ID yet (made at the first
            // connection) - so that DWC finds it valid and keeps connection 1)
            u8 id[14] = {0};
            if (wfc_page_ok(w)) memcpy(id, w + 0xf0, 14);
            else if (wfc_page_ok(w + 0x100)) memcpy(id, w + 0x1f0, 14);
            memset(w, 0, 0x400);
            for (int i = 0; i < 3; i++) w[i * 0x100 + 0xe7] = 0xff;
            memcpy(w + 0xf0, id, 14), memcpy(w + 0x1f0, id, 14);
            for (int i = 1; i < 4; i++) wfc_page_crc(w + i * 0x100);
        }
        memset(w, 0, 0xef);
        memcpy(w + 0x40, NAT_AP_SSID, sizeof NAT_AP_SSID - 1);
        w[0xc8] = 167, w[0xc9] = 86, w[0xca] = 108, w[0xcb] = 126;  // the DNS server: Wiimmfi's (the rest: DHCP)
        w[0xea] = 0x78, w[0xeb] = 0x05;  // MTU 1400
        w[0xe6] = 0;  // no WEP
        w[0xe7] = 0;  // set up
        w[0xef] |= 1, w[0x1ef] |= 1;
        fprintf(stderr, "pxi7: Wi-Fi connection 1: the Wiimmfi access point\n");
    } else {
        memset(w, 0, 0xef);
        w[0xe7] = 0xff;
        w[0xef] &= ~1, w[0x1ef] &= ~1;
        fprintf(stderr, "pxi7: Wi-Fi connection 1 taken out (Wiimmfi off)\n");
    }
    wfc_page_crc(w), wfc_page_crc(w + 0x100);
    wfc_save();
}
static int wfc_touched(u32 addr, u32 size) {
    u32 b = wfc_base();
    return sNvramSize && addr < b + 0x400 && addr + size > b;
}

static void nvram_command(void) {
    u32 cmd = sNvWords[0] >> 8 & 0xff;
    u32 addr = (sNvWords[0] & 0xff) << 16 | (sNvWords[1] & 0xffff);
    u32 size = (sNvWords[2] & 0xffff) << 16 | (sNvWords[3] & 0xffff);
    u8 *ptr = take_pointer(4);  // the buffer (READ: in words 4-5 on the DS, PW: 3-4, RDSR: 0-2)
    switch (cmd) {
    case 0x23: case 0x24:  // READ, FAST_READ
        if (wfc_touched(addr, size)) wfc_sync();
        for (u32 i = 0; i < size; i++) ptr[i] = sNvramSize ? sNvram[(addr + i) % sNvramSize] : 0xff;
        break;
    case 0x22:             // RDSR: one byte
        *ptr = sNvStatus;
        break;
    case 0x20: sNvStatus |= 2; break;   // WREN
    case 0x21: sNvStatus &= ~2; break;  // WRDI
    case 0x0a: case 0x25: {  // PW (page write; 16-bit size, pointer in words 3-4): kept in memory only
        u32 n = sNvWords[2] & 0xffff;
        const u8 *src = ptr;
        for (u32 i = 0; i < n && sNvramSize; i++) sNvram[(addr + i) % sNvramSize] = src[i];
        if (wfc_touched(addr, n)) wfc_save();
        size = n;
        sNvStatus &= ~2;
        break;
    }
    default: break;        // WREN, WRDI, ...
    }
    if (gHostVerbose && cmd != 0x22) fprintf(stderr, "pxi7: NVRAM %02x addr %06x size %x -> %p\n", cmd, addr, size, (void *)ptr);
    pxi7_send(4 | (cmd << 8) << 6);
}

static void nvram_word(u32 data) {
    if (data & 0x02000000) sNvCount = 0;
    if (sNvCount < 8) sNvWords[sNvCount++] = data;
    if (data & 0x01000000) nvram_command();
}

// ---- RTC (tag 5): command in bits 8-14; the ARM7 writes the raw BCD date and time to the system work area
// (year 00-99 = 2000-2099, month, day, day of the week 0 = Sunday; hour with bit 6 the PM flag, minute, second).
// The clock is the host's local time; ACWW_RTC="YYYY-MM-DD HH:MM:SS" pins a fixed one (it doesn't advance), and
// headless runs (ACWW_HEADLESS) default to the fixed 2026-04-01 12:00:00 (a Wednesday) so that they are
// reproducible (the oracle's captures were taken with it); ACWW_RTC=host gives them the local time.
static int sRtcFixed = -1;
static struct tm sRtcTime;

static void rtc_setup(void) {
    const char *e = getenv("ACWW_RTC");
    memset(&sRtcTime, 0, sizeof sRtcTime);
    sRtcFixed = 0;
    if (e && strcmp(e, "host")) {
        int y, mo, d, h = 0, mi = 0, sec = 0;
        if (sscanf(e, "%d-%d-%d %d:%d:%d", &y, &mo, &d, &h, &mi, &sec) >= 3 && y >= 2000 && y <= 2099 && mo >= 1 &&
            mo <= 12 && d >= 1 && d <= 31 && h >= 0 && h < 24 && mi >= 0 && mi < 60 && sec >= 0 && sec < 60) {
            sRtcTime = (struct tm){.tm_year = y - 1900, .tm_mon = mo - 1, .tm_mday = d, .tm_hour = h, .tm_min = mi,
                                   .tm_sec = sec, .tm_isdst = -1};
            sRtcFixed = 1;
        } else {
            fprintf(stderr, "pxi7: ACWW_RTC=\"%s\" is not YYYY-MM-DD HH:MM:SS (2000-2099): using the host's time\n", e);
        }
    } else if (!e && getenv("ACWW_HEADLESS")) {
        sRtcTime = (struct tm){.tm_year = 126, .tm_mon = 3, .tm_mday = 1, .tm_hour = 12, .tm_isdst = -1};
        sRtcFixed = 1;
    }
    if (sRtcFixed) {
        struct tm t = sRtcTime;
        t.tm_hour = 12;  // the day of the week (noon: no DST edge)
        mktime(&t);
        sRtcTime.tm_wday = t.tm_wday;
    }
}

static u32 bcd(int v) { return (u32)(v / 10 << 4 | v % 10); }

static void rtc_command(u32 data) {
    u32 cmd = (data & 0x7f00) >> 8;
    volatile u32 *raw = gHostSysWork.realTimeClock;  // HW_REAL_TIME_CLOCK_BUF
    struct tm t;
    if (sRtcFixed < 0) rtc_setup();
    if (sRtcFixed) t = sRtcTime;
    else {
        if (!host_plat_localtime(&t)) memset(&t, 0, sizeof t);  // (unknown: 2000-01-01 below)
        if (t.tm_year < 100) t = (struct tm){.tm_year = 100, .tm_mday = 1, .tm_wday = 6};  // before 2000: 2000-01-01
        if (t.tm_year > 199)  // after 2099: 2099-12-31 23:59:59
            t = (struct tm){.tm_year = 199, .tm_mon = 11, .tm_mday = 31, .tm_hour = 23, .tm_min = 59, .tm_sec = 59, .tm_wday = 4};
    }
    if (cmd == 0x10 || cmd == 0x11)
        raw[0] = bcd(t.tm_year % 100) | bcd(t.tm_mon + 1) << 8 | bcd(t.tm_mday) << 16 | (u32)t.tm_wday << 24;
    if (cmd == 0x10 || cmd == 0x12)
        raw[1] = (bcd(t.tm_hour) | (t.tm_hour >= 12 ? 0x40u : 0)) | bcd(t.tm_min) << 8 | bcd(t.tm_sec) << 16;
    pxi7_send(5 | (cmd << 8) << 6);
}

// ---- card backup (tag 11, messages with the error bit): the save memory. CARDi_Request sends a request id; the
// first request (0) is followed by the address of the shared command block: result 0x00, source 0x0c,
// destination 0x10, length 0x14, total size 0x18. The reply is a message with the error bit; the result (0 = OK)
// is in the block. Requests (NitroSDK CARDRequest): 2 identify, 6 read, 7 write, 8 program, 9 verify, 10 erase
// page, 11 erase sector, 12 erase chip. The ARM9 side splits a long write into page-sized requests (the stream
// command), each followed by a verify. The save memory is kept in the host; with ACWW_SAVE=<file> it is loaded
// from that file at the first access (a missing file is a blank memory, all 0xff) and every change is written
// through to it. Without ACWW_SAVE, a run with a window saves next to the ROM (<name>.sav); a headless one
// (ACWW_HEADLESS, the tests) starts with a blank save memory and writes nothing to disk; ACWW_SAVE= (empty): neither.
static CARDCmd *sCardBlock;
static int sCardWantBlock;
static u8 *sSave;
static u32 sSaveSize;
static const char *sSavePath;
static FILE *sSaveFile;
static int sSaveBesideRom;  // (the save path is the ROM's name: another save file beside it may be taken)

// (a save state's load leaves the save memory alone - its buffer and its file: the game's save on disk isn't rolled
// back with the state; savestate.c)
static void save_keep(void) {
    host_state_keep(&sSave, sizeof sSave);
    host_state_keep(&sSaveSize, sizeof sSaveSize);
    host_state_keep(&sSavePath, sizeof sSavePath);
    host_state_keep(&sSaveFile, sizeof sSaveFile);
}

// ---- saves from the other versions. The town data (two copies of 0x15fe0 bytes) starts with the version's code:
// 0x8a USA, 0xc5 Europe (the same layout: only the code differs - Universal-Team's LeafEdit treats them alike),
// 0x32 Japan and Korea (other layouts: 0x12224 and 0x173fc bytes a copy, other text encodings). The game reads only
// its own code (SaveData::isValid) and would take any other save for none and start a new town over it.
enum { SAVE_COPY = 0x15fe0, SAVE_SUM = 0x15fdc };
static int save_version(const u8 *d, u32 n) {  // 'U' USA, 'E' Europe, 'J' Japan, 'K' Korea, 0 none
    if (n < 2 * SAVE_COPY) return 0;
    if (d[0] == 0x8a && d[SAVE_COPY] == 0x8a) return 'U';
    if (d[0] == 0xc5 && d[SAVE_COPY] == 0xc5) return 'E';
    if (d[0] == 0x32 && d[0x12224] == 0x32) return 'J';
    if (d[0] == 0x32 && d[0x173fc] == 0x32) return 'K';
    return 0;
}
// a European save as the USA version's: each copy's code, and its checksum (the copy's 16-bit words sum to 0)
static void save_from_europe(u8 *d) {
    for (u32 c = 0; c < 2 * SAVE_COPY; c += SAVE_COPY) {
        if (d[c] != 0xc5) continue;  // (an empty or damaged copy stays as it is)
        u16 sum = 0, old = (u16)(d[c + SAVE_SUM] | d[c + SAVE_SUM + 1] << 8);
        for (u32 i = 0; i < SAVE_COPY; i += 2) sum = (u16)(sum + (d[c + i] | d[c + i + 1] << 8));
        int valid = sum == 0;
        d[c] = 0x8a;
        if (!valid) continue;  // (a copy the European game itself wouldn't have read: left damaged)
        sum = 0;
        for (u32 i = 0; i < SAVE_COPY; i += 2)
            if (i != SAVE_SUM) sum = (u16)(sum + (d[c + i] | d[c + i + 1] << 8));
        u16 cs = (u16)-sum;
        d[c + SAVE_SUM] = (u8)cs, d[c + SAVE_SUM + 1] = (u8)(cs >> 8);
        (void)old;
    }
}
static int copy_file(const char *from, const char *to) {
    FILE *a = fopen(from, "rb"), *b = a ? fopen(to, "wb") : 0;
    int ok = a && b;
    char buf[65536];
    size_t n;
    while (ok && (n = fread(buf, 1, sizeof buf, a)) > 0) ok = fwrite(buf, 1, n, b) == n;
    if (a) fclose(a);
    if (b && fclose(b) != 0) ok = 0;
    return ok;
}
// No save file for the game yet: a save of any version next to it, under another name (another dump's name, an
// emulator's .dsv - DeSmuME's: the data then a footer), is taken: copied to the game's name, the original left alone.
static void save_adopt(void) {
#ifndef _WIN32
    char dir[1024];
    snprintf(dir, sizeof dir, "%s", sSavePath);
    char *slash = strrchr(dir, '/');
    if (slash) *slash = 0; else snprintf(dir, sizeof dir, ".");
    DIR *dp = opendir(dir);
    if (!dp) return;
    struct dirent *e;
    char found[1200] = "";
    int count = 0;
    while ((e = readdir(dp))) {
        const char *x = strrchr(e->d_name, '.');
        if (!x || e->d_name[0] == '.' || (strcasecmp(x, ".sav") && strcasecmp(x, ".dsv"))) continue;
        char path[1200];
        snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
        FILE *f = fopen(path, "rb");
        if (!f) continue;
        static u8 head[0x40000];
        size_t n = fread(head, 1, sizeof head, f);
        fclose(f);
        if (!save_version(head, (u32)n)) continue;
        count++;
        snprintf(found, sizeof found, "%s", path);
    }
    closedir(dp);
    if (count != 1) {
        if (count > 1) fprintf(stderr, "pxi7: %d save files of the game next to it, none called %s: none taken\n", count, sSavePath);
        return;
    }
    if (copy_file(found, sSavePath)) fprintf(stderr, "pxi7: the save file %s taken (copied to %s)\n", found, sSavePath);
    else remove(sSavePath);
#endif
}

static void save_load(u32 size) {
    save_keep();
    if (sSave && size <= sSaveSize) return;
    sSave = realloc(sSave, size);
    memset(sSave + sSaveSize, 0xff, size - sSaveSize);
    sSaveSize = size;
    if (!sSavePath) return;
    if (!sSaveFile) {
        FILE *probe = fopen(sSavePath, "rb");
        if (probe) fclose(probe);
        else if (sSaveBesideRom) save_adopt();
        sSaveFile = fopen(sSavePath, "r+b");  // created at the first write if missing
    }
    if (sSaveFile) {
        fseek(sSaveFile, 0, SEEK_SET);
        size_t n = fread(sSave, 1, size, sSaveFile);
        fprintf(stderr, "pxi7: save memory %#x bytes, %#zx loaded from %s\n", size, n, sSavePath);
        int v = save_version(sSave, (u32)n);
        if (v == 'E') {
            // (the European original kept beside it, once)
            char keep[1100];
            snprintf(keep, sizeof keep, "%s.europe", sSavePath);
            FILE *k = fopen(keep, "rb");
            if (k) fclose(k);
            else if ((k = fopen(keep, "wb"))) { fwrite(sSave, 1, n, k); fclose(k); }
            save_from_europe(sSave);
            fseek(sSaveFile, 0, SEEK_SET);
            fwrite(sSave, 1, n, sSaveFile);
            fflush(sSaveFile);
            fprintf(stderr, "pxi7: a European save: made the USA version's (the original kept as %s)\n", keep);
        } else if (v == 'J' || v == 'K') {
            // (not taken: the game would start a new town over it)
            fclose(sSaveFile);
            sSaveFile = 0;
            sSavePath = 0;
            memset(sSave, 0xff, size);
            host_fatal(v == 'J' ? "the save file is from the Japanese version, whose save layout differs from the USA "
                                  "version's - it can't be used yet (the file is left as it is)"
                                : "the save file is from the Korean version, whose save layout differs from the USA "
                                  "version's - it can't be used yet (the file is left as it is)");
        }
    }
}
// write a changed range through to the file (the whole memory when the file is new)
static void save_store(u32 at, u32 len) {
    save_keep();
    if (!sSavePath || at >= sSaveSize) return;
    if (!sSaveFile) {
        sSaveFile = fopen(sSavePath, "w+b");
        if (!sSaveFile) { perror(sSavePath); sSavePath = 0; return; }
    }
    fseek(sSaveFile, 0, SEEK_END);
    long have = ftell(sSaveFile);
    if (have >= 0 && have < (long)sSaveSize)  // a new or short file: complete it with the memory's rest first
        fwrite(sSave + have, 1, sSaveSize - (u32)have, sSaveFile);
    if (len > sSaveSize - at) len = sSaveSize - at;
    fseek(sSaveFile, (long)at, SEEK_SET);
    fwrite(sSave + at, 1, len, sSaveFile);
    fflush(sSaveFile);
}

static void card_request(u32 data) {
    if (sCardWantBlock) { sCardBlock = take_pointer(11); sCardWantBlock = 0; pxi7_send_after(11 | 1u << 5, 20000); return; }
    CARDCmd *b = sCardBlock;
    if (data == 0) { sCardWantBlock = 1; return; }
    if (!b) { pxi7_send_after(11 | 1u << 5, 20000); return; }
    u32 result = 0;
    if (b->f18) save_load(b->f18);
    // the block's source and destination: a card address and ARM9 memory (include/sys/CardCmd.h)
    uintptr_t src = b->srcBuf, dst = b->dstBuf;
    u32 len = b->len;
    switch (data) {
    case 6:  // read: card address src to memory dst
        for (u32 i = 0; i < len; i++) ((u8 *)dst)[i] = src + i < sSaveSize ? sSave[src + i] : 0xff;
        break;
    case 7: case 8:  // write, program: memory src to card address dst
        for (u32 i = 0; i < len && dst + i < sSaveSize; i++) sSave[dst + i] = ((const u8 *)src)[i];
        save_store((u32)dst, len);
        break;
    case 9:  // verify
        for (u32 i = 0; i < len && dst + i < sSaveSize; i++) if (sSave[dst + i] != ((const u8 *)src)[i]) result = 5;
        break;
    case 10: case 11:  // erase page/sector
        for (u32 i = 0; i < len && dst + i < sSaveSize; i++) sSave[dst + i] = 0xff;
        save_store((u32)dst, len);
        break;
    case 12:  // erase chip
        if (sSave) memset(sSave, 0xff, sSaveSize);
        save_store(0, sSaveSize);
        break;
    default: break;  // identify, read id, status ...
    }
    b->result = result;
    if (gHostVerbose) fprintf(stderr, "pxi7: card %u src %llx dst %llx len %x -> %u (frame %llu)\n", data, (unsigned long long)src, (unsigned long long)dst, len, result,
                              (unsigned long long)host_frame());
    pxi7_send_after(11 | 1u << 5, 20000);
}

// ---- touch panel (tag 6): request type in bits 8-14 (0 one sample, 1 start auto sampling, 2 stop, 3 stability);
// the reply has the type, result 0 and bit 24. Samples (x:12, y:12, touch:1, validity:2) go to HW_TOUCHPANEL_BUF as raw
// ADC values, from screen coordinates through the firmware's calibration points. During auto sampling one sample
// (type 16) is sent per frame. Touches come from ACWW_TOUCH="frame:frames:x,y;..." for now.
static int sTpAuto;
struct TouchScript { u32 frame, len, x, y; };
static struct TouchScript sTouch[64];
static int sTouchCount;

int frontend_touch(int *x, int *y);
// a screen position as the raw ADC sample, through the firmware's calibration points
static u32 tp_raw(s32 sx, s32 sy) {
    const u8 *u = sNvram + (sNvramSize ? (sNvram[0x20] | sNvram[0x21] << 8) * 8 : 0) + 0x58;
    s32 ax1 = u[0] | u[1] << 8, ay1 = u[2] | u[3] << 8, sx1 = u[4], sy1 = u[5];
    s32 ax2 = u[6] | u[7] << 8, ay2 = u[8] | u[9] << 8, sx2 = u[10], sy2 = u[11];
    // (the pixel's centre: the SDK's conversion back truncates, so aiming at the corner gave the pixel up-left of it)
    s32 x = sx2 != sx1 ? ax1 + (2 * (sx - sx1) + 1) * (ax2 - ax1) / (2 * (sx2 - sx1)) : 0;
    s32 y = sy2 != sy1 ? ay1 + (2 * (sy - sy1) + 1) * (ay2 - ay1) / (2 * (sy2 - sy1)) : 0;
    return (u32)(x & 0xfff) | (u32)(y & 0xfff) << 12 | 1u << 24;
}

static void tp_sample(void) {
    u32 f = (u32)host_frame(), raw = 0;
    int lx, ly;
    if (frontend_touch(&lx, &ly)) raw = tp_raw(lx, ly);  // the viewer's mouse
    for (int i = 0; i < sTouchCount; i++)                // ACWW_TOUCH
        if (f >= sTouch[i].frame && f < sTouch[i].frame + sTouch[i].len) raw = tp_raw((s32)sTouch[i].x, (s32)sTouch[i].y);
    ((volatile u16 *)gHostSysWork.touchPanel)[0] = (u16)raw;  // HW_TOUCHPANEL_BUF
    ((volatile u16 *)gHostSysWork.touchPanel)[1] = (u16)(raw >> 16);
}

static void tp_command(u32 data) {
    u32 type = (data & 0x7f00) >> 8;
    if (type == 0) tp_sample();
    if (type == 1) sTpAuto = 1;
    if (type == 2) sTpAuto = 0;
    pxi7_send(6 | ((type << 8) | 0x1000000) << 6);
}

void pxi7_vblank(void) {
    if (sTpAuto && sCount == 0) { tp_sample(); pxi7_send(6 | (16u << 8) << 6); }
}

// ---- power management (tag 8): the power management chip (PMIC) behind the ARM7's SPI. Messages as NVRAM's:
// bit 25 start, bit 24 end, bits 16-23 index, the command in bits 8-14 of the first word, payload in the low bits.
// Commands (all the ARM9 side sends): 0x63 utility (two words: number = word0 bits 0-7 << 16 | word1 bits 0-15),
// 0x65 read register (bits 0-7: register), 0x66 set the LED pattern (bits 0-7), 0x67 get the LED pattern.
// Reply: (command << 8 | value), the value the result (0 = success) - except for a register read, answered with
// command 0x70 + register and the register's value, and 0x67 with the pattern; PMi_CommonCallback takes it apart.
// Answered at once: the sync wrappers spin on memory (PMi_WaitBusy), where virtual time doesn't move.
// PMIC registers: 0 control (bit 0 sound amp, bit 2 bottom backlight, bit 3 top backlight, bit 4 LED blink,
// bit 5 blink fast, bit 6 power off), 1 battery (bit 0 low), 2 mic amp, 3 mic gain.
static u32 sPmWords[4];
static int sPmCount;
static u8 sPmReg[5] = {0x0d, 0x00, 0x00, 0x00, 0x00};  // amp and both backlights on, battery fine
static u8 sPmLedPattern;

static u32 pm_utility(u32 n) {
    u8 *r = &sPmReg[0];
    switch (n) {
    case 1: *r &= ~0x30; break;                 // LED on
    case 2: *r |= 0x30; break;                  // LED blinking fast
    case 3: *r = (u8)((*r | 0x10) & ~0x20); break;  // LED blinking slowly
    case 4: *r |= 0x04; break;                  // bottom backlight (LCD1) on / off
    case 5: *r &= ~0x04; break;
    case 6: *r |= 0x08; break;                  // top backlight (LCD2) on / off
    case 7: *r &= ~0x08; break;
    case 8: *r |= 0x0c; break;                  // both
    case 9: *r &= ~0x0c; break;
    case 10: *r |= 0x01; break;                 // sound amp on / off
    case 11: *r &= ~0x01; break;
    case 14: {                                  // force power off: the DS switches off; end the run like that
        extern void snd7_output_close(void);
        extern void frontend_shutdown(void);
        fprintf(stderr, "host: power off requested by the game (frame %llu)\n", (unsigned long long)host_frame());
        snd7_output_close();
        frontend_shutdown();
        _Exit(0);
    }
    default: break;
    }
    return 0;
}

static void pm_command(void) {
    u32 cmd = sPmWords[0] >> 8 & 0x7f, arg = sPmWords[0] & 0xff, reply_cmd = cmd, value = 0;
    switch (cmd) {
    case 0x63: value = pm_utility(arg << 16 | (sPmWords[1] & 0xffff)); arg = arg << 16 | (sPmWords[1] & 0xffff); break;
    case 0x65: reply_cmd = 0x70 + (arg & 3); value = arg < 5 ? sPmReg[arg] : 0; break;
    case 0x66: sPmLedPattern = (u8)arg; break;
    case 0x67: value = sPmLedPattern; break;
    default: fprintf(stderr, "pxi7: PM command %02x (not handled)\n", cmd); break;
    }
    if (gHostVerbose) fprintf(stderr, "pxi7: PM %02x %x -> %02x (frame %llu)\n", cmd, arg, value, (unsigned long long)host_frame());
    pxi7_send(8 | (reply_cmd << 8 | (value & 0xff)) << 6);
}

static void pm_word(u32 data) {
    if (data & 0x02000000) sPmCount = 0;
    if (sPmCount < 4) sPmWords[sPmCount++] = data;
    if (data & 0x01000000) pm_command();
}

// ---- sound (tag 7): the native sound driver (snd7/): a command list (its pointer handed over), or 0 = process now
struct SNDCommand;
void snd7_receive(struct SNDCommand *list);

// ---- wireless (tag 10): the WM's requests (wm7.c)
void wm7_receive(void *request);
void wm7_init(const char *savePath);

// ---- services
static void service(u32 tag, u32 data, u32 err) {
    switch (tag) {
    case 4: nvram_word(data); break;
    case 5: rtc_command(data); break;
    case 6: tp_command(data); break;
    case 7: snd7_receive(data ? take_pointer(7) : 0); break;
    case 8: pm_word(data); break;
    case 10: wm7_receive(take_pointer(10)); break;  // WM (wireless): wm7.c
    case 11: card_request(data); break;
    case 15:  // WVR: VRAM C/D for the wireless reboot driver; the ARM7 acknowledges at once
        if (data >> 16 == 1) pxi7_send(15 | 0x10000u << 6);
        break;
    default:
        if (gHostVerbose) fprintf(stderr, "pxi7: tag %u data %07x%s (no service)\n", tag, data, err ? " err" : "");
        break;
    }
}

void pxi7_receive(u32 word) { service(word & 31, word >> 6, word >> 5 & 1); }

void pxi7_init(void) {
    const char *t = getenv("ACWW_TOUCH");
    while (t && *t && sTouchCount < 64) {
        unsigned fr, n, x, y;
        if (sscanf(t, "%u:%u:%u,%u", &fr, &n, &x, &y) == 4) sTouch[sTouchCount++] = (struct TouchScript){fr, n, x, y};
        t = strchr(t, ';');
        if (t) t++;
    }
    sSavePath = getenv("ACWW_SAVE");
    if (!sSavePath && !getenv("ACWW_HEADLESS")) {  // (playing: next to the ROM, <name>.sav; test runs: none)
        extern const char *host_rom_path(void);
        static char path[1024];
        sSaveBesideRom = 1;
        const char *rom = host_rom_path();
        size_t n = rom ? strlen(rom) : 0;
        if (n > 4 && !strcmp(rom + n - 4, ".nds")) n -= 4;
#if HOST_IOS
        // (iOS: .NDS too - files copied from elsewhere keep their name as it was; the other hosts keep the name their
        // saves already have)
        else if (n > 4 && rom[n - 4] == '.' && (rom[n - 3] | 32) == 'n' && (rom[n - 2] | 32) == 'd' &&
                 (rom[n - 1] | 32) == 's')
            n -= 4;
#endif
        if (rom && n + 5 < sizeof path) {
            snprintf(path, sizeof path, "%.*s.sav", (int)n, rom);
            sSavePath = path;
            fprintf(stderr, "pxi7: the save file: %s (ACWW_SAVE: another)\n", path);
        }
    }
    if (sSavePath && !*sSavePath) sSavePath = 0;
    const char *fw = getenv("ACWW_FIRMWARE");
    FILE *f = fw ? fopen(fw, "rb") : 0;
    if (f) { sNvramSize = (u32)fread(sNvram, 1, sizeof sNvram, f); fclose(f); }
    else if (fw) fprintf(stderr, "pxi7: can't read the firmware image %s: a default one\n", fw);
    if (!sNvramSize) default_firmware();
    if (sSavePath) {  // (the Wi-Fi settings next to the save file: <name>.wfc)
        size_t n = strlen(sSavePath);
        if (n > 4 && !strcmp(sSavePath + n - 4, ".sav")) n -= 4;
        snprintf(sWfcPath, sizeof sWfcPath, "%.*s.wfc", (int)n, sSavePath);
        wfc_load();
    }
    firmware_user_settings();  // (the copy the game reads: owner, language, the touch screen's calibration)
    wm7_init(sSavePath);  // (the MAC address and the wireless channels in the user settings' copy)
    // ARM7 callbacks ready for every tag (OSSystemWork.pxiHandleChecker[1])
    ((volatile u32 *)gHostSysWork.pxiHandleChecker)[1] = 0xffffffff;
}
