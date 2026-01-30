#include "cheats.hpp"

#include "Helpers/Game.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/IDList.hpp"
#include "RuntimeContext.hpp"
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
					OSDExtras::Notify(TextID::SPOT_STATE_TOO_MANY);		
				}	
				else {
					OSDExtras::Notify(TextID::SPOT_STATE_LOCK);
				}
			} break;

			case 1: {
				Game::ClearLockedSpot(x, y, Game::GetRoom(), 4);
				OSDExtras::Notify(TextID::SPOT_STATE_UNLOCK);
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
				OSDExtras::Notify(TextID::SPOT_STATE_MAP_LOCK);
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
				OSDExtras::Notify(TextID::SPOT_STATE_MAP_UNLOCK);
			} break;
		}

		Sleep(Milliseconds(5));
		if(bypassing) {
			Dropper::DropItemLock(true);
		}
	}

//search and replace
	void SearchReplace(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		u32 x = 0, y = 0;
		u32 count = 0;
		Item ItemToSearch = {0x7FFE, 0};
		Item ItemToReplace = {0x7FFE, 0};
		
		if(!Wrap::KB<u32>(Language::getInstance()->get(TextID::QUICK_MENU_SEARCH_REPLACE_SEARCH), true, 8, *(u32 *)&ItemToSearch, 0x7FFE)) {
			return;
		}
		
		if(!Wrap::KB<u32>(Language::getInstance()->get(TextID::QUICK_MENU_SEARCH_REPLACE_REPLACE), true, 8, *(u32 *)&ItemToReplace, *(u32 *)&ItemToReplace)) {
			return;
		}
		
		if(!ItemToReplace.isValid()) {
			OSDExtras::Notify(TextID::INVALID_ITEM, Color::Red);
			return;
		}

		int res = Dropper::Search_Replace(300, { ItemToSearch }, ItemToReplace, 0x3D, true, Language::getInstance()->get(TextID::SEARCH_REPLACE_ITEMS_REPLACED), true);
		if(res == -1) {
			OSDExtras::Notify(TextID::SAVE_PLAYER_NO, Color::Red);
			return;
		}
		else if(res == -2) {
			OSDExtras::Notify(TextID::ONLY_OUTDOORS, Color::Red);
			return;
		}
	}

//remove all town items
	void RemoveItemsCheat(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		if((MessageBox(Language::getInstance()->get(TextID::REMOVE_ITEM_WARNING), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			Game::RemoveItems(true, 0, 0, 0xFF, 0xFF, true, true);
		}
	}

//Edit Every Pattern
	void editpattern(MenuEntry *entry) {
		for(int i = 0; i < 10; ++i) {
			Player::StealDesign(i);
		}

		entry->Disable();
	}
	
	const int TimeMax[5] = { 60, 24, 30, 12, 50 };
	static int CurrTime = 0;
	
	bool CheckTimeInput(const void *input, std::string &error) {
		const std::string TimeMode[5] = { 
			Language::getInstance()->get(TextID::TIME_MINUTE),
			Language::getInstance()->get(TextID::TIME_HOUR),
			Language::getInstance()->get(TextID::TIME_DAY),
			Language::getInstance()->get(TextID::TIME_MONTH),
			Language::getInstance()->get(TextID::TIME_YEAR)
		};

        u16 in = *static_cast<const u16 *>(input);
        if(in >= TimeMax[CurrTime]) {
			error = Utils::Format(Language::getInstance()->get(TextID::TIME_ERROR).c_str(), (TimeMax[CurrTime] - 1), TimeMode[CurrTime].c_str());
            return 0;
        }

        return 1;
    }
	
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
			KBS.IsHexadecimal(false);
			KBS.SetMaxLength(2);
			CurrTime = i;
			KBS.SetCompareCallback(CheckTimeInput);

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
