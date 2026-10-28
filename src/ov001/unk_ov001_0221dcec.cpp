// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/WfcConfigSlot.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov001/unk_ov001_0221dcec.h"
#include "proto/ov001/unk_ov001_02226c24.h"

struct WfcConfigSlotBody {
    u8 v[0xf0];
};

#pragma thumb on
extern "C" {
s32 DWCi_BACKUPlRead(void *);
}
#pragma thumb off

extern "C" {
extern u32 WM_GetAllowedChannel();
extern void WfcHeap_FreeAndClear(void *);
extern void *WfcHeap_AllocClear(s32, s32);
extern void *WfcHeap_Alloc(s32, s32);
extern void MI_CpuFill8(void *, s32, u32);
extern void MI_CpuCopy8(void *, void *, u32);
extern void MIi_CpuClear16(u32, void *, u32);
extern void DWCi_BACKUPlWritePage(void *, void *, void *);
extern void DWCi_AUTH_GetNewWiFiInfo(void *);
extern void *DWCi_BACKUPlConvWifiInfo(void *);
extern u32 DWCi_BACKUPlConvMaskCidr(void *);
extern void DWCi_BACKUPlConvMaskAddr(u32, void *);
extern void OS_SPrintf(void *, const char *, ...);  // (variadic, as defined)

u32 WfcUtil_HexDigitValue(u32 c);
void WfcUtil_ParseIpDigits(u8 *s, u8 *out);
void WfcConfig_WriteSlot(s32 idx);
void WfcConfig_EraseAll();
void WfcConfig_EraseSlot(s32 idx);
WfcConfigData *WfcConfig_Get();
void WfcConfig_StoreAoss(u8 *src);
void WfcConfig_StoreSimpleStart(u8 *src);
void WfcConfig_CommitEdit();
void WfcConfig_BeginEdit(s32 idx);
u32 WfcConfig_GetSlotStatus(s32 idx);
void WfcConfig_FormatEditDns2(void *p);
void WfcConfig_FormatEditDns1(void *p);
void WfcConfig_FormatEditGateway(void *p);
void WfcConfig_FormatEditSubnetMask(void *p);
void WfcConfig_FormatEditIp(void *p);
void WfcConfig_GetEditSsid(void *src);
void WfcConfig_SetEditDns2(u8 *s);
void WfcConfig_SetEditDns1(u8 *s);
void WfcConfig_SetEditGateway(u8 *s);
void WfcConfig_SetEditSubnetMask(u8 *s);
void WfcConfig_SetEditIp(u8 *s);
void WfcConfig_SetEditWepKey(u8 *s);
void WfcConfig_SetEditSsid(u8 *ssid);
void WfcConfig_SetEditAutoDns(u32 v);
void WfcConfig_SetEditAutoIp(u32 v);
WfcConfigSlot *WfcConfig_GetEdit();
void WfcConfig_Shutdown();
void WfcConfig_Init();
}

extern "C" const u8 sWfcZeroIp[4] = { 0, 0, 0, 0 };
WfcConfigData *sWfcConfig;

#pragma thumb off

void WfcConfig_Init()
{
    WfcConfigData *g = (WfcConfigData *)WfcHeap_Alloc(sizeof(WfcConfigData), 0x20);
    sWfcConfig = g;
    MATHi_CRC16InitTableRev(g->crcTable, 0xa001);
    DWCi_BACKUPlRead(sWfcConfig);
}

void WfcConfig_Shutdown() { WfcHeap_FreeAndClear(&sWfcConfig); }

WfcConfigSlot *WfcConfig_GetEdit() { return &sWfcConfig->editSlot; }

void WfcConfig_SetEditAutoIp(u32 v) { sWfcConfig->editSlot.editAutoIp = v; }

void WfcConfig_SetEditAutoDns(u32 v) { sWfcConfig->editSlot.editAutoDns = v; }

void WfcConfig_SetEditSsid(u8 *ssid)
{
    MI_CpuCopy8(ssid, sWfcConfig->editSlot.ssid, 0x20);
    sWfcConfig->editSlot.status = 0;
}

void WfcConfig_SetEditWepKey(u8 *s)
{
    s32 i;
    s32 n;
    u8 *d;
    MI_CpuFill8(sWfcConfig->editSlot.wepKeys, 0, 0x10);
    n = WfcUtil_StrNLen(s, 0x20);
    switch (n) {
    case 0:
    case 10:
    case 0x1a:
    case 0x20:
        sWfcConfig->editSlot.wepMode.wepAscii = 0;
        d = sWfcConfig->editSlot.wepKeys;
        for (i = 0; i < n; i += 2, d++) {
            s32 hi = WfcUtil_HexDigitValue(s[i]);
            *d = WfcUtil_HexDigitValue(s[i + 1]) + (hi << 4);
        }
        break;
    default: {
        sWfcConfig->editSlot.wepMode.wepAscii = 1;
        MI_CpuCopy8(s, sWfcConfig->editSlot.wepKeys, 0x10);
        break;
    }
    }
    switch (n) {
    case 0:
        sWfcConfig->editSlot.wepMode.wepKeySize = 0;
        return;
    case 5:
    case 10:
        sWfcConfig->editSlot.wepMode.wepKeySize = 1;
        return;
    case 0xd:
    case 0x1a:
        sWfcConfig->editSlot.wepMode.wepKeySize = 2;
        return;
    default:
        sWfcConfig->editSlot.wepMode.wepKeySize = 3;
        return;
    }
}

void WfcConfig_SetEditIp(u8 *s) { WfcUtil_ParseIpDigits(s, sWfcConfig->editSlot.ipAddress); }

void WfcConfig_SetEditSubnetMask(u8 *s) { WfcUtil_ParseIpDigits(s, sWfcConfig->editSlot.editSubnetMask); }

void WfcConfig_SetEditGateway(u8 *s) { WfcUtil_ParseIpDigits(s, sWfcConfig->editSlot.gateway); }

void WfcConfig_SetEditDns1(u8 *s) { WfcUtil_ParseIpDigits(s, &sWfcConfig->editSlot.dnsServers[0]); }

void WfcConfig_SetEditDns2(u8 *s) { WfcUtil_ParseIpDigits(s, &sWfcConfig->editSlot.dnsServers[4]); }

void WfcConfig_GetEditSsid(void *src)
{
    MI_CpuCopy8(sWfcConfig->editSlot.ssid, src, 0x20);
}

void WfcConfig_FormatEditIp(void *out)
{
    u8 *p = sWfcConfig->editSlot.ipAddress;
    OS_SPrintf(out, "%3d%3d%3d%3d", p[0], p[1], p[2], p[3]);
}

void WfcConfig_FormatEditSubnetMask(void *p) {
    u8 *ip = sWfcConfig->editSlot.editSubnetMask;
    OS_SPrintf(p, "%3d%3d%3d%3d", ip[0], ip[1], ip[2], ip[3]);
}

void WfcConfig_FormatEditGateway(void *p) {
    u8 *ip = sWfcConfig->editSlot.gateway;
    OS_SPrintf(p, "%3d%3d%3d%3d", ip[0], ip[1], ip[2], ip[3]);
}

void WfcConfig_FormatEditDns1(void *p) {
    u8 *ip = &sWfcConfig->editSlot.dnsServers[0];
    OS_SPrintf(p, "%3d%3d%3d%3d", ip[0], ip[1], ip[2], ip[3]);
}

void WfcConfig_FormatEditDns2(void *p) {
    u8 *ip = &sWfcConfig->editSlot.dnsServers[4];
    OS_SPrintf(p, "%3d%3d%3d%3d", ip[0], ip[1], ip[2], ip[3]);
}

u32 WfcConfig_GetSlotStatus(s32 idx) {
    return sWfcConfig->slots[idx].status;
}

void WfcConfig_BeginEdit(s32 idx) {
    WfcConfigData *d = sWfcConfig;
    WfcConfigSlot *s = &d->slots[idx];
    *(WfcConfigSlotBody *)&d->editSlot = *(WfcConfigSlotBody *)s;
    d->editSlot.editSlotIndex = idx;
    if (memcmp(s->ipAddress, sWfcZeroIp, 4) != 0) {
        sWfcConfig->editSlot.editAutoIp = 0;
    } else {
        sWfcConfig->editSlot.editAutoIp = 1;
    }
    if (memcmp(s->dnsServers, sWfcZeroIp, 4) != 0 ||
        memcmp(&s->dnsServers[4], sWfcZeroIp, 4) != 0) {
        sWfcConfig->editSlot.editAutoDns = 0;
    } else {
        sWfcConfig->editSlot.editAutoDns = 1;
    }
    DWCi_BACKUPlConvMaskAddr(s->subnetPrefixLen, sWfcConfig->editSlot.editSubnetMask);
}

void WfcConfig_CommitEdit() {
    WfcConfigData *d = sWfcConfig;
    WfcConfigSlot *b = &d->editSlot;
    WfcConfigSlot *s = &d->slots[b->editSlotIndex];
    *(WfcConfigSlotBody *)s = *(WfcConfigSlotBody *)b;
    if (b->editAutoIp != 0) {
        MI_CpuFill8(s->ipAddress, 0, 4);
        MI_CpuFill8(s->gateway, 0, 4);
        s->subnetPrefixLen = 0;
    } else {
        MI_CpuCopy8(b->ipAddress, s->ipAddress, 4);
        MI_CpuCopy8(b->gateway, s->gateway, 4);
        s->subnetPrefixLen = DWCi_BACKUPlConvMaskCidr(b->editSubnetMask);
    }
    if (b->editAutoDns != 0) {
        MI_CpuFill8(s->dnsServers, 0, 8);
    } else {
        MI_CpuCopy8(b->dnsServers, s->dnsServers, 8);
    }
    WfcConfig_WriteSlot(b->editSlotIndex);
}

void WfcConfig_StoreSimpleStart(u8 *src) {
    WfcConfigSlot *b = &sWfcConfig->editSlot;
    u32 n;
    s32 i;
    u8 *d;
    MI_CpuFill8(b, 0, 0xef);
    MI_CpuCopy8(src, b->ssid, 0x20);
    switch (*(s32 *)(src + 0x20)) {
    case 1:
        n = 5;
        b->wepMode.wepKeySize = 1;
        break;
    case 2:
        n = 0xd;
        b->wepMode.wepKeySize = 2;
        break;
    case 3:
        n = 0x10;
        b->wepMode.wepKeySize = 3;
        break;
    default:
        n = 0;
        b->wepMode.wepKeySize = 0;
        break;
    }
    b->wepMode.wepAscii = 0;
    d = b->wepKeys;
    src += 0x28;
    for (i = 0; i < 4; i++, d += 0x10, src += 0x20) {
        MI_CpuCopy8(src, d, n);
    }
    b->status = 2;
    MI_CpuFill8(b->editSubnetMask, 0, 4);
    b->editAutoIp = 1;
    b->editAutoDns = 1;
    WfcConfig_CommitEdit();
}

void WfcConfig_StoreAoss(u8 *src) {
    WfcConfigSlot *b = &sWfcConfig->editSlot;
    MI_CpuFill8(b, 0, 0xef);
    MI_CpuCopy8(src, &b->aossWepKeys[0], 5);
    MI_CpuCopy8(src + 0x6, &b->aossWepKeys[5], 5);
    MI_CpuCopy8(src + 0xc, &b->aossWepKeys[10], 5);
    MI_CpuCopy8(src + 0x12, &b->aossWepKeys[15], 5);
    MI_CpuCopy8(src + 0x18, b->aossWep64Ssid, 0x20);
    MI_CpuCopy8(src + 0x39, &b->wepKeys[0], 0xd);
    MI_CpuCopy8(src + 0x47, &b->wepKeys[0x10], 0xd);
    MI_CpuCopy8(src + 0x55, &b->wepKeys[0x20], 0xd);
    MI_CpuCopy8(src + 0x63, &b->wepKeys[0x30], 0xd);
    MI_CpuCopy8(src + 0x71, b->ssid, 0x20);
    b->wepMode.wepKeySize = 2;
    b->wepMode.wepAscii = 0;
    b->status = 1;
    MI_CpuFill8(b->editSubnetMask, 0, 4);
    b->editAutoIp = 1;
    b->editAutoDns = 1;
    WfcConfig_CommitEdit();
}

WfcConfigData *WfcConfig_Get() {
    return sWfcConfig;
}

void WfcConfig_EraseSlot(s32 idx) {
    WfcConfigSlot *s = &sWfcConfig->slots[idx];
    MI_CpuFill8(s, 0, 0xef);
    s->status = 0xff;
    WfcConfig_WriteSlot(idx);
}

void WfcConfig_EraseAll() {
    volatile u16 z = 0;
    u32 rtc[5];
    void *r8;
    s32 i;
    MIi_CpuClear16(z, sWfcConfig->slots, sizeof(sWfcConfig->slots));
    for (i = 0; i < 3; i++) {
        sWfcConfig->slots[i].status = 0xff;
    }
    DWCi_AUTH_GetNewWiFiInfo(rtc);
    r8 = DWCi_BACKUPlConvWifiInfo(rtc);
    for (i = 0; i < 2; i++) {
        // slots 0 and 1 keep the Wi-Fi user info (0xe bytes) where the edit slot has its edit state
        MI_CpuCopy8(r8, sWfcConfig->slots[i].editSubnetMask, 0xe);
    }
    for (i = 0; i < 4; i++) {
        WfcConfig_WriteSlot(i);
    }
}

void WfcConfig_WriteSlot(s32 idx) {
    s32 flags[4];
    u32 st;
    u32 bit;
    BOOL on;
    s32 i;
    void *p;
    st = sWfcConfig->slots[idx].status;
    on = FALSE;
    bit = 1 << idx;
    MI_CpuFill8(flags, on, 0x10);
    flags[idx] = 1;
    if (idx <= 2) {
        WfcConfigData *d = sWfcConfig;
        if ((d->slots[0].configuredMask & bit) != 0) on = TRUE;
        if (st == 0xff && on) {
            d->slots[0].configuredMask &= ~bit;
            sWfcConfig->slots[1].configuredMask &= ~bit;
            flags[1] = 1;
            flags[0] = 1;
        } else if (st != 0xff && !on) {
            d->slots[0].configuredMask |= bit;
            sWfcConfig->slots[1].configuredMask |= bit;
            flags[1] = 1;
            flags[0] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (flags[i] != 0) {
            WfcConfigData *d = sWfcConfig;
            u32 r = MATH_CalcCRC16(d->crcTable, &d->slots[i], 0xfe);
            sWfcConfig->slots[i].crc16 = r;
        }
    }
    p = WfcHeap_Alloc(0x100, 0x20);
    DWCi_BACKUPlWritePage(sWfcConfig, flags, p);
    WfcHeap_FreeAndClear(&p);
}

void WfcUtil_ParseIpDigits(u8 *s, u8 *out) {
    char tmp[4];
    s32 i;
    s32 off;
    MI_CpuFill8(tmp, 0, 4);
    i = 0;
    off = i;
    for (; i < 4; i++, off += 3) {
        s32 j;
        u8 *p;
        MI_CpuCopy8(s + off, tmp, 3);
        j = 0;
        p = (u8 *)tmp;
        do {
            if (*p != 0) break;
            j++;
            *p++ = 0x20;
        } while (j < 3);
        out[i] = atol(tmp);
    }
}

u32 WfcUtil_HexDigitValue(u32 c) {
    if (c <= 0x39) return c - 0x30;
    if (c <= 0x46) return c - 0x37;
    return c - 0x57;
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov001_0221dcec) {
    HOST_OVERLAY_VAR(sWfcConfig),
    HOST_OVERLAY_VARS_END
};
