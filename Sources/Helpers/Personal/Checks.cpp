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
	u32 InvalidPickStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy) {	
		if(IDList::ItemValid(*ItemToReplace, true)) {
			if((ID == 0xA) || (ID == 0x12) || (ID == 0x13)) {
				if(ItemToPlace->ID == 0x7FFE) {
					*ItemToPlace = {0x7FFE, 0x8000};
					*ItemToShow = {0x7FFE, 0x8000};
				}
			}

			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
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
		static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
		func.Call<void>(var1, item, data);
	}
//Hook invalid drop
	u32 InvalidDropStop(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow) {
		if(IDList::ItemValid(*ItemToPlace)) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			return func.Call<u32>(ID, ItemToReplace, ItemToPlace, ItemToShow);
		}

		return 0xFFFFFFFF;
	}
//Hook to initialize
	void InvalidSpriteStop(u32 pData, Item *SpriteItem) {
		if(IDList::ItemValid(*SpriteItem)) {
			const HookContext &curr = HookContext::GetCurrent();
			static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
			func.Call<void>(pData, SpriteItem);
		}
	}
//Hook for hole check
	bool InvalidHoleStop(Item* item, Item Hole) {
		if(IsIndoorsBool) 
			return true;
		
		return(Item{u16(item->ID & 0x7FFF), item->Flags} == Hole);
	}
//Hook for invalid item check
	bool InvalidItemStop(Item* item) {
		if(!IDList::ItemValid(*item, false)) {
			//Mannequin will not be removed
			if(!IDList::ValidID(item->ID, 0x30CC, 0x30D1))
				return false;
		}
		return true;
	}
	
	bool ConvertFlower/*And Mushroomized Furniture*/(Item *item) {
		if(IDList::ValidID(item->ID, 0x9F, 0xCB)) 
			GameHelper::ToIndoorFlowers(*item);

		else if(IDList::ValidID(item->ID, 0x2120, 0x212A))
			item->ID += 0x959;
		else if(item->ID == 0x211E)
			item->ID += 0x23B;
		else if(item->ID == 0x211F)
			item->ID += 0x2D9;

		return true;
	}

//Hook to check if item is replacable
	bool IsItemReplaceable(Item *item) {
		//If item is replace all		 		|| if item is ID to replace || if item is 7FFE					   and ID to replace is 7FFE
		if(ItemIDToReplace == ReplaceEverything || *item == ItemIDToReplace || (((item->ID & 0x7FFE) == 0x7FFE) && ItemIDToReplace.ID == 0x7FFE)) 
			return true;
			
		return false;
	}

	void NameFunc(u32 r0, u32 r1, u32 r2) {		
		Item itemslotid = {0x7FFE, 0};
		u8 slot = 0;
		if(Inventory::GetSelectedSlot(slot)) {
			if(Inventory::ReadSlot(slot, itemslotid)) {
				itemslotid.Flags = 0;
				if(GameHelper::IsOutdoorItem(itemslotid) || itemslotid.ID == 0x3729) {
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

	bool IsItemDroppable(u32 ItemData, Item *ItemID, int SecondaryItemFlag) {
		if(IDList::ValidID(ItemID->ID, 0x98, 0x9E)) 
			return true;
		else if(ItemID->ID == 0xFD)
			return true;
		else if(IDList::ValidID(ItemID->ID, 0x228E, 0x234B)) 
			return true;
		else if(IDList::ValidID(ItemID->ID, 0x340C, 0x34CD)) 
			return true;

		const HookContext &curr = HookContext::GetCurrent();
        static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
        return func.Call<bool>(ItemData, ItemID, SecondaryItemFlag);
	}

	bool IsItemPlantable(u32 ItemData, Item *ItemID) {
	//seed items which should be plantable
		if(IDList::ValidID(ItemID->ID, 0, 0x97)) 
			return true;
		else if(IDList::ValidID(ItemID->ID, 0x9F, 0xFC)) 
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

	static bool WasSuspended = false;
	void SuspendCallBack(u32 param) {
	/*If Game suspenses*/
		if(!WasSuspended) {
			
			delete ItemList; //delete ItemList to clear a lot of memory to prevent memory issues if game is suspended
			ItemList = nullptr;

			WasSuspended = true;
			goto reset;
		}

		OSD::Notify("Initializing Memory", Color::Purple);
	/*If Game unsuspenses*/
		ItemList = new ItemVec();
		ReserveItemData(ItemList); //redo ItemList when game is unsuspended
		
		WasSuspended = false;

	reset:
		const HookContext &curr = HookContext::GetCurrent();
		static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
		func.Call<void>(param);
	}
}