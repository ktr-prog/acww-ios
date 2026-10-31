// mwcc-flags: -O4,p -str reuse
#include "types.h"
#include "net/gpiProfile.h"
#include "net/gpiOperation.h"
#include "net/gpiCallback.h"
#include "net/SockAddrIn.h"
#include "net/SockHostEnt.h"
#include "net/gpiInfo.h"
#include "net/gp.h"
#include "net/gpiTransfer.h"
#include "net/gpi.h"
#include "net/gpiPeer.h"
#include "net/gsPlatformUtil.h"
#include "proto/autoload_2/unk_0212703c.h"
#include "proto/autoload_2/unk_02128030.h"
#include "proto/autoload_2/unk_02128c60.h"
#include "proto/autoload_2/unk_0212b770.h"
#include "proto/ov065/unk_ov065_02277974.h"
#include "proto/ov065/unk_ov065_0227f2a4.h"
#include "proto/ov065/unk_ov065_02283720.h"
#include "proto/ov065/unk_ov065_0227d96c.h"
#include "proto/ov065/unk_ov065_0227df0c.h"
#include "proto/ov065/unk_ov065_0227e160.h"
#include "proto/ov065/unk_ov065_02280740.h"
#include "proto/ov065/unk_ov065_0228176c.h"
#include "proto/ov065/unk_ov065_02283304.h"

// ov065 TU45: GP gpiInfo.c (0x0227f2a4..0x02280740)

namespace Na {
// ov065_052: DWC/GameSpy GP connection setup helpers (0x0227ee64..0x0227f54c)

extern "C" {

extern char data_ov065_0228d1a4[];
extern char data_ov065_0228d370[];
extern char data_ov065_0228d428[];
extern char data_ov065_0228d43c[];
extern char data_ov065_0228d450[];
extern char data_ov065_0228d478[];
extern char data_ov065_0228d4ac[];
extern char data_ov065_0228d4d4[];
extern char data_ov065_0228d500[];
extern char data_ov065_0228d530[];
extern char data_ov065_0228d564[];
extern char data_ov065_0228d58c[];
extern u8 data_0213a490[];

s32 gpiReset(GPConnection *);
void _strlwr(char *);
char *goastrdup(const char *);
s32 gpiAddOperation(GPConnection *, s32, void *, GPIOperation **, s32, GPCallback, void *);
void gpiDisconnect(GPConnection *, s32);
s32 gpiProcess(GPConnection *, s32);
s32 socket(s32, s32, s32);
s32 bind(s32, void *, s32);
s32 listen(s32, s32);
s32 getsockname(s32, void *, s32 *);
s32 connect(s32, void *, s32);
s32 GOAGetLastError(s32);
s32 SetSockBlocking(s32, s32);
SockHostEnt *Sock_GetHostByName(char *);


s32 gpiSendGetInfo(GPConnection *, s32, s32);
s32 gpiStartConnect(GPConnection *, GPIOperation *);

#define GP_FAIL(str) \
    { \
        gpiSetError(h, 5, str); \
        gpiCallErrorCallback(h, 3, 1); \
        return 3; \
    }








#define CK_NONEMPTY \
    if (*val == 0) { \
        gpiSetErrorString(h, "Invalid value."); \
        return 2; \
    }


}
extern "C" {
void gpiFreeInfoCache(GPIProfile *p);
s32 gpiGetInfo(GPConnection *h, s32 a1, s32 a2, s32 a3, GPCallback a4, void *a5);
s32 gpiSendGetInfo(GPConnection *h, s32 a1, s32 a2);
s32 gpiSetInfos(GPConnection *h, s32 cmd, char *val);
}
}

namespace Nb {
// ov065_053: DWC/GameSpy-like response builder / parser (0x0227faf8..0x0227ff90)




extern "C" {

s32 strncmp(const char *, const char *, s32);
s32 OS_SPrintf(char *, const char *, ...);
char *goastrdup(const char *);
s32 gpiIntToDate(void *, s32, s32 *, s32 *, s32 *);
s32 gpiValueForKey(const char *, const char *, char *, s32);
s32 gpiCheckForError(void *, const char *, s32);







#define FIND(key, dst, n) gpiValueForKey(str, key, dst, n)


}
extern "C" {
s32 gpiAddLocalInfo(GPConnection *h, GPIBuffer *p);
}
}

namespace Nc {
// ov065_054: 0x022804b8..0x02280d70












extern "C" {
s32 shutdown(s32, s32);
s32 closesocket(s32);
s32 OS_SPrintf(char *, const char *, ...);
void ArrayAppend(void *, void *);
s32 socket(s32, s32, s32);
s32 SetSockBlocking(s32, s32);
void GsGpPeer_SetSocketBuffers(s32);
s32 connect(s32, void *, s32);
s32 GOAGetLastError(s32);

extern char data_ov065_0228d894[];
extern char data_ov065_0228d8dc[];
extern char data_ov065_0228d8ec[];
extern char data_ov065_0228d8f0[];
extern char data_ov065_0228d900[];
extern char data_ov065_0228d914[];
extern char data_ov065_0228d918[];
extern char data_ov065_0228d920[];
extern char data_ov065_0228d928[];
extern char data_ov065_0228d944[];
extern char data_ov065_0228d96c[];
extern char data_ov065_0228d9a0[];


s32 gpiIsValidDate(s32 day, s32 mon, s32 year);






void gpiDestroyOperation(GPConnection *h, GPIOperation *n);




// byte-sized unsigned enum: an enum-typed zero is not constant-folded/shared with later zeros
#pragma enumsalwaysint off
enum Unk_ov065_02280a2c_Z { Unk_ov065_02280a2c_Z_0 = 0, Unk_ov065_02280a2c_Z_FF = 0xff };
#pragma enumsalwaysint reset










}
extern "C" {
s32 gpiIntToDate(void *ctx, s32 packed, s32 *pa, s32 *pb, s32 *pc);
}
}

namespace Nc {
extern "C" {
s32 gpiIntToDate(void *ctx, s32 packed, s32 *pa, s32 *pb, s32 *pc) {
    s32 a = (packed >> 24) & 0xff;
    s32 b = (packed >> 16) & 0xff;
    s32 c = packed & 0xffff;
    if (gpiIsValidDate(a, b, c) == 0) {
        gpiSetErrorString(ctx, "Invalid date.");
        return 2;
    }
    *pa = a;
    *pb = b;
    *pc = c;
    return 0;
}
}
}

namespace Nc {
extern "C" {
void gpiInfoCacheToArg(GPIInfoCache *s, GPGetInfoResponseArg *d) {
    if (s->nick) {
        strzcpy(d->nick, s->nick, 0x1f);
    } else {
        d->nick[0] = 0;
    }
    if (s->uniquenick) {
        strzcpy(d->uniquenick, s->uniquenick, 0x15);
    } else {
        d->uniquenick[0] = 0;
    }
    if (s->email) {
        strzcpy(d->email, s->email, 0x33);
    } else {
        d->email[0] = 0;
    }
    if (s->firstname) {
        strzcpy(d->firstname, s->firstname, 0x1f);
    } else {
        d->firstname[0] = 0;
    }
    if (s->lastname) {
        strzcpy(d->lastname, s->lastname, 0x1f);
    } else {
        d->lastname[0] = 0;
    }
    if (s->homepage) {
        strzcpy(d->homepage, s->homepage, 0x4c);
    } else {
        d->homepage[0] = 0;
    }
    d->icquin = s->icquin;
    strzcpy(d->zipcode, s->zipcode, 0xb);
    strzcpy(d->countrycode, s->countrycode, 3);
    d->longitude = s->longitude;
    d->latitude = s->latitude;
    if (s->place) {
        strzcpy(d->place, s->place, 0x80);
    } else {
        d->place[0] = 0;
    }
    d->birthday = s->birthday;
    d->birthmonth = s->birthmonth;
    d->birthyear = s->birthyear;
    d->sex = s->sex;
    d->publicmask = s->publicmask;
    if (s->aimname) {
        strzcpy(d->aimname, s->aimname, 0x33);
    } else {
        d->aimname[0] = 0;
    }
    d->icquin = s->icquin;
    d->longitude = s->longitude;
    d->latitude = s->latitude;
    d->birthday = s->birthday;
    d->birthmonth = s->birthmonth;
    d->birthyear = s->birthyear;
    d->sex = s->sex;
    d->publicmask = s->publicmask;
    d->pic = s->pic;
    d->occupationid = s->occupationid;
    d->industryid = s->industryid;
    d->incomeid = s->incomeid;
    d->marriedid = s->marriedid;
    d->childcount = s->childcount;
    d->interests1 = s->interests1;
    d->ownership1 = s->ownership1;
    d->conntypeid = s->conntypeid;
}
}
}

namespace Nb {
extern "C" {
s32 gpiProcessGetInfo(GPConnection *h, GPIOperation *req, char *str) {
    GPIConnection *ctx = *(GPIConnection **)h;
    struct {
        GPIProfile *e;
        GPICallbackCopy p;
        char buf[0x40];
        char a[0x1f];
        char b[0x15];
        char c[0x33];
        char d[0x1f];
        char e2[0x1f];
        char g[0x33];
    } l;
    s32 r5;
    s32 flag;
    GPIPeer *n;
    GPGetInfoResponseArg *node;

    if (gpiCheckForError(h, str, 1) != 0) {
        return 4;
    }
    if (strncmp(str, "\\pi\\", 4) != 0) {
        gpiSetError(h, 1, "Unexpected data was received from the server.");
        gpiCallErrorCallback(h, 3, 1);
        return 3;
    }
    if (FIND("\\profileid\\", l.buf, 0x40) == 0) {
        gpiSetError(h, 1, "Unexpected data was received from the server.");
        gpiCallErrorCallback(h, 3, 1);
        return 3;
    }
    r5 = atol(l.buf);
    gpiGetProfile(h, r5, &l.e);
    GPIInfoCache s = {0};
    char f[0x4c];
    s.nick = l.a;
    s.uniquenick = l.b;
    s.email = l.c;
    s.firstname = l.d;
    s.lastname = l.e2;
    s.homepage = f;
    s.aimname = l.g;
    if (FIND("\\nick\\", s.nick, 0x1f) == 0) {
        s.nick[0] = 0;
    }
    if (FIND("\\uniquenick\\", s.uniquenick, 0x15) == 0) {
        s.uniquenick[0] = 0;
    }
    if (FIND("\\email\\", s.email, 0x33) == 0) {
        s.email[0] = 0;
    }
    if (FIND("\\firstname\\", s.firstname, 0x1f) == 0) {
        s.firstname[0] = 0;
    }
    if (FIND("\\lastname\\", s.lastname, 0x1f) == 0) {
        s.lastname[0] = 0;
    }
    if (FIND("\\icquin\\", l.buf, 0x40) == 0) {
        s.icquin = -1;
    } else {
        s.icquin = atol(l.buf);
    }
    if (FIND("\\homepage\\", s.homepage, 0x4c) == 0) {
        s.homepage[0] = 0;
    }
    if (FIND("\\zipcode\\", s.zipcode, 0xb) == 0) {
        s.zipcode[0] = 0;
    }
    if (FIND("\\countrycode\\", s.countrycode, 3) == 0) {
        s.countrycode[0] = 0;
    }
    s.longitude = 0;
    s.latitude = 0;
    if (FIND("\\loc\\", s.place, 0x80) == 0) {
        s.place[0] = 0;
    }
    if (FIND("\\birthday\\", l.buf, 0x40) == 0) {
        s.birthday = 0;
        s.birthmonth = 0;
        s.birthyear = 0;
    } else {
        s32 r = gpiIntToDate(h, atol(l.buf), &s.birthday, &s.birthmonth, &s.birthyear);
        if (r != 0) {
            return r;
        }
    }
    if (FIND("\\sex\\", l.buf, 0x40) == 0) {
        s.sex = 0x502;
    } else if (l.buf[0] == 0x30) {
        s.sex = 0x500;
    } else if (l.buf[0] == 0x31) {
        s.sex = 0x501;
    } else {
        s.sex = 0x502;
    }
    if (FIND("\\pmask\\", l.buf, 0x40) == 0) {
        s.publicmask = -1;
    } else {
        s.publicmask = atol(l.buf);
    }
    if (FIND("\\aim\\", s.aimname, 0x33) == 0) {
        s.aimname[0] = 0;
    }
    if (FIND("\\pic\\", l.buf, 0x40) == 0) {
        s.pic = 0;
    } else {
        s.pic = atol(l.buf);
    }
    if (FIND("\\occ\\", l.buf, 0x40) == 0) {
        s.occupationid = 0;
    } else {
        s.occupationid = atol(l.buf);
    }
    if (FIND("\\ind\\", l.buf, 0x40) == 0) {
        s.industryid = 0;
    } else {
        s.industryid = atol(l.buf);
    }
    if (FIND("\\inc\\", l.buf, 0x40) == 0) {
        s.incomeid = 0;
    } else {
        s.incomeid = atol(l.buf);
    }
    if (FIND("\\mar\\", l.buf, 0x40) == 0) {
        s.marriedid = 0;
    } else {
        s.marriedid = atol(l.buf);
    }
    if (FIND("\\chc\\", l.buf, 0x40) == 0) {
        s.childcount = 0;
    } else {
        s.childcount = atol(l.buf);
    }
    if (FIND("\\i1\\", l.buf, 0x40) == 0) {
        s.interests1 = 0;
    } else {
        s.interests1 = atol(l.buf);
    }
    if (FIND("\\o1\\", l.buf, 0x40) == 0) {
        s.ownership1 = 0;
    } else {
        s.ownership1 = atol(l.buf);
    }
    if (FIND("\\conn\\", l.buf, 0x40) == 0) {
        s.conntypeid = 0;
    } else {
        s.conntypeid = atol(l.buf);
    }
    if (FIND("\\sig\\", l.buf, 0x40) == 0) {
        gpiSetError(h, 1, "Unexpected data was received from the server.");
        gpiCallErrorCallback(h, 3, 1);
        return 3;
    }
    flag = ctx->infoCaching;
    for (n = ctx->peerList; n != NULL; n = n->pnext) {
        if (n->profile == r5 && n->state == 0x65) {
            if (l.e == NULL) {
                l.e = gpiProfileListAdd(h, r5);
            }
            n->state = 0x66;
            flag = 1;
        }
    }
    if (l.e == NULL && ctx->infoCaching != 0) {
        l.e = gpiProfileListAdd(h, r5);
    }
    if (flag != 0) {
        GsUtil_Free(l.e->peerSig);
        l.e->peerSig = 0;
        l.e->peerSig = goastrdup(l.buf);
    }
    if (ctx->infoCaching != 0) {
        gpiSetInfoCache(h, l.e, &s);
    }
    l.p = req->callback;
    if (l.p.p.callback != 0) {
        node = (GPGetInfoResponseArg *)GsUtil_Alloc(sizeof(GPGetInfoResponseArg));
        if (node == NULL) {
            gpiSetErrorString(h, "Out of memory.");
            return 1;
        }
        gpiInfoCacheToArg(&s, node);
        node->result = 0;
        node->profile = r5;
        {
            s32 r = gpiAddCallback(h, l.p, node, req, 0);
            if (r != 0) {
                return r;
            }
        }
    }
    gpiRemoveOperation(h, req);
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiAddLocalInfo(GPConnection *h, GPIBuffer *p) {
    GPIConnection *c = *(GPIConnection **)h;
    if (c->updateproBuffer.len > 0) {
        gpiAppendStringToBuffer(h, p, "\\updatepro\\\\sesskey\\");
        gpiAppendIntToBuffer(h, p, c->sessKey);
        gpiAppendStringToBuffer(h, p, c->updateproBuffer.buffer);
        gpiAppendStringToBuffer(h, p, "\\final\\");
        c->updateproBuffer.len = 0;
    }
    if (c->updateuiBuffer.len > 0) {
        gpiAppendStringToBuffer(h, p, "\\updateui\\\\sesskey\\");
        gpiAppendIntToBuffer(h, p, c->sessKey);
        gpiAppendStringToBuffer(h, p, c->updateuiBuffer.buffer);
        gpiAppendStringToBuffer(h, p, "\\final\\");
        c->updateuiBuffer.len = 0;
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiSendLocalInfo(GPConnection *h, char *a, char *b) {
    GPIConnection *c = *(GPIConnection **)h;
    s32 r = gpiAppendStringToBuffer(h, &c->updateproBuffer, a);
    if (r != 0) {
        return r;
    }
    r = gpiAppendStringToBuffer(h, &c->updateproBuffer, b);
    if (r != 0) {
        return r;
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiSendUserInfo(GPConnection *h, char *a, char *b) {
    GPIConnection *c = *(GPIConnection **)h;
    s32 r = gpiAppendStringToBuffer(h, &c->updateuiBuffer, a);
    if (r != 0) {
        return r;
    }
    r = gpiAppendStringToBuffer(h, &c->updateuiBuffer, b);
    if (r != 0) {
        return r;
    }
    return 0;
}
}
}

namespace Nb {
extern "C" {
s32 gpiSetInfoi(GPConnection *h, s32 code, s32 val) {
    char buf[16];
    s32 r;
    switch (code) {
    case 0x708:
        if (val < 0) {
            gpiSetErrorString(h, "Invalid zipcode.");
            return 2;
        }
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\zipcode\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x70b:
        switch (val) {
        case 0x500:
            r = gpiSendLocalInfo(h, "\\sex\\", "0");
        if (r != 0) {
            return r;
        }
            break;
        case 0x501:
            r = gpiSendLocalInfo(h, "\\sex\\", "1");
        if (r != 0) {
            return r;
        }
            break;
        case 0x502:
            r = gpiSendLocalInfo(h, "\\sex\\", "2");
        if (r != 0) {
            return r;
        }
            break;
        default:
            gpiSetErrorString(h, "Invalid sex.");
            return 2;
        }
        break;
    case 0x706:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\icquin\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x70c:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendUserInfo(h, "\\cpubrandid\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x70d:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendUserInfo(h, "\\cpuspeed\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x70e:
        OS_SPrintf(buf, "%d", val / 16);
        r = gpiSendUserInfo(h, "\\memory\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x710:
        OS_SPrintf(buf, "%d", val / 4);
        r = gpiSendUserInfo(h, "\\videocard1ram\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x712:
        OS_SPrintf(buf, "%d", val / 4);
        r = gpiSendUserInfo(h, "\\videocard2ram\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x713:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendUserInfo(h, "\\connectionid\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x714:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendUserInfo(h, "\\connectionspeed\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x715:
        if (val != 0) {
            val = 1;
        }
        OS_SPrintf(buf, "%d", val);
        r = gpiSendUserInfo(h, "\\hasnetwork\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x718:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\pic\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x719:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\occ\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x71a:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\ind\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x71b:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\inc\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x71c:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\mar\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x71d:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\chc\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    case 0x71e:
        OS_SPrintf(buf, "%d", val);
        r = gpiSendLocalInfo(h, "\\i1\\", buf);
        if (r != 0) {
            return r;
        }
        break;
    default:
        gpiSetErrorString(h, "Invalid info.");
        return 2;
    }
    return 0;
}
}
}

namespace Na {
extern "C" {
s32 gpiSetInfos(GPConnection *h, s32 cmd, char *val) {
    GPIConnection *ctx = *h;
    char buf[0x100];
    s32 r;
    char ch;
    s32 c;
    if (val == 0) {
        gpiSetErrorString(h, "Invalid value.");
        return 2;
    }
    switch (cmd) {
    case 0x700:
        CK_NONEMPTY
        strzcpy(buf, val, 0x1f);
        strzcpy(ctx->nick, buf, 0x1f);
        r = gpiSendLocalInfo(h, "\\nick\\", buf);
        if (r != 0) return r;
        break;
    case 0x701:
        CK_NONEMPTY
        strzcpy(buf, val, 0x15);
        strzcpy(ctx->uniquenick, buf, 0x15);
        r = gpiSendLocalInfo(h, "\\uniquenick\\", buf);
        if (r != 0) return r;
        break;
    case 0x702:
        CK_NONEMPTY
        strzcpy(buf, val, 0x33);
        _strlwr(buf);
        strzcpy(ctx->email, buf, 0x33);
        r = gpiSendUserInfo(h, "\\email\\", buf);
        if (r != 0) return r;
        break;
    case 0x703:
        CK_NONEMPTY
        strzcpy(buf, val, 0x1f);
        strzcpy(ctx->password, buf, 0x1f);
        r = gpiSendUserInfo(h, "\\password\\", buf);
        if (r != 0) return r;
        break;
    case 0x704:
        strzcpy(buf, val, 0x1f);
        r = gpiSendLocalInfo(h, "\\firstname\\", buf);
        if (r != 0) return r;
        break;
    case 0x705:
        strzcpy(buf, val, 0x1f);
        r = gpiSendLocalInfo(h, "\\lastname\\", buf);
        if (r != 0) return r;
        break;
    case 0x707:
        strzcpy(buf, val, 0x4c);
        r = gpiSendLocalInfo(h, "\\homepage\\", buf);
        if (r != 0) return r;
        break;
    case 0x708:
        strzcpy(buf, val, 0xb);
        r = gpiSendLocalInfo(h, "\\zipcode\\", buf);
        if (r != 0) return r;
        break;
    case 0x709:
        if (STD_GetStringLength(val) != 2) {
            gpiSetErrorString(h, "Invalid countrycode.");
            return 2;
        }
        strzcpy(buf, val, 3);
        r = gpiSendLocalInfo(h, "\\countrycode\\", buf);
        if (r != 0) return r;
        break;
    case 0x70b:
        c = *val;
        if (c >= 0 && c < 0x80) {
            c = data_0213a490[c];
        }
        ch = c;
        if (ch == 0x4d) {
            STD_CopyString(buf, "0");
        } else if (ch == 0x46) {
            STD_CopyString(buf, "1");
        } else {
            STD_CopyString(buf, "2");
        }
        r = gpiSendLocalInfo(h, "\\sex\\", buf);
        if (r != 0) return r;
        break;
    case 0x706:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\icquin\\", buf);
        if (r != 0) return r;
        break;
    case 0x70d:
        r = gpiSetInfoi(h, 0x70d, atol(val));
        if (r != 0) return r;
        break;
    case 0x70e:
        r = gpiSetInfoi(h, 0x70e, atol(val));
        if (r != 0) return r;
        break;
    case 0x70f:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\videocard1string\\", buf);
        if (r != 0) return r;
        break;
    case 0x710:
        r = gpiSetInfoi(h, 0x710, atol(val));
        if (r != 0) return r;
        break;
    case 0x711:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\videocard2string\\", buf);
        if (r != 0) return r;
        break;
    case 0x712:
        r = gpiSetInfoi(h, 0x712, atol(val));
        if (r != 0) return r;
        break;
    case 0x714:
        r = gpiSetInfoi(h, 0x714, atol(val));
        if (r != 0) return r;
        break;
    case 0x715:
        r = gpiSetInfoi(h, 0x715, atol(val));
        if (r != 0) return r;
        break;
    case 0x716:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\osstring\\", buf);
        if (r != 0) return r;
        break;
    case 0x717:
        strzcpy(buf, val, 0x33);
        r = gpiSendLocalInfo(h, "\\aim\\", buf);
        if (r != 0) return r;
        break;
    case 0x718:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\pic\\", buf);
        if (r != 0) return r;
        break;
    case 0x719:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\occ\\", buf);
        if (r != 0) return r;
        break;
    case 0x71a:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\ind\\", buf);
        if (r != 0) return r;
        break;
    case 0x71b:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\inc\\", buf);
        if (r != 0) return r;
        break;
    case 0x71c:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\mar\\", buf);
        if (r != 0) return r;
        break;
    case 0x71d:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\chc\\", buf);
        if (r != 0) return r;
        break;
    case 0x71e:
        strzcpy(buf, val, 0x100);
        r = gpiSendLocalInfo(h, "\\i1\\", buf);
        if (r != 0) return r;
        break;
    default:
        gpiSetErrorString(h, "Invalid info.");
        return 2;
    }
    return 0;
}
}
}

namespace Na {
extern "C" {
// Not in the original binary: unreferenced weak function compiled right before gpiSetInfos, so that the literals
// "%d", "Invalid info.", "\\birthday\\" are pooled where the original has them; removed by the dead-stripping link (notes.txt).
__declspec(weak) void Unk_ov065_0227f54c_pool_order(void) {
    STD_GetStringLength("%d");
    STD_GetStringLength("Invalid info.");
    STD_GetStringLength("\\birthday\\");
}
}
}

namespace Na {
extern "C" {
s32 gpiSendGetInfo(GPConnection *h, s32 a1, s32 a2) {
    GPIConnection *ctx = *h;
    gpiAppendStringToBuffer(h, &ctx->outputBuffer, "\\getprofile\\\\sesskey\\");
    gpiAppendIntToBuffer(h, &ctx->outputBuffer, ctx->sessKey);
    gpiAppendStringToBuffer(h, &ctx->outputBuffer, "\\profileid\\");
    gpiAppendIntToBuffer(h, &ctx->outputBuffer, a1);
    gpiAppendStringToBuffer(h, &ctx->outputBuffer, "\\id\\");
    gpiAppendIntToBuffer(h, &ctx->outputBuffer, a2);
    gpiAppendStringToBuffer(h, &ctx->outputBuffer, "\\final\\");
    return 0;
}
}
}

namespace Na {
extern "C" {
s32 gpiGetInfo(GPConnection *h, s32 a1, s32 a2, s32 a3, GPCallback a4, void *a5) {
    GPGetInfoResponseArg *m;
    GPIProfile *n;
    GPIOperation *out2;
    GPICallbackCopy pr;
    s32 ok;
    GPIConnection *ctx;
    ctx = *h;
    out2 = 0;
    ok = 0;
    s32 p5;
    s32 r;
    if (a2 == 1) {
        ok = 1;
    }
    if (ctx->infoCaching == 0) {
        ok = 0;
    }
    if (a4 != 0 && ok != 0 && gpiGetProfile(h, a1, &n) != 0 && n->cache != 0) {
        m = (GPGetInfoResponseArg *)GsUtil_Alloc(sizeof(GPGetInfoResponseArg));
        if (m == 0) {
            gpiSetErrorString(h, "Out of memory.");
            return 1;
        }
        gpiInfoCacheToArg(n->cache, m);
        m->result = 0;
        m->profile = a1;
        pr.p.callback = a4;
        pr.p.param = a5;
        r = gpiAddOperation(h, 2, 0, &out2, 1, a4, a5);
        if (r != 0) {
            return r;
        }
        p5 = out2->id;
        r = gpiAddCallback(h, pr, m, out2, 0);
        if (r != 0) {
            return r;
        }
        gpiRemoveOperation(h, out2);
    } else {
        r = gpiAddOperation(h, 2, 0, &out2, a3, a4, a5);
        if (r != 0) {
            return r;
        }
        p5 = out2->id;
        r = gpiSendGetInfo(h, a1, p5);
        if (r != 0) {
            return r;
        }
    }
    if (a3 != 0) {
        r = gpiProcess(h, p5);
        if (r != 0) {
            return r;
        }
    }
    return 0;
}
}
}

namespace Na {
extern "C" {
s32 gpiSetInfoCache(GPConnection *h, GPIProfile *p, GPIInfoCache *q) {
    GPIInfoCache *d;
    if ((*h)->infoCaching == 0) {
        return 1;
    }
    gpiFreeInfoCache(p);
    p->cache = (GPIInfoCache *)GsUtil_Alloc(sizeof(GPIInfoCache));
    d = p->cache;
    if (d != 0) {
#ifdef HOST64
        *d = *q;  // (all of it: 0x110 bytes here, the copy block is the DS's 0xf0)
#else
        *(GPIInfoCacheCopy *)d = *(GPIInfoCacheCopy *)q;
#endif
        p->cache->nick = goastrdup(q->nick);
        p->cache->uniquenick = goastrdup(q->uniquenick);
        p->cache->email = goastrdup(q->email);
        p->cache->firstname = goastrdup(q->firstname);
        p->cache->lastname = goastrdup(q->lastname);
        p->cache->homepage = goastrdup(q->homepage);
        p->cache->aimname = goastrdup(q->aimname);
    }
    if (p->cache != 0) {
        return 1;
    }
    return 0;
}
}
}

namespace Na {
extern "C" {
void gpiFreeInfoCache(GPIProfile *p) {
    if (p->cache != 0) {
        GsUtil_Free(p->cache->nick);
        p->cache->nick = 0;
        GsUtil_Free(p->cache->uniquenick);
        p->cache->uniquenick = 0;
        GsUtil_Free(p->cache->email);
        p->cache->email = 0;
        GsUtil_Free(p->cache->firstname);
        p->cache->firstname = 0;
        GsUtil_Free(p->cache->lastname);
        p->cache->lastname = 0;
        GsUtil_Free(p->cache->homepage);
        p->cache->homepage = 0;
        GsUtil_Free(p->cache->aimname);
        p->cache->aimname = 0;
        GsUtil_Free(p->cache);
        p->cache = 0;
    }
}
}
}
