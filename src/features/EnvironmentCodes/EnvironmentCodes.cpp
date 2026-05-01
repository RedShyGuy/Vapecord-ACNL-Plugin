#include <CTRPluginFramework.hpp>
#include "features/cheats.hpp"
#include "core/infrastructure/Address.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/infrastructure/Language.hpp"
#include "core/checks/IDChecks.hpp"
#include "core/infrastructure/CROEditing.hpp"
#include "core/game_api/Game.hpp"
#include "core/game_api/Dropper.hpp"
#include "core/game_api/NPC.hpp"
#include "core/game_api/Town.hpp"
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
			day2.Write<float>(1.25);
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
			auroraPatch3.Write<float>(1.0); //Brightness of aurora lights

			if(PlayerClass::GetInstance()->IsLoaded()) {
				OSD::NotifySysFont(Language::getInstance()->get(TextID::AURORALIGHTS_RELOAD_ROOM), Color(0xC430BAFF));
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
	static constexpr u32 WeatherPatches[5] = {
		0xE3A00000, 0xE3A00002, 0xE3A00003, 0xE3A00004, 0xE3A00005
	};

	static void ApplyWeatherSelection(u32 selection) {
		static Address weather(0x62FC30);

		if(selection >= 5) {
			weather.Unpatch();
			return;
		}

		weather.Patch(WeatherPatches[selection]);
	}

	void WeatherModApplySaved(MenuEntry *entry, u32 savedValue) {
		(void)entry;
		ApplyWeatherSelection(savedValue);
	}

	void Weathermod(MenuEntry *entry) {
		while(true) {
			static Address weather(0x62FC30);

			std::vector<std::string> weatheropt = {
				Language::getInstance()->get(TextID::VECTOR_WEATHER_SUNNY),
				Language::getInstance()->get(TextID::VECTOR_WEATHER_CLOUDY),
				Language::getInstance()->get(TextID::VECTOR_WEATHER_RAINY),
				Language::getInstance()->get(TextID::VECTOR_WEATHER_STORMY),
				Language::getInstance()->get(TextID::VECTOR_WEATHER_SNOWY),
				Language::getInstance()->get(TextID::VECTOR_DISABLE)
			};

			for(int i = 0; i < 5; ++i) {
				const bool isOn = *(u32 *)weather.addr == WeatherPatches[i];
				weatheropt[i] = (isOn ? Color(pGreen) : Color(pRed)) << weatheropt[i];
			}
			weatheropt[5] = (*(u32 *)weather.addr == weather.origVal ? Color(pGreen) : Color(pRed)) << weatheropt[5];

			Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), weatheropt);
			const int op = optKb.Open();
			if(op < 0 || op > 5) {
				return;
			}

			ApplyWeatherSelection(static_cast<u32>(op));
			entry->SetSavedValue(static_cast<u32>(op));
		}
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
				OSD::NotifySysFont(Language::getInstance()->get(TextID::GRASS_EDITOR_ONLY_IN_TOWN), Color::Red);
				return;
			}

			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				Process::Write8(GetTileOffset(x, y), type);
				OSD::NotifySysFont(Utils::Format(Language::getInstance()->get(TextID::GRASS_EDITOR_CHANGED_GRASS).c_str(), (u8)x, (u8)y));
			}
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			Game::ReloadRoom();
		}

		else if(entry->Hotkeys[2].IsPressed()) {
			switch(opt) {
				case 0:
					type = 0xFF;
					OSD::NotifySysFont(Language::getInstance()->get(TextID::GRASS_EDITOR_FILL_MODE), Color(0x228B22FF));
					opt = true;
				break;
				case 1:
					type = 0;
					OSD::NotifySysFont(Language::getInstance()->get(TextID::GRASS_EDITOR_CLEAR_MODE), Color(0xCD853FFF));
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

	static bool IsWiltCaller(u32 returnAddress) {
		static Address candidateSites[] = {
			Address(0x315A82),
			Address(0x315B10),
			Address(0x3160FE),
			Address(0x31607E),
			Address(0x3159FA),
		};

		for(Address site : candidateSites) {
			if(returnAddress >= (site.addr + 2) && returnAddress <= (site.addr + 6)) {
				return true;
			}
		}

		return false;
	}

	u32 NeverWiltSeedItems(u32 CurrentMap, Item *itemToPlace, u32 x, u32 y, u32 u2, u32 u3) {
		register u32 lrReg asm("lr");
		u32 returnAddress = lrReg & ~1U;

		if(IsWiltCaller(returnAddress)) {
			Item *item = Game::GetItemAtWorldCoords(x, y, CurrentMap);
			if(item != nullptr) {
				item->ID++;
				itemToPlace = item;
			}
		}

		HookContext& ctx = HookContext::GetCurrent();
        return ctx.OriginalFunction<u32, u32, Item *, u32, u32, u32, u32>(CurrentMap, itemToPlace, x, y, u2, u3);
	}

	void ItemsDontDissappearOnInvalidPositions(MenuEntry *entry) {
		static Address itemDeletePatch(0x6FC0DC);

		static Address dontDeleteSeedItemsOnInvalidPositions(0x528A18);

		static Hook neverWiltHook;
		static Address neverWiltFunc(0x2FCA24);

		if(entry->WasJustActivated()) {
			neverWiltHook.InitializeForMitm(neverWiltFunc.addr, (u32)&NeverWiltSeedItems);
			neverWiltHook.Enable();

			itemDeletePatch.Patch(0xE3A00000);
			dontDeleteSeedItemsOnInvalidPositions.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			itemDeletePatch.Unpatch();
			neverWiltHook.Disable();
			dontDeleteSeedItemsOnInvalidPositions.Unpatch();
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

		if(!PluginUtils::Input::PromptNumber<u32>({ Language::getInstance()->get(TextID::QUICK_MENU_SEARCH_REPLACE_SEARCH), true, 8, 0x7FFE }, *(u32 *)&ItemToSearch)) {
			return;
		}

		if(!PluginUtils::Input::PromptNumber<u32>({ Language::getInstance()->get(TextID::QUICK_MENU_SEARCH_REPLACE_REPLACE), true, 8, *(u32 *)&ItemToReplace }, *(u32 *)&ItemToReplace)) {
			return;
		}

		if(!ItemToReplace.isValid()) {
			OSD::NotifySysFont(Language::getInstance()->get(TextID::INVALID_ITEM), Color::Red);
			return;
		}

		int res = Dropper::Search_Replace(300, { ItemToSearch }, ItemToReplace, 0x3D, true, Language::getInstance()->get(TextID::SEARCH_REPLACE_ITEMS_REPLACED), true);
		if(res == -1) {
			OSD::NotifySysFont(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
			return;
		}
		else if(res == -2) {
			OSD::NotifySysFont(Language::getInstance()->get(TextID::ONLY_OUTDOORS), Color::Red);
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

	namespace {
		struct Date {
			u8 day;
			u8 month;
		};

		struct DateRange {
			Date start;
			Date end;
		};

		struct SeasonPeriod {
			DateRange range;
			u8 id;
		};

		static constexpr SeasonPeriod SeasonCalendar[] = {
			{ {{ 1,  1}, {15,  1}}, 0x00 }, //01.01 - 15.01
			{ {{16,  1}, {18,  2}}, 0x01 }, //16.01 - 18.02
			{ {{19,  2}, {24,  2}}, 0x02 }, //19.02 - 24.02
			{ {{25,  2}, {31,  3}}, 0x03 }, //25.02 - 31.03
			{ {{ 1,  4}, { 5,  4}}, 0x04 }, //01.04 - 05.04
			{ {{ 6,  4}, {10,  4}}, 0x05 }, //06.04 - 10.04
			{ {{11,  4}, {14,  4}}, 0x06 }, //11.04 - 14.04
			{ {{15,  4}, { 7,  6}}, 0x07 }, //15.04 - 07.06
			{ {{ 8,  6}, {15,  6}}, 0x08 }, //08.06 - 15.06
			{ {{16,  6}, {27,  6}}, 0x09 }, //16.06 - 27.06
			{ {{28,  6}, { 5,  7}}, 0x0A }, //28.06 - 05.07
			{ {{ 6,  7}, {23,  7}}, 0x0B }, //06.07 - 23.07
			{ {{24,  7}, { 7,  9}}, 0x0C }, //24.07 - 07.09
			{ {{ 8,  9}, {15,  9}}, 0x0D }, //08.09 - 15.09
			{ {{16,  9}, {16, 10}}, 0x0E }, //16.09 - 16.10
			{ {{17, 10}, {24, 10}}, 0x0F }, //17.10 - 24.10
			{ {{25, 10}, { 1, 11}}, 0x10 }, //25.10 - 01.11
			{ {{ 2, 11}, { 9, 11}}, 0x11 }, //02.11 - 09.11
			{ {{10, 11}, {17, 11}}, 0x12 }, //10.11 - 17.11
			{ {{18, 11}, {25, 11}}, 0x13 }, //18.11 - 25.11
			{ {{26, 11}, {30, 11}}, 0x14 }, //26.11 - 30.11
			{ {{ 1, 12}, {10, 12}}, 0x15 }, //01.12 - 10.12
			{ {{11, 12}, {31, 12}}, 0x16 }, //11.12 - 31.12
		};

		// Issue: bumping into bushes shows wrong model when snowy
		// Snowy (seasons 0x16, 0-2) are the same for all foliage
		struct FoliageBushInfo {
			u16 itemID;
			u8  flowerSeason;
			u8  bloomCount;
			u8  blooms[7];
		};

		static constexpr u8 SEASON_COUNT = 23;
		static constexpr u8 SnowSeasons[] = { 0x16, 0x00, 0x01, 0x02 };

		static constexpr FoliageBushInfo FoliageData[] = {
			{ 0x0085, 0x05, 3, { 0x06, 0x07, 0x08 } },                         // pink azalea
			{ 0x0088, 0x05, 3, { 0x06, 0x07, 0x08 } },                         // white azalea
			{ 0x008B, 0x08, 2, { 0x09, 0x0A } },                               // blue hydrangea
			{ 0x008E, 0x08, 2, { 0x09, 0x0A } },                               // pink hydrangea
			{ 0x0091, 0x0A, 3, { 0x0B, 0x0C, 0x0D } },                         // red hibiscus
			{ 0x0094, 0x0A, 3, { 0x0B, 0x0C, 0x0D } },                         // yellow hibiscus
			{ 0x0097, 0x0D, 7, { 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14 } }, // sweet olive
			{ 0x0082, 0x14, 2, { 0x15, 0x03 } },       						   // holly
		};

		enum class FoliageState : u8 { 
			Flower, 
			Bloom, 
			Default, 
			Snowy 
		};

		static bool isSeasonSnowy(u8 season) {
			for(u8 s : SnowSeasons) {
				if(season == s) {
					return true;
				}
			}
			return false;
		}

		static FoliageState GetFoliageState(const FoliageBushInfo &info, u8 season) {
			if (isSeasonSnowy(season)) {
				return FoliageState::Snowy;
			}

			if(season == info.flowerSeason) {
				return FoliageState::Flower;
			}

			for(u8 i = 0; i < info.bloomCount; ++i) {
				if(season == info.blooms[i]) {
					return FoliageState::Bloom;
				}
			}

			return FoliageState::Default;
		}

		static void OnFoliageSeasonChanged(Keyboard &kb, KeyboardEvent &event) {
			if(event.type != KeyboardEvent::CharacterAdded) {
				return;
			}

			u32 season = std::strtoul(kb.GetInput().c_str(), nullptr, 10);
			if(season >= SEASON_COUNT) {
				return;
			}

			static const std::string stateIcons[] = {
				Color(0xFF69B4FF) << Language::getInstance()->get(TextID::SET_FOLIAGE_FLOWERING),
				Color(pGreen) << Language::getInstance()->get(TextID::SET_FOLIAGE_BLOOM),
				Color(pYellow) << Language::getInstance()->get(TextID::SET_FOLIAGE_DEFAULT),
				Color(pBlue) << Language::getInstance()->get(TextID::SET_FOLIAGE_SNOWY),
			};

			std::string msg = Language::getInstance()->get(TextID::SET_FOLIAGE_SEASON_X) + std::string(" ") + std::to_string(season) + ":\n";
			for(u8 i = 0; i < sizeof(FoliageData) / sizeof(FoliageData[0]); ++i) {
				FoliageState state = GetFoliageState(FoliageData[i], static_cast<u8>(season));
				Item item = { FoliageData[i].itemID, 0 };
				msg += item.GetName();
				msg += " : ";
				msg += stateIcons[static_cast<u8>(state)];
				msg += ResetColor();
				msg += "\n";
			}

			msg.pop_back(); //remove last newline
			msg += HorizontalSeparator();

			{
				//Special extra case for trees since they don't follow the same pattern as bushes
				std::string state = Color(pYellow) << Language::getInstance()->get(TextID::SET_FOLIAGE_DEFAULT);

				if (isSeasonSnowy(season)) {
					state = Color(pBlue) << Language::getInstance()->get(TextID::SET_FOLIAGE_SNOWY);
				}
				else if (season == 0x04 || season == 0x05) {
					state = Color(0xFFB7C5FF) << Language::getInstance()->get(TextID::SET_FOLIAGE_CHERRY_BLOSSOM);
				}
				
				Item item = { 0x0026, 0 }; //tree
				msg += item.GetName();
				msg += " : ";
				msg += state;
				msg += ResetColor();
				msg += "\n";
			}

			kb.GetMessage() = msg;
		}

		static void ApplyFoliageSeason(u32 season) {
			static Address foliagePatch(0x59ABEC);
			static Address setBushSnowy(0x5A36C8);
			static Address setTreeSnowyWhenShaken(0x593EB4);
			static Address setTreeAndBushSnowyWhenBumped(0x593CE4);

			if(season >= SEASON_COUNT) {
				foliagePatch.Unpatch();
				setBushSnowy.Unpatch();
				setTreeSnowyWhenShaken.Unpatch();
				setTreeAndBushSnowyWhenBumped.Unpatch();
				return;
			}

			foliagePatch.Patch(0xE3A08000 | season);

			//If season is snowy, we patch this here so bumping trees/bushes shows the correct snowy models and particles
			if(isSeasonSnowy(season)) {
				setBushSnowy.Patch(0xE3A00001);
				setTreeSnowyWhenShaken.Patch(0xE3A00001);
				setTreeAndBushSnowyWhenBumped.Patch(0xE3A00001);
			} else {
				setBushSnowy.Unpatch();
				setTreeSnowyWhenShaken.Unpatch();
				setTreeAndBushSnowyWhenBumped.Unpatch();
			}
		}
	};

	void SetFoliageSeasonApplySaved(MenuEntry *entry, u32 savedValue) {
		(void)entry;
		ApplyFoliageSeason(savedValue);
	}

	void SetFoliageSeason(MenuEntry *entry) {
		static Address foliagePatch(0x59ABEC);

		const bool isActive = *(u32 *)foliagePatch.addr != foliagePatch.origVal;

		std::vector<std::string> options = {
			(isActive ? Color(pGreen) : Color(pRed)) << Language::getInstance()->get(TextID::SET_SEASON),
			(isActive ? Color(pRed) : Color(pGreen)) << Language::getInstance()->get(TextID::VECTOR_DISABLE)
		};

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), options);
		switch(optKb.Open()) {
			case 0: {
				u32 currentSeason = 0;
				u32 currentInstr = *(u32 *)foliagePatch.addr;
				if(isActive && (currentInstr & 0xFFFFFF00) == 0xE3A08000) {
					currentSeason = currentInstr & 0xFF;
				}

				Keyboard kb(Language::getInstance()->get(TextID::FOLIAGE_SEASON));
				kb.SetSlider(0, SEASON_COUNT-1, 1);
				kb.IsHexadecimal(false);
				kb.OnKeyboardEvent(OnFoliageSeasonChanged);

				KeyboardEvent event{};
				event.type = KeyboardEvent::CharacterAdded;
				kb.GetInput() = std::to_string(currentSeason);
				kb.ForceEvent(event);

				u32 value = currentSeason;
				if(kb.Open(value, value) < 0) {
					return;
				}

				ApplyFoliageSeason(value);
				entry->SetSavedValue(value);
				MessageBox(Utils::Format(Language::getInstance()->get(TextID::FOLIAGE_SEASON_SET).c_str(), value))();
				break;
			}
			case 1:
				ApplyFoliageSeason(SEASON_COUNT+1);
				entry->SetSavedValue(SEASON_COUNT+1);
				MessageBox(Language::getInstance()->get(TextID::FOLIAGE_SEASON_DISABLED))();
				break;
			default:
				break;
		}
	}
}
