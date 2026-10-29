#ifndef TALK_VILLAGERTALKREQUESTITEMTOPICS_H
#define TALK_VILLAGERTALKREQUESTITEMTOPICS_H

#include "types.h"
#include "talk/VillagerTalk.h"

struct TalkTopicMsg;
struct PlanErrand;
class PlayerErrandSlot;
class VillagerId;
class VillagerActor;
class VillagerTalkRequestItemTopics;

typedef void (VillagerTalkRequestItemTopics::*Unk_020238b0_Fn)();
typedef void (VillagerTalkRequestItemTopics::*Unk_020238b0_OutFn)(TalkTopicMsg *);

// Villager talk topics of the item request / errand chain (Q* states: hand over item, return, clear, reward); the
// root of the ov069 topic chain (... <- Hobby <- Holiday <- RequestReply <- Topics). All of them are views of the
// VillagerTalk object (0x1a0 bytes, talk/VillagerTalk.h) with member pointers of this class; the fields are the union
// of what the topic functions use. Members defined in src/main/unk_0201c050.cpp.
class VillagerTalkRequestItemTopics {
public:
    void openArbeitItemPicker();
    void clearRequest();
    void selectQComp(TalkTopicMsg *out);
    void selectQReturn(TalkTopicMsg *out);
    void finishRequestChain();
    void selectQEnd(TalkTopicMsg *out);
    void selectQClear(TalkTopicMsg *out);
    void gotoQClearOrEnd();
    void handOverQItemB();
    void selectQItemB(TalkTopicMsg *out);
    void pickRequestReward();
    void gotoQItemB();

    // The fields: each where VillagerTalk (talk/VillagerTalk.h) has it in this build - the DS's offsets (in the
    // comments) in a 32-bit build, the host's on a 64-bit one, where the DS's would land in other members (pointers
    // are twice as large: a villager's talk read its actor from the wrong place and crashed). VT_GAP(from, to) is the
    // room between two of VillagerTalk's members; the checks below the class keep each field in place.
#define VT_OFF(m) __builtin_offsetof(VillagerTalk, m)
#define VT_GAP(prev_end, next) (VT_OFF(next) - (prev_end))
    /* 0x000 */ u8 pad_00[VT_OFF(window)];
    /* 0x03c */ void *window;
    /* 0x040 */ u8 pad_40[VT_GAP(VT_OFF(window) + sizeof(void *), selectFn)];
    /* 0x0ac */ Unk_020238b0_OutFn selectFn;
    /* 0x0b4 */ u8 pad_b4[VT_GAP(VT_OFF(selectFn) + sizeof(Unk_020238b0_OutFn), unk_c4)];
    /* 0x0c4 */ Unk_020238b0_Fn onEndFn;
    /* 0x0cc */ u8 pad_cc[VT_GAP(VT_OFF(unk_c4) + sizeof(Unk_020238b0_Fn), updateFn)];
    /* 0x0ec */ Unk_020238b0_Fn updateFn;
    /* 0x0f4 */ Unk_020238b0_Fn closeFn;
    /* 0x0fc */ VillagerActor *actor;
    /* 0x100 */ char topicFile[0x11e - 0x100];
    /* 0x11e */ u8 topicIndex;
    /* 0x11f */ u8 pad_11f;
    /* 0x120 */ u16 itemFromPlayer;
    /* 0x122 */ u8 pad_122[VT_GAP(VT_OFF(itemFromPlayer) + 2, memoryIndex)];
    /* 0x124 */ s32 memoryIndex;
    /* 0x128 */ union { void *memory; void *unk_128_p; u32 unk_128; };
    /* 0x12c */ s32 partnerMemoryIndex;
    /* 0x130 */ void *partnerMemory;
    /* 0x134 */ union { void *unk_134; u32 unk_134_w; };
    /* 0x138 */ u8 noMemoryUpdate;
    /* 0x139 */ u8 pad_139[VT_GAP(VT_OFF(unk_138) + 1, choiceValues)];
    /* 0x13c */ void *choiceValues[5];
    /* 0x150 */ void *pad_150;  // (VillagerTalk::errandRecord)
    /* 0x154 */ u8 deliveryMinutes;
    /* 0x155 */ u8 deliveryTimeIndex;
    /* 0x156 */ u16 pendingSe;
    /* 0x158 */ VillagerId *deliveryRecipient;
    /* 0x15c */ PlayerErrandSlot *errandSlot;
    /* 0x160 */ PlanErrand *planErrand;
    /* 0x164 */ u8 pad_164[VT_GAP(VT_OFF(unk_160) + sizeof(void *), unk_168)];
    /* 0x168 */ Unk_020238b0_Fn customFn0;
    /* 0x170 */ Unk_020238b0_Fn customFn1;
    /* 0x178 */ Unk_020238b0_Fn customFn2;
    /* 0x180 */ Unk_020238b0_Fn customFn3;
    /* 0x188 */ Unk_020238b0_Fn customFn4;
    /* 0x190 */ u8 partnerMask;
    /* 0x191 */ u8 pad_191[3];
    /* 0x194 */ s32 partnerCount;
    /* 0x198 */ u16 itemToPlayer;
    /* 0x19a */ u8 itemToPlayerIsReceived;
    /* 0x19b */ u8 pad_19b;
    /* 0x19c */ union { s32 price; u32 unk_19c_w; };
};

// each field where VillagerTalk has its counterpart (the class above is a view of a VillagerTalk object)
#define VT_SAME(view, real) \
    static_assert(__builtin_offsetof(VillagerTalkRequestItemTopics, view) == VT_OFF(real), "VillagerTalk view: " #view)
VT_SAME(window, window);
VT_SAME(selectFn, selectFn);
VT_SAME(onEndFn, unk_c4);
VT_SAME(updateFn, updateFn);
VT_SAME(closeFn, closeFn);
VT_SAME(actor, actor);
VT_SAME(topicFile, topicFile);
VT_SAME(topicIndex, topicIndex);
VT_SAME(itemFromPlayer, itemFromPlayer);
VT_SAME(memoryIndex, memoryIndex);
VT_SAME(memory, unk_128_p);
VT_SAME(partnerMemoryIndex, partnerMemoryIndex);
VT_SAME(partnerMemory, unk_130_p);
VT_SAME(unk_134, unk_134_p);
VT_SAME(noMemoryUpdate, unk_138);
VT_SAME(choiceValues, choiceValues);
VT_SAME(pad_150, errandRecord);
VT_SAME(deliveryMinutes, unk_154);
VT_SAME(deliveryTimeIndex, unk_155);
VT_SAME(pendingSe, unk_156);
VT_SAME(deliveryRecipient, unk_158);
VT_SAME(errandSlot, unk_15c);
VT_SAME(planErrand, unk_160);
VT_SAME(customFn0, unk_168);
VT_SAME(customFn4, unk_188);
VT_SAME(partnerMask, pad_190);
VT_SAME(itemToPlayer, itemToPlayer);
VT_SAME(itemToPlayerIsReceived, itemToPlayerIsReceived);
VT_SAME(price, unk_19c);
static_assert(__builtin_offsetof(VillagerTalkRequestItemTopics, partnerCount) == VT_OFF(pad_190) + 4, "VillagerTalk view");
static_assert(sizeof(VillagerTalkRequestItemTopics) <= sizeof(VillagerTalk), "VillagerTalk view: larger than the object");
#undef VT_SAME

#endif // TALK_VILLAGERTALKREQUESTITEMTOPICS_H
