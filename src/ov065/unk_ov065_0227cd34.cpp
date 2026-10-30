// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/gpiProfile.h"
#include "net/gpiOperation.h"
#include "net/gpi.h"
#include "net/gpiPeer.h"
#include "net/gp.h"
#include "net/gsPlatformUtil.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_0227cd34.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_0227d96c.h"
#include "proto/ov065/unk_ov065_0227df0c.h"
#include "proto/ov065/unk_ov065_02280740.h"
#include "proto/ov065/unk_ov065_02283304.h"
extern "C" {
int strncmp(const char *a, const char *b, u32 n);
}

namespace Nc {
// ov065_048: DWC HTTP/session context (0x0227c538..0x0227ce30)
extern "C" {



void gpiDisconnect(GPConnection *, s32);
s32 gpiConnect(GPConnection *, const char *, const char *, const char *, const char *, const char *,
                        const char *, const char *, s32, s32, s32, GPCallback, void *);
s32 gpiCheckConnect(GPConnection *);
void msleep(s32);
s32 gpiFindOperationByID(GPConnection *, GPIOperation **, s32);
s32 gpiProcessCallbacks(GPConnection *, s32);
s32 gpiProcessPeers(GPConnection *);
s32 gpiProcessSearches(GPConnection *);
void gpiAddLocalInfo(GPConnection *, GPIBuffer *);
s32 gpiSendFromBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32, const char *);
s32 gpiRecvToBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32 *, const char *);
void gpiCallErrorCallback(GPConnection *, s32, s32);
s32 gpiOperationsAreBlocking(GPConnection *);
typedef s32 (*gpiProfileMapFunc)(GPConnection *, GPIProfile *, void *);
s32 gpiProfileMap(GPConnection *, gpiProfileMapFunc, void *);
s32 gpiGetProfile(GPConnection *, s32, GPIProfile **);
s32 gpiCanFreeProfile(GPIProfile *);
void gpiRemoveProfile(GPConnection *, GPIProfile *);
s32 gpiInitProfiles(GPConnection *);
void SocketStartUp();
void TableFree(void *);


s32 gpiReset(GPConnection *h);
s32 gpiDestroy(GPConnection *h);
s32 gpiInitialize(GPConnection *h, s32 a, s32 b);
s32 gpiProcess(GPConnection *h, s32 a);
s32 gpiProcessConnectionManager(GPConnection *h);
s32 gpiResetProfile(GPConnection *h, GPIProfile *n, s32 m);
s32 gpiFixBuddyIndices(GPConnection *h, GPIProfile *n, void *data);
}
}

namespace Nd {
// ov065_049: DWC HTTP request setup (0x0227ce44..0x0227d8e0)
extern "C" {
s32 gpiGetProfile(GPConnection *, s32, GPIProfile **);
s32 gpiSendAuthBuddyRequest(GPConnection *, GPIProfile *);
s32 gpiCanFreeProfile(GPIProfile *);
void gpiRemoveProfile(GPConnection *, GPIProfile *);
GPIPeer *gpiGetPeerByProfile(GPConnection *, s32);
GPIPeer *gpiAddPeer(GPConnection *, s32, s32);
s32 gpiPeerGetSig(GPConnection *, GPIPeer *);
s32 gpiPeerStartConnect(void *, GPIPeer *);
s32 gpiPeerAddMessage(void *, GPIPeer *, s32, const char *);
s32 gpiValueForKey(const char *, const char *, char *, s32);
void gpiCallErrorCallback(GPConnection *, s32, s32);
char *goastrdup(const char *);
GPIProfile *gpiProfileListAdd(GPConnection *, s32);


s32 gpiSendServerBuddyMessage(GPConnection *h, s32 a, s32 b, const char *s);
s32 gpiSendBuddyMessage(GPConnection *h, s32 id, s32 b, const char *t);
}
}

namespace Nf {
// ov065_050: DWC HTTP socket send/recv + growable string buffer + callback list (0x0227d8e0..0x0227e1c8)














extern "C" {
s32 OS_SPrintf(char *, const char *, ...);
s32 gpiValueForKey(const char *, const char *, char *, s32);
s32 recv(s32, void *, s32, s32);
s32 send(s32, void *, s32, s32);
s32 GOAGetLastError(s32);
s32 ArrayLength(s32);
s32 shutdown(s32, s32);
s32 closesocket(s32);
s32 gpiDestroyPeer(void *, void *);
s32 gpiDisconnectCleanupProfile(void);

s32 gpiAppendStringToBufferLen(GPConnection *, GPIBuffer *, const char *, s32);
s32 gpiAppendCharToBuffer(GPConnection *, GPIBuffer *, char);
s32 gpiSendOrBufferStringLen(GPConnection *, GPIPeer *, const char *, s32);
s32 gpiSendFromBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32, const char *);
s32 gpiCallCallback(GPConnection *, GPICallbackData *);
void gpiCallErrorCallback(GPConnection *, s32, s32);
}
}

namespace Nf {
extern "C" {
s32 gpiSendAuthBuddyRequest(GPConnection *h, GPIProfile *a) {
    GPIConnection *c = *h;
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\authadd\\");
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sesskey\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, c->sessKey);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\fromprofileid\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, a->profileId);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sig\\");
    gpiAppendStringToBuffer(h, &c->outputBuffer, a->authSig);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\final\\");
    return 0;
}
}
}

namespace Nd {

#define SWAP32(x) ((((x) << 24) & 0xff000000) | ((((x) << 8) & 0xff0000) | ((((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00))))

static inline u16 Swap16(u16 x) {
    return (u16)(((x >> 8) & 0xff) | ((x << 8) & 0xff00));
}

#define ERR3() \
    do { \
        gpiSetError(h, 1, "Unexpected data was received from the server."); \
        gpiCallErrorCallback(h, 3, 1); \
        return 3; \
    } while (0)

#define ERR1() \
    do { \
        gpiSetErrorString(h, "Out of memory."); \
        return 1; \
    } while (0)
extern "C" {
s32 gpiProcessRecvBuddyMessage(GPConnection *h, const char *s) {
    GPIConnection *c = *h;
    s32 code;
    s32 v;
    s32 w;
    GPICallbackCopy p;
    GPICallbackCopy p4;
    GPICallbackCopy p3;
    GPICallbackCopy p2;
    char tmp[0x10];
    char buf[0x1000];
    char buf3[0x100];

    if (gpiValueForKey(s, "\\bm\\", buf, 0x1000) == 0) {
        ERR3();
    }
    code = atol(buf);
    if (gpiValueForKey(s, "\\f\\", buf, 0x1000) == 0) {
        ERR3();
    }
    v = atol(buf);
    if (gpiValueForKey(s, "\\date\\", buf, 0x1000) != 0) {
        w = atol(buf);
    } else {
        w = time(0);
    }
    switch (code) {
    case 1: {
        GPRecvBuddyMessageArg *r5;
        p = *(GPICallbackCopy *)&c->callbacks[3];
        if (p.p.callback == 0) {
            break;
        }
        r5 = (GPRecvBuddyMessageArg *)GsUtil_Alloc(sizeof(GPRecvBuddyMessageArg));
        if (r5 == NULL) {
            ERR1();
        }
        if (gpiValueForKey(s, "\\msg\\", buf, 0x1000) == 0) {
            ERR3();
        }
        r5->message = (char *)GsUtil_Alloc(STD_GetStringLength(buf) + 1);
        if (r5->message == NULL) {
            ERR1();
        }
        STD_CopyString(r5->message, buf);
        r5->profile = v;
        r5->date = w;
        {
            s32 r = gpiAddCallback(h, p, r5, 0, 2);
            if (r != 0) {
                return r;
            }
        }
        break;
    }
    case 2: {
        GPIProfile *n;
        char *t;
        n = gpiProfileListAdd(h, v);
        if (n == NULL) {
            ERR1();
        }
        if (gpiValueForKey(s, "\\msg\\", buf, 0x1000) == 0) {
            ERR3();
        }
        t = strstr(buf, "|signed|");
        if (t == NULL) {
            ERR3();
        }
        *t = 0;
        if (STD_GetStringLength(t + 8) != 0x20) {
            ERR3();
        }
        GsUtil_Free(n->authSig);
        n->authSig = 0;
        n->authSig = goastrdup(t + 8);
        n->requestCount = n->requestCount + 1;
        p2 = *(GPICallbackCopy *)&c->callbacks[1];
        if (p2.p.callback == 0) {
            break;
        }
        {
            GPRecvBuddyRequestArg *r5 = (GPRecvBuddyRequestArg *)GsUtil_Alloc(sizeof(GPRecvBuddyRequestArg));
            if (r5 == NULL) {
                ERR1();
            }
            strzcpy(r5->reason, buf, 0x401);
            r5->profile = v;
            r5->date = w;
            {
                s32 r = gpiAddCallback(h, p2, r5, 0, 6);
                if (r != 0) {
                    return r;
                }
            }
        }
        break;
    }
    case 100: {
        GPIProfile *n;
        GPIBuddyStatus *r5;
        n = gpiProfileListAdd(h, v);
        if (n == NULL) {
            ERR1();
        }
        if (n->buddyStatus == NULL) {
            u8 *q;
            u8 *k;
            n->buddyStatus = (GPIBuddyStatus *)GsUtil_Alloc(sizeof(GPIBuddyStatus));
            if (n->buddyStatus == NULL) {
                ERR1();
            }
            q = (u8 *)n->buddyStatus;
#ifdef HOST64
            // (the whole status, as in ghiNewPost: a pointer counted down to NULL is a loop without end for clang)
            (void)k;
            for (u32 j = 0; j < sizeof(GPIBuddyStatus); j++) q[j] = 0;
#else
            k = (u8 *)0x18;
            do {
                *q++ = 0;
                k--;
            } while (k != NULL);
#endif
            {
                s32 *cnt = &c->numBuddies;
                s32 o = *cnt;
                *cnt = o + 1;
                n->buddyStatus->buddyIndex = o;
            }
        }
        r5 = n->buddyStatus;
        if (gpiValueForKey(s, "\\msg\\", buf, 0x1000) == 0) {
            ERR3();
        }
        if (gpiValueForKey(buf, "|s|", tmp, 0x10) == 0) {
            ERR3();
        }
        r5->status = atol(tmp);
        GsUtil_Free(r5->statusString);
        r5->statusString = NULL;
        if (gpiValueForKey(buf, "|ss|", buf3, 0x100) == 0) {
            buf3[0] = 0;
        }
        r5->statusString = goastrdup(buf3);
        if (r5->statusString == NULL) {
            ERR1();
        }
        GsUtil_Free(r5->locationString);
        r5->locationString = NULL;
        if (gpiValueForKey(buf, "|ls|", buf3, 0x100) == 0) {
            buf3[0] = 0;
        }
        r5->locationString = goastrdup(buf3);
        if (r5->locationString == NULL) {
            ERR1();
        }
        if (gpiValueForKey(buf, "|ip|", tmp, 0x10) == 0) {
            r5->ip = 0;
        } else {
            r5->ip = SWAP32((u32)atol(tmp));
        }
        if (gpiValueForKey(buf, "|p|", tmp, 0x10) == 0) {
            r5->port = 0;
        } else {
            r5->port = Swap16(atol(tmp));
        }
        p3 = *(GPICallbackCopy *)&c->callbacks[2];
        if (p3.p.callback == 0) {
            break;
        }
        {
            GPRecvBuddyStatusArg *m = (GPRecvBuddyStatusArg *)GsUtil_Alloc(sizeof(GPRecvBuddyStatusArg));
            if (m == NULL) {
                ERR1();
            }
            m->profile = v;
            m->index = r5->buddyIndex;
            m->date = w;
            {
                s32 r = gpiAddCallback(h, p3, m, 0, 5);
                if (r != 0) {
                    return r;
                }
            }
        }
        break;
    }
    case 101: {
        char *t;
        char *t2;
        s32 q;
        GPRecvGameInviteArg *r5;
        if (gpiValueForKey(s, "\\msg\\", buf, 0x1000) == 0) {
            ERR3();
        }
        t = strstr(buf, "|p|");
        if (t == NULL) {
            ERR3();
        }
        if (t[3] == 0) {
            ERR3();
        }
        q = atol(t + 3);
        t2 = strstr(buf, "|l|");
        if (t2 != NULL) {
            strzcpy(buf3, t2 + 3, 0x100);
        } else {
            buf3[0] = 0;
        }
        p4 = *(GPICallbackCopy *)&c->callbacks[4];
        if (p4.p.callback == 0) {
            break;
        }
        r5 = (GPRecvGameInviteArg *)GsUtil_Alloc(sizeof(GPRecvGameInviteArg));
        if (r5 == NULL) {
            ERR1();
        }
        r5->profile = v;
        r5->productID = q;
        STD_CopyString(r5->location, buf3);
        {
            s32 r = gpiAddCallback(h, p4, r5, 0, 0);
            if (r != 0) {
                return r;
            }
        }
        break;
    }
    case 102:
        if (gpiValueForKey(s, "\\msg\\", buf, 0x1000) == 0) {
            ERR3();
        }
        gpiSendBuddyMessage(h, v, 0x67, "1");
        break;
    }
    return 0;
}
}
}

namespace Nd {
extern "C" {
s32 gpiSendServerBuddyMessage(GPConnection *h, s32 a, s32 b, const char *s) {
    GPIConnection *c = *h;
    char buf[0xdad];
    strzcpy(buf, s, 0xdad);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\bm\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, b);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sesskey\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, c->sessKey);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\t\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, a);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\msg\\");
    gpiAppendStringToBuffer(h, &c->outputBuffer, buf);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\final\\");
    return 0;
}
}
}

namespace Nd {
extern "C" {
s32 gpiSendBuddyMessage(GPConnection *h, s32 id, s32 b, const char *t) {
    GPIProfile *n;
    GPIPeer *r6;
    r6 = gpiGetPeerByProfile(h, id);
    if (r6 == 0) {
        if (!(gpiGetProfile(h, id, &n) != 0 && n->buddyStatus != NULL && n->buddyStatus->port != 0)) {
            return gpiSendServerBuddyMessage(h, id, b, t);
        }
        r6 = gpiAddPeer(h, id, 1);
        if (r6 == 0) {
            return 1;
        }
        if (n->peerSig == 0) {
            s32 q = gpiPeerGetSig(h, r6);
            if (q != 0) {
                return q;
            }
        } else {
            s32 q = gpiPeerStartConnect(h, r6);
            if (q != 0) {
                return q;
            }
        }
    }
    {
        s32 r = gpiPeerAddMessage(h, r6, b, t);
        if (r != 0) {
            return r;
        }
        return 0;
    }
}
}
}

namespace Nd {
extern "C" {
s32 gpiAuthBuddyRequest(GPConnection *h, s32 id) {
    GPIConnection *c = *h;
    GPIProfile *n;
    s32 r;
    if (gpiGetProfile(h, id, &n) == 0) {
        gpiSetErrorString(h, "Invalid profile.");
        return 2;
    }
    if (n->authSig == 0) {
        gpiSetErrorString(h, "Invalid profile.");
        return 2;
    }
    r = gpiSendAuthBuddyRequest(h, n);
    if (r != 0) {
        return r;
    }
    n->requestCount = n->requestCount - 1;
    if (c->infoCaching == 0) {
        if (n->requestCount <= 0) {
            GsUtil_Free(n->authSig);
            n->authSig = 0;
            if (gpiCanFreeProfile(n) != 0) {
                gpiRemoveProfile(h, n);
            }
        }
    }
    return 0;
}
}
}

namespace Nc {
extern "C" {
s32 gpiFixBuddyIndices(GPConnection *h, GPIProfile *n, void *data) { // a gpiProfileMapFunc: data -> the removed index
    s32 m = *(s32 *)data;
    GPIBuddyStatus *s = n->buddyStatus;
    if (s != NULL) {
        if (s->buddyIndex > m) {
            s->buddyIndex = s->buddyIndex - 1;
        }
    }
    return 1;
}
}
}

namespace Nc {
extern "C" {
s32 gpiDeleteBuddy(GPConnection *h, s32 x) {
    GPIConnection *c = *h;
    GPIProfile *n;
    if (gpiGetProfile(h, x, &n) == 0) {
        gpiSetErrorString(h, "Invalid profile.");
        return 2;
    }
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\delbuddy\\");
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sesskey\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, c->sessKey);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\delprofileid\\");
    gpiAppendIntToBuffer(h, &c->outputBuffer, n->profileId);
    gpiAppendStringToBuffer(h, &c->outputBuffer, "\\final\\");
    if (n->buddyStatus != NULL) {
        s32 r6 = n->buddyStatus->buddyIndex;
        GsUtil_Free(n->buddyStatus->statusString);
        n->buddyStatus->statusString = 0;
        GsUtil_Free(n->buddyStatus->locationString);
        n->buddyStatus->locationString = 0;
        GsUtil_Free(n->buddyStatus);
        n->buddyStatus = 0;
        if (gpiCanFreeProfile(n) != 0) {
            gpiRemoveProfile(h, n);
        }
        c->numBuddies = c->numBuddies - 1;
        gpiProfileMap(h, gpiFixBuddyIndices, &r6);
    }
    return 0;
}
}
}
