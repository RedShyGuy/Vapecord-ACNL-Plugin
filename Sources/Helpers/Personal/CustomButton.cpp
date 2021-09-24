#include "Helpers/CustomButton.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/PlayerPTR.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
/*
Custom Buttons are currently unused
*/
    void CustomButton::WrapItem(u32 ItemData) {
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);
	//writes present lock onto item
		Inventory::WriteLock(slot, 1);
	//Loads Item Icon | present icon
		Code::LoadIcon.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x1EC0, slot);

		static Address restore(0x19B380);
		restore.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}

	void CustomButton::DuplicateItem(u32 ItemData) {
		u32 itemslotid = 0x7FFE;
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);
		Inventory::ReadSlot(slot, itemslotid);		
		
		if(slot == 0xF) 
			Inventory::WriteSlot(0, itemslotid);
		else 
			Inventory::WriteSlot(slot + 1, itemslotid);
		
		static Address restore(0x19B380);
		restore.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::PutItemToStorage(u32 ItemData) {
		u32 itemslotid = 0x7FFE;
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);
		Inventory::ReadSlot(slot, itemslotid);		
		
		if(Inventory::GetNextClosetItem(0x7FFE, slot) != 0xFFFFFFFF) {
			Inventory::WriteSlot(slot, 0x7FFE);	
			PlayerPTR::Write32(0x92F0 + (4 * slot), itemslotid);
		}

		static Address restore(0x19B380);
		restore.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::PayDebt(u32 ItemData) {	
		static Address get(0x3055C8);
		static Address sound(0x58DDF8);
		static Address restore(0x19B380);

		u32 itemslotid = 0x7FFE;
		u8 slot = 0;
		Inventory::GetSelectedSlot(slot);

		Inventory::ReadSlot(slot, itemslotid);
		if(IDList::ValidID(itemslotid, 0x20AC, 0x2117)) { 
			int money = get.Call<int>(PlayerPTR::Pointer(0x6BD0 + (4 * slot)));
				
			int debt = GameHelper::GetMoney((u64 *)PlayerPTR::Pointer(0x6B94));
		//if you try to store more money than you need to, the rest will be set to your bank acc
			if(money >= debt) {
				int diff = std::abs(money - debt);
				
				int bank = GameHelper::GetMoney((u64 *)PlayerPTR::Pointer(0x6B8C));
			//if money that goes to the bank will not fill it it up completely	
				if((bank + diff) <= 999999999) {
					GameHelper::SetMoney(PlayerPTR::Pointer(0x6B8C), bank + diff);
				}
			//If bank is not full but still can hold bells fill it up
				else {
					if(bank != 999999999) {
						diff = 999999999 - diff;
						GameHelper::SetMoney(PlayerPTR::Pointer(0x6B8C), diff);
					}
				}
				
				money = debt; //make money the exact debt
			}

			GameHelper::SetMoney(PlayerPTR::Pointer(0x6B94), debt - money);

			while(itemslotid > 0x20AC) {
				sound.Call<void>(0x1000491);
				Inventory::WriteSlot(slot, itemslotid--);		
			}

			GameHelper::PlaySound(0x492);

			Inventory::WriteSlot(slot, 0x7FFE);
		}
		
		restore.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::RandomOutfit(u32 ItemData) {
		static Address restore(0x19D2A0);

		Player::WriteOutfit(4, Utils::Random(0x280B, 0x28F3), Utils::Random(0x28F5, 0x295B), Utils::Random(0x2493, 0x26F5), Utils::Random(0x26F8, 0x2776), Utils::Random(0x2777, 0x279E), Utils::Random(0x279F, 0x27E5));
		
		restore.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
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
		static Address set(0x312610);

		u32 res = set.Call<u32>(DataPointer, stack, SYS_2D_UI, sysID);

		if(sysID == 0x2A)
			Process::WriteString(stack[1], citemsettings[itemsetting1], 0x20, StringFormat::Utf16);
		else if(sysID == 0x0C) 
			Process::WriteString(stack[1], citemsettings[itemsetting2], 0x20, StringFormat::Utf16);
		else if(sysID == 0x26)
			Process::WriteString(stack[1], coutfitsettings[itemsetting3], 0x20, StringFormat::Utf16);

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

		ItemFuncData* FuncPointer = (ItemFuncData *)0x9509FC;

		static const Address SYSNameFunc(0x5D5860, 0, 0, 0, 0, 0, 0, 0);
		static Hook NameHook;
		NameHook.Initialize(SYSNameFunc.addr, (u32)SetNameCall);
		NameHook.SetFlags(USE_LR_TO_RETURN);

		static const u32 Undo1 = FuncPointer[24].FunctionPointer;
		static const u32 Undo2 = FuncPointer[15].FunctionPointer;
		static const u32 Undo3 = FuncPointer[5].FunctionPointer;

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cbuttons);
		s8 op = optKb.Open();
		if(op < 0)
			return;

	//1st Custom button | replaces medicine button
		if(op == 0) {
			optKb.Populate(citemsettings);
			op = optKb.Open();
			if(op < 0)
				return;

			static const Address AlwaysMedicine(0x19BC04, 0, 0, 0, 0, 0, 0, 0);

			if(op == 4) {
				Process::Patch(AlwaysMedicine.addr, 0x1A000020); //Unpatch

				FuncPointer[24].FunctionPointer = Undo1;

				NameHook.Disable();
				return;
			}

			NameHook.Enable();

			Process::Patch(AlwaysMedicine.addr, 0xE1A00000); //Always Take Medicine

			FuncPointer[24].FunctionPointer = (u32)ItemButtonArr[op];

			itemsetting1 = op;
		}
	//2nd Custom button | replaces release button
		else if(op == 1) {
			optKb.Populate(citemsettings);
			op = optKb.Open();
			if(op < 0)
				return;
			
			static const Address AlwaysRelease(0x19BD60, 0, 0, 0, 0, 0, 0, 0);
			static const Address NeverToss(0x19BD80, 0, 0, 0, 0, 0, 0, 0);

			if(op == 4) {
				Process::Patch(AlwaysRelease.addr, 0x0A000013);
				Process::Patch(NeverToss.addr, 0xE3A02037);

				FuncPointer[15].FunctionPointer = Undo2;

				NameHook.Disable();
				return;
			}

			NameHook.Enable();
				
			Process::Patch(AlwaysRelease.addr, 0xE1A00000); //Enables Always Release
			Process::Patch(NeverToss.addr, 0xE3A0200C); //Changes Toss if Cicada Shell to release

			FuncPointer[15].FunctionPointer = (u32)ItemButtonArr[op];

			itemsetting2 = op;
		}
		
	//3rd Custom button | replaces remove wet suit button
		else if(op == 2) {
			optKb.Populate(coutfitsettings);
			op = optKb.Open();
			if(op < 0)
				return;
			
			static const Address WetSuitButton(0x19DBA4, 0, 0, 0, 0, 0, 0, 0);
			static const Address SocksButton(0x19DC78, 0, 0, 0, 0, 0, 0, 0);

			if(op == 1) {
				Process::Patch(WetSuitButton.addr, 0x1A000009);
				Process::Patch(SocksButton.addr, 0x1A000009);

				FuncPointer[5].FunctionPointer = Undo3;

				NameHook.Disable();
				return;
			}

			NameHook.Enable();
			
			Process::Patch(WetSuitButton.addr, 0xE1A00000); //Always Remove Wet Suit
			Process::Patch(SocksButton.addr, 0xEA000009); //Disable Remove Socks

			FuncPointer[5].FunctionPointer = (u32)OutfitButtonArr[op];

			itemsetting3 = op;
		}
	}
}