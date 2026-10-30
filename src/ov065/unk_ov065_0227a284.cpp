// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/SockHostEnt.h"
#include "net/darray.h"
#include "net/ghttpConnection.h"
#include "net/gsPlatformUtil.h"
#include "net/SockAddrIn.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_0227a284.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_02278328.h"
#include "proto/ov065/unk_ov065_022789fc.h"
#include "proto/ov065/unk_ov065_0227931c.h"
extern "C" {
s32 OS_SPrintf(char *dst, const char *fmt, ...);
int strncmp(const char *a, const char *b, u32 n);
}

// ov065 TU38: ghttp (2): request post / process / response handlers (0x0227a284..0x0227bd20)


extern "C" {
char data_ov065_0228ca58[4] = "%00";
volatile s32 data_ov065_022910e4;
volatile s32 data_ov065_022910e0;
volatile s32 data_ov065_022910dc;
volatile s32 data_ov065_022910e8;
}

namespace Nm {





extern "C" {
extern s32 ghiReferenceCount;
extern u32 ghiThrottleTimeDelay;
extern s32 ghiThrottleBufferSize;
extern char data_ov065_0228ca58[4];

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
s32 ghiProcessConnection(GHIConnection *c);
void ghiHandleStatus(GHIConnection *c);
s32 ghiPostDoPosting(GHIConnection *c);
s32 ghiPostStateDoPosting(GHIPostState *st, GHIConnection *c, s32 first);
s32 ghiPostFileMemoryStateDoPosting(GHIPostState *st, GHIConnection *c);
s32 ghiPostFileDiskStateDoPosting(GHIPostState *st, GHIConnection *c);
s32 ghiPostStringStateDoPosting(GHIPostState *st, GHIConnection *c);
}
}

namespace Na {


// ghiDoReceivingHeaders' local copy of "2147483647" (SDK ghttpProcess.c `char szMaxSize[] = "2147483647";`): copied as
// one 11-byte block (not an SDK type), since a char array initializer emits a separate .data copy of the string.
struct GsHttpMaxSizeString {
    char b[11];
};

extern "C" {
extern volatile s32 data_ov065_022910e8;
extern volatile s32 data_ov065_022910e4;
extern volatile s32 data_ov065_022910e0;
extern volatile s32 data_ov065_022910dc;
extern u16 data_0213a510[];

void ArrayFree(DArrayImplementation *);
void ArrayAppend(DArrayImplementation *, void *);
DArrayImplementation *ArrayNew(s32, s32, ArrayElementFreeFn);
char *goastrdup(const char *);
s32 GOAGetLastError(s32);
void fclose(s32);
s32 fseek(s32, s32, s32);
s32 ftell(s32);
void rewind(s32);
u32 strspn(char *, char *);

s32 ghiPostGetContentLength(GHIConnection *self);
void ghiPostStateCleanup(GHIPostState *it);
s32 ghiPostStateInit(GHIPostState *it);
s32 ghiPostGetHasFilesContentLength(GHIConnection *self);
s32 ghiPostGetNoFilesContentLength(GHIConnection *self);
void ghiFreePost(GHIPost *t);
void ghiPostDataFree(GHIPostData *r);
}


extern "C" {
#pragma enumsalwaysint off
enum Unk_ov065_0227acfc_Z { Unk_ov065_0227acfc_Z_0 = 0, Unk_ov065_0227acfc_Z_FF = 0xff };
#pragma enumsalwaysint reset
}
extern "C" {
void ghiPostCleanupState(GHIConnection *self);
s32 ghiPostInitState(GHIConnection *self);
void ghiPostStateCleanup(GHIPostState *it);
s32 ghiPostStateInit(GHIPostState *it);
s32 ghiPostGetContentLength(GHIConnection *self);
s32 ghiPostGetHasFilesContentLength(GHIConnection *self);
s32 ghiPostGetNoFilesContentLength(GHIConnection *self);
char *ghiPostGetContentType(GHIConnection *self);
void ghiFreePost(GHIPost *t);
s32 ghiIsPostAutoFree(GHIPost *t);
GHIPost *ghiNewPost(void);
void ghiPostDataFree(GHIPostData *r);
void ghiDoReceivingFile(GHIConnection *self);
void ghiDoReceivingHeaders(GHIConnection *self);
}
}

namespace Nb {


extern "C" {
extern s32 ghiThrottleBufferSize;
extern u16 data_0213a510[];


s32 GOAGetLastError(s32);
s32 GSISocketSelect(s32 sock, s32 *rd, s32 *wr, s32 *ex);
s32 SetReceiveBufferSize(s32 sock, s32 val);
s32 SetSockBlocking(s32 sock, s32 flag);
s32 ghiSendBufferedData(GHIConnection *);
s32 ghiAppendIntToBuffer(GHIBuffer *, s32);
s32 ghiPostDoPosting(GHIConnection *);
char *ghiPostGetContentType(GHIConnection *);
void ghiPostCleanupState(GHIConnection *self);
SockHostEnt *Sock_GetHostByName(char *);
s32 ghiParseURL(GHIConnection *);

void ghiAppendToChunkHeaderBuffer(GHIConnection *self, char *p, s32 n);
s32 ghiParseChunkSize(GHIConnection *self);
s32 ghiDeliverIncomingFileData(GHIConnection *self, char *p, s32 n);
s32 ghiParseStatus(GHIConnection *self);
}

#define HTONS(x) ((((x) >> 8) & 0xff) | (((x) << 8) & 0xff00))

static inline s32 Unk_ov065_0227b5d4_Chk(char *s, s32 i) {
    BOOL bad = TRUE;
    s32 v;
    s32 ch = s[i];
    if (ch >= 0 && ch < 0x80) {
        bad = FALSE;
    }
    if (bad) {
        v = 0;
    } else {
        v = data_0213a510[ch] & 0x100;
    }
    return v;
}
extern "C" {
void ghiAppendToChunkHeaderBuffer(GHIConnection *self, char *p, s32 n);
s32 ghiParseChunkSize(GHIConnection *self);
s32 ghiDeliverIncomingFileData(GHIConnection *self, char *p, s32 n);
void ghiDoReceivingStatus(GHIConnection *self);
s32 ghiParseStatus(GHIConnection *self);
void ghiDoWaiting(GHIConnection *self);
void ghiDoPosting(GHIConnection *self);
void ghiDoSendingRequest(GHIConnection *self);
void ghiDoSecuringSession(GHIConnection *self);
void ghiDoConnecting(GHIConnection *self);
void ghiDoHostLookup(GHIConnection *self);
}
}

namespace Nh {







typedef void (*GPCallback)(void *, void *, void *);


extern "C" {
char *goastrdup(const char *);
void gpiSetErrorString(void *, const char *);
s32 gpiSendBuddyMessage(void *, s32, s32, const char *);
s32 gpiDeleteBuddy(void *, s32);
s32 gpiAuthBuddyRequest(void *, s32);
s32 gpiSetInfos(void *, s32, s32);
s32 gpiGetInfo(void *, s32, s32, s32, GPCallback, void *);
s32 gpiProfileSearch(void *, char *, char *, char *, char *, char *, s32, s32, s32, GPCallback, void *);
s32 gpiAppendStringToBuffer(void *, char *, const char *);
s32 gpiAppendIntToBuffer(void *, char *, s32);
s32 gpiGetProfile(void *, s32, void *);
void *gpiFindBuddy(void *, s32);
s32 gpiCanFreeProfile(void *);
s32 gpiRemoveProfile(void *, void *);

}

extern "C" {
}
extern "C" {
BOOL ghiParseURL(GHIConnection *u);
}
}

namespace Nh {
extern "C" {
BOOL ghiParseURL(GHIConnection *u) {
    char *p;
    char *e;
    BOOL https;
    char saved;
    s32 n;
    char *q;
    if (u == NULL) {
        return FALSE;
    }
    p = u->URL;
    if (p == NULL) {
        return FALSE;
    }
    if (strncmp(p, "http://", 7) == 0) {
        https = FALSE;
        p += 7;
    } else if (strncmp(p, "https://", 8) == 0) {
        https = TRUE;
        p += 8;
    } else {
        return FALSE;
    }
    n = strcspn(p, ":/");
    e = p + n;
    saved = p[n];
    p[n] = 0;
    u->serverAddress = goastrdup(p);
    if (u->serverAddress == NULL) {
        return FALSE;
    }
    *e = saved;
    p += n;
    if (*p == ':') {
        p++;
        u->serverPort = atol(p);
        if (u->serverPort == 0) {
            return FALSE;
        }
        do {
            p++;
        } while (*p != 0 && *p != '/');
    } else if (https) {
        u->serverPort = 0x1bb;
    } else {
        u->serverPort = 0x50;
    }
    if (*p == 0) {
        p = "/";
    }
    u->requestPath = goastrdup(p);
    p = u->requestPath;
    q = strchr(p, 0x20);
    while (q != NULL) {
        *q = '+';
        p = u->requestPath;
        q = strchr(p, 0x20);
    }
    if (p != NULL) {
        return TRUE;
    }
    return FALSE;
}
}
}

namespace Nb {
extern "C" {
void ghiDoHostLookup(GHIConnection *self) {
    char *h;
    ghiCallProgressCallback(self, 0, 0);
    SocketStartUp();
    if (ghiParseURL(self) == 0) {
        self->completed = 1;
        self->result = 3;
        return;
    }
    h = self->proxyOverrideServer;
    if (h == 0) {
        h = ghiProxyAddress;
        if (h == 0) {
            h = self->serverAddress;
        }
    }
    self->serverIP = inet_addr(h);
    if (self->serverIP == -1) {
        SockHostEnt *he = Sock_GetHostByName(h);
        if (he == 0) {
            self->completed = 1;
            self->result = 4;
            return;
        }
        self->serverIP = *(u32 *)*he->addrList;
    }
    self->state = 1;
    ghiCallProgressCallback(self, 0, 0);
}
}
}

namespace Nb {
extern "C" {
void ghiDoConnecting(GHIConnection *self) {
    SockAddrIn sa;
    s32 r;
    s32 w[2];
    if (self->socket == -1) {
        self->socket = socket(2, 1, 0);
        if (self->socket == -1) {
            self->completed = 1;
            self->result = 5;
            self->socketError = GOAGetLastError(self->socket);
            return;
        }
        if (SetSockBlocking(self->socket, 0) == 0) {
            self->completed = 1;
            self->result = 5;
            self->socketError = GOAGetLastError(self->socket);
            return;
        }
        if (self->throttle != 0) {
            SetReceiveBufferSize(self->socket, ghiThrottleBufferSize);
        }
        u32 *z = (u32 *)&sa;
        z[0] = 0;
        z[1] = 0;
        sa.family = 2;
        if (self->proxyOverrideServer != 0) {
            sa.port = HTONS(self->proxyOverridePort);
        } else if (ghiProxyAddress != 0) {
            sa.port = HTONS(ghiProxyPort);
        } else {
            sa.port = HTONS(self->serverPort);
        }
        sa.addr = self->serverIP;
        r = connect(self->socket, &sa, 8);
        if (r == -1) {
            s32 e = GOAGetLastError(self->socket);
            if (e != -6 && e != -26 && e != -76) {
                self->completed = 1;
                self->result = 6;
                self->socketError = e;
                return;
            }
        }
    }
    r = GSISocketSelect(self->socket, 0, &w[0], &w[1]) > 0 ? 1 : 0;
    if (r == -1 || w[1] != 0) {
        self->completed = 1;
        self->result = 6;
        if (r == 0) {
            self->socketError = GOAGetLastError(self->socket);
        }
        return;
    }
    if (w[0] != 0) {
        self->state = 2;
        ghiCallProgressCallback(self, 0, 0);
    }
}
}
}

namespace Nb {
extern "C" {
void ghiDoSecuringSession(GHIConnection *self) {
    s32 len;
    char buf[0x400];
    if (self->encryptEnabled == 0) {
        if (strncmp(self->URL, "https://", 8) == 0) {
            self->completed = 1;
            self->result = 0x11;
            return;
        }
        self->state = 3;
        ghiCallProgressCallback(self, 0, 0);
        return;
    }
    if (self->encryptSessionReady != 0) {
        self->state = 3;
        ghiCallProgressCallback(self, 0, 0);
        return;
    }
    if (self->encryptInitialized == 0) {
        if (self->encryptStartFn(self, &self->encryptor) == 3) {
            return;
        }
    }
    if (self->sendBuffer.pos < self->sendBuffer.len) {
        if (ghiSendBufferedData(self) == 0) {
            return;
        }
        if (self->sendBuffer.pos < self->sendBuffer.len) {
            return;
        }
        ghiResetBuffer(&self->sendBuffer);
    }
    len = 0x400;
    ghiDoReceive(self, buf, &len);
}
}
}

namespace Nb {
extern "C" {
void ghiDoSendingRequest(GHIConnection *self) {
    GHIBuffer *b;
    char tmp[0x14];
    if (self->sendBuffer.len == 0) {
        b = &self->sendBuffer;
        const char *m;
        if (self->post != 0) {
            m = "POST ";
        } else if (self->type == 3) {
            m = "HEAD ";
        } else {
            m = "GET ";
        }
        ghiAppendDataToBuffer(b, m, 0);
        if (self->proxyOverrideServer != 0 || ghiProxyAddress != 0) {
            ghiAppendDataToBuffer(b, self->URL, 0);
        } else {
            ghiAppendDataToBuffer(b, self->requestPath, 0);
        }
        ghiAppendDataToBuffer(b, " HTTP/1.1\r\n", 0);
        if (self->serverPort == 0x50) {
            ghiAppendHeaderToBuffer(b, "Host", self->serverAddress);
        } else {
            ghiAppendDataToBuffer(b, "Host: ", 0);
            ghiAppendDataToBuffer(b, self->serverAddress, 0);
            ghiAppendCharToBuffer(b, 0x3a);
            ghiAppendIntToBuffer(b, self->serverPort);
            ghiAppendDataToBuffer(b, "\r\n", 2);
        }
        if (self->sendHeaders == 0 || strstr(self->sendHeaders, "User-Agent") == 0) {
            ghiAppendHeaderToBuffer(b, "User-Agent", "GameSpyHTTP/1.0");
        }
        if (self->persistConnection != 0) {
            ghiAppendHeaderToBuffer(b, "Connection", "Keep-Alive");
        } else {
            ghiAppendHeaderToBuffer(b, "Connection", "close");
        }
        if (self->post != 0) {
            OS_SPrintf(tmp, "%d", self->postingState.totalBytes);
            ghiAppendHeaderToBuffer(b, "Content-Length", tmp);
            ghiAppendHeaderToBuffer(b, "Content-Type", ghiPostGetContentType(self));
        }
        if (self->sendHeaders != 0) {
            ghiAppendDataToBuffer(b, self->sendHeaders, 0);
        }
        ghiAppendDataToBuffer(b, "\r\n", 2);
        if (b != &self->sendBuffer) {
            ghiAppendDataToBuffer(&self->sendBuffer, b->data, b->len);
        }
    }
    if (ghiSendBufferedData(self) != 0 && self->sendBuffer.pos >= self->sendBuffer.len) {
        ghiResetBuffer(&self->sendBuffer);
        if (self->post != 0) {
            self->state = 4;
        } else {
            self->state = 5;
        }
        ghiCallProgressCallback(self, 0, 0);
    }
}
}
}

namespace Nb {
extern "C" {
void ghiDoPosting(GHIConnection *self) {
    s32 old = self->postingState.bytesPosted;
    s32 r = ghiPostDoPosting(self);
    if (r == 0) {
        ghiPostCleanupState(self);
        return;
    }
    if (old != self->postingState.bytesPosted) {
        ghiCallPostCallback(self);
    }
    if (r == 1) {
        ghiPostCleanupState(self);
        self->state = 5;
        ghiCallProgressCallback(self, 0, 0);
    }
}
}
}

namespace Nb {
extern "C" {
void ghiDoWaiting(GHIConnection *self) {
    s32 v[2];
    if (GSISocketSelect(self->socket, v, 0, 0) == -1) {
        self->completed = 1;
        self->result = 5;
        self->socketError = GOAGetLastError(self->socket);
        return;
    }
    if (v[0] != 0) {
        self->state = 6;
        ghiCallProgressCallback(self, 0, 0);
    }
}
}
}

namespace Nb {
extern "C" {
s32 ghiParseStatus(GHIConnection *self) {
    s32 a, b, c, d;
    s32 r;
    r = sscanf(self->recvBuffer.data, "HTTP/%d.%d %d%n", &a, &b, &c, &d);
    while (self->recvBuffer.data[d] != 0 && Unk_ov065_0227b5d4_Chk(self->recvBuffer.data, d) != 0) {
        d++;
    }
    if (r != 3 || a < 1 || c < 100 || c >= 0x258) {
        self->completed = 1;
        self->result = 7;
        return 0;
    }
    self->statusMajorVersion = a;
    self->statusMinorVersion = b;
    self->statusCode = c;
    self->statusStringIndex = d;
    return 1;
}
}
}

namespace Nb {
extern "C" {
void ghiDoReceivingStatus(GHIConnection *self) {
    s32 len;
    char buf[0x400];
    s32 r;
    len = 0x400;
    r = ghiDoReceive(self, buf, &len);
    if (r == 3) {
        return;
    }
    if (r == 1) {
        if (self->recvBuffer.pos == self->recvBuffer.len) {
            return;
        }
    }
    if (r == 0) {
        if (ghiAppendDataToBuffer(&self->recvBuffer, buf, len) == 0) {
            return;
        }
    }
    char *e = strstr(self->recvBuffer.data, "\r\n");
    if (e != 0) {
        s32 d;
        *e = 0;
        d = e - self->recvBuffer.data;
        self->headersEnd = d + 1;
        if (ghiParseStatus(self) == 0) {
            return;
        }
        self->recvBuffer.pos = d + 2;
        self->state = 7;
        ghiCallProgressCallback(self, 0, 0);
        return;
    }
    if (r == 2) {
        self->completed = 1;
        self->result = 7;
        self->socketError = GOAGetLastError(self->socket);
    }
}
}
}

namespace Nb {
extern "C" {
s32 ghiDeliverIncomingFileData(GHIConnection *self, char *p, s32 n) {
    char *a = 0;
    s32 b = 0;
    self->fileBytesReceived += n;
    if (self->fileBytesReceived == self->totalSize || self->connectionClosed != 0) {
        self->completed = 1;
    }
    if (self->type == 0) {
        if (ghiAppendDataToBuffer(&self->getFileBuffer, p, n) == 0) {
            return 0;
        }
        a = self->getFileBuffer.data;
        b = self->getFileBuffer.len;
    } else if (self->type == 1) {
        if (n != 0) {
            self->completed = 1;
            self->result = 13;
            return 0;
        }
        a = p;
        b = n;
    } else if (self->type == 2) {
        a = p;
        b = n;
    }
    ghiCallProgressCallback(self, a, b);
    return 1;
}
}
}

namespace Nb {
extern "C" {
s32 ghiParseChunkSize(GHIConnection *self) {
    s32 v;
    if (sscanf(self->chunkHeader, "%x", &v) != 1) {
        return -1;
    }
    return v;
}
}
}

namespace Nb {
extern "C" {
void ghiAppendToChunkHeaderBuffer(GHIConnection *self, char *p, s32 n) {
    if (n != 0 && self->chunkHeaderLen < 10) {
        s32 l = 10 - self->chunkHeaderLen;
        if (l >= n) {
            l = n;
        }
        memcpy(self->chunkHeader + self->chunkHeaderLen, p, l);
        self->chunkHeaderLen += l;
        self->chunkHeader[self->chunkHeaderLen] = 0;
    }
}
}
}

namespace Nb {
extern "C" {
s32 ghiProcessIncomingFileData(GHIConnection *self, char *p, s32 n) {
    if (self->chunkedTransfer != 0) {
        while (n > 0) {
            if (self->chunkReadingState == 0) {
                char *nl = strchr(p, 10);
                if (nl != 0) {
                    ghiAppendToChunkHeaderBuffer(self, p, nl - p);
                    s32 k = nl + 1 - p;
                    n -= k;
                    p = nl + 1;
                    self->chunkBytesLeft = ghiParseChunkSize(self);
                    s32 t = self->chunkBytesLeft;
                    if (t == -1) {
                        self->completed = 1;
                        self->result = 7;
                        return 0;
                    }
                    if (t == 0) {
                        self->chunkReadingState = 3;
                    } else {
                        self->chunkReadingState = 1;
                    }
                } else {
                    ghiAppendToChunkHeaderBuffer(self, p, n);
                    return 1;
                }
            } else if (self->chunkReadingState == 1) {
                s32 c = self->chunkBytesLeft;
                if (c >= n) {
                    c = n;
                }
                if (ghiDeliverIncomingFileData(self, p, c) == 0) {
                    return 0;
                }
                p += c;
                n -= c;
                self->chunkBytesLeft = self->chunkBytesLeft - c;
                if (self->chunkBytesLeft == 0) {
                    self->chunkReadingState = 2;
                }
            } else if (self->chunkReadingState == 2) {
                char *nl = strchr(p, 10);
                if (nl == 0) {
                    return 1;
                }
                nl = nl + 1;
                n -= nl - p;
                p = nl;
                self->chunkHeader[0] = 0;
                self->chunkHeaderLen = 0;
                self->chunkBytesLeft = 0;
                self->chunkReadingState = 0;
            } else if (self->chunkReadingState == 3) {
                self->completed = 1;
                return 1;
            } else {
                return 0;
            }
        }
        return 1;
    }
    return ghiDeliverIncomingFileData(self, p, n);
}
}
}

namespace Na {
extern "C" {
void ghiDoReceivingHeaders(GHIConnection *self) {
    s32 len;
    char buf[0x1000];
    s32 r4;
    s32 off;
    u8 *p;
    char *rest;
    s32 rem;
    char *q;
    char *digits;
    s32 st;
    char *e;
    len = 0x1000;
    r4 = ghiDoReceive(self, buf, &len);
    if (r4 == 3) {
        return;
    }
    if (r4 == 1 && self->recvBuffer.pos == self->recvBuffer.len) {
        return;
    }
    if (r4 == 0 && ghiAppendDataToBuffer(&self->recvBuffer, buf, len) == 0) {
        return;
    }
    off = self->recvBuffer.pos;
    p = (u8 *)self->recvBuffer.data + off;
    self->headerStringIndex = off;
    q = strstr((char *)p, "\r\n\r\n");
    if (q == NULL) {
        q = strstr((char *)p, "\n\n");
    }
    if (q == NULL) {
        goto nomatch;
    }
    q[2] = 0;
    rest = q + 4;
    rem = self->recvBuffer.len - (rest - self->recvBuffer.data);
    self->recvBuffer.len = (u8 *)(q + 2) - (u8 *)self->recvBuffer.data;
    self->headersEnd = (u8 *)(q + 2) - (u8 *)self->recvBuffer.data;
    self->recvBuffer.pos = self->headersEnd;
    st = self->statusCode / 100;
    if (st == 1) {
        if (rem != 0) {
            memmove(self->recvBuffer.data, rest, rem + 1);
            self->recvBuffer.len = rem;
            self->recvBuffer.pos = 0;
        } else {
            ghiResetBuffer(&self->recvBuffer);
        }
        self->state = 6;
        ghiCallProgressCallback(self, 0, 0);
        return;
    }
    if (st == 3) {
        if (self->redirectCount > 10) {
            self->completed = 1;
            self->result = 0xb;
            return;
        }
        q = strstr((char *)p, "Location:");
        if (q != NULL) {
            char *d = q + 9;
            s32 c;
            s32 v;
            goto t1;
        l1:
            d++;
        t1:
            c = *d;
            if (c < 0 || c >= 0x80) {
                v = 0;
            } else {
                v = data_0213a510[c] & 0x100;
            }
            if (v != 0) {
                goto l1;
            }
            e = d;
            goto t2;
        l2:
            e++;
        t2:
            c = *e;
            if (c == 0) {
                goto d2;
            }
            if (c < 0 || c >= 0x80) {
                v = 0;
            } else {
                v = data_0213a510[c] & 0x100;
            }
            if (v == 0) {
                goto l2;
            }
        d2:
            *e = 0;
            if (*d == '/') {
                s32 l = STD_GetStringLength(self->serverAddress);
                s32 m = STD_GetStringLength(d);
                self->redirectURL = (char *)GsUtil_Alloc(l + 0xe + m);
                if (self->redirectURL == NULL) {
                    self->completed = 1;
                    self->result = 1;
                }
                OS_SPrintf(self->redirectURL, "http://%s:%d%s", self->serverAddress, self->serverPort, d);
                return;
            }
            self->redirectURL = goastrdup(d);
            if (self->redirectURL != NULL) {
                return;
            }
            self->completed = 1;
            self->result = 1;
            return;
        }
    }
    q = strstr((char *)p, "Content-Length:");
    if (q != NULL) {
        s32 n;
        char *d0;
        s32 dl;
        char *t;
        GsHttpMaxSizeString szMaxSize = *(GsHttpMaxSizeString *)"2147483647";
        char *hdr = szMaxSize.b;
        e = q + 0x10;
        t = e;
        n = STD_GetStringLength(hdr);
        while (t != NULL && *t != 0 && *t != 10 && *t != 13 && *t != 0x20) {
            t++;
        }
        dl = t - e;
        if (dl > n) {
            self->completed = 1;
            self->result = 0x10;
            return;
        }
        if (n == dl) {
            if (strncmp(e, hdr, dl) >= 0) {
                self->completed = 1;
                self->result = 0x10;
                return;
            }
        }
        self->totalSize = atol(e);
    }
    self->chunkedTransfer = strstr((char *)p, "Transfer-Encoding: chunked") != NULL ? 1 : 0;
    if (self->chunkedTransfer != 0) {
        self->chunkHeader[0] = 0;
        self->chunkHeaderLen = 0;
        self->chunkBytesLeft = 0;
        self->chunkReadingState = 0;
    }
    if ((u32)(self->type - 3) <= 1) {
        self->completed = 1;
        return;
    }
    self->state = 8;
    if (q != NULL) {
        if (self->totalSize == 0) {
            self->completed = 1;
            return;
        }
    }
    if (rem > 0) {
        ghiProcessIncomingFileData(self, rest, rem);
    }
    return;
nomatch:
    if (r4 == 2) {
        self->completed = 1;
        self->result = 7;
        self->socketError = GOAGetLastError(self->socket);
    }
}
}
}

namespace Na {
extern "C" {
// Not in the original binary: unreferenced weak function compiled right after ghiDoReceivingHeaders, so that the literal
// "2147483647" is pooled where the original has it; removed by the dead-stripping link (see notes.txt).
__declspec(weak) void Unk_ov065_0227ae94_pool_order(void) {
    STD_GetStringLength("2147483647");
}
}
}

namespace Na {
extern "C" {
void ghiDoReceivingFile(GHIConnection *self) {
    s32 len;
    char buf[0x2000];
    s32 start = current_time();
    u32 elapsed = 0;
    s32 r;
    while (self->completed == 0 && elapsed < self->maxRecvTime) {
        len = 0x2000;
        r = ghiDoReceive(self, buf, &len);
        if (r == 3 || r == 1) {
            break;
        }
        if (r == 2) {
            self->completed = 1;
            if (self->totalSize > 0 && self->fileBytesReceived < self->totalSize) {
                self->result = 0xf;
                return;
            }
            break;
        }
        if (ghiProcessIncomingFileData(self, buf, len) == 0) {
            break;
        }
        elapsed = current_time() - start;
    }
}
}
}

namespace Na {
extern "C" {
void ghiPostDataFree(GHIPostData *r) {
    GsUtil_Free(r->name);
    if (r->type == 0) {
        GsUtil_Free(r->data.string.string);
    } else if (r->type == 1) {
        GsUtil_Free(r->data.fileDisk.filename);
        GsUtil_Free(r->data.fileDisk.reportFilename);
        GsUtil_Free(r->data.fileDisk.contentType);
    } else if (r->type == 2) {
        GsUtil_Free(r->data.fileMemory.reportFilename);
        GsUtil_Free(r->data.fileMemory.contentType);
    }
}
}
}

namespace Na {
extern "C" {
GHIPost *ghiNewPost(void) {
    GHIPost *t = (GHIPost *)GsUtil_Alloc(sizeof(GHIPost));
    u8 *p;
    u32 i;
    u8 *q;
    u8 *k;
    Unk_ov065_0227acfc_Z z;
    if (t == NULL) {
        return NULL;
    }
    q = (u8 *)t;
    z = Unk_ov065_0227acfc_Z_0;
#ifdef HOST64
    // (the whole post, 0x14 bytes on the DS; the DS's count down in a pointer to NULL is undefined in C, and clang
    // made it a loop without end - it wrote on until a fault: the crash connecting, ghiNewPost+0x14)
    (void)k;
    for (i = 0; i < sizeof(GHIPost); i++) q[i] = z;
#else
    k = (u8 *)0x14;
    do {
        *q++ = z;
        k--;
    } while (k != NULL);
#endif
    t->autoFree = 1;
    t->data = ArrayNew(sizeof(GHIPostData), 0, (ArrayElementFreeFn)ghiPostDataFree);
    if (t->data == NULL) {
        GsUtil_Free(t);
        return NULL;
    }
    return t;
}
}
}

namespace Na {
extern "C" {
s32 ghiIsPostAutoFree(GHIPost *t) {
    return t->autoFree;
}
}
}

namespace Na {
extern "C" {
void ghiFreePost(GHIPost *t) {
    ArrayFree(t->data);
    GsUtil_Free(t);
}
}
}

namespace Na {
extern "C" {
s32 ghiPostAddString(GHIPost *self, char *a, char *b) {
    s32 len;
    s32 cnt;
    s32 i;
    s32 c;
    a = goastrdup(a);
    b = goastrdup(b);
    if (a == NULL || b == NULL) {
        GsUtil_Free(a);
        GsUtil_Free(b);
        return 0;
    }
    GHIPostData item = {0, 0, 0, 0, 0, 0};
    item.type = 0;
    item.name = a;
    item.data.string.string = b;
    len = STD_GetStringLength(b);
    item.data.string.len = len;
    item.data.string.invalidChars = 0;
    c = strspn(b, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@-.*");
    if (c != len) {
        cnt = 0;
        item.data.string.invalidChars = 1;
        for (i = 0; b[i] != 0; i++) {
            c = b[i];
            if (strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@-.*", c) == NULL && c != 0x20) {
                cnt++;
            }
        }
        item.data.string.extendedChars = cnt;
    }
    ArrayAppend(self->data, &item);
    return 1;
}
}
}

namespace Na {
extern "C" {
char *ghiPostGetContentType(GHIConnection *self) {
    if (self->post == NULL) {
        return "";
    }
    if (self->post->hasFiles != 0) {
        return "multipart/form-data; boundary=Qr4G823s23d---<<><><<<>--7d118e0536";
    }
    return "application/x-www-form-urlencoded";
}
}
}

namespace Na {
extern "C" {
s32 ghiPostGetNoFilesContentLength(GHIConnection *self) {
    GHIPost *t = self->post;
    s32 sum = 0;
    s32 n;
    s32 i;
    n = ArrayLength(t->data);
    if (n == 0) {
        return sum;
    }
    i = sum;
    if (i < n) {
        do {
            GHIPostData *r = (GHIPostData *)ArrayNth(t->data, i);
            s32 l = STD_GetStringLength(r->name);
            s32 t = sum + l;
            s32 u = t + r->data.string.len;
            sum = u + r->data.string.extendedChars * 2 + 1;
            i++;
        } while (i < n);
    }
    return sum + (n - 1);
}
}
}

namespace Na {
extern "C" {
s32 ghiPostGetHasFilesContentLength(GHIConnection *self) {
    GHIPost *t = self->post;
    s32 sum = 0;
    s32 n;
    s32 i;
    if (data_ov065_022910e8 == 0) {
        s32 l = STD_GetStringLength("--Qr4G823s23d---<<><><<<>--7d118e0536");
        data_ov065_022910e8 = l;
        data_ov065_022910e4 = l + 0x2f;
        data_ov065_022910e0 = l + 0x4c;
        data_ov065_022910dc = l + 4;
    }
    n = ArrayLength(t->data);
    for (i = 0; i < n; i++) {
        GHIPostData *r = (GHIPostData *)ArrayNth(t->data, i);
        if (r->type == 0) {
            sum += data_ov065_022910e4;
            sum += STD_GetStringLength(r->name);
            sum += r->data.string.len;
        } else if (r->type == 1) {
            sum += data_ov065_022910e0;
            sum += STD_GetStringLength(r->name);
            sum += STD_GetStringLength(r->data.fileDisk.reportFilename);
            sum += STD_GetStringLength(r->data.fileDisk.contentType);
            sum += ((GHIPostState *)ArrayNth(self->postingState.states, i))->fileLength;
        } else if (r->type == 2) {
            sum += data_ov065_022910e0;
            sum += STD_GetStringLength(r->name);
            sum += STD_GetStringLength(r->data.fileMemory.reportFilename);
            sum += STD_GetStringLength(r->data.fileMemory.contentType);
            sum += r->data.fileMemory.len;
        } else {
            return 0;
        }
    }
    return sum + data_ov065_022910dc;
}
}
}

namespace Na {
extern "C" {
s32 ghiPostGetContentLength(GHIConnection *self) {
    if (self->post == NULL) {
        return 0;
    }
    if (self->post->hasFiles != 0) {
        return ghiPostGetHasFilesContentLength(self);
    }
    return ghiPostGetNoFilesContentLength(self);
}
}
}

namespace Na {
extern "C" {
s32 ghiPostStateInit(GHIPostState *it) {
    s32 t = it->data->type;
    s32 z = 0;
    it->pos = -1;
    if (t == 0) {
    } else if (t == 1) {
        if (it->file == 0) {
            return z;
        }
        if (fseek(it->file, z, 2) != 0) {
            return 0;
        }
        it->fileLength = ftell(it->file);
        if (it->fileLength == -1) {
            return 0;
        }
        rewind(it->file);
    } else if (t == 2) {
    } else {
        return z;
    }
    return 1;
}
}
}

namespace Na {
extern "C" {
void ghiPostStateCleanup(GHIPostState *it) {
    switch (it->data->type) {
    case 0:
        break;
    case 1:
        if (it->file != 0) {
            fclose(it->file);
        }
        it->file = 0;
        break;
    }
}
}
}

namespace Na {
extern "C" {
s32 ghiPostInitState(GHIConnection *self) {
    GHIPostState item;
    s32 n;
    s32 i;
    if (self->post == NULL) {
        return 0;
    }
    self->postingState.index = 0;
    self->postingState.bytesPosted = 0;
    self->postingState.totalBytes = 0;
    self->postingState.callback = self->post->callback;
    self->postingState.param = self->post->param;
    n = ArrayLength(self->post->data);
    self->postingState.states = ArrayNew(sizeof(GHIPostState), n, NULL);
    if (self->postingState.states == NULL) {
        return 0;
    }
    i = 0;
    if (i < n) {
        GHIPostState *pi = &item;
        volatile s32 z = 0;
        do {
            GHIPostData *rec = (GHIPostData *)ArrayNth(self->post->data, i);
            s32 t = z;
            pi->data = NULL;
            pi->pos = t;
            pi->file = t;
            pi->fileLength = t;
            item.data = rec;
            if (ghiPostStateInit(pi) == 0) {
                for (i--; i >= 0; i--) {
                    ghiPostStateCleanup((GHIPostState *)ArrayNth(self->postingState.states, i));
                }
                ArrayFree(self->postingState.states);
                self->postingState.states = NULL;
                return 0;
            }
            ArrayAppend(self->postingState.states, pi);
            i++;
        } while (i < n);
    }
    self->postingState.totalBytes = ghiPostGetContentLength(self);
    return 1;
}
}
}

namespace Na {
extern "C" {
void ghiPostCleanupState(GHIConnection *self) {
    if (self->postingState.states != NULL) {
        s32 n = ArrayLength(self->postingState.states);
        s32 i = 0;
        if (i < n) {
            do {
                ghiPostStateCleanup((GHIPostState *)ArrayNth(self->postingState.states, i));
                i++;
            } while (i < n);
        }
        ArrayFree(self->postingState.states);
        self->postingState.states = NULL;
    }
    if (self->post != NULL) {
        if (self->post->autoFree != 0) {
            ghiFreePost(self->post);
            self->post = NULL;
        }
    }
}
}
}

namespace Nm {
extern "C" {
s32 ghiPostStringStateDoPosting(GHIPostState *st, GHIConnection *c) {
    GHIPostData *q = st->data;
    if (q->data.string.len == 0) {
        return 1;
    }
    if (c->post->hasFiles == 0 && q->data.string.invalidChars != 0) {
        char *s = q->data.string.string;
        struct T4 {
            char b[4];
        };
        T4 tmp = *(T4 *)data_ov065_0228ca58;
        s32 i = 0;
        char ch = s[i];
        if (ch != 0) {
            do {
                if (strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_@-.*", ch) != 0) {
                    ghiAppendCharToBuffer(&c->sendBuffer, ch);
                } else if (ch == 0x20) {
                    ghiAppendCharToBuffer(&c->sendBuffer, 0x2b);
                } else {
                    tmp.b[1] = "0123456789ABCDEF"[ch / 16];
                    tmp.b[2] = "0123456789ABCDEF"[ch % 16];
                    ghiAppendDataToBuffer(&c->sendBuffer, tmp.b, 3);
                }
                i++;
                ch = s[i];
            } while (ch != 0);
        }
        return 1;
    }
    s32 n = q->data.string.len - st->pos;
    s32 r = ghiDoSend(c, q->data.string.string, n);
    if (r == -1) {
        return 0;
    }
    st->pos = st->pos + r;
    if (r == n) {
        return 1;
    }
    return 2;
}
}
}

namespace Nm {
extern "C" {
s32 ghiPostFileDiskStateDoPosting(GHIPostState *st, GHIConnection *c) {
    char buf[0x1000];
    s32 r;
    do {
        s32 n = fread(buf, 1, 0x1000, st->file);
        if (n <= 0) {
            c->completed = 1;
            c->result = 14;
            return 0;
        }
        st->pos = st->pos + n;
        if (st->pos > st->fileLength) {
            c->completed = 1;
            c->result = 14;
            return 0;
        }
        r = ghiTrySendThenBuffer(c, buf, n);
        if (r == 0) {
            return 0;
        }
        if (st->pos == st->fileLength) {
            return 1;
        }
    } while (r == 1);
    return 2;
}
}
}

namespace Nm {
extern "C" {
s32 ghiPostFileMemoryStateDoPosting(GHIPostState *st, GHIConnection *c) {
    GHIPostData *p = st->data;
    s32 len = p->data.fileMemory.len;
    if (len == 0) {
        return 1;
    }
    do {
        s32 n = len - st->pos;
        if (n >= 0x8000) {
            n = 0x8000;
        }
        s32 r = ghiDoSend(c, p->data.fileMemory.buffer + st->pos, n);
        if (r == -1) {
            return 0;
        }
        st->pos = st->pos + r;
        p = st->data;
        len = p->data.fileMemory.len;
        if (len == st->pos) {
            return 1;
        }
        if (r == 0) {
            return 2;
        }
    } while (1);
}
}
}

namespace Nm {
extern "C" {
s32 ghiPostStateDoPosting(GHIPostState *st, GHIConnection *c, s32 first) {
    char buf[2048];
    if (st->pos == -1) {
        st->pos = 0;
        if (c->post->hasFiles == 0) {
            if (first != 0) {
                OS_SPrintf(buf, "%s=", st->data->name);
            } else {
                OS_SPrintf(buf, "&%s=", st->data->name);
            }
        } else {
            GHIPostData *p = st->data;
            if (p->type == 0) {
                OS_SPrintf(buf, "%sContent-Disposition: form-data; name=\"%s\"\r\n\r\n", first != 0 ? "--Qr4G823s23d---<<><><<<>--7d118e0536\r\n" : "\r\n--Qr4G823s23d---<<><><<<>--7d118e0536\r\n", p->name);
            } else if (p->type == 1 || p->type == 2) {
                char *a, *b;
                if (p->type == 1) {
                    a = p->data.fileDisk.reportFilename;
                    b = p->data.fileDisk.contentType;
                } else {
                    a = p->data.fileMemory.reportFilename;
                    b = p->data.fileMemory.contentType;
                }
                OS_SPrintf(buf, "%sContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: %s\r\n\r\n", first != 0 ? "--Qr4G823s23d---<<><><<<>--7d118e0536\r\n" : "\r\n--Qr4G823s23d---<<><><<<>--7d118e0536\r\n", p->name, a, b);
            }
        }
        s32 r = ghiTrySendThenBuffer(c, buf, STD_GetStringLength(buf));
        if (r == 0) {
            return 0;
        }
        if (r == 2) {
            return 2;
        }
    }
    if (st->data->type == 0) {
        return ghiPostStringStateDoPosting(st, c);
    }
    if (st->data->type == 1) {
        return ghiPostFileDiskStateDoPosting(st, c);
    }
    return ghiPostFileMemoryStateDoPosting(st, c);
}
}
}

namespace Nm {
extern "C" {
s32 ghiPostDoPosting(GHIConnection *c) {
    GHIPostingState *l = &c->postingState;
    s32 cnt = ArrayLength(l->states);
    if (c->sendBuffer.len != 0) {
        if (ghiSendBufferedData(c) == 0) {
            return 0;
        }
        if (c->sendBuffer.pos < c->sendBuffer.len) {
            return 2;
        }
        ghiResetBuffer(&c->sendBuffer);
        if (c->postingState.index == cnt) {
            return 1;
        }
    }
    for (; l->index < cnt; l->index++) {
        GHIPostState *s = (GHIPostState *)ArrayNth(l->states, l->index);
        s32 r = ghiPostStateDoPosting(s, c, l->index == 0 ? 1 : 0);
        if (r == 0) {
            return 0;
        }
        if (r == 2) {
            return 2;
        }
    }
    if (c->post->hasFiles != 0) {
        s32 n = STD_GetStringLength("\r\n--Qr4G823s23d---<<><><<<>--7d118e0536--\r\n");
        if (ghiTrySendThenBuffer(c, "\r\n--Qr4G823s23d---<<><><<<>--7d118e0536--\r\n", n) == 0) {
            return 0;
        }
    }
    if (c->sendBuffer.len != 0) {
        return 2;
    }
    return 1;
}
}
}

namespace Nm {
extern "C" {
void ghiHandleStatus(GHIConnection *c) {
    s32 code = c->statusCode;
    switch (code / 100) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        return;
    case 4:
        switch (code) {
        case 401:
            c->result = 9;
            return;
        case 402:
        case 405:
        case 406:
        case 407:
        case 408:
        case 409:
            break;
        case 403:
            c->result = 10;
            return;
        case 404:
        case 410:
            c->result = 11;
            return;
        }
        c->result = 8;
        return;
    case 5:
        c->result = 12;
        break;
    }
}
}
}

namespace Nm {
extern "C" {
s32 ghiProcessConnection(GHIConnection *c) {
    s32 r;
    if (c->processing != 0) {
        return 0;
    }
    c->processing = 1;
    if (c->state == 0) {
        ghiDoHostLookup(c);
    }
    if (c->state == 1) {
        ghiDoConnecting(c);
    }
    if (c->state == 2) {
        ghiDoSecuringSession(c);
    }
    if (c->state == 3) {
        ghiDoSendingRequest(c);
    }
    if (c->state == 4) {
        ghiDoPosting(c);
    }
    if (c->state == 5) {
        ghiDoWaiting(c);
    }
    if (c->state == 6) {
        ghiDoReceivingStatus(c);
    }
    if (c->state == 7) {
        ghiDoReceivingHeaders(c);
    }
    if (c->state == 8) {
        ghiDoReceivingFile(c);
    }
    if (c->redirectURL != 0) {
        ghiRedirectConnection(c);
    }
    r = c->completed;
    if (r != 0) {
        ghiHandleStatus(c);
        ghiCallCompletedCallback(c);
        ghiFreeConnection(c);
    } else {
        c->processing = 0;
    }
    return r;
}
}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_0227a284) {
    HOST_OVERLAY_VAR(data_ov065_0228ca58),
    HOST_OVERLAY_VAR(data_ov065_022910e4),
    HOST_OVERLAY_VAR(data_ov065_022910e0),
    HOST_OVERLAY_VAR(data_ov065_022910dc),
    HOST_OVERLAY_VAR(data_ov065_022910e8),
    HOST_OVERLAY_VARS_END
};
