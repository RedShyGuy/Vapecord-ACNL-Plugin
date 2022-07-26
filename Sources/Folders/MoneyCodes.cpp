#include "cheats.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/Town.hpp"

namespace CTRPluginFramework {
//Wallet Mod
	void wallet(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 money = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 5, money, 0))
			GameHelper::EncryptValue(&player->PocketMoney, money);
	}
//Bank Mod
	void bank(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 money = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 9, money, 0))
			GameHelper::EncryptValue(&player->BankAmount, money);
	}
//Meow Coupon Mod
	void coupon(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 coupon = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 4, coupon, 0))
			GameHelper::EncryptValue(&player->MeowCoupons, coupon);
	}
//Badges Mod
	void badges(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> badgesopt = {
			Color(0xFFD700FF) << Language->Get("VECTOR_BADGE_GOLD"),
			Color(0xC0C0C0FF) << Language->Get("VECTOR_BADGE_SILVER"),
			Color(0xCD7F32FF) << Language->Get("VECTOR_BADGE_BRONZE"),
			Language->Get("VECTOR_BADGE_NONE")
		};

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), badgesopt);

		int index = optKb.Open();
		if(index < 0)
			return;

		bool WithStats = MessageBox("Do you want to set the appropiate badge stats?\n(This will edit all badge related game stats)", DialogType::DialogYesNo).SetClear(ClearScreen::Top)();

		for(int i = 0; i < 24; ++i) 
			GameHelper::SetBadges(i, std::abs(index - 3), WithStats);
	}
//Medals Mod  32DC51B8 31F2C6BC
	void medals(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 medal = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 4, medal, 0)) {
			GameHelper::EncryptValue(&player->MedalAmount, medal);
		}	
	}	
//turnip Mod	
	void turnips(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();

		if(!town) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		u32 turnip = 0;
		if(Wrap::KB<u32>(Language->Get("ENTER_AMOUNT"), false, 5, turnip, 0)) {
			for(int i = 0; i < 6; ++i) {
				GameHelper::EncryptValue(&town->TurnipPrices[i], turnip); //AM
				GameHelper::EncryptValue(&town->TurnipPrices[i + 6], turnip); //PM
			}
		}
	}
}