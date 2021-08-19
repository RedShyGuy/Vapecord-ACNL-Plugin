#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
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

			static FUNCT func(Code::PlaceItemOffset);
			return func.Call<u32>(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow, worldx, worldy, 0, 0, 0, 0, 0);
		}
		return 0xFFFFFFFF;
	}
//Hook invalid drop
	u32 InvalidDropStop(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow) {
		if(IDList::ItemValid((*ItemToPlace & 0xFFFFFFFF))) {
			static FUNCT func(Code::PlaceItemOffset);
			return func.Call<u32>(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow);
		}

		return 0xFFFFFFFF;
	}

//hook invalid show off	
	u32 InvalidShowOffStop(u32 pOffset, u32 ItemOffset) {
		if(IDList::ItemValid(*(u32 *)ItemOffset)) {
			static const u32 ShowOffFunc = Region::AutoRegion(0x6523B0, 0x6518D8, 0x6513E8, 0x6513E8, 0x650EA8, 0x650EA8, 0x650A50, 0x650A50);
			static FUNCT func(ShowOffFunc);
			return func.Call<u32>(pOffset, ItemOffset);
		}
		return 0;
	}
	
//Hook invalid eat
	u32 InvalidEatStop(u32 pOffset, u32 ItemOffset, u32 InvData, u32 u0) {			
		if(IDList::ItemValid(*(u32 *)ItemOffset)) {
			static const u32 EatFunc = Region::AutoRegion(0x650E88, 0x6503B0, 0x64FEC0, 0x64FEC0, 0x64F980, 0x64F980, 0x64F528, 0x64F528);

			static FUNCT func(EatFunc);
			return func.Call<u32>(pOffset, ItemOffset, InvData, u0);
		}
		return 0;
	}
//Hook to initialize	
	void InvalidSpriteStop(u32 pData, u32 SpriteItem) {
		if(IDList::ItemValid(*(u32 *)SpriteItem)) {
			static FUNCT func(Code::CopyPasteFunc);
			func.Call<void>(pData, SpriteItem);
		}
	}
//Hook for hole check
	bool InvalidHoleStop(u32* Item, u32 Hole) {
		if(*(bool *)IndoorsBool) 
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

	void NameFunc(u32 u0, u32 u1, u32 u2) {		
		u16 itemslotid = Inventory::ReadSlot(Inventory::GetSelectedSlot());
		if(GameHelper::IsOutdoorItem(itemslotid) || itemslotid == 0x3729) {
			std::string name = IDList::GetSeedName(itemslotid);
			if(name != "error") {
				u32 InvPointer = *(u32 *)(GameHelper::BaseInvPointer() + 0xC);
				Process::Write32(InvPointer + 0xCFC, 0x000E000E);
				Process::Write32(InvPointer + 0xD00, 0x00040000);
				Process::Write32(InvPointer + 0xD04, 0xCD030100);
				
				if(!IDList::ItemValid(itemslotid, false)) 
					Process::WriteString(InvPointer + 0xD08, "Invalid Item", StringFormat::Utf16);
				else
					Process::WriteString(InvPointer + 0xD08, name, StringFormat::Utf16);
			}
		}
		
	//loads box
		static const u32 OpenBox = Region::AutoRegion(0x5D5548, 0x5D4A78, 0x5D4590, 0x5D4590, 0x5D3DC4, 0x5D3DC4, 0x5D3A98, 0x5D3A98);

		static FUNCT func(OpenBox);
		func.Call<void>(u0, u1, u2);
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

		static const u32 O_IsItemDroppable = Region::AutoRegion(0x26FEA0, 0x26F8E4, 0x26FE9C, 0x26FE9C, 0x26FDA8, 0x26FDA8, 0x26FD74, 0x26FD74);
		static FUNCT func(O_IsItemDroppable);
		return func.Call<bool>(ItemData, ItemID, SecondaryItemFlag);
	}

	bool IsItemPlantable(u32 ItemData, u32 *ItemID) {
	//seed items which should be plantable
		if(IDList::ValidID((*ItemID & 0xFFFF), 0, 0x97)) 
			return true;
		else if(IDList::ValidID((*ItemID & 0xFFFF), 0x9F, 0xFC)) 
			return true;

		static const u32 Plant = Region::AutoRegion(0x26FE64, 0x26F8A8, 0x26FE60, 0x26FE60, 0x26FD6C, 0x26FD6C, 0x26FD38, 0x26FD38);
		static FUNCT func(Plant);
		return func.Call<bool>(ItemData, ItemID);
	}

	int CatalogPatch_Keyboard(u32 u0, u32 u1, u32 u2) {
		if(!GameHelper::IsInRoom(0x38) && !GameHelper::IsInRoom(0x39) && !GameHelper::IsInRoom(0x3A) && !GameHelper::IsInRoom(0x3B) && !GameHelper::IsInRoom(0x3C)) {
			OSD::Notify("Search function is currently not supported!", Color::Red);
			return 0;
		}

		static const u32 address = Region::AutoRegion(0x52A32C, 0x529C80, 0x529374, 0x529374, 0x528C60, 0x528C60, 0x528984, 0x528984);
		static FUNCT func(address);
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

	void SetHook(Hook &hook, u32 address, u32 callback, u32 flags) {
		hook.Initialize(address, (u32)callback);
		hook.SetFlags(flags);
		hook.Enable();
	}

//check for accidental invalid item eat/drop/show off etc
	void AccidentalCrashCheck(void) {
		u32 found = Utils::Search<u32>(0x07000000, 0xF608B, { 0xE5C01068, 0xE12FFF1E });
		*(u32 *)found = 0xE1A00000;

		static Hook SaveButtonCheck;
		SetHook(SaveButtonCheck, Code::nosave - 0x10, (u32)IsSTARTPressed, USE_LR_TO_RETURN);

		static Hook CatalogPHook1, CatalogPHook2;
		static const u32 CatalogPOffset1 = Region::AutoRegion(0x21C408, 0x21BE4C, 0x21C428, 0x21C428, 0x21C348, 0x21C348, 0x21C314, 0x21C314);
		static const u32 CatalogPOffset2 = Region::AutoRegion(0x21C0AC, 0x21BAF0, 0x21C0CC, 0x21C0CC, 0x21BFEC, 0x21BFEC, 0x21BFB8, 0x21BFB8);

		SetHook(CatalogPHook1, CatalogPOffset1, (u32)CatalogPatch_Keyboard, USE_LR_TO_RETURN);
		SetHook(CatalogPHook2, CatalogPOffset2, (u32)CatalogPatch_SearchFunction, USE_LR_TO_RETURN);
		
		static Hook IPHook;	
		static const u32 IPOffset = Region::AutoRegion(0x59A258, 0x599770, 0x5992A0, 0x5992A0, 0x598B90, 0x598B90, 0x598864, 0x598864);
		SetHook(IPHook, IPOffset, (u32)InvalidPickStop, USE_LR_TO_RETURN);
		
		static Hook IEHook;
		static const u32 IEOffset = Region::AutoRegion(0x5C0A38, 0x5BFF68, 0x5BFA80, 0x5BFA80, 0x5BF370, 0x5BF370, 0x5BF044, 0x5BF044);
		SetHook(IEHook, IEOffset, (u32)InvalidEatStop, USE_LR_TO_RETURN);
		
		static Hook ISOHook;
		static const u32 ISOOffset = Region::AutoRegion(0x5C0AFC, 0x5C002C, 0x5BFB44, 0x5BFB44, 0x5BF434, 0x5BF434, 0x5BF108, 0x5BF108);
		SetHook(ISOHook, ISOOffset, (u32)InvalidShowOffStop, USE_LR_TO_RETURN);
		
		static Hook InvDropHook, InvPlantHook;
		static const u32 InvDropOffset = Region::AutoRegion(0x597850, 0x596D68, 0x596898, 0x596898, 0x596188, 0x596188, 0x595E5C, 0x595E5C);
		static const u32 InvPlantOffset = Region::AutoRegion(0x597724, 0x596C3C, 0x59676C, 0x59676C, 0x59605C, 0x59605C, 0x595D30, 0x595D30);
		SetHook(InvDropHook, InvDropOffset, (u32)InvalidDropStop, USE_LR_TO_RETURN);
		SetHook(InvPlantHook, InvPlantOffset, (u32)InvalidDropStop, USE_LR_TO_RETURN);
		
		static Hook ISHook;
		static const u32 ISOffset = Region::AutoRegion(0x670EA0, 0x6703C8, 0x66FED8, 0x66FED8, 0x66F998, 0x66F998, 0x66F540, 0x66F540);
		SetHook(ISHook, ISOffset, (u32)InvalidSpriteStop, USE_LR_TO_RETURN);
		
		static Hook IHHook;
		static const u32 IHOffset = Region::AutoRegion(0x5980F4, 0x59760C, 0x59713C, 0x59713C, 0x596A2C, 0x596A2C, 0x596700, 0x596700);
		SetHook(IHHook, IHOffset, (u32)InvalidHoleStop, USE_LR_TO_RETURN);
		
		static Hook IIHook;
		static const u32 IIOffset = Region::AutoRegion(0x72511C, 0x724464, 0x724124, 0x7240FC, 0x7238B8, 0x723890, 0x723460, 0x723460);
		SetHook(IIHook, IIOffset, (u32)InvalidItemStop, USE_LR_TO_RETURN);
		
		static Hook CFHook;
		static const u32 CFOffset = Region::AutoRegion(0x323514, 0x322F28, 0x322868, 0x322868, 0x32251C, 0x32251C, 0x3223D4, 0x3223D4);
		SetHook(CFHook, CFOffset, (u32)ConvertFlower, USE_LR_TO_RETURN);

		static Hook NameHook;
		static const u32 NameWrite = Region::AutoRegion(0x19C498, 0x19BEE0, 0x19C4B8, 0x19C4B8, 0x19C3F8, 0x19C3F8, 0x19C3F8, 0x19C3F8);
		SetHook(NameHook, NameWrite, (u32)NameFunc, USE_LR_TO_RETURN);

		static Hook ReplaceHook;
		static const u32 init = Region::AutoRegion(0x165528, 0x164F70, 0x165548, 0x165548, 0x165510, 0x165510, 0x165510, 0x165510);
		SetHook(ReplaceHook, init, (u32)IsItemReplaceable, USE_LR_TO_RETURN);

		static Hook DropHook1, DropHook2, DropHook3, DropHook4;
		static const u32 Drop1 = Region::AutoRegion(0x19B66C, 0x19B0B4, 0x19B68C, 0x19B68C, 0x19B5DC, 0x19B5DC, 0x19B5DC, 0x19B5DC);
		static const u32 Drop2 = Region::AutoRegion(0x19C044, 0x19BA8C, 0x19C064, 0x19C064, 0x19BFA4, 0x19BFA4, 0x19BFA4, 0x19BFA4);
		static const u32 Drop3 = Region::AutoRegion(0x19CFF0, 0x19CA38, 0x19D010, 0x19D010, 0x19CF50, 0x19CF50, 0x19CF50, 0x19CF50);
		static const u32 Drop4 = Region::AutoRegion(0x2AEA64, 0x2AE498, 0x2AEA60, 0x2AEA60, 0x2AE960, 0x2AE960, 0x2AE938, 0x2AE938);
		SetHook(DropHook1, Drop1, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook2, Drop2, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook3, Drop3, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook4, Drop4, (u32)IsItemDroppable, USE_LR_TO_RETURN);

		static Hook PlantHook1, PlantHook2, PlantHook3, PlantHook4;
		static const u32 Plant1 = Region::AutoRegion(0x19B688, 0x19B0D0, 0x19B6A8, 0x19B6A8, 0x19B5F8, 0x19B5F8, 0x19B5F8, 0x19B5F8);
		static const u32 Plant2 = Region::AutoRegion(0x19B97C, 0x19B3C4, 0x19B99C, 0x19B99C, 0x19B8EC, 0x19B8EC, 0x19B8EC, 0x19B8EC);
		static const u32 Plant3 = Region::AutoRegion(0x2AEAC4, 0x2AE4F8, 0x2AEAC0, 0x2AEAC0, 0x2AE9C0, 0x2AE9C0, 0x2AE998, 0x2AE998);
		static const u32 Plant4 = Region::AutoRegion(0x2AFD24, 0x2AF758, 0x2AFD20, 0x2AFD20, 0x2AFC20, 0x2AFC20, 0x2AFBF8, 0x2AFBF8);
		SetHook(PlantHook1, Plant1, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook2, Plant2, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook3, Plant3, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook4, Plant4, (u32)IsItemPlantable, USE_LR_TO_RETURN);

		/*static Hook hook;
		static const u32 address = Region::AutoRegion(0x323424, 0, 0, 0, 0, 0, 0, 0);
		hook.Initialize(address, (u32)func);
		hook.SetFlags(USE_LR_TO_RETURN);
		hook.Enable();*/

		/*static Hook ButtonCheck;
		static const u32 KeyPressedFunc = Region::AutoRegion(0x304A14, 0, 0, 0, 0, 0, 0, 0);
		ButtonCheck.Initialize(KeyPressedFunc, (u32)IsKeyDown);
		ButtonCheck.SetFlags(0);
		ButtonCheck.Enable();*/
	}
	
//check for indoor items	
	void IndoorsSeedItemCheck(void) {
	//needed cause of bug in CTRPF p.p
		Process::Write8(IndoorsBool, Player::IsIndoors());
		
		if(!DropPatternON || !Player::IsIndoors()) 
			return;
		
		RestoreDropPattern();
		DropPatternON = false;
		OSD::Notify("Drop Pattern Restored!", Color::Orange);
	}	

	//2001 - 2012 = Fruits | Get stacked into Fruit Basket (Not Flags, nor Item IDs)

	//2018 - 2029 = Fruit Baskets | Have flags indicating amount (8 is max, if more gets "0-Basket")

	//223F - 2282 = letter paper | Have flags indicating amount (3 is max, works more but game only allows 3)

	//2283 - 228C = turnips | Have their own ID's for the amount (Not Flags)

	/*u32 func(u32 InvAddress, u32 InvSlot, u32 ItemToInv, u32 ItemLock, u32 ItemToReplace) {
		u8 slot = 0;
	//If Fruits picked up
		if(IDList::ValidID(*(u16 *)ItemToInv, 0x2001, 0x2012)) { 
		//Single Fruit Search
			if(Player::GetNextItem(*(u16 *)ItemToInv, slot) != 0xFFFF) { //If fruit was found in inv
				Player::WriteInvSlot(slot, ((*(u16 *)(ItemToInv) + 0x17) + (1 << 16))); //converts current fruit to fruit basket with flag 1 (aka two fruits in basket)
				return 1;
			}
		//Fruit Basket Search
			if(Player::GetNextItem(*(u16 *)(ItemToInv) + 0x17, slot) != 0xFFFF) { //If correct fruit basket was found in inv
				u32 itemID = PlayerPTR::Pointer(0x6BD0 + (0x4 * slot));
				u8 flag = (*(u32 *)(itemID) >> 16);
			//If basket is not full
				if(flag < 8) { 
					Player::WriteInvSlot(slot, (*(u32 *)(itemID) + (1 << 16))); //adds one to the flag of the fruit basket
					return 1;
				}
			}
		}
	//If Fruit Basket picked up
		if(IDList::ValidID(*(u16 *)ItemToInv, 0x2018, 0x2029)) { //If Fruit Basket
			u16 flags[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
			int len = (sizeof(flags)/sizeof(flags[0]));
			if(Player::GetNextItem(*(u16 *)ItemToInv, slot, flags, len) != 0xFFFF) {
				OSD::Notify(Utils::Format("Slot: %2X || Item was found!", slot));
				u32 itemID = PlayerPTR::Pointer(0x6BD0 + (0x4 * slot));
				u8 flagInv = (*(u32 *)(itemID) >> 16);
				u8 flagNew = (*(u32 *)(ItemToInv) >> 16) + 1; //as flag "1" is 2 fruits
				if((flagInv + flagNew) <= 8) {
					OSD::Notify(Utils::Format("Slot: %2X || Flag Sum was below or 8", slot));
					Player::WriteInvSlot(slot, (*(u32 *)(itemID) + (flagNew << 16))); //adds new added flag to fruit basket
					return 1;
				}

				int diff = std::abs(flagInv - flagNew);
				if((flagInv + diff) <= 8) {
					OSD::Notify(Utils::Format("Slot: %2X || InvFlag - NewFlag = lower than or 8", slot));
					Player::WriteInvSlot(slot, (*(u32 *)(itemID) + (diff << 16))); //adds new added flag to fruit basket
					return 1;
				}
				else {
					if(flagInv != 8) {
						OSD::Notify(Utils::Format("Slot: %2X || Flag is not 8", slot));
						int diff = std::abs(8 - flagInv);
						Player::WriteInvSlot(slot, (*(u32 *)(itemID) + (diff << 16))); //adds new added flag to fruit basket
						return 1;
					}
				}
			}
		}
	//Standard Function to write item to inv
		u32 ItemToInvFunc = Region::AutoRegion(0x323484, 0, 0, 0, 0, 0, 0, 0);
		Process::Write32((u32)&FUN, ItemToInvFunc);
		return FUN(InvAddress, InvSlot, ItemToInv, ItemLock, ItemToReplace);
	}*/

	/*bool IsKeyDown(u32 key) {
	//custom instruction to check for ZL and ZR and the CStick
		if(Controller::IsKeyDown(Key::ZL) && key == 0x300000) return 1;
		if(Controller::IsKeyDown(Key::ZR) && key == 0x500000) return 1;
		if(Controller::IsKeyDown(Key::CStickRight) && key == 0x8000000) return 1;
		if(Controller::IsKeyDown(Key::CStickLeft) && key == 0x4000000) return 1;
		if(Controller::IsKeyDown(Key::CStickUp) && key == 0x1000000) return 1;
		if(Controller::IsKeyDown(Key::CStickDown) && key == 0x2000000) return 1;

	//this is the standard function to check for keys, exactly copied from the game
		int iVar1;

		static const u32 KeyCheck = Region::AutoRegion(0x5CF1AC, 0, 0, 0, 0, 0, 0, 0);

		static FUNCT func = FUNCT(KeyCheck);
		iVar1 = func.Call<int>();
		
		if(iVar1 == 0) {
			static const u32 KeyPointer = Region::AutoRegion(0x9762F4, 0, 0, 0, 0, 0, 0, 0);
			if(*(u32 *)(*(u32 *)(KeyPointer) + 0xD0) == 0) 
				iVar1 = 0;
			else 
				iVar1 = **(u32 **)(*(u32 *)(KeyPointer) + 0xD8);

			key = *(u32 *)(iVar1 + 0x110) & key;
			if(key != 0) 
				key = 1;
		}
		else 
			key = 0;

		return key;
	}*/
}