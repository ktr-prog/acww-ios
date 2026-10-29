// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/gpi.h"
#include "net/gpiProfile.h"
#include "net/gsPlatformUtil.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_0227c6f0.h"
#include "proto/ov065/unk_ov065_02283720.h"

// ov065_048: DWC HTTP/session context (0x0227c538..0x0227ce30)
extern "C" {



void gpiDisconnect(GPConnection *, s32);
void gpiSetErrorString(GPConnection *, const char *);
s32 gpiConnect(GPConnection *, const char *, const char *, const char *, const char *, const char *,
                        const char *, const char *, s32, s32, s32, GPCallback, void *);
s32 gpiCheckConnect(GPConnection *);
void msleep(s32);
s32 gpiFindOperationByID(GPConnection *, GPIOperation **, s32);
s32 gpiProcessCallbacks(GPConnection *, s32);
s32 gpiProcessPeers(GPConnection *);
s32 gpiProcessSearches(GPConnection *);
void gpiFailedOpCallback(GPConnection *, GPIOperation *);
void gpiRemoveOperation(GPConnection *, GPIOperation *);
void gpiAddLocalInfo(GPConnection *, GPIBuffer *);
s32 gpiSendFromBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32, const char *);
s32 gpiRecvToBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32 *, const char *);
void gpiSetError(GPConnection *, s32, const char *);
void gpiCallErrorCallback(GPConnection *, s32, s32);
s32 gpiCheckForError(GPConnection *, char *, s32);
s32 gpiProcessRecvBuddyMessage(GPConnection *, char *);
s32 gpiOperationsAreBlocking(GPConnection *);
s32 gpiProcessOperation(GPConnection *, GPIOperation *, char *);
void gpiProfileMap(GPConnection *, s32 (*)(GPConnection *, GPIProfile *, s32), s32);
s32 gpiGetProfile(GPConnection *, s32, GPIProfile **);
void gpiAppendStringToBuffer(GPConnection *, GPIBuffer *, const char *);
void gpiAppendIntToBuffer(GPConnection *, GPIBuffer *, s32);
s32 gpiCanFreeProfile(GPIProfile *);
void gpiRemoveProfile(GPConnection *, GPIProfile *);
s32 gpiInitProfiles(GPConnection *);
void SocketStartUp();
void TableFree(void *);

s32 strncmp(const char *, const char *, u32);

s32 gpiReset(GPConnection *h);
s32 gpiDestroy(GPConnection *h);
s32 gpiInitialize(GPConnection *h, s32 a, s32 b);
s32 gpiProcess(GPConnection *h, s32 a);
s32 gpiProcessConnectionManager(GPConnection *h);
s32 gpiResetProfile(GPConnection *h, GPIProfile *n, s32 m);
}

extern "C" {
s32 gpiInitialize(GPConnection *h, s32 a, s32 b) {
    GPIConnection *c;
    *h = 0;
    c = (GPIConnection *)GsUtil_Alloc(sizeof(GPIConnection));
    if (c == NULL) {
        return 1;
    }
    memset(c, 0, sizeof *c);  // (0x490 on the DS)
    c->errorString[0] = 0;
    c->errorCode = 0;
    c->infoCaching = 1;
    c->infoCachingBuddyOnly = 0;
    c->simulation = 0;
    c->firewall = 0;
    c->productID = a;
    c->namespaceID = b;
    if (gpiInitProfiles(&c) == 0) {
        GsUtil_Free(c);
        c = 0;
        return 1;
    }
    c->diskCache = 0;
    {
        s32 i;
        for (i = 0; i < 6; i++) {
            c->callbacks[i].callback = 0;
            c->callbacks[i].param = 0;
        }
    }
    c->unk_460 = 0;
    gpiDebug(&c, "\n\n\n\n\n*************\ngpiInitialize\n");
    {
        s32 r = gpiReset(&c);
        if (r != 0) {
            gpiDestroy(&c);
            return r;
        }
    }
    SocketStartUp();
    srand(current_time());
    *h = c;
    return 0;
}
}

extern "C" {
s32 gpiDestroy(GPConnection *h) {
    GPIConnection *c = *h;
    gpiDisconnect(h, 1);
    GsUtil_Free(c->unk_460);
    c->unk_460 = 0;
    TableFree(c->profileTable);
    GsUtil_Free(c);
    *h = 0;
    return 0;
}
}

extern "C" {
s32 gpiResetProfile(GPConnection *h, GPIProfile *n, s32 m) {
    n->buddyStatus = 0;
    n->authSig = 0;
    n->requestCount = 0;
    n->peerSig = 0;
    return 1;
}
}

extern "C" {
s32 gpiReset(GPConnection *h) {
    GPIConnection *c = *h;
    GPIOperation *n;
    c->nick[0] = 0;
    c->uniquenick[0] = 0;
    c->email[0] = 0;
    c->cmSocket = -1;
    c->connectState = 0;
    c->socketBuffer.len = 0;
    c->socketBuffer.pos = 0;
    c->socketBuffer.size = 0;
    GsUtil_Free(c->socketBuffer.buffer);
    c->socketBuffer.buffer = 0;
    c->socketBuffer.buffer = 0;
    c->inputBufferSize = 0;
    GsUtil_Free(c->inputBuffer);
    c->inputBuffer = 0;
    c->inputBuffer = 0;
    c->outputBuffer.len = 0;
    c->outputBuffer.pos = 0;
    c->outputBuffer.size = 0;
    GsUtil_Free(c->outputBuffer.buffer);
    c->outputBuffer.buffer = 0;
    c->outputBuffer.buffer = 0;
    c->updateproBuffer.len = 0;
    c->updateproBuffer.pos = 0;
    c->updateproBuffer.size = 0;
    GsUtil_Free(c->updateproBuffer.buffer);
    c->updateproBuffer.buffer = 0;
    c->updateproBuffer.buffer = 0;
    c->updateuiBuffer.len = 0;
    c->updateuiBuffer.pos = 0;
    c->updateuiBuffer.size = 0;
    GsUtil_Free(c->updateuiBuffer.buffer);
    c->updateuiBuffer.buffer = 0;
    c->updateuiBuffer.buffer = 0;
    c->peerSocket = -1;
    c->nextOperationID = 2;
    n = c->operationList;
    while (n != NULL) {
        gpiRemoveOperation(h, n);
        n = c->operationList;
    }
    c->operationList = 0;
    c->numBuddies = 0;
    gpiProfileMap(h, gpiResetProfile, 0);
    c->userid = 0;
    c->profileid = 0;
    c->sessKey = 0;
    c->numSearches = 0;
    c->fatalError = 0;
    c->peerList = 0;
    c->lastStatus = -1;
    c->lastStatusString[0] = 0;
    c->lastLocationString[0] = 0;
    return 0;
}
}

extern "C" {
s32 gpiProcessConnectionManager(GPConnection *h) {
    GPIOperation *rec;
    s32 len;
    s32 flag = 0;
    GPIConnection *c = *h;
    char *p;
    s32 r;
    for (;;) {
        gpiAddLocalInfo(h, &c->outputBuffer);
        r = gpiSendFromBuffer(h, c->cmSocket, &c->outputBuffer, &flag, 1, "CM");
        if (r != 0) {
            return r;
        }
        r = gpiRecvToBuffer(h, c->cmSocket, &c->socketBuffer, &len, &flag, "CM");
        if (r != 0) {
            if (r == 3) {
                gpiSetError(h, 5, "There was an error reading from the server.");
                gpiCallErrorCallback(h, 3, 1);
                return 3;
            }
            return r;
        }
        p = strstr(c->socketBuffer.buffer, "\\final\\");
        if (p != NULL) {
            do {
                *p = 0;
                gpiDebug(h, "CMD: %s\n", c->socketBuffer.buffer);
                len = p - c->socketBuffer.buffer;
                if (len > c->inputBufferSize) {
                    s32 n = len;
                    if (n < 0x800) {
                        n = 0x800;
                    }
                    c->inputBufferSize = c->inputBufferSize + n;
                    void *np = GsUtil_Realloc(c->inputBuffer, c->inputBufferSize + 1);
                    if (np == NULL) {
                        gpiSetErrorString(h, "Out of memory.");
                        return 1;
                    }
                    c->inputBuffer = (char *)np;
                }
                memcpy(c->inputBuffer, c->socketBuffer.buffer, len + 1);
                c->socketBuffer.len = c->socketBuffer.len - ((p + 7) - c->socketBuffer.buffer);
                memmove(c->socketBuffer.buffer, p + 7, c->socketBuffer.len + 1);
                char *q = c->inputBuffer;
                char *f = strstr(q, "\\id\\");
                if (f != NULL) {
                    s32 id = atol(f + 4);
                    if (gpiFindOperationByID(h, &rec, id) == 0) {
                        gpiDebug(h, "No matching operation found for id %d\n", id);
                    } else {
                        r = gpiProcessOperation(h, rec, c->inputBuffer);
                        if (r != 0) {
                            return r;
                        }
                    }
                } else {
                    if (gpiCheckForError(h, q, 1) != 0) {
                        return 4;
                    }
                    q = c->inputBuffer;
                    if (strncmp(q, "\\bm\\", 4) == 0) {
                        r = gpiProcessRecvBuddyMessage(h, q);
                        if (r != 0) {
                            return r;
                        }
                    } else if (strncmp(q, "\\ka\\", 10) != 0) {
                        gpiDebug(h, "Received an unrecognized, unsolicited message.\n");
                    }
                }
                p = strstr(c->socketBuffer.buffer, "\\final\\");
            } while (p != NULL);
        }
        if (flag != 0) {
            c->connectState = 4;
            gpiSetError(h, 7, "The server has closed the connection.");
            gpiCallErrorCallback(h, 3, 1);
            return 0;
        }
        r = gpiOperationsAreBlocking(h);
        if (r != 0) {
            msleep(10);
        }
        if (r == 0) {
            return 0;
        }
    }
}
}

extern "C" {
s32 gpiProcess(GPConnection *h, s32 arg) {
    GPIConnection *c = *h;
    s32 r = 0;
    GPIOperation *rec;
    if (c->connectState == 1) {
        s32 zero = 0;
        s32 k;
        do {
            r = gpiCheckConnect(h);
            if (r == 0 && arg != 0 && c->connectState == 1) {
                k = 1;
            } else {
                k = zero;
            }
            if (k != 0) {
                msleep(10);
            }
        } while (k != 0);
        if (r != 0) {
            if (gpiFindOperationByID(h, &rec, 1) != 0) {
                rec->result = 4;
            }
        }
    }
    if ((u32)(c->connectState - 2) <= 1) {
        if (r == 0) {
            r = gpiProcessConnectionManager(h);
        }
        if (r == 0) {
            r = gpiProcessPeers(h);
        }
    }
    if (r == 0) {
        r = gpiProcessSearches(h);
    }
    rec = c->operationList;
    if (rec != NULL) {
        do {
            if (rec->result != 0) {
                gpiFailedOpCallback(h, rec);
                GPIOperation *o = rec;
                rec = rec->pnext;
                gpiRemoveOperation(h, o);
            } else {
                rec = rec->pnext;
            }
        } while (rec != NULL);
    }
    {
        s32 t = gpiProcessCallbacks(h, arg);
        if (t == 0) {
            if (c->fatalError != 0) {
                gpiDisconnect(h, 0);
            }
            t = r;
        }
        return t;
    }
}
}
