#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
    void OnTitleScreen(u8 roomId, bool u0, bool u1, bool u2);
    void InvalidGiveItem(bool var1, Item* item, u32 data);
    u32 InvalidPickStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy);
    u32 InvalidDropStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow);
    void InvalidSpriteStop(u32 pData, Item *SpriteItem);
    bool InvalidHoleStop(Item* item, Item Hole);
    bool InvalidItemStop(Item* item);
    bool ConvertFlower(Item *item);
    bool IsItemReplaceable(Item *item);
    void NameFunc(u32 u0, u32 u1, u32 u2);
    bool IsItemDroppable(u32 ItemData, Item *ItemID, int SecondaryItemFlag);
    bool IsItemPlantable(u32 ItemData, Item *ItemID);
    int CatalogPatch_Keyboard(u32 u0, u32 u1, u32 u2);
    bool CatalogPatch_SearchFunction(void);
    bool IsSTARTDown(u32 data, u32 key);
    void SetTitle(u32 dataParam, u32 *stack);
    void SetText(u32 dataParam, u32 *stack);
    const char* SetProDesignStyle(Item *ItemID, u32 data, u32 data2);
    bool FruitStacking(u32 InvAddress, int slot, Item *item, u32 ItemLock, u32 ItemToReplace);
}