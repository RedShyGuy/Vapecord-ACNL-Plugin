#include "features/cheats.hpp"
#include "core/HUD.hpp"
#include "core/game_api/Game.hpp"
#include "core/game_api/Player.hpp"
#include "core/game_api/Dropper.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/game_api/Inventory.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/checks/IDChecks.hpp"
#include "core/RuntimeContext.hpp"
#include "core/infrastructure/CROEditing.hpp"
#include "Color.h"

namespace CTRPluginFramework {
//Shops Always Open
	void ShopsAlwaysOpen(MenuEntry *entry) {
		static Address shopretail(0x309348);
		static Address shopnookling(0x711B14);
		static Address shopgarden(0x711BCC);
		static Address shopables(0x713EB0);
		static Address shopshampoodle(0x71D42C);
		static Address shopkicks(0x71184C);
		static Address shopnooks(0x71F654);
		static Address shopkatrina(0x718098);
		static Address shopredd(0x718444);

		static Address ShopOpen[9] = {
			shopretail, shopnookling, shopgarden, shopables,
			shopshampoodle, shopkicks, shopnooks, shopkatrina, shopredd
		};

		if(entry->WasJustActivated()) {
			for(int i = 0; i < 9; ++i) {
				ShopOpen[i].Patch(0xE3A00001);
			}
		}
		else if(!entry->IsActivated()) {
			for(int i = 0; i < 9; ++i) {
				ShopOpen[i].Unpatch();
			}
		}
    }

//Disable Save Menus
	void nonesave(MenuEntry *entry) {
		static Address noSave(0x1A0980);

		if(entry->WasJustActivated()) {
			noSave.Patch(0xE1A00000);
			RuntimeContext::getInstance()->setSaveMenuDisabled(true);
		}
		else if(!entry->IsActivated()) {
			noSave.Unpatch();
			RuntimeContext::getInstance()->setSaveMenuDisabled(false);
		}
	}

//Disable Item Locks /*Credits to Nico*/
	void bypass(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			Dropper::DropItemLock(true);
			bypassing = true;
		}
		else if(!entry->IsActivated()) {
			Dropper::DropItemLock(false);
			bypassing = false;
		}
	}
//Can't Fall In Holes Or Pitfalls /*Credits to Nico*/
	void noTrap(MenuEntry *entry) {
		static Address notraps1(0x65A668);
		static Address notraps2(0x6789E4);

		if(entry->WasJustActivated()) {
			notraps1.Patch(0xEA000014);
			notraps2.Patch(0xEA00002D);
		}
		else if(!entry->IsActivated()) {
			notraps1.Unpatch();
			notraps2.Unpatch();
		}
	}

	void SetSpotState(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		const std::vector<std::string> spotVEC = {
			Language::getInstance()->get(TextID::VECTOR_QUICK_LOCK_SPOT),
			Language::getInstance()->get(TextID::VECTOR_QUICK_UNLOCK_SPOT),
			Language::getInstance()->get(TextID::VECTOR_QUICK_LOCK_MAP),
			Language::getInstance()->get(TextID::VECTOR_QUICK_UNLOCK_MAP)
		};

		u32 x = 0, y = 0;
		PlayerClass::GetInstance()->GetWorldCoords(&x, &y);

		if(bypassing) {
			Dropper::DropItemLock(false);
		}

		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), spotVEC);
		switch(KB.Open()) {
			default: break;
			case 0: {
				if(Game::CreateLockedSpot(0x12, x, y, Game::GetRoom(), true) == 0xFFFFFFFF) {
					HUD::Notify(Language::getInstance()->get(TextID::SPOT_STATE_TOO_MANY));
				}
				else {
					HUD::Notify(Language::getInstance()->get(TextID::SPOT_STATE_LOCK));
				}
			} break;

			case 1: {
				Game::ClearLockedSpot(x, y, Game::GetRoom(), 4);
				HUD::Notify(Language::getInstance()->get(TextID::SPOT_STATE_UNLOCK));
			} break;

			case 2: {
				x = 0, y = 0;
				while(Game::CreateLockedSpot(0x12, 0x10 + x, 0x10 + y, Game::GetRoom(), true) != 0xFFFFFFFF) {
					x++;
					if(x % 6 == 2) {
						y++;
						x = 0;
					}

					Sleep(Milliseconds(40));
				}
				HUD::Notify(Language::getInstance()->get(TextID::SPOT_STATE_MAP_LOCK));
			} break;

			case 3: {
				x = 0x10, y = 0x10;
				bool res = true;

				while(res) {
					while(res) {
						if(Game::GetItemAtWorldCoords(x, y)) {
							if(Game::GetLockedSpotIndex(x, y, Game::GetRoom()) != 0xFFFFFFFF) {
								Game::ClearLockedSpot(x, y, Game::GetRoom(), 4);
								Sleep(Milliseconds(40));
							}
						}
						else {
							res = false;
						}

						y++;
					}
					res = true;
					y = 0x10;
					x++;
					if(!Game::GetItemAtWorldCoords(x, y)) {
						res = false;
					}
				}
				HUD::Notify(Language::getInstance()->get(TextID::SPOT_STATE_MAP_UNLOCK));
			} break;
		}

		Sleep(Milliseconds(5));
		if(bypassing) {
			Dropper::DropItemLock(true);
		}
	}

	const int TimeMax[5] = { 60, 24, 30, 12, 50 };
	void TTKeyboard(MenuEntry *entry) {
		const std::string TimeMode[5] = {
			Language::getInstance()->get(TextID::TIME_MINUTE),
			Language::getInstance()->get(TextID::TIME_HOUR),
			Language::getInstance()->get(TextID::TIME_DAY),
			Language::getInstance()->get(TextID::TIME_MONTH),
			Language::getInstance()->get(TextID::TIME_YEAR)
		};

		const std::vector<std::string> TTKB {
			Language::getInstance()->get(TextID::TIME_BACKWARDS),
			Language::getInstance()->get(TextID::TIME_FORWARD)
		};

		u8 timedat[5] = { 0, 0, 0, 0, 0 };
		Keyboard KB("", TTKB);
		int ch = KB.Open();
		if(ch < 0) {
			return;
		}

		for(int i = 0; i < 5; ++i) {
			Keyboard KBS(Utils::Format(Language::getInstance()->get(TextID::TIME_KB1).c_str(), TimeMode[i].c_str()));
			KBS.SetSlider(0, TimeMax[i] - 1);
			KBS.IsHexadecimal(false);

			int cho = KBS.Open(timedat[i]);
			if(cho < 0) {
				return;
			}
		}

		Game::SetCurrentTime(ch, timedat[0], timedat[1], timedat[2], timedat[3], timedat[4]);
	}

	void TimeTravel(MenuEntry *entry) {
		static u32 PressedTicks = 0;
		int minute = 1;

		if(entry->Hotkeys[0].IsDown() || entry->Hotkeys[0].IsPressed()) {
			PressedTicks++;
			if((PressedTicks < 50 ? (PressedTicks % 8) == 1 : (PressedTicks % 3) == 1) || PressedTicks > 100) {
				Game::SetCurrentTime(true, minute, 0, 0, 0, 0);
			}
		}

		else if(entry->Hotkeys[1].IsDown() || entry->Hotkeys[1].IsPressed()) {
			PressedTicks++;
			if((PressedTicks < 50 ? (PressedTicks % 8) == 1 : (PressedTicks % 3) == 1) || PressedTicks > 100) {
				Game::SetCurrentTime(false, minute, 0, 0, 0, 0);
			}
		}
	//somehow doesnt work always?
		else if(Controller::IsKeysReleased(entry->Hotkeys[0].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[1].GetKeys())) {
			PressedTicks = 0;
		}
	}
}
