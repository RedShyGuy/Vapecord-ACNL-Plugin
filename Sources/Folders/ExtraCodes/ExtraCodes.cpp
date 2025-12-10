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
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		const std::vector<std::string> spotVEC = {
			Language::getInstance()->get("VECTOR_QUICK_LOCK_SPOT"), 
			Language::getInstance()->get("VECTOR_QUICK_UNLOCK_SPOT"),
			Language::getInstance()->get("VECTOR_QUICK_LOCK_MAP"),
			Language::getInstance()->get("VECTOR_QUICK_UNLOCK_MAP")
		};

		u32 x = 0, y = 0;
		PlayerClass::GetInstance()->GetWorldCoords(&x, &y);

		if(bypassing) {
			Dropper::DropItemLock(false);
		}

		Keyboard KB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), spotVEC);
		switch(KB.Open()) {
			default: break;
			case 0: {
				if(Game::CreateLockedSpot(0x12, x, y, Game::GetRoom(), true) == 0xFFFFFFFF) {
					OSD::Notify("Error: Too many locked spots are already existing!");		
				}	
				else {
					OSD::Notify("Locked Spot");
				}
			} break;

			case 1: {
				Game::ClearLockedSpot(x, y, Game::GetRoom(), 4);
				OSD::Notify("Unlocked Spot");
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
				OSD::Notify("Locked Map");
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
				OSD::Notify("Unlocked Map");
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
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		u32 x = 0, y = 0;
		u32 count = 0;
		Item ItemToSearch = {0x7FFE, 0};
		Item ItemToReplace = {0x7FFE, 0};
		
		if(!Wrap::KB<u32>(Language::getInstance()->get("QUICK_MENU_SEARCH_REPLACE_SEARCH"), true, 8, *(u32 *)&ItemToSearch, 0x7FFE)) {
			return;
		}
		
		if(!Wrap::KB<u32>(Language::getInstance()->get("QUICK_MENU_SEARCH_REPLACE_REPLACE"), true, 8, *(u32 *)&ItemToReplace, *(u32 *)&ItemToReplace)) {
			return;
		}
		
		if(!ItemToReplace.isValid()) {
			OSD::Notify("Item Is Invalid!", Color::Red);
			return;
		}

		int res = Dropper::Search_Replace(300, { ItemToSearch }, ItemToReplace, 0x3D, true, "items replaced!", true);
		if(res == -1) {
			OSD::Notify("Your player needs to be loaded!", Color::Red);
			return;
		}
		else if(res == -2) {
			OSD::Notify("Only works outside!", Color::Red);
			return;
		}
	}

//remove all town items
	void RemoveItemsCheat(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		if((MessageBox(Language::getInstance()->get("REMOVE_ITEM_WARNING"), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			Game::RemoveItems(true, 0, 0, 0xFF, 0xFF, true, true);
		}
	}

//Water All Flowers	
	void WaterAllFlowers(MenuEntry *entry) {	
		if(entry->Hotkeys[0].IsPressed()) {
			u32 x = 0x10, y = 0x10;
			bool res = true;
			
			while(res) {
				while(res) {
					if(Game::GetItemAtWorldCoords(x, y)) {
						Game::WaterFlower(x, y);
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
			OSD::Notify("Success!");
		}
    }
//Weed Remover
	void weedremover(MenuEntry *entry) {	
		const std::vector<std::string> weedopt = {
			Language::getInstance()->get("WEED_REMOVER_OFF"), 
			Language::getInstance()->get("WEED_REMOVER_ON")
		};
		
		static int size = 400;
		if(entry->Hotkeys[0].IsPressed()) {
			Keyboard KB(Language::getInstance()->get("WEED_REMOVER_KEY"), weedopt);

			switch(KB.Open()) {
				case 0: size = 5000; break;
				case 1: 
				default: size = 300; break;
			}
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {
			int res = Dropper::Search_Replace(size, { {0x7C, 0}, {0x7D, 0}, {0x7E, 0}, {0x7F, 0}, {0xCC, 0}, {0xF8, 0} }, {0x7FFE, 0}, 0x3D, false, "Weed Removed!", true);
			if(res == -1) {
				OSD::Notify("Your player needs to be loaded!", Color::Red);
			}
			else if(res == -2) {
				OSD::Notify("Only works outdoors!", Color::Red);
			}
		}
	}
//Edit Every Pattern
	void editpattern(MenuEntry *entry) {
		for(int i = 0; i < 10; ++i) {
			Player::StealDesign(i);
		}

		entry->Disable();
	}
	
	static const int _GrassTile[64] = {
		0,	1,	4,	5,	16,	17,	20,	21,
		2,	3,	6,	7,	18,	19,	22,	23,
		8,	9,	12,	13,	24,	25,	28,	29,
		10,	11,	14,	15,	26,	27,	30,	31,
		32,	33,	36,	37,	48,	49,	52,	53,
		34,	35,	38,	39,	50,	51,	54,	55,
		40,	41,	44,	45,	56,	57,	60,	61,
		42,	43,	46,	47,	58,	59,	62,	63
	};
/*Calculations copied from the ACNL Web Save Editor, credits goes to the creator*/
	u32 GetTileOffset(int x, int y) {
		const int Add = 64 * ((y / 8) * 8 * 2 + (x / 8)) + _GrassTile[(y % 8) * 8 + (x % 8)];
		const u32 GrassStart = *(u32 *)(Game::GetCurrentMap() + 0x28);
		return (GrassStart + Add);
	}
	
	void grasscomplete(MenuEntry *entry) {		
		const std::vector<std::string> GrassKB {
			Language::getInstance()->get("GRASS_EDITOR_FILL"),
			Language::getInstance()->get("GRASS_EDITOR_CLEAR")
		};
		
		if(!Game::IsGameInRoom(0)) {
			MessageBox(Color::Red << Language::getInstance()->get("ONLY_TOWN_ERROR")).SetClear(ClearScreen::Top)();
			return;
		}
		
		const u32 GrassStart = *(u32 *)(Game::GetCurrentMap() + 0x28);
		Keyboard KB(Language::getInstance()->get("GRASS_EDITOR_KB1") << "\n" << Color(0x228B22FF) << 
					Language::getInstance()->get("GRASS_EDITOR_KB2")  << "\n" << Color(0xCD853FFF) << 
					Language::getInstance()->get("GRASS_EDITOR_KB3"), GrassKB);
					
		switch(KB.Open()) {
			case 0:
				std::memset((void *)GrassStart, -1, 0x2800);
				Game::ReloadRoom();
			break;
			case 1:
				std::memset((void *)GrassStart, 0, 0x2800);
				Game::ReloadRoom();
			break;
			default: break;
		}
	}
	
//remove/add grass!!!! :))))))
	void grasseditor(MenuEntry *entry) {
		static bool opt = false;
		static u8 type = 0;

		if(entry->Hotkeys[0].IsPressed()) {
			if(!Game::IsGameInRoom(0)) {
				OSD::Notify("Error: Only Works In Town!", Color::Red);
				return;
			}
			
			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				Process::Write8(GetTileOffset(x, y), type);
				OSD::Notify(Utils::Format("Changed Grass at: X%02X|Y%02X", (u8)x, (u8)y));
			}
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {
			Game::ReloadRoom();
		}
		
		else if(entry->Hotkeys[2].IsPressed()) {
			switch(opt) {
				case 0:
					type = 0xFF;
					OSD::Notify("Grass: Fill-Mode Active!", Color(0x228B22FF));
					opt = true;
				break;
				case 1:
					type = 0;
					OSD::Notify("Grass: Clear-Mode Active!", Color(0xCD853FFF));
					opt = false;
				break;
			}
		}
	}
	
	const int TimeMax[5] = { 60, 24, 30, 12, 50 };
	static int CurrTime = 0;
	
	bool CheckTimeInput(const void *input, std::string &error) {
		const std::string TimeMode[5] = { 
			Language::getInstance()->get("TIME_MINUTE"),
			Language::getInstance()->get("TIME_HOUR"),
			Language::getInstance()->get("TIME_DAY"),
			Language::getInstance()->get("TIME_MONTH"),
			Language::getInstance()->get("TIME_YEAR")
		};

        u16 in = *static_cast<const u16 *>(input);
        if(in >= TimeMax[CurrTime]) {
			error = Utils::Format(Language::getInstance()->get("TIME_ERROR").c_str(), (TimeMax[CurrTime] - 1), TimeMode[CurrTime].c_str());
            return 0;
        }

        return 1;
    }
	
	void TTKeyboard(MenuEntry *entry) {
		const std::string TimeMode[5] = {
			Language::getInstance()->get("TIME_MINUTE"),
			Language::getInstance()->get("TIME_HOUR"),
			Language::getInstance()->get("TIME_DAY"),
			Language::getInstance()->get("TIME_MONTH"),
			Language::getInstance()->get("TIME_YEAR")
		};

		std::vector<std::string> TTKB {
			Language::getInstance()->get("TIME_BACKWARDS"),
			Language::getInstance()->get("TIME_FORWARD")
		};
		
		u8 timedat[5] = { 0, 0, 0, 0, 0 };
		Keyboard KB("", TTKB);
		int ch = KB.Open();
		if(ch < 0) {
			return;
		}
		
		for(int i = 0; i < 5; ++i) {			
			Keyboard KBS(Utils::Format(Language::getInstance()->get("TIME_KB1").c_str(), TimeMode[i].c_str()));
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

	bool ThinkToBuriedItems(Item *item) {
		if((item->Flags >> 12) == 8) {
			item->Flags &= 0x0FFF;
		}

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		return func.Call<bool>(item);
	}

	Item* PickBuriedItems(u32 pInstance, u8 wX, u8 wY) {
		Item* item = Game::GetItemAtWorldCoords(wX, wY);
		if((item->Flags >> 12) == 8) {
			item->Flags &= 0x0FFF;
		}

		return item;
	}

	void BuriedInspector(MenuEntry *entry) {
		static Hook BuriedHook, PickBuriedHook;
		static const Address BuriedAddress(0x665534);
		static const Address PickBuriedAddress(0x59A0BC);

		if(entry->WasJustActivated()) {
			BuriedHook.Initialize(BuriedAddress.addr, (u32)ThinkToBuriedItems);
			BuriedHook.SetFlags(USE_LR_TO_RETURN);
			BuriedHook.Enable();

			PickBuriedHook.Initialize(PickBuriedAddress.addr, (u32)PickBuriedItems);
			PickBuriedHook.SetFlags(USE_LR_TO_RETURN);
			PickBuriedHook.Enable();
		}
		else if(!entry->IsActivated()) {
			BuriedHook.Disable();
			PickBuriedHook.Disable();
		}
	}
}
