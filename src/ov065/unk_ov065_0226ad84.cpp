// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/WifiLinkWork.h"
#include "net/WifiApControl.h"
#include "net/WifiApConfig.h"
#include "net/WifiApContext.h"
#include "net/SockStartupConfig.h"
#include "proto/ov065/unk_ov065_0226aca8.h"
#include "proto/ov065/unk_ov065_0226ad84.h"







extern "C" {

WifiApAllocator *sWifiApAllocator;
void *sWifiApSocketConfig;
void *sWifiApLinkWork;
WifiApContext *sWifiApContext;
WifiApControl *sWifiApControl;

u32 OS_DisableInterrupts();
void OS_RestoreInterrupts(u32);
void MIi_CpuClear32(u32, void *, u32);
void MIi_CpuCopy32(void *, void *, u32);
s32 strncmp(void *, void *, u32);
void DWCi_BM_GetApInfo(void *);
void DGT_Hash1GetDigest_R(u8 *out, void *ctx);
void DGT_Hash1SetSource(void *ctx, const void *data, u32 len);
void DGT_Hash1Reset(void *ctx);

WifiLinkWork *WifiLink_GetWork();
s32 WifiLink_Init(void *, u32);
s32 WifiAp_CleanupStep(u8 *);
s32 WifiAp_GetErrorCode2();
u8 WifiAp_StepFailedCleanup();
u8 WifiAp_ProcessConnect();
u8 WifiAp_StepRecoverLink();
u8 WifiAp_ProcessSearch();
u8 WifiAp_ProcessStartup();
u8 WifiAp_ProcessNetSetup();



s32 WifiAp_Init(WifiApConfig *cfg) {
    WifiApContext *ec;
    WifiApAllocator *f8;
    WifiApControl *fc;
    s32 r;
    fc = (WifiApControl *)cfg->allocFunc(1, sizeof(WifiApControl));  // (0x18 on the DS)
    sWifiApControl = fc;
    { volatile u32 z = 0; MIi_CpuClear32(z, sWifiApControl, sizeof(WifiApControl)); }
    fc = sWifiApControl;
    fc->unk_00 = cfg->allocFunc;
    fc->unk_04 = cfg->freeFunc;
    fc->state = 1;
    fc->furthestState = 1;
    fc->allocMask = 1;
    sWifiApContext = (WifiApContext *)WifiAp_AllocBlock(0x10, sizeof(WifiApContext));
    sWifiApLinkWork = WifiAp_AllocBlock(2, WIFI_LINK_WORK_SIZE);
    sWifiApSocketConfig = WifiAp_AllocBlock(4, sizeof(SockStartupConfig));  // (0x58 on the DS)
    sWifiApAllocator = (WifiApAllocator *)WifiAp_AllocBlock(8, sizeof(WifiApAllocator));  // (0xc on the DS)
    { volatile u32 z = 0; MIi_CpuClear32(z, sWifiApContext, sizeof(WifiApContext)); }
    { volatile u32 z = 0; MIi_CpuClear32(z, sWifiApLinkWork, WIFI_LINK_WORK_SIZE); }
    { volatile u32 z = 0; MIi_CpuClear32(z, sWifiApSocketConfig, sizeof(SockStartupConfig)); }
    { volatile u32 z = 0; MIi_CpuClear32(z, sWifiApAllocator, sizeof(WifiApAllocator)); }
    ec = sWifiApContext;
    ec->dmaNo = cfg->dmaNo;
    ec->powerMode = cfg->powerMode;
    f8 = sWifiApAllocator;
    f8->allocFunc = cfg->allocFunc;
    f8->freeFunc = cfg->freeFunc;
    f8->unk_08 = 0;
    ec->apFilter = cfg->apFilter;
    ec->netCheckMode = cfg->netCheckMode;
    DWCi_BM_GetApInfo(ec);
    r = WifiLink_Init(sWifiApLinkWork, WIFI_LINK_WORK_SIZE);
    if (r == 1 || r >= 4) {
        WifiAp_FreeAll();
        return 0;
    }
    return 1;
}

s32 WifiAp_Process() {
    u8 st = WifiAp_GetState();
    u8 r = st;
    if (st == 1) {
        r = WifiAp_ProcessStartup();
    } else if (st < 7) {
        u32 irq = OS_DisableInterrupts();
        r = WifiAp_ProcessSearch();
        WifiAp_SetState(r);
        OS_RestoreInterrupts(irq);
    } else if (st < 9) {
        r = WifiAp_ProcessConnect();
    } else if (st < 10) {
        r = WifiAp_StepRecoverLink();
    } else if (st < 16) {
        r = WifiAp_ProcessNetSetup();
    } else if (st == 17) {
        r = WifiAp_StepFailedCleanup();
    }
    WifiAp_SetState(r);
    if (r == 16) {
        s32 t = WifiAp_GetConnectResultKind();
        WifiAp_FreeContext();
        return t;
    }
    if (r == 18) {
        WifiAp_FreeContext();
        return -1;
    }
    return 0;
}

s32 WifiAp_GetStatus() {
    u32 n = WifiAp_GetState();
    if (n <= 1) {
        return 0;
    }
    if (n < 7) {
        return 1;
    }
    if (n == 9) {
        return 4;
    }
    if (n < 10) {
        return 2;
    }
    if (n == 11) {
        return 4;
    }
    if (n < 16) {
        return 3;
    }
    if (n == 16) {
        return 5;
    }
    if (n == 17) {
        return 4;
    }
    return WifiAp_GetErrorCode2();
}

u32 WifiAp_GetConnectedApType() {
    u32 r = 0xff;
    u32 n = WifiAp_GetState();
    if (n >= 0xa && n <= 0x10) {
        r = sWifiApControl->connectedApType;
    }
    return r;
}

s32 WifiAp_RequestCleanup() {
    u8 st = WifiAp_GetState();
    if (st == 0 || st == 0x12) {
        WifiAp_FreeAll();
        return 1;
    }
    WifiAp_CleanupStep(&st);
    WifiAp_SetState(st);
    return 0;
}

void WifiAp_SetApEntry(u32 idx, void *dst) {
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    MIi_CpuCopy32(dst, &c->userSettings[idx], 0xf0);
}

void *WifiAp_AllocBlock(u32 m, u32 a) {
    WifiApControl *f = (WifiApControl *)WifiAp_GetBlock(1);
    if ((f->allocMask & m) == 0) {
        f->allocMask |= m;
        return f->unk_00(m, a);
    }
    return 0;
}

void WifiAp_FreeBlock(u32 m, void *a, u32 b) {
    WifiApControl *f = (WifiApControl *)WifiAp_GetBlock(1);
    if ((f->allocMask & m) != 0) {
        f->allocMask &= ~m;
        f->unk_04(m, a, b);
    }
}

void WifiAp_FreeAll() {
    WifiApControl *f = (WifiApControl *)WifiAp_GetBlock(1);
    if ((f->allocMask & 0x10) != 0) {
        void *o = WifiAp_GetBlock(0x10);
        f->allocMask &= ~0x10;
        f->unk_04(0x10, o, sizeof(WifiApContext));
    }
    if ((f->allocMask & 8) != 0) {
        void *o = WifiAp_GetBlock(8);
        f->allocMask &= ~8;
        f->unk_04(8, o, sizeof(WifiApAllocator));
    }
    if ((f->allocMask & 4) != 0) {
        void *o = WifiAp_GetBlock(4);
        f->allocMask &= ~4;
        f->unk_04(4, o, sizeof(SockStartupConfig));
    }
    if ((f->allocMask & 2) != 0) {
        void *o = WifiAp_GetBlock(2);
        f->allocMask &= ~2;
        f->unk_04(2, o, WIFI_LINK_WORK_SIZE);
    }
    if ((f->allocMask & 1) != 0) {
        f->allocMask &= ~1;
        f->unk_04(1, f, sizeof(WifiApControl));
    }
}

void *WifiAp_GetBlock(u32 m) {
    if ((m & 1) != 0) {
        return sWifiApControl;
    }
    if ((m & 2) != 0) {
        return sWifiApLinkWork;
    }
    if ((m & 4) != 0) {
        return sWifiApSocketConfig;
    }
    if ((m & 8) != 0) {
        return sWifiApAllocator;
    }
    if ((m & 0x10) != 0) {
        return sWifiApContext;
    }
    return 0;
}

void WifiAp_SetState(u8 v) {
    WifiApControl *f = (WifiApControl *)WifiAp_GetBlock(1);
    WifiApContext *e = (WifiApContext *)WifiAp_GetBlock(0x10);
    f->state = v;
    if (v < 0x10 && v > f->furthestState) {
        f->furthestState = v;
        if (v > 7) {
            f->furthestApIndex = WifiAp_FoldApIndex(e->apType);
            f->furthestApStatus = e->foundApInfo[e->selectedAp].status;
        }
    }
}

u8 WifiAp_GetState() {
    return sWifiApControl->state;
}

void WifiAp_SetError(u32 v) {
    WifiApControl *f = (WifiApControl *)WifiAp_GetBlock(1);
    f->errorCode = v;
    f->errorState = WifiAp_GetState();
}

u32 WifiAp_GetErrorCode() {
    return sWifiApControl->errorCode;
}

void WifiAp_SetConnectedApType(u32 v) {
    sWifiApControl->connectedApType = WifiAp_FoldApIndex(v);
}

u32 WifiAp_FoldApIndex(u32 x) {
    if (x > 2) {
        x = (u8)(x - 3);
    }
    return x;
}

void WifiAp_FreeContext() {
    WifiAp_FreeBlock(8, sWifiApAllocator, 0xc);
    WifiAp_FreeBlock(0x10, sWifiApContext, sizeof(WifiApContext));
}

s32 WifiAp_GetConnectResultKind() {
    WifiApContext *e = sWifiApContext;
    u8 idx = e->selectedAp;
    u8 *cur = e->foundApBss[idx].bssid;
    u32 i;
    u32 n;
    if (e->apType >= 6) {
        return 1;
    }
    i = 0;
    n = e->numFoundAps;
    for (; i < n; i = (u8)(i + 1)) {
        if (i != (u32)idx && e->foundApInfo[i].apType < 6) {
            if (strncmp(cur, e->foundApBss[i].bssid, e->foundApBss[i].ssidLength) == 0) {
                return 2;
            }
        }
    }
    return 1;
}

void WifiAp_HashReset(void *ctx) {
    DGT_Hash1Reset(ctx);
}

void WifiAp_HashSetSource(void *ctx, const void *data, u32 len) {
    DGT_Hash1SetSource(ctx, data, len);
}

void WifiAp_HashGetDigest(u8 *out, void *ctx) {
    DGT_Hash1GetDigest_R(out, ctx);
}

BOOL WifiAp_MacEquals(u8 *a, u8 *b) {
    s32 i;
    for (i = 0; i < 6; i++) {
        if (a[i] != b[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

s32 WifiAp_GetLinkLevel() {
    u32 irq = OS_DisableInterrupts();
    WifiLinkWork *c = WifiLink_GetWork();
    s32 r = 0;
    if (c != 0 && c->phase == 9) {
        r = WifiLink_GetLinkLevel();
    }
    OS_RestoreInterrupts(irq);
    return r;
}

}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0226ad84) {
    HOST_OVERLAY_VAR(sWifiApAllocator),
    HOST_OVERLAY_VAR(sWifiApSocketConfig),
    HOST_OVERLAY_VAR(sWifiApLinkWork),
    HOST_OVERLAY_VAR(sWifiApContext),
    HOST_OVERLAY_VAR(sWifiApControl),
    HOST_OVERLAY_VARS_END
};
