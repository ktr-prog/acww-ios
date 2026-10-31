#include "types.h"
#ifdef HOST64
extern "C" int host_never_resetti(void);  // (modern/host/features.c: the settings screen's switch)
#define HOST_NEVER_RESETTI() host_never_resetti()
#else
#define HOST_NEVER_RESETTI() 0
#endif
#include "save/SaveData.h"
#include "game/ReddShop.h"
#include "game/Unk_020aec00.h"
#include "item/BottleLetterRecord.h"
#include "item/LetterOutbox.h"
#include "item/LetterStorage.h"
#include "item/PlayerMailbox.h"
#include "item/ReceivedLetterBlock.h"
#include "player/PlayerData.h"
#include "player/PlayerSpNpcRecord.h"
#include "room/HouseData.h"
#include "save/BbsBoard.h"
#include "save/BlancaFaceRecord.h"
#include "save/ContestRecord.h"
#include "save/LostChildRecord.h"
#include "save/MuseumData.h"
#include "save/Pattern.h"
#include "save/SaveRecord4.h"
#include "save/TownExchangeRecord.h"
#include "save/TownId.h"
#include "field/LooseSnowballs.h"
#include "game/ConstellationStore.h"
#include "game/ReddLastSale.h"
#include "game/RoostGuestRoll.h"
#include "game/TurnipMarket.h"
#include "item/ItemClassOrders.h"
#include "save/SnowmanRecords.h"
#include "town/ResettiVisitFlag.h"
#include "town/TownMap.h"
#include "town/TownStyleRecord.h"
#include "sys/Heap.h"
#include "player/PlayerId.h"
#include "proto/main/unk_0209e394.h"
#include "town/TownState.h"
#include "game/NookShop.h"
#include "save/VillagerData.h"
#include "proto/main/unk_02039b6c.h"
#include "proto/main/unk_0204c318.h"
#include "proto/main/unk_020407fc.h"
#include "proto/main/unk_02063904.h"
#include "proto/main/unk_02096c10.h"
#include "proto/main/unk_020b0774.h"
#include "proto/main/unk_020af514.h"
#include "proto/autoload_2/unk_020e8558.h"
#include "proto/main/unk_020039ec.h"
#include "proto/main/unk_02039c08.h"
#include "proto/main/unk_0203c92c.h"
#include "proto/main/unk_02070560.h"
#include "proto/main/unk_020850e0.h"
#include "proto/main/unk_02085940.h"
#include "proto/main/unk_0208eeac.h"
#include "proto/main/unk_0208f0b0.h"
#include "proto/main/unk_02095cdc.h"
#include "proto/main/unk_02097444.h"
#include "proto/main/unk_0209c4a8.h"
#include "proto/main/unk_0209d81c.h"
#include "proto/main/unk_0209dc94.h"
#include "proto/main/unk_0209eb94.h"
#include "proto/main/unk_020af3f4.h"
#include "proto/main/unk_020afeb4.h"
#include "proto/main/unk_020c00c0.h"
#include "proto/main/unk_0204cc1c.h"


// The save record's members are still byte blocks (SaveData.h): the helpers get them as their own types.
extern "C" {
void SaveData_SyncClockOffset(SaveData *p);
void Clock_Init();
void VillagerStates_SetBirthdayVisitor(s32 v);
void SaveVillagers_UpdateAllRoomInfo(void *self);
void SaveVillagers_UpdateOutdoor(SaveVillagers *self, s32 x);
void SaveVillagers_DailyUpdate(u8 *self, s32 flag);
void SaveVillagers_PlaceMissingHouses(u8 *self);
void VillagerStates_ResetErrands();
void SaveVillagers_AssignErrands(void *bb);
s32 Scene_GetCurrent();
void HappyRoom_SendWelcomeLetter();
void Weather_RerollRainSlantAlt3();
void Sky_RequestBirds();
void SaveVillagers_PickPlayerBirthdayVisitor(u8 *self, void *p);
s32 VillagerStates_GetBirthdayVisitor();
void ReddPassword_ForgetResident(u32 i);
void MI_CpuFill8(void *, s32, u32);
void AbleShop_Reset(void *s);
void ReddShop_Reset(ReddShop *g);
void TownState_Reset(TownState *p);
void NookShop_Clear(NookShop *p);
void HappyRoomDate_SetToday(u8 *out);
void SaveVillagers_Clear(u8 *self);
void HappyRoomDate_Destruct();
u8 *SaveVillagers_Destruct(u8 *self);
void *AbleShop_Destruct(void *p);
}

extern "C" const u32 sSndOutputModeTable[2] = {1, 0};


SaveData::~SaveData() {
    s32 i;
    ((SaveRecord4 *)(&footer))->destruct();
    LostChildRecord_Destruct(&lostChild);
    SaveData_DestructDateRecord();
    ((ItemClassOrders *)itemClassOrders)->~ItemClassOrders();
    HappyRoomDate_Destruct();
    ((SnowmanRecords *)snowmen)->~SnowmanRecords();
    AbleShop_Destruct(&ableShop);
    ((ReddShop *)(&reddShop))->~ReddShop();
    Weather_Destruct();
    TurnipMarket_Destruct();
    ((ReddLastSale *)reddLastSale)->~ReddLastSale();
    ((ContestRecord *)(&contestRecord))->destruct();
    RecycleBin_Destruct();
    LostAndFound_Destruct();
    TownState_Destruct();
    EventWeekSlots_Destruct();
    new (&nookShop) Unk_020aec00();
    ((MuseumData *)(&museum))->destruct();
    ((ReceivedLetterBlock *)(&receivedLetters))->~ReceivedLetterBlock();
    BottleLetterRecord_Destruct((Letter *)bottleLetter);
    new (&townStyle) TownStyleRecord();
    ((BlancaFaceRecord *)(&blancaFace))->destruct();
    for (i = 4; i-- > 0;) ChestStorage_Destruct(); // dressers[i]
    ((ConstellationStore *)constellations)->~ConstellationStore();
    ((LetterOutbox *)(&letterOutbox))->~LetterOutbox();
    for (i = 4; i-- > 0;) ((PlayerMailbox *)mailboxes[i])->~PlayerMailbox();
    ((BbsBoard *)(&bbsBoard))->~BbsBoard();
    ((TownExchangeRecord *)(&townExchange))->~TownExchangeRecord();
    ((AbleSistersPatterns *)(&ableSistersPatterns))->~AbleSistersPatterns();
    ((HouseData *)(&house))->~HouseData();
    RoostGuestRoll_Destruct();
    GulliverQuest_Destruct();
    ((TownMap *)townMap)->~TownMap();
    SaveVillagers_Destruct(villagers);
    PlayerDataArray_Destruct(&players);
    TownId_Destruct();
}

BOOL SaveData::isValid() {
    if (marker == 0x8a) {
        if (((SaveRecord4 *)(&footer))->isStateValid() != 0) return TRUE;
    }
    return FALSE;
}

BOOL SaveData::testFlag(u32 n) {
    s32 w = n >> 5;
    u32 b = n & 0x1f;
    BOOL r;
    if (w < 1) {
        r = TRUE;
        if (((r << b) & flags[w]) != 0) goto end;
    }
    r = FALSE;
end:
    return r;
}

void SaveData::setFlag(u32 n) {
    s32 w = n >> 5;
    u32 b = n & 0x1f;
    if (w < 1) {
        u32 m = 1 << b;
        *(volatile u32 *)&flags[w] = m | *(volatile u32 *)&flags[w];
    }
}

void SaveData::clearFlag(u32 n) {
    s32 w = n >> 5;
    u32 b = n & 0x1f;
    if (w < 1) {
        u32 m = ~(1 << b);
        *(volatile u32 *)&flags[w] = m & *(volatile u32 *)&flags[w];
    }
}

void SaveData::clear() {
    MI_CpuFill8(this, 0, 0x15fe0);
}

void SaveData::reset() {
    clear();
    s32 i;
    for (i = 0; i < 4; i++) resetPlayer(i);
    TownId_Clear((u16 *)townId);
    PlayerDataArray_ResetAll(&players);
    SaveVillagers_Clear(villagers);
    AbleShop_Reset(&ableShop);
    ReddShop_Reset((ReddShop *)reddShop);
    ((TownMap *)townMap)->clear();
    ((BbsBoard *)(&bbsBoard))->clear();
    ((HouseData *)(&house))->reset();
    TownState_Reset(&townState);
    Weather_SetDateToday(&weather);
    NookShop_Clear((NookShop *)nookShop);
    ((BlancaFaceRecord *)(&blancaFace))->reset();
    ((TurnipMarket *)turnipMarket)->clear();
    ((ReddLastSale *)reddLastSale)->clear();
    GulliverQuest_Clear(&gulliverQuest);
    ((ContestRecord *)(&contestRecord))->clear();
    ((LetterOutbox *)(&letterOutbox))->clear();
    LostAndFound_Clear((u16 *)lostAndFound);
    RecycleBin_Clear((u16 *)recycleBin);
    ConstellationStore_Clear((ConstellationStore *)constellations);  // (the save record keeps its bytes)
    ((BottleLetterRecord *)(&bottleLetter))->clearRecord();
    ReceivedLetter_Clear(&receivedLetters);
    HappyRoomDate_SetToday(happyRoomDate);
    ((SaveRecord4 *)(&unk_15fc5))->resetDate();
    ((SnowmanRecords *)snowmen)->clearAll();
    EventWeekSlots_Reset(eventWeekSlots);
    ((LostChildRecord *)(&lostChild))->clear();
    ((RoostGuestRoll *)roostGuestRoll)->clear();
    TownExchange_Clear(&townExchange);
}

void SaveData::resetPlayer(s32 i) {
    ((PlayerMailbox *)(&mailboxes[i]))->clear();
    ChestStorage_Clear((u16 *)dressers[i]);
    ((MuseumData *)(&museum))->releasePlayerDonations(i);
    Pattern_RemovePlayerItems(i);
    ReddPassword_ForgetResident((u8)i);
    u8 *r = (u8 *)SaveManager_GetLetterStorage();
    if (r) ((LetterStorage *)(r + i * 0x477c))->clear();
}

void SaveData::setupNewTown() {
    PlayerData *a = PlayerData_GetCurrent();
    SaveData_SyncClockOffset(this);
    TownMap_Generate((TownMap *)townMap, gCurrentHeap);
    SaveData_RefreshTownBlockMap(this);
    ((SaveRecord4 *)(&unk_15fc5))->setDateToday((void *)(0));
    a->getPlayerId()->setTown((TownId *)(&townId));
    PlayerPatterns *t = (PlayerPatterns *)a->getPatterns();
    t->initDefaultPatterns((Unk_020942c8 *)a->getPlayerId());
    SaveData_InitNew((u8 *)this);
    LooseSnowballs_Get()->reset();
    ((TownMap *)townMap)->updateGroundSeason();
    RoomEntry_Reset();
    a->setFlag(1);
    a->setFlag(0x23);
    a->clearFlag(9);
    VillagerStates_SetBirthdayVisitor(-1);
    SaveVillagers_UpdateOutdoor((SaveVillagers *)villagers, 1);
    TownSessionState_GetResettiFlag(TownSessionState_Get())->clear();
    SaveVillagers_UpdateAllRoomInfo(&villagers);
}

void SaveData::setupNewResident() {
    PlayerData *a = PlayerData_GetCurrent();
    SaveData_RefreshTownBlockMap(this);
    HouseRoomMaps_UpdateAll();
    HouseRoomMaps_BindBg();
    Clock_Init();
    a->getPlayerId()->setTown((TownId *)(&townId));
    PlayerPatterns *t = (PlayerPatterns *)a->getPatterns();
    t->initDefaultPatterns((Unk_020942c8 *)a->getPlayerId());
    a->setFlag(1);
    a->setFlag(0x23);
    a->clearFlag(9);
    SaveVillagers_DailyUpdate(villagers, 0);
    SaveVillagers_PlaceMissingHouses(villagers);
    VillagerStates_ResetErrands();
    SaveVillagers_AssignErrands(&villagers);
    LooseSnowballs_Get()->reset();
    ((TownMap *)townMap)->updateGroundSeason();
    RoomEntry_Reset();
    a->setSkyShotHits(0);
    VillagerStates_SetBirthdayVisitor(-1);
    SaveVillagers_UpdateOutdoor((SaveVillagers *)villagers, 1);
    TownSessionState_GetResettiFlag(TownSessionState_Get())->clear();
    SaveVillagers_UpdateAllRoomInfo(&villagers);
}

void SaveData::setupContinue() {
    PlayerData *a = PlayerData_GetCurrent();
    if (Scene_GetCurrent() != 6) {
        SaveData_RefreshTownBlockMap(this);
        HouseRoomMaps_UpdateAll();
        HouseRoomMaps_BindBg();
        Clock_Init();
        SaveVillagers_DailyUpdate(villagers, 0);
    }
    SaveVillagers_PlaceMissingHouses(villagers);
    VillagerStates_ResetErrands();
    SaveVillagers_AssignErrands(&villagers);
    LooseSnowballs_Get()->reset();
    ((TownMap *)townMap)->updateGroundSeason();
    RoomEntry_Reset();
    HappyRoom_SendWelcomeLetter();
    Snd_SetOutputMode(sSndOutputModeTable[PlayerOptions_IsStereo()]);
    LostChild_LoadFromTown();
    BottleLetter_WashUpThrownBottle();
    Constellation_ImportExchanged();
    LetterDelivery_DeliverReceivedLetter();
    GameStart_Clear();
    Weather_RerollRainSlantAlt3();
    Sky_RequestBirds();
    ((TurnipMarket *)turnipMarket)->spoilOnReset();
    SaveVillagers_PickPlayerBirthdayVisitor(villagers, NULL);
    SaveVillagers_UpdateOutdoor((SaveVillagers *)villagers, 1);
    if (Scene_GetCurrent() == 6) {
        if (VillagerStates_GetBirthdayVisitor() == -1) {
            if (a->testFlag(0x23) == 0) {
                // (the host's setting "Never get Resetti": the last session's end without saving forgiven - no reset
                // counted, no visit)
                if (a->testFlag(2) != 0 && !HOST_NEVER_RESETTI()) {
                    ((PlayerSpNpcRecord *)a->getSpNpcRecord())->addResetCount();
                    TownSessionState_GetResettiFlag(TownSessionState_Get())->set(1);
                }
            }
        }
    }
    a->setFlag(2);
    a->sendForeignVillagerLetter();
    if (Scene_GetCurrent() != 6) {
        SaveVillagers_UpdateAllRoomInfo(&villagers);
    }
}

