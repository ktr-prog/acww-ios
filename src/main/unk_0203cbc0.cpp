#include "types.h"
#include "talk/MsgTag.h"
#include "talk/BmgReader.h"
#include "talk/MsgParser.h"
#include "talk/MailTextBuilder.h"
#include "talk/MsgRequest.h"
#include "talk/MsgWalker.h"
#include "talk/MailMsgRequest.h"
#include "talk/MsgString.h"
#include "talk/MsgString33.h"
#include "player/PlayerData.h"
#include "player/PlayerId.h"
#include "proto/main/unk_0203cbc0.h"
#include "proto/main/unk_020501d4.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_020b2c50.h"

extern "C" {
void MI_CpuFill8(void *dst, u32 value, u32 size);
}
extern const char *const sMailFolderDirs[3];
extern const char *const sMailPartDirs[8];

enum Unk_0203d134_E { Unk_0203d134_E0 = 0, Unk_0203d134_E15 = 15 };

extern "C" s32 Stub_ReturnFalse(void) { return 0; }

extern "C" void Main_PreTaskStub(void) {}

#ifdef HOST64
extern "C" void host_test_post_task(void);  // (modern/host/frontend.c: test scripts' hooks, ACWW_WFC_TEST)
extern "C" void Main_PostTaskStub(void) { host_test_post_task(); }
#else
extern "C" void Main_PostTaskStub(void) {}
#endif

extern "C" void ScreenLayers_AcquireStub(void) {}

extern "C" void ScreenLayers_ReleaseStub(void) {}

extern "C" void Scene_PostCreateStub(void) {}

BmgReader512::BmgReader512() : BmgReader(0) {}

BmgReader512::~BmgReader512() {}

void BmgReader512::clearBuffer() {
    MI_CpuFill8(buffer, 0, 0x200);
}

u8 *BmgReader512::getBuffer() {
    return buffer;
}

u32 BmgReader512::getBufferSize() {
    return 0x200;
}

MailTextExpander::MailTextExpander(MailTextBuilder *owner) : builder(owner) {
    curChar = 0;
    outLen = 0;
    success = 1;
    capitalizeNext = 0;
    selectEnd = 0;
    articleMode = 0;
    trackNamePos = 0;
    charCount = 0;
    newlineCount = 0;
}

MailTextExpander::~MailTextExpander() {}

u8 MailTextExpander::expand(u8 flag) {
    success = 1;
    outLen = 0;
    trackNamePos = flag;
    charCount = 0;
    newlineCount = 0;
    reset();
    begin(builder->reader.getBuffer());
    run(FALSE);
    return success;
}

void MailTextExpander::onBegin() {
    capitalizeNext = 0;
    selectEnd = 0;
    articleMode = 0;
}

void MailTextExpander::onEnd() {}

void MailTextExpander::onChar(u32 v) {
    if (capitalizeNext) {
        curChar = Text_ToUpper(v);
        capitalizeNext = 0;
    } else {
        curChar = v;
    }
    appendChar();
    if (cursor == selectEnd) {
        selectEnd = 0;
        popText();
    }
}

void MailTextExpander::onTag(u8 *p) {
    tag.parse(p);
    s32 r4 = tag.group;
    MailTextExpanderFn fn = 0;
    if (r4 == 4 && tag.isSlotTag()) {
        fn = data_020d9460;
    } else if (r4 == 0) {
        fn = data_020d9470;
    } else if (r4 == 0xb) {
        fn = data_020d9468;
    }
    if (fn) {
        (this->*fn)();
    } else {
        success = 0;
    }
}

BOOL MailTextExpander::canContinue() {
    return TRUE;
}

void MailTextExpander::appendChar() {
    s32 t = curChar;
    s8 c = (s8)t;
    s32 idx = outLen;
    if ((u32)(0x200 - idx) > 1) {
        if (t == 10 && trackNamePos != 0) {
            newlineCount++;
            if (newlineCount == 1) {
                builder->namePos = charCount;
            }
        } else {
            outLen++;
            builder->output[idx] = c;
            if (trackNamePos != 0) {
                charCount++;
            }
        }
    } else {
        success = 0;
    }
}

void MailTextExpander::insertSlot() {
    s32 i = tag.getSlotIndex();
    MsgString33 *e = &builder->slots[i];
    pushText(e->data());
    articleMode = 0;
}

void MailTextExpander::insertGlyphTag() {
    s32 sel = tag.id;
    if (sel == 0) {
        pushText((u8 *)Text_GetSpecialCharStr6());
    } else if (sel == 1) {
        pushText((u8 *)Text_GetSpecialCharStr7());
    } else if (sel == 7) {
        pushText((u8 *)Text_GetSpecialCharStr4());
    } else if (sel == 8) {
        pushText((u8 *)Text_GetSpecialCharStr1());
    }
}

void MailTextExpander::handleGrammarTag() {
    s32 sel = tag.id;
    s32 n;
    if (sel == 1) {
        setUnkMode1(sel);
    } else if (sel == 2) {
        setUnkMode2(sel);
    } else if (sel == 3) {
        setCapitalizeNext(sel);
    } else if (sel == 4) {
        selectByUnkCondition(sel);
    } else if (sel >= 5 && sel <= 15) {
        Unk_0203d134_E en = (Unk_0203d134_E)(sel - 5);
        MailTextBuilder *o = builder;
        selectBySlotForm(&o->slots[en]);
    }
}

void MailTextExpander::setUnkMode1(s32 sel) {
    articleMode = 1;
}

void MailTextExpander::setUnkMode2(s32 sel) {
    articleMode = 2;
}

void MailTextExpander::setCapitalizeNext(s32 sel) {
    capitalizeNext = 1;
}

void MailTextExpander::selectByUnkCondition(s32 sel) {
    char *a, *b;
    tag.getStrings2(&a, &b);
    void *pd;
    if ((pd = PlayerData_GetCurrent()) != 0) {
        if (((PlayerId *)(((PlayerData *)(pd))->getPlayerId()))->getGender() == 0) {
            if (a != 0) {
                pushText((u8 *)a);
            }
        } else if (b != 0) {
            selectEnd = cursor;
            pushText((u8 *)b);
        }
    }
}

void MailTextExpander::selectBySlotForm(MsgString33 *e) {
    char *a, *b, *c;
    tag.getStrings3(&a, &b, &c);
    s32 m = e->attr.form;
    if (m == 0) {
        if (a != 0) {
            pushText((u8 *)a);
        }
    } else if (m == 1) {
        if (b != 0) {
            pushText((u8 *)b);
        }
    } else if (m == 2) {
        if (c != 0) {
            selectEnd = cursor;
            pushText((u8 *)c);
        }
    }
}

extern "C" BOOL MailText_LoadLetter(MsgString *a, MsgString *b, MsgString *c, u32 *d, const u8 *e, const char *f) {
    MailMsgRequest obj;
    obj.setFolder(0);
    obj.setFileName(f);
    obj.msgIndex = *e;
    obj.setDest(a);
    obj.setNamePosOut(d);
    obj.setPart(1);
    gMailTextBuilder.reset();
    u32 r5 = gMailTextBuilder.load(&obj);
    obj.setNamePosOut(0);
    obj.setDest(b);
    obj.setPart(2);
    gMailTextBuilder.reset();
    u32 r4 = gMailTextBuilder.load(&obj);
    obj.setDest(c);
    obj.setPart(3);
    gMailTextBuilder.reset();
    u32 r0 = gMailTextBuilder.load(&obj);
    BOOL result;
    if (r5 != 0 && r4 != 0 && r0 != 0) {
        result = TRUE;
    } else {
        result = FALSE;
    }
    return result;
}

extern "C" BOOL MailText_LoadLetterZ(MsgString *a, MsgString *b, MsgString *c, u32 *d, u8 *e1, u8 *e2, u8 *e3, u8 *e4, const char *name) {
    MailMsgRequest l;
    BOOL r5, r6, r4, r0, ok;
    l.setFolder(1);
    l.setFileName(name);
    l.setDest(a);
    l.setNamePosOut(d);
    l.msgIndex = *e1;
    l.setPart(4);
    gMailTextBuilder.reset();
    r5 = gMailTextBuilder.load(&l);
    l.setNamePosOut(0);
    l.setDest(b);
    l.msgIndex = *e2;
    l.setPart(5);
    gMailTextBuilder.reset();
    r6 = gMailTextBuilder.load(&l);
    l.setDest(b);
    l.msgIndex = *e3;
    l.setPart(6);
    gMailTextBuilder.reset();
    r4 = gMailTextBuilder.load(&l);
    l.setDest(c);
    l.msgIndex = *e4;
    l.setPart(7);
    gMailTextBuilder.reset();
    r0 = gMailTextBuilder.load(&l);
    if (r5 && r6 && r4 && r0) ok = TRUE; else ok = FALSE;
    return ok;
}

extern "C" BOOL MailText_LoadBbs(MsgString *a, u8 *p, const char *name) {
    MailMsgRequest l;
    l.setFolder(2);
    l.setFileName(name);
    l.setDest(a);
    l.msgIndex = *p;
    l.setPart(0);
    gMailTextBuilder.reset();
    BOOL r = gMailTextBuilder.load(&l);
    return r;
}

extern "C" s32 MailText_SetSlot(s32 i, void *x) { return gMailTextBuilder.slots[i].copy((MsgString *)(x)); }

extern "C" void MailText_SetSlotMonth(s32 i, s32 x) { String_GetMonthName(&gMailTextBuilder.slots[i], x); }

extern "C" void MailText_SetSlotDayOrdinal(s32 i, s32 x) { String_GetDayOrdinal(&gMailTextBuilder.slots[i], x); }

MailTextBuilder::MailTextBuilder() : expander(this) {
    namePos = -1;
    MI_CpuFill8(output, 0, 0x200);
}

MailTextBuilder::~MailTextBuilder() {}

void MailTextBuilder::reset() {
    reader.clearBuffer();
    MI_CpuFill8(output, 0, 0x200);
    namePos = -1;
}

BOOL MailTextBuilder::load(MailMsgRequest *p) {
    char buf[0x44];
    const char *s = p->getPartDir();
    if (s != NULL) {
        Str_SPrintf(buf, "%s/%s/%s.bmg", p->getMsgDir(), s, p->fileName);
    } else {
        Str_SPrintf(buf, "%s/%s.bmg", p->getMsgDir(), p->fileName);
    }
    BOOL a = reader.open(buf);
    BOOL b = a ? reader.loadMessage(&p->msgIndex) : 0;
    bool ok = a & b;
    reader.close();
    if (ok) {
        MsgString *q = p->getDest();
        u32 *out = p->getNamePosOut();
        BOOL m = p->isAppendPart();
        ok &= expander.expand(out != 0 ? TRUE : FALSE);
        if (m) {
            ok &= (BOOL)q->append(output);
        } else {
            ok &= (BOOL)q->set(output);
        }
        if (out) *out = namePos;
    }
    return ok;
}

MailMsgRequest::MailMsgRequest() : folder(0), part(0), dest(0), namePosOut(0) {}

MailMsgRequest::~MailMsgRequest() {}

const char *MailMsgRequest::getMsgDir() { return sMailFolderDirs[folder]; }

BOOL MailMsgRequest::isAppendPart() {
    if (part == 6) return TRUE;
    return FALSE;
}

const char *MailMsgRequest::getPartDir() { return sMailPartDirs[part]; }

void MailMsgRequest::setFolder(u32 v) { folder = v; }

void MailMsgRequest::setPart(u32 v) { part = v; }

void MailMsgRequest::setDest(MsgString *v) { dest = v; }

void MailMsgRequest::setNamePosOut(u32 *v) { namePosOut = v; }

MsgString *MailMsgRequest::getDest() { return dest; }

// ---------------------------------------------------------------------------------------------------------------------

u32 *MailMsgRequest::getNamePosOut() { return namePosOut; }

// Declarations for data defined further down (definition order sets the data layout)
const char *const sMailPartDirs[8] = {NULL, data_020d9450, data_020d943c, data_020d9434, data_020d9458, data_020d9448, data_020d9440, data_020d9438};

char data_020d943c[] = "msg";

// ---- data
const char *const sMailFolderDirs[3] = {data_020d9488, data_020d949c, data_020d9478};

char data_020d9440[] = "msgb";

char data_020d9448[] = "msga";

char data_020d9450[] = "super";

char data_020d9458[] = "superz";

// the tag handlers of onTag
MailTextExpanderFn data_020d9460 = &MailTextExpander::insertSlot;

MailTextExpanderFn data_020d9468 = &MailTextExpander::handleGrammarTag;

MailTextExpanderFn data_020d9470 = &MailTextExpander::insertGlyphTag;

char data_020d9438[] = "psz";

char data_020d9478[] = "/script/ENG/bbs";

char data_020d9488[] = "/script/ENG/mail";

char data_020d949c[] = "/script/ENG/mailz";

char data_020d9434[] = "ps";

MailTextBuilder gMailTextBuilder;
