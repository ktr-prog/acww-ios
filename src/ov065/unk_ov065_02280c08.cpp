// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/darray.h"
#include "net/gpiInfo.h"
#include "net/gpiPeer.h"
#include "net/gpiTransfer.h"
#include "net/SockAddrIn.h"
#include "net/gpiProfile.h"
#include "net/gpi.h"
#include "net/gsPlatformUtil.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_02280c08.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_0227d96c.h"
#include "proto/ov065/unk_ov065_0227df0c.h"
#include "proto/ov065/unk_ov065_0228176c.h"
#include "proto/ov065/unk_ov065_02283304.h"

// ov065 TU47: GP gpiPeer.c (0x02280c08..0x0228176c)

namespace Na {
// ov065_054: 0x022804b8..0x02280d70












extern "C" {
void gpiSetError(void *, s32, const char *);
s32 gpiProcessConnect(void *, void *, char *);
s32 gpiProcessGetInfo(void *, void *, char *);
s32 shutdown(s32, s32);
s32 closesocket(s32);
s32 OS_SPrintf(char *, const char *, ...);
void ArrayAppend(void *, void *);
s32 socket(s32, s32, s32);
s32 SetSockBlocking(s32, s32);
void GsGpPeer_SetSocketBuffers(s32);
s32 connect(s32, void *, s32);
s32 GOAGetLastError(s32);
void gpiCallErrorCallback(void *, s32, s32);

extern char data_ov065_0228d884[];
extern char data_ov065_0228d894[];
extern char data_ov065_0228d8dc[];


s32 gpiIsValidDate(s32 day, s32 mon, s32 year);






void gpiDestroyOperation(GPConnection *h, GPIOperation *n);




// byte-sized unsigned enum: an enum-typed zero is not constant-folded/shared with later zeros
#pragma enumsalwaysint off
enum Unk_ov065_02280a2c_Z { Unk_ov065_02280a2c_Z_0 = 0, Unk_ov065_02280a2c_Z_FF = 0xff };
#pragma enumsalwaysint reset










}
extern "C" {
s32 gpiPeerFinishTransferMessage(GPConnection *h, GPIPeer *n, char *str, s32 len);
s32 gpiPeerStartTransferMessage(GPConnection *h, GPIPeer *n, s32 a, GPITransferID *s);
s32 gpiPeerAddMessage(GPConnection *h, GPIPeer *n, s32 a, const char *b);
}
}

namespace Nb {
// ov065_055: friend/auth connection task list (0x02280e7c..0x0228176c)


extern "C" {

s32 strncmp(const char *, const char *, s32);
s32 OS_SPrintf(char *, const char *, ...);

DArrayImplementation *ArrayNew(s32, s32, void (*)(void *));
void ArrayDeleteAt(DArrayImplementation *, s32);
void *ArrayNth(DArrayImplementation *, s32);
s32 ArrayLength(DArrayImplementation *);
void ArrayFree(DArrayImplementation *);
void MD5Digest(char *, s32, char *);
s32 accept(s32, s32, s32);
s32 shutdown(s32, s32);
s32 closesocket(s32);
s32 CanReceiveOnSocket(s32 fd);
s32 GetSendBufferSize(s32);
s32 GetReceiveBufferSize(s32);
s32 SetSendBufferSize(s32, s32);
s32 SetReceiveBufferSize(s32, s32);
s32 SetSockBlocking(s32, s32);
char *goastrdup(const char *);
s32 gpiSendBuddyMessage(GPConnection *, s32, s32, const char *);
s32 gpiSendServerBuddyMessage(GPConnection *, s32, s32, const char *);
s32 gpiSendFromBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32, const char *);
s32 gpiRecvToBuffer(GPConnection *, s32, GPIBuffer *, s32 *, s32 *, const char *);
s32 gpiSendGetInfo(GPConnection *, s32, s32);
s32 gpiAddOperation(GPConnection *, s32, void *, GPIOperation **, s32, GPCallback, void *);
void gpiHandleTransferMessage(GPConnection *, GPIPeer *, s32, const char *);

s32 gpiCanFreeProfile(GPIProfile *);
s32 gpiProcessPeerInitiatingConnection(GPConnection *, GPIPeer *);
s32 gpiProcessPeerAcceptingConnection(GPConnection *, GPIPeer *);
s32 gpiProcessPeerConnected(GPConnection *, GPIPeer *);
s32 gpiPeerSendMessages(GPConnection *, GPIPeer *);
s32 gpiProcessPeer(GPConnection *, GPIPeer *);
void gpiRemovePeer(GPConnection *, GPIPeer *);
void gpiDestroyPeer(GPConnection *, GPIPeer *);
void GsGpPeer_SetSocketBuffers(s32);
void gpiFreeMessage(void *);
GPIPeer *gpiAddPeer(GPConnection *, s32, s32);















}
extern "C" {
s32 gpiPeerGetSig(GPConnection *h, GPIPeer *n);
GPIPeer *gpiAddPeer(GPConnection *h, s32 a, s32 b);
void gpiFreeMessage(void *p);
GPIPeer *gpiGetPeerByProfile(GPConnection *h, s32 id);
s32 gpiProcessPeers(GPConnection *h);
void GsGpPeer_SetSocketBuffers(s32 s);
void gpiRemovePeer(GPConnection *h, GPIPeer *n);
void gpiDestroyPeer(GPConnection *h, GPIPeer *n);
s32 gpiProcessPeer(GPConnection *h, GPIPeer *n);
s32 gpiProcessPeerConnected(GPConnection *h, GPIPeer *n);
s32 gpiPeerSendMessages(GPConnection *h, GPIPeer *n);
s32 gpiProcessPeerAcceptingConnection(GPConnection *h, GPIPeer *n);
s32 gpiProcessPeerInitiatingConnection(GPConnection *h, GPIPeer *n);
}
}

namespace Nb {
extern "C" {
s32 gpiProcessPeerInitiatingConnection(GPConnection *h, GPIPeer *n) {
    GPIConnection *c = *h;
    s32 out;
    s32 len;
    s32 flag;
    GPIProfile *e;
    s32 r;
    s32 keep;
    char *p;
    switch (n->state) {
    case 0x65:
        break;
    case 0x66:
        r = gpiPeerStartConnect(h, n);
        if (r != 0) {
            return r;
        }
        break;
    case 0x67:
        r = gpiCheckSocketConnect(h, n->sock, &out);
        if (r != 0) {
            return r;
        }
        if (out == 4) {
            gpiSetErrorString(h, "Error connecting to a peer.");
            return 3;
        } else if (out == 3) {
            keep = 1;
            if (gpiGetProfile(h, n->profile, &e) == 0) {
                gpiSetErrorString(h, "Error connecting to a peer.");
                return 3;
            }
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\auth\\");
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\pid\\");
            gpiAppendIntToBuffer(h, &n->outputBuffer, c->profileid);
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\nick\\");
            gpiAppendStringToBuffer(h, &n->outputBuffer, c->nick);
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\sig\\");
            gpiAppendStringToBuffer(h, &n->outputBuffer, e->peerSig);
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\final\\");
            {
                GPIPeer *m = c->peerList;
                while (m != NULL) {
                    if (m->profile == n->profile && m != n && m->state <= 0x67) {
                        keep = 0;
                    }
                    m = m->pnext;
                }
            }
            if (keep != 0) {
                GsUtil_Free(e->peerSig);
                e->peerSig = NULL;
                if (gpiCanFreeProfile(e) != 0) {
                    gpiRemoveProfile(h, e);
                }
            }
            n->state = 0x68;
        }
        break;
    case 0x68:
        r = gpiRecvToBuffer(h, n->sock, &n->inputBuffer, &len, &flag, "PR");
        if (r != 0) {
            return r;
        }
        p = strstr(n->inputBuffer.buffer, "\\final\\");
        if (p != NULL) {
            char *q;
            *p = 0;
            q = n->inputBuffer.buffer;
            if (strncmp(q, "\\anack\\", 7) == 0) {
                n->nackCount++;
                if (n->nackCount > 1) {
                    gpiSetErrorString(h, "Error getting buddy authorization.");
                    return 3;
                }
                r = gpiPeerGetSig(h, n);
                if (r != 0) {
                    return r;
                }
            } else if (strncmp(q, "\\aack\\", 6) != 0) {
                gpiSetErrorString(h, "Error parsing buddy message.");
                return 3;
            }
            n->state = 0x69;
            n->inputBuffer.len = 0;
        }
        break;
    }
    if (n->outputBuffer.len > 0) {
        r = gpiSendFromBuffer(h, n->sock, &n->outputBuffer, &flag, 1, "PR");
        if (flag != 0 || r != 0) {
            n->state = 0x6a;
        }
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiProcessPeerAcceptingConnection(GPConnection *h, GPIPeer *n) {
    GPIConnection *c = *h;
    s32 len;
    s32 flag;
    char b1[0x10];
    char b2[0x1f];
    char b3[0x21];
    char b5[0x21];
    char b4[0x100];
    char *p;
    char *q;
    s32 x;
    s32 r;
    r = gpiRecvToBuffer(h, n->sock, &n->inputBuffer, &len, &flag, "PR");
    if (r != 0) {
        return r;
    }
    if (flag != 0) {
        n->state = 0x6a;
        return 0;
    }
    p = strstr(n->inputBuffer.buffer, "\\final\\");
    if (p != NULL) {
        *p = 0;
        q = n->inputBuffer.buffer;
        if (strncmp(q, "\\auth\\", 6) == 0) {
            if (gpiValueForKey(q, "\\pid\\", b1, 0x10) == 0) {
                n->state = 0x6a;
                return 0;
            }
            x = atol(b1);
            if (gpiValueForKey(n->inputBuffer.buffer, "\\nick\\", b2, 0x1f) == 0) {
                n->state = 0x6a;
                return 0;
            }
            if (gpiValueForKey(n->inputBuffer.buffer, "\\sig\\", b3, 0x21) == 0) {
                n->state = 0x6a;
                return 0;
            }
            OS_SPrintf(b4, "%s%d%d", c->password, c->profileid, x);
            MD5Digest(b4, STD_GetStringLength(b4), b5);
            if (strcmp(b3, b5) != 0) {
                gpiAppendStringToBuffer(h, &n->outputBuffer, "\\anack\\");
                gpiAppendStringToBuffer(h, &n->outputBuffer, "\\final\\");
                n->state = 0x6a;
                return 0;
            }
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\aack\\");
            gpiAppendStringToBuffer(h, &n->outputBuffer, "\\final\\");
            n->state = 0x69;
            n->profile = x;
        } else {
            n->state = 0x6a;
            return 0;
        }
        n->inputBuffer.len = 0;
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiPeerSendMessages(GPConnection *h, GPIPeer *n) {
    s32 i;
    s32 flag;
    s32 r;
    if (n->outputBuffer.len != 0) {
        return 0;
    }
    if (ArrayLength(n->messages) != 0) {
        i = 0;
        do {
            GPIMessage *e = (GPIMessage *)ArrayNth(n->messages, i);
            r = gpiSendFromBuffer(h, n->sock, &e->buffer, &flag, i, "PR");
            if (flag != 0 || r != 0) {
                n->state = 0x6a;
                return 0;
            }
            if (e->buffer.pos != e->buffer.len) {
                break;
            }
            ArrayDeleteAt(n->messages, i);
        } while (ArrayLength(n->messages) != 0);
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiProcessPeerConnected(GPConnection *h, GPIPeer *n) {
    GPIConnection *c = *h;
    s32 len;
    s32 flag;
    GPICallbackCopy pr;
    char *v;
    s32 type;
    s32 ext;
    s32 r;
    if (n->outputBuffer.len != 0) {
        r = gpiSendFromBuffer(h, n->sock, &n->outputBuffer, &flag, 1, "PR");
        if (flag != 0 || r != 0) {
            n->state = 0x6a;
            return 0;
        }
    }
    if (n->outputBuffer.len == 0) {
        r = gpiPeerSendMessages(h, n);
        if (r != 0) {
            return r;
        }
        if (n->state == 0x6a) {
            return 0;
        }
    }
    r = gpiRecvToBuffer(h, n->sock, &n->inputBuffer, &len, &flag, "PR");
    if (r != 0) {
        n->state = 0x6a;
        return 0;
    }
    if (len > 0) {
        n->timeout = time(0) + 0x12c;
    }
    do {
        r = gpiReadMessageFromBuffer(h, &n->inputBuffer, &v, &type, &ext);
        if (r != 0) {
            return r;
        }
        if (v != 0) {
            switch (type) {
            case 1:
                pr = *(GPICallbackCopy *)&c->callbacks[3];
                if (pr.p.callback != 0) {
                    GPRecvBuddyMessageArg *m = (GPRecvBuddyMessageArg *)GsUtil_Alloc(sizeof(GPRecvBuddyMessageArg));
                    if (m == NULL) {
                        gpiSetErrorString(h, "Out of memory.");
                        return 1;
                    }
                    m->profile = n->profile;
                    m->message = goastrdup(v);
                    m->date = time(0);
                    r = gpiAddCallback(h, pr, m, 0, 2);
                    if (r != 0) {
                        return r;
                    }
                }
                break;
            case 0x66:
                gpiSendBuddyMessage(h, n->profile, 0x67, "1");
                break;
            case 0xc8:
            case 0xc9:
            case 0xca:
            case 0xcb:
            case 0xcc:
            case 0xcd:
            case 0xce:
            case 0xcf:
            case 0xd0:
                gpiHandleTransferMessage(h, n, type, n->inputBuffer.buffer);  // (the stripped handler reads the headers only)
                break;
            }
            gpiClipBufferToPosition(h, &n->inputBuffer);
        }
    } while (v != 0);
    if (flag != 0) {
        n->state = 0x6a;
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
// Not in the original binary: unreferenced weak function compiled right before gpiProcessPeerConnected, so that the literal
// "Out of memory." is pooled before "1" as in the original; removed by the dead-stripping link (see notes.txt).
__declspec(weak) void Unk_ov065_02281180_pool_order(void) {
    STD_GetStringLength("PR");
    STD_GetStringLength("Out of memory.");
}
}
}

namespace Nb {
extern "C" {
s32 gpiProcessPeer(GPConnection *h, GPIPeer *n) {
    s32 r = 0;
    if (n->state != 0x69) {
        if (n->initiated != 0) {
            r = gpiProcessPeerInitiatingConnection(h, n);
        } else {
            r = gpiProcessPeerAcceptingConnection(h, n);
        }
    }
    if (r == 0 && n->state == 0x69) {
        r = gpiProcessPeerConnected(h, n);
    }
    return r;
}
}
}

namespace Nb {
extern "C" {
void gpiDestroyPeer(GPConnection *h, GPIPeer *n) {
    shutdown(n->sock, 2);
    closesocket(n->sock);
    GsUtil_Free(n->inputBuffer.buffer);
    n->inputBuffer.buffer = NULL;
    GsUtil_Free(n->outputBuffer.buffer);
    n->outputBuffer.buffer = NULL;
    if (n->messages != NULL) {
        ArrayFree(n->messages);
        n->messages = NULL;
    }
    GsUtil_Free(n);
}
}
}

namespace Nb {
extern "C" {
void gpiRemovePeer(GPConnection *h, GPIPeer *n) {
    GPIConnection *c = *h;
    GPIPeer *p = c->peerList;
    if (p == n) {
        c->peerList = n->pnext;
    } else {
        GPIPeer *q = p->pnext;
        while (q != n) {
            if (q == NULL) {
                gpiDebug(h, "Tried to remove peer not in list.");
                return;
            }
            p = q;
            q = q->pnext;
        }
        p->pnext = n->pnext;
    }
    {
        s32 i = 0;
        while (ArrayLength(n->messages) != 0) {
            GPIMessage *e = (GPIMessage *)ArrayNth(n->messages, i);
            if (e->type < 0x64) {
                gpiSendServerBuddyMessage(h, n->profile, e->type, e->buffer.buffer + e->start);
            }
            ArrayDeleteAt(n->messages, i);
        }
    }
    gpiDestroyPeer(h, n);
}
}
}

namespace Nb {
extern "C" {
void GsGpPeer_SetSocketBuffers(s32 s) {
    SetReceiveBufferSize(s, 0x4000);
    SetReceiveBufferSize(s, 0x8000);
    SetReceiveBufferSize(s, 0x10000);
    SetReceiveBufferSize(s, 0x20000);
    SetReceiveBufferSize(s, 0x40000);
    SetSendBufferSize(s, 0x4000);
    SetSendBufferSize(s, 0x8000);
    SetSendBufferSize(s, 0x10000);
    GetReceiveBufferSize(s);
    GetSendBufferSize(s);
}
}
}

namespace Nb {
extern "C" {
s32 gpiProcessPeers(GPConnection *h) {
    GPIConnection *c = *h;
    GPIPeer *n;
    s32 s;
    if (c->peerSocket != -1 && CanReceiveOnSocket(c->peerSocket) != 0) {
        s = accept(c->peerSocket, 0, 0);
        if (s != -1) {
            n = gpiAddPeer(h, -1, 0);
            if (n != NULL) {
                n->state = 0x68;
                n->sock = s;
                SetSockBlocking(s, 0);
                GsGpPeer_SetSocketBuffers(n->sock);
            } else {
                closesocket(s);
            }
        }
    }
    {
        GPIPeer *m = c->peerList;
        s32 *z = 0;
        while (m != NULL) {
            GPIPeer *next = m->pnext;
            s32 r = gpiProcessPeer(h, m);
            if (m->state == 0x6a || r != 0 || time(z) > m->timeout) {
                gpiRemovePeer(h, m);
            }
            m = next;
        }
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
GPIPeer *gpiGetPeerByProfile(GPConnection *h, s32 id) {
    GPIConnection *c = *h;
    GPIPeer *n = c->peerList;
    while (n != NULL) {
        if (n->profile == id && n->state == 0x69) {
            return n;
        }
        n = n->pnext;
    }
    return NULL;
}
}
}

namespace Nb {
extern "C" {
void gpiFreeMessage(void *p) {
    GsUtil_Free(*(void **)p);
    *(void **)p = NULL;
}
}
}

namespace Nb {
extern "C" {
GPIPeer *gpiAddPeer(GPConnection *h, s32 a, s32 b) {
    GPIConnection *c = *h;
    GPIPeer *n = (GPIPeer *)GsUtil_Alloc(sizeof(GPIPeer));
    if (n == NULL) {
        return NULL;
    }
    memset(n, 0, sizeof *n);  // (0x40 on the DS)
    n->state = 0x64;
    n->initiated = b;
    n->sock = -1;
    n->profile = a;
    n->timeout = time(0) + 0x12c;
    n->pnext = c->peerList;
    n->messages = ArrayNew(sizeof(GPIMessage), 0, gpiFreeMessage);
    c->peerList = n;
    return n;
}
}
}

namespace Nb {
extern "C" {
s32 gpiPeerGetSig(GPConnection *h, GPIPeer *n) {
    GPIOperation *e;
    s32 r = gpiAddOperation(h, 2, 0, &e, 0, 0, 0);
    if (r != 0) {
        return r;
    }
    r = gpiSendGetInfo(h, n->profile, e->id);
    if (r != 0) {
        return r;
    }
    n->state = 0x65;
    return 0;
}
}
}

namespace Na {
extern "C" {
s32 gpiPeerStartConnect(GPConnection *h, GPIPeer *n) {
    SockAddrIn sa;
    GPIProfile *p;
    s32 e;
    if (gpiGetProfile(h, n->profile, &p) == 0) {
        gpiSetErrorString(h, "Error connecting to a peer.");
        return 3;
    }
    n->sock = socket(2, 1, 0);
    if (n->sock == -1) {
        gpiSetError(h, 5, "There was an error creating a socket.");
        gpiCallErrorCallback(h, 3, 0);
        return 3;
    }
    if (SetSockBlocking(n->sock, 0) == 0) {
        gpiSetError(h, 5, "There was an error making a socket non-blocking.");
        gpiCallErrorCallback(h, 3, 0);
        return 3;
    }
    GsGpPeer_SetSocketBuffers(n->sock);
    SockAddrIn *ad = &sa;
    ((u32 *)ad)[0] = 0;
    ((u32 *)ad)[1] = 0;
    sa.family = 2;
    sa.addr = p->buddyStatus->ip;
    sa.port = p->buddyStatus->port;
    if (connect(n->sock, ad, 8) == -1) {
        e = GOAGetLastError(n->sock);
        if (e != -6 && e != -0x1a && e != -0x4c) {
            gpiSetError(h, 5, "There was an error connecting a socket.");
            gpiCallErrorCallback(h, 3, 1);
            return 3;
        }
    }
    n->state = 0x67;
    return 0;
}
}
}

namespace Na {
extern "C" {
s32 gpiPeerAddMessage(GPConnection *h, GPIPeer *n, s32 a, const char *b) {
    s32 len = STD_GetStringLength(b);
    GPIMessage t = {{0, 0, 0, 0}, 0, 0};
    s32 r;
    t.type = a;
    r = gpiAppendStringToBuffer(h, &t.buffer, "\\m\\");
    if (r != 0) {
        return r;
    }
    r = gpiAppendIntToBuffer(h, &t.buffer, a);
    if (r != 0) {
        return r;
    }
    r = gpiAppendStringToBuffer(h, &t.buffer, "\\len\\");
    if (r != 0) {
        return r;
    }
    r = gpiAppendIntToBuffer(h, &t.buffer, len);
    if (r != 0) {
        return r;
    }
    r = gpiAppendStringToBuffer(h, &t.buffer, "\\msg\\\n");
    if (r != 0) {
        return r;
    }
    t.start = t.buffer.len;
    r = gpiAppendStringToBufferLen(h, &t.buffer, b, len);
    if (r != 0) {
        return r;
    }
    r = gpiAppendCharToBuffer(h, &t.buffer, 0);
    if (r != 0) {
        return r;
    }
    ArrayAppend((void *)n->messages, &t);
    n->timeout = time(0) + 0x12c;
    return 0;
}
}
}

namespace Na {
extern "C" {
s32 gpiPeerStartTransferMessage(GPConnection *h, GPIPeer *n, s32 a, GPITransferID *s) {
    char buf[0x44];
    OS_SPrintf(buf, "\\m\\%d\\xfer\\%d %u %u", a, s->profileid, s->count, s->time);
    return gpiSendOrBufferString(h, n, buf);
}
}
}

namespace Na {
extern "C" {
s32 gpiPeerFinishTransferMessage(GPConnection *h, GPIPeer *n, char *str, s32 len) {
    char buf[0x24];
    s32 r;
    if (str == 0) {
        str = "";
    }
    if (len == -1) {
        len = STD_GetStringLength(str);
    }
    OS_SPrintf(buf, "\\len\\%d\\msg\\\n", len);
    r = gpiSendOrBufferString(h, n, buf);
    if (r != 0) {
        return r;
    }
    r = gpiSendOrBufferStringLen(h, n, str, len);
    if (r != 0) {
        return r;
    }
    r = gpiSendOrBufferChar(h, n, 0);
    if (r != 0) {
        return r;
    }
    n->timeout = time(0) + 0x12c;
    return 0;
}
}
}
