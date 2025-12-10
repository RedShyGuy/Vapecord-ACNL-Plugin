#include "Address/Address.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/ACSystem.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Wrapper.hpp"
#include "Files.h"
#include "Helpers/Checks.hpp"
#include "RuntimeContext.hpp"

extern "C" bool __IsPlayerHouse() {
	u8 pID = (u8)CTRPluginFramework::Player::GetPlayerStatus(4);
	u8 stageID = CTRPluginFramework::Player::GetRoom(4);

	static const u8 r_Array[24] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };

	int i = 0;

	if(pID < 4) {
		while(i < 6) {
			if(r_Array[i + pID * 6] == stageID) {
				return 1;
			}

			i++;
		}
	}
	return 0;
}

namespace CTRPluginFramework {
//Hook invalid pickup
	u32 InvalidPickStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy) {	
		if(ItemToReplace->isValid()) {
			if((ID == 0xA) || (ID == 0x12) || (ID == 0x13)) {
				if(ItemToPlace->ID == 0x7FFE) {
					*ItemToPlace = {0x7FFE, 0x8000};
					*ItemToShow = {0x7FFE, 0x8000};
				}
			}

			const HookContext &curr = HookContext::GetCurrent();
			static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
			return func.Call<u32>(ID, ItemToReplace, ItemToPlace, ItemToShow, worldx, worldy, 0, 0, 0, 0, 0);
		}
		return 0xFFFFFFFF;
	}
//Hook invalid give item 
	void InvalidGiveItem(bool var1, Item* item, u32 data) {
		bool isItemBuried = (item->Flags >> 12) == 8;
		if (isItemBuried) {
			item->Flags &= 0x0FFF;
		}

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		func.Call<void>(var1, item, data);
	}
//Hook invalid drop
	u32 InvalidDropStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow) {
		if(ItemToPlace->isValid()) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
			return func.Call<u32>(ID, ItemToReplace, ItemToPlace, ItemToShow);
		}

		return 0xFFFFFFFF;
	}
//Hook to initialize
	void InvalidSpriteStop(u32 pData, Item *SpriteItem) {
		if(SpriteItem->isValid()) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
			func.Call<void>(pData, SpriteItem);
		}
	}
//Hook for hole check
	bool InvalidHoleStop(Item* item, Item Hole) {
		if(RuntimeContext::getInstance()->isIndoors()) {
			return true;
		}
		
		return(Item{u16(item->ID & 0x7FFF), item->Flags} == Hole);
	}
//Hook for invalid item check
	bool InvalidItemStop(Item* item) {
		if(!item->isValid(false)) {
			//Mannequin will not be removed
			if(!IDList::ValidID(item->ID, 0x30CC, 0x30D1)) {
				return false;
			}
		}
		return true;
	}
	
	bool ConvertFlower/*And Mushroomized Furniture*/(Item *item) {
		if(IDList::ValidID(item->ID, 0x9F, 0xCB)) {
			Game::ToIndoorFlowers(*item);
		}
		else if(IDList::ValidID(item->ID, 0x2120, 0x212A)) {
			item->ID += 0x959;
		}
		else if(item->ID == 0x211E) {
			item->ID += 0x23B;
		}
		else if(item->ID == 0x211F) {
			item->ID += 0x2D9;
		}

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		return func.Call<bool>(item);
	}

//Hook to check if item is replacable
	bool IsItemReplaceable(Item *item) {
		//If item is replace all		 		|| if item is ID to replace || if item is 7FFE					   and ID to replace is 7FFE
		if(ItemIDToReplace == ReplaceEverything || *item == ItemIDToReplace || (((item->ID & 0x7FFE) == 0x7FFE) && ItemIDToReplace.ID == 0x7FFE)) {
			return true;
		}
			
		return false;
	}

	void NameFunc(u32 r0, u32 r1, u32 r2) {		
		Item itemslotid = {0x7FFE, 0};
		u8 slot = 0;

		if(Inventory::GetSelectedSlot(slot)) {
			if(Inventory::ReadSlot(slot, itemslotid)) {
				itemslotid.Flags = 0;
				if(Game::IsOutdoorItem(itemslotid) || itemslotid.ID == 0x3729) {
					std::string name = itemslotid.GetName();
					u32 InvPointer = *(u32 *)(Game::BaseInvPointer() + 0xC);
					Process::Write32(InvPointer + 0xCFC, 0x000E000E);
					Process::Write32(InvPointer + 0xD00, 0x00040000);
					Process::Write32(InvPointer + 0xD04, 0xCD030100);
 
					Process::WriteString(InvPointer + 0xD08, itemslotid.isValid(false) ? name : "Invalid Item", StringFormat::Utf16);				
				}
			}
		}
		
	//loads box
		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        func.Call<void>(r0, r1, r2);
	}

	bool IsItemDroppable(u32 ItemData, Item *ItemID, int SecondaryItemFlag) {
		if(IDList::ValidID(ItemID->ID, 0x98, 0x9E)) {
			return true;
		}
		else if(ItemID->ID == 0xFD) {
			return true;
		}
		else if(IDList::ValidID(ItemID->ID, 0x228E, 0x234B)) {
			return true;
		}
		else if(IDList::ValidID(ItemID->ID, 0x340C, 0x34CD)) {
			return true;
		}

		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        return func.Call<bool>(ItemData, ItemID, SecondaryItemFlag);
	}

	bool IsItemPlantable(u32 ItemData, Item *ItemID) {
	//seed items which should be plantable
		if(IDList::ValidID(ItemID->ID, 0, 0x97)) {
			return true;
		}
		else if(IDList::ValidID(ItemID->ID, 0x9F, 0xFC)) {
			return true;
		}

		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        return func.Call<bool>(ItemData, ItemID);
	}

	int CatalogPatch_Keyboard(u32 u0, u32 u1, u32 u2) {
		if(!Game::IsGameInRoom(0x38) && !Game::IsGameInRoom(0x39) && !Game::IsGameInRoom(0x3A) && !Game::IsGameInRoom(0x3B) && !Game::IsGameInRoom(0x3C)) {
			OSD::Notify("Search function is currently not supported!", Color::Red);
			return 0;
		}

		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        return func.Call<int>(u0, u1, u2);
	} 
//basically "forces" a B press directly for the search function to break
	bool CatalogPatch_SearchFunction(void) {
		if(!Game::IsGameInRoom(0x38) && !Game::IsGameInRoom(0x39) && !Game::IsGameInRoom(0x3A) && !Game::IsGameInRoom(0x3B) && !Game::IsGameInRoom(0x3C)) {
			return 1;
		}
		
		return ACSystem::IsKeyDown(GameKey::B);
	}

	bool IsSTARTDown(u32 data, u32 key) {
		return Controller::IsKeyDown(Key::Start);
	}

    void SetTitle(u32 dataParam, u32 *stack) {
        Process::WriteString(stack[1], "Did you know?", StringFormat::Utf16);

		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        func.Call<void>(dataParam, stack);
	}

    void SetText(u32 dataParam, u32 *stack) {
        Process::WriteString(stack[1], "I have no clue what to write here", StringFormat::Utf16);

		const HookContext &curr = HookContext::GetCurrent();
        static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        func.Call<void>(dataParam, stack);
    }

	const char* SetProDesignStyle(Item *ItemID, u32 data, u32 data2) {
		switch(ItemID->ID) {
			case 0x33AA: //Umbrella
				return "icn_313";

			case 0x33AB: //Horned Hat
			case 0x33AC: //Female Hat
			case 0x33AD: //Knit Hat
				return "icn_314";

			case 0x33AE: //T-Shirt
			case 0x33AF: //Tank Shirt
			case 0x33B0: //Distorted Sleeves T-Shirt
			case 0x33B1: //Long Sleeve Shirt
				return "icn_312";

			case 0x33B2: //Tank Dress
			case 0x33B3: //Sleeve Dress
			case 0x33B4: //Long Sleeve Dress
				return "icn_385";

			case 0x22: //tree (growing 1)
			case 0x23: //tree (growing 2)
			case 0x24: //tree (growing 3)
			case 0x25: //tree (growing 4)
			case 0x26: //tree
				return "icn_prsnt2_04";

			case 0x27: //cedar (growing 1)
			case 0x28: //cedar (growing 2)
			case 0x29: //cedar (growing 3)
			case 0x2A: //cedar (growing 4)
			case 0x2B: //cedar
				return "icn_prsnt2_02";

			case 0x2C: //coconut palm tree (growing 1)
			case 0x2D: //coconut palm tree (growing 2)
			case 0x2E: //coconut palm tree (growing 3)
			case 0x2F: //coconut palm tree (growing 3)
			case 0x30: //coconut palm tree
				return "icn_prsnt2_03";

			case 0x31: //banana palm tree (growing 1)
			case 0x32: //banana palm tree (growing 2)
			case 0x33: //banana palm tree (growing 3)
			case 0x34: //banana palm tree (growing 4)
			case 0x35: //banana palm tree
				return "icn_prsnt2_03";

			case 0x36: //bamboo (growing 1)
			case 0x37: //bamboo (growing 2)
			case 0x38: //bamboo (growing 3)
			case 0x39: //bamboo
				return "icn_prsnt2_01";

			case 0xCD: //rafflesia
				return "icn_prsnt_04";
		}

		if (ItemID->ID >= 1 && ItemID->ID <= 6) { //wilted trees
			return  "icn_prsnt2_06";
		}

		if (ItemID->ID >= 0xCE && ItemID->ID <= 0xF8) { //wilted flowers
			return  "icn_prsnt2_06";
		}

		if (ItemID->ID >= 7 && ItemID->ID <= 0x21) { //stumps
			return  "icn_prsnt_01";
		}

		if (ItemID->ID >= 0x3A && ItemID->ID <= 0x75) { //trees
			return "icn_prsnt2_04";
		}

		if (ItemID->ID >= 0x76 && ItemID->ID <= 0x7B) { //cedar trees
			return "icn_prsnt2_02";
		}

		if (ItemID->ID >= 0x80 && ItemID->ID <= 0x97) { //bushes
			return "icn_prsnt2_05";
		}

		if (ItemID->ID >= 0x7C && ItemID->ID <= 0x7F) { //weed
			return "icn_prsnt_03";
		}

		if (ItemID->ID >= 0x98 && ItemID->ID <= 0x9C) { //rocks
			return "icn_prsnt_02";
		}

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		return func.Call<const char*>(ItemID, data, data2);
	}                                                                                                                                                                                                                             

	//2001 - 2012 = Fruits | Get stacked into Fruit Basket (Not Flags, nor Item IDs)

	//2018 - 2029 = Fruit Baskets | Have flags indicating amount (8 is max, if more gets "0-Basket")
	 
	//223F - 2282 = letter paper | Have flags indicating amount (3 is max, works more but game only allows 3)

	//2283 - 228C = turnips | Have their own ID's for the amount (Not Flags)
	/**
	 * Currently only for fruits
	 * Works, only missing is that if the inventory is full,
	 * with non full baskets, it will still tell you that the inventory is full,
	 * 
	 * This method needs to be hooked onto 0x724e68 at 0x59A1F8
	 * That method returns the first free slot in the inventory, if there is not a free slot it returns -1
	 * Though the next issue is that this method does not contain the item, so there is no way of telling how many fruits 
	 * the player wants to pick up to calculate if it would fit into the baskets in the inventory.
	 * 
	 * The item is located at Register 5 when calling, could get it via moving register 5 to 1 for example
	 * 
	 * There still needs to be one more patch, as even if the inventory full message gets ignored via the patch, 
	 * the picked up fruits still dont get put into the inventory, there is likely one more place where it leads to the 
	 * put into inventory method
	*/
	/*Hook to 0x323424*/
	bool FruitStacking(u32 InvAddress, int slot, Item *item, u32 ItemLock, u32 ItemToReplace) {
		// Hook fallback helper (original call)
		auto call_original = [&](void) -> u32 {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
			return func.Call<u32>(InvAddress, slot, item, ItemLock, ItemToReplace);
		};

		const u16 fruitMin = 0x2001;
		const u16 fruitMax = 0x2012;
		const u16 basketOffset = 0x17;
		const u16 emptyID = 0x7FFE;
		const int INVENTORY_SIZE = 16;
		const int MAX_FRUITS_PER_BASKET = 9;

		if (!item) {
			return call_original();
		}

		bool isFruit = IDList::ValidID(item->ID, fruitMin, fruitMax);
		bool isBasket = IDList::ValidID(item->ID, fruitMin + basketOffset, fruitMax + basketOffset);

		if (!isFruit && !isBasket) {
			return call_original();
		}

		u16 basketID = isFruit ? (item->ID + basketOffset) : item->ID;
		int fruitsToAdd = 0;
		if (isFruit) {
			fruitsToAdd = 1;
		} 
		else {
			int existingFlag = (int)item->Flags;
			int contained = existingFlag + 1; // mapping flag -> fruit count
			if (contained < 1) {
				contained = 1;
			}
			if (contained > MAX_FRUITS_PER_BASKET) {
				contained = MAX_FRUITS_PER_BASKET;
			}
			fruitsToAdd = contained;
		}

		// --- 1) Read whole inventory snapshot ---
		Item snapshot[INVENTORY_SIZE];
		for (int i = 0; i < INVENTORY_SIZE; ++i) {
			Inventory::ReadSlot(i, snapshot[i]); // read current state
		}

		// make a working copy we will modify (simulate)
		Item working[INVENTORY_SIZE];
		for (int i = 0; i < INVENTORY_SIZE; ++i) {
			working[i] = snapshot[i];
		}

		// Helper-lambda: get number of fruits in a basket item
		auto basketFruits = [&](const Item &it)->int {
			int f = (int)it.Flags + 1;
			if (f < 1) {
				f = 1;
			}
			if (f > MAX_FRUITS_PER_BASKET) {
				f = MAX_FRUITS_PER_BASKET;
			}
			return f;
		};

		// Helper-lambda: set basket flags from fruit count
		auto setBasketFromFruits = [&](Item &it, int fruits) {
			if (fruits < 1) {
				fruits = 1;
			}
			if (fruits > MAX_FRUITS_PER_BASKET) {
				fruits = MAX_FRUITS_PER_BASKET;
			}
			it.ID = basketID;
			it.Flags = (u8)(fruits - 1); // flag = fruits - 1
		};

		int remaining = fruitsToAdd;

		// --- 2) Pass: fill existing baskets (left-to-right slot order) ---
		for (int i = 0; i < INVENTORY_SIZE && remaining > 0; ++i) {
			if (working[i].ID == basketID) {
				int cur = basketFruits(working[i]);
				if (cur < MAX_FRUITS_PER_BASKET) {
					int canAdd = MAX_FRUITS_PER_BASKET - cur;
					int toAdd = (remaining <= canAdd) ? remaining : canAdd;
					cur += toAdd;
					setBasketFromFruits(working[i], cur);
					remaining -= toAdd;
				}
			}
		}

		// --- 3) Pass: convert loose fruits in inventory into baskets and fill them ---
		if (remaining > 0) {
			u16 fruitID = basketID - basketOffset; // corresponding fruit id
			for (int i = 0; i < INVENTORY_SIZE && remaining > 0; ++i) {
				if (working[i].ID == fruitID) {
					// If we have at least 1 remaining, converting a loose fruit into a basket consumes 1 fruit.
					// That yields an initial basket with 2 fruits. This is valid even if remaining == 1,
					// because we can pair that single remaining fruit with the loose fruit that was in inventory.
					// Do the conversion:
					setBasketFromFruits(working[i], 2); // now holds 2 fruits
					remaining -= 1; // we used one of the fruitsToAdd to pair with this loose fruit

					// Now try to fill the newly converted basket with any additional remaining
					int now = basketFruits(working[i]); // should be 2
					if (now < MAX_FRUITS_PER_BASKET && remaining > 0) {
						int canAdd = MAX_FRUITS_PER_BASKET - now;
						int toAdd = (remaining <= canAdd) ? remaining : canAdd;
						now += toAdd;
						setBasketFromFruits(working[i], now);
						remaining -= toAdd;
					}
				}
			}
		}

		// --- 4) Compute how many new slots we'd need for the rest ---
		// Important: if remaining > 0, we will place as many full baskets (size 9) as possible,
		// and the last chunk may be 1..8 fruits. If last chunk == 1 it must be placed as a loose fruit (not a 0-flag basket).
		int neededSlots = 0;
		if (remaining > 0) {
			int temp = remaining;
			while (temp > 0) {
				int chunk = (temp >= MAX_FRUITS_PER_BASKET) ? MAX_FRUITS_PER_BASKET : temp;
				temp -= chunk;
				neededSlots++;
			}
		}

		// Count free slots in working snapshot (after conversions above)
		int freeSlots = 0;
		for (int i = 0; i < INVENTORY_SIZE; ++i) {
			if (working[i].ID == emptyID) {
				freeSlots++;
			}
		}

		// If not enough space for required new slots -> abort (no changes)
		if (neededSlots > freeSlots) {
			return 0;
		}

		// --- 5) If enough free slots, allocate new baskets or loose fruit in first free slots (left-to-right) ---
		int remainingToPlace = remaining;
		for (int i = 0; i < INVENTORY_SIZE && remainingToPlace > 0; ++i) {
			if (working[i].ID == emptyID) {
				int put = (remainingToPlace >= MAX_FRUITS_PER_BASKET) ? MAX_FRUITS_PER_BASKET : remainingToPlace;
				// IMPORTANT FIX: if put == 1, do NOT create a basket with flag 0.
				// Instead, write a loose fruit item (fruitID) with Flags = 0.
				if (put == 1) {
					working[i].ID = (basketID - basketOffset); // fruit ID
					working[i].Flags = 0;
				} 
				else {
					setBasketFromFruits(working[i], put);
				}
				remainingToPlace -= put;
			}
		}
		remaining = remainingToPlace; // should be 0 now

		// --- 6) Commit: write only slots that changed compared to snapshot ---
		for (int i = 0; i < INVENTORY_SIZE; ++i) {
			if (snapshot[i].ID != working[i].ID || snapshot[i].Flags != working[i].Flags) {
				Inventory::WriteSlot(i, working[i]);
			}
		}

		return 1;
	}
}
