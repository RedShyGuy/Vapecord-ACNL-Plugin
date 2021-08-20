#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {

/*
Custom Buttons are currently unused
*/
    void CustomButton::WrapItem(u32 ItemData) {
	//writes present lock onto item
		Inventory::WriteLock(Inventory::GetSelectedSlot(), 1);
	//Loads Item Icon | present icon
		FUNCT(Code::LoadIcon).Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x1EC0, Inventory::GetSelectedSlot());

		FUNCT(0x19B380).Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}

	void CustomButton::DuplicateItem(u32 ItemData) {
		u32 itemslotid = Inventory::ReadSlot(Inventory::GetSelectedSlot());		
		
		if(Inventory::GetSelectedSlot() == 0xF) 
			Inventory::WriteSlot(0, itemslotid);
		else 
			Inventory::WriteSlot(Inventory::GetSelectedSlot() + 1, itemslotid);
		
		FUNCT(0x19B380).Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::PutItemToStorage(u32 ItemData) {
		u32 itemslotid = Inventory::ReadSlot(Inventory::GetSelectedSlot());		
		u8 slot = 0;
		
		if(Inventory::GetNextClosetItem(0x7FFE, slot) != 0xFFFFFFFF) {
			Inventory::WriteSlot(Inventory::GetSelectedSlot(), 0x7FFE);	
			PlayerPTR::Write32(0x92F0 + (0x4 * slot), itemslotid);
		}

		FUNCT(0x19B380).Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::PayDebt(u32 ItemData) {	
		u32 itemslotid = Inventory::ReadSlot(Inventory::GetSelectedSlot());
		if(IDList::ValidID(itemslotid, 0x20AC, 0x2117)) {  
			int money = FUNCT(0x3055C8).Call<int>(PlayerPTR::Pointer(0x6BD0 + (0x4 * Inventory::GetSelectedSlot())));
				
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
			
			Inventory::WriteSlot(Inventory::GetSelectedSlot(), 0x7FFE);
		}
		
		FUNCT(0x19B380).Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
	
	void CustomButton::RandomOutfit(u32 ItemData) {
		Player::WriteOutfit(4, Utils::Random(0x280B, 0x28F3), Utils::Random(0x28F5, 0x295B), Utils::Random(0x2493, 0x26F5), Utils::Random(0x26F8, 0x2776), Utils::Random(0x2777, 0x279E), Utils::Random(0x279F, 0x27E5));
		
		FUNCT(0x19D2A0).Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC));
	}
}