#include "ds_div.h"
#include "uptr.h"
// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/sb_crypt.h"
#include "net/SockAddrIn.h"
#include "net/sb_internal.h"
#include "net/gsPlatformUtil.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212fa54.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_02288510.h"
#include "proto/ov065/unk_ov065_022884fc.h"

// ov065_066: GameSpy transport (RC4-like cipher, connection manager) 0x022884fc..0x02288df0





struct SBQueryEngine;




extern "C" {
extern u8 data_0213a410[];

u32 func_0213335c(u32 a, u32 b);
void SocketStartUp(void);
s32 CanReceiveOnSocket(s32 s);
s32 recvfrom(s32 s, void *buf, s32 len, u32 flags, void *sa, s32 *salen);
s32 sendto(s32 s, void *buf, s32 len, u32 flags, void *sa, s32 salen);
s32 closesocket(s32 s);
s32 socket(s32 a, s32 b, s32 c);
void *TableNew2(s32 a, s32 b, s32 c, void *cmp, void *hash, void *free);
s32 NTSLengthSB(u8 *buf, s32 n);
void SBReleaseStr(s32 a, void *p);
void SBServerAddKeyValue(void *e, char *k, char *v);
void SBServerParseQR2FullKeysSingle(void *e, u8 *buf, s32 n);
void SBServerParseKeyVals(void *e, u8 *buf);
}

typedef GOACryptState Cipher;
typedef SBQueryEngine Mgr;
typedef _SBServer Ent;
typedef SBServerFIFO List;
typedef SockAddrIn Sa;

extern "C" {
u32 GOADecryptByte(Cipher *c, u32 x);
void GOAHashInit(Cipher *c);
u32 keyrand(Cipher *c, u32 n, u8 *key, u32 keylen, u8 *j, u32 *idx);
s32 FIFORemove(List *l, Ent *e);
void ProcessIncomingReplies(Mgr *m, s32 flag);
void TimeoutOldQueries(Mgr *m);
void QueueNextQueries(Mgr *m);
void QEStartQuery(Mgr *m, Ent *e);
s32 ParseSingleICMPReply(Mgr *m, Ent *e, u8 *buf, s32 n);
void ParseSingleGOAReply(Mgr *m, Ent *e, u8 *buf, s32 n);
void ParseSingleQR2Reply(Mgr *m, Ent *e, u8 *buf, s32 n);
void FIFOClear(List *l);
Ent *FIFOGetFirst(List *l);
void FIFOAddFront(List *l, Ent *e);
void FIFOAddRear(List *l, Ent *e);
s32 KeyValCompareKey(SBKeyValuePair *a, SBKeyValuePair *b);
u32 KeyValHashKey(SBKeyValuePair *p, u32 n);
void KeyValFree(SBKeyValuePair *p);
u32 StringHash(u8 *s, u32 n);

void QEStartQuery(Mgr *m, Ent *e) {
    Sa sa;
    u8 buf[0x100];
    s32 len;
    FIFOAddRear(&m->querylist, e);
    e->updatetime = current_time();
    sa.family = 2;
    if ((e->state & 0x20) == 0) {
        if (m->queryversion == 1) {
            buf[0] = 0xfe;
            buf[1] = 0xfd;
            buf[2] = 0;
            {
                u8 *q = &buf[3];
                u8 *t = (u8 *)&e->updatetime;
                q[0] = t[0];
                q[1] = t[1];
                q[2] = t[2];
                q[3] = t[3];
            }
            if (e->state & 4) {
                s32 i = 0;
                buf[7] = m->numserverkeys;
                if (m->numserverkeys > 0) {
                    do {
                        buf[i + 8] = m->serverkeys[i];
                        i++;
                    } while (i < m->numserverkeys);
                }
                buf[m->numserverkeys + 8] = 0;
                buf[m->numserverkeys + 9] = 0;
                len = m->numserverkeys + 10;
            } else {
                buf[7] = 0xff;
                buf[8] = 0xff;
                buf[9] = 0xff;
                len = 10;
            }
        } else {
            if (e->state & 4) {
                u8 *d = buf;
                u8 *sp = (u8 *)"\\basic\\\\info\\";
#ifdef HOST64
                // (a counter, not a pointer counted down to NULL: undefined in C, a loop without end for clang)
                s32 k = 13;
                do {
                    *d++ = *sp++;
                    k--;
                } while (k != 0);
#else
                u8 *k = (u8 *)13;
                do {
                    *d++ = *sp++;
                    k--;
                } while (k != NULL);
#endif
                len = 13;
            } else {
                u8 *d = buf;
                u8 *sp = (u8 *)"\\status\\";
#ifdef HOST64
                // (a counter, not a pointer counted down to NULL: undefined in C, a loop without end for clang)
                s32 k = 8;
                do {
                    *d++ = *sp++;
                    k--;
                } while (k != 0);
#else
                u8 *k = (u8 *)8;
                do {
                    *d++ = *sp++;
                    k--;
                } while (k != NULL);
#endif
                len = 8;
            }
        }
        if (e->publicip == m->mypublicip && (e->flags & 2) != 0) {
            sa.addr = e->privateip;
            sa.port = e->privateport;
        } else {
            sa.addr = e->publicip;
            sa.port = e->publicport;
        }
        sendto(m->querysock, buf, len, 0, &sa, 8);
    }
}

void SBQueryEngineInit(Mgr *m, s32 max, s32 mode, s32 force, SBEngineCallbackFn cb, void *user) {
    if (force != 0 || __GSIACResult == 1) {
        SocketStartUp();
        m->queryversion = mode;
        m->maxupdates = max;
        m->numserverkeys = 0;
        m->ListCallback = cb;
        m->instance = user;
        m->mypublicip = 0;
        m->querysock = socket(2, 2, 0);
        FIFOClear(&m->pendinglist);
        FIFOClear(&m->querylist);
    }
}

void SBQueryEngineSetPublicIP(Mgr *m, u32 v) {
    m->mypublicip = v;
}

void SBEngineHaltUpdates(Mgr *m) {
    FIFOClear(&m->pendinglist);
    FIFOClear(&m->querylist);
}

void SBEngineCleanup(Mgr *m) {
    closesocket(m->querysock);
    m->querysock = -1;
    FIFOClear(&m->pendinglist);
    FIFOClear(&m->querylist);
}

void SBQueryEngineUpdateServer(Mgr *m, Ent *e, s32 front, s32 code) {
    u8 *p = &e->state;
    *p &= 0xc3;
    if (code == 0) {
        *p |= 4;
    } else if (code == 1) {
        *p |= 8;
    } else if (code == 2) {
        return;
    }
    if (m->querylist.count < m->maxupdates) {
        QEStartQuery(m, e);
        return;
    }
    if (front != 0) {
        FIFOAddFront(&m->pendinglist, e);
        return;
    }
    FIFOAddRear(&m->pendinglist, e);
}

void ParseSingleQR2Reply(Mgr *m, Ent *e, u8 *buf, s32 n) {
    s32 i = 0;
    s32 r;
    if (*(s8 *)buf == 0) {
        buf += 5;
        n -= 5;
        if (e->state & 4) {
            if (m->numserverkeys > 0) {
                do {
                    r = NTSLengthSB(buf, n);
                    if (r < 0) {
                        break;
                    }
                    SBServerAddKeyValue(e, qr2_registered_key_list[m->serverkeys[i]], (char *)buf);
                    buf += r;
                    n -= r;
                    i++;
                } while (i < m->numserverkeys);
            }
            e->state |= 0x41;
        } else {
            SBServerParseQR2FullKeysSingle(e, buf, n);
            e->state |= 0x43;
        }
        e->state &= 0xf3;
        e->updatetime = current_time() - e->updatetime;
        FIFORemove(&m->querylist, e);
        m->ListCallback(m, 0, e, m->instance);
    }
}

void ParseSingleGOAReply(Mgr *m, Ent *e, u8 *buf, s32 n) {
    BOOL found;
    if (strstr((char *)buf, "\\final\\") != 0) {
        found = TRUE;
    } else {
        found = FALSE;
    }
    SBServerParseKeyVals(e, buf);
    if (found) {
        if (e->state & 4) {
            e->state |= 0x41;
        } else {
            e->state |= 0x42;
        }
        e->state &= 0xf3;
        e->updatetime = current_time() - e->updatetime;
        FIFORemove(&m->querylist, e);
        m->ListCallback(m, 0, e, m->instance);
    }
}

s32 ParseSingleICMPReply(Mgr *m, Ent *e, u8 *buf, s32 n) {
    return 1;
}

void ProcessIncomingReplies(Mgr *m, s32 flag) {
    s32 sock;
    Sa sa;
    s32 len = 8;
    u8 buf[0x800];
    s32 n;
    Ent *e;
    if (flag != 0) {
        sock = m->icmpsock;
    } else {
        sock = m->querysock;
    }
    while (CanReceiveOnSocket(sock) != 0) {
        n = recvfrom(sock, buf, 0x7ff, 0, &sa, &len);
        if (n == -1) {
            break;
        }
        buf[n] = 0;
        for (e = m->querylist.first; e != 0; e = e->next) {
            if (flag != 0 && (e->flags & 8) != 0 && e->icmpip == sa.addr) {
                goto match;
            }
            if (e->publicip == sa.addr) {
                if (e->publicport == sa.port) {
                    goto match;
                }
                if (flag != 0) {
                    goto match;
                }
            }
            if (e->publicip == m->mypublicip && (e->flags & 2) != 0 && e->privateip == sa.addr && e->privateport == sa.port) {
            match:
                if (flag != 0) {
                    if (ParseSingleICMPReply(m, e, buf, n) != 0) {
                        break;
                    }
                    continue;
                }
                if (m->queryversion == 1) {
                    ParseSingleQR2Reply(m, e, buf, n);
                } else {
                    ParseSingleGOAReply(m, e, buf, n);
                }
                break;
            }
        }
    }
}

void TimeoutOldQueries(Mgr *m) {
    u32 now = current_time();
    Ent *e = m->querylist.first;
    if (e != 0) {
        do {
            if (now <= e->updatetime + 0x9c4) {
                return;
            }
            e->flags |= 0x10;
            m->querylist.first->updatetime = 0x9c4;
            m->querylist.first->flags &= 0xd3;
            m->ListCallback(m, 1, m->querylist.first, m->instance);
            FIFOGetFirst(&m->querylist);
            e = m->querylist.first;
        } while (e != 0);
    }
}

void QueueNextQueries(Mgr *m) {
    while (m->querylist.count < m->maxupdates && m->pendinglist.count > 0) {
        Ent *e = FIFOGetFirst(&m->pendinglist);
        QEStartQuery(m, e);
    }
}

void SBQueryEngineThink(Mgr *m) {
    if (m->querylist.count != 0) {
        ProcessIncomingReplies(m, 0);
        TimeoutOldQueries(m);
        if (m->pendinglist.count > 0) {
            QueueNextQueries(m);
        }
        if (m->querylist.count == 0) {
            m->ListCallback(m, 2, 0, m->instance);
        }
    }
}

void SBQueryEngineAddQueryKey(Mgr *m, u32 b) {
    s32 k = m->numserverkeys;
    if (k < 0x14) {
        m->numserverkeys = k + 1;
        m->serverkeys[k] = b;
    }
}

void SBQueryEngineRemoveServerFromFIFOs(Mgr *m, Ent *e) {
    if (FIFORemove(&m->querylist, e) == 0) {
        FIFORemove(&m->pendinglist, e);
    }
}

u32 keyrand(Cipher *c, u32 n, u8 *key, u32 keylen, u8 *j, u32 *idx) {
    u32 r;
    u32 mask;
    u32 cnt;
    if (n == 0) {
        return 0;
    }
    cnt = 0;
    mask = 1;
    if (n > 1) {
        do {
            mask = mask * 2 + 1;
        } while (mask < n);
    }
    do {
        u32 t = (*idx)++;
        *j = c->cards[*j] + key[t];
        if (*idx >= keylen) {
            *idx = 0;
            *j = *j + keylen;
        }
        r = mask & *j;
        cnt++;
        if (cnt > 11) {
            r = DS_UMOD(r, n);
        }
    } while (r > n);
    return (u8)r;
}

void GOAHashInit(Cipher *c) {
    s32 i;
    s32 v;
    c->rotor = 1;
    c->ratchet = 3;
    c->avalanche = 5;
    c->last_plain = 7;
    c->last_cipher = 0xb;
    for (i = 0, v = 0xff; i < 0x100; i++, v--) {
        c->cards[i] = v;
    }
}

void GOACryptInit(Cipher *c, u8 *key, u32 keylen) {
    s32 i;
    u32 idx;
    u8 j;
    if (keylen < 1) {
        GOAHashInit(c);
        return;
    }
    for (i = 0; i < 0x100; i++) {
        c->cards[i] = i;
    }
    idx = 0;
    j = 0;
    for (i = 0xff; i >= 0; i--) {
        u32 r = keyrand(c, i, key, keylen, &j, &idx);
        u8 t = c->cards[i];
        c->cards[i] = c->cards[r];
        c->cards[r] = t;
    }
    c->rotor = c->cards[1];
    c->ratchet = c->cards[3];
    c->avalanche = c->cards[5];
    c->last_plain = c->cards[7];
    c->last_cipher = c->cards[j];
    j = 0;
    idx = 0;
}

u32 GOADecryptByte(Cipher *c, u32 x) {
    u8 a = c->rotor;
    c->rotor = a + 1;
    c->ratchet = c->ratchet + c->cards[a];
    u8 t = c->last_cipher;
    u8 old = c->cards[t];
    c->cards[t] = c->cards[c->ratchet];
    c->cards[c->ratchet] = c->cards[c->last_plain];
    c->cards[c->last_plain] = c->cards[c->rotor];
    c->cards[c->rotor] = old;
    c->avalanche = c->avalanche + c->cards[old];
    u32 v = x ^ c->cards[(c->cards[c->avalanche] + c->cards[c->rotor]) & 0xff];
    v ^= c->cards[c->cards[(c->cards[c->ratchet] + (c->cards[c->last_plain] + c->cards[c->last_cipher])) & 0xff]];
    c->last_plain = v;
    c->last_cipher = x;
    return c->last_plain;
}

void GOADecrypt(Cipher *c, u8 *buf, s32 n) {
    s32 i;
    for (i = 0; i < n; i++) {
        buf[i] = GOADecryptByte(c, buf[i]);
    }
}
}
