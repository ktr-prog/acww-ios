#include "uptr.h"
#include "types.h"
#include "text/Unk_02050288.h"
#include "Unk_020d8c7c.h"
#include "sys/StackPad.h"
#include "talk/MsgStringBase.h"
#include "talk/MsgStringAttr.h"
#include "talk/MsgTag.h"
#include "talk/MsgRunner.h"
#include "talk/BmgReader.h"
#include "talk/BmgMsgAttr.h"
#include "talk/MsgParser.h"
#include "talk/EncodedStringBase.h"
#include "talk/Flag18.h"
#include "talk/MsgProcessor.h"
#include "talk/MsgWalker.h"
#include "talk/EncodedString.h"
#include "talk/MsgRenderProcessor.h"
#include "talk/MsgQuery.h"
#include "talk/MsgCopyProcessor.h"
#include "talk/MsgUiProc.h"
#include "talk/MsgRequest.h"
#include "talk/MsgTextLabel.h"
#include "talk/MsgString.h"
#include "talk/MsgString33.h"
#include "talk/ChoiceString.h"
#include "sys/ProcProfile.h"
#include "proto/autoload_2/unk_020e7fd4.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/autoload_2/unk_02119434.h"
#include "proto/main/unk_02063904.h"
#include "proto/autoload_2/unk_0212a120.h"
#include "proto/main/unk_0200137c.h"
#include "proto/main/unk_020029e8.h"
#include "proto/main/unk_020116e0.h"
#include "proto/main/unk_020501d4.h"
#include "proto/main/unk_02065e88.h"
#include "proto/main/unk_020742f4.h"
#include "proto/main/unk_0208eeac.h"
#include "proto/main/unk_020a6974.h"
#include "proto/main/unk_020a8ba0.h"
#include "proto/main/unk_020b7190.h"
#include "proto/main/unk_020b7d84.h"

extern "C" {
// Other files
}

extern "C" {
void NNS_FndRemoveListObject(void *list, void *obj);
}

extern "C" {
void NNS_FndAppendListObject(void *list, void *obj);
}

extern "C" {
void MI_CpuFill8(void *dst, u32 value, u32 size);
}

extern "C" {
void MI_CpuCopy8(const void *src, void *dst, u32 size);
}

extern "C" {
void FS_CloseFile(FSFile *file);
}

extern "C" {
BOOL FS_OpenFile(FSFile *file, const char *path);
}

extern "C" {
char *strncpy(char *dst, const char *src, u32 n);
}

extern "C" {
u32 strlen(const char *s);
}

extern "C" {
// This file
BOOL Msg_EncodeGameChar(u8 *out, const u8 *src);
}

extern "C" {
BOOL Msg_DecodeGameChar(char *out, u8 c);
}

extern "C" {
BOOL Input_IsTouchBlocked(void);
}

extern "C" {
BOOL Input_IsKeyBlocked(void);
}

extern "C" {
void Input_ResetMode(void);
}

extern "C" {
u16 Msg_ReadU16(u8 *p, u32 i);
}

extern "C" {
u8 Msg_ReadU8(u8 *p, u32 i);
}

extern "C" {
u8 Bmg_ReadU8(void *p);
}

extern "C" {
u16 Bmg_ReadU16(void *p);
}

extern "C" {
u32 Bmg_ReadU32(void *p);
}

extern "C" {
u32 Bmg_ReadMagic(void *p);
}

extern "C" {
void MsgUiProc_HideObjPlane(void);
}

extern "C" {
void MsgUiProc_ShowObjPlane(void);
}

extern "C" {
extern u16 gPad[2];
}

extern u8 sInputButtonMode;

extern u8 sInputLocked;

// class of the byte globals registered with the destructor at 0x020082a8
struct TalkMsgIndex {
    u8 v;
    TalkMsgIndex(u8 x) { v = x; }
    ~TalkMsgIndex() {}  // on the DS the empty function PlayerActor::func_020082a8
};

extern TalkMsgIndex gU8None;
extern TalkMsgIndex gTalkMsgIndexNone;
extern TalkMsgIndex gTalkMsgIndexEnd;
extern const u8 sColorTags[10][7];
extern const u32 sBmgMsgAttrTableA[25];
extern const u32 sBmgMsgAttrTableB[25];
extern const u32 sBmgMsgAttrTableC[25];
extern ProcProfile sMsgUiProcProfile;

class MsgString;

class MsgParser;
class MsgStringAttr;
class MsgProcessor;
class MsgWalker;
class EncodedString;
class MsgRequest;
class MsgRenderProcessor;
class MsgTextLabel;
class MsgQuery;
class MsgString33;
class MsgCopyProcessor;
class BmgReader;

// ChoiceString: talk/ChoiceString.h

extern "C" {
extern BmgInfHeader sBmgInfHeader;
}

extern "C" {
extern BmgDatHeader sBmgDatHeader;
}

extern "C" {
extern BmgFileHeader sBmgFileHeader;
}

extern MsgRenderProcessor gMsgRenderProcessor;
extern MsgCopyProcessor gMsgCopyProcessor;
extern MsgQuery gMsgQuery;

extern "C" {
void BmgMsgAttr_Clear(BmgMsgAttr *s);
}

// ---- Touch input
static inline BOOL IsTouching() {
    return gTouchHeld != 0 && gTouchChanged != 0;
}

// ---- MsgQuery
// MsgTag::MsgTag(), which func_020a6a0c calls to re-construct its token in place. A placement new adds a
// null check, and C++ has no other way to call a constructor on an existing object.

// prototypes (test harness)
extern "C" void MsgUiProc_ShowObjPlane();
extern "C" void MsgUiProc_HideObjPlane();
extern "C" u32 Bmg_ReadMagic(void *p);
extern "C" u32 Bmg_ReadU32(void *p);
extern "C" u16 Bmg_ReadU16(void *p);
extern "C" u8 Bmg_ReadU8(void *p);
extern "C" u8 *Bmg_GetMsgAttr(u8 *p);
extern "C" void MsgRunner_DtorStub(void);
extern "C" MsgTextLabel *MsgTextLabel_CreateVram(u32 a, s32 b, s32 c);
extern "C" MsgTextLabel *MsgTextLabel_CreateBuffer(void *buf, s32 b, s32 c);
extern "C" void MsgTextLabel_Destroy(MsgTextLabel *obj);
extern "C" u32 Msg_MeasureWidth(u8 *text);
extern "C" u32 TextLabel_MeasureMsgWidth(MsgString *s);
extern "C" BOOL EncodedString_SetRaw(StrBuf *buf, const void *src, s32 len);
extern "C" u8 Msg_ReadU8(u8 *p, u32 i);
extern "C" u16 Msg_ReadU16(u8 *p, u32 i);
extern "C" void MsgTag_DtorStub();
extern "C" u8 *Msg_GetColorTag(s32 i);
extern "C" BmgMsgAttr *BmgMsgAttr_Init(BmgMsgAttr *s);
extern "C" void BmgMsgAttr_Fini();
extern "C" void BmgMsgAttr_Copy(BmgMsgAttr *d, BmgMsgAttr *s);
extern "C" void BmgMsgAttr_Clear(BmgMsgAttr *s);
extern "C" void *BmgMsgAttr_Get(void *p);
extern "C" u8 BmgMsgAttr_GetByte04(BmgMsgAttr *s);
extern "C" u8 BmgMsgAttr_GetByte05(BmgMsgAttr *s);
extern "C" u8 BmgMsgAttr_GetByte06(BmgMsgAttr *s);
extern "C" u8 BmgMsgAttr_GetByte07(BmgMsgAttr *s);
extern "C" u8 BmgMsgAttr_GetByte09(BmgMsgAttr *s);
extern "C" void BmgMsgAttr_GetByte08(u8 *out, BmgMsgAttr *s);
extern "C" u32 BmgMsgAttr_LookupUnkA(BmgMsgAttr *s);
extern "C" u32 BmgMsgAttr_LookupUnkB(BmgMsgAttr *s);
extern "C" u32 BmgMsgAttr_LookupUnkC(BmgMsgAttr *s);
extern "C" BOOL Input_IsTouchTrig();
extern "C" BOOL Input_IsTouchTrigInRect(s32 x0, s32 x1, s32 y0, s32 y1);
extern "C" BOOL Input_GetTouchTrigPos(u32 *x, u32 *y);
extern "C" BOOL Input_GetTouchHeldPos(u32 *a, u32 *b);
extern "C" BOOL Input_IsAnyKeyTrig(void);
extern "C" BOOL Input_IsATrig(void);
extern "C" BOOL Input_IsBTrig(void);
extern "C" BOOL Input_IsUpTrig(void);
extern "C" BOOL Input_IsDownTrig(void);
extern "C" BOOL Input_IsStartTrig(void);
extern "C" BOOL Input_IsAHeld(void);
extern "C" BOOL Input_IsBHeld(void);
extern "C" BOOL Input_IsUpHeld(void);
extern "C" BOOL Input_IsDownHeld(void);
extern "C" void Input_Lock(void);
extern "C" void Input_Unlock(void);
extern "C" void Input_SetTouchMode(void);
extern "C" void Input_SetButtonMode(void);
extern "C" BOOL Input_IsTouchMode(void);
extern "C" u8 Input_IsButtonMode(void);
extern "C" void Input_ResetMode(void);
extern "C" void Input_LoadMode(void);
extern "C" void Input_StoreMode(void);
extern "C" BOOL Input_IsKeyBlocked(void);
extern "C" BOOL Input_IsTouchBlocked(void);
extern "C" u8 *Msg_FindTag(u8 *text, s32 group, s32 id);
extern "C" u32 Msg_GetCharAt(u8 *text, u32 index);
extern "C" u32 Msg_GetCharFromEnd(u8 *text, u32 n);
extern "C" u32 Msg_CountChars(u8 *text);
extern "C" u8 *Msg_SkipLines(u8 *text, u32 n);
extern "C" BOOL Msg_DecodeGameChar(char *out, u8 c);
extern "C" BOOL Msg_EncodeGameChar(u8 *out, const u8 *src);

const u32 sBmgMsgAttrTableC[25] = {0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1};
TalkMsgIndex gU8None(0xff);
u8 sInputLocked;
BmgFileHeader sBmgFileHeader;

extern "C" void MsgUiProc_ShowObjPlane() { Gfx2d_ShowMainPlanes(0x10); }

extern "C" void MsgUiProc_HideObjPlane() { Gfx2d_HideMainPlanes(0x10); }

// ---- MsgStringAttr
MsgStringAttr::MsgStringAttr() {
    form = -1;
    attrA = gU8None.v;
    attrB = gU8None.v;
}

MsgStringAttr::~MsgStringAttr() {}

void MsgStringAttr::copyFrom(MsgStringAttr *other) {
    form = other->form;
    attrA = other->attrA;
    attrB = other->attrB;
}

void MsgStringAttr::reset() {
    form = -1;
    attrA = gU8None.v;
    attrB = gU8None.v;
}

extern "C" u32 Bmg_ReadMagic(void *p) {
    u8 tmp[8];
    u8 *src, *dst;
    dst = tmp;
    src = (u8 *)p + 4;
    while (src != p) {
        src--;
        *dst = *src;
        dst++;
    }
    return *(u32 *)tmp;
}

extern "C" u32 Bmg_ReadU32(void *p) { return *(u32 *)p; }

extern "C" u16 Bmg_ReadU16(void *p) { return *(u16 *)p; }

extern "C" u8 Bmg_ReadU8(void *p) { return *(u8 *)p; }

// ---- BmgReader
BmgReader::BmgReader(u8 arg1) {
    hasAttributes = arg1;
    isOpen = 0;
    msgIndex = gU8None.v;
    textOffset = 0;
    textSize = 0;
    FS_InitFile(&file);
    MI_CpuFill8(&filePath, 0, 0x3f);
}

BmgReader::~BmgReader() {
    close();
}

u8 BmgReader::open(const char *path) {
    strncpy((char *)filePath, path, 0x3e);
    isOpen = FS_OpenFile(&file, path) ? 1 : 0;
    return isOpen;
}

void BmgReader::close() {
    if (isOpen != 0) {
        FS_CloseFile(&file);
        isOpen = 0;
        msgIndex = gU8None.v;
    }
}

BOOL BmgReader::loadMessage(u8 *arg1) {
    BOOL ok;
    resetState();
    msgIndex = *arg1;
    BOOL r = readFileHeader();
    ok = TRUE;
    if (!(r & ok)) {
        ok = FALSE;
    }
    if (ok) {
        ok &= readInfHeader();
        if (ok) ok = TRUE; else ok = FALSE;
    }
    if (ok) {
        ok &= readDatHeader();
        if (ok) ok = TRUE; else ok = FALSE;
    }
    if (ok) {
        if (hasAttributes != 0) {
            ok &= readInfEntryWithAttr();
            if (ok) ok = TRUE; else ok = FALSE;
        } else {
            ok &= readInfEntry();
            if (ok) ok = TRUE; else ok = FALSE;
        }
    }
    if (ok) {
        ok &= readText();
        if (ok) ok = TRUE; else ok = FALSE;
    }
    return ok;
}

void BmgReader::resetState() {
    StackPad8 pad;
    msgIndex = 0;
    MI_CpuFill8(entry, 0, 12);
    textOffset = 0;
    textSize = 0;
    MI_CpuFill8(&sBmgFileHeader, 0, 0x20);
    MI_CpuFill8(&sBmgInfHeader, 0, 0x14);
    MI_CpuFill8(&sBmgDatHeader, 0, 0xc);
}

BOOL BmgReader::readFileHeader() {
    File_ReadRange(&file, &sBmgFileHeader, 0x20, 0);
    sBmgFileHeader.magic = Bmg_ReadMagic(&sBmgFileHeader);
    sBmgFileHeader.type = Bmg_ReadMagic(&sBmgFileHeader.type);
    sBmgFileHeader.fileSize = Bmg_ReadU32(&sBmgFileHeader.fileSize);
    sBmgFileHeader.sectionCount = Bmg_ReadU32(&sBmgFileHeader.sectionCount);
    sBmgFileHeader.encoding = Bmg_ReadU8(&sBmgFileHeader.encoding);
    sBmgFileHeader.unk_1c = Bmg_ReadU32(&sBmgFileHeader.unk_1c);
    BOOL c1 = sBmgFileHeader.type == 0x626d6731;
    BOOL c2 = sBmgFileHeader.fileSize != 0;
    BOOL c3 = sBmgFileHeader.sectionCount == 2;
    if (sBmgFileHeader.magic == 0x4d455347 && c1 && c2 && c3) {
        return TRUE;
    }
    return FALSE;
}

BOOL BmgReader::readInfHeader() {
    File_ReadRange(&file, &sBmgInfHeader, 0x14, 0x20);
    sBmgInfHeader.magic = Bmg_ReadMagic(&sBmgInfHeader);
    sBmgInfHeader.size = Bmg_ReadU32(&sBmgInfHeader.size);
    sBmgInfHeader.msgCount = Bmg_ReadU16(&sBmgInfHeader.msgCount);
    sBmgInfHeader.entrySize = Bmg_ReadU16(&sBmgInfHeader.entrySize);
    sBmgInfHeader.groupId = Bmg_ReadU16(&sBmgInfHeader.groupId);
    sBmgInfHeader.defaultColor = Bmg_ReadU8(&sBmgInfHeader.defaultColor);
    BOOL c1 = sBmgInfHeader.magic == 0x494e4631;
    u16 n = sBmgInfHeader.msgCount;
    BOOL c2 = FALSE;
    if (n <= 0x100 && msgIndex < n) {
        c2 = TRUE;
    }
    BOOL c3 = sBmgInfHeader.entrySize == (hasAttributes ? 12 : 4);
    BOOL c4 = sBmgInfHeader.groupId == 0;
    if (c1 && c2 && c3 && c4) {
        return TRUE;
    }
    return FALSE;
}

BOOL BmgReader::readDatHeader() {
    File_ReadRange(&file, &sBmgDatHeader, 12, sBmgInfHeader.size + 0x20);
    sBmgDatHeader.magic = Bmg_ReadMagic(&sBmgDatHeader);
    sBmgDatHeader.size = Bmg_ReadU32(&sBmgDatHeader.size + 0);
    return sBmgDatHeader.magic == 0x44415431;
}

BOOL BmgReader::readInfEntry() {
    u32 buf[2];
    u32 off = msgIndex * 4 + 0x10;
    BOOL last = (u32)(sBmgInfHeader.msgCount - 1) == msgIndex;
    File_ReadRange(&file, buf, last ? 4 : 8, off + 0x20);
    textOffset = Bmg_ReadU32(&buf[0]);
    if (last) {
        textSize = sBmgDatHeader.size - (textOffset + 8);
    } else {
        textSize = Bmg_ReadU32(&buf[1]) - textOffset;
    }
    entry[0] = textOffset;
    return TRUE;
}

BOOL BmgReader::readInfEntryWithAttr() {
    BmgInfEntryAttr buf[2];
    s32 count;
    s32 i;
    u32 off = msgIndex * 12 + 0x10;
    BOOL last = (u32)(sBmgInfHeader.msgCount - 1) == msgIndex;
    File_ReadRange(&file, buf, last ? 12 : 24, off + 0x20);
    if (last) {
        count = 1;
    } else {
        count = 2;
    }
    for (i = 0; i < count; i++) {
        BmgInfEntryAttr *p = &buf[i];
        p->textOffset = Bmg_ReadU32(&p->textOffset);
        p->attrs[0] = Bmg_ReadU8(&p->attrs[0]);
        p->attrs[1] = Bmg_ReadU8(&p->attrs[1]);
        p->attrs[2] = Bmg_ReadU8(&p->attrs[2]);
        p->attrs[3] = Bmg_ReadU8(&p->attrs[3]);
        p->attrs[4] = Bmg_ReadU8(&p->attrs[4]);
        p->attrs[5] = Bmg_ReadU8(&p->attrs[5]);
    }
    textOffset = buf[0].textOffset;
    if (last) {
        textSize = sBmgDatHeader.size - (textOffset + 8);
    } else {
        textSize = buf[1].textOffset - textOffset;
    }
    MI_CpuCopy8(buf, entry, 12);
    return TRUE;
}

BOOL BmgReader::readText() {
    u8 *a = getBuffer();
    u32 b = getBufferSize();
    u32 off = textOffset + 8;
    if (b >= textSize) {
        b = textSize;
    }
    File_ReadRange(&file, a, b, sBmgInfHeader.size + 0x20 + off);
    return TRUE;
}

// ---- MsgRunner
extern "C" u8 *Bmg_GetMsgAttr(u8 *p) {
    u8 *r = NULL;
    if (((BmgReader *)p)->hasAttributes != 0) {
        r = (u8 *)((BmgReader *)p)->entry;
    }
    return r;
}

MsgRunner::MsgRunner(MsgWalker *obj) {
    walker = obj;
    text = NULL;
    stopPos = NULL;
}

extern "C" void MsgRunner_DtorStub(void) {}

void MsgRunner::reset() {
    walker->reset();
    text = NULL;
    stopPos = NULL;
}

BOOL MsgRunner::advance() {
    BOOL r = FALSE;
    if (text != NULL) {
        stopPos = walker->run(FALSE);
        if (stopPos != NULL) {
            r = TRUE;
        } else {
            text = NULL;
        }
    }
    return r;
}

void MsgRunner::start(u8 *p) {
    text = p;
    stopPos = p;
    walker->begin(p);
}

// ---- MsgParser
void MsgParser::reset() {
    cursor = NULL;
    while (!callStack.isEmpty()) {
        callStack.pop();
    }
}

MsgParser::MsgParser() {
    cursor = NULL;
}

MsgParser::~MsgParser() {
    reset();
}

void MsgParser::onBegin() {}

void MsgParser::onEnd() {}

void MsgParser::onChar(u32 c) {}

void MsgParser::onTag(u8 *p) {}

void MsgParser::processTag() {
    u8 *p = cursor;
    cursor = p + p[1];
    onTag(p);
}

void MsgParser::skip(s32 n) {
    if (cursor != NULL) {
        cursor = cursor + n;
    }
}

void MsgParser::unreadTag(u8 *p) {
    cursor = cursor - p[1];
}

BOOL MsgParser::isLeadByte(u32 c) {
    return FALSE;
}

BOOL MsgParser::step(u32 arg) {
    u32 c = *cursor;
    BOOL r = TRUE;
    if (c == 0 || (arg != 0 && c == 0xa)) {
        if (callStack.isEmpty()) {
            onEnd();
            r = FALSE;
        } else {
            popText();
        }
    } else if (c == 0x1a) {
        processTag();
    } else {
        if (isLeadByte(c)) {
            c = c << 8;
            cursor = cursor + 1;
            c |= *cursor;
        }
        cursor = cursor + 1;
        onChar(c);
    }
    return r;
}

void MsgParser::begin(u8 *p) {
    cursor = p;
    onBegin();
}

void MsgParser::pushText(const u8 *p) {
    if (p != NULL) {
        callStack.push(cursor);
        cursor = const_cast<u8 *>(p);
    }
}

void MsgParser::popText() {
    cursor = *callStack.top();
    callStack.pop();
}

// ---- MsgWalker
BOOL MsgWalker::canContinue() {
    return TRUE;
}

u8 *MsgWalker::run(u8 *end) {
    u8 *r = NULL;
    for (;;) {
        if (cursor == end) {
            onEnd();
            break;
        }
        if (!canContinue()) {
            r = cursor;
            break;
        }
        if (!step(0)) {  // (r is still NULL here)
            break;
        }
    }
    return r;
}

// ---- MsgProcessor
MsgProcessor::MsgProcessor(u8 flag) {
    label = NULL;
    stopAtNewline = flag;
}

MsgProcessor::~MsgProcessor() {}

u32 MsgProcessor::run(u8 *p) {
    for (;;) {
        if (cursor == p) {
            onEnd();
            break;
        }
        if (!step(stopAtNewline)) {
            break;
        }
    }
    return 0;
}

void MsgProcessor::setLabel(MsgTextLabel *p) { label = p; }

void MsgProcessor::clearLabel() { label = NULL; }

void MsgProcessor::setStopAtNewline() { stopAtNewline = 1; }

void MsgProcessor::clearStopAtNewline() { stopAtNewline = 0; }

// ---- MsgRenderProcessor
MsgRenderProcessor::MsgRenderProcessor() : MsgProcessor(1) {
    altTextEnd = NULL;
}

MsgRenderProcessor::~MsgRenderProcessor() {}

void MsgRenderProcessor::onBegin() {
    if (label != NULL) {
        label->onBegin();
    }
}

void MsgRenderProcessor::onEnd() {
    if (label != NULL) {
        label->onEnd();
    }
}

void MsgRenderProcessor::onChar(u32 c) {
    if (label != NULL) {
        label->onChar(c);
    }
    if (altTextEnd != NULL && cursor == altTextEnd) {
        altTextEnd = NULL;
        popText();
    }
}

void MsgRenderProcessor::onTag(u8 *p) {
    MsgTag s;
    s.parse(p);
    u32 a = s.group;
    u32 b = s.id;
    if (a == 2) {
        skip(s.getTrailingStringsSize());
    } else if (a == 0xff) {
        if (b == 2) {
            u32 x;
            char *y, *z;
            s.getAltTextArgs(&x, &y, &z);
            if (!Talk_IsAltTextEnabled()) {
                skip(x * 2);
                pushText((u8 *)z);
                altTextEnd = (u8 *)y;
            }
        }
    } else if (a == 0) {
        const u8 *r = NULL;
        if (b == 0) {
            r = Text_GetSpecialCharStr6();
        } else if (b == 1) {
            r = Text_GetSpecialCharStr7();
        } else if (b == 6) {
            r = Text_GetSpecialCharStr5();
        } else if (b == 7) {
            r = Text_GetSpecialCharStr4();
        } else if (b == 8) {
            r = Text_GetSpecialCharStr1();
        } else if (b == 9) {
            r = Text_GetSpecialCharStr9();
        } else if (b == 10) {
            r = Text_GetSpecialCharStr10();
        }
        if (r != NULL) {
            pushText(r);
        }
    }
    if (label != NULL) {
        label->onTag(p);
    }
}

// ---- MsgTextLabel
extern "C" MsgTextLabel *MsgTextLabel_CreateVram(u32 a, s32 b, s32 c) {
    BOOL ok = FALSE;
    void *mem = Heap_Alloc(static_cast<Heap *>(gTextHeap), sizeof(MsgTextLabel));
    MsgTextLabel *obj = NULL;
    if (mem != NULL) {
        obj = new (mem) MsgTextLabel(a, b, c);
        ok = TRUE;
    }
    if (ok) {
        NNS_FndAppendListObject(gTextLabelList, obj);
    }
    return obj;
}

extern "C" MsgTextLabel *MsgTextLabel_CreateBuffer(void *buf, s32 b, s32 c) {
    BOOL ok = FALSE;
    void *mem = Heap_Alloc(static_cast<Heap *>(gTextHeap), sizeof(MsgTextLabel));
    MsgTextLabel *obj = NULL;
    if (mem != NULL) {
        obj = new (mem) MsgTextLabel(static_cast<u8 *>(buf), b, c);
        ok = TRUE;
    }
    if (ok) {
        NNS_FndAppendListObject(gTextLabelList, obj);
    }
    return obj;
}

extern "C" void MsgTextLabel_Destroy(MsgTextLabel *obj) {
    if (obj != NULL) {
        NNS_FndRemoveListObject(gTextLabelList, obj);
        obj->~MsgTextLabel();
        Heap_Free(static_cast<Heap *>(gTextHeap), obj);
    }
}

extern "C" u32 Msg_MeasureWidth(u8 *text) {
    u32 r = 0;
    MsgTextLabel *obj = MsgTextLabel_CreateVram(0, 1, 2);
    if (obj != NULL) {
        obj->textStart = text;
        r = obj->measureWidth();
    }
    MsgTextLabel_Destroy(obj);
    return r;
}

extern "C" u32 TextLabel_MeasureMsgWidth(MsgString *s) {
    return Msg_MeasureWidth(s->data());
}

MsgTextLabel::MsgTextLabel(u32 tileIndex, s32 widthTiles, s32 heightTiles) : TextLabel(tileIndex, widthTiles, heightTiles) {
    mode = 0;
    processor = NULL;
}

MsgTextLabel::MsgTextLabel(u8 *buf, s32 widthTiles, s32 heightTiles) : TextLabel(buf, widthTiles, heightTiles) {
    mode = 0;
    processor = NULL;
}

MsgTextLabel::~MsgTextLabel() {}

void MsgTextLabel::setProcessor(MsgProcessor *v) { processor = v; }

void MsgTextLabel::onBegin() {
    if (mode == 1) {
        beginRow();
    } else if (mode == 2) {
        beginMeasure();
    }
}

void MsgTextLabel::onEnd() {
    if (mode == 1) {
        flushRow();
    } else if (mode == 2) {
        endMeasure();
    }
}

void MsgTextLabel::onChar(u32 c) {
    if (mode == 1) {
        drawChar(c);
    } else if (mode == 2) {
        measureChar(c);
    }
}

void MsgTextLabel::onTag(u8 *p) {
    if (mode == 1) {
        MsgTag s;
        s.parse(p);
        u32 a = *(volatile u32 *)&s.group;
        u32 b = *(volatile u32 *)&s.id;
        if (a == 0xff) {
            if (b == 0) {
                fgColor = Talk_ColorTagToTextColor(s.getArgU8());
            }
        }
    }
}

void MsgTextLabel::draw() {
    MsgProcessor *p = processor;
    if (p == NULL) {
        p = &gMsgRenderProcessor;
    }
    mode = 1;
    p->setLabel(this);
    p->reset();
    p->begin(textStart);
    p->run(textEnd);
    p->clearLabel();
    mode = 0;
}

u32 MsgTextLabel::measureWidth() {
    MsgProcessor *p = processor;
    if (p == NULL) {
        p = &gMsgRenderProcessor;
    }
    mode = 2;
    p->setLabel(this);
    p->reset();
    p->begin(textStart);
    p->run(textEnd);
    p->clearLabel();
    mode = 0;
    return curX;
}

// ---- MsgCopyProcessor
MsgCopyProcessor::MsgCopyProcessor() : MsgProcessor(1) {
    dest = NULL;
    srcText = NULL;
    copyMode = 0;
    result = 0;
}

MsgCopyProcessor::~MsgCopyProcessor() {}

void MsgCopyProcessor::beginCopy(MsgString *s, u8 *str, u32 mode, u8 flag) {
    dest = s;
    srcText = str;
    copyMode = mode;
    result = 0;
    if (flag) {
        setStopAtNewline();
    } else {
        clearStopAtNewline();
    }
    begin(str);
}

void MsgCopyProcessor::finish() {
    dest = NULL;
    srcText = NULL;
    copyMode = 0;
    result = 0;
}

u8 MsgCopyProcessor::getResult() {
    return result;
}

void MsgCopyProcessor::onEnd() {
    if (dest != NULL) {
        if (copyMode == 0) {
            result = dest->assignRange(srcText, cursor);
        } else if (copyMode == 1) {
            result = dest->appendRange(srcText, cursor);
        }
    }
}

// ---- MsgString
MsgString::MsgString() : length(0) {}

MsgString::~MsgString() {}

void MsgString::clear() {
    StrBuf_Clear((StrBuf *)this);
    length = 0;
    attr.reset();
}

u8 MsgString::set(u8 *str) {
    gMsgCopyProcessor.beginCopy(this, str, 0, 0);
    gMsgCopyProcessor.run(NULL);
    u8 r = gMsgCopyProcessor.getResult();
    gMsgCopyProcessor.finish();
    return r;
}

u8 MsgString::copy(MsgString *other) {
    u8 r = set(other->data());
    attr.copyFrom(&other->attr);
    return r;
}

BOOL MsgString::fromEncoded(EncodedString *src, BOOL a, BOOL b) {
    s32 srcSize = src->capacity();
    u8 *srcPtr = src->data();
    u8 *dst = data();
    u32 dstSize = capacity();
    u32 pos = 0;
    BOOL over = FALSE;
    BOOL done = FALSE;
    s32 i = 0;
    for (; i < srcSize;) {
        u8 *out = dst + pos;
        char tmp[8];
        u32 n = Msg_DecodeGameChar(tmp, *srcPtr);
        if (pos + n > dstSize) {
            over = TRUE;
        }
        if (!over) {
            if (n == 1) {
                s8 c = tmp[0];
                if (c == 0) {
                    done = TRUE;
                    if (b) {
                        strcpy(tmp, (const char *)Text_GetSpecialCharStr3());
                        n = strlen(tmp);
                        if (pos + n > dstSize) {
                            over = done;
                        }
                    }
                } else if (a) {
                    if (c == 0xa) {
                        strcpy(tmp, (const char *)Text_GetSpecialCharStr2());
                        n = strlen(tmp);
                        if (pos + n > dstSize) {
                            over = TRUE;
                        }
                    }
                }
            }
            if (!over) {
                for (u32 j = 0; j < n; j++) {
                    out[j] = tmp[j];
                }
            }
        }
        if (over) {
            break;
        }
        pos += n;
        if (done) {
            break;
        }
        i++;
        srcPtr++;
    }
    if (!over && pos < dstSize) {
        done = TRUE;
        while (pos < dstSize) {
            dst[pos] = 0;
            pos++;
        }
    }
    if (!done) {
        *(dst + dstSize - 1) = 0;
    }
    attr.copyFrom(&src->attr);
    if (!over && done) {
        return TRUE;
    }
    return FALSE;
}

u8 MsgString::setLine(u8 *str) {
    gMsgCopyProcessor.beginCopy(this, str, 0, 1);
    gMsgCopyProcessor.run(NULL);
    u8 r = gMsgCopyProcessor.getResult();
    gMsgCopyProcessor.finish();
    return r;
}

u8 MsgString::append(u8 *str) {
    gMsgCopyProcessor.beginCopy(this, str, 1, 0);
    gMsgCopyProcessor.run(NULL);
    u8 r = gMsgCopyProcessor.getResult();
    gMsgCopyProcessor.finish();
    return r;
}

u8 MsgString::appendString(MsgString *other) {
    return append(other->data());
}

BOOL MsgString::equals(MsgString *other) {
    u8 *o = other->data();
    return strcmp((const char *)data(), (const char *)o) == 0;
}

BOOL MsgString::assignRange(u8 *start, u8 *end) {
    clear();
    u8 *buf = data();
    u32 cap = capacity();
    s32 len = end - start;
    BOOL ok;
    if ((u32)(len + 1) <= cap) {
        ok = TRUE;
    } else {
        ok = FALSE;
    }
    if (ok) {
        length = len;
    } else {
        length = cap - 1;
    }
    MI_CpuCopy8(start, buf, length);
    return ok;
}

BOOL MsgString::appendRange(u8 *start, u8 *end) {
    u8 *buf = data();
    u32 cap = capacity();
    buf += length;
    cap -= length;
    s32 len = end - start;
    BOOL ok;
    if ((u32)(len + 1) <= cap) {
        ok = TRUE;
    } else {
        ok = FALSE;
    }
    if (!ok) {
        len = cap - 1;
    }
    MI_CpuCopy8(start, buf, len);
    length += len;
    return ok;
}

// ---- EncodedString
EncodedString::EncodedString() {}

EncodedString::~EncodedString() {}

extern "C" BOOL EncodedString_SetRaw(StrBuf *buf, const void *src, s32 len) {
    return StrBuf_SetBytes(buf, src, len);
}

BOOL EncodedString::fromMsgString(MsgString *src) {
    u8 *sp = src->data();
    u32 srcSize = src->capacity();
    u32 consumed = 0;
    u8 *dp = data();
    u32 dstSize = capacity();
    u32 count = 0;
    BOOL ok = TRUE;
    while (consumed < srcSize && count < dstSize) {
        u8 c;
        if (*sp == 0) {
            break;
        }
        u32 n = Msg_EncodeGameChar(&c, sp);
        if (n == 0) {
            n = 1;
            ok = FALSE;
        } else {
            *dp = c;
            dp++;
            count++;
        }
        sp += n;
        consumed += n;
    }
    if (count >= dstSize && *sp != 0) {
        ok = FALSE;
    }
    while (count < dstSize) {
        *dp = 0;
        count++;
        dp++;
    }
    attr.copyFrom(&src->attr);
    return ok;
}

// ---- MsgTag

extern "C" u8 Msg_ReadU8(u8 *p, u32 i) {
    u8 t = p[i];
    return Bmg_ReadU8(&t);
}

extern "C" u16 Msg_ReadU16(u8 *p, u32 i) {
    u8 *q = p + i;
    u16 t = q[0] | (q[1] << 8);
    return Bmg_ReadU16(&t);
}

void MsgTag::parse(u8 *p) {
    u8 a = Msg_ReadU8(p, 0);
    u8 b = Msg_ReadU8(p, 1);
    u8 c = Msg_ReadU8(p, 2);
    u16 d = Msg_ReadU16(p, 3);
    group = c;
    id = d;
    argLen = b - 5;
    args = (char *)(p + 5);
    raw = p;
}

MsgTag::MsgTag() {
    group = -1;
    id = -1;
    argLen = 0;
    args = NULL;
}

extern "C" void MsgTag_DtorStub() {}

void MsgTag::getArgs1(u8 *a) {
    *a = Msg_ReadU8((u8 *)args, 0);
}

void MsgTag::getArgs2(u8 *a, u8 *b) {
    *a = Msg_ReadU8((u8 *)args, 0);
    *b = Msg_ReadU8((u8 *)args, 1);
}

void MsgTag::getArgs3(u8 *a, u8 *b, u8 *c) {
    *a = Msg_ReadU8((u8 *)args, 0);
    *b = Msg_ReadU8((u8 *)args, 1);
    *c = Msg_ReadU8((u8 *)args, 2);
}

void MsgTag::getArgs4(u8 *a, u8 *b, u8 *c, u8 *d) {
    *a = Msg_ReadU8((u8 *)args, 0);
    *b = Msg_ReadU8((u8 *)args, 1);
    *c = Msg_ReadU8((u8 *)args, 2);
    *d = Msg_ReadU8((u8 *)args, 3);
}

void MsgTag::getArgs5(u8 *a, u8 *b, u8 *c, u8 *d, u8 *e) {
    *a = Msg_ReadU8((u8 *)args, 0);
    *b = Msg_ReadU8((u8 *)args, 1);
    *c = Msg_ReadU8((u8 *)args, 2);
    *d = Msg_ReadU8((u8 *)args, 3);
    *e = Msg_ReadU8((u8 *)args, 4);
}

void MsgTag::getArgBytes(u8 *buf, s32 n) {
    for (s32 i = 0; i < n; i++) buf[i] = Msg_ReadU8((u8 *)args, i);
}

void MsgTag::getArgU16(u16 *out) { *out = Msg_ReadU16((u8 *)args, 0); }

u32 MsgTag::readArgStrings2(u8 *a1, MsgString *a2, u8 *a3, MsgString *s0) {
    u8 buf[2];
    MsgString *arr[2];
    getArgs2(&buf[0], &buf[1]);
    *a1 = buf[0];
    *a3 = buf[1];
    arr[0] = a2;
    arr[1] = s0;
    char *p = args + argLen + 1;
    u32 total = 1;
    for (u32 i = 0; i < 2; i++) {
        MsgString *t = arr[i];
        t->setLine((u8 *)p);
        u32 n = t->length + 1;
        total += n;
        p += n;
    }
    return total;
}

u32 MsgTag::readArgStrings3(u8 *a1, MsgString *a2, u8 *a3, MsgString *s0, u8 *s1, MsgString *s2) {
    u8 buf[8];
    MsgString *arr[3];
    getArgs3(&buf[0], &buf[1], &buf[2]);
    *a1 = buf[0];
    *a3 = buf[1];
    *s1 = buf[2];
    arr[0] = a2;
    arr[1] = s0;
    arr[2] = s2;
    char *p = args + argLen + 1;
    u32 total = 1;
    for (s32 i = 0; i < 3; i++) {
        MsgString *t = arr[i];
        t->setLine((u8 *)p);
        u32 n = t->length + 1;
        total += n;
        p += n;
    }
    return total;
}

u32 MsgTag::readArgStrings4(u8 *a1, MsgString *a2, u8 *a3, MsgString *s0, u8 *s1, MsgString *s2,
                                u8 *s3, MsgString *s4) {
    u8 buf[8];
    MsgString *arr[4];
    getArgs4(&buf[0], &buf[1], &buf[2], &buf[3]);
    *a1 = buf[0];
    *a3 = buf[1];
    *s1 = buf[2];
    *s3 = buf[3];
    arr[0] = a2;
    arr[1] = s0;
    arr[2] = s2;
    arr[3] = s4;
    char *p = args + argLen + 1;
    u32 total = 1;
    for (s32 i = 0; i < 4; i++) {
        MsgString *t = arr[i];
        t->setLine((u8 *)p);
        u32 n = t->length + 1;
        total += n;
        p += n;
    }
    return total;
}

u32 MsgTag::readArgStrings5(u8 *a1, MsgString *a2, u8 *a3, MsgString *s0, u8 *s1, MsgString *s2,
                                u8 *s3, MsgString *s4, u8 *s5, MsgString *s6) {
    u8 buf[12];
    MsgString *arr[5];
    getArgs5(&buf[0], &buf[1], &buf[2], &buf[3], &buf[4]);
    *a1 = buf[0];
    *a3 = buf[1];
    *s1 = buf[2];
    *s3 = buf[3];
    *s5 = buf[4];
    arr[0] = a2;
    arr[1] = s0;
    arr[2] = s2;
    arr[3] = s4;
    arr[4] = s6;
    char *p = args + argLen + 1;
    u32 total = 1;
    for (s32 i = 0; i < 5; i++) {
        MsgString *t = arr[i];
        t->setLine((u8 *)p);
        u32 n = t->length + 1;
        total += n;
        p += n;
    }
    return total;
}

u32 MsgTag::getTrailingStringsSize() {
    s32 k = id;
    u32 n = 0;
    if (k == 0 || k == 4) {
        n = 2;
    } else if (k == 1 || k == 5) {
        n = 3;
    } else if (k == 2 || k == 6) {
        n = 4;
    } else if (k == 3 || k == 7) {
        n = 5;
    }
    ChoiceString t;
    u32 total = 1;
    char *p = args + argLen + 1;
    for (u32 i = 0; i < n; i++) {
        t.setLine((u8 *)p);
        u32 m = t.length + 1;
        total += m;
        p += m;
    }
    return total;
}

u32 MsgTag::readArgBytes8Strings2(u8 *a1, u8 *a2, u8 *a3, u8 *s0, u8 *s1, u8 *s2, u8 *s3, u8 *s4, MsgString *s5,
                                MsgString *s6) {
    struct {
        u32 pad;
        u8 buf[8];
        MsgString *arr[2];
    } L;
    getArgBytes(L.buf, 8);
    *a1 = L.buf[0];
    *a3 = L.buf[2];
    *s1 = L.buf[4];
    *s3 = L.buf[6];
    *a2 = L.buf[1];
    *s0 = L.buf[3];
    *s2 = L.buf[5];
    *s4 = L.buf[7];
    L.arr[0] = s5;
    L.arr[1] = s6;
    char *p = args + argLen + 1;
    u32 total = 1;
    for (u32 i = 0; i < 2; i++) {
        MsgString *t = L.arr[i];  // (by element: the DS stepped 4 bytes, a pointer's size there)
        t->setLine((u8 *)p);
        u32 n = t->length + 1;
        total += n;
        p += n;
    }
    return total;
}

BOOL MsgTag::isSlotTag() {
    if (id >= 15 && id <= 25) return TRUE;
    return FALSE;
}

s32 MsgTag::getSlotIndex() { return id - 15; }

void MsgTag::getStrings2(char **a, char **b) {
    char *s;
    char *p;
    *a = NULL;
    *b = NULL;
    s = args;
    if (s[0] != 0) *a = s;
    p = strchr(s, 0);
    p++;
    if ((u32)(p - args) < argLen) *b = p;
}

void MsgTag::getStrings3(char **a, char **b, char **c) {
    char *s;
    char *p;
    char *q;
    *a = NULL;
    *b = NULL;
    *c = NULL;
    s = args;
    if (s[0] != 0) *a = s;
    p = strchr(s, 0);
    if (p[1] != 0) *b = p + 1;
    q = strchr(p + 1, 0);
    q++;
    if ((u32)(q - args) < argLen) *c = q;
}

void MsgTag::getAltTextArgs(u32 *a, char **b, char **c) {
    *a = Msg_ReadU8((u8 *)args, 0);
    *b = args + argLen;
    *c = args + 1;
}

u8 MsgTag::getArgU8() {
    u8 v;
    getArgs1(&v);
    return v;
}

// ---- BmgMsgAttr

extern "C" u8 *Msg_GetColorTag(s32 i) { return (u8 *)sColorTags[i]; }

extern "C" BmgMsgAttr *BmgMsgAttr_Init(BmgMsgAttr *s) {
    BmgMsgAttr_Clear(s);
    return s;
}

extern "C" void BmgMsgAttr_Fini() {}

extern "C" void BmgMsgAttr_Copy(BmgMsgAttr *d, BmgMsgAttr *s) {
    d->textOffset = s->textOffset;
    d->windowStyle = s->windowStyle;
    d->friendshipDelta = s->friendshipDelta;
    d->startEvent = s->startEvent;
    d->endEvent = s->endEvent;
    d->nextMsg = s->nextMsg;
    d->msgSe = s->msgSe;
    d->unk_0a = s->unk_0a;
    d->unk_0b = s->unk_0b;
}

extern "C" void BmgMsgAttr_Clear(BmgMsgAttr *s) { MI_CpuFill8(s, 0, 0xc); }

extern "C" void *BmgMsgAttr_Get(void *p) { return p; }

extern "C" u8 BmgMsgAttr_GetByte04(BmgMsgAttr *s) { return s->windowStyle; }

extern "C" u8 BmgMsgAttr_GetByte05(BmgMsgAttr *s) { return s->friendshipDelta; }

extern "C" u8 BmgMsgAttr_GetByte06(BmgMsgAttr *s) { return s->startEvent; }

extern "C" u8 BmgMsgAttr_GetByte07(BmgMsgAttr *s) { return s->endEvent; }

extern "C" u8 BmgMsgAttr_GetByte09(BmgMsgAttr *s) { return s->msgSe; }

extern "C" void BmgMsgAttr_GetByte08(u8 *out, BmgMsgAttr *s) { *out = s->nextMsg; }

extern "C" u32 BmgMsgAttr_LookupUnkA(BmgMsgAttr *s) { return sBmgMsgAttrTableA[BmgMsgAttr_GetByte04(s)]; }

extern "C" u32 BmgMsgAttr_LookupUnkB(BmgMsgAttr *s) { return sBmgMsgAttrTableB[BmgMsgAttr_GetByte04(s)]; }

extern "C" u32 BmgMsgAttr_LookupUnkC(BmgMsgAttr *s) { return sBmgMsgAttrTableC[BmgMsgAttr_GetByte04(s)]; }

// ---- MsgString33
MsgString33::MsgString33() { initEmpty(); }

MsgString33::~MsgString33() {}

u32 MsgString33::capacity() { return 0x21; }

u8 *MsgString33::data() { return text; }

void MsgString33::initEmpty() { clear(); }

// ---- MsgRequest
MsgRequest::MsgRequest() {
    msgIndex = gU8None.v;
    MI_CpuFill8(fileName, 0, 0x1a);
}

MsgRequest::~MsgRequest() {}

void MsgRequest::resetMsg() {
    msgIndex = gU8None.v;
    MI_CpuFill8(fileName, 0, 0x1a);
}

void MsgRequest::setFileName(const char *src) {
    strncpy(fileName, src, 0x19);
}

extern "C" BOOL Input_IsTouchTrig() {
    BOOL r = FALSE;
    if (!Input_IsTouchBlocked() && IsTouching()) r = TRUE;
    return r;
}

extern "C" BOOL Input_IsTouchTrigInRect(s32 x0, s32 x1, s32 y0, s32 y1) {
    BOOL r = FALSE;
    if (!Input_IsTouchBlocked() && IsTouching()) {
        s32 x = gTouchPressX;
        s32 y = gTouchPressY;
        if (x >= x0 && x < x1 && y >= y0 && y < y1) r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_GetTouchTrigPos(u32 *x, u32 *y) {
    BOOL r = FALSE;
    if (!Input_IsTouchBlocked() && IsTouching()) {
        u32 a = gTouchPressX;
        u32 b = gTouchPressY;
        if (x != NULL) *x = a;
        if (y != NULL) *y = b;
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_GetTouchHeldPos(u32 *a, u32 *b) {
    BOOL r = FALSE;
    if (!Input_IsTouchBlocked() && gTouchHeld) {
        u8 x = gTouchCurX;
        u8 y = gTouchCurY;
        if (a) {
            *a = x;
        }
        if (b) {
            *b = y;
        }
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsAnyKeyTrig(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[1] & 0xfff)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsATrig(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[1] & 0x1)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsBTrig(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[1] & 0x2)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsUpTrig(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[1] & 0x40)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsDownTrig(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[1] & 0x80)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsStartTrig(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[1] & 0x8)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsAHeld(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[0] & 0x1)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsBHeld(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[0] & 0x2)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsUpHeld(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[0] & 0x40)) {
        r = TRUE;
    }
    return r;
}

extern "C" BOOL Input_IsDownHeld(void) {
    BOOL r = FALSE;
    if (!Input_IsKeyBlocked() && (gPad[0] & 0x80)) {
        r = TRUE;
    }
    return r;
}

extern "C" void Input_Lock(void) { sInputLocked = 1; }

extern "C" void Input_Unlock(void) { sInputLocked = 0; }

extern "C" void Input_SetTouchMode(void) { sInputButtonMode = 0; }

extern "C" void Input_SetButtonMode(void) { sInputButtonMode = 1; }

extern "C" BOOL Input_IsTouchMode(void) {
    if (!sInputButtonMode) {
        return TRUE;
    }
    return FALSE;
}

extern "C" u8 Input_IsButtonMode(void) {
    return sInputButtonMode;
}

extern "C" void Input_ResetMode(void) {
    sInputLocked = 0;
    sInputButtonMode = 0;
}

extern "C" void Input_LoadMode(void) {
    if (InputMode_IsButtons()) {
        sInputButtonMode = 1;
    } else {
        sInputButtonMode = 0;
    }
}

extern "C" void Input_StoreMode(void) {
    if (sInputButtonMode) {
        InputMode_SetButtons();
    } else {
        InputMode_SetTouch();
    }
}

extern "C" BOOL Input_IsKeyBlocked(void) {
    if (sInputLocked && !sInputButtonMode) {
        return TRUE;
    }
    return FALSE;
}

extern "C" BOOL Input_IsTouchBlocked(void) {
    if (sInputLocked && sInputButtonMode) {
        return TRUE;
    }
    return FALSE;
}

MsgQuery::MsgQuery() : mode(0) {
    curChar = 0;
    findGroup = -1;
    findId = -1;
    targetIndex = 0;
    charCount = 0;
    targetLines = 0;
    lineCount = 0;
    isDone = 0;
}

MsgQuery::~MsgQuery() {}

extern "C" u8 *Msg_FindTag(u8 *a, s32 b, s32 c) {
    gMsgQuery.resetQuery();
    gMsgQuery.mode = 0;
    gMsgQuery.findGroup = b;
    gMsgQuery.findId = c;
    gMsgQuery.begin(a);
    gMsgQuery.run(FALSE);
    if (((Flag18 *)((u8 *)&gMsgQuery.findGroup))->flag) {
        return gMsgQuery.tag.raw;
    }
    return 0;
}

extern "C" u32 Msg_GetCharAt(u8 *a, u32 b) {
    gMsgQuery.resetQuery();
    gMsgQuery.mode = 1;
    gMsgQuery.targetIndex = b;
    gMsgQuery.begin(a);
    gMsgQuery.run(FALSE);
    if (((Flag18 *)((u8 *)&gMsgQuery.findGroup))->flag) {
        return gMsgQuery.curChar;
    }
    return 0;
}

extern "C" u32 Msg_GetCharFromEnd(u8 *a, u32 b) {
    s32 n = Msg_CountChars(a) - b - 1;
    u32 r = 0;
    if (n >= 0) {
        r = Msg_GetCharAt(a, n);
    }
    return r;
}

extern "C" u32 Msg_CountChars(u8 *a) {
    gMsgQuery.resetQuery();
    gMsgQuery.mode = 3;
    gMsgQuery.begin(a);
    gMsgQuery.run(FALSE);
    if (((Flag18 *)((u8 *)&gMsgQuery.findGroup))->flag) {
        return gMsgQuery.charCount;
    }
    return 0;
}

extern "C" u8 *Msg_SkipLines(u8 *a, u32 b) {
    gMsgQuery.resetQuery();
    gMsgQuery.mode = 4;
    gMsgQuery.targetLines = b;
    gMsgQuery.begin(a);
    gMsgQuery.run(FALSE);
    if (((Flag18 *)((u8 *)&gMsgQuery.findGroup))->flag) {
        return gMsgQuery.cursor;
    }
    return 0;
}

void MsgQuery::onBegin() {
    if (mode == 4 && targetLines == 0) {
        isDone = 1;
    }
}

void MsgQuery::onEnd() {
    if (mode == 3) {
        isDone = 1;
    }
}

void MsgQuery::onChar(u32 v) {
    curChar = v;
    static void (MsgQuery::*tbl[5])() = {0, &MsgQuery::onCharFindNth, 0, &MsgQuery::onCharCount,
                                             &MsgQuery::onCharCountLines};
    void (MsgQuery::*fn)() = tbl[mode];
    if (fn) {
        (this->*fn)();
    }
}

const u8 sColorTags[10][7] = {
    {26, 6, 255, 0, 0, 0, 0},
    {26, 6, 255, 0, 0, 1, 0},
    {26, 6, 255, 0, 0, 2, 0},
    {26, 6, 255, 0, 0, 3, 0},
    {26, 6, 255, 0, 0, 4, 0},
    {26, 6, 255, 0, 0, 5, 0},
    {26, 6, 255, 0, 0, 6, 0},
    {26, 6, 255, 0, 0, 7, 0},
    {26, 6, 255, 0, 0, 8, 0},
    {26, 6, 255, 0, 0, 9, 0}};
u8 sInputButtonMode;
TalkMsgIndex gTalkMsgIndexNone(0xff);
const u32 sBmgMsgAttrTableA[25] = {0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
const u32 sBmgMsgAttrTableB[25] = {0, 0, 1, 1, 2, 2, 1, 0, 1, 2, 0, 2, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1};
TalkMsgIndex gTalkMsgIndexEnd(0xfe);
ProcProfile sMsgUiProcProfile = {(void *(*)())MsgUiProc_Create, 0xc9, 0xc7};
BmgDatHeader sBmgDatHeader;

void MsgQuery::onTag(u8 *cmd) {
    tag.parse(cmd);
    static void (MsgQuery::*tbl[5])() = {&MsgQuery::onTagFind, 0, 0, 0, 0};
    void (MsgQuery::*fn)() = tbl[mode];
    if (fn) {
        (this->*fn)();
    }
}

BOOL MsgQuery::canContinue() {
    return isDone == 0;
}

void MsgQuery::resetQuery() {
    mode = 0;
    new (&tag) MsgTag();
    curChar = 0;
    findGroup = -1;
    findId = -1;
    targetIndex = 0;
    charCount = 0;
    targetLines = 0;
    lineCount = 0;
    isDone = 0;
    reset();
}

void MsgQuery::onCharFindNth() {
    if (charCount++ == targetIndex) {
        isDone = 1;
    }
}

void MsgQuery::onCharCount() {
    charCount++;
}

void MsgQuery::onCharCountLines() {
    if (curChar == 10) {
        lineCount++;
        if (lineCount == targetLines) {
            isDone = 1;
        }
    }
}

void MsgQuery::onTagFind() {
    tag.eq(*(volatile s32 *)&findGroup, *(volatile s32 *)&findId, &isDone);
}

extern "C" BOOL Msg_DecodeGameChar(char *out, u8 c) { return Text_GameCharToAscii(out, c); }

extern "C" BOOL Msg_EncodeGameChar(u8 *out, const u8 *src) { return Text_AsciiToGameCharPtr(out, src); }

BmgInfHeader sBmgInfHeader;
MsgRenderProcessor gMsgRenderProcessor;
MsgCopyProcessor gMsgCopyProcessor;
MsgQuery gMsgQuery;

