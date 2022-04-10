#include "cheats.hpp"
#include "TextFileParser.hpp"
#include "NonHacker.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/IDList.hpp"
#include "RegionCodes.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/CROEditing.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "Helpers/QuickMenu.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "NonHacker.hpp"
#include "Color.h"

extern "C" void MoveFurn(void);
extern "C" void PATCH_MoveFurnButton(void);
extern "C" void PATCH_ToolAnim(void);

u8 toolTypeAnimID = 6;

extern "C" bool __IsAnimID(u8 toolAnimID) {
	static const u8 toolAnimIDArr[18] = { 0xB0, 0x49, 0x55, 0x6C, 0xA0, 0x98, 0x8F, 0x91, 0xC3, 0xCE, 0xCF, 0x8D, 0x8E, 0x91, 0xB1, 0xB1, 0x70, 0x9A };
	return std::find(std::begin(toolAnimIDArr), std::end(toolAnimIDArr), toolAnimID) != std::end(toolAnimIDArr);
}

namespace CTRPluginFramework {
	static bool AllOFF = false;
//disable commands
	void disablecommands(MenuEntry *entry) {
		std::vector<std::string> noncommands = {
			Language->Get("VECTOR_ANIM_COMM"),
			Language->Get("VECTOR_EMOT_COMM"),
			Language->Get("VECTOR_SNAK_COMM"),
			Language->Get("VECTOR_MUSI_COMM"),
			Language->Get("VECTOR_ITEM_COMM"),
			Language->Get("VECTOR_ALL_COMM")
		};

		for(int i = 0; i < 5; ++i) 
			noncommands[i] = (NonHacker::Accessible[i] ? Color(pGreen) : Color(pRed)) << noncommands[i];

		noncommands[5] = (AllOFF ? Color(pGreen) : Color(pRed)) << noncommands[5];

		Keyboard keyboard(Language->Get("COMM_CHOOSE"), noncommands);

		Sleep(Milliseconds(100));
        s8 choice = keyboard.Open();
        if(choice < 0)
			return;

		if(choice == 5) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			if(AllOFF) {
				*menu += NonHackerCallBack;
				AllOFF = false;
			}
			else {
				*menu -= NonHackerCallBack;
				AllOFF = true;
			}
			goto update;
		}

		NonHacker::Accessible[choice] = !NonHacker::Accessible[choice];

		update:
		disablecommands(entry);
	}

//Change Tool Animation
	void tooltype(MenuEntry *entry) {
		static Hook hook;
		if(Wrap::KB<u8>(Language->Get("TOOL_ANIM_ENTER_ANIM"), true, 2, toolTypeAnimID, toolTypeAnimID)) {
			if(toolTypeAnimID == 0) { //if switched OFF
				hook.Disable();
				return;
			}

			if(!IDList::AnimationValid(toolTypeAnimID))
				toolTypeAnimID = 6;

			hook.Initialize(Code::AnimFunction.addr + 0x10, (u32)PATCH_ToolAnim);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		}
	}
//Change Gametype
	void mgtype(MenuEntry *entry) {
		std::vector<std::string> gametype = {
			Language->Get("VECTOR_GAMETYPE_OFFLINE"),
			Language->Get("VECTOR_GAMETYPE_ONLINE1"),
			Language->Get("VECTOR_GAMETYPE_ONLINE2"),
			Language->Get("VECTOR_GAMETYPE_DREAM"),
		};

		bool IsON;
		
		for(int i = 0; i < 4; ++i) { 
			IsON = GameHelper::GetGameType() == i;
			gametype[i] = (IsON ? Color(pGreen) : Color(pRed)) << gametype[i];
		}
		
        Keyboard keyboard(Language->Get("GAME_TYPE_CHOOSE"), gametype);

		Sleep(Milliseconds(100));
        s8 gametchoice = keyboard.Open();
        if(gametchoice < 0)	
			return;
	
		GameHelper::ChangeGameType(gametchoice);
		mgtype(entry);
    }
//Unbreakable Flowers	
	void unbreakableflower(MenuEntry *entry) { 
		if(entry->WasJustActivated())
			Process::Patch(Code::UnbreakableFlower.addr, 0xE3A0801D);
		else if(!entry->IsActivated())
			Process::Patch(Code::UnbreakableFlower.addr, 0x0A00004B);
	}
//Weather Mod	
	void Weathermod(MenuEntry *entry) { 
		static const Address weather(0x62FC30, 0x62F158, 0x62EC68, 0x62EC68, 0x62E728, 0x62E728, 0x62E2D0, 0x62E2D0);
		
		std::vector<std::string> weatheropt = {
			Language->Get("VECTOR_WEATHER_SUNNY"),
			Language->Get("VECTOR_WEATHER_CLOUDY"),
			Language->Get("VECTOR_WEATHER_RAINY"),
			Language->Get("VECTOR_WEATHER_STORMY"),
			Language->Get("VECTOR_WEATHER_SNOWY"),
			Language->Get("VECTOR_DISABLE")
		};
		
		static constexpr u32 Weathers[5] = {
			0xE3A00000, 0xE3A00002, 0xE3A00003, 0xE3A00004, 0xE3A00005
		};

		bool IsON;
		
		for(int i = 0; i < 5; ++i) { 
			IsON = *(u32 *)weather.addr == Weathers[i];
			weatheropt[i] = (IsON ? Color(pGreen) : Color(pRed)) << weatheropt[i];
		}
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), weatheropt);

		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
			
		if(op == 5) {
			Process::Patch(weather.addr, 0xE1A00004);
			return;
		}
			
		Process::Patch(weather.addr, Weathers[op]);
		Weathermod(entry);
	}
//always aurora lights
	void auroralights(MenuEntry *entry) {
		static const Address auroraPatch1(0x62FD4C, 0x62F274, 0x62ED84, 0x62ED84, 0x62E844, 0x62E844, 0x62E3EC, 0x62E3EC);
		static const Address auroraPatch2(0x630590, 0x62FAB8, 0x62F5C8, 0x62F5C8, 0x62F088, 0x62F088, 0x62EC30, 0x62EC30);

		if(entry->WasJustActivated()) {
			Process::Patch(auroraPatch2.addr, 0xE3A00001); //Makes aurora appear in every season and weather
			Process::Patch(auroraPatch1.addr, 0xEA000023); //Makes aurora appear no matter the date or time
			Process::WriteFloat(auroraPatch1.addr + 0xA4, 1.0); //Brightness of aurora lights

			if(PlayerClass::GetInstance()->IsLoaded())
				OSD::Notify("Reload the room to see changes!", Color(0xC430BAFF));
		}

		CRO::Write<u32>("Outdoor", 0x1DB44, 0xE1A00000);

		if(!entry->IsActivated()) {
			CRO::Write<u32>("Outdoor", 0x1DB44, 0x1A000002);

			Process::Patch(auroraPatch2.addr, 0xE3A00000);
			Process::Patch(auroraPatch1.addr, 0x0A000023);
			Process::WriteFloat(auroraPatch1.addr + 0xA4, 0.0);
		}
	}

//reload room
	void ReloadRoomCheat(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSD::Notify("Your player needs to be loaded!", Color::Red);
			return;
		}

		GameHelper::ReloadRoom();
	}

	std::vector<std::string> cogNotes;
	static bool IsOnStartMenu = false;
	void QuickMenuOptions(void);

	void CogCheatCallback(Keyboard& keyboard, KeyboardEvent& event) {
        std::string& input = keyboard.GetMessage();
		static std::string cheatDesc = "";
		input = (IsOnStartMenu ? ToggleDrawMode(Render::UNDERLINE | Render::BOLD) + "\uE052 | Quick Menu Options...\n\n" + ToggleDrawMode(Render::UNDERLINE | Render::BOLD) : "") + cheatDesc;

		if(event.type == KeyboardEvent::KeyPressed && IsOnStartMenu) {
			if(event.affectedKey == Key::L) {
				QuickMenuOptions();
				keyboard.Close();
			}
		}

        if(event.type != KeyboardEvent::SelectionChanged)
            return;

		if(event.selectedIndex < 0 || event.selectedIndex >= cogNotes.size() || cogNotes.empty()) {
			cheatDesc.clear();
			return;
		}

		cheatDesc.clear();
		cheatDesc += ToggleDrawMode(Render::UNDERLINE) + "Cheat Description:\n" + ToggleDrawMode(Render::UNDERLINE);
		cheatDesc += cogNotes[event.selectedIndex];
	}

	void QuickMenuOptions(void) {
		Sleep(Milliseconds(100));
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"), std::vector<std::string>{ "Add Cog-Cheat", "Remove Cog-Cheat" });
		IsOnStartMenu = false;
		s8 res = KB.Open();
		if(res < 0)
			return;

		std::vector<EntryData> cogEntrys;
        QuickMenu::ListAvailableCogEntrys(cogEntrys);

		std::vector<std::string> CogNames;

		cogNotes.clear();
	//add entry to quick menu
		if(res == 0) {
		//all possible entrys have been added to the quick menu
			if(cogEntrys.size() <= 0) {
				Sleep(Milliseconds(100));
				MessageBox("Error", "You have already added every Cog-Cheat to the Quick Menu!").SetClear(ClearScreen::Top)();
				return;
			}

		//push all existing menu entrys into name/note vector
			for(auto edata : cogEntrys) {
				CogNames.push_back(edata.entry->Name());
				cogNotes.push_back(edata.entry->Note());
			}

			Sleep(Milliseconds(100));
			KB.Populate(CogNames);
			KB.OnKeyboardEvent(CogCheatCallback);
			res = KB.Open();

			if(res >= 0) {
				Sleep(Milliseconds(100));
				MessageBox(Utils::Format("Added %s to the Quick Menu!", PluginMenuData::RemoveColorFromString(cogEntrys[res].entry->Name()).c_str())).SetClear(ClearScreen::Top)();
			
				QuickMenu::AddEntry(cogEntrys[res]);
			}
		}

		else if(res == 1) {
		//quick menu is empty and can't remove any entrys
			if(QuickMenu::obj_QuickMenu.size() <= 0) {
				Sleep(Milliseconds(100));
				MessageBox("Error", "The Quick Menu is empty!").SetClear(ClearScreen::Top)();
				return;
			}

		//push all existing quick menu entrys into name/note vector
			for(auto edata : QuickMenu::obj_QuickMenu) {
				CogNames.push_back(edata.entry->Name());
				cogNotes.push_back(edata.entry->Note());
			}

			Sleep(Milliseconds(100));
			KB.Populate(CogNames);
			KB.OnKeyboardEvent(CogCheatCallback);
			res = KB.Open();

			if(res >= 0) {
				Sleep(Milliseconds(100));
				MessageBox(Utils::Format("Removed %s from the Quick Menu!", PluginMenuData::RemoveColorFromString(QuickMenu::obj_QuickMenu[res].entry->Name()).c_str())).SetClear(ClearScreen::Top)();

				QuickMenu::RemoveEntry(QuickMenu::obj_QuickMenu[res]);
			}
		}
	}

//Quick Menu
	void QuickMenuEntry(MenuEntry *entry) {	
		std::vector<std::string> QMEntryNames;
		
		if(entry->Hotkeys[0].IsPressed()) {
			cogNotes.clear();
			for(auto edata : QuickMenu::obj_QuickMenu) {
				QMEntryNames.push_back(edata.entry->Name());
				cogNotes.push_back(edata.entry->Note());
			}

			if(QMEntryNames.empty()) {
				QuickMenuOptions();
				return;
			}

			Sleep(Milliseconds(100));
			Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"), QMEntryNames);
			KB.OnKeyboardEvent(CogCheatCallback);
			IsOnStartMenu = true;
			s8 res = KB.Open();
			cogNotes.clear();
			if(res < 0)
				return;

			PluginMenuData::GetMenuFunc(QuickMenu::obj_QuickMenu[res].entry)(entry);
		}	
	}
//More Than 3 Numbers On Island
	void morenumberisland(MenuEntry *entry) {
		static const Address numbers(0xAD7158, 0xAD6158, 0xAD6158, 0xAD6158, 0xAD0158, 0xACF158, 0xACF158, 0xACF158);
		Process::Write8(numbers.addr, 2);
		
		if(!entry->IsActivated()) 
			Process::Write8(numbers.addr, 0);
	}
//Large FOV	
	void fovlarge(MenuEntry *entry) {
		static float OnOff = 1.0;
		
		Process::WriteFloat(Code::fov.addr, OnOff); 
		
		if(GameHelper::RoomCheck() == 1 || fovbool) 
			OnOff = 1.0; 
		else 
			OnOff = 0.75; 
		
		if(!entry->IsActivated()) 
			Process::WriteFloat(Code::fov.addr, 1.0);
	}
//Move Furniture
	void roomSeeder(MenuEntry *entry) {
		static const Address movefurn(0x5B531C, 0x5B4834, 0x5B4364, 0x5B4364, 0x5B3C54, 0x5B3C54, 0x5B3928, 0x5B3928);
		static const Address lightswitch(0x5B7558, 0x5B6A70, 0x5B65A0, 0x5B65A0, 0x5B5E90, 0x5B5E90, 0x5B5B64, 0x5B5B64);
		static const Address MoveFurnPatch(0x326B98, 0x3265AC, 0x325EEC, 0x325EEC, 0x325B78, 0x325B78, 0x325A30, 0x325A30);

		static Hook hook1, hook2, hook3;

		if(entry->WasJustActivated()) {
			hook1.Initialize(movefurn.addr, (u32)MoveFurn);
		  	hook1.SetFlags(USE_LR_TO_RETURN);
			hook1.Enable();	

			hook2.Initialize(lightswitch.addr, (u32)MoveFurn);
		  	hook2.SetFlags(USE_LR_TO_RETURN);
			hook2.Enable();	

			hook3.Initialize(MoveFurnPatch.addr, (u32)PATCH_MoveFurnButton);
			hook3.SetFlags(USE_LR_TO_RETURN);
			hook3.Enable();	
		}
		
		else if(!entry->IsActivated()) {
			hook1.Disable();
			hook2.Disable();
			hook3.Disable();
		}
    }
//Can Walk When Talk /*Made by Jay*/
	void walktalkentry(MenuEntry *entry) { 
		static const Address walktalk(0x655390, 0x6548B8, 0x6543C8, 0x6543C8, 0x653E88, 0x653E88, 0x653A30, 0x653A30);
		if(entry->WasJustActivated()) 
			Process::Patch(walktalk.addr, 0x1A000000); 
		else if(!entry->IsActivated()) 
			Process::Patch(walktalk.addr, 0x0A000000); 
	}

//Keyboard Extender
	void key_limit(MenuEntry* entry) {
		if(!GameKeyboard::IsOpen())
			return;
		
		if(Inventory::GetCurrent() == 4) 
			return;
		
		u32 KeyData = *(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x1328;
		static const Address KeyEnter(0xAD7253, 0xAD6253, 0xAD6253, 0xAD6253, 0xAD0253, 0xACF253, 0xACF253, 0xACF253);
		static const Address KeyAt(0xAD75C0, 0xAD65C0, 0xAD65C0, 0xAD65C0, 0xAD05C0, 0xACF5C0, 0xACF5C0, 0xACF5C0);
	
		Process::Write8(KeyData + 0xC, 0x41);
		Process::Write8(KeyData + 0x12B, 0x44);
		Process::Write8(KeyEnter.addr, 1);
		Process::Write8(KeyAt.addr, 1);
    }
//Custom Keyboard Symbols
	void CustomKeyboard(MenuEntry* entry) {   	
		static constexpr u16 patch[110] = {
			0xE000, 0xE001, 0xE002, 0xE003, 0xE004, 0xE005, 0xE006, 0xE008, 0xE009,
			0xE070, 0xE06F, 0xE06C, 0xE00C, 0xE00D, 0xE00E, 0xE00F, 0xE010, 0xE011,
			0xE012, 0xE013, 0xE014, 0xE03C, 0xE03B, 0xE03D, 0xE072, 0xE019, 0xE01A,
			0xE01B, 0xE01C, 0xE01D, 0xE01E, 0xE01F, 0xE020, 0xE021, 0xE022, 0xE023,
			0xE024, 0xE025, 0xE026, 0xE027, 0xE028, 0xE029, 0xE02A, 0xE02B, 0xE02C,
			0xE02D, 0xE02E, 0xE02F, 0xE030, 0xE031, 0xE032, 0xE033, 0xE034, 0xE035,
			0xE036, 0xE037, 0xE038, 0xE039, 0xE079, 0xE07A, 0xE07B, 0xE07C, 0xE03E,
			0xE03F, 0xE040, 0xE041, 0xE042, 0xE043, 0xE044, 0xE045, 0xE046, 0xE047,
			0xE048, 0xE049, 0xE04A, 0xE04B, 0xE04C, 0xE04D, 0xE04E, 0xE04F, 0xE050,
			0xE051, 0xE052, 0xE053, 0xE054, 0xE055, 0xE056, 0xE057, 0xE058, 0xE059,
			0xE05A, 0xE05B, 0xE05C, 0xE05D, 0xE05E, 0xE05F, 0xE060, 0xE061, 0xE062,
			0xE063, 0xE064, 0xE065, 0xE069, 0xE06A, 0xE073, 0xE067, 0xE074, 0xE075,
			0xE076, 0xE077
        };	
	
		static const Address IsOpen(0xAD7050, 0xAD6050, 0xAD6050, 0xAD6050, 0xAD0050, 0xACF050, 0xACF050, 0xACF050);		
        if(*(u16 *)IsOpen.addr == 0x0103) {
			u32 offset = 0;
			static const Address customKey(0xAD7630, 0xAD6630, 0xAD6630, 0xAD6630, 0xAD0630, 0xACF630, 0xACF630, 0xACF630);	
            offset = *(u32 *)customKey.addr;
            if(offset != 0) {	
                Process::Read32(offset + 0x224, offset);

				u16 value = 0;
			//US/EU French, US/EU English, US/EU Spanish, EU Italian, EU German
				if(Process::Read16(offset + 0x26, value) && value == 0x2E) 
					Process::CopyMemory((void *)(offset + 0x26), (void *)patch, 0x6E * 2); 
			//Japanese Keyboard
				else if(Process::Read16(offset + 0x1DC, value) && value == 0x3001) 
					Process::CopyMemory((void *)(offset + 0x1DC), (void *)patch, 0x6E * 2); 
			}
		}
    }
			
//Beans Particle Changer	
	void BeansParticleChanger(MenuEntry *entry) {
		static const Address beans(0x673E0C, 0x673334, 0x672E44, 0x672E44, 0x672904, 0x672904, 0x6724AC, 0x6724AC);
        static u16 input = 0; 
		
        if(entry->Hotkeys[0].IsDown()) {
			if(Wrap::KB<u16>(Language->Get("BEANS_PARTICLE_ENTER_ID"), true, 3, input, 0)) 
				Process::Patch(beans.addr, input);
		}
		
		if(!entry->IsActivated()) 
			Process::Patch(beans.addr, 0x205);
	}

//Always Daytime /*Made by Jay*/
	void Daytime(MenuEntry *entry) {
		static const Address day1(0x4B10A4, 0x4B0A1C, 0x4B00EC, 0x4B00EC, 0x4AFD84, 0x4AFD84, 0x4AFC44, 0x4AFC44);
		static const Address day2(0x1E6D58, 0x1E679C, 0x1E6D78, 0x1E6D78, 0x1E6CB4, 0x1E6CB4, 0x1E6C80, 0x1E6C80);
		static const Address day3(0x4B10AC, 0x4B0A24, 0x4B00F4, 0x4B00F4, 0x4AFD8C, 0x4AFD8C, 0x4AFC4C, 0x4AFC4C);
		static const Address day4(0x4B10C8, 0x4B0A40, 0x4B0110, 0x4B0110, 0x4AFDA8, 0x4AFDA8, 0x4AFC68, 0x4AFC68);
		
		static const u32 DayTime[3] = { day1.addr, day3.addr, day4.addr };
		
		static constexpr u32 DayTimePatch[2][3] = {
            { 0xE3A01788, 0xE3A00000, 0xE8871015 },
			{ 0xE1A01521, 0xE3A06000, 0xE8871004 }
		};
		
		if(entry->WasJustActivated()) {
			for(int i = 0; i < 3; ++i)
                Process::Patch(DayTime[i], DayTimePatch[0][i]);

			Process::WriteFloat(day2.addr, 1.25);
		}
		else if(!entry->IsActivated()) {
			for(int i = 0; i < 3; ++i)
                Process::Patch(DayTime[i], DayTimePatch[1][i]);

			Process::WriteFloat(day2.addr, 1.0);
		}
	}
//Fast Mode	
	void fast(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsPressed()) { //Key::R + Key::DPadDown
			turbo = !turbo;
			OSD::Notify("Fast mode " << (turbo ? Color::Green << "ON" : Color::Red << "OFF"));
		}
	}
//Fast Text Speed
	void fasttalk(MenuEntry *entry) { 
		static const Address fastt(0x5FC6AC, 0x5FBBDC, 0x5FB6E4, 0x5FB6E4, 0x5FAF64, 0x5FAF64, 0x5FABEC, 0x5FABEC);
		if(entry->WasJustActivated()) {
			Process::Patch(fastt.addr, 0xEA000000);
			Process::Patch(fastt.addr + 8, 0xE3500001);
		}
		else if(!entry->IsActivated()) {
			Process::Patch(fastt.addr, 0xE1A00004);
			Process::Patch(fastt.addr + 8, 0xE3500000);
		}	
	}
//Fast Game Speed	
	void speedentry(MenuEntry *entry) {
		static const Address speed(0x54DDB4, 0x54D2CC, 0x54CDFC, 0x54CDFC, 0x54C6E8, 0x54C6E8, 0x54C40C, 0x54C40C);
		Process::Patch(speed.addr, GameHelper::GameSaving() ? 0xE59400A0 : 0xE3E004FF); 
		
		if(!entry->IsActivated()) 
			Process::Patch(speed.addr, 0xE59400A0); 
	}
}
