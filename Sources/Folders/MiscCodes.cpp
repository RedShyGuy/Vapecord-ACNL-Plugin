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
#include "Helpers/Player.hpp"
#include "RuntimeContext.hpp"
#include "Color.h"

extern "C" void PATCH_MoveFurnButton(void);
extern "C" void PATCH_ToolAnim(void);

u8 toolTypeAnimID = 6;

namespace CTRPluginFramework {
//Change Tool Animation
	void tooltype(MenuEntry *entry) {
		static Hook hook;
		if(Wrap::KB<u8>(Language::getInstance()->get(TextID::TOOL_ANIM_ENTER_ANIM), true, 2, toolTypeAnimID, toolTypeAnimID)) {
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
			Language::getInstance()->get(TextID::VECTOR_GAMETYPE_OFFLINE),
			Language::getInstance()->get(TextID::VECTOR_GAMETYPE_ONLINE1),
			Language::getInstance()->get(TextID::VECTOR_GAMETYPE_ONLINE2),
			Language::getInstance()->get(TextID::VECTOR_GAMETYPE_DREAM),
		};

		bool IsON;
		
		for(int i = 0; i < 4; ++i) { 
			IsON = Game::GetGameMode() == i;
			gametype[i] = (IsON ? Color(pGreen) : Color(pRed)) << gametype[i];
		}
		
        Keyboard keyboard(Language::getInstance()->get(TextID::GAME_TYPE_CHOOSE), gametype);

        int gametchoice = keyboard.Open();
        if(gametchoice < 0)	{
			return;
		}
	
		Game::ChangeGameMode((Game::GameMode)gametchoice);
		mgtype(entry);
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
			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::PLAYING_MUSIC_NOW_PLAYING_STRING).c_str(), IDList::GetMusicName(musicID).c_str()), Color(0x00FF00FF));
		} else {
			OSDExtras::Notify(Utils::Format(Language::getInstance()->get(TextID::PLAYING_MUSIC_NOW_PLAYING_ID).c_str(), musicID), Color(0x00FF00FF));
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

//reload room
	void ReloadRoomCheat(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSDExtras::Notify(TextID::SAVE_PLAYER_NO, Color::Red);
			return;
		}

		Game::ReloadRoom();
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

	u32 MoveFurniturePatch(void) {
		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		func.Call<u32>();

		return Player::IsIndoors();
	}

	u32 LightSwitchPatch(void) {
		static const u8 r_Array[14] = { 
			0x02, //Train Station
			0x26, //Town Hall
			0x30, //Police Station
			0x31, //Police Station
			0x39, //T&T Mart
			0x3A, //Super T&T
			0x3B, //TIY
			0x3F, //Able Sisters Mable & Sable
			0x41, //Nooks Homes
			0x43, //Gardening Store
			0x44, //Gardening Store
			0x48, //Shampoodle
			0x5A, //Post Office
		};

		u8 stageID = CTRPluginFramework::Player::GetRoom(4);
		for (u8 i = 0; i < 14; ++i) {
			if(stageID == r_Array[i]) {
				return 0; //Disable lightswitch if invalid room
			}
		}

		return Player::IsIndoors();
	}

//Move Furniture
	void roomSeeder(MenuEntry *entry) {
		static const Address movingFurniture(0x4E1720);
		static const Address pickingUpFurniture(0x678AC0);
		static const Address placingFurniture1(0x76B880);
		static const Address placingFurniture2(0x26FED8);
		static const Address placingFurniture3(0x4E78A8);
		static const Address lightswitchVisible(0x3279CC);
		static const Address lightswitchFunction(0x3277E8);
		static const Address moveFurnButton(0x326B98);

		static Hook movingFurnitureHook;
		static Hook pickingUpFurnitureHook;
		static Hook placingFurnitureHook1;
		static Hook placingFurnitureHook2;
		static Hook placingFurnitureHook3;
		static Hook lightSwitchVisibleHook;
		static Hook lightSwitchFunctionHook;
		static Hook moveFurnButtonHook;

		if(entry->WasJustActivated()) {
			movingFurnitureHook.Initialize(movingFurniture.addr, (u32)MoveFurniturePatch);
		  	movingFurnitureHook.SetFlags(USE_LR_TO_RETURN);
			movingFurnitureHook.Enable();

			pickingUpFurnitureHook.Initialize(pickingUpFurniture.addr, (u32)MoveFurniturePatch);
		  	pickingUpFurnitureHook.SetFlags(USE_LR_TO_RETURN);
			pickingUpFurnitureHook.Enable();

			placingFurnitureHook1.Initialize(placingFurniture1.addr, (u32)MoveFurniturePatch);
		  	placingFurnitureHook1.SetFlags(USE_LR_TO_RETURN);
			placingFurnitureHook1.Enable();

			placingFurnitureHook2.Initialize(placingFurniture2.addr, (u32)MoveFurniturePatch);
		  	placingFurnitureHook2.SetFlags(USE_LR_TO_RETURN);
			placingFurnitureHook2.Enable();

			placingFurnitureHook3.Initialize(placingFurniture3.addr, (u32)MoveFurniturePatch);
		  	placingFurnitureHook3.SetFlags(USE_LR_TO_RETURN);
			placingFurnitureHook3.Enable();

			lightSwitchVisibleHook.Initialize(lightswitchVisible.addr, (u32)LightSwitchPatch);
		  	lightSwitchVisibleHook.SetFlags(USE_LR_TO_RETURN);
			lightSwitchVisibleHook.Enable();

			lightSwitchFunctionHook.Initialize(lightswitchFunction.addr, (u32)LightSwitchPatch);
		  	lightSwitchFunctionHook.SetFlags(USE_LR_TO_RETURN);
			lightSwitchFunctionHook.Enable();

			moveFurnButtonHook.Initialize(moveFurnButton.addr, (u32)PATCH_MoveFurnButton);
			moveFurnButtonHook.SetFlags(USE_LR_TO_RETURN);
			moveFurnButtonHook.Enable();
		}
		
		else if(!entry->IsActivated()) {
			movingFurnitureHook.Disable();
			pickingUpFurnitureHook.Disable();
			placingFurnitureHook1.Disable();
			placingFurnitureHook2.Disable();
			placingFurnitureHook3.Disable();
			lightSwitchVisibleHook.Disable();
			lightSwitchFunctionHook.Disable();
			moveFurnButtonHook.Disable();
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
			
//Beans Particle Changer	
	void BeansParticleChanger(MenuEntry *entry) {
		static Address beans(0x673E0C);
        static u16 input = 0; 
		
        if(entry->Hotkeys[0].IsDown()) {
			if(Wrap::KB<u16>(Language::getInstance()->get(TextID::BEANS_PARTICLE_ENTER_ID), true, 3, input, 0)) {
				beans.Patch(input);
			}
		}
		
		if(!entry->IsActivated()) {
			beans.Unpatch();
		}
	}

//Fast Mode
	void fast(MenuEntry *entry) {
		if (entry->WasJustActivated()) {
			RuntimeContext::getInstance()->setTurbo(true);
		} else if (!entry->IsActivated()) {
			RuntimeContext::getInstance()->setTurbo(false);
		}
	}

	static bool gFastTalkEnabled = false;
	static bool gSpeedEnabled = false;

//Fast Text Speed
	void fasttalk(MenuEntry *entry) {
		gFastTalkEnabled = entry->IsActivated();

		static Address fastt(0x5FC6AC);
		static Address fastt2 = fastt.MoveOffset(8);

		if(entry->WasJustActivated()) {
			fastt.Patch(0xEA000000);
			fastt2.Patch(0xE3500001);
		}
		if(!gFastTalkEnabled) {
			fastt.Unpatch();
			fastt2.Unpatch();
		}	
	}

//Fast Game Speed
	void speedentry(MenuEntry *entry) {
		gSpeedEnabled = entry->IsActivated();

		static Address speed(0x54DDB4);

		if (Game::IsGameSaving()) {
			speed.Unpatch();
		}
		else {
			speed.Patch(0xE3E004FF);
		}
		
		if(!gSpeedEnabled) {
			speed.Unpatch();
		}
	}
//Fast Isabelle (Fast Text + Game Speed when in the Isabelle greeting room)
	void fastisabelle(MenuEntry *entry) {
		if (entry->WasJustActivated() && Game::GetRoom() == 0x63) {
			OSDExtras::Notify(TextID::FAST_PLAYER_SELECT_INFO, Color::Red);
			entry->Disable();
			return;
		}

		static Address blockKeys(0x541FF8);
		static Address speed(0x54DDB4);
		static Address fastt(0x5FC6AC);
		static Address fastt2 = fastt.MoveOffset(8);

		static bool blockKeysPatchedByIsabelle = false;
		static bool speedPatchedByIsabelle = false;
		static bool fastTalkPatchedByIsabelle = false;

		bool active = (Game::GetRoom() == 0x63 && entry->IsActivated());

		if (active) { // Isabelle
			if (!gSpeedEnabled) {
				if (Game::IsGameSaving()) {
					speed.Unpatch();
					speedPatchedByIsabelle = false;
				}
				else {
					speed.Patch(0xE3E004FF);
					speedPatchedByIsabelle = true;
				}
			}
			
			if (!gFastTalkEnabled && !fastTalkPatchedByIsabelle) {
				fastt.Patch(0xEA000000);
				fastt2.Patch(0xE3500001);
				fastTalkPatchedByIsabelle = true;
			}


			if (!blockKeysPatchedByIsabelle) {
				blockKeys.Patch(0xEA000068); //Block all keys except A
				blockKeysPatchedByIsabelle = true;
			}

			Controller::InjectKey(Key::A);
		}
		else {
			if (speedPatchedByIsabelle && !gSpeedEnabled) {
				speed.Unpatch();
				speedPatchedByIsabelle = false;
			}
			
			if (fastTalkPatchedByIsabelle && !gFastTalkEnabled) {
				fastt.Unpatch();
				fastt2.Unpatch();
				fastTalkPatchedByIsabelle = false;
			}

			if (blockKeysPatchedByIsabelle) {
				blockKeys.Unpatch();
				blockKeysPatchedByIsabelle = false;
			}
		}
	}
}
