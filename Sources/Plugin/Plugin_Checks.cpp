
#include "Helpers/Player.hpp"
#include "Helpers/Dropper.hpp"
#include "Address/Address.hpp"

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
	bool IsSTARTDown(u32 data, u32 key);
	void SetTitle(u32 dataParam, u32 *stack);
	void SetText(u32 dataParam, u32 *stack);
	void SuspendCallBack(u32 param);
	const char* SetProDesignStyle(Item *ItemID, u32 data, u32 data2);
	
//check for accidental invalid item eat/drop/show off etc
	void PluginHooks(void) {
		/*
		static Hook suspendHook;
		static const Address suspendAddress("SUSPENDADDRESS");
		SetHook(suspendHook, suspendAddress.addr, (u32)SuspendCallBack, USE_LR_TO_RETURN);
		*/

		/*static Hook titleHook, textHook;
		static const Address warningTXT("WARNINGTXT");
		SetHook(titleHook, warningTXT.addr, (u32)SetTitle, USE_LR_TO_RETURN);
		SetHook(textHook, warningTXT.addr + 0xA8, (u32)SetText, USE_LR_TO_RETURN);*/

		static Hook SaveButtonCheck;
		SetHook(SaveButtonCheck, Address("NOSAVE").addr - 0x10, (u32)IsSTARTDown, USE_LR_TO_RETURN);

		static Hook CatalogPHook1, CatalogPHook2;
		static const Address CatalogPOffset1("CATALOGPOFFSET1");
		static const Address CatalogPOffset2("CATALOGPOFFSET2");
		SetHook(CatalogPHook1, CatalogPOffset1.addr, (u32)CatalogPatch_Keyboard, USE_LR_TO_RETURN);
		SetHook(CatalogPHook2, CatalogPOffset2.addr, (u32)CatalogPatch_SearchFunction, USE_LR_TO_RETURN);
		
		static Hook IPHook;	
		static const Address IPOffset("IPOFFSET");
		SetHook(IPHook, IPOffset.addr, (u32)InvalidPickStop, USE_LR_TO_RETURN);

		static Hook InvDropHook, InvPlantHook;
		static const Address InvDropOffset("INVDROPOFFSET");
		static const Address InvPlantOffset("INVPLANTOFFSET");
		SetHook(InvDropHook, InvDropOffset.addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
		SetHook(InvPlantHook, InvPlantOffset.addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
		
		static Hook ISHook;
		static const Address ISOffset("ISOFFSET");
		SetHook(ISHook, ISOffset.addr, (u32)InvalidSpriteStop, USE_LR_TO_RETURN);

		static Hook InvalidGiveItemHook;
		static const Address InvalidGiveItemAddress("INVALIDGIVEITEMADDRESS");
		SetHook(InvalidGiveItemHook, InvalidGiveItemAddress.addr, (u32)InvalidGiveItem, USE_LR_TO_RETURN);
		
		static Hook IHHook;
		static const Address IHOffset("IHOFFSET");
		SetHook(IHHook, IHOffset.addr, (u32)InvalidHoleStop, USE_LR_TO_RETURN);
		
		static Hook IIHook;
		static const Address IIOffset("IIOFFSET");
		SetHook(IIHook, IIOffset.addr, (u32)InvalidItemStop, USE_LR_TO_RETURN);
		
		static Hook CFHook;
		static const Address CFOffset("CFOFFSET");
		SetHook(CFHook, CFOffset.addr, (u32)ConvertFlower, USE_LR_TO_RETURN);

		static Hook NameHook;
		static const Address NameWrite("NAMEWRITE");
		SetHook(NameHook, NameWrite.addr, (u32)NameFunc, USE_LR_TO_RETURN);

		static Hook ReplaceHook;
		static const Address init("INIT");
		SetHook(ReplaceHook, init.addr, (u32)IsItemReplaceable, USE_LR_TO_RETURN);

		static Hook DropHook1, DropHook2, DropHook3, DropHook4;
		static const Address Drop1("DROP1");
		static const Address Drop2("DROP2");
		static const Address Drop3("DROP3");
		static const Address Drop4("DROP4");
		SetHook(DropHook1, Drop1.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook2, Drop2.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook3, Drop3.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
		SetHook(DropHook4, Drop4.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);

		static Hook PlantHook1, PlantHook2, PlantHook3, PlantHook4;
		static const Address Plant1("PLANT1");
		static const Address Plant2("PLANT2");
		static const Address Plant3("PLANT3");
		static const Address Plant4("PLANT4");
		SetHook(PlantHook1, Plant1.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook2, Plant2.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook3, Plant3.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
		SetHook(PlantHook4, Plant4.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);

		static Hook OnTitleScreenHook;
		static const Address titleScreenWarp("TITLESCREENWARP");
		SetHook(OnTitleScreenHook, titleScreenWarp.addr, (u32)OnTitleScreen, USE_LR_TO_RETURN);

		static Hook OnProDesignHook;
		static const Address setItemIcon("SETITEMICON");
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
