// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/WifiApNdwcshapPermTable.h"
#include "net/DwcHttp.h"
#include "net/WifiApContext.h"
#include "net/WifiApControl.h"
#include "net/NasAuthWork.h"
#include "types64.h"
#include "proto/autoload_2/unk_02114410.h"
#include "proto/autoload_2/unk_02115468.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_0226ad84.h"
#include "proto/ov065/unk_ov065_0226cb18.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_0225f1a0.h"
extern "C" {
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void MI_CpuFill8(void *dst, u32 data, u32 size);
}
#pragma opt_strength_reduction off

extern "C" {

const u8 sWifiApUsbKeyPermTable[16] = {0x05, 0x01, 0x0c, 0x04, 0x02, 0x03, 0x0a, 0x00, 0x0b, 0x07, 0x09, 0x08, 0x06, 0x00, 0x00, 0x00};
const u8 sWifiApUsbKeySBox[16] = {0x0a, 0x0d, 0x0e, 0x08, 0x09, 0x03, 0x06, 0x00, 0x0c, 0x05, 0x02, 0x07, 0x0b, 0x01, 0x0f, 0x04};

s8 sWifiApUsbKey1[16] = {0x67, 0x77, 0x69, 0x27, 0x36, 0x26, 0x66, 0x73, 0x3d, 0x30, 0x4e, 0x66, 0x7e, 0, 0, 0};
s8 sWifiApUsbKey2[16] = {0x25, 0x28, 0x65, 0x67, 0x45, 0x72, 0x29, 0x61, 0x67, 0x28, 0x73, 0x26, 0x6d, 0, 0, 0};
u8 sWifiApNdwcshapPermTable[24] = {0x17, 0x14, 0x11, 0x0d, 0x0b, 0x06, 0x0f, 0x0e, 0x09, 0x15, 0x0c, 0x04, 0x02, 0x01, 0x12, 0x10, 0x05, 0x03, 0x13, 0x0a, 0x07, 0x08, 0x00, 0x16};
s8 sWifiApNdwcshapKey2[28] = {0x33, 0x38, 0x67, 0x36, 0x7a, 0x78, 0x6a, 0x6b, 0x32, 0x30, 0x67, 0x76, 0x6d, 0x76, 0x5d, 0x36, 0x5e, 0x3d, 0x6a, 0x26, 0x25, 0x76, 0x59, 0x31, 0, 0, 0, 0};
s8 sWifiApNdwcshapKey1[28] = {0x39, 0x35, 0x32, 0x75, 0x79, 0x62, 0x6a, 0x6e, 0x70, 0x6d, 0x75, 0x39, 0x30, 0x33, 0x62, 0x69, 0x61, 0x40, 0x62, 0x6b, 0x35, 0x6d, 0x5b, 0x2d, 0, 0, 0, 0};

s8 *sWifiApNdwcshapKey1Ptr = sWifiApNdwcshapKey1;
s8 *sWifiApUsbKey1Ptr = sWifiApUsbKey1;
s8 *sWifiApNdwcshapKey2Ptr = sWifiApNdwcshapKey2;
s8 *sWifiApUsbKey2Ptr = sWifiApUsbKey2;

}

namespace N_d080 {
extern "C" {











typedef NasAuthWork S;

extern "C" {
extern S *sNasAuth;
extern NasUserIdInfo sNasUserId;
extern u32 data_0220064c;
extern char data_ov065_0228b708[];
extern char data_ov065_0228b714[];
extern char *sNasLangCodeTable[];
extern char data_ov065_0228b798[];
extern char data_ov065_0228b7a0[];
extern char data_ov065_0228b7ac[];
extern char data_ov065_0228b7c8[];
extern char data_ov065_0228b7d0[];
extern char data_ov065_0228b7dc[];
extern char data_ov065_0228b7e4[];
extern char data_ov065_0228b7ec[];
extern char data_ov065_0228b7f4[];
extern char data_ov065_0228b7fc[];
extern char data_ov065_0228b804[];
extern char data_ov065_0228b80c[];
extern char data_ov065_0228b814[];
extern char data_ov065_0228b81c[];
extern char data_ov065_0228b838[];
extern char data_ov065_0228b848[];
extern char data_ov065_0228b850[];
extern char data_ov065_0228b858[];
extern char data_ov065_0228b860[];
extern char data_ov065_0228b864[];
extern char data_ov065_0228b86c[];
extern char data_ov065_0228b874[];
extern char data_ov065_0228b87c[];
extern char data_ov065_0228b884[];
extern char data_ov065_0228b88c[];
extern char data_ov065_0228b894[];
extern char data_ov065_0228b8a0[];
extern char data_ov065_0228b8b4[];
extern char data_ov065_0228b8c4[];
extern char data_ov065_0228b8cc[];
extern char data_ov065_0228b8d8[];
extern char data_ov065_0228b8e4[];
extern char data_ov065_0228b8f0[];
extern char data_ov065_0228b8f8[];
extern char data_ov065_0228b900[];
extern char data_ov065_0228b90c[];
extern char data_ov065_0228b918[];
extern char data_ov065_0228b924[];

extern void OS_GetMacAddress(void *p);
extern s32 RTC_GetDate(void *p);
extern s32 RTC_GetTime(void *p);
extern u32 OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(u32 v);
extern s32 OS_SPrintf(char *buf, const char *fmt, ...);
extern s32 OS_SNPrintf(char *buf, u32 n, const char *fmt, ...);
extern s32 strlen(const char *s);
extern s32 wcslen(const void *s);
extern void DWCi_BM_GetWiFiInfo(void *p);
extern s32 DWCi_AUTH_UpDateWiFiID(void *p, u8 *buf);
extern void DWCi_AUTH_RemakeWiFiID(void *p);
extern void DWCi_AUTH_MakeWiFiID(u8 *buf);
extern void OS_JoinThread(void *p);
extern void OS_Sleep(u32 ms);

extern u8 *WifiLink_GetConnectedBssid(void);
extern u8 *WifiLink_GetConnectedSsid(u16 *out);
extern s32 DwcHttp_AddField(DwcHttpFieldList *f, const char *k, const char *v);
extern s32 DwcHttp_AddHeader(void *a, const char *k, const char *v);
extern s32 DwcHttp_AddFormParam(void *a, const char *k, const char *v, u32 n);
extern char *DwcHttp_FindField(void *buf, s32 n, const char *key);
extern s32 DwcHttp_GetFieldDecoded(void *buf, u32 n, const char *key, void *out, u32 max);
extern s32 DwcHttp_GetFieldString(void *buf, u32 n, const char *key, void *out, u32 max);
extern s32 DwcHttp_ParseResponse(void *buf, u32 n, u32 a, void *b);
extern s32 DwcHttp_Destroy(void *p);
extern s32 NasAuth_SendRequest(s32 a);











void WifiAp_GetUsbApWepKey(u8 *p, u8 *out);
s32 WifiAp_IsUsbConnectorAp(void *p);
void WifiAp_GetNdwcshapApInfo(u8 *a, void *dst);
void WifiAp_GetNdwcshapWepKey(u8 *a, u8 *b);
s32 WifiAp_IsNdwcshapAp(u8 *a);

s32 WifiAp_IsNdwcshapAp(u8 *a) {
    u8 buf[0x1c];
    WifiAp_DecodeNdwcshapSsid(a, buf);
    if (memcmp(buf, "NDWCSHAP", 8) == 0) {
        return TRUE;
    }
    return FALSE;
}

void WifiAp_GetNdwcshapWepKey(u8 *a, u8 *b) {
    u8 buf[0x18];
    WifiAp_DecodeNdwcshapSsid(a, buf);
    WifiAp_DeriveNdwcshapWepKey(buf, b);
}

void WifiAp_GetNdwcshapApInfo(u8 *a, void *dst) {
    u8 buf[0x18];
    WifiAp_DecodeNdwcshapSsid(a, buf);
    if (memcmp(buf, "NDWCSHAP", 8) == 0) {
        MI_CpuCopy8(buf + 8, dst, 10);
    }
}

s32 WifiAp_IsUsbConnectorAp(void *p) {
    if (memcmp(p, "NWCUSBAP", 8) == 0) {
        return TRUE;
    }
    return FALSE;
}

void WifiAp_GetUsbApWepKey(u8 *p, u8 *out) {
    WifiAp_DeriveUsbApWepKey(p + 0xc, out);
}

}
}
}  // namespace N_d080

namespace N_c750 {
extern "C" {





extern u8 gWifiLinkAnyBssid[];
extern u8 gWifiLinkAnySsid[];
extern u8 sWifiApSocketConfigTemplate[];
extern s8 *sWifiApUsbKey1Ptr;
extern s8 *sWifiApUsbKey2Ptr;
extern s8 *sWifiApNdwcshapKey1Ptr;
extern s8 *sWifiApNdwcshapKey2Ptr;
extern u8 sWifiApNdwcshapPermTable[];

extern "C" {
s32 WifiLink_GetPhase(void);
s32 WifiAp_OnLinkNotify(void *a);
void WifiAp_BuildSearchList(void);
void WifiAp_StartScan(void *a, void *b, s32 c, u32 d);
s32 WifiAp_CheckSearchResult(void *ctx, s32 v);
s32 WifiAp_StepScanFoundChannels(void *ctx);
s32 WifiAp_StepScanSsids(void *ctx);
s32 WifiAp_StepScanAllChannels(void *ctx);
s32 WifiAp_BeginSearch(void *ctx);
void Sock_SetDnsServers(void *a, void *b);
s32 Sock_GetHostId(void);
s32 Sock_Startup(void *p);
s32 Sock_Cleanup(void);
s32 SockCore_TryShutdown(void);
s32 NetCheck_GetState(void);
u32 NetCheck_GetErrorCode(void);
s32 NetCheck_Abort(void);
s32 NetCheck_Destroy(void);
s32 NetCheck_Start(void *cfg);
void DGT_Hash1Reset(void *ctx);
void DGT_Hash1SetSource(void *ctx, void *p, u32 n);
void DGT_Hash1GetDigest_R(void *out, void *ctx);

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






















s32 WifiAp_StepStopSockets(void);
s32 WifiAp_StepConnected(WifiApContext *c);
s32 WifiAp_StepWaitNetCheck(WifiApContext *c);
s32 WifiAp_StepStartNetCheck(void);
s32 WifiAp_StepWaitAddress(WifiApContext *c);
s32 WifiAp_StepStartSockets(WifiApContext *c);
s32 WifiAp_ProcessNetSetup(void);
s32 WifiAp_Base64Decode(u8 *in, u8 *out, u32 len, u32 max);
s32 WifiAp_Base64Value(u32 c);

void WifiAp_DecodeNdwcshapSsid(u8 *a, u8 *b) {
    s32 i;
    WifiApNdwcshapPermTable t;
    t = *(WifiApNdwcshapPermTable *)sWifiApNdwcshapPermTable;
    WifiAp_Base64Decode(a, b, 0x20, 0x18);
    s32 j;
    for (j = 0; j < 0x18; j++) {
        b[j] ^= sWifiApNdwcshapKey1Ptr[j];
    }
    for (i = 0; i < 0x18; i++) {
        u32 j = (u8)i;
        u32 cur = j;
        u8 s = b[i];
        if (t.b[j] != 0xff) {
            do {
                u8 *slot = &t.b[cur];
                cur = t.b[cur];
                u8 nv = b[cur];
                b[t.b[j]] = s;
                j = cur;
                *slot = 0xff;
                s = nv;
            } while (t.b[cur] != 0xff);
        }
    }
    for (j = 0; j < 0x18; j++) {
        b[j] ^= sWifiApNdwcshapKey2Ptr[j];
    }
}

void WifiAp_DeriveNdwcshapWepKey(u8 *a, u8 *b) {
    u8 digest[0x14];
    u8 ctx[0x58];
    DGT_Hash1Reset(ctx);
    DGT_Hash1SetSource(ctx, a, 0x18);
    DGT_Hash1GetDigest_R(digest, ctx);
    MI_CpuCopy8(digest + 3, b, 13);
}

void WifiAp_DeriveUsbApWepKey(u8 *a, u8 *b) {
    u8 tmp[13];
    s32 i;
    s32 j;
    for (i = 0; i < 13; i++) {
        b[i] = a[i] ^ a[13 + i % 7];
    }
    for (j = 0; j < 7; j++) {
        b[j + 3] ^= a[13 + j];
    }
    for (j = 0; j < 13; j++) {
        b[j] ^= sWifiApUsbKey1Ptr[j];
    }
    MI_CpuCopy8(b, tmp, 13);
    {
        u8 *pt;
        const u8 *pk;
        i = 0;
        pt = tmp;
        pk = sWifiApUsbKeyPermTable;
        for (; i < 13; i++) {
            b[*pk] = *pt;
            pt++;
            pk++;
        }
    }
    for (j = 0; j < 13; j++) {
        b[j] ^= sWifiApUsbKey2Ptr[j];
    }
    for (i = 0; i < 13; i++) {
        u8 v = b[i];
        b[i] = (sWifiApUsbKeySBox[(v >> 4) & 15] << 4) | sWifiApUsbKeySBox[v & 15];
    }
    for (i = 0; i < 3; i++) {
        b[i] ^= b[i + 6];
        b[i + 3] ^= b[i + 9];
        b[i + 6] ^= b[i + 3];
        b[i + 9] ^= b[i];
        b[12] ^= b[i];
    }
}

s32 WifiAp_Base64Value(u32 c) {
    if (c >= 0x41 && c <= 0x5a) {
        return c - 0x41;
    }
    if (c >= 0x61 && c <= 0x7a) {
        s32 r = c - 0x61;
        return r + 0x1a;
    }
    if (c >= 0x30 && c <= 0x39) {
        s32 r = c - 0x30;
        return r + 0x34;
    }
    if (c == 0x2b) {
        return 0x3e;
    }
    if (c == 0x2f) {
        return 0x3f;
    }
    s32 t;
    if (c == 0x3d) {
        t = 0;
    } else {
        t = 1;
    }
    return -t;
}

s32 WifiAp_Base64Decode(u8 *in, u8 *out, u32 len, u32 max) {
    s32 rem;
    s32 full;
    u32 cnt;
    u32 n;
    n = (len * 3) >> 2;
    if (max >= n) {
        rem = len & 3;
        full = len - rem;
    } else {
        return -1;
    }
    s32 i = 0;
    if (full > 0) {
        cnt = 0;
        do {
            u32 v = 0;
            s32 j;
            for (j = 0; j < 4; j++) {
                v |= WifiAp_Base64Value(in[i + j]) << ((3 - j) * 6);
            }
            u32 tmp = v;
            s32 k = 0;
            s32 o = cnt * 3;
            for (; k < 3; k++) {
                out[o] = ((u8 *)&tmp)[2 - k];
                o++;
            }
            cnt++;
            i += 4;
        } while (i < full);
    }
    if (rem != 0) {
        u32 v = 0;
        u32 tmp = 0;
        s32 j;
        s32 k;
        for (j = 0; j < rem; j++) {
            v |= WifiAp_Base64Value(in[full + j]) << ((3 - j) * 6);
            tmp |= v;
        }
        k = 0;
        if (rem > 0) {
            s32 base = (full * 3) / 4;
            do {
                out[base] = ((u8 *)&tmp)[2 - k];
                base++;
                k++;
            } while (k < rem);
        }
    }
    return n;
}

s32 WifiAp_ProcessNetSetup(void) {
    s32 a = WifiAp_GetState();
    WifiApContext *c = (WifiApContext *)WifiAp_GetBlock(0x10);
    if (WifiLink_GetPhase() == 9) {
        switch (a) {
        case 10:
            a = WifiAp_StepStartSockets(c);
            break;
        case 12:
            a = WifiAp_StepWaitAddress(c);
            break;
        case 13:
            a = WifiAp_StepStartNetCheck();
            break;
        case 14:
            a = WifiAp_StepWaitNetCheck(c);
            break;
        case 15:
            a = WifiAp_StepConnected(c);
            break;
        case 11:
            a = WifiAp_StepStopSockets();
            break;
        }
    } else {
        switch (a) {
        case 0xf:
            a = WifiAp_StepConnected(c);
            break;
        case 0xb:
            a = WifiAp_StepStopSockets();
            break;
        case 0xe:
            NetCheck_Abort();
            NetCheck_Destroy();
        default:
            c->foundApInfo[c->selectedAp].status = 2;
            a = 0xb;
            break;
        }
    }
    return a;
}

s32 WifiAp_StepStartSockets(WifiApContext *c) {
    u8 *p = (u8 *)WifiAp_GetBlock(1);
    u8 *q = (u8 *)WifiAp_GetBlock(4);
    WifiAp_BuildSocketConfig(p, c, q);
    sSockYieldMode = 4;
    if (Sock_Startup(q) != 0) {
        WifiAp_SetError(2);
        return 0x11;
    }
    return 0xc;
}

s32 WifiAp_StepWaitAddress(WifiApContext *c) {
    if (Sock_GetHostId() != 0) {
        WifiAp_ApplyStaticDns(c);
        if (c->netCheckMode == 1) {
            return 0xf;
        }
        return 0xd;
    }
    s64 now = OS_GetTick();
    s64 d = now - (s64)c->stepStartTick;
    u64 r = ((u64)d << 6) / 0x1ff6210LL;
    if (r >= 10) {
        c->foundApInfo[c->selectedAp].status = 1;
        return 0xb;
    }
    return 0xc;
}

s32 WifiAp_StepStartNetCheck(void) {
    if (NetCheck_Start(WifiAp_GetBlock(8)) != 0) {
        WifiAp_SetError(3);
        return 0x11;
    }
    return 0xe;
}

s32 WifiAp_StepWaitNetCheck(WifiApContext *c) {
    WifiApControl *p = (WifiApControl *)WifiAp_GetBlock(1);
    s32 r = NetCheck_GetState();
    if (r != 0) {
        s32 x = WifiAp_FoldApIndex(c->apType);
        if (p->furthestApIndex == x) {  // (0x15 and 0x10 on the DS)
            p->netCheckError = NetCheck_GetErrorCode();
        }
        NetCheck_Destroy();
        if (r != 0xb) {
            c->foundApInfo[c->selectedAp].status = 1;
            return 0xb;
        }
        return 0xf;
    }
    return 0xe;
}

s32 WifiAp_StepConnected(WifiApContext *c) {
    WifiAp_SetConnectedApType(c->apType);
    return 0x10;
}

s32 WifiAp_StepStopSockets(void) {
    if (SockCore_TryShutdown() != 0) {
        return 0xb;
    }
    s32 r = Sock_Cleanup();
    if (r == 0 || r == -0x27) {
        return 9;
    }
    return 0xb;
}

}
}
}  // namespace N_c750

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0226cb18) {
    HOST_OVERLAY_VAR(sWifiApUsbKey1),
    HOST_OVERLAY_VAR(sWifiApUsbKey2),
    HOST_OVERLAY_VAR(sWifiApNdwcshapPermTable),
    HOST_OVERLAY_VAR(sWifiApNdwcshapKey2),
    HOST_OVERLAY_VAR(sWifiApNdwcshapKey1),
    HOST_OVERLAY_VAR(sWifiApNdwcshapKey1Ptr),
    HOST_OVERLAY_VAR(sWifiApUsbKey1Ptr),
    HOST_OVERLAY_VAR(sWifiApNdwcshapKey2Ptr),
    HOST_OVERLAY_VAR(sWifiApUsbKey2Ptr),
    HOST_OVERLAY_VARS_END
};
