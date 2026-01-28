#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Wrapper.hpp"
#include "Language.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/CROEditing.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/NPC.hpp"
#include "Helpers/Town.hpp"
#include "Color.h"

namespace CTRPluginFramework {
    //Always Daytime /*Made by Jay*/
	void Daytime(MenuEntry *entry) {
		static Address day1(0x4B10A4);
		static Address day2(0x1E6D58);
		static Address day3(0x4B10AC);
		static Address day4(0x4B10C8);
		
		if(entry->WasJustActivated()) {
			day1.Patch(0xE3A01788);
			day2.WriteFloat(1.25);
			day3.Patch(0xE3A00000);
			day4.Patch(0xE8871015);
		}
		
		if(!entry->IsActivated()) {
			day1.Unpatch();
			day2.Unpatch();
			day3.Unpatch();
			day4.Unpatch();
		}
	}

    //always aurora lights
	void auroralights(MenuEntry *entry) {
		static Address auroraPatch1(0x62FD4C);
		static Address auroraPatch2(0x630590);
		static Address auroraPatch3 = auroraPatch1.MoveOffset(0xA4);

		if(entry->WasJustActivated()) {
			auroraPatch1.Patch(0xEA000023); //Makes aurora appear no matter the date or time
			auroraPatch2.Patch(0xE3A00001); //Makes aurora appear in every season and weather
			auroraPatch3.WriteFloat(1.0); //Brightness of aurora lights

			if(PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify("Reload the room to see changes!", Color(0xC430BAFF));
			}
		}

		CRO::Write<u32>("Outdoor", 0x1DB44, 0xE1A00000);

		if(!entry->IsActivated()) {
			CRO::Write<u32>("Outdoor", 0x1DB44, 0x1A000002);

			auroraPatch1.Unpatch();
			auroraPatch2.Unpatch();
			auroraPatch3.Unpatch();
		}
	}

    //Unbreakable Flowers
	void unbreakableflower(MenuEntry *entry) { 
		static Address unbreakableFlowerPatch(0x597F64);

		if(entry->WasJustActivated()) {
			unbreakableFlowerPatch.Patch(0xE3A0801D);
		}
		else if(!entry->IsActivated()) {
			unbreakableFlowerPatch.Unpatch();
		}
	}
//Weather Mod
	void Weathermod(MenuEntry *entry) { 
		static Address weather(0x62FC30);
		
		std::vector<std::string> weatheropt = {
			Language::getInstance()->get(TextID::VECTOR_WEATHER_SUNNY),
			Language::getInstance()->get(TextID::VECTOR_WEATHER_CLOUDY),
			Language::getInstance()->get(TextID::VECTOR_WEATHER_RAINY),
			Language::getInstance()->get(TextID::VECTOR_WEATHER_STORMY),
			Language::getInstance()->get(TextID::VECTOR_WEATHER_SNOWY),
			Language::getInstance()->get(TextID::VECTOR_DISABLE)
		};
		
		static constexpr u32 Weathers[5] = {
			0xE3A00000, 0xE3A00002, 0xE3A00003, 0xE3A00004, 0xE3A00005
		};

		bool IsON;
		
		for(int i = 0; i < 5; ++i) { 
			IsON = *(u32 *)weather.addr == Weathers[i];
			weatheropt[i] = (IsON ? Color(pGreen) : Color(pRed)) << weatheropt[i];
		}
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), weatheropt);

		int op = optKb.Open();
		if(op < 0) {
			return;
		}
		
		if(op == 5) {
			weather.Unpatch();
			return;
		}
		
		weather.Patch(Weathers[op]);
		Weathermod(entry);
	}

	//Water All Flowers	
	void WaterAllFlowers(MenuEntry *entry) {
		if (!MessageBox(Language::getInstance()->get(TextID::WATER_FLOWER_QUESTION), DialogType::DialogYesNo).SetClear(ClearScreen::Both)()) {
			return;
		}

		u32 x = 0x10, y = 0x10;
		bool res = true;
		
		while(res) {
			while(res) {
				if(Game::GetItemAtWorldCoords(x, y)) {
					Game::WaterFlowerAtWorldCoords(x, y);
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

		MessageBox(Language::getInstance()->get(TextID::WATER_FLOWER_SUCCESS)).SetClear(ClearScreen::Both)();
    }
//Weed Remover
	void weedremover(MenuEntry *entry) {	
		if (!MessageBox(Language::getInstance()->get(TextID::REMOVE_WEED_QUESTION), DialogType::DialogYesNo).SetClear(ClearScreen::Both)()) {
			return;
		}
		
		static int size = 300;
		
		int res = Dropper::Search_Replace(size, { {0x7C, 0}, {0x7D, 0}, {0x7E, 0}, {0x7F, 0}, {0xCC, 0}, {0xF8, 0} }, {0x7FFE, 0}, 0x3D, false, "Weed Removed!", true);
		if(res == -1) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Both)();
		}
		else if(res == -2) {
			MessageBox(Language::getInstance()->get(TextID::ONLY_OUTDOORS)).SetClear(ClearScreen::Both)();
		} else {
			MessageBox(Language::getInstance()->get(TextID::REMOVE_WEED_SUCCESS)).SetClear(ClearScreen::Both)();
		}
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
			Language::getInstance()->get(TextID::GRASS_EDITOR_FILL),
			Language::getInstance()->get(TextID::GRASS_EDITOR_CLEAR)
		};
		
		if(!Game::IsGameInRoom(0)) {
			MessageBox(Color::Red << Language::getInstance()->get(TextID::ONLY_TOWN_ERROR)).SetClear(ClearScreen::Top)();
			return;
		}
		
		const u32 GrassStart = *(u32 *)(Game::GetCurrentMap() + 0x28);
		Keyboard KB(Language::getInstance()->get(TextID::GRASS_EDITOR_KB1) << "\n" << Color(0x228B22FF) << 
					Language::getInstance()->get(TextID::GRASS_EDITOR_KB2)  << "\n" << Color(0xCD853FFF) << 
					Language::getInstance()->get(TextID::GRASS_EDITOR_KB3), GrassKB);
					
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

	void KeepGrassState(MenuEntry *entry) {
		static Address grassPatch(0x30CB34);

		if (entry->WasJustActivated()) {
			grassPatch.Patch(0xE12FFF1E); //bx lr
		} else if (!entry->IsActivated()) {
			grassPatch.Unpatch();
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

	void ItemsDontDissappearOnInvalidPositions(MenuEntry *entry) {
		static Address itemDeletePatch(0x6FC0DC);

		if(entry->WasJustActivated()) {
			itemDeletePatch.Patch(0xE3A00000);
		}
		else if(!entry->IsActivated()) {
			itemDeletePatch.Unpatch();
		}
	}
}