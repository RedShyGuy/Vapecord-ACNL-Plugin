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
		static const Address push("PUSH");

		if(entry->WasJustActivated()) 
			Process::Patch(push.addr, 0xEA00002D);
		else if(!entry->IsActivated())
			Process::Patch(push.addr, 0x2A00002D);
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
				if(entry->Hotkeys[1].IsDown()) 
					pCoords[0] += cspeed; //DPadRight
				if(entry->Hotkeys[2].IsDown()) 
					pCoords[0] -= cspeed; //DPadLeft
				if(entry->Hotkeys[3].IsDown()) 
					pCoords[2] += cspeed; //DPadDown
				if(entry->Hotkeys[4].IsDown()) 
					pCoords[2] -= cspeed; //DPadUp
			}
		}
	}
//Moonjump
	void moonjump(MenuEntry *entry) {
		u32 i = PlayerClass::GetInstance()->Offset(0x8C6);
		if(entry->Hotkeys[0].IsDown()) 
			Process::Write32(i, 0x7FFFFF); 
		else if(entry->Hotkeys[1].IsDown()) 
			Process::Write32(i, 0x19D5D);
	}
	
//Touch Warp
	void tch_warp(MenuEntry *entry) {
		float *pCoords = PlayerClass::GetInstance()->GetCoordinates();
		if(pCoords == nullptr)
			return;
		
		if(!GameHelper::MapBoolCheck()) 
			return;

		if(!Touch::IsDown())
			return;

		PlayerClass::CalculateCoordinates(Touch::GetPosition(), pCoords);
    }
//Walk Over Things
	void walkOver(MenuEntry *entry) {		
		static const Address walkover1("WALKOVER1");
		static const Address walkover2("WALKOVER2");
		static const Address walkover3("WALKOVER3");
		static const Address walkover4("WALKOVER4");
		static const Address walkover5("WALKOVER5");
		static const Address walkover6("WALKOVER6");
		static const Address walkover7("WALKOVER7");
		static const Address walkover8("WALKOVER8");
		
		const u32 WalkOver[8] = { walkover1.addr, walkover2.addr, walkover3.addr, walkover4.addr, walkover5.addr, walkover6.addr, walkover7.addr, walkover8.addr };
		
		static const u32 WalkOverPatch[2][8] = {
            { 0xEA000094, 0xEA000052, 0xEA000001, 0xEA000014, 0xE1A00000, 0xE1A00000, 0xEA000026, 0xEA000065 },
            { 0x0A000094, 0x0A000052, 0x0A000001, 0xDA000014, 0xED841A05, 0xED840A07, 0x0A000026, 0x0A000065 }
        };
		
		if(entry->Hotkeys[0].IsPressed()) {
			bool index = *(u32 *)walkover1.addr == 0x0A000094 ? 0 : 1;

            OSD::Notify("Walk Over Things " << (index ? (Color::Red << "OFF") : (Color::Green << "ON")));

			for(int i = 0; i < 8; ++i)
                Process::Patch(WalkOver[i], WalkOverPatch[index][i]);
        }
	    if(!entry->IsActivated()) {
			for(int i = 0; i < 8; ++i)
                Process::Patch(WalkOver[i], WalkOverPatch[1][i]);
		}
    }
//Movement Changer
	void MovementChanger(MenuEntry *entry) {	
		static const Address Disable25Anim("DISABLE25ANIM"); //Disable going out of water animation
		static const Address AlwaysWalk("ALWAYSWALK"); //Makes you walk all the time
		static const Address AlwaysSwim("ALWAYSSWIM"); //Makes you swim all the time
		static const Address DisableYChange("DISABLEYCHANGE"); //Makes Y-Coord not go down when swimming	
		static const Address move4("MOVE4");
		static const Address move5("MOVE5");
		
		static const u32 MoveChanger[6] = { Disable25Anim.addr, AlwaysWalk.addr, AlwaysSwim.addr, DisableYChange.addr, move4.addr, move5.addr };
		
		static const u32 MoveChangerPatch[3][6] = {
            { 0xEA000067, 0x03A00001, 0xE3A00001, 0xE12FFF1E, 0xEA00000D, 0xE3A00000 }, //Swimming
            { 0x1A000067, 0x03A00000, 0xE3A00000, 0xED902A00, 0xE1A00004, 0xE3A00001 }, //Walking
			{ 0x1A000067, 0x03A00001, 0xE3A00000, 0xED902A00, 0xE1A00004, 0xE3A00001 } //OFF
        };
		
		bool index = *(u32 *)Disable25Anim.addr == 0x1A000067 ? 0 : 1;
	
		if(entry->Hotkeys[0].IsPressed()) {
			if(index) 
                OSD::Notify("Movement Mode: Walking", Color::Green);	
		    else 
                OSD::Notify("Movement Mode: Swimming", Color::Blue);
				
			for(int i = 0; i < 6; ++i)
                Process::Patch(MoveChanger[i], MoveChangerPatch[index][i]);
        }
		
		if(!entry->IsActivated()) {
			for(int i = 0; i < 6; ++i)
                Process::Patch(MoveChanger[i], MoveChangerPatch[2][i]);
		}
    }
//Walk Particle	
	void Walkparticle(MenuEntry *entry) {
		static const Address WalkParticlePatch("WALKPARTICLEPATCH");
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
			
		if(!entry->IsActivated()) 
			hook.Disable();
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
		static const Address visi1("VISI1");
		static const Address visi2("VISI2");
		static const Address visi3("VISI3");
		
		static const u32 VisiMod[3] = { visi1.addr, visi2.addr, visi3.addr };
		
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

			for(int i = 0; i < 3; ++i)
                Process::Patch(VisiMod[i], VisiModPatch[mode][i]);
        }

		if(!entry->IsActivated()) {
			for(int i = 0; i < 3; ++i)
                Process::Patch(VisiMod[i], VisiModPatch[2][i]);
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
		static const Address sp1("SP1");
		static const Address sp2("SP2");
		static const Address sp3("SP3");
		static const Address sp4("SP4");
		static const Address sp5("SP5");
		static const Address sp6("SP6");
		static const Address sp7("SP7");
		static const Address sp8("SP8");
		
		if(!entry->IsActivated()) 
			walkSpeed = 1;
		
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
				s8 res = GameHelper::RoomFunction(val, 1, 1, 0);	
				if(res == 1)
					OSD::Notify(Utils::Format("Warping to room %02X", val));
				else if(res == -1)
					OSD::Notify("Player needs to be loaded to warp!", Color::Red);
				else if(res == -2)
					OSD::Notify("Only works while playing offline!", Color::Red);
				else
					OSD::Notify("An error has occured while trying to warp!", Color::Red);
			}
		}
	}

//Shovel Knockback
	void shovelknockback(MenuEntry *entry) {
		static Address rockHitting("ROCKHITTING");
		static Address itemHitting("ITEMHITTING");

		if(entry->WasJustActivated()) {
			Process::Patch(rockHitting.addr, 0xEB000000);
			Process::Patch(itemHitting.addr, 0xEB000000);
		}
		
		else if(!entry->IsActivated()) {
			Process::Patch(rockHitting.addr, 0xE1A00004);
			Process::Patch(itemHitting.addr, 0xE1A00004);
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
