// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "nitro/math.h"
#include "net/DwcNetChannel.h"
#include "net/DwcNetChannelTable.h"

#include "types64.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_02277140.h"
#include "proto/ov065/unk_ov065_02277974.h"

// ov065_039: DWC-like send/receive channel table (0x0227702c..0x022778b0)





extern "C" {
DwcNetChannelTable *sDwcNetChannels;
MATHRandContext32 sDwcNetRandState;

u32 DwcConn_GetAid(void *connection);
void *DwcConn_FindConnectionByAid(u32 aid);
void DwcCore_SetError(s32, s32);
s32 DwcCore_HasError(void);
s32 DwcConn_GetAidList(u8 **);
s32 DwcConn_GetMyAid(void);
s32 DwcConn_IsAidValid(s32);
s32 DwcConn_IsAidConnected(s32);
s32 gt2GetOutgoingBufferFreeSpace(void *connection);
s32 gt2GetConnectionState(void *connection);
s32 gt2Ping(void *connection);
void gt2Send(void *connection, void *message, s32 len, s32 reliable);
void DwcMatch_OnSyncPacket(u32 a, s32 b, u8 *c);
void MI_CpuCopy8(void *, void *, s32);
void MI_CpuFill8(void *, s32, s32);
void strncpy(void *, const void *, s32);
void OS_GetMacAddress(void *);
// (the GT2 callbacks below take the GT2 connection)
void DwcNet_OnReceive(void *a, void *b, s32 c, s32 d);
void DwcNet_OnPing(void *a, s32 b);
void DwcNet_RecvReliable(void *a, void *buf, s32 n);
void DwcNet_RecvUnreliable(void *a, void *buf, s32 n);

}

u32 DwcNet_Rand32(u32 n) {
    u32 hi;
    u32 nn = n;
    if (sDwcNetRandState.x == 0 && sDwcNetRandState.mul == 0 && sDwcNetRandState.add == 0) {
        u64 s;
        u64 t;
        OS_GetMacAddress(&s);
        t = OS_GetTick();
        s = ((s >> 24) & 0xffffff) | (t << 24);
        sDwcNetRandState.x = s;
        sDwcNetRandState.mul = 0x5d588b656c078965ULL;
        sDwcNetRandState.add = 0x269ec3;
    }
    {
        sDwcNetRandState.x = (u64)((s64)sDwcNetRandState.mul * (s64)sDwcNetRandState.x) + sDwcNetRandState.add;
        if (nn == 0) {
            return (u32)(sDwcNetRandState.x >> 32);
        } else {
            return (u32)(((u64)((s64)(sDwcNetRandState.x >> 32) * (s64)nn)) >> 32);
        }
    }
}

BOOL DwcNet_CanSendReliable(s32 id) {
    return DwcNet_CanSend(id, 1);
}

BOOL DwcNet_CanSend(s32 id, s32 m) {
    if ((m == 1 && DwcConn_IsAidValid(id) == 0) || DwcConn_IsAidConnected(id) == 0) {
        return FALSE;
    }
    if (DwcNet_IsSending(id) == 1) {
        return FALSE;
    }
    if (DwcNet_GetSendSpace(id) >= DwcNet_GetHeaderSize(m)) {
        return TRUE;
    }
    return FALSE;
}

BOOL DwcNet_SendReliable(s32 id, u8 *buf, s32 n) {
    return DwcNet_SendData(1, id, buf, n);
}

BOOL DwcNet_SendData(s32 m, s32 id, u8 *buf, s32 n) {
    DwcNetChannel *r = DwcNet_GetChannel(id);
    DwcNetFrameHeader h;
    s32 chunk;
    if (DwcCore_HasError() != 0) {
        return FALSE;
    }
    if (DwcNet_CanSend(id, m) == 0) {
        return FALSE;
    }
    r->isSending = 1;
    r->sendData = buf;
    r->sentBytes = 0;
    r->sendSize = n;
    DwcNet_BuildHeader(&h, m, n);
    DwcNet_SendToAid(id, &h, 8, 1);
    chunk = sDwcNetChannels->maxChunkSize;
    if (n <= chunk) {
        chunk = n;
    }
    if (chunk > DwcNet_GetSendSpace(id)) {
        return TRUE;
    }
    DwcNet_SendToAid(id, buf, chunk, 1);
    r->sentBytes = r->sentBytes + chunk;
    if (r->sentBytes == r->sendSize) {
        if (sDwcNetChannels->sendDoneCallback != NULL && m == 1) {
            sDwcNetChannels->sendDoneCallback(r->sendSize, id);
        }
        r->isSending = 0;
        r->sendData = NULL;
        r->sentBytes = 0;
        r->sendSize = 0;
    }
    return TRUE;
}

BOOL DwcNet_SetRecvBuffer(s32 id, u8 *buf, s32 n) {
    DwcNetChannel *r = DwcNet_GetChannel(id);
    if (DwcNet_GetRecvState(id) == 2) {
        return FALSE;
    }
    r->recvBuffer = buf;
    r->recvBufSize = n;
    r->recvState = 1;
    r->recvBytes = 0;
    r->recvSize = 0;
    return TRUE;
}

void DwcNet_PingAid(s32 id) {
    void *h = DwcConn_FindConnectionByAid(id);
    if (id != DwcConn_GetMyAid() && h != 0 && gt2GetConnectionState(h) == 1 && DwcCore_HasError() == 0) {
        gt2Ping(h);
    }
}

void DwcNet_SetSendDoneCallback(void *cb) {
    sDwcNetChannels->sendDoneCallback = (void (*)(u32, u32))cb;
}

void DwcNet_SetRecvCallback(void *cb) {
    sDwcNetChannels->recvCallback = (void (*)(u32, u8 *, u32))cb;
}

void DwcNet_SetPingCallback(void *cb) {
    sDwcNetChannels->pingCallback = (void (*)(u32, u32))cb;
}

void DwcNet_SetMaxChunkSize(u32 v) {
    if (v > 0x5b9) {
        v = 0x5b9;
    }
    sDwcNetChannels->maxChunkSize = v;
}

void DwcNet_BuildHeader(void *p, u32 a, u32 b) {
    DwcNetFrameHeader *h = (DwcNetFrameHeader *)p;
    strncpy(h->magic, "DT", 2);
    h->frameType = a;
    h->dataSize = b;
}

u32 DwcNet_ParseHeader(void *src) {
    DwcNetFrameHeader h;
    MI_CpuCopy8(src, &h, 8);
    if (memcmp(h.magic, "DT", 2) == 0) {
        return h.frameType;
    }
    return 0;
}

s32 DwcNet_GetHeaderSize(s32 m) {
    switch (m) {
    case 2:
    case 3:
    case 4:
        return 0xc;
    }
    return 8;
}

void DwcNet_InitChannelTable(void *p) {
    sDwcNetChannels = (DwcNetChannelTable *)p;
    MI_CpuFill8(p, 0, sizeof(DwcNetChannelTable));  // (0x614 on the DS)
    sDwcNetChannels->maxChunkSize = 0x5b9;
}

void DwcNet_OnReceive(void *a, void *b, s32 c, s32 d) {
    DwcNetChannelTable *g = sDwcNetChannels;
    if (g != NULL && b != NULL && c != 0) {
        if (d != 0) {
            DwcNet_RecvReliable(a, b, c);
        } else {
            DwcNet_RecvUnreliable(a, b, c);
        }
    }
}

void DwcNet_OnPing(void *a, s32 b) {
    if (sDwcNetChannels->pingCallback != NULL) {
        sDwcNetChannels->pingCallback(b, DwcConn_GetAid(a));
    }
}

void DwcNet_ProcessSend(void) {
    if (sDwcNetChannels != NULL) {
        u8 *list;
        s32 n = DwcConn_GetAidList(&list);
        s32 i = 0;
        if (n > 0) {
            s32 z0 = 0;
            s32 z1 = 0;
            do {
                s32 id = list[i];
                DwcNetChannel *r;
                if (id != DwcConn_GetMyAid()) {
                    if (DwcNet_IsSending(id) == 1) {
                        s32 rem;
                        s32 chunk;
                        r = DwcNet_GetChannel(id);
                        rem = r->sendSize - r->sentBytes;
                        chunk = sDwcNetChannels->maxChunkSize;
                        if (rem <= chunk) {
                            chunk = rem;
                        }
                        if (DwcNet_GetSendSpace(id) < chunk) {
                            goto next;
                        }
                        DwcNet_SendToAid(id, r->sendData + r->sentBytes, chunk, 1);
                        r->sentBytes = r->sentBytes + chunk;
                        if (r->sentBytes == r->sendSize) {
                            if (sDwcNetChannels->sendDoneCallback != NULL) {
                                sDwcNetChannels->sendDoneCallback(r->sendSize, id);
                            }
                            r->isSending = z0;
                            r->sendData = NULL;
                            r->sentBytes = z0;
                            r->sendSize = z0;
                        }
                    }
                }
                if (DwcConn_IsAidValid(id) != 0) {
                    r = DwcNet_GetChannel(id);
                    if (sDwcNetChannels->recvTimeoutCallback != NULL && r->timeoutMs != 0) {
                        u64 t = OS_GetTick();
                        u64 d = (t - *(u64 *)&r->lastRecvTick) << 6;
                        if ((u32)(d / 0x82ea) > r->timeoutMs) {
                            sDwcNetChannels->recvTimeoutCallback(id);
                            *(u64 *)&r->lastRecvTick = t;
                        }
                    }
                }
            next:
                i++;
            } while (i < n);
        }
    }
}

void DwcNet_ResetChannel(s32 id) {
    if (sDwcNetChannels != NULL) {
        s32 z = 0;
        sDwcNetChannels->channels[id].sentBytes = z;
        sDwcNetChannels->channels[id].recvBytes = z;
        sDwcNetChannels->channels[id].sendSize = z;
        sDwcNetChannels->channels[id].recvSize = z;
        sDwcNetChannels->channels[id].isSending = z;
        sDwcNetChannels->channels[id].recvType = z;
    }
}

void DwcNet_ClearChannelTable(void) {
    sDwcNetChannels = NULL;
}

DwcNetChannel *DwcNet_GetChannel(s32 id) {
    return &sDwcNetChannels->channels[id];
}

u32 DwcNet_IsSending(s32 id) {
    return sDwcNetChannels->channels[id].isSending;
}

u32 DwcNet_GetRecvState(s32 id) {
    return sDwcNetChannels->channels[id].recvState;
}

void DwcNet_SendToAid(s32 id, void *buf, s32 n, s32 f) {
    gt2Send(DwcConn_FindConnectionByAid(id), buf, n, f);
}

void DwcNet_RecvReliable(void *a, void *buf, s32 n) {
    s32 id = DwcConn_GetAid(a);
    switch (DwcNet_GetRecvState(id)) {
    case 0: {
        u32 t = DwcNet_ParseHeader(buf);
        if (t < 2 || t > 4) {
            return;
        }
        DwcNet_RecvHeader(id, buf, n);
        return;
    }
    case 1:
        DwcNet_RecvHeader(id, buf, n);
        return;
    case 2:
        DwcNet_RecvBodyChunk(id, buf, n);
        return;
    case 3:
        DwcNet_RecvControlBody(id, buf, n);
        return;
    case 4:
        sDwcNetChannels->channels[id].recvState = 1;
        sDwcNetChannels->channels[id].recvBytes = 0;
        sDwcNetChannels->channels[id].recvSize = 0;
        return;
    default:
        DwcCore_SetError(6, -0x17d4a);
        return;
    }
}

void DwcNet_RecvUnreliable(void *a, void *buf, s32 n) {
    s32 id = DwcConn_GetAid(a);
    DwcNetChannel *r = &sDwcNetChannels->channels[id];
    if (r->recvBuffer != NULL && r->recvBufSize >= n) {
        MI_CpuCopy8(buf, r->recvBuffer, n);
        if (sDwcNetChannels->recvCallback != NULL) {
            sDwcNetChannels->recvCallback(id, r->recvBuffer, n);
        }
        if (sDwcNetChannels->recvTimeoutCallback != NULL && r->timeoutMs != 0) {
            u64 t = OS_GetTick();
            r->lastRecvTick = (u32)t;
            r->lastRecvTickHi = (u32)(t >> 32);
        }
    }
}

void DwcNet_RecvHeader(s32 id, void *buf, s32 n) {
    DwcNetChannel *r = &sDwcNetChannels->channels[id];
    u32 t;
    DwcNetFrameHeader h;
    r->prevRecvState = DwcNet_GetRecvState(id);
    t = DwcNet_ParseHeader(buf);
    switch (t) {
    case 0:
        break;
    case 1:
        if (n != 8) {
            return;
        }
        MI_CpuCopy8(buf, &h, 8);
        r->recvSize = h.dataSize;
        r->recvBytes = 0;
        if (r->recvBuffer != NULL && r->recvBufSize >= r->recvSize) {
            r->recvState = 2;
        } else {
            r->recvState = 4;
        }
        break;
    case 2:
    case 3:
    case 4:
        r->recvState = 3;
        break;
    }
    r->recvType = t;
}

void DwcNet_RecvBodyChunk(s32 id, void *buf, s32 n) {
    DwcNetChannel *r = &sDwcNetChannels->channels[id];
    if (DwcNet_GetRecvState(id) == 2) {
        if (r->recvBytes + n > r->recvBufSize) {
            DwcCore_SetError(6, -0x17d54);
            return;
        }
        MI_CpuCopy8(buf, r->recvBuffer + r->recvBytes, n);
    }
    r->recvBytes = r->recvBytes + n;
    s32 sz = r->recvSize;
    if (r->recvBytes == sz) {
        r->recvState = 1;
        r->recvBytes = 0;
        r->recvSize = 0;
        if (sDwcNetChannels->recvCallback != NULL) {
            sDwcNetChannels->recvCallback(id, r->recvBuffer, sz);
        }
    }
    if (sDwcNetChannels->recvTimeoutCallback != NULL && r->timeoutMs != 0) {
        u64 t = OS_GetTick();
        r->lastRecvTick = (u32)t;
        r->lastRecvTickHi = (u32)(t >> 32);
    }
}

void DwcNet_RecvControlBody(s32 a, void *b, s32 c) {
    DwcNetChannel *r = DwcNet_GetChannel(a);
    r->recvState = r->prevRecvState;
    u32 t = r->recvType;
    switch (t) {
    case 2:
    case 3:
    case 4:
        DwcMatch_OnSyncPacket(a, t, (u8 *)b);
        break;
    }
}

s32 DwcNet_GetSendSpace(s32 id) {
    s32 t = gt2GetOutgoingBufferFreeSpace(DwcConn_FindConnectionByAid(id)) - 0x207;
    if (t <= 0) {
        t = 0;
    }
    return t;
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_02277140) {
    HOST_OVERLAY_VAR(sDwcNetChannels),
    HOST_OVERLAY_VAR(sDwcNetRandState),
    HOST_OVERLAY_VARS_END
};
