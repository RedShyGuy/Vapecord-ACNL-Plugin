#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
    void CheckInvalidBadge(u32 data, u32 badge, int badgeType, u32 r3, u32 r4);
    void OnTitleScreen(u8 roomId, bool u0, bool u1, bool u2);
    void InvalidGiveItem(bool var1, Item* item, u32 data);
    u32 InvalidPickStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy);
    u32 InvalidDropStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow);
    void InvalidSpriteStop(u32 pData, Item *SpriteItem);
    bool InvalidHoleStop(Item* item, Item Hole);
    bool InvalidItemStop(Item* item);
    bool ConvertFlower(Item *item);
    bool IsItemReplaceable(Item *item);
    void SetHoveredItemName(u32 r0, u32 r1, u32 r2, u32 r3);
    void SetItemName(u32 u0, u32 u1, u32 u2);
    bool IsItemDroppable(u32 ItemData, Item *ItemID, int SecondaryItemFlag);
    bool IsItemPlantable(u32 ItemData, Item *ItemID);
    int CatalogPatch_Keyboard(u32 u0, u32 u1, u32 u2);
    bool CatalogPatch_SearchFunction(void);
    bool IsSTARTDown(u32 data, u32 key);
    //void SetTitle(u32 dataParam, u32 *stack);
    //void SetText(u32 dataParam, u32 *stack);
    const char* SetProDesignStyle(Item *ItemID, u32 data, u32 data2);
    bool FruitStacking(u32 InvAddress, int slot, Item *item, u32 ItemLock, u32 ItemToReplace);
}