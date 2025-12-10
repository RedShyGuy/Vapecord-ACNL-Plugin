#include "Helpers/CustomButton.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Wrapper.hpp"
#include "Address/Address.hpp"
#include "Language.hpp"

namespace CTRPluginFramework {
/*
Custom Buttons
*/
    void CustomButton::WrapItem(u32 ItemData) {
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);
	//writes present lock onto item
		ACNL_Player *player = Player::GetSaveData();
		player->InventoryItemLocks[slot] = 1;
	//Loads Item Icon | present icon
		Address(0x26DC00).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC) + 0x1EC0, slot);

		Address(0x19B380).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC));
	}

	void CustomButton::DuplicateItem(u32 ItemData) {
		Item itemslotid = {0x7FFE, 0};
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);
		Inventory::ReadSlot(slot, itemslotid);	
		
		Inventory::WriteSlot((slot == 0xF) ? 0 : (slot + 1), itemslotid);
		
		Address(0x19B380).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::PutItemToStorage(u32 ItemData) {
		Item itemslotid = {0x7FFE, 0};
		u8 slot = 0, closetslot = 0;
		
		Inventory::GetSelectedSlot(slot);
		Inventory::ReadSlot(slot, itemslotid);		
		
		if(Inventory::GetNextClosetItem({0x7FFE, 0}, closetslot)) {
			Inventory::WriteSlot(slot, {0x7FFE, 0});	

			ACNL_Player *player = Player::GetSaveData();
			player->Dressers[closetslot] = (Item)itemslotid;
		}

		Address(0x19B380).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::PayDebt(u32 ItemData) {	
		static Address GetMoneyFunc(0x3055C8);
		static Address CallSound(0x58DDF8);

		Item itemslotid = {0x7FFE, 0};
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);

		ACNL_Player *player = Player::GetSaveData();

		if(player) {
			Inventory::ReadSlot(slot, itemslotid);
			if(IDList::ValidID(itemslotid.ID, 0x20AC, 0x2117)) { 
				int money = GetMoneyFunc.Call<int>(&player->Inventory[slot]);
					
				int debt = Game::DecryptValue(&player->DebtAmount);
			//if you try to store more money than you need to, the rest will be set to your bank acc
				if(money >= debt) {
					int diff = std::abs(money - debt);
					
					int bank = Game::DecryptValue(&player->BankAmount);
				//if money that goes to the bank will not fill it it up completely	
					if((bank + diff) <= 999999999) {
						Game::EncryptValue(&player->BankAmount, bank + diff);
					}
				//If bank is not full but still can hold bells fill it up
					else {
						if(bank != 999999999) {
							diff = 999999999 - diff;
							Game::EncryptValue(&player->BankAmount, diff);
						}
					}
					
					money = debt; //make money the exact debt
				}

				Game::EncryptValue(&player->DebtAmount, debt - money);

				while(itemslotid.ID > 0x20AC) {
					CallSound.Call<void>(0x1000491);
					itemslotid.ID--;
					Inventory::WriteSlot(slot, itemslotid);		
				}

				Game::PlaySound(0x492);

				Inventory::WriteSlot(slot, {0x7FFE, 0});
			}
		}

		Address(0x19B380).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::RandomOutfit(u32 ItemData) {
		Player::WriteOutfit(4, (Item)Utils::Random(0x280B, 0x28F3), 
							   (Item)Utils::Random(0x28F5, 0x295B), 
							   (Item)Utils::Random(0x2493, 0x26F5), 
							   (Item)Utils::Random(0x26F8, 0x2776), 
							   (Item)Utils::Random(0x2777, 0x279E), 
							   (Item)Utils::Random(0x279F, 0x27E5));
		
		Address(0x19D2A0).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC));
	}

	/*void claim(void) {
		u32 DesignData = *(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x464; //0x32DC4E1C
		
		const u32 GetSlot = 0x724214;
		Process::Write32((u32)&FUN, GetSlot);
		u8 DesignSlot = FUN(DesignData);
		
		u32 Design = Player::GetSaveOffset(4) + 0x548C;		
		u8 correctslot = *(u8 *)(Design + (0x1 * DesignSlot));
		
		std::string PlayerName = "", TownName = "";
		Process::ReadString(PlayerPTR::Pointer(0x55A8), PlayerName, 0x10, StringFormat::Utf16);
		Process::ReadString(PlayerPTR::Pointer(0x55BE), TownName, 0x10, StringFormat::Utf16);
		
		Player::SetDesign(correctslot, "", *(u16 *)PlayerPTR::Pointer(0x55A6), PlayerName, *(u8 *)PlayerPTR::Pointer(0x55BA), *(u16 *)PlayerPTR::Pointer(0x55BC), TownName, *(u32 *)PlayerPTR::Pointer(0x55D0), 0xFF, 0xFF);
	
		FUN_008188a0(iParm1 + 0xa4,PTR_FUN_00852d64,DAT_00852d68);
	}

	void claimdesignbutton(MenuEntry *entry) {
		static Hook hook;
		
		const u32 AlwaysWear = 0x258E58;
		const u32 AlwaysUmbrella = 0x2585B8;
		
		if(entry->WasJustActivated()) {	
			Process::Write32(AlwaysWear, 0xE1A00000);
			Process::Write32(AlwaysUmbrella, 0xE1A00000);
		
			u32 HookOff = 0x25A0A4;
			hook.Initialize(HookOff, (u32)claim);
			hook.SetFlags(0);
			hook.Enable();
		}
		
		if(GameHelper::BaseInvPointer() != 0) {
			if(!Inventory::Opened())
				return;
			
			u32 Designs = *(u32 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x22C0);
			
			for(int i = 0; i < 6; i++) {
				u32 CurrDesign = Designs + (0xAC * i);
			//If ID is Wear
				if(*(u8 *)(CurrDesign + 0xA8) == 0)
					Process::WriteString((CurrDesign + 0x20), "Claim Design", 0x28, StringFormat::Utf16);
			}
		}
		
		if(!entry->IsActivated()) {	
			Process::Write32(AlwaysWear, 0x1A000010);
			Process::Write32(AlwaysUmbrella, 0x0A000009);
		    hook.Disable();
		}
	}
	*/	

	static int itemsetting1 = 0;
	static int itemsetting2 = 0;
	static int itemsetting3 = 0;

	static const std::vector<std::string> citemsettings = {
		"Duplicate", "Wrap it!", "Put in Storage", "Pay Debt", "Disable",
	};
	static const std::vector<std::string> coutfitsettings = { 
		"Random Outfit", "Disable", 
	};

	u32 SetNameCall(u32 DataPointer, u32 *stack, char *SYS_2D_UI, u8 sysID) {
		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		u32 res = func.Call<u32>(DataPointer, stack, SYS_2D_UI, sysID);

		if(sysID == 0x2A && itemsetting1 != -1) {
			Process::WriteString(stack[1], citemsettings[itemsetting1], 0x20, StringFormat::Utf16);
		}
		else if(sysID == 0x0C && itemsetting2 != -1) {
			Process::WriteString(stack[1], citemsettings[itemsetting2], 0x20, StringFormat::Utf16);
		}
		else if(sysID == 0x26 && itemsetting3 != -1) {
			Process::WriteString(stack[1], coutfitsettings[itemsetting3], 0x20, StringFormat::Utf16);
		}

		return res;
	}

	using FuncType = void(*)(u32);

	static const FuncType ItemButtonArr[4] = {
		CustomButton::DuplicateItem, CustomButton::WrapItem, CustomButton::PutItemToStorage, CustomButton::PayDebt
	};
	static const FuncType OutfitButtonArr[1] = {
		CustomButton::RandomOutfit
	};

	struct ItemFuncData {
		u32 FunctionPointer;
		u32 Null;
	};
	
//put item into storage
	void SettingsButton(MenuEntry *entry) {
		static const std::vector<std::string> cbuttons = { 
			"Item Button 1", "Item Button 2", "Outfit Button 1",
		};

		static const Address pointer_FUNC(0x9509FC);
		ItemFuncData* FuncPointer = (ItemFuncData *)pointer_FUNC.addr;

		static const Address SYSNameFunc(0x5D5860);
		static Hook NameHook;
		NameHook.Initialize(SYSNameFunc.addr, (u32)SetNameCall);
		NameHook.SetFlags(USE_LR_TO_RETURN);

		static const u32 Undo1 = FuncPointer[24].FunctionPointer;
		static const u32 Undo2 = FuncPointer[15].FunctionPointer;
		static const u32 Undo3 = FuncPointer[5].FunctionPointer;

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cbuttons);
		int op = optKb.Open();
		if(op < 0) {
			return;
		}

	//1st Custom button | replaces medicine button
		if(op == 0) {
			optKb.Populate(citemsettings);
			op = optKb.Open();
			if(op < 0) {
				return;
			}

			static const Address AlwaysMedicine(0x19BC04);

			if(op == 4) {
				Process::Patch(AlwaysMedicine.addr, 0x1A000020); //Unpatch

				FuncPointer[24].FunctionPointer = Undo1;

				itemsetting1 = -1;
				goto end;
			}

			Process::Patch(AlwaysMedicine.addr, 0xE1A00000); //Always Take Medicine

			FuncPointer[24].FunctionPointer = (u32)ItemButtonArr[op];

			itemsetting1 = op;
		}
	//2nd Custom button | replaces release button
		else if(op == 1) {
			optKb.Populate(citemsettings);
			op = optKb.Open();
			if(op < 0) {
				return;
			}
			
			static const Address AlwaysRelease(0x19BD60);
			static const Address NeverToss(0x19BD80); 

			if(op == 4) {
				Process::Patch(AlwaysRelease.addr, 0x0A000013);
				Process::Patch(NeverToss.addr, 0xE3A02037);

				FuncPointer[15].FunctionPointer = Undo2;

				itemsetting2 = -1;
				goto end;
			}
				
			Process::Patch(AlwaysRelease.addr, 0xE1A00000); //Enables Always Release
			Process::Patch(NeverToss.addr, 0xE3A0200C); //Changes Toss if Cicada Shell to release

			FuncPointer[15].FunctionPointer = (u32)ItemButtonArr[op];

			itemsetting2 = op;
		}
		
	//3rd Custom button | replaces remove wet suit button
		else if(op == 2) {
			optKb.Populate(coutfitsettings);
			op = optKb.Open();
			if(op < 0) {
				return;
			}
			
			static const Address WetSuitButton(0x19DBA4);
			static const Address SocksButton(0x19DC78);

			if(op == 1) {
				Process::Patch(WetSuitButton.addr, 0x1A000009);
				Process::Patch(SocksButton.addr, 0x1A000009);

				FuncPointer[5].FunctionPointer = Undo3;

				itemsetting3 = -1;
				goto end;
			}
			
			Process::Patch(WetSuitButton.addr, 0xE1A00000); //Always Remove Wet Suit
			Process::Patch(SocksButton.addr, 0xEA000009); //Disable Remove Socks

			FuncPointer[5].FunctionPointer = (u32)OutfitButtonArr[op];

			itemsetting3 = op;
		}

	end:
	//If all custom buttons are disabled, disable the name hook
		if(itemsetting1 == -1 && itemsetting2 == -1 && itemsetting3 == -1) {
			NameHook.Disable();
		}
		else {
			NameHook.Enable();
		}
	}

//Used for plugin checks

	void CustomPresentOpening() {
		Item itemslotid = {0x7FFE, 0};
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);

		Inventory::ReadSlot(slot, itemslotid);
		Inventory::WriteSlot(slot, itemslotid, 0);

		Game::PlaySound(0x407);

		Address(0x19B380).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC));
	}

	void InitCustomPresentOpeningFunction() {
		static const Address pointer_FUNC(0x9509FC);
		ItemFuncData* FuncPointer = (ItemFuncData *)pointer_FUNC.addr;
		FuncPointer[19].FunctionPointer = (u32)CustomPresentOpening;
	}
}
