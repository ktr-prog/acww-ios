#include "uptr.h"
// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "nitro/wm.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/main/unk_0206d49c.h"
#include "proto/ov067/unk_ov067_0225f1a0.h"

struct WlxBlockEntry {
    u32 ggid;
    void (*doneCallback)(u32, void *);
    u8 *sendData; // (bufferInfo[0..7] in the DS layout)
    u32 sendSize;
    u32 unk_10[2];
    u8 *recvData;
    u32 recvBufSize;
    u32 unk_20[2];
};

struct WlxBlock {
    u8 reqSeq;
    u8 reqKind;
    u16 reqSegment;
    u8 ackSeq;
    u8 ackKind;
    u16 ackSegment;
    u8 *sendData;
    u32 sendSize;
    u32 unk_10;
    u16 sendCheck;
    u16 unk_16;
    u8 *recvData;
    u32 recvSize;
    u32 recvBufSize;
    u16 recvCheck;
    u16 unk_26;
    u32 recvBitmap[0x56];
    u16 sendSegSize;
    u16 recvSegSize;
    u32 numRecvSegments;
    u32 numMissingSegments;
    u16 lastReqSegments[2];
    WlxBlockEntry *curEntry;
    WlxBlockEntry entries[16];
};

struct WlxBlockPacketHdr {
    u8 reqSeq;
    u8 reqKind;
    u16 reqSegment;
    u8 ackSeq;
    u8 ackKind;
    u16 ackSegment;
};

// the (seq, kind, segment) triple of WlxBlockPacketHdr / WlxBlock, copied as two halfwords
struct WlxBlockSeqWords {
    u16 w0;
    u16 w1;
};

struct WlxBlockSeqCopy {
    WlxBlockSeqWords words;
};

// argument of Wlx_OnWmEvent events 7 (packet to send) and 8 (received packet)
struct WlxMpPacket {
    WlxBlockPacketHdr *packet;
    u16 length;
    u16 aidMask;
};

struct WlxWmMsg {
    u16 apiid;
    u16 errcode;
    u16 mpState;
    u16 unk_06;
    union {
        u16 state;   // 0x08: StartParent / StartConnect / scan state
        u16 channel; // 0x08: measured channel
    };
    union {
        u16 myAid;     // 0x0a: StartConnect: own aid
        u16 busyRatio; // 0x0a: measured busy ratio
        u16 mac01;     // 0x0a: StartParent: MAC of the child
    };
    union {
        u16 unk_0c;
        u16 mac23;
    };
    union {
        u16 numBeacons; // 0x0e: scan: number of BSS descriptions
        u16 mac45;
    };
    u16 aid; // 0x10: StartParent: aid of the child
};

typedef s32 (*WlxWmEventFn)(u32, void *);

struct WlxWmWork {
    u8 wmBuf[0xf00];      // WM_Init work area
    u8 mpSendBuf[0x220];  // WM_StartMPEx send buffer (sendBufSize)
    u8 mpRecvBuf[0x3dc0]; // WM_StartMPEx receive buffer (recvBufSize)
    u8 mpSendData[0x200];
    u16 dmaNo;
    u16 channel;
    u16 aid;
    u16 connectedMask;
    u16 sendBufSize;
    u16 recvBufSize;
    s32 isSending;
    s32 state;
    s32 requestedState;
    WlxWmEventFn eventCallback;
    WMParentParam *parentParam;
    s32 needMeasureChannel;
    s32 bestChannelBusy;
    s32 numBeacons;
    u8 pad_510c[0x14];
    WMBssDesc bssDesc;     // the parent found by the scan (WM_StartConnectEx)
    u8 scanBuffer[0x400];  // WM_StartScanEx result buffer
    // WM_StartScanEx's parameter (the DS kept its fields here, the buffer's address in 32 bits; WM_StartScanEx takes a
    // WMScanExParam, whose buffer is a pointer on 64-bit hosts)
    WMScanExParam scanParam;
    u8 pad_5614[0x5640 - 0x5614 - (sizeof(WMScanExParam) - 0x34)];
};


struct WlxRole {
    s32 column;
    s32 row;
    s32 reseedRow;
    s32 parentDisabled;
    s32 pattern[4][4];
};

struct WlxMember {
    u16 aid;
    u16 mac01;
    u16 mac23;
    u16 mac45;
};

typedef void (*WlxEventFn)(s32, void *);  // (the results are not used)

struct WlxWork {
    u32 dmaNo;
    WlxEventFn eventCallback;
    WlxRole role;
    WlxMember members[16];
    u8 idleMpCount;
    u8 pad_d9[0xe0 - 0xd9];
    WMParentParam parentParam;
    WlxWmWork wm;
    WlxBlock block;
};

extern "C" {
s32 OS_DisableInterrupts(void);
s32 OS_EnableInterrupts(void);
void OS_RestoreInterrupts(s32);
s32 WM_GetNextTgid(void);
s32 WM_EndScan(void (*)(WlxWmMsg *));
void MIi_CpuClear32(u32, void *, u32);
void MIi_CpuCopy32(const void *, void *, u32);
void OS_GetMacAddress(void *);
void MI_CpuFill8(void *, u32, u32);
u32 WM_GetAllowedChannel(void);
s32 WM_StartScanEx(void (*)(WlxWmMsg *), void *);
s32 WM_StartConnectEx(void (*)(WlxWmMsg *), void *, u32, u32, u32);
s32 WM_MeasureChannel(void (*)(WlxWmMsg *), u32, u32, u32, u32);
s32 WM_StartMPEx(void (*)(WlxWmMsg *), void *, u32, void *, u32, u32, u32, u32, u32, u32, u32);
s32 WM_SetParentParameter(void (*)(WlxWmMsg *), void *);
s32 WM_StartParent(void (*)(WlxWmMsg *));
void DC_InvalidateRange(void *, u32);
void MI_CpuCopy8(void *, void *, u32);
u32 _u32_div_f(u32, u32);
s32 WM_Reset(void *);
s32 WM_Disable(void *);
s32 WM_PowerOff(void *);
s32 WM_PowerOn(void *);
s32 WM_Init(void *, u32);
s32 WM_Enable(void *);
s32 WM_SetIndCallback(void *);
s32 WM_SetPortCallback(u32, void *, u32);
s32 WM_Finish(void);
s32 WM_SetMPDataToPortEx(void *, void *, void *, u32, u32, u32, u32);

// data
extern WlxWmWork *sWlxWm;
extern WlxWork *sWlx;
extern const u32 sWlxRoleTable[16];

// functions of this overlay
void WlxBlock_BuildSend(WlxBlock *w, WlxMpPacket *msg);
void WlxBlock_SetCurrent(WlxBlock *w, void *p);
void *WlxBlock_GetCurrent(WlxBlock *w);
void WlxBlock_Init(WlxBlock *w);
void WlxWm_Init(WlxWmWork *c, WMParentParam *s, WlxWmEventFn cb, u32 v);
void WlxWm_ScanStep(WlxWmMsg *m);
void WlxWm_MeasureChannelStep(WlxWmMsg *m);
void WlxWm_ConnectStep(WlxWmMsg *m);
void WlxWm_OnChildEvent(WlxWmMsg *m);
void WlxWm_StartParentStep(WlxWmMsg *m);
void WlxWm_OnParentEvent(WlxWmMsg *m);
void WlxWm_SendMpData(WlxWmWork *s);
void WlxWm_Fail(WlxWmWork *s);
void WlxRole_Init(WlxRole *p);
}

extern "C" char data_ov067_02261b3c[14];
extern "C" char data_ov067_02261b0c[13];
extern "C" char data_ov067_02261c1c[18];
extern "C" char data_ov067_02261b4c[14];
extern "C" char data_ov067_02261cd0[20];
extern "C" char data_ov067_02261bcc[17];
extern "C" char data_ov067_02261e70[27];
extern "C" char data_ov067_02261a8c[10];
extern "C" char data_ov067_02261e00[25];
extern "C" char *sWmApiNames[44];
extern "C" char data_ov067_02261a74[9];
extern "C" char data_ov067_02261ac8[12];
extern "C" char data_ov067_02261a68[9];
extern "C" char data_ov067_02261b7c[15];
extern "C" char data_ov067_02261aa4[11];
extern "C" char data_ov067_02261d10[22];
extern "C" char data_ov067_02261b9c[15];
extern "C" char data_ov067_02261d88[23];
extern "C" char data_ov067_02261da0[23];
extern "C" char data_ov067_02261d40[22];
extern "C" char data_ov067_02261b5c[14];
extern "C" char data_ov067_02261dd0[23];
extern "C" char data_ov067_02261d70[22];
extern "C" char data_ov067_02261c94[20];
extern "C" char data_ov067_02261ca8[20];
extern "C" char data_ov067_02261ea8[29];
extern "C" char data_ov067_02261e8c[28];
extern "C" char data_ov067_02261c6c[19];
extern "C" char data_ov067_02261c80[19];
extern "C" char data_ov067_02261b2c[13];
extern "C" char data_ov067_02261ae0[12];
extern "C" char data_ov067_02261b1c[13];
extern "C" char data_ov067_02261be0[18];
extern "C" char data_ov067_02261bf4[18];
extern "C" char data_ov067_02261a80[10];
extern "C" char data_ov067_02261c44[19];
extern "C" char data_ov067_02261e1c[25];
extern "C" char data_ov067_02261e54[26];
extern "C" char data_ov067_02261ad4[12];
extern "C" char data_ov067_02261b6c[15];
extern "C" char data_ov067_02261abc[11];
extern "C" char data_ov067_02261bbc[16];
extern "C" char data_ov067_02261db8[23];
extern "C" char data_ov067_02261bac[15];
extern "C" char data_ov067_02261c58[19];
extern "C" char data_ov067_02261cf8[21];
extern "C" char data_ov067_02261ab0[11];
extern "C" char data_ov067_02261afc[13];
extern "C" char data_ov067_02261cbc[20];
extern "C" char data_ov067_02261ee8[31];
extern "C" char data_ov067_02261c30[19];
extern "C" char data_ov067_02261ec8[30];
extern "C" char data_ov067_02261b8c[15];
extern "C" char data_ov067_02261d28[22];
extern "C" char data_ov067_02261de8[23];
extern "C" char data_ov067_02261f08[32];
extern "C" char data_ov067_02261a98[11];
extern "C" char data_ov067_02261ce4[20];
extern "C" char data_ov067_02261a60[7];
extern "C" char data_ov067_02261aec[13];
extern "C" char data_ov067_02261d58[22];
extern "C" char data_ov067_02261c08[18];
extern "C" char data_ov067_02261e38[25];
extern "C" char *sWmErrCodeNames[20];

extern "C" char data_ov067_02261b3c[14] = "WM_SetDCFData";

extern "C" char data_ov067_02261b0c[13] = "WM_SetMPData";

extern "C" char data_ov067_02261c1c[18] = "WM_MeasureChannel";

extern "C" char data_ov067_02261b4c[14] = "WM_Initialize";

extern "C" char data_ov067_02261cd0[20] = "WM_ERRCODE_NO_ENTRY";

extern "C" char data_ov067_02261bcc[17] = "WM_EndKeySharing";

extern "C" char data_ov067_02261e70[27] = "WM_ERRCODE_SEND_QUEUE_FULL";

extern "C" char data_ov067_02261a8c[10] = "WM_EndDCF";

extern "C" char data_ov067_02261e00[25] = "WM_ERRCODE_INVALID_PARAM";

extern "C" char *sWmApiNames[44] = {
    data_ov067_02261b4c,
    data_ov067_02261a74,
    data_ov067_02261a60,
    data_ov067_02261a80,
    data_ov067_02261a98,
    data_ov067_02261abc,
    data_ov067_02261ae0,
    data_ov067_02261d10,
    data_ov067_02261b8c,
    data_ov067_02261aec,
    data_ov067_02261afc,
    data_ov067_02261ab0,
    data_ov067_02261bbc,
    data_ov067_02261b5c,
    data_ov067_02261aa4,
    data_ov067_02261b0c,
    data_ov067_02261a68,
    data_ov067_02261ac8,
    data_ov067_02261b3c,
    data_ov067_02261a8c,
    data_ov067_02261b2c,
    data_ov067_02261c58,
    data_ov067_02261bcc,
    data_ov067_02261b1c,
    data_ov067_02261bac,
    data_ov067_02261db8,
    data_ov067_02261c80,
    data_ov067_02261c08,
    data_ov067_02261dd0,
    data_ov067_02261b9c,
    data_ov067_02261c1c,
    data_ov067_02261de8,
    data_ov067_02261d70,
    data_ov067_02261ad4,
    data_ov067_02261f08,
    data_ov067_02261be0,
    data_ov067_02261ce4,
    data_ov067_02261ea8,
    data_ov067_02261b6c,
    data_ov067_02261b7c,
    data_ov067_02261ca8,
    data_ov067_02261ee8,
    data_ov067_02261ee8,
    data_ov067_02261ee8
};

extern "C" char data_ov067_02261a74[9] = "WM_Reset";

extern "C" WlxWmWork *sWlxWm = 0;

extern "C" char data_ov067_02261ac8[12] = "WM_StartDCF";

extern "C" WlxWork *sWlx = 0;

extern "C" char data_ov067_02261a68[9] = "WM_EndMP";

extern "C" char data_ov067_02261b7c[15] = "WM_SetWEPKeyEx";

extern "C" char data_ov067_02261aa4[11] = "WM_StartMP";

extern "C" char data_ov067_02261d10[22] = "WM_SetParentParameter";

extern "C" char data_ov067_02261b9c[15] = "WM_SetLifeTime";

extern "C" char data_ov067_02261d88[23] = "WM_ERRCODE_SEND_FAILED";

extern "C" char data_ov067_02261da0[23] = "WM_ERRCODE_FLASH_ERROR";

extern "C" char data_ov067_02261d40[22] = "WM_ERRCODE_WM_DISABLE";

extern "C" char data_ov067_02261b5c[14] = "WM_Disconnect";

extern "C" char data_ov067_02261dd0[23] = "(W-Alarm ind. in ARM7)";

extern "C" char data_ov067_02261d70[22] = "WM_GetWirelessCounter";

extern "C" char data_ov067_02261c94[20] = "WM_ERRCODE_NO_CHILD";

extern "C" char data_ov067_02261ca8[20] = "WM_SetPowerSaveMode";

extern "C" char data_ov067_02261ea8[29] = "(auto-disconnection in ARM7)";

extern "C" char data_ov067_02261e8c[28] = "WM_ERRCODE_WL_INVALID_PARAM";

extern "C" char data_ov067_02261c6c[19] = "WM_ERRCODE_SUCCESS";

extern "C" char data_ov067_02261c80[19] = "(WM_StartTestMode)";

extern "C" char data_ov067_02261b2c[13] = "WM_SetWEPKey";

extern "C" char data_ov067_02261ae0[12] = "WM_PowerOff";

extern "C" char data_ov067_02261b1c[13] = "WM_GetKeySet";

extern "C" char data_ov067_02261be0[18] = "WM_SetMPFrequency";

extern "C" char data_ov067_02261bf4[18] = "WM_ERRCODE_FAILED";

extern "C" char data_ov067_02261a80[10] = "WM_Enable";

extern "C" char data_ov067_02261c44[19] = "WM_ERRCODE_TIMEOUT";

extern "C" char data_ov067_02261e1c[25] = "WM_ERRCODE_WL_LENGTH_ERR";

extern "C" char data_ov067_02261e54[26] = "WM_ERRCODE_OVER_MAX_ENTRY";

extern "C" char data_ov067_02261ad4[12] = "WM_SetEntry";

extern "C" char data_ov067_02261b6c[15] = "WM_StartScanEx";

extern "C" char data_ov067_02261abc[11] = "WM_PowerOn";

extern "C" char data_ov067_02261bbc[16] = "WM_StartConnect";

extern "C" char data_ov067_02261db8[23] = "WM_SetBeaconIndication";

extern "C" char data_ov067_02261bac[15] = "WM_SetGameInfo";

extern "C" const u32 sWlxRoleTable[16] = {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1};

extern "C" char data_ov067_02261c58[19] = "WM_StartKeySharing";

extern "C" char data_ov067_02261cf8[21] = "WM_ERRCODE_OPERATING";

extern "C" char data_ov067_02261ab0[11] = "WM_EndScan";

extern "C" char data_ov067_02261afc[13] = "WM_StartScan";

extern "C" char data_ov067_02261cbc[20] = "WM_ERRCODE_DCF_TEST";

extern "C" char data_ov067_02261ee8[31] = "(MP-timing controller in ARM7)";

extern "C" char data_ov067_02261c30[19] = "WM_ERRCODE_NO_DATA";

extern "C" char data_ov067_02261ec8[30] = "WM_ERRCODE_INVALID_POLLBITMAP";

extern "C" char data_ov067_02261b8c[15] = "WM_StartParent";

extern "C" char data_ov067_02261d28[22] = "WM_ERRCODE_FIFO_ERROR";

extern "C" char data_ov067_02261de8[23] = "WM_InitWirelessCounter";

extern "C" char data_ov067_02261f08[32] = "(auto-deauthentication in ARM7)";

extern "C" char data_ov067_02261a98[11] = "WM_Disable";

extern "C" char data_ov067_02261ce4[20] = "WMi_SetBeaconPeriod";

extern "C" char data_ov067_02261a60[7] = "WM_End";

extern "C" char data_ov067_02261aec[13] = "WM_EndParent";

extern "C" char data_ov067_02261d58[22] = "WM_ERRCODE_NO_DATASET";

extern "C" char data_ov067_02261c08[18] = "(WM_StopTestMode)";

extern "C" char data_ov067_02261e38[25] = "WM_ERRCODE_ILLEGAL_STATE";

extern "C" char *sWmErrCodeNames[20] = {
    data_ov067_02261c6c,
    data_ov067_02261bf4,
    data_ov067_02261cf8,
    data_ov067_02261e38,
    data_ov067_02261d40,
    data_ov067_02261d58,
    data_ov067_02261e00,
    data_ov067_02261c94,
    data_ov067_02261d28,
    data_ov067_02261c44,
    data_ov067_02261e70,
    data_ov067_02261cd0,
    data_ov067_02261e54,
    data_ov067_02261ec8,
    data_ov067_02261c30,
    data_ov067_02261d88,
    data_ov067_02261cbc,
    data_ov067_02261e8c,
    data_ov067_02261e1c,
    data_ov067_02261da0
};

extern "C" s32 sWlxState = 0;

#pragma thumb off

// Count leading zeros: the NitroSDK form of MATH_CountLeadingZeros (math.h), a one-instruction inline asm in the
// SDK itself. mwcc 1.2 has no intrinsic for clz. Kept by decision (see the project's assembly policy).
static inline u32 Clz(u32 x) {
    return x == 0 ? 32 : __builtin_clz(x);
}




extern "C" void Wlx_SelectNextEntry(void) {
    WlxBlockEntry *r;
    r = (WlxBlockEntry *)WlxBlock_FindEntry(&sWlx->block, sWlx->block.curEntry, 0, 0);
    if (r == NULL) {
        r = (WlxBlockEntry *)WlxBlock_FindEntry(&sWlx->block, 0, 0, 1);
    }
    WlxBlock_SetCurrent(&sWlx->block, r);
    sWlx->parentParam.ggid = r->ggid;
}

extern "C" s32 Wlx_OnWmEvent(u32 cmd, void *arg) {
    s32 ret = 0;
    WlxWmMsg *m = (WlxWmMsg *)arg;
    switch (cmd) {
    case 5:
        if (Wlx_GetState() == 3) {
            WlxBlock_OnMpEndStub(&sWlx->block, arg);
            if (sWlx->wm.connectedMask == 0) {
                sWlx->idleMpCount++;
                if (sWlx->idleMpCount > 10) {
                    sWlx->idleMpCount = ret;
                    if (WlxRole_NextIsParent(&sWlx->role) == 0) {
                        WlxWm_RequestState(&sWlx->wm, 5);
                    }
                }
            }
        }
        break;
    case 6:
        ret = WlxBlock_SelectByBeacon(&sWlx->block, (u32 *)arg);
        if (ret != 0) {
            WlxBlockEntry *e = (WlxBlockEntry *)WlxBlock_GetCurrent(&sWlx->block);
            sWlx->parentParam.ggid = e->ggid;
        }
        break;
    case 0: {
        WlxWork *g = sWlx;
        s32 ie = OS_DisableInterrupts();
        sWlxState = 0;
        WlxEventFn cb = sWlx->eventCallback;
        if (cb != NULL) {
            cb(0, g);
        }
        OS_RestoreInterrupts(ie);
        break;
    }
    case 2: {
        s32 ie = OS_DisableInterrupts();
        sWlxState = 2;
        WlxEventFn cb = sWlx->eventCallback;
        if (cb != NULL) {
            cb(2, 0);
        }
        OS_RestoreInterrupts(ie);
        break;
    }
    case 1:
        if (Wlx_GetState() != 3) {
            WlxWm_RequestState(&sWlx->wm, 0);
        } else if (WlxRole_NextIsParent(&sWlx->role) != 0) {
            Wlx_SelectNextEntry();
            WlxWm_RequestState(&sWlx->wm, 4);
        } else {
            WlxWm_RequestState(&sWlx->wm, 5);
        }
        break;
    case 3:
        if (Wlx_GetState() != 3) {
            WlxWm_RequestState(&sWlx->wm, 0);
        }
        sWlx->idleMpCount = 0;
        {
            WlxWork *g = sWlx;
            u16 i = g->wm.aid;
            WlxMember *r = &g->members[i];
            r->aid = i;
            OS_GetMacAddress(&r->mac01);
        }
        break;
    case 4:
        if (Wlx_GetState() != 3) {
            WlxWm_RequestState(&sWlx->wm, 0);
        }
        {
            WlxWork *g = sWlx;
            u16 i = g->wm.aid;
            WlxMember *r = &g->members[i];
            r->aid = i;
            OS_GetMacAddress(&r->mac01);
        }
        break;
    case 9: {
        WlxBlockEntry *e = (WlxBlockEntry *)WlxBlock_GetCurrent(&sWlx->block);
        s32 f = Wlx_IsParent();
        u32 x, y, idx;
        if (f != 0) {
            x = sWlx->parentParam.parentMaxSize;
        } else {
            x = sWlx->wm.bssDesc.gameInfo.parentMaxSize;
        }
        if (f != 0) {
            y = sWlx->parentParam.childMaxSize;
        } else {
            y = sWlx->wm.bssDesc.gameInfo.childMaxSize;
        }
        idx = (u16)(f != 0 ? m->aid : 0);
        WlxWork *g = sWlx;
        WlxMember *tbl = g->members;
        WlxMember *r = &tbl[idx];
        WlxBlock_Reset(&g->block, x, y);
        r->aid = idx;
        if (f != 0) {
            r->mac01 = m->mac01;
            r->mac23 = m->mac23;
            r->mac45 = m->mac45;
        } else {
            const u16 *q = (const u16 *)sWlx->wm.bssDesc.bssid;
            r->mac01 = q[0];
            r->mac23 = q[1];
            r->mac45 = q[2];
        }
        if (Wlx_GetState() == 3 && e != NULL) {
            WlxBlock_StartSend(&sWlx->block, e->sendData, e->sendSize, e->recvData, e->recvBufSize);
        } else {
            sWlx->block.reqKind = 5;
        }
        WlxBlock_OnConnectStub(&sWlx->block, (u16)(1 << idx));
        if (sWlx->block.reqKind != 5) {
            WlxEventFn cb = sWlx->eventCallback;
            if (cb != NULL) {
                cb(4, r);
            }
        }
        break;
    }
    case 10:
        WlxBlock_OnDisconnectStub(&sWlx->block, (u16)(uptr)arg);  // event 10: arg is the aid
        if (Wlx_GetConnectedMask() == 0) {
            if (Wlx_GetState() != 3) {
                if (Wlx_GetState() != 1) {
                    if (WlxBlock_GetCurrent(&sWlx->block) != NULL) {
                        break;
                    }
                }
            }
            sWlx->idleMpCount = 0;
            WlxWm_RequestState(&sWlx->wm, 3);
        }
        break;
    case 7:
        WlxBlock_BuildSend(&sWlx->block, (WlxMpPacket *)arg);
        break;
    case 8:
        ret = WlxBlock_Receive(&sWlx->block, (WlxMpPacket *)arg);
        break;
    default:
        Fatal_Trap();
        break;
    }
    return ret;
}

extern "C" void Wlx_Init(void *work, void (*cb)(s32, void *), u32 c) {
    volatile u32 z;
    s32 ie = OS_DisableInterrupts();
    if (Wlx_GetState() == 0) {
        if (((uptr)work & 0x1f) != 0) {
            Fatal_Trap();
        }
        z = 0;
        sWlx = (WlxWork *)work;
        MIi_CpuClear32(z, work, 0x5b74);
        sWlx->dmaNo = c;
        sWlx->eventCallback = cb;
        WlxRole_Init(&sWlx->role);
        WlxBlock_Init(&sWlx->block);
        sWlx->parentParam.maxEntry = 1;
        sWlx->parentParam.parentMaxSize = 0x200;
        sWlx->parentParam.childMaxSize = 0x200;
        sWlx->parentParam.CS_Flag = 1;
        {
            WlxWork *g = sWlx;
            WlxWm_Init(&g->wm, &g->parentParam, Wlx_OnWmEvent, g->dmaNo);
        }
        s32 ie2 = OS_DisableInterrupts();
        sWlxState = 2;
        WlxEventFn cb = sWlx->eventCallback;
        if (cb != NULL) {
            cb(2, 0);
        }
        OS_RestoreInterrupts(ie2);
    }
    OS_RestoreInterrupts(ie);
}

extern "C" s32 Wlx_SetPacketSizes(u32 a, u32 b, u32 c) {
    s32 r = 0;
    if (c > 1 || a < 0x14 || a > 0x200 || b < 0x14 || b > 0x200) {
    } else {
        s32 t = 0x14a + (a + 0x26) * 4 + c * ((b + 0x20) * 4 + 0x70);
        if (t < 0x15e0) {
            r = 1;
            sWlx->parentParam.parentMaxSize = a;
            sWlx->parentParam.childMaxSize = b;
        }
    }
    return r;
}

extern "C" void Wlx_StartExchange(void) {
    s32 ie = OS_DisableInterrupts();
    if (Wlx_GetState() == 2) {
        WlxWm_RequestState(&sWlx->wm, 3);
        s32 ie2 = OS_DisableInterrupts();
        sWlxState = 3;
        WlxEventFn cb = sWlx->eventCallback;
        if (cb != NULL) {
            cb(3, 0);
        }
        OS_RestoreInterrupts(ie2);
    }
    OS_RestoreInterrupts(ie);
}

extern "C" void Wlx_StopExchange(void) {
    s32 ie = OS_DisableInterrupts();
    if (Wlx_GetState() == 3) {
        WlxWm_RequestState(&sWlx->wm, 2);
    }
    OS_RestoreInterrupts(ie);
}

extern "C" void Wlx_Stop(void) {
    s32 ie = OS_DisableInterrupts();
    switch (Wlx_GetState()) {
    case 0:
    case 1:
        break;
    case 2:
    case 3: {
        s32 ie2 = OS_DisableInterrupts();
        sWlxState = 1;
        WlxEventFn cb = sWlx->eventCallback;
        if (cb != NULL) {
            cb(1, 0);
        }
        OS_RestoreInterrupts(ie2);
        if (Wlx_GetConnectedMask() == 0) {
            WlxWm_RequestState(&sWlx->wm, 0);
        }
        break;
    }
    }
    OS_RestoreInterrupts(ie);
}

extern "C" s32 Wlx_GetState(void) {
    return sWlxState;
}

extern "C" s32 Wlx_IsParent(void) {
    if (sWlx->wm.state == 4) {
        return 1;
    }
    return 0;
}

extern "C" u16 Wlx_GetConnectedMask(void) {
    u16 r = sWlx->wm.connectedMask;
    if (r != 0) {
        r = r | (1 << sWlx->wm.aid);
    }
    return r;
}

extern "C" void Wlx_RegisterData(u32 a0, void (*a1)(u32, void *), u8 *a2, u32 a3, u8 *a4, u32 a5) {
    s32 ie = OS_DisableInterrupts();
    WlxBlockEntry *e = (WlxBlockEntry *)WlxBlock_FindEntry(&sWlx->block, 0, a0, 1);
    if (e == NULL) {
        e = (WlxBlockEntry *)WlxBlock_FindEntry(&sWlx->block, 0, 0, 1);
        if (e == NULL) {
            Fatal_Trap();
        } else {
            e->ggid = a0;
            e->doneCallback = a1;
            e->sendData = a2;
            e->sendSize = a3;
            e->recvData = a4;
            e->recvBufSize = a5;
        }
    }
    OS_RestoreInterrupts(ie);
}

extern "C" void WlxRole_Init(WlxRole *p) {
    p->column = (u32)OS_GetTick() & 3;
    p->row = (u32)(OS_GetTick() >> 2) & 3;
    p->reseedRow = 0;
    p->parentDisabled = 0;
    MIi_CpuCopy32(sWlxRoleTable, p->pattern, 0x40);
}

extern "C" s32 WlxRole_NextIsParent(WlxRole *p) {
    p->column++;
    if (p->column >= 4) {
        p->column = 0;
        p->row++;
        if (p->row >= 4) {
            p->row = 0;
        }
        if (p->row == p->reseedRow) {
            p->reseedRow = (u32)OS_GetTick() & 3;
            p->row = p->reseedRow;
        }
    }
    if (p->pattern[p->row][p->column] != 0) {
        if (p->parentDisabled == 0) {
            return 1;
        }
    }
    return 0;
}

extern "C" void WlxWm_Fail(WlxWmWork *s) {
    if (s->state == 1) {
        s->state = s->requestedState;
    }
    WlxWm_RequestState(s, 0);
}

extern "C" void WlxWm_HandleError(WlxWmWork *s, s32 id, u32 arg) {
    if (arg == 0) {
        return;
    }
    switch (id) {
    case 15:
        break;
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
        WlxWm_Fail(s);
        break;
    case 7:
    case 8:
    case 11:
    case 13:
    case 14:
    case 30:
    case 0x26:
        WlxWm_Fail(s);
        break;
    case 12:
        if (arg == 1 || arg - 0xb <= 1) {
            s->state = 5;
            WlxWm_RequestState(s, 3);
        } else {
            WlxWm_Fail(s);
        }
        break;
    case 0x80:
        WlxWm_Fail(s);
        break;
    case 0x81:
        break;
    }
}

extern "C" s32 WlxWm_CheckApiCall(WlxWmWork *s, u32 id, u32 arg) {
    s32 r = WlxDebug_CheckApiResult(id, arg);
    if (r == 0) {
        WlxWm_HandleError(s, id, arg);
    }
    return r;
}

extern "C" s32 WlxWm_CheckCallback(WlxWmWork *s, void *m) {
    s32 r = WlxDebug_CheckCallbackResult((u16 *)m);
    if (r == 0) {
        WlxWm_HandleError(s, ((u16 *)m)[0], ((u16 *)m)[1]);
    }
    return r;
}

extern "C" void WlxWm_SendMpData(WlxWmWork *s) {
    struct {
        void *d;
        u16 e;
        u16 f;
    } l;
    if (s->isSending != 0) {
        return;
    }
    u32 v;
    if (s->aid == 0) {
        v = s->parentParam->parentMaxSize;
    } else {
        v = s->bssDesc.gameInfo.childMaxSize;
    }
    u16 fl = s->connectedMask;
    l.e = v;
    l.d = s->mpSendData;
    l.f = fl;
    if (s->eventCallback != NULL) {
        s->eventCallback(7, &l.d);
    }
    if (l.e > v) {
        return;
    }
    s->isSending = WlxWm_CheckApiCall(s, 0xf, WM_SetMPDataToPortEx((void *)WlxWm_OnMpDataSent, NULL, l.d, l.e, l.f, 4, 2));
}

extern "C" void WlxWm_SetState(WlxWmWork *s, s32 st, void *arg) {
    s->state = st;
    s32 prev = s->requestedState;
    if (prev == st) {
        switch (st) {
        case 0:
        case 1:
            break;
        case 2:
            if (s->eventCallback == NULL) {
                return;
            }
            s->eventCallback(2, 0);
            return;
        case 3:
            if (s->eventCallback == NULL) {
                return;
            }
            s->eventCallback(1, 0);
            return;
        case 4:
            s->isSending = 0;
            if (s->eventCallback == NULL) {
                return;
            }
            s->eventCallback(3, 0);
            return;
        case 5:
            s->isSending = 0;
            if (s->eventCallback != NULL) {
                s->eventCallback(4, 0);
            }
            s->connectedMask |= 1;
            if (s->eventCallback != NULL) {
                s->eventCallback(9, arg);
            }
            WlxWm_SendMpData(s);
            return;
        }
    } else {
    switch (st) {
    case 0:
        WlxWm_EnableStep(NULL);
        return;
    case 1:
        break;
    case 2:
        switch (prev) {
        case 0:
            WlxWm_DisableStep(NULL);
            return;
        case 1:
        case 2:
            break;
        case 3:
        case 4:
        case 5:
            WlxWm_PowerOnStep(NULL);
            return;
        }
        break;
    case 3:
        switch (prev) {
        case 0:
        case 2:
            WlxWm_PowerOffStep(NULL);
            return;
        case 1:
        case 3:
            break;
        case 4:
            if (s->needMeasureChannel != 0) {
                WlxWm_MeasureChannelStep(0);
                return;
            }
            WlxWm_StartParentStep(0);
            return;
        case 5:
            WlxWm_ScanStep(0);
            return;
        }
        break;
    case 4:
    case 5:
        WlxWm_ResetStep(NULL);
        return;
    }
    }
}

extern "C" void WlxWm_OnMpDataSent(void *m) {
    WlxWmWork *s = sWlxWm;
    WlxWm_CheckCallback(s, m);
    s->isSending = 0;
    if (s->connectedMask == 0) {
        return;
    }
    WlxWm_SendMpData(s);
}

extern "C" void WlxWm_OnIndication(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (m->errcode != 8) {
        return;
    }
    WlxDebug_Printf("WM_ERRCODE_FIFO_ERROR Indication!\n");
    s->requestedState = 6;
    s->state = 6;
}

extern "C" void WlxWm_OnPortRecv(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (WlxWm_CheckCallback(s, m) == 0) {
        return;
    }
    u32 t = m->state;
    WlxMpPacket c;
    switch (t) {
    case 7:
        return;
    case 0x15: {
        s32 r = 0;
        c.aidMask = 1 << m->aid;
        c.length = m->length;
        c.packet = (WlxBlockPacketHdr *)m->data;
        if (s->eventCallback != NULL) {
            r = s->eventCallback(8, &c);
        }
        if (r == 0) {
            return;
        }
        WlxWm_RequestState(s, 3);
        return;
    }
    case 9: {
        WlxWmEventFn cb = s->eventCallback;
        u32 sh = 1 << m->aid;
        if (cb == NULL) {
            return;
        }
        cb(10, (void *)(uptr)sh);  // (event 10: the departed aid's bit as the argument)
        return;
    }
    }
}

extern "C" void WlxWm_EnableStep(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(s, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        s->needMeasureChannel = 1;
        s->state = 1;
        WlxWm_CheckApiCall(s, 0, WM_Init(s->wmBuf, s->dmaNo));
        WlxWm_CheckApiCall(s, 3, WM_Enable((void *)WlxWm_EnableStep));
        return;
    }
    if (m->apiid != 3) {
        return;
    }
    if (WlxWm_CheckApiCall(s, 0x80, WM_SetIndCallback((void *)WlxWm_OnIndication)) == 0) {
        return;
    }
    if (WlxWm_CheckApiCall(s, 0x81, WM_SetPortCallback(4, (void *)WlxWm_OnPortRecv, 0)) == 0) {
        return;
    }
    WlxWm_SetState(s, 2, 0);
}

extern "C" void WlxWm_PowerOnStep(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(s, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        s->state = 1;
        WlxWm_CheckApiCall(s, 5, WM_PowerOn((void *)WlxWm_PowerOnStep));
        return;
    }
    if (m->apiid != 5) {
        return;
    }
    WlxWm_SetState(s, 3, 0);
}

extern "C" void WlxWm_PowerOffStep(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(s, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        s->state = 1;
        WlxWm_CheckApiCall(s, 6, WM_PowerOff((void *)WlxWm_PowerOffStep));
        return;
    }
    if (m->apiid != 6) {
        return;
    }
    WlxWm_SetState(s, 2, 0);
}

extern "C" void WlxWm_DisableStep(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(s, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        s->state = 1;
        WlxWm_CheckApiCall(s, 4, WM_Disable((void *)WlxWm_DisableStep));
        return;
    }
    if (m->apiid != 4) {
        return;
    }
    if (WlxWm_CheckApiCall(s, 2, WM_Finish()) == 0) {
        return;
    }
    sWlxWm = NULL;
    s->state = 0;
    if (s->eventCallback == NULL) {
        return;
    }
    s->eventCallback(0, 0);
}

extern "C" void WlxWm_ResetStep(WMPortRecvCallback *m) {
    WlxWmWork *s = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(s, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        s->state = 1;
        WlxWm_CheckApiCall(s, 1, WM_Reset((void *)WlxWm_ResetStep));
        return;
    }
    if (m->apiid != 1) {
        return;
    }
    s->aid = 0;
    WlxWm_SetState(s, 3, 0);
}

extern "C" void WlxWm_OnParentEvent(WlxWmMsg *m) {
    WlxWmWork *c;
    s32 t = m->state;
    c = sWlxWm;
    if (t == 0) {
        WlxWm_StartParentStep(m);
        return;
    }
    if (m->errcode != 0) {
        return;
    }
    switch (t) {
    case 0:
        return;
    case 7: {
        BOOL first = c->connectedMask == 0 ? TRUE : FALSE;
        WlxDebug_Printf("connected(%02X+=%02X)\n", c->connectedMask, 1 << m->aid);
        c->connectedMask = c->connectedMask | (u16)(1 << m->aid);
        if (c->eventCallback != NULL) {
            c->eventCallback(9, m);
        }
        if (first) {
            WlxWm_SendMpData(c);
        }
        return;
    }
    case 9:
        WlxDebug_Printf("disconnected(%02X-=%02X)\n", c->connectedMask, 1 << m->aid);
        c->connectedMask = c->connectedMask & (u16)~(1 << m->aid);
        WlxWmEventFn cb;
        cb = c->eventCallback;
        u32 a;
        a = 1 << m->aid;
        if (cb != NULL) {
            cb(0xa, (void *)(uptr)a);  // (event 10: the aid bit as the argument)
        }
        return;
    case 2: {
        void *a = c->parentParam;
        if (c->eventCallback != NULL) {
            c->eventCallback(5, a);
        }
        return;
    }
    }
}

extern "C" void WlxWm_StartParentStep(WlxWmMsg *m) {
    WlxWmWork *c = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(c, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        c->state = 1;
        c->parentParam->channel = c->channel;
        c->parentParam->tgid = WM_GetNextTgid();
        WlxDebug_Printf("start parent. (%2dch, TGID=%02X, GGID=%04X)\n", c->channel, c->parentParam->tgid, c->parentParam->ggid);
        WlxWm_CheckApiCall(c, 7, WM_SetParentParameter(WlxWm_StartParentStep, c->parentParam));
        return;
    }
    if (m->apiid == 7) {
        WlxWm_CheckApiCall(c, 8, WM_StartParent(WlxWm_OnParentEvent));
        return;
    }
    if (m->apiid == 8) {
        BOOL b = FALSE;
        c->aid = b;
        c->connectedMask = b;
        if (c->parentParam->CS_Flag == 0) {
            b = TRUE;
        }
        WlxWm_CheckApiCall(c, 0xe, WM_StartMPEx(WlxWm_StartParentStep, c->mpRecvBuf, c->recvBufSize, c->mpSendBuf, c->sendBufSize, (u16)b, 0, 0, 0, 0, 0));
        return;
    }
    if (m->apiid != 0xe) {
        return;
    }
    if (m->mpState != 0xa) {
        return;
    }
    WlxWm_SetState(c, 4, 0);
}

extern "C" void WlxWm_OnChildEvent(WlxWmMsg *m) {
    WlxWmWork *c = sWlxWm;
    if (WlxWm_CheckCallback(c, m) == 0) {
        return;
    }
    switch (m->state) {
    case 7:
        if (c->state == 5) {
            return;
        }
        WlxWm_ConnectStep(m);
        return;
    case 9:
        if (c->state == 1) {
            c->requestedState = 3;
            return;
        }
        c->requestedState = 4;
        WlxWm_ResetStep(0);
        return;
    case 6:
    case 8:
        break;
    default:
        WlxWm_Fail(c);
        break;
    }
}

extern "C" void WlxWm_ConnectStep(WlxWmMsg *m) {
    WlxWmWork *c = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(c, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        c->state = 1;
        WlxWm_CheckApiCall(c, 0xc, WM_StartConnectEx(WlxWm_OnChildEvent, &c->bssDesc, 0, 1, 0));
        return;
    }
    if (m->apiid == 0xc) {
        c->aid = m->myAid;
        BOOL b = c->parentParam->CS_Flag == 0 ? TRUE : FALSE;
        WlxWm_CheckApiCall(c, 0xe, WM_StartMPEx(WlxWm_ConnectStep, c->mpRecvBuf, c->recvBufSize, c->mpSendBuf, c->sendBufSize, (u16)b, 0, 0, 0, 0, 0));
        return;
    }
    if (m->apiid != 0xe) {
        return;
    }
    if (m->mpState != 0xa) {
        return;
    }
    WlxWm_SetState(c, 5, m);
}

extern "C" void WlxWm_MeasureChannelStep(WlxWmMsg *m) {
    WlxWmWork *c = sWlxWm;
    u32 v = 0;
    if (m == NULL) {
        c->state = 1;
        c->channel = v;
        c->bestChannelBusy = 0x65;
    } else if (WlxWm_CheckCallback(c, m) != 0) {
        v = m->channel;
        if (c->bestChannelBusy > m->busyRatio) {
            c->bestChannelBusy = m->busyRatio;
            c->channel = v;
        }
        if (v == 32 - Clz(WM_GetAllowedChannel())) {
            c->needMeasureChannel = 0;
            WlxWm_SetState(c, 3, 0);
        }
    } else {
        c->needMeasureChannel = 0;
    }
    if (c->needMeasureChannel == 0) {
        return;
    }
    u32 a = WlxWm_NextAllowedChannel(v);
    WlxWm_CheckApiCall(c, 0x1e, WM_MeasureChannel(WlxWm_MeasureChannelStep, 3, 0x11, a, 0x1e));
}

extern "C" void WlxWm_ScanStep(WlxWmMsg *m) {
    WlxWmWork *c = sWlxWm;
    if (m != NULL) {
        if (WlxWm_CheckCallback(c, m) == 0) {
            return;
        }
    }
    if (m == NULL) {
        c->state = 1;
        c->numBeacons = 0;
        c->scanParam.scanBuf = (WMBssDesc *)c->scanBuffer;
        c->scanParam.scanBufSize = 0x400;
        c->scanParam.channelList = WM_GetAllowedChannel();
        c->scanParam.maxChannelTime = 0x6e;
        MI_CpuFill8(c->scanParam.bssid, 0xff, 6);
        c->scanParam.scanType = 1;
        c->scanParam.ssidLength = 0;
        MI_CpuFill8(c->scanParam.ssid, 0xff, 0x20);
        WlxWm_CheckApiCall(c, 0x26, WM_StartScanEx(WlxWm_ScanStep, &c->scanParam));
        return;
    }
    if (m->apiid == 0x26) {
        if (m->state == 5) {
            DC_InvalidateRange(c->scanBuffer, sizeof(c->scanBuffer));
            c->numBeacons = m->numBeacons;
        }
        WlxWm_CheckApiCall(c, 0xb, WM_EndScan(WlxWm_ScanStep));
        return;
    }
    if (m->apiid != 0xb) {
        return;
    }
    BOOL found = FALSE;
    if (c->requestedState == 5) {
        s32 i;
        u8 *p = c->scanBuffer;
        WlxDebug_Printf("found:%d beacons\n", c->numBeacons);
        i = 0;
        if (c->numBeacons > 0) {
            do {
                s32 n = *(u16 *)p << 1;
                WlxDebug_Printf("   GGID=%08X(%2dch:%3dBYTE)\n", n >= 0x48 ? *(s32 *)(p + 0x44) : -1, *(u16 *)(p + 0x36), n);
                if (n >= 0x48) {
                    found = FALSE;
                    if (c->eventCallback != NULL) {
                        found = ((s32 (*)(u32, void *))c->eventCallback)(6, p);
                    }
                    if (found) {
                        WlxDebug_Printf("     -> matched!\n");
                        MI_CpuCopy8(p, &c->bssDesc, sizeof(WMBssDesc));
                        break;
                    }
                }
                i++;
                p += (n + 3) & ~3;
            } while (i < c->numBeacons);
        }
    }
    if (found) {
        WlxWm_ConnectStep(NULL);
        return;
    }
    if (c->requestedState == 5) {
        c->requestedState = 3;
    }
    WlxWm_SetState(c, 3, 0);
}

extern "C" void WlxWm_Init(WlxWmWork *c, WMParentParam *s, WlxWmEventFn cb, u32 v) {
    s32 r = OS_EnableInterrupts();
    WM_GetNextTgid();
    OS_RestoreInterrupts(r);
    sWlxWm = c;
    volatile u32 z = 0;
    MIi_CpuClear32(z, c, sizeof *c);  // (0x5640 on the DS)
    c->aid = 0;
    c->isSending = 1;
    c->eventCallback = cb;
    c->dmaNo = v;
    c->sendBufSize = 0x220;
    c->recvBufSize = 0x3dc0;
    c->state = 0;
    c->parentParam = s;
    c->parentParam->entryFlag = 1;
    c->parentParam->beaconPeriod = 0x5a;
    c->parentParam->channel = 1;
}

extern "C" void WlxWm_RequestState(WlxWmWork *x, u32 v) {
    x->requestedState = v;
    u32 cur = x->state;
    if (cur == 1) {
        return;
    }
    if (cur == x->requestedState) {
        return;
    }
    WlxWm_SetState(x, cur, 0);
}

extern "C" void WlxBlock_Init(WlxBlock *w) {
    volatile u32 tmp;
    w->curEntry = 0;
    tmp = 0;
    MIi_CpuClear32(tmp, w, 4);
}

extern "C" void WlxBlock_Reset(WlxBlock *w, u32 a, u32 b) {
    volatile u32 tmp;
    w->sendSegSize = a - 8;
    w->recvSegSize = b - 8;
    w->reqSeq = 0;
    w->reqKind = 0;
    w->ackSeq = 0;
    w->ackKind = 0;
    w->sendData = 0;
    w->sendSize = 0;
    w->sendCheck = 0;
    tmp = 0;
    MIi_CpuClear32(tmp, &w->recvBitmap, 0x158);
    w->recvData = 0;
    w->recvSize = 0;
    w->recvBufSize = 0;
    w->numRecvSegments = 0;
}

extern "C" void WlxBlock_StartSend(WlxBlock *w, u8 *p, u32 q, u8 *r, u32 s) {
    if (w->reqKind != 0) {
        return;
    }
    w->reqKind = 1;
    w->sendData = p;
    w->sendSize = (u16)q;
    w->sendCheck = MATH_CalcChecksum8(p, q);
    w->recvData = r;
    w->recvBufSize = (u16)s;
}

extern "C" void *WlxBlock_FindEntry(WlxBlock *w, void *start, u32 key, u32 flag) {
    WlxBlockEntry *s = (WlxBlockEntry *)start;
    WlxBlockEntry *p;

    WlxBlockEntry *e;
    WlxBlockEntry *b;
    if (s == 0) {
        s = &w->entries[15];
    }
    p = s;
    e = &w->entries[16];
    b = &w->entries[0];
    do {
        BOOL m;
        p++;
        if (p >= e) {
            p = b;
        }
        m = p->ggid == key ? TRUE : FALSE;
        if (flag != 0) {
            if (m != 0) {
                goto done;
            }
        }
        if (flag == 0) {
            if (m == 0) {
                goto done;
            }
        }
    } while (p != s);
    p = 0;
done:
    return p;
}

extern "C" void *WlxBlock_GetCurrent(WlxBlock *w) {
    return w->curEntry;
}

extern "C" void WlxBlock_SetCurrent(WlxBlock *w, void *p) {
    w->curEntry = (WlxBlockEntry *)p;
}

extern "C" void WlxBlock_OnMpEndStub(void *, void *) {
}

extern "C" BOOL WlxBlock_SelectByBeacon(WlxBlock *w, u32 *x) {
    BOOL r = FALSE;
    if (x[0x44 / 4] != 0) {
        void *p = WlxBlock_FindEntry(w, 0, x[0x44 / 4], 1);
        if (p != 0) {
            WlxBlock_SetCurrent(w, p);
            r = TRUE;
        }
    }
    return r;
}

extern "C" void WlxBlock_BuildSend(WlxBlock *w, WlxMpPacket *msg) {
    WlxBlockPacketHdr *h = msg->packet;
    WlxDebug_Printf("--SEND:ACK=(%3d,%d,%04X),REQ=(%3d,%d,%04X)\n", w->ackSeq, w->ackKind, w->ackSegment, w->reqSeq, w->reqKind, w->reqSegment);
    *(WlxBlockSeqCopy *)&h->reqSeq = *(WlxBlockSeqCopy *)&w->reqSeq;
    *(WlxBlockSeqCopy *)&h->ackSeq = *(WlxBlockSeqCopy *)&w->ackSeq;
    if (w->ackSeq == w->reqSeq) {
        u16 *p = (u16 *)((u8 *)msg->packet + 8);
        switch (w->ackKind) {
        case 1:
            WlxDebug_Printf("       INIT(%6d)\n", w->sendSize);
            p[0] = w->sendSize;
            p[1] = w->sendCheck;
            break;
        case 2: {
            u32 seg = w->sendSegSize;
            u32 off = w->ackSegment * seg;
            u32 rem = w->sendSize - off;
            if (rem > seg) {
                rem = seg;
            }
            MI_CpuCopy8(w->sendData + off, p, rem);
            break;
        }
        }
    }
    msg->length = (w->sendSegSize + 9) & ~1;
}

extern "C" void WlxBlock_StoreSegment(WlxBlock *w, s32 idx, void *src) {
    u32 *bm;
    u32 rem;
    s32 j;
    u32 off;
    u32 seg;
    u32 r5;
    s32 k;
    s32 n;
    u32 *slot;
    s32 i0;
    u32 total;
    if (w->recvData == 0) {
        return;
    }
    if ((u32)idx >= w->numRecvSegments) {
        return;
    }
    bm = w->recvBitmap;
    u32 bit = 1 << (idx & 0x1f);
    slot = &bm[idx >> 5];
    u32 t = bm[idx >> 5];
    if (t & bit) {
        return;
    }
    seg = w->recvSegSize;
    off = idx * seg;
    rem = w->recvSize - off;
    if (rem > seg) {
        rem = seg;
    }
    MI_CpuCopy8(src, w->recvData + off, rem);
    *slot |= bit;
    w->numMissingSegments = w->numMissingSegments - 1;
    if (w->numMissingSegments == 0) {
        w->reqKind = 4;
        return;
    }
    i0 = w->lastReqSegments[0];
    total = w->numRecvSegments;
    r5 = i0;
    if (i0 >= total) {
        r5 = total - 1;
    }
    for (;;) {
        i0++;
        if (i0 >= total) {
            i0 = 0;
        }
        if (i0 == r5) {
            i0 = w->lastReqSegments[1];
            break;
        }
        if (w->recvBitmap[i0 >> 5] & (1 << (i0 & 0x1f))) {
            continue;
        }
        for (j = 0; j < 2; j++) {
            if (i0 == w->lastReqSegments[j]) {
                break;
            }
        }
        if (j < 2) {
            continue;
        }
        break;
    }
    n = 2; k = n; k = k - 1; while (k > 0) { w->lastReqSegments[k] = w->lastReqSegments[k - 1]; k = k - 1; }
    w->lastReqSegments[0] = i0;
    w->reqSegment = w->lastReqSegments[0];
}

extern "C" BOOL WlxBlock_Receive(WlxBlock *w, WlxMpPacket *msg) {
    WlxBlockPacketHdr *h;
    BOOL r6;
    h = msg->packet;
    r6 = FALSE;
    if (msg->length >= w->recvSegSize) {
        WlxDebug_Printf("--RECV:REQ=(%3d,%d,%04X),ACK=(%3d,%d,%04X)\n", h->reqSeq, h->reqKind, h->reqSegment, h->ackSeq, h->ackKind, h->ackSegment);
        if (h->reqSeq == w->reqSeq) {
            *(WlxBlockSeqCopy *)&w->ackSeq = *(WlxBlockSeqCopy *)&h->reqSeq;
        }
        if (h->ackSeq == w->reqSeq) {
            u16 *p2 = (u16 *)((u8 *)msg->packet + 8);
            switch (h->ackKind) {
            case 1:
                w->recvSize = p2[0];
                w->recvCheck = p2[1];
                w->numRecvSegments = (u16)_u32_div_f(w->recvSize + w->recvSegSize - 1, w->recvSegSize);
                w->numMissingSegments = w->numRecvSegments;
                w->reqSegment = 0;
                w->reqKind = 2;
                WlxDebug_Printf("       INIT(%6d)\n", w->recvSize);
                break;
            case 2:
                WlxBlock_StoreSegment(w, h->ackSegment, p2);
                break;
            case 5:
                r6 = TRUE;
                break;
            }
        }
        if (h->ackSeq == w->reqSeq) {
            if (h->ackKind == 4) {
                if (w->ackKind == 4) {
                    WlxBlockEntry *cb = w->curEntry;
                    void (*fn)(u32, void *) = cb->doneCallback;
                    WlxBlockEntry *e = (WlxBlockEntry *)WlxBlock_GetCurrent(w);
                    struct {
                        u8 *a;
                        u32 b;
                        u32 c;
                        u16 d;
                    } l;
                    volatile u32 tmp;
                    WlxBlock_SetCurrent(w, 0);
                    l.a = w->recvData;
                    l.b = w->recvSize;
                    l.c = w->recvBufSize;
                    l.d = w->recvCheck;
                    w->reqSeq = w->reqSeq + 1;
                    w->reqKind = 0;
                    w->sendData = 0;
                    w->sendSize = 0;
                    tmp = 0;
                    MIi_CpuClear32(tmp, &w->recvBitmap, 0x158);
                    w->recvData = 0;
                    w->recvSize = 0;
                    w->recvBufSize = 0;
                    w->numRecvSegments = 0;
                    if (fn != 0) {
                        fn(5, &l);
                    }
                    if (e->ggid != 0) {
                        WlxBlock_SetCurrent(w, e);
                    }
                    if (w->sendData == 0) {
                        w->reqKind = 5;
                    } else {
                        w->reqKind = 1;
                    }
                }
            }
        }
    }
    return r6;
}

extern "C" void WlxBlock_OnConnectStub(void *, u32) {
}

extern "C" void WlxBlock_OnDisconnectStub(void *, u32) {
}

extern "C" void WlxDebug_Printf(const char *fmt, ...) {
}

extern "C" char *WlxDebug_GetWmApiName(s32 n) {
    if (n < 0x2c) {
        return sWmApiNames[n];
    }
    if (n == 0x80) {
        return "WM_SetIndCallback";
    }
    if (n == 0x81) {
        return "PortSendCallback";
    }
    if (n == 0x82) {
        return "PortRecvCallback";
    }
    if (n == 0x83) {
        return "WM_ReadStatus";
    }
    return "(unknown)";
}

extern "C" char *WlxDebug_GetWmErrorName(s32 n) {
    if (n < 0x14) {
        return sWmErrCodeNames[n];
    }
    return "(unknown WMErrCode)";
}

extern "C" BOOL WlxDebug_CheckApiResult(s32 code, s32 x) {
    BOOL r = TRUE;
    if (x == 0) {
        WlxDebug_Printf(">< %s succeeded.\n", WlxDebug_GetWmApiName(code));
    } else if (x == 2) {
        WlxDebug_Printf(">  %s started.\n", WlxDebug_GetWmApiName(code));
    } else {
        char *n = WlxDebug_GetWmApiName(code);
        WlxDebug_Printf(">< %s failed. %s\n", n, WlxDebug_GetWmErrorName(x));
        r = FALSE;
    }
    return r;
}

extern "C" BOOL WlxDebug_CheckCallbackResult(u16 *m) {
    u32 r5 = m[1];
    u32 c = m[0];
    BOOL r6 = r5 == 0 ? TRUE : FALSE;
    if (r6) {
        if (c == 0x80) {
            goto end;
        }
        if (c == 0xe) {
            if (m[2] != 0xa) {
                goto end;
            }
        }
        if (c == 0xc) {
            if (m[4] != 6) {
                goto end;
            }
        }
        WlxDebug_Printf(" < %s succeeded.\n", WlxDebug_GetWmApiName(c));
    } else {
        if (c == 0xe) {
            if (r5 == 9 || r5 == 0xd || r5 == 0xf) {
                r6 = TRUE;
            }
        }
        if (r6 == 0) {
            char *n = WlxDebug_GetWmApiName(c);
            char *e = WlxDebug_GetWmErrorName(r5);
            WlxDebug_Printf(" < %s failed. %s(cmd=%02X,res=%02X)\n", n, e, m[2], m[3]);
        }
    }
end:
    return r6;
}

extern "C" u16 WlxWm_NextAllowedChannel(s32 n) {
    u32 m = WM_GetAllowedChannel();
    if (m == 0) {
        Fatal_Trap();
    } else if (m == 0x8000) {
        Fatal_Trap();
    } else {
        n++;
        if (((1 << (n - 1)) & m) == 0) {
            do {
                n++;
                if (n > 16) {
                    n = 1;
                }
            } while (((1 << (n - 1)) & m) == 0);
        }
    }
    return (u16)n;
}

#pragma thumb reset

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov067_0225f1a0) {
    HOST_OVERLAY_VAR(data_ov067_02261b3c),
    HOST_OVERLAY_VAR(data_ov067_02261b0c),
    HOST_OVERLAY_VAR(data_ov067_02261c1c),
    HOST_OVERLAY_VAR(data_ov067_02261b4c),
    HOST_OVERLAY_VAR(data_ov067_02261cd0),
    HOST_OVERLAY_VAR(data_ov067_02261bcc),
    HOST_OVERLAY_VAR(data_ov067_02261e70),
    HOST_OVERLAY_VAR(data_ov067_02261a8c),
    HOST_OVERLAY_VAR(data_ov067_02261e00),
    HOST_OVERLAY_VAR(sWmApiNames),
    HOST_OVERLAY_VAR(data_ov067_02261a74),
    HOST_OVERLAY_VAR(sWlxWm),
    HOST_OVERLAY_VAR(data_ov067_02261ac8),
    HOST_OVERLAY_VAR(sWlx),
    HOST_OVERLAY_VAR(data_ov067_02261a68),
    HOST_OVERLAY_VAR(data_ov067_02261b7c),
    HOST_OVERLAY_VAR(data_ov067_02261aa4),
    HOST_OVERLAY_VAR(data_ov067_02261d10),
    HOST_OVERLAY_VAR(data_ov067_02261b9c),
    HOST_OVERLAY_VAR(data_ov067_02261d88),
    HOST_OVERLAY_VAR(data_ov067_02261da0),
    HOST_OVERLAY_VAR(data_ov067_02261d40),
    HOST_OVERLAY_VAR(data_ov067_02261b5c),
    HOST_OVERLAY_VAR(data_ov067_02261dd0),
    HOST_OVERLAY_VAR(data_ov067_02261d70),
    HOST_OVERLAY_VAR(data_ov067_02261c94),
    HOST_OVERLAY_VAR(data_ov067_02261ca8),
    HOST_OVERLAY_VAR(data_ov067_02261ea8),
    HOST_OVERLAY_VAR(data_ov067_02261e8c),
    HOST_OVERLAY_VAR(data_ov067_02261c6c),
    HOST_OVERLAY_VAR(data_ov067_02261c80),
    HOST_OVERLAY_VAR(data_ov067_02261b2c),
    HOST_OVERLAY_VAR(data_ov067_02261ae0),
    HOST_OVERLAY_VAR(data_ov067_02261b1c),
    HOST_OVERLAY_VAR(data_ov067_02261be0),
    HOST_OVERLAY_VAR(data_ov067_02261bf4),
    HOST_OVERLAY_VAR(data_ov067_02261a80),
    HOST_OVERLAY_VAR(data_ov067_02261c44),
    HOST_OVERLAY_VAR(data_ov067_02261e1c),
    HOST_OVERLAY_VAR(data_ov067_02261e54),
    HOST_OVERLAY_VAR(data_ov067_02261ad4),
    HOST_OVERLAY_VAR(data_ov067_02261b6c),
    HOST_OVERLAY_VAR(data_ov067_02261abc),
    HOST_OVERLAY_VAR(data_ov067_02261bbc),
    HOST_OVERLAY_VAR(data_ov067_02261db8),
    HOST_OVERLAY_VAR(data_ov067_02261bac),
    HOST_OVERLAY_VAR(data_ov067_02261c58),
    HOST_OVERLAY_VAR(data_ov067_02261cf8),
    HOST_OVERLAY_VAR(data_ov067_02261ab0),
    HOST_OVERLAY_VAR(data_ov067_02261afc),
    HOST_OVERLAY_VAR(data_ov067_02261cbc),
    HOST_OVERLAY_VAR(data_ov067_02261ee8),
    HOST_OVERLAY_VAR(data_ov067_02261c30),
    HOST_OVERLAY_VAR(data_ov067_02261ec8),
    HOST_OVERLAY_VAR(data_ov067_02261b8c),
    HOST_OVERLAY_VAR(data_ov067_02261d28),
    HOST_OVERLAY_VAR(data_ov067_02261de8),
    HOST_OVERLAY_VAR(data_ov067_02261f08),
    HOST_OVERLAY_VAR(data_ov067_02261a98),
    HOST_OVERLAY_VAR(data_ov067_02261ce4),
    HOST_OVERLAY_VAR(data_ov067_02261a60),
    HOST_OVERLAY_VAR(data_ov067_02261aec),
    HOST_OVERLAY_VAR(data_ov067_02261d58),
    HOST_OVERLAY_VAR(data_ov067_02261c08),
    HOST_OVERLAY_VAR(data_ov067_02261e38),
    HOST_OVERLAY_VAR(sWmErrCodeNames),
    HOST_OVERLAY_VAR(sWlxState),
    HOST_OVERLAY_VARS_END
};
