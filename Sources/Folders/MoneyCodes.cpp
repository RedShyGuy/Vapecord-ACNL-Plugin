#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
//Wallet Mod
	void wallet(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 money = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 5, money, 0))
			GameHelper::SetMoney(PlayerPTR::Pointer(0x6F08), money);
	}
//Bank Mod
	void bank(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 money = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 9, money, 0))
			GameHelper::SetMoney(PlayerPTR::Pointer(0x6B8C), money);
	}
//Meow Coupon Mod
	void coupon(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 coupon = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 4, coupon, 0))
			GameHelper::SetMoney(PlayerPTR::Pointer(0x8D1C), coupon);
	}
//Badges Mod
	void badges(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> badgesopt = {
			Color(0xFFD700FF) << Language->Get("VECTOR_BADGE_GOLD"),
			Color(0xC0C0C0FF) << Language->Get("VECTOR_BADGE_SILVER"),
			Color(0xCD7F32FF) << Language->Get("VECTOR_BADGE_BRONZE"),
			Language->Get("VECTOR_BADGE_NONE")
		};

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(badgesopt);
		s8 index = optKb.Open();
		if(index < 0)
			return;

		for(int i = 0; i < 24; ++i) 
			GameHelper::SetBadges(i, std::abs(index - 3));
	}
//Medals Mod  32DC51B8 31F2C6BC
	void medals(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 medal = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 4, medal, 0)) {
			GameHelper::SetMoney(PlayerPTR::Pointer(0x6B9C), medal);
		}	
	}	
//turnip Mod	
	void turnips(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 turnip = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 5, turnip, 0)) {
			for(int i = 0; i < 6; ++i) {
				u32 TurnipOffset = Save::GetInstance()->Address(0x6ADE0);
				GameHelper::SetMoney(TurnipOffset + i * 16, turnip); //AM
				GameHelper::SetMoney(TurnipOffset + i * 16 + 8, turnip); //PM
			}
		}
	}
}