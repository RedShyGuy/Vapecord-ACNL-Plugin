#include "RegionCodes.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Dropper.hpp"

namespace CTRPluginFramework {
	void SetHook(Hook &hook, u32 address, u32 callback, u32 flags) {
		hook.Initialize(address, callback);
		hook.SetFlags(flags);
		hook.Enable();
	}

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
	bool IsSTARTPressed(u32 data, u32 key);
	void SetTitle(u32 dataParam, u32 *stack);
	void SetText(u32 dataParam, u32 *stack);
	void SuspendCallBack(u32 param);
	const char* SetProDesignStyle(Item *ItemID, u32 data, u32 data2);
	
//check for accidental invalid item eat/drop/show off etc
	void PluginHooks(void) {
		/*
		static Hook suspendHook;
		static const Address suspendAddress(0x124EB8, 0x124928, 0x124EDC, 0x124EDC, 0x124EA4, 0x124EA4, 0x124EA4, 0x124EA4);
		SetHook(suspendHook, suspendAddress.addr, (u32)SuspendCallBack, USE_LR_TO_RETURN);
		*/

		/*static Hook titleHook, textHook;
		static const Address warningTXT(0x2F319C, 0, 0, 0, 0, 0, 0, 0);
		SetHook(titleHook, warningTXT.addr, (u32)SetTitle, USE_LR_TO_RETURN);
		SetHook(textHook, warningTXT.addr + 0xA8, (u32)SetText, USE_LR_TO_RETURN);*/

		static Hook SaveButtonCheck;
		SetHook(SaveButtonCheck, Code::nosave.addr - 0x10, (u32)IsSTARTPressed, USE_LR_TO_RETURN);

		static Hook CatalogPHook1, CatalogPHook2;
		static const Address CatalogPOffset1(0x21C408, 0x21BE4C, 0x21C428, 0x21C428, 0x21C348, 0x21C348, 0x21C314, 0x21C314);
		static const Address CatalogPOffset2(0x21C0AC, 0x21BAF0, 0x21C0CC, 0x21C0CC, 0x21BFEC, 0x21BFEC, 0x21BFB8, 0x21BFB8);
		SetHook(CatalogPHook1, CatalogPOffset1.addr, (u32)CatalogPatch_Keyboard, USE_LR_TO_RETURN);
		SetHook(CatalogPHook2, CatalogPOffset2.addr, (u32)CatalogPatch_SearchFunction, USE_LR_TO_RETURN);
		
		static Hook IPHook;	
		static const Address IPOffset(0x59A258, 0x599770, 0x5992A0, 0x5992A0, 0x598B90, 0x598B90, 0x598864, 0x598864);
		SetHook(IPHook, IPOffset.addr, (u32)InvalidPickStop, USE_LR_TO_RETURN);

		static Hook InvDropHook, InvPlantHook;
		static const Address InvDropOffset(0x597850, 0x596D68, 0x596898, 0x596898, 0x596188, 0x596188, 0x595E5C, 0x595E5C);
		static const Address InvPlantOffset(0x597724, 0x596C3C, 0x59676C, 0x59676C, 0x59605C, 0x59605C, 0x595D30, 0x595D30);
		SetHook(InvDropHook, InvDropOffset.addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
		SetHook(InvPlantHook, InvPlantOffset.addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
		
		static Hook ISHook;
		static const Address ISOffset(0x670EA0, 0x6703C8, 0x66FED8, 0x66FED8, 0x66F998, 0x66F998, 0x66F540, 0x66F540);
		SetHook(ISHook, ISOffset.addr, (u32)InvalidSpriteStop, USE_LR_TO_RETURN);

		static Hook InvalidGiveItemHook;
		static const Address InvalidGiveItemAddress(0x2339CC, 0x233410, 0x2339EC, 0x2339EC, 0x23390C, 0x23390C, 0x2338D8, 0x2338D8);
		SetHook(InvalidGiveItemHook, InvalidGiveItemAddress.addr, (u32)InvalidGiveItem, USE_LR_TO_RETURN);
		
		static Hook IHHook;
		static const Address IHOffset(0x5980F4, 0x59760C, 0x59713C, 0x59713C, 0x596A2C, 0x596A2C, 0x596700, 0x596700);
		SetHook(IHHook, IHOffset.addr, (u32)InvalidHoleStop, USE_LR_TO_RETURN);
		
		static Hook IIHook;
		static const Address IIOffset(0x72511C, 0x724464, 0x724124, 0x7240FC, 0x7238B8, 0x723890, 0x723460, 0x723460);
		SetHook(IIHook, IIOffset.addr, (u32)InvalidItemStop, USE_LR_TO_RETURN);
		
		static Hook CFHook;
		static const Address CFOffset(0x323514, 0x322F28, 0x322868, 0x322868, 0x32251C, 0x32251C, 0x3223D4, 0x3223D4);
		SetHook(CFHook, CFOffset.addr, (u32)ConvertFlower, USE_LR_TO_RETURN);

		static Hook NameHook;
		static const Address NameWrite(0x19C498, 0x19BEE0, 0x19C4B8, 0x19C4B8, 0x19C3F8, 0x19C3F8, 0x19C3F8, 0x19C3F8);
		SetHook(NameHook, NameWrite.addr, (u32)NameFunc, USE_LR_TO_RETURN);

		static Hook ReplaceHook;
		static const Address init(0x165528, 0x164F70, 0x165548, 0x165548, 0x165510, 0x165510, 0x165510, 0x165510);
		SetHook(ReplaceHook, init.addr, (u32)IsItemReplaceable, USE_LR_TO_RETURN);

		static Hook DropHook1, DropHook2, DropHook3, DropHook4;
		static const Address Drop1(0x19B66C, 0x19B0B4, 0x19B68C, 0x19B68C, 0x19B5DC, 0x19B5DC, 0x19B5DC, 0x19B5DC);
		static const Address Drop2(0x19C044, 0x19BA8C, 0x19C064, 0x19C064, 0x19BFA4, 0x19BFA4, 0x19BFA4, 0x19BFA4);
		static const Address Drop3(0x19CFF0, 0x19CA38, 0x19D010, 0x19D010, 0x19CF50, 0x19CF50, 0x19CF50, 0x19CF50);
		static const Address Drop4(0x2AEA64, 0x2AE498, 0x2AEA60, 0x2AEA60, 0x2AE960, 0x2AE960, 0x2AE938, 0x2AE938);
		SetHook(DropHook1, Drop1.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook2, Drop2.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook3, Drop3.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook4, Drop4.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);

		static Hook PlantHook1, PlantHook2, PlantHook3, PlantHook4;
		static const Address Plant1(0x19B688, 0x19B0D0, 0x19B6A8, 0x19B6A8, 0x19B5F8, 0x19B5F8, 0x19B5F8, 0x19B5F8);
		static const Address Plant2(0x19B97C, 0x19B3C4, 0x19B99C, 0x19B99C, 0x19B8EC, 0x19B8EC, 0x19B8EC, 0x19B8EC);
		static const Address Plant3(0x2AEAC4, 0x2AE4F8, 0x2AEAC0, 0x2AEAC0, 0x2AE9C0, 0x2AE9C0, 0x2AE998, 0x2AE998);
		static const Address Plant4(0x2AFD24, 0x2AF758, 0x2AFD20, 0x2AFD20, 0x2AFC20, 0x2AFC20, 0x2AFBF8, 0x2AFBF8);
		SetHook(PlantHook1, Plant1.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook2, Plant2.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook3, Plant3.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook4, Plant4.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);

		static Hook OnTitleScreenHook;
		static const Address titleScreenWarp(0x109D52, 0x109D56, 0x109D56, 0x109D56, 0x109D56, 0x109D56, 0x109D56, 0x109D56);
		SetHook(OnTitleScreenHook, titleScreenWarp.addr, (u32)OnTitleScreen, USE_LR_TO_RETURN);

		static Hook OnProDesignHook;
		static const Address setItemIcon(0x2B9110, 0x2B8B44, 0x2B910C, 0x2B910C, 0x2B9004, 0x2B9004, 0x2B8FE4, 0x2B8FE4);
		SetHook(OnProDesignHook, setItemIcon.addr, (u32)SetProDesignStyle, USE_LR_TO_RETURN);

		/*static Hook hook;
		static const u32 address(0x323424, 0, 0, 0, 0, 0, 0, 0);
		hook.Initialize(address, (u32)func);
		hook.SetFlags(USE_LR_TO_RETURN);
		hook.Enable();*/

		/*static Hook ButtonCheck;
		static const u32 KeyPressedFunc(0x304A14, 0, 0, 0, 0, 0, 0, 0);
		ButtonCheck.Initialize(KeyPressedFunc, (u32)IsKeyDown);
		ButtonCheck.SetFlags(0);
		ButtonCheck.Enable();*/
	}
	
//check for indoor items	
	void IndoorsSeedItemCheck(void) {
		IsIndoorsBool = Player::IsIndoors();

		if(!DropPatternON || !IsIndoorsBool) 
			return;
		
		Dropper::RestorePattern();
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
		u32 ItemToInvFunc(0x323484, 0, 0, 0, 0, 0, 0, 0);
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

		static const u32 KeyCheck(0x5CF1AC, 0, 0, 0, 0, 0, 0, 0);

		static FUNCTION func(KeyCheck);
		iVar1 = func.Call<int>();
		
		if(iVar1 == 0) {
			static const u32 KeyPointer(0x9762F4, 0, 0, 0, 0, 0, 0, 0);
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