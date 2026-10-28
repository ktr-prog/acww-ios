// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "host_mem.h"
#include "net/WfcMoveMbWork.h"
#include "nitro/wm.h"
#include "proto/autoload_2/unk_02115468.h"
#include "proto/autoload_2/unk_02119434.h"
#include "proto/autoload_2/unk_02122e24.h"
#include "proto/main/unk_0206d49c.h"
#include "proto/ov001/unk_ov001_02220ad8.h"
#include "proto/ov001/unk_ov001_022218a4.h"

#pragma thumb off

// (fixed parameters, as the functions they point to have: through a variadic type, Apple arm64 would pass the
// arguments on the stack while the function reads them from registers)
typedef s32 (*WfcMoveWhJudgeFunc)(void *);
typedef s32 (*WfcMoveWhWepKeyFunc)(void *, void *);

struct WfcMoveWhWork {
    s32 userGameInfo;
    u16 userGameInfoLength;
    u8 pad_06[2];
    s32 ggid;
    u16 tgid;
    u16 entryFlag;
    u16 maxEntry;
    u16 multiBootFlag;
    u16 KS_Flag;
    u16 CS_Flag;
    u16 beaconPeriod;
    u8 pad_1a[0x32 - 0x1a];
    u16 channel;
    u16 parentMaxSize;
    u16 childMaxSize;
    u8 pad_38[8];
    s32 sysState;
    s32 connectMode;
    s32 receiverFunc;
    WfcMoveWhJudgeFunc judgeAcceptFunc;
    u16 myAid;
    u16 connectBitmap;
    s32 errCode;
    u32 rand;
    u16 decidedChannel;
    u16 channelBusyRatio;
    u16 channelBitmap;
    u8 pad_62[0xf80 - 0x62];
    u8 sendBuffer[0x1060 - 0xf80];
    u8 recvBuffer[0x12a0 - 0x1060];
    s32 sendBufferSize;
    s32 recvBufferSize;
    u8 pad_12a8[0x13a8 - 0x12a8];
    u32 unk_13a8;
    WfcMoveWhWepKeyFunc parentWepKeyGenerator;
    u32 unk_13b0;
    u8 pad_13b4[0xc];
    u8 wepKey[0x20];
    u8 dsInfo[0x1c00 - 0x13e0];
    u8 dataSet[0x1e00 - 0x1c00];
    u8 keySetBuf[4];
};

typedef void (*WfcMoveWhPrintFn)(u32, const void *, ...);

extern "C" {
extern WfcMoveMbWork *sWfcMoveMb;

s32 MB_CommResponseRequest(s32, s32);
s32 OS_DisableInterrupts();
void OS_RestoreInterrupts(s32);
void MB_DisconnectChild(s32);
s32 FS_OpenFile(void *, s32);
s32 MB_GetSegmentLength(void *);
s32 MB_ReadSegment(void *, void *, u32);
s32 MB_RegisterFile(void *, void *);
void FS_CloseFile(void *);
s32 MB_StartParentFromIdle(s32);
void MI_CpuCopy8(void *, void *, u32);
s32 MB_Init(void *, void *, s32, s32, s32);
void MB_SetParentCommParam(u32, u32);
s32 WM_End(void *);
s32 WM_StepDataSharing(void *, void *, void *);
void *WM_GetSharedDataAddress(void *, void *, s32);
u32 WM_GetDispersionBeaconPeriod();
s32 WM_SetIndCallback(void *);
s32 WM_Initialize(void *, void *, s32);
s32 WM_Reset(void *);
s32 WM_Disconnect(void *, s32);
s32 WM_EndMP(void *);
s32 WM_EndKeySharing(void *);
s32 WM_EndParent(void *);
s32 WM_MeasureChannel(void *, s32, s32, s32, s32);
s32 WM_GetAllowedChannel();
void OS_GetMacAddress(u16 *);
s32 WM_StartKeySharing(void *, s32);
s32 WM_StartDataSharing(void *, s32, s32, s32, s32);
s32 WM_StartMPEx(void *, void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);
s32 WM_StartParent(void *);
s32 WM_SetWEPKey(void *, s32, void *);
s32 WM_SetParentParameter(void *, void *);

void WfcMoveWh_SetWork(WfcMoveWhWork *);

}
// Declarations for data defined further down (definition order sets the data layout)
extern "C" char data_ov001_0222b4ec[];
extern "C" char data_ov001_0222b508[];
extern "C" WfcMoveWhPrintFn sWfcMoveWhDebugPrint;
extern "C" char data_ov001_0222b524[];
extern "C" char data_ov001_0222b540[];
extern "C" char data_ov001_0222b55c[];
extern "C" char data_ov001_0222b57c[];
extern "C" char data_ov001_0222b59c[];
extern "C" char data_ov001_0222b5bc[];
extern "C" char data_ov001_0222b5e0[];
extern "C" char data_ov001_0222b604[];
extern "C" char *sWfcMoveWhSysStateNames[10];
extern "C" WfcMoveWhWork *sWfcMoveWh;

#define G (sWfcMoveWh)
#define LOG (sWfcMoveWhDebugPrint)

extern "C" void WfcMoveWh_SetWork(WfcMoveWhWork *p) {
    G = p;
    p->sysState = 0;
    G->unk_13a8 = 0;
    G->parentWepKeyGenerator = 0;
    G->unk_13b0 = 0;
}

extern "C" void WfcMoveWh_ChangeSysState(s32 n) {
    if (LOG) {
        LOG(0x8000000, "%s -> ", sWfcMoveWhSysStateNames[G->sysState]);
    }
    G->sysState = n;
    if (LOG) {
        LOG(0x8000000, "%s\n", sWfcMoveWhSysStateNames[G->sysState]);
    }
}

extern "C" void WfcMoveWh_SetError(u32 v) {
    WfcMoveWhWork *g = G;
    if ((u32)(g->sysState - 9) > 1) {
        g->errCode = v;
    }
}

extern "C" s32 WfcMoveWh_StateInSetParentParam() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = WM_SetParentParameter((void *)WfcMoveWh_StateOutSetParentParam, G);
    if (r == 2) {
        return TRUE;
    }
    WfcMoveWh_SetError(r);
    WfcMoveWh_ChangeSysState(9);
    return FALSE;
}

extern "C" void WfcMoveWh_StateOutSetParentParam(u16 *p) {
    if (p[1] != 0) {
        WfcMoveWh_SetError(p[1]);
        WfcMoveWh_ChangeSysState(9);
        return;
    }
    if (G->parentWepKeyGenerator != 0) {
        if (WfcMoveWh_StateInSetWepKey()) {
            return;
        }
        WfcMoveWh_ChangeSysState(9);
        return;
    }
    if (WfcMoveWh_StateInStartParent()) {
        return;
    }
    WfcMoveWh_ChangeSysState(9);
}

extern "C" s32 WfcMoveWh_StateInSetWepKey() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = G->parentWepKeyGenerator(G->wepKey, G);
    r = WM_SetWEPKey((void *)WfcMoveWh_StateOutSetWepKey, r, G->wepKey);
    if (r == 2) {
        return TRUE;
    }
    WfcMoveWh_SetError(r);
    WfcMoveWh_ChangeSysState(9);
    return FALSE;
}

extern "C" void WfcMoveWh_StateOutSetWepKey(u16 *p) {
    if (p[1] != 0) {
        WfcMoveWh_SetError(p[1]);
        WfcMoveWh_ChangeSysState(9);
        return;
    }
    if (WfcMoveWh_StateInStartParent()) {
        return;
    }
    WfcMoveWh_ChangeSysState(9);
}

extern "C" s32 WfcMoveWh_StateInStartParent() {
    s32 r;
    if ((u32)(G->sysState - 4) <= 2) {
        return TRUE;
    }
    r = WM_StartParent((void *)WfcMoveWh_StateOutStartParent);
    if (r != 2) {
        WfcMoveWh_SetError(r);
        return FALSE;
    }
    G->myAid = 0;
    G->connectBitmap = 1;
    return TRUE;
}

extern "C" void WfcMoveWh_StateOutStartParent(u16 *p) {
    u32 sh = p[8];
    u32 mask = (u16)(1 << sh);
    if (p[1] != 0) {
        WfcMoveWh_SetError(p[1]);
        WfcMoveWh_ChangeSysState(9);
        return;
    }
    switch (p[4]) {
    case 2:
        return;
    case 7: {
        s32 r;
        if (LOG) {
            LOG(0x8000000, "StartParent - new child (aid %x) connected\n", sh);
        }
        if (G->judgeAcceptFunc != 0 && (r = G->judgeAcceptFunc(p)) == 0) {
            r = WM_Disconnect(0, p[8]);
            if (r == 2) {
                return;
            }
            WfcMoveWh_SetError(r);
            WfcMoveWh_ChangeSysState(9);
            return;
        }
        G->connectBitmap |= mask;
        return;
    }
    case 9:
        if (LOG) {
            LOG(0x8000000, "StartParent - child (aid %x) disconnected\n", sh);
        }
        G->connectBitmap &= ~mask;
        return;
    case 0:
        if (WfcMoveWh_StateInStartParentMp()) {
            return;
        }
        WfcMoveWh_ChangeSysState(9);
        return;
    default:
        if (LOG) {
            LOG(0x8000000, "unknown indicate, state = %d\n", p[4]);
        }
        return;
    }
}

extern "C" s32 WfcMoveWh_StateInStartParentMp() {
    s32 r;
    if ((u32)(G->sysState - 4) <= 2) {
        return TRUE;
    }
    WfcMoveWh_ChangeSysState(4);
    {
        WfcMoveWhWork *g = G;
        r = WM_StartMPEx((void *)WfcMoveWh_StateOutStartParentMp, g->recvBuffer, (u16)g->recvBufferSize, g->sendBuffer, (u16)g->sendBufferSize, 1, 0, 0, 0, 0, 0);
    }
    if (r == 2) {
        return TRUE;
    }
    WfcMoveWh_SetError(r);
    return FALSE;
}

extern "C" void WfcMoveWh_StateOutStartParentMp(u16 *p) {
    if (p[1] != 0) {
        WfcMoveWh_SetError(p[1]);
        WfcMoveWh_ChangeSysState(9);
        return;
    }
    switch (p[2]) {
    case 10: {
        WfcMoveWhWork *g = G;
        if (g->connectMode == 2) {
            if (g->sysState == 4) {
                if (DWCi_MOV_WH_StateInStartParentKeyShare()) {
                    return;
                }
                if (LOG) {
                    LOG(0x8000000, "DWCi_MOV_WH_StateInStartParentKeyShare failed\n");
                }
                WfcMoveWh_ChangeSysState(9);
                return;
            } else if (g->sysState == 6) {
                return;
            }
        } else if (g->connectMode == 4) {
            s32 r = WM_StartDataSharing(g->dsInfo, 0xd, 7, 0x44, 1);
            if (r != 0) {
                WfcMoveWh_SetError(r);
                WfcMoveWh_ChangeSysState(9);
                return;
            }
            WfcMoveWh_ChangeSysState(5);
            return;
        }
        WfcMoveWh_ChangeSysState(4);
        return;
    }
    case 11:
        break;
    case 12:
    case 13:
    default:
        if (LOG) {
            LOG(0x8000000, "unknown indicate, state = %d\n", p[2]);
        }
        break;
    }
}

extern "C" s32 DWCi_MOV_WH_StateInStartParentKeyShare() {
    s32 r;
    WfcMoveWh_ChangeSysState(6);
    r = WM_StartKeySharing(G->keySetBuf, 0xd);
    if (r == 2) {
        return TRUE;
    }
    WfcMoveWh_SetError(r);
    return FALSE;
}

extern "C" s32 WfcMoveWh_EndKeySharing() {
    s32 r;
    r = WM_EndKeySharing((u8 *)sWfcMoveWh + 0x1e00);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" s32 WfcMoveWh_StateInEndParentMp() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = WM_EndMP((void *)WfcMoveWh_StateOutEndParentMp);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" void WfcMoveWh_StateOutEndParentMp(WMMsg *a) {
    if (a->errcode != 0) {
        WfcMoveWh_SetError(a->errcode);
        WfcMoveWh_Reset();
        return;
    }
    if (DWCi_MOV_WH_StateInEndParent() != 0) return;
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "DWCi_MOV_WH_StateInEndParent failed\n");
    WfcMoveWh_Reset();
}

static void func_ov001_dead_unknown() {
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "unknown indicate, state = %d\n", 0);
}

extern "C" s32 DWCi_MOV_WH_StateInEndParent() {
    s32 r;
    r = WM_EndParent((void *)WfcMoveWh_StateOutEndParent);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" void WfcMoveWh_StateOutEndParent(WMMsg *a) {
    if (a->errcode != 0) {
        WfcMoveWh_SetError(a->errcode);
        return;
    }
    WfcMoveWh_ChangeSysState(1);
}

extern "C" s32 WfcMoveWh_StateInEndChildKeyShare() {
    s32 r;
    if (sWfcMoveWh->sysState != 6) return 0;
    WfcMoveWh_ChangeSysState(3);
    r = WM_EndKeySharing((u8 *)sWfcMoveWh + 0x1e00);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" s32 WfcMoveWh_StateInEndChildMp() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = WM_EndMP((void *)WfcMoveWh_StateOutEndChildMp);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" void WfcMoveWh_StateOutEndChildMp(WMMsg *a) {
    if (a->errcode != 0) {
        WfcMoveWh_SetError(a->errcode);
        DWCi_MOV_WH_Finalize();
        return;
    }
    if (WfcMoveWh_StateInEndChild() != 0) return;
    WfcMoveWh_ChangeSysState(9);
}

extern "C" s32 WfcMoveWh_StateInEndChild() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = WM_Disconnect((void *)WfcMoveWh_StateOutEndChild, 0);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    WfcMoveWh_Reset();
    return 0;
}

extern "C" void WfcMoveWh_StateOutEndChild(WMMsg *a) {
    if (a->errcode != 0) {
        WfcMoveWh_SetError(a->errcode);
        return;
    }
    WfcMoveWh_ChangeSysState(1);
}

extern "C" s32 WfcMoveWh_StateInReset() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = WM_Reset((void *)WfcMoveWh_StateOutReset);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" void WfcMoveWh_StateOutReset(WMMsg *a) {
    if (a->errcode != 0) {
        WfcMoveWh_ChangeSysState(9);
        WfcMoveWh_SetError(a->errcode);
        return;
    }
    WfcMoveWh_ChangeSysState(1);
}

extern "C" void WfcMoveWh_StateOutEnd(WMMsg *a) {
    if (a->errcode != 0) {
        WfcMoveWh_ChangeSysState(10);
        return;
    }
    WfcMoveWh_ChangeSysState(0);
}

extern "C" void WfcMoveWh_SetGgid(s32 x) {
    sWfcMoveWh->ggid = x;
}

extern "C" u16 WfcMoveWh_GetConnectedBitmap() {
    return sWfcMoveWh->connectBitmap;
}

extern "C" s32 WfcMoveWh_GetSysState() {
    return sWfcMoveWh->sysState;
}

extern "C" s32 WfcMoveWh_StartMeasureChannel() {
    u16 v[3];
    u16 r;
    OS_GetMacAddress(v);
    u32 m = *HW_SYSWORK(volatile u32, vblankCount);
    u32 a = v[0] + m;
    u32 b = v[1] + a;
    sWfcMoveWh->rand = v[2] + b;
    sWfcMoveWh->rand = sWfcMoveWh->rand * 0x10dcd + 0x3039;
    sWfcMoveWh->decidedChannel = 0;
    sWfcMoveWh->channelBusyRatio = 0x65;
    WfcMoveWh_ChangeSysState(3);
    r = WfcMoveWh_MeasureNextChannel(1);
    if (r == 0x18) {
        WfcMoveWh_SetError(0x18);
        WfcMoveWh_ChangeSysState(9);
        return 0;
    }
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    WfcMoveWh_ChangeSysState(9);
    return 0;
}

extern "C" u16 WfcMoveWh_MeasureNextChannel(u16 x) {
    s32 r = WM_GetAllowedChannel();
    if (r == 0x8000) {
        WfcMoveWh_SetError(3);
        WfcMoveWh_ChangeSysState(9);
        return 3;
    }
    if (r == 0) {
        WfcMoveWh_SetError(0x16);
        WfcMoveWh_ChangeSysState(9);
        return 0x18;
    }
    while ((1 << (x - 1) & r) == 0) {
        x++;
        if (x > 16) return 0x18;
    }
    return WfcMoveWh_StateInMeasureChannel((void *)WfcMoveWh_StateOutMeasureChannel, x);
}

extern "C" void WfcMoveWh_StateOutMeasureChannel(WMMeasureChannelCallback *a) {
    if (a->errcode != 0) {
        WfcMoveWh_SetError(a->errcode);
        WfcMoveWh_ChangeSysState(9);
        return;
    }
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "channel %d bratio = %x\n", a->channel, a->ccaBusyRatio);
    {
        WfcMoveWhWork *g = sWfcMoveWh;
        u16 y = a->ccaBusyRatio;
        u16 x = a->channel;
        u16 w = g->channelBusyRatio;
        u16 r;
        if (w > y) {
            g->channelBusyRatio = y;
            sWfcMoveWh->channelBitmap = 1 << (x - 1);
        } else if (w == y) {
            g->channelBitmap = g->channelBitmap | (1 << (x - 1));
        }
        r = WfcMoveWh_MeasureNextChannel(x + 1);
        if (r == 0x18) {
            WfcMoveWh_ChangeSysState(7);
            return;
        }
        if (r == 2) return;
        WfcMoveWh_ChangeSysState(9);
    }
}

extern "C" s32 WfcMoveWh_StateInMeasureChannel(void *cb, s32 x) {
    return WM_MeasureChannel(cb, 3, 0x11, x, 0x1e);
}

extern "C" u16 WfcMoveWh_DecideChannel() {
    if (sWfcMoveWh->sysState != 7) Fatal_Trap();
    WfcMoveWh_ChangeSysState(1);
    sWfcMoveWh->decidedChannel = WfcMoveWh_PickRandomChannel(sWfcMoveWh->channelBitmap);
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "decided channel = %d\n", sWfcMoveWh->decidedChannel);
    return sWfcMoveWh->decidedChannel;
}

extern "C" s16 WfcMoveWh_PickRandomChannel(u16 mask) {
    s16 i;
    s16 last = 0;
    u16 count = 0;
    u16 r;
    for (i = 0; i < 16; i++) {
        if (mask & (1 << i)) {
            last = i + 1;
            count++;
        }
    }
    if (count <= 1) return last;
    sWfcMoveWh->rand = sWfcMoveWh->rand * 0x10dcd + 0x3039;
    r = (count * (sWfcMoveWh->rand & 0xff)) >> 8;
    for (i = 0; i < 16; i++) {
        if (mask & 1) {
            if (r == 0) return i + 1;
            r--;
        }
        mask >>= 1;
    }
    return 0;
}

extern "C" BOOL WfcMoveWh_Initialize() {
    WfcMoveWhWork **g = &sWfcMoveWh;
    (*g)->recvBufferSize = 0;
    (*g)->sendBufferSize = 0;
    (*g)->receiverFunc = 0;
    (*g)->myAid = 0;
    (*g)->connectBitmap = 1;
    (*g)->errCode = 0;
    (*g)->userGameInfo = 0;
    (*g)->userGameInfoLength = 0;
    (*g)->judgeAcceptFunc = 0;
    if (WfcMoveWh_StateInInitialize() != 0) return TRUE;
    return FALSE;
}

extern "C" void WfcMoveWh_IndicationCallback(WMMsg *a) {
    if (a->errcode != 8) return;
    WfcMoveWh_ChangeSysState(9);
    Fatal_Trap();
}

extern "C" s32 WfcMoveWh_StateInInitialize() {
    s32 r;
    WfcMoveWh_ChangeSysState(3);
    r = WM_Initialize((u8 *)sWfcMoveWh + 0x80, (void *)WfcMoveWh_StateOutInitialize, 2);
    if (r == 2) return 1;
    WfcMoveWh_SetError(r);
    WfcMoveWh_ChangeSysState(10);
    return 0;
}

extern "C" void WfcMoveWh_StateOutInitialize(u16 *p) {
    if (p[1] != 0) {
        WfcMoveWh_SetError(p[1]);
        WfcMoveWh_ChangeSysState(0xa);
        return;
    }
    s32 r = WM_SetIndCallback((void *)WfcMoveWh_IndicationCallback);
    if (r != 0) {
        WfcMoveWh_SetError(r);
        WfcMoveWh_ChangeSysState(0xa);
        return;
    }
    WfcMoveWh_ChangeSysState(1);
}

extern "C" s32 WfcMoveWh_ParentConnect(s32 a, u32 b, u32 c) {
    if (sWfcMoveWh->sysState != 1) Fatal_Trap();
    sWfcMoveWh->recvBufferSize = 0x180;
    sWfcMoveWh->sendBufferSize = 0xe0;
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "recv buffer size = %d\n", sWfcMoveWh->recvBufferSize);
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "send buffer size = %d\n", sWfcMoveWh->sendBufferSize);
    sWfcMoveWh->connectMode = a;
    WfcMoveWh_ChangeSysState(3);
    sWfcMoveWh->tgid = b;
    sWfcMoveWh->channel = c;
    sWfcMoveWh->beaconPeriod = WM_GetDispersionBeaconPeriod();
    sWfcMoveWh->parentMaxSize = 0xd0;
    sWfcMoveWh->childMaxSize = 0x44;
    sWfcMoveWh->maxEntry = 2;
    sWfcMoveWh->CS_Flag = 0;
    sWfcMoveWh->multiBootFlag = 0;
    sWfcMoveWh->entryFlag = 1;
    sWfcMoveWh->KS_Flag = (a == 2) ? 1 : 0;
    if (a == 0 || a == 2 || a == 4) return WfcMoveWh_StateInSetParentParam();
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "unknown connect mode %d\n", a);
    return 0;
}

extern "C" void WfcMoveWh_SetJudgeCallback(void *a) {
    sWfcMoveWh->judgeAcceptFunc = (WfcMoveWhJudgeFunc)a;
}

extern "C" void *WfcMoveWh_GetSharedData(s32 a) {
    WfcMoveWhWork *g = sWfcMoveWh;
    return WM_GetSharedDataAddress(g->dsInfo, g->dataSet, a);
}

extern "C" s32 DWCi_MOV_WH_StepDataSharing(void *a) {
    WfcMoveWhWork *g = sWfcMoveWh;
    s32 r = WM_StepDataSharing(g->dsInfo, a, g->dataSet);
    if (r == 7) {
        if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "DWCi_MOV_WH_StepDataSharing - Warning No Child\n");
        return 0;
    }
    if (r == 5) {
        if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "DWCi_MOV_WH_StepDataSharing - Warning No DataSet\n");
        WfcMoveWh_SetError(r);
        return 0;
    }
    if (r == 0) return 1;
    WfcMoveWh_SetError(r);
    return 0;
}

extern "C" void WfcMoveWh_Reset() {
    if (WfcMoveWh_StateInReset() != 0) return;
    WfcMoveWh_ChangeSysState(0xa);
}

extern "C" void DWCi_MOV_WH_Finalize() {
    s32 st = sWfcMoveWh->sysState;
    if (st == 1) {
        if (sWfcMoveWhDebugPrint == 0) return;
        sWfcMoveWhDebugPrint(0x8000000, "already DWCi_MOV_WH_SYSSTATE_IDLE\n");
        return;
    }
    if (sWfcMoveWhDebugPrint != 0) sWfcMoveWhDebugPrint(0x8000000, "DWCi_MOV_WH_Finalize, state = %d\n", st);
    st = sWfcMoveWh->sysState;
    if (st != 6 && st != 5 && st != 4) {
        WfcMoveWh_ChangeSysState(3);
        WfcMoveWh_Reset();
        return;
    }
    WfcMoveWh_ChangeSysState(3);
    switch (sWfcMoveWh->connectMode) {
    case 3:
        if (WfcMoveWh_StateInEndChildKeyShare() != 0) return;
        WfcMoveWh_Reset();
        return;
    case 1:
    case 5:
        if (WfcMoveWh_StateInEndChildMp() != 0) return;
        WfcMoveWh_Reset();
        return;
    case 2:
        if (WfcMoveWh_EndKeySharing() != 0) return;
        WfcMoveWh_Reset();
        return;
    case 0:
    case 4:
        if (WfcMoveWh_StateInEndParentMp() != 0) return;
        WfcMoveWh_Reset();
        return;
    }
}

extern "C" s32 WfcMoveWh_End() {
    if (sWfcMoveWh->sysState != 1) Fatal_Trap();
    WfcMoveWh_ChangeSysState(3);
    if (WM_End((void *)WfcMoveWh_StateOutEnd) == 2) return 1;
    WfcMoveWh_ChangeSysState(9);
    return 0;
}

extern "C" char data_ov001_0222b4ec[] = "DWCi_MOV_WH_SYSSTATE_STOP";

extern "C" char data_ov001_0222b508[] = "DWCi_MOV_WH_SYSSTATE_IDLE";

extern "C" WfcMoveWhPrintFn sWfcMoveWhDebugPrint = 0;

extern "C" char data_ov001_0222b524[] = "DWCi_MOV_WH_SYSSTATE_BUSY";

extern "C" char data_ov001_0222b540[] = "DWCi_MOV_WH_SYSSTATE_ERROR";

extern "C" char data_ov001_0222b55c[] = "DWCi_MOV_WH_SYSSTATE_SCANNING";

extern "C" char data_ov001_0222b57c[] = "DWCi_MOV_WH_SYSSTATE_CONNECTED";

extern "C" char data_ov001_0222b59c[] = "DWCi_MOV_WH_SYSSTATE_KEYSHARING";

extern "C" char data_ov001_0222b5bc[] = "DWCi_MOV_WH_SYSSTATE_DATASHARING";

extern "C" char data_ov001_0222b5e0[] = "DWCi_MOV_WH_SYSSTATE_CONNECT_FAIL";

extern "C" char data_ov001_0222b604[] = "DWCi_MOV_WH_SYSSTATE_MEASURECHANNEL";

extern "C" char *sWfcMoveWhSysStateNames[10] = {data_ov001_0222b4ec, data_ov001_0222b508, data_ov001_0222b55c, data_ov001_0222b524, data_ov001_0222b57c, data_ov001_0222b5bc, data_ov001_0222b59c, data_ov001_0222b604, data_ov001_0222b5e0, data_ov001_0222b540};

extern "C" WfcMoveWhWork *sWfcMoveWh = 0;

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov001_022218a4) {
    HOST_OVERLAY_VAR(data_ov001_0222b4ec),
    HOST_OVERLAY_VAR(data_ov001_0222b508),
    HOST_OVERLAY_VAR(sWfcMoveWhDebugPrint),
    HOST_OVERLAY_VAR(data_ov001_0222b524),
    HOST_OVERLAY_VAR(data_ov001_0222b540),
    HOST_OVERLAY_VAR(data_ov001_0222b55c),
    HOST_OVERLAY_VAR(data_ov001_0222b57c),
    HOST_OVERLAY_VAR(data_ov001_0222b59c),
    HOST_OVERLAY_VAR(data_ov001_0222b5bc),
    HOST_OVERLAY_VAR(data_ov001_0222b5e0),
    HOST_OVERLAY_VAR(data_ov001_0222b604),
    HOST_OVERLAY_VAR(sWfcMoveWhSysStateNames),
    HOST_OVERLAY_VAR(sWfcMoveWh),
    HOST_OVERLAY_VARS_END
};
