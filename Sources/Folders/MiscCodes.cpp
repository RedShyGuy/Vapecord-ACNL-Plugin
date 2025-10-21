#include "cheats.hpp"
#include "NonHacker.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/IDList.hpp"

#include "Helpers/Game.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/CROEditing.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/GameKeyboard.hpp"
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
			Language::getInstance()->get("VECTOR_ANIM_COMM"),
			Language::getInstance()->get("VECTOR_EMOT_COMM"),
			Language::getInstance()->get("VECTOR_SNAK_COMM"),
			Language::getInstance()->get("VECTOR_MUSI_COMM"),
			Language::getInstance()->get("VECTOR_ITEM_COMM"),
			Language::getInstance()->get("VECTOR_ALL_COMM")
		};

		for(int i = 0; i < 5; ++i) 
			noncommands[i] = (NonHacker::Accessible[i] ? Color(pGreen) : Color(pRed)) << noncommands[i];

		noncommands[5] = (AllOFF ? Color(pGreen) : Color(pRed)) << noncommands[5];

		Keyboard keyboard(Language::getInstance()->get("COMM_CHOOSE"), noncommands);

        int choice = keyboard.Open();
        if(choice < 0)
			return;

		if(choice == 5) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			if(AllOFF) {
				*menu += NonHacker_Player00;
				*menu += NonHacker_Player01;
				*menu += NonHacker_Player02;
				*menu += NonHacker_Player03;
				AllOFF = false;
			}
			else {
				*menu -= NonHacker_Player00;
				*menu -= NonHacker_Player01;
				*menu -= NonHacker_Player02;
				*menu -= NonHacker_Player03;
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
		if(Wrap::KB<u8>(Language::getInstance()->get("TOOL_ANIM_ENTER_ANIM"), true, 2, toolTypeAnimID, toolTypeAnimID)) {
			if(toolTypeAnimID == 0) { //if switched OFF
				hook.Disable();
				return;
			}

			if(!IDList::AnimationValid(toolTypeAnimID))
				toolTypeAnimID = 6;

			hook.Initialize(Address("ANIMFUNCTION").addr + 0x10, (u32)PATCH_ToolAnim);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		}
	}
//Change Gametype
	void mgtype(MenuEntry *entry) {
		std::vector<std::string> gametype = {
			Language::getInstance()->get("VECTOR_GAMETYPE_OFFLINE"),
			Language::getInstance()->get("VECTOR_GAMETYPE_ONLINE1"),
			Language::getInstance()->get("VECTOR_GAMETYPE_ONLINE2"),
			Language::getInstance()->get("VECTOR_GAMETYPE_DREAM"),
		};

		bool IsON;
		
		for(int i = 0; i < 4; ++i) { 
			IsON = GameHelper::GetGameType() == i;
			gametype[i] = (IsON ? Color(pGreen) : Color(pRed)) << gametype[i];
		}
		
        Keyboard keyboard(Language::getInstance()->get("GAME_TYPE_CHOOSE"), gametype);

        int gametchoice = keyboard.Open();
        if(gametchoice < 0)	
			return;
	
		GameHelper::ChangeGameType(gametchoice);
		mgtype(entry);
    }
//Unbreakable Flowers	
	void unbreakableflower(MenuEntry *entry) { 
		if(entry->WasJustActivated())
			Process::Patch(Address("UNBREAKABLEFLOWER").addr, 0xE3A0801D);
		else if(!entry->IsActivated())
			Process::Patch(Address("UNBREAKABLEFLOWER").addr, 0x0A00004B);
	}
//Weather Mod	
	void Weathermod(MenuEntry *entry) { 
		static const Address weather("WEATHER");
		
		std::vector<std::string> weatheropt = {
			Language::getInstance()->get("VECTOR_WEATHER_SUNNY"),
			Language::getInstance()->get("VECTOR_WEATHER_CLOUDY"),
			Language::getInstance()->get("VECTOR_WEATHER_RAINY"),
			Language::getInstance()->get("VECTOR_WEATHER_STORMY"),
			Language::getInstance()->get("VECTOR_WEATHER_SNOWY"),
			Language::getInstance()->get("VECTOR_DISABLE")
		};
		
		static constexpr u32 Weathers[5] = {
			0xE3A00000, 0xE3A00002, 0xE3A00003, 0xE3A00004, 0xE3A00005
		};

		bool IsON;
		
		for(int i = 0; i < 5; ++i) { 
			IsON = *(u32 *)weather.addr == Weathers[i];
			weatheropt[i] = (IsON ? Color(pGreen) : Color(pRed)) << weatheropt[i];
		}
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), weatheropt);

		int op = optKb.Open();
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
		static const Address auroraPatch1("AURORAPATCH1");
		static const Address auroraPatch2("AURORAPATCH2");

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
//More Than 3 Numbers On Island
	void morenumberisland(MenuEntry *entry) {
		static const Address numbers("NUMBERS");
		Process::Write8(numbers.addr, 2);
		
		if(!entry->IsActivated()) 
			Process::Write8(numbers.addr, 0);
	}
//Large FOV	
	void fovlarge(MenuEntry *entry) {
		static float OnOff = 1.0;
		
		Process::WriteFloat(Address("FOV").addr, OnOff); 
		
		if(GameHelper::RoomCheck() == 1 || fovbool) 
			OnOff = 1.0; 
		else 
			OnOff = 0.75; 
		
		if(!entry->IsActivated()) 
			Process::WriteFloat(Address("FOV").addr, 1.0);
	}
//Move Furniture
	void roomSeeder(MenuEntry *entry) {
		static const Address movefurn("MOVEFURN");
		static const Address lightswitch("LIGHTSWITCH");
		static const Address MoveFurnPatch("MOVEFURNPATCH");

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
		static const Address walktalk("WALKTALK");
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
		static const Address KeyEnter("KEYENTER");
		static const Address KeyAt("KEYAT");
	
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
	
		static const Address IsOpen("ISOPEN");		
        if(*(u16 *)IsOpen.addr == 0x0103) {
			u32 offset = 0;
			static const Address customKey("CUSTOMKEY");	
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
		static const Address beans("BEANS");
        static u16 input = 0; 
		
        if(entry->Hotkeys[0].IsDown()) {
			if(Wrap::KB<u16>(Language::getInstance()->get("BEANS_PARTICLE_ENTER_ID"), true, 3, input, 0)) 
				Process::Patch(beans.addr, input);
		}
		
		if(!entry->IsActivated()) 
			Process::Patch(beans.addr, 0x205);
	}

//Always Daytime /*Made by Jay*/
	void Daytime(MenuEntry *entry) {
		static const Address day1("DAY1");
		static const Address day2("DAY2");
		static const Address day3("DAY3");
		static const Address day4("DAY4");
		
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
		static const Address fastt("FASTT");
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
		static const Address speed("SPEED");
		Process::Patch(speed.addr, GameHelper::GameSaving() ? 0xE59400A0 : 0xE3E004FF);
		
		if(!entry->IsActivated())
			Process::Patch(speed.addr, 0xE59400A0);
	}
//Fast Isabelle (Fast Text + Game Speed when in the Isabelle greeting room)
	void fastisabelle(MenuEntry *entry) {
		static const Address speed("SPEED");
		static const Address fastt("FASTT");

		u8 roomID = GameHelper::RoomCheck();
		if (roomID == 0x63 && entry->IsActivated()) { // Isabelle
			Process::Patch(speed.addr, GameHelper::GameSaving() ? 0xE59400A0 : 0xE3E004FF);
			
			Process::Patch(fastt.addr, 0xEA000000);
			Process::Patch(fastt.addr + 8, 0xE3500001);
		}
		else {
			Process::Patch(speed.addr, 0xE59400A0);

			Process::Patch(fastt.addr, 0xE1A00004);
			Process::Patch(fastt.addr + 8, 0xE3500000);
		}
	}
}
