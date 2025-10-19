#include "cheats.hpp"
#include "Helpers/Address.hpp"
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
		static const Address push(0x652288, 0x6517B0, 0x6512C0, 0x6512C0, 0x650D80, 0x650D80, 0x650928, 0x650928);

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
		static const Address walkover1(0x6503FC, 0x64F924, 0x64F434, 0x64F434, 0x64EEF4, 0x64EEF4, 0x64EA9C, 0x64EA9C);
		static const Address walkover2(0x650414, 0x64F93C, 0x64F44C, 0x64F44C, 0x64EF0C, 0x64EF0C, 0x64EAB4, 0x64EAB4);
		static const Address walkover3(0x650578, 0x64FAA0, 0x64F5B0, 0x64F5B0, 0x64F070, 0x64F070, 0x64EC18, 0x64EC18);
		static const Address walkover4(0x6505F0, 0x64FB18, 0x64F628, 0x64F628, 0x64F0E8, 0x64F0E8, 0x64EC90, 0x64EC90);
		static const Address walkover5(0x6506A4, 0x64FBCC, 0x64F6DC, 0x64F6DC, 0x64F19C, 0x64F19C, 0x64ED44, 0x64ED44);
		static const Address walkover6(0x6506BC, 0x64FBE4, 0x64F6F4, 0x64F6F4, 0x64F1B4, 0x64F1B4, 0x64ED5C, 0x64ED5C);
		static const Address walkover7(0x6506C0, 0x64FBE8, 0x64F6F8, 0x64F6F8, 0x64F1B8, 0x64F1B8, 0x64ED60, 0x64ED60);
		static const Address walkover8(0x6506EC, 0x64FC14, 0x64F724, 0x64F724, 0x64F1E4, 0x64F1E4, 0x64ED8C, 0x64ED8C);
		
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
		static const Address Disable25Anim(0x67F748, 0x67EC70, 0x67E780, 0x67E780, 0x67E240, 0x67E240, 0x67DDE8, 0x67DDE8); //Disable going out of water animation
		static const Address AlwaysWalk(0x64E824, 0x64DD4C, 0x64D85C, 0x64D85C, 0x64D31C, 0x64D31C, 0x64CEC0, 0x64CEC0); //Makes you walk all the time
		static const Address AlwaysSwim(0x64E82C, 0x64DD54, 0x64D864, 0x64D864, 0x64D324, 0x64D324, 0x64CECC, 0x64CECC); //Makes you swim all the time
		static const Address DisableYChange(0x56BE7C, 0x56B394, 0x56AEC4, 0x56AEC4, 0x56A7B4, 0x56A7B4, 0x56A4D4, 0x56A4D4); //Makes Y-Coord not go down when swimming	
		static const Address move4(0x65352C, 0x652A54, 0x652564, 0x652564, 0x652024, 0x652024, 0x651BCC, 0x651BCC);
		static const Address move5(0x763ABC, 0x762AA0, 0x762AC4, 0x762A9C, 0x76225C, 0x762234, 0x761E04, 0x761E04);
		
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
		static const Address WalkParticlePatch(0x652694, 0x651BBC, 0x6516CC, 0x6516CC, 0x65118C, 0x65118C, 0x650D34, 0x650D34);
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
		static const Address visi1(0x655E44, 0x65536C, 0x654E7C, 0x654E7C, 0x65493C, 0x65493C, 0x6544E4, 0x6544E4);
		static const Address visi2(0x67743C, 0x676964, 0x676474, 0x676474, 0x675F34, 0x675F34, 0x675ADC, 0x675ADC);
		static const Address visi3(0x68DC3C, 0x68D164, 0x68CC74, 0x68CC74, 0x68C734, 0x68C734, 0x68C2DC, 0x68C2DC);
		
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
		static const Address sp1(0x887880, 0x886878, 0x88670C, 0x88670C, 0x880B2C, 0x87FB2C, 0x87FADC, 0x87FADC);
		static const Address sp2(0x887888, 0x886880, 0x886714, 0x886714, 0x880B34, 0x87FB34, 0x87FB34, 0x87FB34);
		static const Address sp3(0x887958, 0x886950, 0x8867E4, 0x8867E4, 0x880C04, 0x87FC04, 0x87FC04, 0x87FC04);
		static const Address sp4(0x5D4C80, 0x5D41B0, 0x5D3CC8, 0x5D3CC8, 0x5D34FC, 0x5D34FC, 0x5D31D0, 0x5D31D0);
		static const Address sp5(0x8879B8, 0x8869B0, 0x886844, 0x886844, 0x880C64, 0x87FC64, 0x87FC64, 0x87FC64);
		static const Address sp6(0x887C68, 0x886C60, 0x886AF4, 0x886AF4, 0x880F14, 0x87FF14, 0x87FF14, 0x87FF14);
		static const Address sp7(0x94EF34, 0x94DF24, 0x94DF34, 0x94DF34, 0x947F34, 0x946F34, 0x946F34, 0x946F34);
		static const Address sp8(0x8878A4, 0x88689C, 0x886730, 0x886730, 0x880B50, 0x87FB50, 0x87FB50, 0x87FB50);
		
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
		static Address rockHitting(0x66E9F0, 0x66DF18, 0x66DA28, 0x66DA28, 0x66D4E8, 0x66D4E8, 0x66D090, 0x66D090);
		static Address itemHitting(0x67211C, 0x671644, 0x671154, 0x671154, 0x670C14, 0x670C14, 0x6707BC, 0x6707BC);

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
