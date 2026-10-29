// mwcc-flags: -O4,p -str reuse
#include "ds_div.h"
#include "types.h"
#include "net/gpiSearch.h"
#include "net/gpiProfile.h"
#include "net/gpi.h"
#include "net/gpiOperation.h"
#include "net/SockAddrIn.h"
#include "net/SockHostEnt.h"
#include "net/gpersist.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_02281a5c.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_0227df0c.h"
#include "proto/ov065/unk_ov065_0227e160.h"
#include "proto/ov065/unk_ov065_02280740.h"
#include "proto/ov065/unk_ov065_02283304.h"
#include "proto/ov065/unk_ov065_02278328.h"
extern "C" {
int strncmp(const char *a, const char *b, u32 n);
}

// ov065 TU49: GP gpiSearch.c (0x02281a5c..0x02283304)

namespace Nb {











extern "C" {
void _strlwr(char *);
s32 gpiProcess(void *, s32);
s32 socket(s32, s32, s32);
s32 SetSockBlocking(s32, s32);
SockHostEnt *Sock_GetHostByName(const char *);
s32 connect(s32, void *, s32);
s32 GOAGetLastError(s32);
s32 gpiPeerStartTransferMessage(void *, s32, s32, void *);
s32 gpiSendOrBufferString(void *, s32, char *);
s32 gpiPeerFinishTransferMessage(void *, s32, const char *, s32);
s32 GSISocketSelect(s32, s32, s32 *, s32 *);
char *strncpy(char *dst, const char *src, u32 n);
s32 OS_SPrintf(char *buf, const char *fmt, ...);

s32 gpiInitSearchData(void *, void *, s32);
s32 gpiStartSearch(void *, void *, s32, GPCallback, void *);
s32 gpiStartProfileSearch(void *, void *);
}

}

namespace Na {



extern "C" {

typedef s32 (*gpiProfileMapFunc)(GPConnection *, GPIProfile *, void *);

void gpiFreeInfoCache(void *);

s32 gpiProcessSearch(GPConnection *, GPIOperation *);
s32 gpiCheckForBuddy(GPConnection *, GPIProfile *, void *);
s32 gpiProfileMapCallback(GPIProfile *, void *);
s32 gpiCheckProfileForUser(GPConnection *, GPIProfile *, void *);
s32 gpiGetProfile(GPConnection *, s32, void *);
void gpiProfilesTableFree(void *);
s32 gpiProfileMap(GPConnection *, gpiProfileMapFunc, void *);
s32 gpiProfilesTableCompare(void *, void *);
s32 gpiProfilesTableHash(void *, s32);


void gpiAppendStringToBuffer(GPConnection *, char **, const char *);
void gpiAppendIntToBuffer(GPConnection *, char **, s32);
s32 gpiSendFromBuffer(GPConnection *, s32, char **, s32 *, s32, const char *);
s32 gpiRecvToBuffer(GPConnection *, s32, char **, s32 *, s32 *, const char *);
void msleep(s32);
s32 gpiProfileSearch(GPConnection *, char *, char *, char *, char *, char *, s32, s32, s32, GPCallback, void *);










#define ERR3() { gpiSetError(h, 1, "Error reading from the search server."); gpiCallErrorCallback(h, 3, 1); return 3; }
#define ERRMEM(m) { gpiSetErrorString(h, m); return 1; }
#define GETTOK(t) r = gpiReadKeyAndValue(h, c->inputBuffer, &pos, t, buf); if (r != 0) { return r; }

}
}

namespace Nb {
extern "C" {

char data_ov065_0228dadc[0x40] = "gpsp.gs.nintendowifi.net";
}
}
namespace Nb {
extern "C" {
s32 gpiStartProfileSearch(void *h0, void *o0) {
    GPConnection *h = (GPConnection *)h0;
    GPIOperation *o = (GPIOperation *)o0;
    GPISearchData *s = (GPISearchData *)o->data;
    SockHostEnt *ent;
    SockAddrIn sa;
    s32 r;
    s32 m;
    s->inputBufferCapacity = 0x1000;
    s->inputBuffer = (char *)GsUtil_Alloc(s->inputBufferCapacity + 1);
    if (s->inputBuffer == NULL) {
        gpiSetErrorString(h, "Out of memory.");
        return 1;
    }
    s->sock = socket(2, 1, 0);
    if (s->sock == -1) {
        gpiSetError(h, 5, "There was an error creating a socket.");
        gpiCallErrorCallback(h, 3, 1);
        return 3;
    }
    if (SetSockBlocking(s->sock, 0) == 0) {
        gpiSetError(h, 5, "There was an error making a socket non-blocking.");
        gpiCallErrorCallback(h, 3, 1);
        return 3;
    }
    ent = Sock_GetHostByName(data_ov065_0228dadc);
    if (ent == NULL) {
        gpiSetError(h, 5, "Could not resolve search mananger host name.");
        gpiCallErrorCallback(h, 3, 1);
        return 3;
    }
    u32 *w = (u32 *)&sa;
    w[0] = 0;
    w[1] = 0;
    sa.family = 2;
    sa.addr = *(u32 *)*ent->addrList;
    sa.port = 0xcd74;
    if (connect(s->sock, &sa, 8) == -1) {
        r = GOAGetLastError(s->sock);
        if (r != -6 && r != -0x1a && r != -0x4c) {
            gpiSetError(h, 5, "There was an error connecting a socket.");
            gpiCallErrorCallback(h, 3, 1);
            return 3;
        }
    }
    o->state = 1;
    return 0;
}

s32 gpiInitSearchData(void *h0, void *out, s32 p2) {
    GPISearchData *cn;
    GPConnection *h = (GPConnection *)h0;
    cn = (GPISearchData *)GsUtil_Alloc(sizeof(GPISearchData));
    if (cn == NULL) {
        gpiSetErrorString(h, "Out of memory.");
        return 1;
    }
    memset(cn, 0, sizeof *cn);  // (0x144 on the DS)
    cn->type = p2;
    cn->sock = -1;
    cn->inputBuffer = 0;
    cn->inputBufferLength = 0;
    cn->inputBufferPos = 0;
    cn->inputBufferCapacity = 0;
    cn->outputBufferLength = 0;
    cn->outputBufferPos = 0;
    cn->outputBufferCapacity = 0x1000;
    cn->outputBuffer = (char *)GsUtil_Alloc(cn->outputBufferCapacity + 1);
    if (cn->outputBuffer == NULL) {
        gpiSetErrorString(h, "Out of memory.");
        return 1;
    }
    cn->processing = 0;
    cn->remove = 0;
    *(GPISearchData **)out = cn;
    return 0;
}

s32 gpiStartSearch(void *h0, void *cn, s32 p2, GPCallback p3, void *p4) {
    GPConnection *h = (GPConnection *)h0;
    GPIOperation *o;
    s32 r;
    (*h)->numSearches += 1;
    r = gpiAddOperation(h, 3, cn, &o, p2, p3, p4);
    if (r != 0) {
        return r;
    }
    r = gpiStartProfileSearch(h, o);
    if (r != 0) {
        return r;
    }
    if (o->blocking != 0) {
        r = gpiProcess(h, o->id);
        if (r != 0) {
            return r;
        }
    }
    return 0;
}

s32 gpiProfileSearch(GPConnection *h, char *a, char *b, char *c, char *d, char *e, s32 f, s32 g, s32 p8, GPCallback p9, void *p10) {
    GPISearchData *cn;
    s32 r;
    if ((a == NULL || *a == 0) && (c == NULL || *c == 0) && (d == NULL || *d == 0) && (e == NULL || *e == 0) && f == 0 && (b == NULL || *b == 0)) {
        gpiSetErrorString(h, "No search criteria.");
        return 2;
    }
    r = gpiInitSearchData(h, &cn, 1);
    if (r != 0) {
        return r;
    }
    if (a == NULL) {
        cn->nick[0] = 0;
    } else {
        strzcpy(cn->nick, a, 0x1f);
    }
    if (b == NULL) {
        cn->uniquenick[0] = 0;
    } else {
        strzcpy(cn->uniquenick, b, 0x15);
    }
    if (c == NULL) {
        cn->email[0] = 0;
    } else {
        strzcpy(cn->email, c, 0x33);
    }
    _strlwr(cn->email);
    if (d == NULL) {
        cn->firstname[0] = 0;
    } else {
        strzcpy(cn->firstname, d, 0x1f);
    }
    if (e == NULL) {
        cn->lastname[0] = 0;
    } else {
        strzcpy(cn->lastname, e, 0x1f);
    }
    cn->icquin = f;
    if (g < 0) {
        g = 0;
    }
    cn->skip = g;
    r = gpiStartSearch(h, cn, p8, p9, p10);
    if (r != 0) {
        return r;
    }
    return 0;
}

}
}

namespace Na {
extern "C" {
s32 gpiProcessSearch(GPConnection *h, GPIOperation *node) {
    s32 done, save1;
    GPIConnection *ctx = *h;
    s32 done2;
    GPFindPlayersResponseArg *p4;
    GPGetReverseBuddiesResponseArg *p7;
    s32 cnt;
    s32 retry;
    GPISearchData *c = (GPISearchData *)node->data;
    s32 r;
    s32 v8c;
    s32 pos;
    GPICallbackCopy pr1;
    s32 vv[2];
    GPICallbackCopy pr8, pr7, pr6, pr5, pr4, pr3, pr2;
    GPProfileSearchResponseArg s1;
    char tok[0x200];
    char buf[0x200];

    if (node->blocking != 0) {
        retry = 1;
    } else {
        retry = 0;
    }
again:
    r = gpiSendFromBuffer(h, c->sock, &c->outputBuffer, &vv[1], 1, "SM");
    if (r != 0) {
        return r;
    }
    if (node->state == 1) {
        r = gpiCheckSocketConnect(h, c->sock, &v8c);
        if (r != 0) {
            return r;
        }
        if (v8c == 4) {
            gpiSetError(h, 0xd01, "Could not connect to the search manager.");
            gpiCallErrorCallback(h, 4, 0);
            return 4;
        }
        if (v8c != 3) {
            goto endchk;
        }
        if (c->type == 1) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\search\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sesskey\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->sessKey);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\profileid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->profileid);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\namespaceid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->namespaceID);
            if (c->nick[0] != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\nick\\");
                gpiAppendStringToBuffer(h, &c->outputBuffer, c->nick);
            }
            if (c->uniquenick[0] != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\uniquenick\\");
                gpiAppendStringToBuffer(h, &c->outputBuffer, c->uniquenick);
            }
            if (c->email[0] != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\email\\");
                gpiAppendStringToBuffer(h, &c->outputBuffer, c->email);
            }
            if (c->firstname[0] != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\firstname\\");
                gpiAppendStringToBuffer(h, &c->outputBuffer, c->firstname);
            }
            if (c->lastname[0] != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\lastname\\");
                gpiAppendStringToBuffer(h, &c->outputBuffer, c->lastname);
            }
            if (c->icquin != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\icquin\\");
                gpiAppendIntToBuffer(h, &c->outputBuffer, c->icquin);
            }
            if (c->skip > 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\skip\\");
                gpiAppendIntToBuffer(h, &c->outputBuffer, c->skip);
            }
        } else if (c->type == 2) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\valid\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\email\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->email);
        } else if (c->type == 3) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\nicks\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\email\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->email);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\pass\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->password);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\namespaceid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->namespaceID);
        } else if (c->type == 4) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\pmatch\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sesskey\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->sessKey);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\profileid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->profileid);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\productid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, c->productID);
        } else if (c->type == 5) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\check\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\nick\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->nick);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\email\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->email);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\pass\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->password);
        } else if (c->type == 6) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\newuser\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\nick\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->nick);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\email\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->email);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\pass\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->password);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\productID\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->productID);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\namespaceid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->namespaceID);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\uniquenick\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->uniquenick);
            if (c->cdkey[0] != 0) {
                gpiAppendStringToBuffer(h, &c->outputBuffer, "\\cdkey\\");
                gpiAppendStringToBuffer(h, &c->outputBuffer, c->cdkey);
            }
        } else if (c->type == 7) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\others\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\sesskey\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->sessKey);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\profileid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->profileid);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\namespaceid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->namespaceID);
        } else if (c->type == 8) {
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\uniquesearch\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\preferrednick\\");
            gpiAppendStringToBuffer(h, &c->outputBuffer, c->uniquenick);
            gpiAppendStringToBuffer(h, &c->outputBuffer, "\\namespaceid\\");
            gpiAppendIntToBuffer(h, &c->outputBuffer, ctx->namespaceID);
        }
        gpiAppendStringToBuffer(h, &c->outputBuffer, "\\gamename\\");
        gpiAppendStringToBuffer(h, &c->outputBuffer, __GSIACGamename);
        gpiAppendStringToBuffer(h, &c->outputBuffer, "\\final\\");
        node->state = 4;
        goto endchk;
    }
    if (node->state != 4) {
        goto endchk;
    }
    r = gpiRecvToBuffer(h, c->sock, &c->inputBuffer, &vv[0], &vv[1], "SM");
    if (r != 0) {
        if (r != 3) {
            return r;
        }
        gpiSetError(h, 0xd01, "There was an error reading from the server.");
        gpiCallErrorCallback(h, 3, 0);
        return 3;
    }
    if (strstr(c->inputBuffer, "\\final\\") == 0) {
        goto endchk;
    }
    pos = 0;
    node->state = 5;
    if (gpiCheckForError(h, c->inputBuffer, 1) != 0) {
        c->remove = 1;
        return 4;
    }
    if (c->type == 1) {
        done = 0;
        s1.result = 0;
        s1.numMatches = 0;
        s1.matches = 0;
        s1.more = 0x601;
        do {
            GETTOK(tok)
            if (strcmp(tok, "bsrdone") == 0) {
                GETTOK(tok)
                if (strcmp(tok, "more") == 0) {
                    if (strcmp(buf, "0") != 0) {
                        s1.more = 0x600;
                    }
                }
                done = 1;
            } else if (strcmp(tok, "bsr") == 0) {
                GPProfileSearchMatch *e;
                s32 idx;
                GPProfileSearchMatch *base;
                s1.numMatches++;
                base = (GPProfileSearchMatch *)GsUtil_Realloc(s1.matches, s1.numMatches * sizeof(GPProfileSearchMatch));
                s1.matches = base;
                if (base == 0) ERRMEM("Out of memory.")
                idx = s1.numMatches - 1;
                e = &base[idx];
                memset(e, 0, 0xac);
                base[idx].profile = atol(buf);
                done2 = 0;
                do {
                    save1 = pos;
                    GETTOK(tok)
                    if (strcmp(tok, "nick") == 0) {
                        strzcpy(e->nick, buf, 0x1f);
                    } else if (strcmp(tok, "uniquenick") == 0) {
                        strzcpy(e->uniquenick, buf, 0x15);
                    } else if (strcmp(tok, "firstname") == 0) {
                        strzcpy(e->firstname, buf, 0x1f);
                    } else if (strcmp(tok, "lastname") == 0) {
                        strzcpy(e->lastname, buf, 0x1f);
                    } else if (strcmp(tok, "email") == 0) {
                        strzcpy(e->email, buf, 0x33);
                    } else if (strcmp(tok, "bsr") == 0 || strcmp(tok, "bsrdone") == 0) {
                        done2 = 1;
                        pos = save1;
                    }
                } while (done2 == 0);
            } else {
                ERR3()
            }
        } while (done == 0);
        {
            s32 t = s1.more;
            pr1 = node->callback;
            if (pr1.p.callback != 0) {
                pr1.p.callback(h, &s1, pr1.p.param);
            }
            if (t == 0x600 && s1.more == 0x600) {
                r = gpiProfileSearch(h, c->nick, c->uniquenick, c->email, c->firstname, c->lastname, c->icquin, s1.numMatches + c->skip, node->blocking, node->callback.p.callback, node->callback.p.param);
                if (r != 0) {
                    return r;
                }
            }
        }
        GsUtil_Free(s1.matches);
        s1.matches = 0;
        goto done;
    } else if (c->type == 2) {
        GPIsValidEmailResponseArg *p;
        pr2 = node->callback;
        if (pr2.p.callback == 0) {
            goto done;
        }
        GETTOK(tok)
        if (strcmp(tok, "vr") != 0) ERR3()
        p = (GPIsValidEmailResponseArg *)GsUtil_Alloc(sizeof(GPIsValidEmailResponseArg));
        if (p == 0) ERRMEM("Out of memory.")
        p->result = 0;
        strzcpy(p->email, c->email, 0x33);
        if (buf[0] == 0x30) {
            p->isValid = 0;
        } else {
            p->isValid = 1;
        }
        r = gpiAddCallback(h, pr2, p, node, 0);
        if (r == 0) {
            goto done;
        }
        return r;
    } else if (c->type == 3) {
        GPGetUserNicksResponseArg *p;
        pr3 = node->callback;
        if (pr3.p.callback == 0) {
            goto done;
        }
        p = (GPGetUserNicksResponseArg *)GsUtil_Alloc(sizeof(GPGetUserNicksResponseArg));
        if (p == 0) ERRMEM("Out of memory.")
        p->result = 0;
        STD_CopyString(p->email, c->email);
        p->numNicks = 0;
        p->nicks = 0;
        p->uniquenicks = 0;
        GETTOK(tok)
        if (strcmp(tok, "nr") != 0) ERR3()
        done = 0;
        do {
            GETTOK(tok)
            if (strcmp(tok, "nick") == 0) {
                void *t = GsUtil_Realloc(p->nicks, (p->numNicks + 1) * sizeof(char *));
                if (t == 0) ERRMEM("Out of memory.")
                p->nicks = (char **)t;
                t = GsUtil_Alloc(0x1f);
                if (t == 0) ERRMEM("Out of memory.")
                p->nicks[p->numNicks] = (char *)t;
                strzcpy(p->nicks[p->numNicks], buf, 0x1f);
                p->numNicks++;
            } else if (strcmp(tok, "uniquenick") == 0) {
                if (p->numNicks > 0) {
                    void *t = GsUtil_Realloc(p->uniquenicks, p->numNicks * sizeof(char *));
                    if (t == 0) ERRMEM("Out of memory.")
                    p->uniquenicks = (char **)t;
                    t = GsUtil_Alloc(0x15);
                    if (t == 0) ERRMEM("Out of memory.")
                    p->uniquenicks[p->numNicks - 1] = (char *)t;
                    strzcpy(p->uniquenicks[p->numNicks - 1], buf, 0x15);
                }
            } else if (strcmp(tok, "ndone") == 0) {
                done = 1;
            } else {
                ERR3()
            }
        } while (done == 0);
        r = gpiAddCallback(h, pr3, p, node, 3);
        if (r == 0) {
            goto done;
        }
        return r;
    } else if (c->type == 4) {
        pr4 = node->callback;
        if (pr4.p.callback == 0) {
            goto done;
        }
        p4 = (GPFindPlayersResponseArg *)GsUtil_Alloc(sizeof(GPFindPlayersResponseArg));
        if (p4 == 0) ERRMEM("Out of memory.")
        p4->productID = c->productID;
        done = 0;
        p4->result = 0;
        p4->numMatches = 0;
        p4->matches = 0;
        do {
            GETTOK(tok)
            if (strcmp(tok, "psrdone") == 0) {
                done = 1;
            } else if (strcmp(tok, "psr") == 0) {
                GPFindPlayerMatch *e;
                s32 idx;
                GPFindPlayerMatch *base;
                p4->numMatches++;
                p4->matches = (GPFindPlayerMatch *)GsUtil_Realloc(p4->matches, p4->numMatches * sizeof(GPFindPlayerMatch));
                base = p4->matches;
                if (base == 0) ERRMEM("Out of memory.")
                idx = p4->numMatches - 1;
                e = &base[idx];
                memset(e, 0, 0x128);
                e->status = 1;
                base[idx].profile = atol(buf);
                done2 = 0;
                do {
                    save1 = pos;
                    GETTOK(tok)
                    if (strcmp(tok, "status") == 0) {
                        strzcpy(e->statusString, buf, 0x100);
                    } else if (strcmp(tok, "nick") == 0) {
                        strzcpy(e->nick, buf, 0x1f);
                    }
                    if (strcmp(tok, "statuscode") == 0) {
                        e->status = atol(buf);
                    } else if (strcmp(tok, "psr") == 0 || strcmp(tok, "psrdone") == 0) {
                        done2 = 1;
                        pos = save1;
                    }
                } while (done2 == 0);
            } else {
                ERR3()
            }
        } while (done == 0);
        r = gpiAddCallback(h, pr4, p4, node, 4);
        if (r == 0) {
            goto done;
        }
        return r;
    } else if (c->type == 5) {
        s32 a4;
        s32 a6;
        GPCheckResponseArg *p;
        pr5 = node->callback;
        if (pr5.p.callback == 0) {
            goto done;
        }
        GETTOK(tok)
        if (strcmp(tok, "cur") != 0) ERR3()
        a4 = atol(buf);
        if (a4 != 0) {
            ctx->errorCode = a4;
            a6 = 0;
        } else {
            if (gpiValueForKey(c->inputBuffer, "\\pid\\", buf, 0x200) == 0) ERR3()
            a6 = atol(buf);
        }
        p = (GPCheckResponseArg *)GsUtil_Alloc(sizeof(GPCheckResponseArg));
        if (p == 0) ERRMEM("Out of memory.")
        p->result = a4;
        p->profile = a6;
        r = gpiAddCallback(h, pr5, p, node, 0);
        if (r == 0) {
            goto done;
        }
        return r;
    } else if (c->type == 6) {
        s32 a4;
        s32 a6;
        GPCheckResponseArg *p;
        pr6 = node->callback;
        if (pr6.p.callback == 0) {
            goto done;
        }
        GETTOK(tok)
        if (strcmp(tok, "nur") != 0) ERR3()
        a4 = atol(buf);
        if (a4 != 0) {
            ctx->errorCode = a4;
        }
        if (gpiValueForKey(c->inputBuffer, "\\pid\\", buf, 0x200) == 0) {
            if (a4 == 0) ERR3()
            a6 = 0;
        } else {
            a6 = atol(buf);
        }
        p = (GPCheckResponseArg *)GsUtil_Alloc(sizeof(GPCheckResponseArg));
        if (p == 0) ERRMEM("Out of memory.")
        p->result = a4;
        p->profile = a6;
        r = gpiAddCallback(h, pr6, p, node, 0);
        if (r == 0) {
            goto done;
        }
        return r;
    } else if (c->type == 7) {
        pr7 = node->callback;
        if (pr7.p.callback == 0) {
            goto done;
        }
        p7 = (GPGetReverseBuddiesResponseArg *)GsUtil_Alloc(sizeof(GPGetReverseBuddiesResponseArg));
        if (p7 == 0) ERRMEM("Out of memory.")
        p7->result = 0;
        p7->numProfiles = 0;
        p7->profiles = 0;
        GETTOK(tok)
        if (strcmp(tok, "others") != 0) ERR3()
        done = 0;
        do {
            GETTOK(tok)
            if (strcmp(tok, "odone") == 0) {
                done = 1;
            } else if (strcmp(tok, "o") == 0) {
                GPProfileSearchMatch *e;
                s32 idx;
                GPProfileSearchMatch *base;
                void *t = GsUtil_Realloc(p7->profiles, (p7->numProfiles + 1) * sizeof(GPProfileSearchMatch));
                if (t == 0) ERRMEM("Out of memory.")
                p7->profiles = (GPProfileSearchMatch *)t;
                base = p7->profiles;
                idx = p7->numProfiles;
                e = &base[idx];
                memset(e, 0, 0xac);
                p7->numProfiles++;
                base[idx].profile = atol(buf);
                done2 = 0;
                do {
                    save1 = pos;
                    GETTOK(tok)
                    if (strcmp(tok, "nick") == 0) {
                        strzcpy(e->nick, buf, 0x1f);
                    } else if (strcmp(tok, "uniquenick") == 0) {
                        strzcpy(e->uniquenick, buf, 0x15);
                    } else if (strcmp(tok, "first") == 0) {
                        strzcpy(e->firstname, buf, 0x1f);
                    } else if (strcmp(tok, "last") == 0) {
                        strzcpy(e->lastname, buf, 0x1f);
                    } else if (strcmp(tok, "email") == 0) {
                        strzcpy(e->email, buf, 0x33);
                    } else if (strcmp(tok, "o") == 0 || strcmp(tok, "odone") == 0) {
                        done2 = 1;
                        pos = save1;
                    }
                } while (done2 == 0);
            } else {
                ERR3()
            }
        } while (done == 0);
        r = gpiAddCallback(h, pr7, p7, node, 8);
        if (r == 0) {
            goto done;
        }
        return r;
    } else if (c->type == 8) {
        GPSuggestUniqueNickResponseArg *p;
        pr8 = node->callback;
        if (pr8.p.callback == 0) {
            goto done;
        }
        cnt = 0;
        p = (GPSuggestUniqueNickResponseArg *)GsUtil_Alloc(sizeof(GPSuggestUniqueNickResponseArg));
        if (p == 0) ERRMEM("Out of memory.")
        p->result = 0;
        p->numSuggestedNicks = 0;
        p->suggestedNicks = 0;
        GETTOK(tok)
        if (strcmp(tok, "us") != 0) ERR3()
        p->numSuggestedNicks = atol(buf);
        p->suggestedNicks = (char **)GsUtil_Alloc(p->numSuggestedNicks * sizeof(char *));
        if (p->suggestedNicks == 0) ERRMEM("Out of memory.")
        done = 0;
        do {
            GETTOK(tok)
            if (strcmp(tok, "nick") == 0) {
                p->suggestedNicks[cnt] = (char *)GsUtil_Alloc(0x15);
                if (p->suggestedNicks[cnt] == 0) ERRMEM("Out of memory.")
                strzcpy(p->suggestedNicks[cnt], buf, 0x15);
                cnt++;
            } else if (strcmp(tok, "usdone") == 0) {
                p->numSuggestedNicks = cnt;
                done = 1;
            } else {
                ERR3()
            }
        } while (done == 0);
        r = gpiAddCallback(h, pr8, p, node, 9);
        if (r != 0) {
            return r;
        }
    }
done:
    c->remove = 1;
    retry = 0;
endchk:
    if (retry != 0) {
        msleep(10);
    }
    if (retry != 0) {
        goto again;
    }
    return 0;
}

s32 gpiProcessSearches(GPConnection *h) {
    GPIConnection *c = *h;
    s32 n = 0;
    s32 i;
    GPIOperation **arr;
    GPIOperation *nd;
    s32 z = 0;
    if (c->numSearches > 0) {
        arr = (GPIOperation **)GsUtil_Alloc(c->numSearches * sizeof(GPIOperation *));
        if (arr == 0) {
            gpiSetErrorString(h, "Out of memory.");
            return 1;
        }
        for (nd = c->operationList; nd != 0; nd = nd->pnext) {
            if (nd->type == 3 && nd->state != 5 && ((GPISearchData *)nd->data)->processing == 0) {
                arr[n++] = nd;
                ((GPISearchData *)nd->data)->processing = 1;
            }
        }
        for (i = 0; i < n; i++) {
            s32 r = gpiProcessSearch(h, arr[i]);
            if (r != 0) {
                arr[i]->result = r;
            }
        }
        for (i = 0; i < n; i++) {
            GPISearchData *s = (GPISearchData *)arr[i]->data;
            s->processing = z;
            if (s->remove != 0) {
                gpiRemoveOperation(h, arr[i]);
            }
        }
        GsUtil_Free(arr);
    }
    return 0;
}

s32 gpiProfilesTableHash(void *elem, s32 n) { // a TableHashFn over GPIProfile elements
    return DS_MOD(((GPIProfile *)elem)->profileId, n);
}

s32 gpiProfilesTableCompare(void *a, void *b) { // an ArrayCompareFn over GPIProfile elements
    return ((GPIProfile *)a)->profileId - ((GPIProfile *)b)->profileId;
}

void gpiProfilesTableFree(void *p) {
    GPIProfile *e = (GPIProfile *)p;
    if (e->buddyStatus != 0) {
        GsUtil_Free(e->buddyStatus->statusString);
        e->buddyStatus->statusString = 0;
        GsUtil_Free(e->buddyStatus->locationString);
        e->buddyStatus->locationString = 0;
        GsUtil_Free(e->buddyStatus);
        e->buddyStatus = 0;
    }
    gpiFreeInfoCache(e);
    GsUtil_Free(e->authSig);
    e->authSig = 0;
    GsUtil_Free(e->peerSig);
    e->peerSig = 0;
}

s32 gpiInitProfiles(GPConnection *h) {
    GPIConnection *c = *h;
    c->numBuddies = 0;
    c->numProfiles = 0;
    c->profileTable = TableNew(sizeof(GPIProfile), 4, gpiProfilesTableHash, gpiProfilesTableCompare, gpiProfilesTableFree);
    if (c->profileTable != 0) {
        return 1;
    }
    return 0;
}

}
}

// ---- overlay variables (generated: tools/host_overlay_vars.py) ----
#include "host_overlay_vars.h"

HOST_OVERLAY_VARS(unk_ov065_02281a5c) {
    HOST_OVERLAY_VAR(Nb::data_ov065_0228dadc),
    HOST_OVERLAY_VARS_END
};
