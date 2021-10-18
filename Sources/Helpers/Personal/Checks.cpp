#include "Helpers/Address.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/ACSystem.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Wrapper.hpp"
#include "RegionCodes.hpp"

extern "C" bool __IsPlayerHouse() {
	u8 pID = (u8)CTRPluginFramework::Player::GetPlayerStatus(4);
	u8 stageID = CTRPluginFramework::GameHelper::RoomCheck();

	static const u8 r_Array[24] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };

	int i = 0;

	if(pID < 4) {
		while(i < 6) {
			if(r_Array[i + pID * 6] == stageID) 
				return 1;

			i++;
		}
	}
	return 0;
}

namespace CTRPluginFramework {
//Hook invalid pickup
	u32 InvalidPickStop(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy) {	
		if(IDList::ItemValid((*ItemToReplace & 0xFFFFFFFF), true)) {
			if((ID == 0xA) || (ID == 0x12) || (ID == 0x13)) {
				if((*ItemToPlace & 0xFFFF) == 0x7FFE) {
					*ItemToPlace = 0x80007FFE;
					*ItemToShow = 0x80007FFE;
				}
			}

			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			return func.Call<u32>(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow, worldx, worldy, 0, 0, 0, 0, 0);
		}
		return 0xFFFFFFFF;
	}
//Hook invalid drop
	u32 InvalidDropStop(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow) {
		if(IDList::ItemValid((*ItemToPlace & 0xFFFFFFFF))) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			return func.Call<u32>(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow);
		}

		return 0xFFFFFFFF;
	}

//hook invalid show off	
	u32 InvalidShowOffStop(u32 pOffset, u32 ItemOffset) {
		if(IDList::ItemValid(*(u32 *)ItemOffset)) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			return func.Call<u32>(pOffset, ItemOffset);
		}
		return 0;
	}
	
//Hook invalid eat
	u32 InvalidEatStop(u32 pOffset, u32 ItemOffset, u32 InvData, u32 u0) {			
		if(IDList::ItemValid(*(u32 *)ItemOffset)) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			return func.Call<u32>(pOffset, ItemOffset, InvData, u0);
		}
		return 0;
	}
//Hook to initialize	
	void InvalidSpriteStop(u32 pData, u32 SpriteItem) {
		if(IDList::ItemValid(*(u32 *)SpriteItem)) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			func.Call<void>(pData, SpriteItem);
		}
	}
//Hook for hole check
	bool InvalidHoleStop(u32* Item, u32 Hole) {
		if(IsIndoorsBool) 
			return true;
		
		return((*Item & 0xFFFF7FFF) == Hole);
	}
//Hook for invalid item check
	bool InvalidItemStop(u32* Item) {
		if(!IDList::ItemValid(*Item, false)) {
			//Mannequin will not be removed
			if(!IDList::ValidID(*Item, 0x30CC, 0x30D1))
				return false;
		}
		return true;
	}
	
	bool ConvertFlower(u32 *Item) {
		if(IDList::ValidID((*Item & 0xFFFF), 0x9F, 0xCB)) 
			GameHelper::ToIndoorFlowers(*Item);

		return true;
	}

//Hook to check if item is replacable
	bool IsItemReplaceable(u32 *item) {
		//If item is replace all		 || if item is ID to replace || if item is 7FFE					 and ID to replace is 7FFE
		if(ItemIDToReplace == 0xFFFFFFFF || *item == ItemIDToReplace || (((*item & 0x7FFE) == 0x7FFE) && ItemIDToReplace == 0x7FFE)) 
			return true;
			
		return false;
	}

	void NameFunc(u32 r0, u32 r1, u32 r2) {		
		u32 itemslotid = 0x7FFE;
		u8 slot = 0;
		if(Inventory::GetSelectedSlot(slot)) {
			if(Inventory::ReadSlot(slot, itemslotid)) {
				itemslotid = itemslotid & 0xFFFF;
				if(GameHelper::IsOutdoorItem(itemslotid) || itemslotid == 0x3729) {
					std::string name = "";
					if(IDList::GetSeedName(itemslotid, name)) {
						u32 InvPointer = *(u32 *)(GameHelper::BaseInvPointer() + 0xC);
						Process::Write32(InvPointer + 0xCFC, 0x000E000E);
						Process::Write32(InvPointer + 0xD00, 0x00040000);
						Process::Write32(InvPointer + 0xD04, 0xCD030100);

						Process::WriteString(InvPointer + 0xD08, IDList::ItemValid(itemslotid, false) ? name : "Invalid Item", StringFormat::Utf16);
					}					
				}
			}
		}
		
	//loads box
		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        func.Call<void>(r0, r1, r2);
	}

	bool IsItemDroppable(u32 ItemData, u32 *ItemID, int SecondaryItemFlag) {
		if(IDList::ValidID((*ItemID & 0xFFFF), 0x98, 0x9E)) 
			return true;
		else if((*ItemID & 0xFFFF) == 0xFD)
			return true;
		else if(IDList::ValidID((*ItemID & 0xFFFF), 0x228E, 0x234B)) 
			return true;
		else if(IDList::ValidID((*ItemID & 0xFFFF), 0x340C, 0x34CD)) 
			return true;

		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        return func.Call<bool>(ItemData, ItemID, SecondaryItemFlag);
	}

	bool IsItemPlantable(u32 ItemData, u32 *ItemID) {
	//seed items which should be plantable
		if(IDList::ValidID((*ItemID & 0xFFFF), 0, 0x97)) 
			return true;
		else if(IDList::ValidID((*ItemID & 0xFFFF), 0x9F, 0xFC)) 
			return true;

		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        return func.Call<bool>(ItemData, ItemID);
	}

	int CatalogPatch_Keyboard(u32 u0, u32 u1, u32 u2) {
		if(!GameHelper::IsInRoom(0x38) && !GameHelper::IsInRoom(0x39) && !GameHelper::IsInRoom(0x3A) && !GameHelper::IsInRoom(0x3B) && !GameHelper::IsInRoom(0x3C)) {
			OSD::Notify("Search function is currently not supported!", Color::Red);
			return 0;
		}

		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        return func.Call<int>(u0, u1, u2);
	} 
//basically "forces" a B press directly for the search function to break
	bool CatalogPatch_SearchFunction(void) {
		if(!GameHelper::IsInRoom(0x38) && !GameHelper::IsInRoom(0x39) && !GameHelper::IsInRoom(0x3A) && !GameHelper::IsInRoom(0x3B) && !GameHelper::IsInRoom(0x3C)) 
			return 1;
		
		return ACSystem::IsKeyDown(GameKey::B);
	}

	bool IsSTARTPressed(u32 data, u32 key) {
		return Controller::IsKeyPressed(Key::Start);
	}

    void SetTitle(u32 dataParam, u32 *stack) {
        Process::WriteString(stack[1], "Did you know?", StringFormat::Utf16);

		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        func.Call<void>(dataParam, stack);
	}

    void SetText(u32 dataParam, u32 *stack) {
        Process::WriteString(stack[1], "I have no clue what to write here", StringFormat::Utf16);

		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        func.Call<void>(dataParam, stack);
    }
}