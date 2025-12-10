#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Dropper.hpp"
#include "Color.h"

extern "C" void SetWalkParticleID(void);

u32 WalkParticleID = 0;

namespace CTRPluginFramework {
//Players can't push you
	void noPush(MenuEntry *entry) { 
		static Address push(0x652288);

		if(entry->WasJustActivated()) {
			push.Patch(0xEA00002D);
		}
		else if(!entry->IsActivated()) {
			push.Unpatch();
		}
	}

//Definition for Coordinate Mod Speed
	float cspeed = 5.0;
//Coordinate Mod Speed Changer Keyboard
	void coordspeed(MenuEntry *entry) {
		Keyboard kb(Language::getInstance()->get("ENTER_ID"));
		kb.GetMessage() = Utils::Format(Language::getInstance()->get("COORD_MOD_ENTER_SPEED").c_str(), 5);
		kb.IsHexadecimal(false);
		kb.SetMaxLength(2);
		kb.Open(cspeed);
	}

//Coordinate Modifier
	void coordinate(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsDown()) {//A
			float *pCoords = PlayerClass::GetInstance()->GetCoordinates();
			if(pCoords != nullptr && !MapEditorActive) { // if not in tile selection mode & valid player obj
				if(entry->Hotkeys[1].IsDown()) {
					pCoords[0] += cspeed; //DPadRight
				}
				if(entry->Hotkeys[2].IsDown()) {
					pCoords[0] -= cspeed; //DPadLeft
				}
				if(entry->Hotkeys[3].IsDown()) {
					pCoords[2] += cspeed; //DPadDown
				}
				if(entry->Hotkeys[4].IsDown()) {
					pCoords[2] -= cspeed; //DPadUp
				}		
			}
		}
	}

//Moonjump
	void moonjump(MenuEntry *entry) {
		u32 i = PlayerClass::GetInstance()->Offset(0x8C6);
		if(entry->Hotkeys[0].IsDown()) {
			Process::Write32(i, 0x7FFFFF); 
		}
		else if(entry->Hotkeys[1].IsDown()) {
			Process::Write32(i, 0x19D5D);
		}
	}
	
//Touch Warp
	void tch_warp(MenuEntry *entry) {
		float *pCoords = PlayerClass::GetInstance()->GetCoordinates();
		if(pCoords == nullptr) {
			return;
		}
		
		if(!Game::MapBoolCheck()) {
			return;
		}

		if(!Touch::IsDown()) {
			return;
		}

		PlayerClass::CalculateCoordinates(Touch::GetPosition(), pCoords);
    }

//Walk Over Things
	void walkOver(MenuEntry *entry) {		
		static Address walkover1(0x6503FC);
		static Address walkover2(0x650414);
		static Address walkover3(0x650578);
		static Address walkover4(0x6505F0);
		static Address walkover5(0x6506A4);
		static Address walkover6(0x6506BC);
		static Address walkover7(0x6506C0);
		static Address walkover8(0x6506EC);
		
		static Address WalkOver[8] = { 
			walkover1, walkover2, walkover3, walkover4, 
			walkover5, walkover6, walkover7, walkover8 
		};
		
		static const u32 WalkOverPatch[8] = {
            0xEA000094, 0xEA000052, 0xEA000001, 0xEA000014, 
			0xE1A00000, 0xE1A00000, 0xEA000026, 0xEA000065
        };
		
		if(entry->Hotkeys[0].IsPressed()) {
			bool isOFF = *(u32 *)walkover1.addr == walkover1.origVal;

			if (isOFF) {
				for(int i = 0; i < 8; ++i) {
					WalkOver[i].Patch(WalkOverPatch[i]);
				}
				OSD::Notify("Walk Over Things " << Color::Green << "ON");
			}
			else {
				for(int i = 0; i < 8; ++i) {
					WalkOver[i].Unpatch();
				}
				OSD::Notify("Walk Over Things " << Color::Red << "OFF");
			}
        }

	    if(!entry->IsActivated()) {
			for(int i = 0; i < 8; ++i) {
				WalkOver[i].Unpatch();
			}
		}
    }
//Movement Changer
	void MovementChanger(MenuEntry *entry) {	
		static Address Disable25Anim(0x67F748); //Disable going out of water animation
		static Address AlwaysWalk(0x64E824); //Makes you walk all the time
		static Address AlwaysSwim(0x64E82C); //Makes you swim all the time
		static Address DisableYChange(0x56BE7C); //Makes Y-Coord not go down when swimming	
		static Address move4(0x65352C);
		static Address move5(0x763ABC);
		
		static Address MoveChanger[6] = { 
			Disable25Anim, AlwaysWalk, AlwaysSwim, DisableYChange, move4, move5 
		};
		
		static const u32 MoveChangerPatch[3][6] = {
			{ 0x1A000067, 0x03A00000, 0xE3A00000, 0xED902A00, 0xE1A00004, 0xE3A00001 }, //Walking
            { 0xEA000067, 0x03A00001, 0xE3A00001, 0xE12FFF1E, 0xEA00000D, 0xE3A00000 } //Swimming
        };
		
		bool isWalking = *(u32 *)Disable25Anim.addr == Disable25Anim.origVal;
	
		if(entry->Hotkeys[0].IsPressed()) {
			if(isWalking) {
				OSD::Notify("Movement Mode: Swimming", Color::Blue);
			}
		    else {
				OSD::Notify("Movement Mode: Walking", Color::Green);
			}
			
			for(int i = 0; i < 6; ++i) {
				MoveChanger[i].Patch(MoveChangerPatch[isWalking][i]);
			}
        }
		
		if(!entry->IsActivated()) {
			for(int i = 0; i < 6; ++i) {
				MoveChanger[i].Unpatch();
			}
		}
    }
//Walk Particle	
	void Walkparticle(MenuEntry *entry) {
		static const Address WalkParticlePatch(0x652694);
		static Hook hook;

		if(entry->WasJustActivated()) {
			hook.Initialize(WalkParticlePatch.addr, (u32)SetWalkParticleID);
			hook.SetFlags(USE_LR_TO_RETURN);
		}

        if(entry->Hotkeys[0].IsPressed()) {
			if(Wrap::KB<u32>(Language::getInstance()->get("WALK_PARTICLE_CHANGE_ENTER_ID"), true, 3, WalkParticleID, WalkParticleID)) {
				hook.Enable();
			}
		}
			
		if(!entry->IsActivated()) {
			hook.Disable();
		}
    }  
//Player Teleporter	
	void stalk(MenuEntry *entry) {
		static s8 pos = -1;
		static bool allforce = false;

		if(entry->Hotkeys[0].IsPressed()) {
			if(pos < 3) {
				allforce = false;
				pos++;
			}
			else {
				allforce = true;
				pos = -1;
			}
				
			OSD::Notify(allforce ? "Teleport: all players" : Utils::Format("Teleport: player %02X", pos));
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {
			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				if(!allforce && pos >= 0) {
					Animation::ExecuteAnimationWrapper(pos, 0x34, {1, 0}, 1, 1, 1, 0, x, y, true);
					OSD::Notify(Utils::Format("Teleported player %02X to you", pos));
				}
				else {
					for(u8 i = 0; i < 4; ++i) {
						Animation::ExecuteAnimationWrapper(i, 0x34, {1, 0}, 1, 1, 1, 0, x, y, true);
					}
					OSD::Notify("Teleported players to you");
				}
			}
		}
	}
//Player Visibility Changer	
	void onlineplayermod(MenuEntry *entry) {
		static Address visi1(0x655E44);
		static Address visi2(0x67743C);
		static Address visi3(0x68DC3C);
		
		static Address VisiMod[3] = { 
			visi1, visi2, visi3 
		};
		
		static const u32 VisiModPatch[3][3] = { 
            { 0xE3A01017, 0xE3A07006, 0xE1A00000 },
            { 0xE3A01016, 0xE3A07000, 0x1BFF021A },
			{ 0xE3A01017, 0xE1A07002, 0x1BFF021A }
        };
		
		if(entry->Hotkeys[0].IsPressed()) {
			int mode = 0;
			
			switch(*(u32 *)visi2.addr) {
				case 0xE1A07002:
					mode = 0;
					OSD::Notify("Visibility: Stationary", Color::Blue);
				break;
				case 0xE3A07006:
					mode = 1;
					OSD::Notify("Visibility: Invisible", Color::Yellow);
				break;
				case 0xE3A07000:
					mode = 2;
					OSD::Notify("Visibility: Default", Color::Green);
				break;
			}

			for(int i = 0; i < 3; ++i) {
				VisiMod[i].Patch(VisiModPatch[mode][i]);
			}
        }

		if(!entry->IsActivated()) {
			for(int i = 0; i < 3; ++i) {
				VisiMod[i].Unpatch();
			}
		}
    }

	void SpeedCheck(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		float ID = atof(input.c_str());
		if(ID >= 15.0f) {
			keyboard.SetError(Color::Red << Language::getInstance()->get("SPEED_MOD_ERROR"));
			return;
		}
	}

//Definition for changable Speed of the Player Speed Changer
	float walkSpeed = 1;
//Player Speed Changer	
	void speedMod(MenuEntry *entry) {
		static const Address sp1(0x887880);
		static const Address sp2(0x887888);
		static const Address sp3(0x887958);
		static const Address sp4(0x5D4C80);
		static const Address sp5(0x8879B8);
		static const Address sp6(0x887C68);
		static const Address sp7(0x94EF34);
		static const Address sp8(0x8878A4);
		
		if(!entry->IsActivated()) {
			walkSpeed = 1;
		}
		
		Process::WriteFloat(sp1.addr, walkSpeed);
		Process::WriteFloat(sp2.addr, walkSpeed);
		Process::WriteFloat(sp3.addr, walkSpeed);
		Process::WriteFloat(sp4.addr, 1.8f * walkSpeed * 4096.0f); //swim speed
		Process::Write16(sp5.addr, 0x16BC); //fast 0x20 rotate
		Process::Write16(sp6.addr, 0xB5E); //fast 0x1F rotate
		Process::Write16(sp7.addr, 0xB5E); //fast 0x1F rotate
		Process::Write8(sp8.addr, 0x14 * walkSpeed); //slide
	}
//Player Speed Changer Keyboard
	void menuSpeedMod(MenuEntry *entry) {
		Keyboard kb(Language::getInstance()->get("ENTER_ID"));
		kb.GetMessage() = Utils::Format(Language::getInstance()->get("SPEED_MOD_SPEED").c_str(), 1);
		kb.IsHexadecimal(false);
		kb.SetMaxLength(7);
		kb.OnKeyboardEvent(SpeedCheck);
		kb.Open(walkSpeed);
	}

//InputChangeEvent for Room Warper	
	void onRoomChange(Keyboard &k, KeyboardEvent &e) {
		std::string& input = k.GetInput();	
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(!IDList::RoomValid(ID & 0xFF)) {
			k.SetError(Color::Red << Language::getInstance()->get("INVALID_ID"));
			return;
		}
		
		k.GetMessage() = Language::getInstance()->get("ROOM_WARPING_ENTER_ID") << "\n\n" << IDList::GetRoomName(!input.empty() ? ID : 0);
	}

//Room Warper
	void roomWarp(MenuEntry *entry) {	
		if(entry->Hotkeys[0].IsPressed()) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify("Player needs to be loaded to use room teleport!");
				return;
			}		
			
			u8 val;
			if(Wrap::KB<u8>(Language::getInstance()->get("ROOM_WARPING_ENTER_ID"), true, 2, val, 0, onRoomChange)) {		
				s8 res = Game::RoomFunction(val, 1, 1, 0);	
				if(res == 1) {
					OSD::Notify(Utils::Format("Warping to room %02X", val));
				}
				else if(res == -1) {
					OSD::Notify("Player needs to be loaded to warp!", Color::Red);
				}
				else if(res == -2) {
					OSD::Notify("Only works while playing offline!", Color::Red);
				}
				else {
					OSD::Notify("An error has occured while trying to warp!", Color::Red);
				}
			}
		}
	}

//Shovel Knockback
	void shovelknockback(MenuEntry *entry) {
		static Address rockHitting(0x66E9F0);
		static Address itemHitting(0x67211C);

		if(entry->WasJustActivated()) {
			rockHitting.Patch(0xEB000000);
			itemHitting.Patch(0xEB000000);
		}
		
		else if(!entry->IsActivated()) {
			rockHitting.Unpatch();
			itemHitting.Unpatch();
		}
	}
}

//disable Return To Town 0x61F508 0xE1A00000


//disable Arrive At Hut 0x61F8C4 0xE1A00000

//me: 61F50C
//force: 627B88

//0x31FE4ECC
//0x61E8BC ArriveAtIsland
//0x61F258 ArriveAtTown 
//game uses 61B9F8 
//me uses same 61B9F8
//force uses 627B88

//me 626698
//other 61F584
//0x61E954 LeaveIsland

//61F564


//627B88

//627B88
//61B9F8

//61F584
//628B88
//6228D4
//628BB8



//5B6848 Func for Return To Town + Arrive At Hut
//5B6660 Actual Func which does room warp
