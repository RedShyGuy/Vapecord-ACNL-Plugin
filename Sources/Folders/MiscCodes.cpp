#include "cheats.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/CROEditing.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "RuntimeContext.hpp"
#include "Color.h"

extern "C" void MoveFurn(void);
extern "C" void PATCH_MoveFurnButton(void);
extern "C" void PATCH_ToolAnim(void);

extern "C" bool __IsIndoors(void) {
	return CTRPluginFramework::RuntimeContext::getInstance()->isIndoors();
}

u8 toolTypeAnimID = 6;

extern "C" bool __IsAnimID(u8 toolAnimID) {
	static const u8 toolAnimIDArr[18] = { 0xB0, 0x49, 0x55, 0x6C, 0xA0, 0x98, 0x8F, 0x91, 0xC3, 0xCE, 0xCF, 0x8D, 0x8E, 0x91, 0xB1, 0xB1, 0x70, 0x9A };
	return std::find(std::begin(toolAnimIDArr), std::end(toolAnimIDArr), toolAnimID) != std::end(toolAnimIDArr);
}

namespace CTRPluginFramework {
//Change Tool Animation
	void tooltype(MenuEntry *entry) {
		static Hook hook;
		if(Wrap::KB<u8>(Language::getInstance()->get("TOOL_ANIM_ENTER_ANIM"), true, 2, toolTypeAnimID, toolTypeAnimID)) {
			if(toolTypeAnimID == 0) { //if switched OFF
				hook.Disable();
				return;
			}

			if(!IDList::AnimationValid(toolTypeAnimID)) {
				toolTypeAnimID = 6;
			}

			hook.Initialize(Address(0x64DB90).addr + 0x10, (u32)PATCH_ToolAnim);
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
			IsON = Game::GetGameType() == i;
			gametype[i] = (IsON ? Color(pGreen) : Color(pRed)) << gametype[i];
		}
		
        Keyboard keyboard(Language::getInstance()->get("GAME_TYPE_CHOOSE"), gametype);

        int gametchoice = keyboard.Open();
        if(gametchoice < 0)	{
			return;
		}
	
		Game::ChangeGameType(gametchoice);
		mgtype(entry);
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

	void ShowPlayingMusic(u32 musicData, u32 r1, u32 r2, u32 r3) {
		/*
		KK Songs are differently handled
		This does not work sometimes, example fortune shop, the switching back to the Fortune Shop Melody doesnt get recognized
		Also after new player build house, then spoke to isabelle, then left the town hall, then after the success melody, the town melody doesnt get recognized
		Shrunks perfomance doesn't get recognized
		Switching from nook store to closign soon doesnt get recognized
		Sapling Ceremony doesnt come up
		On this day, you became major didnt come up
		Campsite camper, wrong id
		*/

		u16 musicID = *(u16 *)(musicData + 8);
		if ((u8)musicID <= 0xFF) {
			OSD::Notify(Utils::Format("Now Playing: %s", IDList::GetMusicName(musicID).c_str()), Color(0x00FF00FF));
		} else {
			OSD::Notify(Utils::Format("Now Playing: %04X", musicID), Color(0x00FF00FF));
		}

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
        func.Call<void>(musicData, r1, r2, r3);
	}

	void radioPlayer(MenuEntry *entry) {
		static Hook hook;
		if(entry->WasJustActivated()) {
			hook.Initialize(Address(0x58C414).addr, (u32)ShowPlayingMusic);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		}
		else if(!entry->IsActivated()) {
			hook.Disable();
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

//reload room
	void ReloadRoomCheat(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSD::Notify("Your player needs to be loaded!", Color::Red);
			return;
		}

		Game::ReloadRoom();
	}

//More Than 3 Numbers On Island
	void morenumberisland(MenuEntry *entry) {
		static const Address numbers(0xAD7158);
		Process::Write8(numbers.addr, 2);
		
		if(!entry->IsActivated()) {
			Process::Write8(numbers.addr, 0);
		}
	}

//Large FOV	
	void fovlarge(MenuEntry *entry) {
		static Address fovlargeMod(0x47E48C);

		static float OnOff = 1.0;
		
		fovlargeMod.WriteFloat(OnOff);
		
		if(Game::GetRoom() == 1 || RuntimeContext::getInstance()->isFov()) {
			OnOff = 1.0; 
		}
		else {
			OnOff = 0.75; 
		}
		
		if(!entry->IsActivated()) {
			fovlargeMod.Unpatch();
		}
	}
//Move Furniture
	void roomSeeder(MenuEntry *entry) {
		static const Address movefurn(0x5B531C);
		static const Address lightswitch(0x5B7558);
		static const Address MoveFurnPatch(0x326B98);

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
		static Address walktalk(0x655390);

		if(entry->WasJustActivated()) {
			walktalk.Patch(0x1A000000);
		}
		else if(!entry->IsActivated()) {
			walktalk.Unpatch();
		}
	}

//Keyboard Extender
	void key_limit(MenuEntry* entry) {
		if(!GameKeyboard::IsOpen()) {
			return;
		}
		
		if(Inventory::GetCurrent() == 4) {
			return;
		}
		
		u32 KeyData = *(u32 *)(Game::BaseInvPointer() + 0xC) + 0x1328;
		static const Address KeyEnter(0xAD7253);
		static const Address KeyAt(0xAD75C0);
	
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
	
		static const Address IsOpen(0xAD7050);

        if(*(u16 *)IsOpen.addr == 0x0103) {
			u32 offset = 0;
			static const Address customKey(0xAD7630);	
            offset = *(u32 *)customKey.addr;
            if(offset != 0) {
                Process::Read32(offset + 0x224, offset);

				u16 value = 0;
			//US/EU French, US/EU English, US/EU Spanish, EU Italian, EU German
				if(Process::Read16(offset + 0x26, value) && value == 0x2E) {
					Process::CopyMemory((void *)(offset + 0x26), (void *)patch, 0x6E * 2); 
				}
			//Japanese Keyboard
				else if(Process::Read16(offset + 0x1DC, value) && value == 0x3001) {
					Process::CopyMemory((void *)(offset + 0x1DC), (void *)patch, 0x6E * 2); 
				}
			}
		}
    }
			
//Beans Particle Changer	
	void BeansParticleChanger(MenuEntry *entry) {
		static Address beans(0x673E0C);
        static u16 input = 0; 
		
        if(entry->Hotkeys[0].IsDown()) {
			if(Wrap::KB<u16>(Language::getInstance()->get("BEANS_PARTICLE_ENTER_ID"), true, 3, input, 0)) {
				beans.Patch(input);
			}
		}
		
		if(!entry->IsActivated()) {
			beans.Unpatch();
		}
	}

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
//Fast Mode	
	void fast(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsPressed()) { //Key::R + Key::DPadDown
			RuntimeContext::getInstance()->setTurbo(!RuntimeContext::getInstance()->isTurbo());
			OSD::Notify("Fast mode " << (RuntimeContext::getInstance()->isTurbo() ? Color::Green << "ON" : Color::Red << "OFF"));
		}
	}
//Fast Text Speed
	void fasttalk(MenuEntry *entry) { 
		static Address fastt(0x5FC6AC);
		static Address fastt2 = fastt.MoveOffset(8);

		if(entry->WasJustActivated()) {
			fastt.Patch(0xEA000000);
			fastt2.Patch(0xE3500001);
		}
		if(!entry->IsActivated()) {
			fastt.Unpatch();
			fastt2.Unpatch();
		}	
	}
//Fast Game Speed	
	void speedentry(MenuEntry *entry) {
		static Address speed(0x54DDB4);

		if (Game::GameSaving()) {
			speed.Unpatch();
		}
		else {
			speed.Patch(0xE3E004FF);
		}
		
		if(!entry->IsActivated()) {
			speed.Unpatch();
		}
	}
//Fast Isabelle (Fast Text + Game Speed when in the Isabelle greeting room)
	void fastisabelle(MenuEntry *entry) {
		static Address speed(0x54DDB4);
		static Address fastt(0x5FC6AC);
		static Address fastt2 = fastt.MoveOffset(8);

		u8 roomID = Game::GetRoom();
		if (roomID == 0x63 && entry->IsActivated()) { // Isabelle
			if (Game::GameSaving()) {
				speed.Unpatch();
			}
			else {
				speed.Patch(0xE3E004FF);
			}

			fastt.Patch(0xEA000000);
			fastt2.Patch(0xE3500001);
		}
		else {
			speed.Unpatch();
			fastt.Unpatch();
			fastt2.Unpatch();
		}
	}
}
