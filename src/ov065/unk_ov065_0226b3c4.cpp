#include "uptr.h"
// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/WifiLinkConfig.h"
#include "net/WifiLinkSendState.h"
#include "net/WifiLinkWork.h"
#include "net/WifiApControl.h"
#include "net/WifiApConfig.h"
#include "net/WifiApNdwcshapPermTable.h"
#include "net/WifiApContext.h"
#include "net/WifiLinkEvent.h"
#include "net/SockStartupConfig.h"
#include "proto/autoload_2/unk_02114410.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_0225f1a0.h"
#include "proto/ov065/unk_ov065_0226aca8.h"
#include "proto/ov065/unk_ov065_0226ad84.h"
#include "proto/ov065/unk_ov065_0226b3c4.h"

extern "C" {
// the C library and SDK routines (src/autoload_2/unk_0212a120.c, the SDK assembly)
int strncmp(const char *a, const char *b, u32 n);
void MI_CpuCopy8(const void *src, void *dst, u32 n);
}


extern "C" {

extern const char sWifiApSsidFreespot[8] = {'F', 'R', 'E', 'E', 'S', 'P', 'O', 'T'};
extern const char sWifiApSsidWayport[8] = {'W', 'a', 'y', 'p', 'o', 'r', 't', '2'};
extern const char sWifiApSsidNintendoWfc[12] = "NINTENDOWFC";
extern const u32 sWifiApScanChannelBits[13] = {0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400, 0x8800, 0x9000, 0xa000};

char sWifiApSsidUsbConnector[12] = "NWCUSBAP";
char sWifiApHostName[12] = "NINTENDO-DS";

extern const SockStartupConfig sWifiApSocketConfigTemplate = {
    0x1000000, NULL, NULL, 1, 0, 0, 0, 0, 0, 0x1000, 0x1000, 0x2e4, 0, {0, 0, 0, 0, 0},
    sWifiApHostName,
    4,
    {0, 0},
};

}

namespace N_c750 {
extern "C" {





extern u8 gWifiLinkAnyBssid[];
extern u8 gWifiLinkAnySsid[];
extern s8 *sWifiApUsbKey1Ptr;
extern s8 *sWifiApUsbKey2Ptr;
extern s8 *sWifiApNdwcshapKey1Ptr;
extern s8 *sWifiApNdwcshapKey2Ptr;
extern u8 sWifiApUsbKeyPermTable[];
extern u8 sWifiApUsbKeySBox[];
extern u8 sWifiApNdwcshapPermTable[];

extern "C" {
s32 WifiLink_GetPhase(void);
s32 WifiLink_StartupAsync(WifiLinkConfig *a, WifiLinkNotifyCallback b);
void WifiAp_OnLinkNotify(WifiLinkEvent *m);
s32 WifiAp_BuildSearchList(s32 mode);
void WifiAp_StartScan(void *a, void *b, s32 c, u32 d);
void Sock_SetDnsServers(void *a, void *b);
s32 Sock_GetHostId(void);
s32 Sock_Startup(void *p);
s32 Sock_Cleanup(void);
s32 SockCore_TryShutdown(void);
s32 NetCheck_GetState(void);
u32 NetCheck_GetErrorCode(void);
s32 NetCheck_Abort(void);
s32 NetCheck_Destroy(void);
s32 NetCheck_Start(void);

s32 WifiAp_Base64Value(u32 c);
u32 WifiAp_PrefixToNetmask(s32 n);
u32 WifiAp_ReadAddress(u8 *p);
s32 WifiAp_Base64Decode(u8 *in, u8 *out, u32 len, u32 max);
s32 WifiAp_StepStopSockets(void);
s32 WifiAp_StepConnected(WifiApContext *c);
s32 WifiAp_StepWaitNetCheck(WifiApContext *c);
s32 WifiAp_StepStartNetCheck(void);
s32 WifiAp_StepWaitAddress(WifiApContext *c);
s32 WifiAp_StepStartSockets(WifiApContext *c);
void WifiAp_BuildSocketConfig(u8 *a, WifiApContext *b, u8 *c);
void WifiAp_ApplyStaticDns(WifiApContext *c);






















s32 WifiAp_GetNthFoundChannel(u32 n);
void WifiAp_ApplyStaticDns(WifiApContext *c);
u32 WifiAp_PrefixToNetmask(s32 n);
u32 WifiAp_ReadAddress(u8 *p);
void WifiAp_BuildSocketConfig(u8 *a, WifiApContext *b, u8 *c);

// a: the control block (its allocator first), c: the socket library's start-up settings (the DS wrote them as words
// 1-8: the allocator, then DHCP and the addresses - by member here, the 64-bit layout differs)
void WifiAp_BuildSocketConfig(u8 *a, WifiApContext *b, u8 *c) {
    SockStartupConfig *o = (SockStartupConfig *)c;
    WifiApControl *f = (WifiApControl *)a;
    MI_CpuCopy8(&sWifiApSocketConfigTemplate, o, sizeof *o);
    o->allocFunc = (SockAllocFunc)f->unk_00;
    o->freeFunc = (SockFreeFunc)f->unk_04;
    if (b->apType < 6) {
        WifiApUserSetting *q = &b->userSettings[WifiAp_FoldApIndex(b->apType)];
        if (q->ip[0] != 0) {
            o->dhcpMode = 0;
            o->ownIp = WifiAp_ReadAddress(q->ip);
            o->netmask = WifiAp_PrefixToNetmask(q->prefixLength);
            o->gateway = WifiAp_ReadAddress(q->gateway);
            o->dns1 = WifiAp_ReadAddress(q->dns[0]);
            o->dns2 = WifiAp_ReadAddress(q->dns[1]);
        } else {
            o->dhcpMode = 1;
            o->ownIp = 0;
            o->netmask = 0;
            o->gateway = 0;
            o->dns1 = 0;
            o->dns2 = 0;
        }
    }
}

u32 WifiAp_ReadAddress(u8 *p) {
    u32 x = 0;
    x |= p[0] << 24;
    x |= p[1] << 16;
    x |= p[2] << 8;
    x |= p[3];
    return ((x << 24) & 0xff000000) | (((x << 8) & 0xff0000) | (((x >> 24) & 0xff) | ((x >> 8) & 0xff00)));
}

u32 WifiAp_PrefixToNetmask(s32 n) {
    n = 0x20 - n;
    s32 i = 0;
    u32 x = -1;
    for (; i < n; i++) {
        x <<= 1;
    }
    return ((x << 24) & 0xff000000) | (((x << 8) & 0xff0000) | (((x >> 24) & 0xff) | ((x >> 8) & 0xff00)));
}

void WifiAp_ApplyStaticDns(WifiApContext *c) {
    u32 buf[2];
    if (c->apType < 6) {
        WifiApUserSetting *q = &c->userSettings[WifiAp_FoldApIndex(c->apType)];
        u32 sum = q->dns[0][3] + (q->dns[0][2] + (q->dns[0][0] + q->dns[0][1]));
        if (q->ip[0] == 0 && sum != 0) {
            buf[0] = WifiAp_ReadAddress(q->dns[0]);
            buf[1] = WifiAp_ReadAddress(q->dns[1]);
            Sock_SetDnsServers(&buf[0], &buf[1]);
        }
    }
}

s32 WifiAp_ProcessStartup(void) {
    s32 a = WifiLink_GetPhase();
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    if (a == 1) {
        WifiLinkConfig cfg;
        cfg.dmaNo = c->dmaNo;
        cfg.bssDescBuffer = NULL;
        cfg.bssDescBufferSize = 0;
        cfg.bssDescMode = 0;
        WifiAp_BuildSearchList(0);
        s32 r = WifiLink_StartupAsync(&cfg, WifiAp_OnLinkNotify);
        if (r == 1 || r >= 4) {
            WifiAp_SetError(1);
            return 0x11;
        }
    } else {
        return 1;
    }
    return 2;
}

s32 WifiAp_ProcessSearch(void) {
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    s32 a = WifiAp_GetState();
    s32 b = WifiLink_GetPhase();
    if (a == 2 && b == 3) {
        a = WifiAp_BeginSearch(c);
    } else if (a == 6) {
        a = WifiAp_CheckSearchResult(c, a);
    } else if (b == 3 || b == 6) {
        a = WifiAp_CheckSearchResult(c, a);
        if (a != 7) {
            if (a == 3) {
                a = WifiAp_StepScanAllChannels(c);
            } else if (a == 4) {
                a = WifiAp_StepScanSsids(c);
            } else if (a == 5) {
                a = WifiAp_StepScanFoundChannels(c);
            }
        }
    }
    return a;
}

void WifiAp_AddFoundChannel(u32 v) {
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    if (v > 0xd) {
        v = 0xd;
    }
    c->foundChannelMask |= 1 << (v - 1);
}

s32 WifiAp_GetNthFoundChannel(u32 n) {
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    u8 i;
    u8 cnt;
    u32 m;
    m = c->foundChannelMask;
    if (m == 0) {
        return -1;
    }
    i = 0;
    cnt = i;
    do {
        if (m & (1 << i)) {
            if (cnt == n) {
                return (s8)i;
            }
            cnt++;
        }
        i++;
    } while (i < 0xd);
    return -1;
}

void WifiAp_RescanForState(u32 r) {
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    switch (r) {
    case 3: {
        s64 t = OS_GetTick();
        c->stepStartTick = t;
        WifiAp_StartScan(gWifiLinkAnyBssid, gWifiLinkAnySsid, c->scanChannel, 0x200000);
        break;
    }
    case 4: {
        s64 t = OS_GetTick();
        c->stepStartTick = t;
        WifiApSearchEntry *e = &c->searchEntries[c->searchIndex];
        WifiAp_StartScan(gWifiLinkAnyBssid, e->ssid, e->channelIndex, 0x300000);
        break;
    }
    case 5: {
        s64 t = OS_GetTick();
        c->stepStartTick = t;
        WifiAp_StartScan(gWifiLinkAnyBssid, c->searchEntries[c->searchIndex].ssid, c->scanChannel, 0x300000);
        break;
    }
    }
}

}
}
}  // namespace N_c750

namespace N_be44 {
extern "C" {

// ov065_021: connection-state machine helpers (0x0226be44..0x0226c700)

extern u8 gWifiLinkAnyBssid[];
extern u8 gWifiLinkAnySsid[];

extern "C" {
s32 WifiAp_CleanupStep(u8 *p);
s32 WifiLink_GetPhase(void);
s32 WifiLink_EndSearchAsync(void);
s32 WifiLink_DisconnectAsync(void);
s32 WifiLink_TerminateAsync(void);
s32 WifiLink_SearchAsync(void *a, void *b, u32 c);
s32 WifiAp_GetNthFoundChannel(u32 v);
void MIi_CpuClear32(u32 v, void *dst, u32 n);

s32 WifiAp_GetWirelessOffError(void);
s32 WifiAp_MapStartupError(u32 r);
u8 WifiAp_BuildSearchFromSettings(WifiApContext *ctx);
s32 WifiAp_BuildSearchWithFreespot(WifiApContext *ctx);
s32 WifiAp_BuildSearchWithHotspots(WifiApContext *ctx);
s32 WifiAp_BuildSearchList(s32 mode);
s32 WifiAp_RestartSearch(WifiApContext *ctx);
s32 WifiAp_NextSearchPass(WifiApContext *ctx);
s32 WifiAp_FinishSearchPass(WifiApContext *ctx, s32 s);
void WifiAp_StartScan(void *a, void *b, s32 n, u32 flags);






















s32 WifiAp_GetWirelessOffError(void);
s32 WifiAp_MapStartupError(u32 r);
s32 WifiAp_GetErrorCode2(void);
u32 WifiAp_BuildSearchFromFound(s32 n, WifiApFoundInfo *p, WifiApSearchEntry *out, WMBssDesc *rec);
u8 WifiAp_BuildSearchFromSettings(WifiApContext *ctx);
s32 WifiAp_BuildSearchWithHotspots(WifiApContext *ctx);
s32 WifiAp_BuildSearchWithFreespot(WifiApContext *ctx);
s32 WifiAp_BuildSearchList(s32 mode);
void WifiAp_StartScan(void *a, void *b, s32 n, u32 flags);
s32 WifiAp_NextSearchPass(WifiApContext *ctx);
s32 WifiAp_FinishSearchPass(WifiApContext *ctx, s32 s);
s32 WifiAp_RestartSearch(WifiApContext *ctx);

s32 WifiAp_BeginSearch(WifiApContext *ctx) {
    ctx->stepStartTick = OS_GetTick();
    ctx->scanChannel = 0;
    ctx->stepStartTick = OS_GetTick();
    WifiAp_StartScan(gWifiLinkAnyBssid, gWifiLinkAnySsid, ctx->scanChannel, 0x200000);
    return 3;
}

s32 WifiAp_StepScanAllChannels(WifiApContext *ctx) {
    u64 dt = OS_GetTick() - ctx->stepStartTick;
    if ((dt << 6) / 0x82ea >= 0x12c) {
        ctx->scanChannel = ctx->scanChannel + 2;
        if (ctx->scanChannel >= 0xd) {
            return WifiAp_FinishSearchPass(ctx, 3);
        }
        ctx->stepStartTick = OS_GetTick();
        WifiAp_StartScan(gWifiLinkAnyBssid, gWifiLinkAnySsid, ctx->scanChannel, 0x200000);
    }
    return 3;
}

s32 WifiAp_RestartSearch(WifiApContext *ctx) {
    ctx->stepCount = 0;
    ctx->searchRestarts = ctx->searchRestarts + 1;
    WifiAp_BuildSearchList(0);
    ctx->scanChannel = 1;
    return 3;
}

s32 WifiAp_StepScanSsids(WifiApContext *ctx) {
    u64 dt = OS_GetTick() - ctx->stepStartTick;
    if ((dt << 6) / 0x82ea >= 0x96 || ctx->searchEntries[ctx->searchIndex].lo == 1) {
        ctx->searchEntries[ctx->searchIndex].lo = 0;
        ctx->searchIndex++;
        if (ctx->numSearchEntries <= ctx->searchIndex) {
            ctx->searchIndex = 0;
            return WifiAp_FinishSearchPass(ctx, 4);
        }
        ctx->stepStartTick = OS_GetTick();
        WifiAp_StartScan(gWifiLinkAnyBssid, ctx->searchEntries[ctx->searchIndex].ssid, ctx->searchEntries[ctx->searchIndex].channelIndex, 0x300000);
    }
    return 4;
}

s32 WifiAp_StepScanFoundChannels(WifiApContext *ctx) {
    u64 dt = OS_GetTick() - ctx->stepStartTick;
    if ((dt << 6) / 0x82ea >= 0x96 || ctx->searchEntries[ctx->searchIndex].lo == 1) {
        ctx->searchEntries[ctx->searchIndex].lo = 0;
        ctx->searchIndex++;
        if (ctx->numSearchEntries == ctx->searchIndex) {
            ctx->stepCount++;
            ctx->searchIndex = 0;
            ctx->scanChannel = WifiAp_GetNthFoundChannel(ctx->stepCount);
        }
        if (ctx->scanChannel < 0) {
            ctx->stepCount = 0;
            return WifiAp_FinishSearchPass(ctx, 5);
        }
        ctx->stepStartTick = OS_GetTick();
        WifiAp_StartScan(gWifiLinkAnyBssid, ctx->searchEntries[ctx->searchIndex].ssid, ctx->scanChannel, 0x300000);
    }
    return 5;
}

s32 WifiAp_FinishSearchPass(WifiApContext *ctx, s32 s) {
    switch (s) {
    case 3:
        if (ctx->numFoundAps != 0 || ctx->foundChannelMask != 0) {
            if (WifiAp_BuildSearchList(1) != 0) {
                s = 4;
            } else {
                s = WifiAp_NextSearchPass(ctx);
            }
        } else if (ctx->searchRestarts < 1) {
            s = WifiAp_RestartSearch(ctx);
        } else {
            s = 6;
        }
        break;
    case 4:
        s = WifiAp_NextSearchPass(ctx);
        break;
    case 5:
        if (ctx->searchRestarts < 1) {
            s = WifiAp_RestartSearch(ctx);
        } else {
            s = 6;
        }
        break;
    }
    WifiAp_RescanForState(s);
    return s;
}

s32 WifiAp_CheckSearchResult(WifiApContext *ctx, s32 s) {
    u8 i;
    u8 n;
    if (s == 0x11) {
        return s;
    }
    i = 0;
    n = ctx->numFoundAps;
    for (; i < n; i++) {
        if (ctx->foundApInfo[i].status == 0) {
            break;
        }
    }
    if (s == 6) {
        if (n != i) {
            goto reset;
        }
        if (i == 0) {
            WifiAp_SetError(5);
        } else {
            WifiAp_SetError(6);
        }
        return 0x11;
    }
    if (n == 0) {
        return s;
    }
    if (n == i) {
        return s;
    }
    if (ctx->foundApInfo[i].linkLevel < 0x14) {
        return s;
    }
reset:
    ctx->selectedAp = i;
    if (WifiLink_EndSearchAsync() != 1) {
        ctx->resumeState = s;
        s = 7;
    }
    return s;
}

s32 WifiAp_NextSearchPass(WifiApContext *ctx) {
    if (ctx->foundChannelMask != 0 && WifiAp_BuildSearchList(2) != 0) {
        ctx->scanChannel = WifiAp_GetNthFoundChannel(0);
        return 5;
    }
    if (ctx->searchRestarts < 1) {
        return WifiAp_RestartSearch(ctx);
    }
    return 6;
}

void WifiAp_StartScan(void *a, void *b, s32 n, u32 flags) {
    if (n > 0xc) {
        n = 0xc;
    }
    WifiLink_SearchAsync(a, b, flags | sWifiApScanChannelBits[n]);
}

s32 WifiAp_StepRecoverLink(void) {
    WifiApContext *ctx = (WifiApContext *)WifiAp_GetBlock(0x10);
    u32 st = 9;
    switch (WifiLink_GetPhase()) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        st = ctx->resumeState;
        if (ctx->altApTypePending == 1) {
            ctx->foundApInfo[ctx->selectedAp].status = 0;
            st = 7;
        } else if (st >= 3 && st <= 5) {
            WifiAp_RescanForState(st);
        }
        break;
    case 4:
    case 5:
        break;
    case 6:
        WifiLink_EndSearchAsync();
        break;
    case 7:
    case 8:
        break;
    case 9:
        WifiLink_DisconnectAsync();
        break;
    case 10:
        break;
    case 12:
        WifiLink_TerminateAsync();
        WifiAp_SetError(4);
        st = 0x11;
        break;
    case 11:
        WifiAp_SetError(0);
        st = 0x11;
        break;
    }
    return st;
}

s32 WifiAp_BuildSearchList(s32 mode) {
    WifiApContext *ctx = (WifiApContext *)WifiAp_GetBlock(0x10);
    volatile s32 z = 0;
    MIi_CpuClear32(z, ctx->searchEntries, 0x144);
    switch (mode) {
    case 0:
        ctx->numSearchEntries = WifiAp_BuildSearchWithFreespot(ctx);
        break;
    case 1:
        ctx->numSearchEntries = WifiAp_BuildSearchFromFound(ctx->numFoundAps, ctx->foundApInfo, ctx->searchEntries, ctx->foundApBss);
        break;
    case 2:
        ctx->numSearchEntries = WifiAp_BuildSearchWithHotspots(ctx);
        break;
    }
    return ctx->numSearchEntries;
}

s32 WifiAp_IsFreespot(WMBssDesc *rec) {
    if (strncmp((const char *)rec->ssid, sWifiApSsidFreespot, 8) == 0) {
        return 8;
    }
    return 0;
}

s32 WifiAp_BuildSearchWithFreespot(WifiApContext *ctx) {
    u8 n;
    WifiApSearchEntry *out = ctx->searchEntries;
    n = WifiAp_BuildSearchFromSettings(ctx);
    out += n;
    if (ctx->apFilter == 0 || ctx->apFilter == 6) {
        MI_CpuCopy8(sWifiApSsidFreespot, out->ssid, 8);
        out->ssidLength = 8;
        out->apType = 8;
        n++;
    }
    return n;
}

s32 WifiAp_BuildSearchWithHotspots(WifiApContext *ctx) {
    u8 n;
    WifiApSearchEntry *out = ctx->searchEntries;
    n = WifiAp_BuildSearchFromSettings(ctx);
    out += n;
    if (ctx->apFilter == 0 || ctx->apFilter == 4) {
        MI_CpuCopy8(sWifiApSsidUsbConnector, out->ssid, 8);
        out->ssidLength = 8;
        out->apType = 6;
        n++;
        out++;
    }
    if (ctx->apFilter == 0 || ctx->apFilter == 7) {
        MI_CpuCopy8(sWifiApSsidWayport, out->ssid, 8);
        out->ssidLength = 8;
        out->apType = 9;
        n++;
        out++;
    }
    if (ctx->apFilter == 0 || ctx->apFilter == 8) {
        MI_CpuCopy8(sWifiApSsidNintendoWfc, out->ssid, 0xb);
        out->ssidLength = 0xb;
        out->apType = 0xa;
        n++;
    }
    return n;
}

u8 WifiAp_BuildSearchFromSettings(WifiApContext *ctx) {
    s32 i;
    u8 cnt;
    WifiApUserSetting *q;
    WifiApSearchEntry *out;
    cnt = 0;
    q = ctx->userSettings;
    out = ctx->searchEntries;
    for (i = 0; i < 3; q++, i++) {
        u32 lo = ctx->apFilter;
        if (lo == 0 || lo == i + 1) {
            if (q->setType != 0xff) {
                u8 k = 0;
                BOOL ok;
                do {
                    u8 c = q->ssid[0][k];
                    if (c == 0) {
                        break;
                    }
                    out->ssid[k] = c;
                    k++;
                } while (k < 0x20);
                if (k != 0) {
                    out->ssidLength = k;
                    out->apType = i;
                    ok = TRUE;
                } else {
                    ok = FALSE;
                }
                if (ok) {
                    cnt++;
                    out++;
                }
                if (q->setType == 1) {
                    u8 k2 = 0;
                    BOOL ok2;
                    do {
                        u8 c = q->ssid[1][k2];
                        if (c == 0) {
                            break;
                        }
                        out->ssid[k2] = c;
                        k2++;
                    } while (k2 < 0x20);
                    if (k2 != 0) {
                        out->ssidLength = k2;
                        out->apType = i + 3;
                        ok2 = TRUE;
                    } else {
                        ok2 = FALSE;
                    }
                    if (ok2) {
                        cnt++;
                        out++;
                    }
                }
            }
        }
    }
    return cnt;
}

u32 WifiAp_BuildSearchFromFound(s32 n, WifiApFoundInfo *p, WifiApSearchEntry *out, WMBssDesc *rec) {
    u8 cnt = 0;
    u8 i = 0;
    if (n > 0) {
        do {
            if (i >= 9) {
                break;
            }
            if (p->status == 0 && rec->channel != p->channel) {
                u8 k = 0;
                do {
                    u8 c = rec->ssid[k];
                    if (c == 0) {
                        break;
                    }
                    out->ssid[k] = c;
                    k++;
                } while (k < 0x20);
                out->ssidLength = k;
                out->channelIndex = rec->channel - 1;
                out++;
                cnt++;
            }
            p++;
            rec++;
            i++;
        } while (i < n);
    }
    return cnt;
}

s32 WifiAp_StepFailedCleanup(void) {
    if (WifiAp_CleanupStep(&((WifiApControl *)WifiAp_GetBlock(1))->errorState) == 1) {  // (0xa on the DS)
        return 0x12;
    }
    return 0x11;
}

s32 WifiAp_GetErrorCode2(void) {
    WifiApControl *p = (WifiApControl *)WifiAp_GetBlock(1);
    s32 n = WifiAp_GetErrorCode();
    if (n < 4) {
        return WifiAp_MapStartupError(n);
    }
    if (n < 5) {
        return WifiAp_GetWirelessOffError();
    }
    if (n == 5) {
        return WifiAp_GetNoApErrorCode(p);
    }
    return WifiAp_GetErrorCodeForState(p);
}

s32 WifiAp_MapStartupError(u32 r) {
    switch (r) {
    case 1:
        return -9;
    case 0:
        return -10;
    case 2:
        return -8;
    case 3:
        return -7;
    }
    return 0;
}

s32 WifiAp_GetWirelessOffError(void) {
    return -6;
}

s32 WifiAp_GetNoApErrorCode(WifiApControl *p) {
    if (p->anyApFound == 0) {
        return -0xc3b3;
    }
    return -0xc79b;
}

}
}
}  // namespace N_be44

namespace N_b488 {
extern "C" {




extern "C" {
void WifiAp_SortFoundAp(u32 n, WifiApContext *ctx);
s32 WifiAp_IsUsbConnectorAp(void *p);
s32 WifiAp_IsNdwcshapAp(void *p);
s32 WifiAp_GetUsbApWepKey(void *a, void *b);
s32 WifiAp_GetNdwcshapWepKey(void *a, void *b);
s32 WifiLink_GetPhase(void);
s32 WifiLink_ConnectAsync(void *rec, void *buf, u32 v);
s32 WifiLink_Finish(void);
s32 WifiLink_CleanupAsync(void);
s32 WifiLink_EndSearchAsync(void);
s32 WifiLink_DisconnectAsync(void);
s32 WifiLink_TerminateAsync(void);
s32 SockCore_TryShutdown(void);
s32 Sock_Cleanup(void);
s32 NetCheck_Abort(void);
s32 NetCheck_Destroy(void);
void MIi_CpuCopy32(void *src, void *dst, u32 n);
void MI_CpuFill8(void *dst, s32 v, u32 n);

s32 WifiAp_MatchSpecialSsid(WMBssDesc *rec);
s32 WifiAp_MatchSearchEntry(WMBssDesc *rec, s32 n, WifiApSearchEntry *e);
s32 WifiAp_AddFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx);
void WifiAp_OnApFound(WMBssDesc *rec);
u32 WifiAp_GetAuthOption(WifiApContext *ctx);
u32 WifiAp_GetPowerOption(WifiApContext *ctx);
u32 WifiAp_SelectApType(WifiApContext *ctx);
BOOL WifiAp_GetWepSetting(WifiApContext *ctx, u32 idx, u8 *out);
s32 WifiAp_PrepareConnect(WifiApContext *ctx);
s32 WifiAp_StepConnect(WifiApContext *ctx);
s32 WifiAp_StepLinkShutdown(void);
s32 WifiAp_StopSocketLayer(void);

static inline u32 Unk_ov065_0226b488_Level(u16 f) {
    u32 v;
    if (f & 2) {
        v = ((u32)f << 22) >> 24;
    } else {
        v = (u8)(((s32)f >> 2) + 0x19);
    }
    return v;
}








struct WifiApWepModeBits {
    u8 v : 2;
};













void WifiAp_UpdateFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx);
void WifiAp_StoreNewFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx);
s32 WifiAp_AddFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx);
s32 WifiAp_MatchSearchEntry(WMBssDesc *rec, s32 n, WifiApSearchEntry *e);
s32 WifiAp_MatchSpecialSsid(WMBssDesc *rec);
void WifiAp_OnApFound(WMBssDesc *rec);
void WifiAp_OnLinkNotify(WifiLinkEvent *m);
BOOL WifiAp_GetWepSetting(WifiApContext *ctx, u32 idx, u8 *out);
u32 WifiAp_GetAuthOption(WifiApContext *ctx);
u32 WifiAp_GetPowerOption(WifiApContext *ctx);
u32 WifiAp_SelectApType(WifiApContext *ctx);
s32 WifiAp_StepConnect(WifiApContext *ctx);
s32 WifiAp_PrepareConnect(WifiApContext *ctx);
s32 WifiAp_StopSocketLayer(void);
s32 WifiAp_StepLinkShutdown(void);
s32 WifiAp_CleanupStep(u8 *p);

s32 WifiAp_GetErrorCodeForState(WifiApControl *o) {
    s32 r;
    if (o->furthestState < 10) {
        if (o->furthestApStatus == 3) {
            r = (s32)0xffff3864 - o->furthestApIndex;
        } else if (o->furthestApStatus == 4) {
            r = (s32)0xffff3800 - o->furthestApIndex;
        } else {
            r = (s32)0xffff379c - o->furthestApIndex;
        }
    } else if (o->furthestState < 13) {
        r = (s32)0xffff34e0 - o->furthestApIndex;
    } else {
        r = o->netCheckError;
        if (r == 0) {
            r = (s32)0xffff3cb0 - o->furthestApIndex;
        } else if (r == -1) {
            r = (s32)0xffff347c - o->furthestApIndex;
        } else if (r == -2) {
            r = (s32)0xffff3418 - o->furthestApIndex;
        } else if (r == -3) {
            r = (s32)0xffff33b4 - o->furthestApIndex;
        } else if (r == -4) {
            r = (s32)0xffff30f8 - o->furthestApIndex;
        } else if (r == -5) {
            r = (s32)0xffff3094 - o->furthestApIndex;
        } else if (r == -6) {
            r = (s32)0xffff3030 - o->furthestApIndex;
        }
    }
    return r;
}

s32 WifiAp_CleanupStep(u8 *p) {
    if (*p <= 10) {
        s32 r = WifiAp_StepLinkShutdown();
        if (r == 1) {
            *p = 0;
            return 1;
        }
        if (r == -1) {
            *p = 0x12;
            return 1;
        }
    } else if (*p == 0xe) {
        NetCheck_Abort();
        NetCheck_Destroy();
        *p = 0xc;
    } else if (*p < 0x12) {
        if (WifiAp_StopSocketLayer() == 1) {
            *p = 10;
        }
    }
    return 0;
}

s32 WifiAp_StepLinkShutdown(void) {
    switch (WifiLink_GetPhase()) {
    case 0:
        return 1;
    case 1:
        WifiLink_Finish();
        break;
    case 2:
        break;
    case 3:
        WifiLink_CleanupAsync();
        break;
    case 4:
    case 5:
        break;
    case 6:
        WifiLink_EndSearchAsync();
        break;
    case 7:
    case 8:
        break;
    case 9:
        WifiLink_DisconnectAsync();
        break;
    case 10:
        break;
    case 12:
        WifiLink_TerminateAsync();
        break;
    case 11:
        WifiAp_SetError(0);
        return -1;
    }
    return 0;
}

s32 WifiAp_StopSocketLayer(void) {
    if (SockCore_TryShutdown() != 0) {
        return 0;
    }
    s32 r = Sock_Cleanup();
    if (r == 0 || r == -0x27) {
        return 1;
    }
    return 0;
}

s32 WifiAp_ProcessConnect(void) {
    s32 r = WifiAp_GetState();
    WifiApContext *ctx = (WifiApContext *)WifiAp_GetBlock(0x10);
    switch (r) {
    case 7:
        r = WifiAp_PrepareConnect(ctx);
        break;
    case 8:
        r = WifiAp_StepConnect(ctx);
        break;
    }
    return r;
}

s32 WifiAp_PrepareConnect(WifiApContext *ctx) {
    WMBssDesc *rec = ctx->foundApBss + ctx->selectedAp;
    ctx->apType = WifiAp_SelectApType(ctx);
    MI_CpuFill8(ctx->wepSetting, 0, 0x52);
    if (WifiAp_GetWepSetting(ctx, ctx->apType, ctx->wepSetting) != 0) {
        ctx->useSharedKey = 1;
        if ((((s32)rec->capaInfo >> 4) & 1) == 0) {
            ctx->foundApInfo[ctx->selectedAp].status = 3;
            return 9;
        }
        if (ctx->apType == 6 && rec->ssid[9] == 0) {
            ctx->foundApInfo[ctx->selectedAp].status = 3;
            return 9;
        }
    } else {
        ctx->useSharedKey = 0;
        if ((((s32)rec->capaInfo >> 4) & 1) == 1) {
            ctx->foundApInfo[ctx->selectedAp].status = 3;
            return 9;
        }
    }
    ctx->stepCount = 0;
    ctx->connectFailKind = 0;
    return 8;
}

s32 WifiAp_StepConnect(WifiApContext *ctx) {
    s32 s = WifiLink_GetPhase();
    WMBssDesc *rec = ctx->foundApBss + ctx->selectedAp;
    u32 r6;
    if (s == 3) {
        r6 = WifiAp_GetPowerOption(ctx);
        ctx->stepCount = ctx->stepCount + 1;
        if (ctx->stepCount > 3) {
            ctx->stepCount = 0;
            ctx->foundApInfo[ctx->selectedAp].status = 1;
            return 9;
        }
        if (ctx->stepCount != 1) {
            if (ctx->connectFailKind == 1) {
                ctx->useSharedKey = 0;
            } else if (ctx->connectFailKind == 2) {
                ctx->stepCount = 0;
                ctx->foundApInfo[ctx->selectedAp].status = 3;
                return 9;
            } else if (ctx->connectFailKind == 3) {
                ctx->stepCount = 0;
                ctx->foundApInfo[ctx->selectedAp].status = 4;
                return 9;
            }
        }
        WifiLink_ConnectAsync(rec, ctx->wepSetting, r6 | WifiAp_GetAuthOption(ctx));
    } else if (s == 9) {
        s64 t;
        ctx->stepCount = 0;
        t = OS_GetTick();
        ctx->stepStartTick = t;
        return 10;
    }
    return 8;
}

u32 WifiAp_SelectApType(WifiApContext *ctx) {
    struct {
        WMBssDesc *rec;
        s32 result;
        s32 i;
        u8 *d;
    } l;
    l.rec = ctx->foundApBss + ctx->selectedAp;
    l.result = 0;
    if (ctx->altApTypePending == 0) {
        u32 cnt = l.result;
        u32 proto = l.rec->ssidLength;
        if (proto == 0x20) {
            l.result = WifiAp_MatchSpecialSsid(l.rec);
            if (l.result > 0) {
                cnt++;
            } else {
                l.result = 0;
            }
        } else if (proto == 8) {
            l.result = WifiAp_IsFreespot(l.rec);
            if (l.result != 0) {
                cnt++;
            } else {
                l.result = 0;
            }
        }
        l.i = 0;
        s32 n = ctx->numSearchEntries;
        if (n > 0) {
            WifiApSearchEntry *e;
            l.d = ctx->searchEntries[0].ssid;
            e = ctx->searchEntries;
            do {
                u32 pr = l.rec->ssidLength;
                if (pr == e->ssidLength && strncmp((const char *)l.rec->ssid, (const char *)l.d, pr) == 0) {
                    if (cnt == 0) {
                        l.result = e->apType;
                    } else {
                        e->hi = 1;
                        ctx->altApTypePending = 1;
                    }
                    cnt++;
                }
                l.d += sizeof(WifiApSearchEntry);
                e++;
                l.i++;
            } while (l.i < ctx->numSearchEntries);
        }
    } else {
        WifiApSearchEntry *e;
        s32 cnt;
        s32 i = l.result;
        cnt = i;
        if (i < ctx->numSearchEntries) {
            e = ctx->searchEntries;
            do {
                if (e->hi == 1) {
                    if (cnt == 0) {
                        e->hi = 0;
                        l.result = e->apType;
                    }
                    cnt++;
                }
                e++;
                i++;
            } while (i < ctx->numSearchEntries);
        }
        if (cnt == 1) {
            ctx->altApTypePending = 0;
        }
    }
    return (u8)l.result;
}

u32 WifiAp_GetPowerOption(WifiApContext *ctx) {
    if (ctx->powerMode == 1) {
        return 0x30000;
    }
    return 0x20000;
}

u32 WifiAp_GetAuthOption(WifiApContext *ctx) {
    if (ctx->useSharedKey == 1) {
        return 0xc0000;
    }
    return 0x80000;
}

BOOL WifiAp_GetWepSetting(WifiApContext *ctx, u32 idx, u8 *out) {
    WifiApUserSetting *c = ctx->userSettings;
    switch (idx) {
    case 2:
        c++;
    case 1:
        c++;
    case 0:
        out[0] = c->wepMode;
        MI_CpuCopy8(c->wepKey, out + 2, 0x50);
        break;
    case 5:
        c++;
    case 4:
        c++;
    case 3:
        out[0] = 1;
        MI_CpuCopy8(c->key2, out + 2, 0x14);
        out[0x16] = 0;
        break;
    case 6:
        out[0] = 2;
        WifiAp_GetUsbApWepKey(ctx->foundApBss[ctx->selectedAp].ssid, out + 2);
        break;
    case 7:
        out[0] = 2;
        WifiAp_GetNdwcshapWepKey(ctx->foundApBss[ctx->selectedAp].ssid, out + 2);
        break;
    case 8:
    case 9:
        break;
    }
    if (out[0] != 0) {
        return TRUE;
    }
    return FALSE;
}

void WifiAp_OnLinkNotify(WifiLinkEvent *m) {
    WifiApContext *ctx = (WifiApContext *)WifiAp_GetBlock(0x10);
    if (m->request == 5) {
        if (m->result != 0) {
            switch (m->detail) {
            case 0xd:
                ctx->connectFailKind = 1;
                break;
            case 0xf:
                ctx->connectFailKind = 2;
                break;
            case 0x11:
                ctx->connectFailKind = 3;
                break;
            default:
                ctx->connectFailKind = 4;
                break;
            }
        }
    } else if (m->request == 7) {
        WifiAp_OnApFound(m->bssDesc);
    }
}

void WifiAp_OnApFound(WMBssDesc *rec) {
    s32 r6 = -1;
    WifiApContext *ctx = (WifiApContext *)WifiAp_GetBlock(0x10);
    ((u8 *)WifiAp_GetBlock(1))[0xb] = 1;
    switch (WifiAp_GetState()) {
    case 3: {
        u16 proto = rec->ssidLength;
        u8 c;
        if (proto == 0 || (c = rec->ssid[0]) == 0) {
            WifiAp_AddFoundChannel(rec->channel);
        } else if (proto == 1 || c == 0x20) {
            WifiAp_AddFoundChannel(rec->channel);
            r6 = WifiAp_MatchSearchEntry(rec, ctx->numSearchEntries, ctx->searchEntries);
        } else {
            r6 = WifiAp_MatchSearchEntry(rec, ctx->numSearchEntries, ctx->searchEntries);
        }
        break;
    }
    case 4:
    case 5:
        r6 = WifiAp_MatchSearchEntry(rec, 1, ctx->searchEntries + ctx->searchIndex);
        if (r6 >= 0) {
            ctx->searchEntries[ctx->searchIndex].lo = 1;
        }
        break;
    default:
        return;
    }
    if (r6 >= 0) {
        WifiAp_SortFoundAp(WifiAp_AddFoundAp(r6, rec, ctx), ctx);
    }
}

s32 WifiAp_MatchSpecialSsid(WMBssDesc *rec) {
    WifiApContext *ctx = (WifiApContext *)WifiAp_GetBlock(0x10);
    if (ctx->apFilter == 0 || ctx->apFilter == 4) {
        if ((u8)(((s32)rec->capaInfo >> 4) & 1) == 1) {
            if (WifiAp_IsUsbConnectorAp(rec->ssid) == 1) {
                return 6;
            }
        }
    }
    if (ctx->apFilter == 0 || ctx->apFilter == 5) {
        if ((u8)(((s32)rec->capaInfo >> 4) & 1) == 1) {
            if (WifiAp_IsNdwcshapAp(rec->ssid) == 1) {
                return 7;
            }
        }
    }
    return -1;
}

s32 WifiAp_MatchSearchEntry(WMBssDesc *rec, s32 n, WifiApSearchEntry *e) {
    s32 i;
    u16 proto;
    if (rec->ssidLength == 0x20) {
        s32 r = WifiAp_MatchSpecialSsid(rec);
        if (r > 0) {
            return r;
        }
    }
    i = 0;
    if (n > 0) {
        proto = rec->ssidLength;
        do {
            if ((u8)proto == e->ssidLength && strncmp((const char *)rec->ssid, (const char *)e->ssid, proto) == 0) {
                return e->apType;
            }
            e++;
            i++;
        } while (i < n);
    }
    return -1;
}

s32 WifiAp_AddFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx) {
    s32 i = 0;
    s32 found = -1;
    WMBssDesc *f;
    u8 *e;
    u32 b0;
    s32 n;
    n = ctx->numFoundAps;
    if (n > 0) {
        f = ctx->foundApBss;
        b0 = rec->bssid[0];
        do {
            e = f->bssid;
            if (b0 == e[0] && rec->bssid[1] == e[1] && rec->bssid[2] == e[2] &&
                rec->bssid[3] == e[3] && rec->bssid[4] == e[4] && rec->bssid[5] == e[5]) {
                found = i;
                break;
            }
            f++;
            i++;
        } while (i < n);
    }
    if (found == -1) {
        WifiAp_StoreNewFoundAp((u8)a, rec, ctx);
        if (ctx->numFoundAps < 10) {
            ctx->numFoundAps++;
        }
        found = 10;
    } else {
        WifiAp_UpdateFoundAp(found, rec, ctx);
    }
    return found;
}

void WifiAp_StoreNewFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx) {
    WifiApFoundInfo *p = &ctx->foundApInfo[10];
    WMBssDesc *q = &ctx->foundApBss[10];
    p->apType = a;
    u16 f = rec->rssi;
    p->linkLevel = (u8)Unk_ov065_0226b488_Level(f);
    p->channel = ctx->scanChannel + 1;
    MIi_CpuCopy32(rec, q, sizeof(WMBssDesc));
}

void WifiAp_UpdateFoundAp(u32 a, WMBssDesc *rec, WifiApContext *ctx) {
    WifiApFoundInfo *p = &ctx->foundApInfo[a];
    WMBssDesc *q = &ctx->foundApBss[a];
    u16 f = rec->rssi;
    u8 w = (u8)Unk_ov065_0226b488_Level(f);
    if (w > p->linkLevel) {
        p->linkLevel = w;
        p->channel = ctx->scanChannel + 1;
    }
    MIi_CpuCopy32(rec, q, sizeof(WMBssDesc));
}

}
}
}  // namespace N_b488

namespace N_ab40 {
extern "C" {

// ov065_019: network library, connection/event state (0x0226ab40..0x0226b3c4)


typedef void (*WifiLinkRecvCallback)(void *, void *, void *, u32);









extern "C" {

extern WifiLinkSendState sWifiLinkSendState;
extern u8 sWifiLinkSendLock[];
extern WifiApAllocator *sWifiApAllocator;
extern WifiApControl *sWifiApControl;

u32 OS_DisableInterrupts();
void OS_RestoreInterrupts(u32);
s32 func_02133150(s32, s32);
void MIi_CpuClear32(u32, void *, u32);
void MIi_CpuCopy32(void *, void *, u32);
s32 WM_SetDCFData(void *, void *, void *, u32);
void DWCi_BM_GetApInfo(void *);

WifiLinkWork *WifiLink_GetWork();
s32 WifiLink_Init(void *, u32);
s32 WifiLink_UnlockFromIrq(void *);
s32 WifiLink_TryLockFromIrq(void *);
void WifiLink_OnKeepAliveSent();
s32 WifiAp_CleanupStep(u8 *);
s32 WifiAp_GetErrorCode2();
u8 WifiAp_ProcessNetSetup();






































void WifiAp_SortFoundAp(u32 n, WifiApContext *ctx) {
    WifiApFoundInfo *keys = ctx->foundApInfo;
    WMBssDesc *recs = ctx->foundApBss;
    s32 j = n - 1;
    if (j >= 0) {
        WifiApFoundInfo *p4 = &keys[j];
        WMBssDesc *p6 = &recs[j];
        do {
            WifiApFoundInfo tk;
            WMBssDesc tr;
            if (keys[n].linkLevel < p4->linkLevel) {
                break;
            }
            MIi_CpuCopy32(p4, &tk, 4);
            MIi_CpuCopy32(&keys[n], p4, 4);
            MIi_CpuCopy32(&tk, &keys[n], 4);
            MIi_CpuCopy32(p6, &tr, 0xc0);
            MIi_CpuCopy32(&recs[n], p6, 0xc0);
            MIi_CpuCopy32(&tr, &recs[n], 0xc0);
            n = j;
            p4--;
            p6--;
            j--;
        } while (j >= 0);
    }
    { volatile u32 z = 0; keys += 10; MIi_CpuClear32(z, keys, 4); }
    { volatile u32 z = 0; MIi_CpuClear32(z, recs + 10, 0xc0); }
}

}
}
}  // namespace N_ab40

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0226b3c4) {
    HOST_OVERLAY_VAR(sWifiApSsidUsbConnector),
    HOST_OVERLAY_VAR(sWifiApHostName),
    HOST_OVERLAY_VARS_END
};
