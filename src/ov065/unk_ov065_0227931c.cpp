// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/GsSocket.h"
#include "net/SockHostEnt.h"
#include "net/ghttpConnection.h"
#include "net/gsPlatformUtil.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/itcm/unk_01ffa4a0.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_0227931c.h"
#include "proto/ov065/unk_ov065_02278328.h"
#include "proto/ov065/unk_ov065_022789fc.h"

// ov065 TU37: ghttp (1): ghttpBuffer / connection table / ghttpMain (0x0227931c..0x0227a284)


extern "C" {
s32 ghiThrottleBufferSize = 125;
u32 ghiThrottleTimeDelay = 250;
GHIConnection **ghiConnections;
s32 ghiNextUniqueID;
s32 ghiNumConnections;
s32 ghiConnectionsLen;
char *ghiProxyAddress;
u32 ghiProxyPort;
s32 ghiReferenceCount;
}

namespace Ng {




extern s32 GSINitroErrno;
extern GsHostAddr data_ov065_02291094;
extern u8 data_0213a410[];

extern SockHostEnt localhost;
extern GsHostAddrList data_ov065_022910a8;
extern u8 data_ov065_0229107c[];

extern "C" {
void MI_CpuFill8(void *p, s32 v, s32 n);
s32 Sock_SendTo(s32 a, s32 b, s32 c, u32 d, void *sa);
s32 Sock_Send(s32 a, s32 b, s32 c, u32 d);
s32 Sock_RecvFrom(s32 a, s32 b, s32 c, u32 d, u8 *sa);
s32 Sock_Recv(s32 a, s32 b, s32 c, u32 d);
s32 Sock_Accept(s32 a, u8 *sa);
s32 Sock_Listen(s32 a);
s32 Sock_Connect(s32 a, void *sa);
s32 Sock_Bind(s32 a, void *sa);
s32 Sock_Shutdown(s32 a);
s32 Sock_Close(s32 a);
s32 Sock_Create(s32 a, s32 b);
s32 Sock_Poll(GsPollFd *arr, u32 n, s64 timeout);
s32 Sock_Fcntl(s32 a, s32 cmd, u32 flags);
u32 SockCore_GetHostIp();
s32 IpAddr_StoreBe32(u32 v, u32 *p);
void OS_Sleep(s32 ms);
u64 func_02132ef8(u64 a, u32 b, u32 c);
s32 OS_SPrintf(char *buf, const char *fmt, ...);

s32 CheckRcode(s32 a, s32 b);
s32 setsockopt(s32 a, s32 b, s32 c, const void *d, s32 e);
s32 getsockopt(s32 a, s32 b, s32 c, void *val, s32 *len);
s32 GSISocketSelect(s32 sock, s32 *rd, s32 *wr, s32 *ex);

}

extern "C" {
static inline u32 Unk_ov065_02278dfc_Ntohl(u32 x) {
    return ((x << 24) & 0xff000000) | (((x << 8) & 0xff0000) | (((x >> 24) & 0xff) | ((x >> 8) & 0xff00)));
}
}
extern "C" {
}
}

namespace Nc {

typedef s32 (*GsHttpDecryptFn)(GHIConnection *, void *, u8 *, s32 *, u8 *, s32 *);
typedef void (*GsHttpEncryptCleanupFn)(GHIConnection *, void *);


extern "C" {
extern s32 ghiConnectionsLen;
extern s32 ghiNumConnections;
extern s32 ghiNextUniqueID;
extern u32 ghiThrottleTimeDelay;
extern s32 ghiThrottleBufferSize;

void ghiPostCleanupState(void *);
BOOL ghiIsPostAutoFree(void *);
void ghiFreePost(void *);

void ghiUnlock();
void ghiLock();
BOOL ghiFreeConnection(GHIConnection *);
s32 ghiFindFreeSlot();
BOOL ghiDecryptReceivedData(GHIConnection *);
void ghiEnumConnections(BOOL (*)(GHIConnection *));
}


extern "C" {
}
extern "C" {
void ghiCallPostCallback(GHIConnection *self);
void ghiCallProgressCallback(GHIConnection *self, char *p1, u32 p2);
void ghiCallCompletedCallback(GHIConnection *self);
s32 ghiDoReceive(GHIConnection *self, char *buf, s32 *plen);
BOOL ghiDecryptReceivedData(GHIConnection *self);
void ghiCleanupConnections();
void ghiRedirectConnection(GHIConnection *self);
void ghiEnumConnections(BOOL (*cb)(GHIConnection *));
BOOL ghiFreeConnection(GHIConnection *s);
GHIConnection *ghiNewConnection();
s32 ghiFindFreeSlot();
void ghiUnlock();
void ghiLock();
void ghiFreeLock();
void ghiCreateLock();
}
}

namespace Nm {





extern "C" {
extern s32 ghiReferenceCount;
extern char *ghiProxyAddress;
extern u32 ghiThrottleTimeDelay;
extern s32 ghiThrottleBufferSize;

s32 ghiPostAddString(void *, const char *, const char *);
GHIPost *ghiNewPost();
void ghiEnumConnections(s32 (*)(GHIConnection *));
char *goastrdup(const char *);
GHIConnection *ghiNewConnection();
BOOL ghiFreeConnection(GHIConnection *);
BOOL ghiPostInitState(GHIConnection *);
void msleep(s32);
void ghiDoHostLookup(GHIConnection *);
void ghiDoConnecting(GHIConnection *);
void ghiDoSecuringSession(GHIConnection *);
void ghiDoSendingRequest(GHIConnection *);
void ghiDoPosting(GHIConnection *);
void ghiDoWaiting(GHIConnection *);
void ghiDoReceivingStatus(GHIConnection *);
void ghiDoReceivingHeaders(GHIConnection *);
void ghiDoReceivingFile(GHIConnection *);
void ghiRedirectConnection(GHIConnection *);
void ghiCallCompletedCallback(GHIConnection *);
void ghiUnlock();
void ghiLock();
void ghiFreeLock();
void ghiCreateLock();
void ghiCleanupConnections();
s32 ghiSendBufferedData(GHIConnection *);
s32 OS_SPrintf(char *, const char *, ...);
s32 fread(void *, s32, s32, u32);

s32 ghiProcessConnection(GHIConnection *);
void ghiHandleStatus(GHIConnection *);
s32 ghiPostStateDoPosting(GHIPostState *, GHIConnection *, s32);
s32 ghiPostFileMemoryStateDoPosting(GHIPostState *, GHIConnection *);
s32 ghiPostFileDiskStateDoPosting(GHIPostState *, GHIConnection *);
s32 ghiPostStringStateDoPosting(GHIPostState *, GHIConnection *);
void ghttpStartup();
}
extern "C" {
s32 ghttpPostAddStringA(GHIPost *a, const char *b, const char *c);
GHIPost *ghttpNewPost();
void ghttpThink();
s32 ghttpPostExA(const char *a, const char *b, GHIPost *c, u32 d, s32 e, ghttpProgressCallback f, ghttpCompletedCallback g, void *h);
s32 ghttpPostA(const char *a, GHIPost *b, s32 c, ghttpCompletedCallback d, void *e);
s32 ghttpGetExA(const char *a, const char *b, char *c, s32 d, GHIPost *e, u32 f, s32 g, ghttpProgressCallback h, ghttpCompletedCallback i, void *j);
s32 ghttpGetA(const char *a, s32 b, ghttpCompletedCallback c, void *d);
void ghttpCleanup();
void ghttpStartup();
}
}

namespace Nm {
extern "C" {
void ghttpStartup() {
    ghiLock();
    if (++ghiReferenceCount == 1) {
        ghiCreateLock();
        ghiThrottleBufferSize = 0x7d;
        ghiThrottleTimeDelay = 0xfa;
    } else {
        ghiUnlock();
    }
}
}
}

namespace Nm {
extern "C" {
void ghttpCleanup() {
    ghiLock();
    if (--ghiReferenceCount == 0) {
        ghiCleanupConnections();
        if (ghiProxyAddress != 0) {
            GsUtil_Free(ghiProxyAddress);
            ghiProxyAddress = 0;
        }
        ghiUnlock();
        ghiFreeLock();
    } else {
        ghiUnlock();
    }
}
}
}

namespace Nm {
extern "C" {
s32 ghttpGetA(const char *a, s32 b, ghttpCompletedCallback c, void *d) {
    return ghttpGetExA(a, 0, 0, 0, 0, 0, b, 0, c, d);
}
}
}

namespace Nm {
extern "C" {
s32 ghttpGetExA(const char *a, const char *b, char *c, s32 d, GHIPost *e, u32 f, s32 g, ghttpProgressCallback h, ghttpCompletedCallback i, void *j) {
    GHIConnection *conn;
    if (a == 0 || *a == 0) {
        return -1;
    }
    if (d < 0) {
        return -1;
    }
    if (c != 0 && d == 0) {
        return -1;
    }
    if (ghiReferenceCount == 0) {
        ghttpStartup();
    }
    conn = ghiNewConnection();
    if (conn == 0) {
        return -1;
    }
    conn->type = 0;
    conn->URL = goastrdup(a);
    if (conn->URL == 0) {
        ghiFreeConnection(conn);
        return -1;
    }
    if (b != 0 && *b != 0) {
        conn->sendHeaders = goastrdup(b);
        if (conn->sendHeaders == 0) {
            ghiFreeConnection(conn);
            return -1;
        }
    }
    conn->post = e;
    conn->blocking = g;
    conn->progressCallback = h;
    conn->completedCallback = i;
    conn->callbackParam = j;
    conn->throttle = f;
    conn->userBufferSupplied = (c != 0) ? 1 : 0;
    BOOL ok;
    if (conn->userBufferSupplied != 0) {
        ok = ghiInitFixedBuffer(conn, &conn->getFileBuffer, c, d);
    } else {
        ok = ghiInitBuffer(conn, &conn->getFileBuffer, 0x800, 0x800);
    }
    if (ok == 0) {
        ghiFreeConnection(conn);
        return -1;
    }
    if (e != 0) {
        if (ghiPostInitState(conn) == 0) {
            ghiFreeConnection(conn);
            return -1;
        }
    }
    if (g != 0) {
        if (ghiProcessConnection(conn) == 0) {
            s32 t = 10;
            do {
                msleep(t);
            } while (ghiProcessConnection(conn) == 0);
        }
        return 0;
    }
    return conn->request;
}
}
}

namespace Nm {
extern "C" {
s32 ghttpPostA(const char *a, GHIPost *b, s32 c, ghttpCompletedCallback d, void *e) {
    return ghttpPostExA(a, 0, b, 0, c, 0, d, e);
}
}
}

namespace Nm {
extern "C" {
s32 ghttpPostExA(const char *a, const char *b, GHIPost *c, u32 d, s32 e, ghttpProgressCallback f, ghttpCompletedCallback g, void *h) {
    GHIConnection *conn;
    if (a == 0 || *a == 0) {
        return -1;
    }
    if (c == 0) {
        return -1;
    }
    if (ghiReferenceCount == 0) {
        ghttpStartup();
    }
    conn = ghiNewConnection();
    if (conn == 0) {
        return -1;
    }
    conn->type = 4;
    conn->URL = goastrdup(a);
    if (conn->URL == 0) {
        ghiFreeConnection(conn);
        return -1;
    }
    if (b != 0 && *b != 0) {
        conn->sendHeaders = goastrdup(b);
        if (conn->sendHeaders == 0) {
            ghiFreeConnection(conn);
            return -1;
        }
    }
    conn->post = c;
    conn->blocking = e;
    conn->progressCallback = f;
    conn->completedCallback = g;
    conn->callbackParam = h;
    conn->throttle = d;
    if (c != 0) {
        if (ghiPostInitState(conn) == 0) {
            ghiFreeConnection(conn);
            return -1;
        }
    }
    if (e != 0) {
        if (ghiProcessConnection(conn) == 0) {
            s32 t = 10;
            do {
                msleep(t);
            } while (ghiProcessConnection(conn) == 0);
        }
        return 0;
    }
    return conn->request;
}
}
}

namespace Nm {
extern "C" {
void ghttpThink() {
    ghiEnumConnections(ghiProcessConnection);
}
}
}

namespace Nm {
extern "C" {
GHIPost *ghttpNewPost() {
    return ghiNewPost();
}
}
}

namespace Nm {
extern "C" {
s32 ghttpPostAddStringA(GHIPost *a, const char *b, const char *c) {
    if (a == 0) {
        return 0;
    }
    if (b == 0 || *b == 0) {
        return 0;
    }
    if (c == 0) {
        c = "";
    }
    return ghiPostAddString(a, b, c);
}
}
}

namespace Nc {
extern "C" {
s32 ghiFindFreeSlot() {
    s32 i = 0;
    s32 base;
    s32 end;
    for (i = 0; i < ghiConnectionsLen; i++) {
        if (ghiConnections[i]->inUse == 0) {
            return i;
        }
    }
    base = ghiConnectionsLen;
    end = base + 4;
    void *p = GsUtil_Realloc(ghiConnections, end * sizeof(GHIConnection *));
    if (p == 0) {
        return -1;
    }
    ghiConnections = (GHIConnection **)p;
    i = base;
    for (; i < end; i++) {
        ghiConnections[i] = (GHIConnection *)GsUtil_Alloc(sizeof(GHIConnection));
        if (ghiConnections[i] == 0) {
            for (i--; i >= base; i--) {
                GsUtil_Free(ghiConnections[i]);
            }
            return -1;
        }
        ghiConnections[i]->inUse = 0;
    }
    ghiConnectionsLen = end;
    return base;
}
}
}

namespace Nc {
extern "C" {
GHIConnection *ghiNewConnection() {
    GHIConnection *s;
    s32 idx;
    BOOL r;
    ghiLock();
    idx = ghiFindFreeSlot();
    if (idx == -1) {
        ghiUnlock();
        return 0;
    }
    s = ghiConnections[idx];
    memset(s, 0, sizeof *s);  // (0x184 on the DS)
    s->inUse = 1;
    s->request = idx;
    s->uniqueID = ghiNextUniqueID++;
    s->type = 0;
    s->state = 0;
    s->URL = 0;
    s->serverAddress = 0;
    s->serverIP = 0;
    s->serverPort = 0;
    s->requestPath = 0;
    s->sendHeaders = 0;
    s->saveFile = 0;
    s->blocking = 0;
    s->persistConnection = 0;
    s->result = 0;
    s->progressCallback = 0;
    s->completedCallback = 0;
    s->callbackParam = 0;
    s->socket = -1;
    s->socketError = 0;
    s->userBufferSupplied = 0;
    s->statusMajorVersion = 0;
    s->statusMinorVersion = 0;
    s->statusCode = 0;
    s->statusStringIndex = 0;
    s->headerStringIndex = 0;
    s->headersEnd = 0;
    s->completed = 0;
    s->fileBytesReceived = 0;
    s->totalSize = -1;
    s->redirectURL = 0;
    s->redirectCount = 0;
    s->chunkedTransfer = 0;
    s->processing = 0;
    s->throttle = 0;
    s->lastThrottleRecv = 0;
    s->post = 0;
    s->maxRecvTime = 0x1f4;
    s->proxyOverridePort = 0x50;
    s->proxyOverrideServer = 0;
    s->encryptor = 0;
    r = ghiInitBuffer(s, &s->sendBuffer, 0x800, 0x1000);
    if (r != 0) {
        r = ghiInitBuffer(s, &s->recvBuffer, 0x800, 0x800);
    }
    if (r != 0) {
        r = ghiInitBuffer(s, &s->decodeBuffer, 0x800, 0x400);
    }
    if (r == 0) {
        ghiFreeConnection(s);
        ghiUnlock();
        return 0;
    }
    ghiNumConnections++;
    ghiUnlock();
    return s;
}
}
}

namespace Nc {
extern "C" {
BOOL ghiFreeConnection(GHIConnection *s) {
    if (s == 0) {
        return FALSE;
    }
    if (s->inUse == 0) {
        return FALSE;
    }
    if (s->request < 0) {
        return FALSE;
    }
    if (s->request >= ghiConnectionsLen) {
        return FALSE;
    }
    ghiLock();
    GsUtil_Free(s->URL);
    GsUtil_Free(s->serverAddress);
    GsUtil_Free(s->requestPath);
    GsUtil_Free(s->sendHeaders);
    GsUtil_Free(s->redirectURL);
    GsUtil_Free(s->proxyOverrideServer);
    if (s->socket != -1) {
        shutdown(s->socket, 2);
        closesocket(s->socket);
    }
    ghiFreeBuffer(&s->sendBuffer);
    ghiFreeBuffer(&s->recvBuffer);
    ghiFreeBuffer(&s->decodeBuffer);
    ghiFreeBuffer(&s->getFileBuffer);
    if (s->postingState.states != 0) {
        ghiPostCleanupState(s);
    }
    if (s->post != 0) {
        if (ghiIsPostAutoFree(s->post) != 0) {
            ghiFreePost(s->post);
            s->post = 0;
        }
    }
    if (s->encryptInitialized != 0) {
        if (s->encryptCleanupFn != 0) {
            s->encryptCleanupFn(s, &s->encryptor);
        }
        s->encryptInitialized = 0;
    }
    s->inUse = 0;
    ghiNumConnections--;
    ghiUnlock();
    return TRUE;
}
}
}

namespace Nc {
extern "C" {
void ghiEnumConnections(BOOL (*cb)(GHIConnection *)) {
    if (ghiNumConnections > 0) {
        s32 i;
        ghiLock();
        for (i = 0; i < ghiConnectionsLen; i++) {
            GHIConnection *s = ghiConnections[i];
            if (s->inUse != 0) {
                cb(s);
            }
        }
        ghiUnlock();
    }
}
}
}

namespace Nc {
extern "C" {
void ghiRedirectConnection(GHIConnection *self) {
    self->state = 0;
    GsUtil_Free(self->URL);
    self->URL = self->redirectURL;
    self->redirectURL = 0;
    GsUtil_Free(self->serverAddress);
    self->serverAddress = 0;
    self->serverIP = 0;
    self->serverPort = 0;
    GsUtil_Free(self->requestPath);
    self->requestPath = 0;
    shutdown(self->socket, 2);
    closesocket(self->socket);
    self->socket = -1;
    ghiResetBuffer(&self->sendBuffer);
    ghiResetBuffer(&self->recvBuffer);
    ghiResetBuffer(&self->decodeBuffer);
    self->statusMajorVersion = 0;
    self->statusMinorVersion = 0;
    self->statusCode = 0;
    self->statusStringIndex = 0;
    self->headerStringIndex = 0;
    self->headersEnd = 0;
    self->connectionClosed = 0;
    self->redirectCount++;
}
}
}

namespace Nc {
extern "C" {
void ghiCleanupConnections() {
    if (ghiConnections != 0) {
        s32 i;
        ghiEnumConnections(ghiFreeConnection);
        for (i = 0; i < ghiConnectionsLen; i++) {
            GsUtil_Free(ghiConnections[i]);
        }
        GsUtil_Free(ghiConnections);
        ghiConnections = 0;
        ghiConnectionsLen = 0;
        ghiNumConnections = 0;
    }
}
}
}

namespace Nc {
extern "C" {
void ghiCreateLock() {
}
}
}

namespace Nc {
extern "C" {
void ghiFreeLock() {
}
}
}

namespace Nc {
extern "C" {
void ghiLock() {
}
}
}

namespace Nc {
extern "C" {
void ghiUnlock() {
}
}
}

namespace Nc {
extern "C" {
BOOL ghiDecryptReceivedData(GHIConnection *self) {
    s32 inl = 0;
    s32 outl = 0;
    s32 r;
    do {
        s32 pos = self->decodeBuffer.pos;
        u8 *in = (u8 *)self->decodeBuffer.data + pos;
        inl = self->decodeBuffer.len - pos;
        s32 w = self->recvBuffer.len;
        u8 *out = (u8 *)self->recvBuffer.data + w;
        outl = self->recvBuffer.size - w;
        r = self->decryptFn(self, &self->encryptor, in, &inl, out, &outl);
        if (r == 2 && ghiResizeBuffer(&self->recvBuffer, self->recvBuffer.sizeIncrement) == 0) {
            return FALSE;
        }
    } while (r == 2 && outl == 0);
    self->decodeBuffer.pos += inl;
    self->recvBuffer.len += outl;
    if (self->decodeBuffer.pos > 0xff) {
        s32 rest = self->decodeBuffer.len - self->decodeBuffer.pos;
        if (rest == 0) {
            ghiResetBuffer(&self->decodeBuffer);
        } else {
            memmove(self->decodeBuffer.data, self->decodeBuffer.data + self->decodeBuffer.pos, rest);
            self->decodeBuffer.pos = 0;
            self->decodeBuffer.len = rest;
        }
    }
    if (r == 3) {
        self->completed = 1;
        self->result = 0x11;
        return FALSE;
    }
    return TRUE;
}
}
}

namespace Nc {
extern "C" {
s32 ghiDoReceive(GHIConnection *self, char *buf, s32 *plen) {
    s32 len;
    s32 n = *plen - 1;
    if (self->throttle != 0) {
        u32 t = current_time();
        if (t < self->lastThrottleRecv + ghiThrottleTimeDelay) {
            return 1;
        }
        self->lastThrottleRecv = t;
        if (n >= ghiThrottleBufferSize) {
            n = ghiThrottleBufferSize;
        }
    }
    if (self->recvBuffer.pos < self->recvBuffer.len) {
        ghiReadDataFromBuffer(&self->recvBuffer, buf, plen);
        if (self->recvBuffer.pos == self->recvBuffer.len) {
            self->recvBuffer.len = self->headersEnd;
            self->recvBuffer.pos = self->headersEnd;
        }
        return 0;
    }
    len = recv(self->socket, buf, n, 0);
    if (len == -1) {
        s32 e = GOAGetLastError(self->socket);
        if (e == -6 || e == -26 || e == -76) {
            return 1;
        }
        self->completed = 1;
        self->result = 5;
        self->socketError = e;
        self->connectionClosed = 1;
        return 3;
    }
    if (len == 0) {
        self->connectionClosed = 1;
        return 2;
    }
    if (self->encryptEnabled != 0) {
        if (ghiAppendDataToBuffer(&self->decodeBuffer, buf, len) == 0) {
            return 3;
        }
        if (ghiDecryptReceivedData(self) == 0) {
            self->completed = 1;
            self->result = 0x11;
            return 3;
        }
        if (self->recvBuffer.len - self->recvBuffer.pos <= 0) {
            buf[0] = 0;
            *plen = 0;
            return 1;
        }
        len = *plen - 1;
        if (ghiReadDataFromBuffer(&self->recvBuffer, buf, &len) == 0) {
            return 3;
        }
        if (self->recvBuffer.pos == self->recvBuffer.len) {
            self->recvBuffer.len = self->headersEnd;
            self->recvBuffer.pos = self->headersEnd;
        }
        if (len <= 0) {
            return 1;
        }
    }
    s32 r = 0;
    buf[len] = 0;
    *plen = len;
    if (len <= 0) {
        r = 1;
    }
    return r;
}
}
}

namespace Nc {
extern "C" {
s32 ghiDoSend(GHIConnection *self, const char *buf, s32 len) {
    s32 r = send(self->socket, buf, len, 0);
    if (r == -1) {
        s32 e = GOAGetLastError(self->socket);
        if (e == -6 || e == -26 || e == -76) {
            return 0;
        }
        self->completed = 1;
        self->result = 5;
        self->socketError = e;
        return -1;
    }
    if (self->state == 4) {
        self->postingState.bytesPosted += r;
    }
    return r;
}
}
}

namespace Nc {
extern "C" {
s32 ghiTrySendThenBuffer(GHIConnection *self, const char *buf, s32 len) {
    s32 r = 0;
    if (self->sendBuffer.len == 0) {
        r = ghiDoSend(self, buf, len);
        if (r == -1) {
            return 0;
        }
        if (r == len) {
            return 1;
        }
    }
    if (ghiAppendDataToBuffer(&self->sendBuffer, buf + r, len - r) == 0) {
        return 0;
    }
    return 2;
}
}
}

namespace Nc {
extern "C" {
void ghiCallCompletedCallback(GHIConnection *self) {
    if (self->completedCallback != 0) {
        char *a;
        u32 b;
        if (self->type != 0) {
            a = 0;
            b = 0;
        } else {
            a = self->getFileBuffer.data;
            b = self->fileBytesReceived;
        }
        s32 r = self->completedCallback(self->request, self->result, a, b, self->callbackParam);
        if (a != 0 && r == 0) {
            self->getFileBuffer.dontFree = 1;
        }
    }
}
}
}

namespace Nc {
extern "C" {
void ghiCallProgressCallback(GHIConnection *self, char *p1, u32 p2) {
    if (self->progressCallback != 0) {
        self->progressCallback(self->request, self->state, p1, p2, self->fileBytesReceived, self->totalSize, self->callbackParam);
    }
}
}
}

namespace Nc {
extern "C" {
void ghiCallPostCallback(GHIConnection *self) {
    if (self->postingState.callback != 0) {
        u32 a = ArrayLength(self->postingState.states);
        self->postingState.callback(self->request, self->postingState.bytesPosted, self->postingState.totalBytes, self->postingState.index, a, self->callbackParam);
    }
}
}
}

namespace Ng {
extern "C" {
s32 ghiResizeBuffer(GHIBuffer *o, s32 n) {
    s32 newsize;
    void *p;
    if (o == 0) {
        return FALSE;
    }
    if (n <= 0) {
        return FALSE;
    }
    newsize = o->size + n;
    p = GsUtil_Realloc(o->data, newsize);
    if (p == 0) {
        return FALSE;
    }
    o->data = (char *)p;
    o->size = newsize;
    return TRUE;
}
}
}

namespace Ng {
extern "C" {
s32 ghiInitBuffer(GHIConnection *ow, GHIBuffer *o, s32 size, s32 grow) {
    if (ow == 0) {
        return FALSE;
    }
    if (o == 0) {
        return FALSE;
    }
    if (size <= 0) {
        return FALSE;
    }
    if (grow <= 0) {
        return FALSE;
    }
    o->connection = ow;
    o->data = 0;
    o->size = 0;
    o->len = 0;
    o->pos = 0;
    o->sizeIncrement = grow;
    o->fixed = 0;
    o->dontFree = 0;
    o->isEncrypted = 0;
    if (ghiResizeBuffer(o, size) == 0) {
        return FALSE;
    }
    *o->data = 0;
    return TRUE;
}
}
}

namespace Ng {
extern "C" {
s32 ghiInitFixedBuffer(GHIConnection *ow, GHIBuffer *o, char *buf, s32 size) {
    if (ow == 0) {
        return FALSE;
    }
    if (o == 0) {
        return FALSE;
    }
    if (buf == 0) {
        return FALSE;
    }
    if (size <= 0) {
        return FALSE;
    }
    o->connection = ow;
    o->data = buf;
    o->size = size;
    o->len = 0;
    o->sizeIncrement = 0;
    o->fixed = 1;
    o->dontFree = 1;
    o->isEncrypted = 0;
    *o->data = 0;
    return TRUE;
}
}
}

namespace Ng {
extern "C" {
void ghiFreeBuffer(GHIBuffer *o) {
    if (o != 0 && o->data != 0) {
        if (o->dontFree == 0) {
            GsUtil_Free(o->data);
        }
        memset(o, 0, sizeof *o);  // (0x24 on the DS)
    }
}
}
}

namespace Ng {
extern "C" {
s32 ghiAppendDataToBuffer(GHIBuffer *o, const char *s, s32 len) {
    GHIConnection *ow = o->connection;
    s32 n;
    s32 r;
    if (o == 0) {
        return FALSE;
    }
    if (s == 0) {
        return FALSE;
    }
    if (len < 0) {
        return FALSE;
    }
    if (len == 0) {
        len = STD_GetStringLength(s);
    }
    if (o->isEncrypted == 1) {
        do {
            n = o->size - o->len;
            r = ow->encryptFn(ow, &ow->encryptor, s, &len, o->data + o->len, &n);
            if (r == 2) {
                if (o->fixed != 0) {
                    o->connection->completed = 1;
                    o->connection->result = 2;
                    return FALSE;
                }
                if (ghiResizeBuffer(o, o->sizeIncrement) != 0) {
                    o->connection->completed = 1;
                    o->connection->result = 1;
                    return FALSE;
                }
            } else {
                o->len += n;
            }
        } while (r == 2);
    } else {
        s32 t = o->len + len;
        while (t >= o->size) {
            if (o->fixed != 0) {
                o->connection->completed = 1;
                o->connection->result = 2;
                return FALSE;
            }
            if (ghiResizeBuffer(o, o->sizeIncrement) == 0) {
                o->connection->completed = 1;
                o->connection->result = 1;
                return FALSE;
            }
        }
        memcpy(o->data + o->len, s, len);
        o->len = t;
        o->data[o->len] = 0;
    }
    return TRUE;
}
}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0227931c) {
    HOST_OVERLAY_VAR(ghiThrottleBufferSize),
    HOST_OVERLAY_VAR(ghiThrottleTimeDelay),
    HOST_OVERLAY_VAR(ghiConnections),
    HOST_OVERLAY_VAR(ghiNextUniqueID),
    HOST_OVERLAY_VAR(ghiNumConnections),
    HOST_OVERLAY_VAR(ghiConnectionsLen),
    HOST_OVERLAY_VAR(ghiProxyAddress),
    HOST_OVERLAY_VAR(ghiProxyPort),
    HOST_OVERLAY_VAR(ghiReferenceCount),
    HOST_OVERLAY_VARS_END
};
