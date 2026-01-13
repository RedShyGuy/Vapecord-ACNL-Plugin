#include "Helpers/CustomButton.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Wrapper.hpp"
#include "Address/Address.hpp"
#include "Language.hpp"

namespace CTRPluginFramework {
	struct ItemButtonSettings {
		std::string name;
		bool active;
	};

	static std::vector<ItemButtonSettings> citemsettings = { 
		{ "Duplicate", false }, 
		{ "Wrap it!", false }, 
		{ "Put in Storage", false }, 
		{ "Pay Debt", false }
	};

	void CustomButton::DuplicateItem(u32 ItemData) {
		static Address getItem(0x7250B8);

		u32 itemSlot = *(u32 *)(ItemData + 0xCC);
		Item *item = getItem.Call<Item*>(ItemData + 0x1EC0, itemSlot);

		Inventory::WriteSlot((itemSlot == 0xF) ? 0 : (itemSlot + 1), *item);
	}

	void CustomButton::WrapItem(u32 ItemData) {
		static Address getItem(0x7250B8);
		u32 itemSlot = *(u32 *)(ItemData + 0xCC);

	//writes present lock onto item
		ACNL_Player *player = Player::GetSaveData();
		player->InventoryItemLocks[itemSlot] = 1;

		Game::PlaySound(0x406);
	//Loads Item Icon | present icon
		Address(0x26DC00).Call<void>(ItemData + 0x1EC0, itemSlot);
	}

	void CustomButton::PutItemToStorage(u32 ItemData) {
		static Address getItem(0x7250B8);
		u32 itemSlot = *(u32 *)(ItemData + 0xCC);
		Item *item = getItem.Call<Item*>(ItemData + 0x1EC0, itemSlot);
		
		u8 closetslot;
		if(Inventory::GetNextClosetItem({0x7FFE, 0}, closetslot)) {
			Inventory::WriteSlot(itemSlot, {0x7FFE, 0});	

			ACNL_Player *player = Player::GetSaveData();
			player->Dressers[closetslot] = *item;
		}
	}

	void CustomButton::PayDebt(u32 ItemData) {	
		static Address GetMoneyFunc(0x3055C8);
		static Address CallSound(0x58DDF8);

		static Address getItem(0x7250B8);
		u32 itemSlot = *(u32 *)(ItemData + 0xCC);
		Item *item = getItem.Call<Item*>(ItemData + 0x1EC0, itemSlot);

		ACNL_Player *player = Player::GetSaveData();

		if(player) {
			if(IDList::ValidID(item->ID, 0x20AC, 0x2117)) { 
				int money = GetMoneyFunc.Call<int>(&player->Inventory[itemSlot]);
					
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

				while(item->ID > 0x20AC) {
					CallSound.Call<void>(0x1000491);
					item->ID--;
					Inventory::WriteSlot(itemSlot, *item);	
				}

				Game::PlaySound(0x492);

				Inventory::WriteSlot(itemSlot, {0x7FFE, 0});
			}
		}
	}

	u32 SetNameCall(u32 DataPointer, u32 *stack, char *SYS_2D_UI, u16 sysID) {
		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);

		if (sysID >= 0x250) {
			u32 res = func.Call<u32>(DataPointer, stack, SYS_2D_UI, 0xD);

			if(sysID == 0x250) {
				Process::WriteString(stack[1], "Duplicate", 0x20, StringFormat::Utf16);
			}
			else if(sysID == 0x251) {
				Process::WriteString(stack[1], "Wrap it!", 0x20, StringFormat::Utf16);
			}
			else if(sysID == 0x252) {
				Process::WriteString(stack[1], "Put in Storage", 0x20, StringFormat::Utf16);
			}
			else if(sysID == 0x253) {
				Process::WriteString(stack[1], "Pay Debt", 0x20, StringFormat::Utf16);
			}

			return res;
		} else {
			return func.Call<u32>(DataPointer, stack, SYS_2D_UI, sysID);
		}
	}

	void SetButtonData(u32 *data, u32 stringContext, u32 stringId, u32 u0, u32 u1, u32 functionId, u8 u2) {
		data[0] = 1;
		data[1] = 0;
		
		data[0x1D] = 0;
		data[0x1E] = 0;
		data[0x1F] = 0;

		data[2] = stringContext;
		data[3] = stringId;
		data[4] = u0;
		data[5] = u1;
		data[6] = functionId;

		data[0x1C] = u2;
	}

	/*
	there can only be 5 options + 1 (quit button) at once on an item
	if the custom ones exceed that, only the possible ones will be shown
	*/
	void SetCustomButtonData(u32 *data, u32 sys2dUi) {
		register u32 optionCount asm("r4");

		register u32 itemOffset asm("r6");
		Item itemId = *(Item *)itemOffset;
		Item_Category category = Game::GetItemCategory(itemId);

		int addedOptions = 0;

		if (citemsettings[0].active && (optionCount + (addedOptions + 1)) < 5) {
			SetButtonData(data + (addedOptions * 8), sys2dUi, 0x250, 0, 0, 0x40, 7); //Duplicate
			addedOptions++;
		}
		if (citemsettings[1].active && (optionCount + (addedOptions + 1)) < 5) {
			SetButtonData(data + (addedOptions * 8), sys2dUi, 0x251, 0, 0, 0x41, 7); //Wrap It
			addedOptions++;
		}
		if (citemsettings[2].active && (optionCount + (addedOptions + 1)) < 5) {
			SetButtonData(data + (addedOptions * 8), sys2dUi, 0x252, 0, 0, 0x42, 7); //Put in Storage
			addedOptions++;
		}
		if (citemsettings[3].active && category == Item_Category::Bells && (optionCount + (addedOptions + 1)) < 5) {
			SetButtonData(data + (addedOptions * 8), sys2dUi, 0x253, 0, 0, 0x43, 7); //Pay Debt
			addedOptions++;
		}

		static Address buttonDataFunc(0x19C440);
		static Address buttonDataFuncOption1 = buttonDataFunc.MoveOffset(4);
		static Address buttonDataFuncOption2 = buttonDataFunc.MoveOffset(8);
		buttonDataFuncOption1.Patch(0xE2846001 + addedOptions);
		buttonDataFuncOption2.Patch(0xE2842000 + addedOptions);

		SetButtonData(data + (addedOptions * 8), sys2dUi, 0xB, 0, 0, 0x29, 1); //Quit Button
	}

	using FuncType = u32(*)(u32*);

	struct ItemFuncData {
		u32 Func;
		u32 Null;
	};

	int SetCustomButtonFunctions(int param1) {
		static Address sub_71DE5C(0x71DE5C);
		static Address sub_75AB28(0x75AB28);
		static Address sub_75AB40(0x75AB40);
		static Address sub_75AB34(0x75AB34);
		static Address sub_5C1B30(0x5C1B30);
		static Address sub_8176E4(0x8176E4);
		static Address sub_84B550(0x84B550);
		static Address sub_9509FC(0x9509FC);
		ItemFuncData *funcDataArray = (ItemFuncData *)sub_9509FC.addr;

		u32 v2 = param1 + 0xE0;

		if (!sub_71DE5C.Call<bool>(param1 + 0x1BC)) {
			u32 result = sub_75AB28.Call<u32>(v2);
			if (result) {
				u32 value = *(u32 *)(sub_75AB40.Call<u32>(param1 + 0xE0) + 0xA8);
				if (value == 0x1A || value == 0x1B || value == 0x1C) {
					*(u8 *)(param1 + 0x69F) = 0;
					return param1 + 0x1BC;
				}
			}
			return 0;
		}
		else {
			u32 v3 = sub_75AB40.Call<u32>(v2);
			sub_75AB34.Call<u32>(param1 + 0xE0);
			*(u8 *)(param1 + 0x1E9C) = 0;

			u32 v4 = *(u32 *)(v3 + 0xA8);
			if (v4 == 0x2A) {
				v4 = 0;
				*(u8 *)(param1 + 0x1E9C) = 1;
			}

			if (sub_5C1B30.Call<bool>(4)) {
				if (v4 >= 0x40) { //Custom funcs start at index 0x40
					switch(v4) {
						case 0x40: //Duplicate Item
							CustomButton::DuplicateItem(param1);
							break;
						case 0x41: //Wrap Item
							CustomButton::WrapItem(param1);
							break;
						case 0x42: //Put Item to Storage
							CustomButton::PutItemToStorage(param1);
							break;
						case 0x43: //Pay Debt
							CustomButton::PayDebt(param1);
							break;
					}
					return sub_8176E4.Call<u32>(param1 + 0xA4, *(u32 *)sub_84B550.addr, *(u32 *)(sub_84B550.addr + 4));
				}

				ItemFuncData funcData = funcDataArray[v4];
				u32 offsetFlag = funcData.Null;
				FuncType func = nullptr;
				u32 *targetAddr = (u32 *)(param1 + (offsetFlag >> 1));

				if ((offsetFlag & 1) == 0) {
					func = (FuncType)funcData.Func;
				} else {
					func = (FuncType)((u8 *)funcData.Func + *targetAddr);
				}

				return func(targetAddr);
			}
			else {
				return sub_8176E4.Call<u32>(param1 + 0xA4, *(u32 *)sub_84B550.addr, *(u32 *)(sub_84B550.addr + 4));
			}
		}
	}

	void SettingsButton(MenuEntry *entry) {
		static Hook hook, hook2, hook3;
		static Address buttonDataFunc(0x19C440);
		static Address buttonDataFuncOption1 = buttonDataFunc.MoveOffset(4);
		static Address buttonDataFuncOption2 = buttonDataFunc.MoveOffset(8);
		static Address buttonFunctionFunc(0x19EE20);
		static Address buttonFunctionFuncEnd = buttonFunctionFunc.MoveOffset(4);
		static Address SYSNameFunc(0x5D5860);

		std::vector<std::string> options;
		for (ItemButtonSettings &setting : citemsettings) {
			if (setting.active) {
				options.push_back(Color::Green << setting.name);
			} else {
				options.push_back(Color::Red << setting.name);
			}
		}

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"));
		optKb.Populate(options);
		int op = optKb.Open();
		if(op < 0) {
			return;
		}

		citemsettings[op].active = !citemsettings[op].active;

		bool anyActive = false;
		for (ItemButtonSettings &setting : citemsettings) {
			if (setting.active) {
				anyActive = true;
			}
		}
		
		if (anyActive) {
			hook.Initialize(buttonDataFunc.addr, (u32)SetCustomButtonData);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();

			hook2.Initialize(buttonFunctionFunc.addr, (u32)SetCustomButtonFunctions);
			hook2.SetFlags(USE_LR_TO_RETURN);
			hook2.Enable();
			buttonFunctionFuncEnd.Patch(0xE12FFF1E); //BX LR
			
			hook3.Initialize(SYSNameFunc.addr, (u32)SetNameCall);
			hook3.SetFlags(USE_LR_TO_RETURN);
			hook3.Enable();
		} else {
			hook.Disable();
			hook2.Disable();
			hook3.Disable();

			buttonFunctionFuncEnd.Unpatch();
			buttonDataFuncOption1.Unpatch();
			buttonDataFuncOption2.Unpatch();
		}

		SettingsButton(entry);
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

	static const std::vector<std::string> coutfitsettings = { 
		"Random Outfit", "Disable", 
	};
	
	void SettingsButtonASD(MenuEntry *entry) {
	//3rd Custom button | replaces remove wet suit button
		static const Address WetSuitButton(0x19DBA4);
		static const Address SocksButton(0x19DC78);

		Process::Patch(WetSuitButton.addr, 0xE1A00000); //Always Remove Wet Suit
		Process::Patch(SocksButton.addr, 0xEA000009); //Disable Remove Socks
	}
}
