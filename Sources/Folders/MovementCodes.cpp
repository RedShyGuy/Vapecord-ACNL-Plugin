#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

extern "C" void SetWalkParticleID(void);

u32 WalkParticleID = 0;

namespace CTRPluginFramework {
//Players can't push you
	void noPush(MenuEntry *entry) { 
		static const u32 push = Region::AutoRegion(0x652288, 0x6517B0, 0x6512C0, 0x6512C0, 0x650D80, 0x650D80, 0x650928, 0x650928);
		
		std::vector<std::string> cmnOpt =  { "" };

		bool IsON = *(u32 *)push == 0xEA00002D;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
			
		Process::Patch(push, IsON ? 0x2A00002D : 0xEA00002D);
		noPush(entry);
	}
//Definition for Coordinate Mod Speed
	float cspeed = 5.0;
//Coordinate Mod Speed Changer Keyboard
	void coordspeed(MenuEntry *entry) {
		Keyboard kb(Language->Get("ENTER_ID"));
		kb.GetMessage() = Utils::Format(Language->Get("COORD_MOD_ENTER_SPEED").c_str(), 5);
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
		static const u32 walkover1 = Region::AutoRegion(0x6503FC, 0x64F924, 0x64F434, 0x64F434, 0x64EEF4, 0x64EEF4, 0x64EA9C, 0x64EA9C);
		static const u32 walkover2 = Region::AutoRegion(0x650414, 0x64F93C, 0x64F44C, 0x64F44C, 0x64EF0C, 0x64EF0C, 0x64EAB4, 0x64EAB4);
		static const u32 walkover3 = Region::AutoRegion(0x650578, 0x64FAA0, 0x64F5B0, 0x64F5B0, 0x64F070, 0x64F070, 0x64EC18, 0x64EC18);
		static const u32 walkover4 = Region::AutoRegion(0x6505F0, 0x64FB18, 0x64F628, 0x64F628, 0x64F0E8, 0x64F0E8, 0x64EC90, 0x64EC90);
		static const u32 walkover5 = Region::AutoRegion(0x6506A4, 0x64FBCC, 0x64F6DC, 0x64F6DC, 0x64F19C, 0x64F19C, 0x64ED44, 0x64ED44);
		static const u32 walkover6 = Region::AutoRegion(0x6506BC, 0x64FBE4, 0x64F6F4, 0x64F6F4, 0x64F1B4, 0x64F1B4, 0x64ED5C, 0x64ED5C);
		static const u32 walkover7 = Region::AutoRegion(0x6506C0, 0x64FBE8, 0x64F6F8, 0x64F6F8, 0x64F1B8, 0x64F1B8, 0x64ED60, 0x64ED60);
		static const u32 walkover8 = Region::AutoRegion(0x6506EC, 0x64FC14, 0x64F724, 0x64F724, 0x64F1E4, 0x64F1E4, 0x64ED8C, 0x64ED8C);
		
		const u32 WalkOver[8] = { walkover1, walkover2, walkover3, walkover4, walkover5, walkover6, walkover7, walkover8 };
		
		static const u32 WalkOverPatch[2][8] = {
            { 0xEA000094, 0xEA000052, 0xEA000001, 0xEA000014, 0xE1A00000, 0xE1A00000, 0xEA000026, 0xEA000065 },
            { 0x0A000094, 0x0A000052, 0x0A000001, 0xDA000014, 0xED841A05, 0xED840A07, 0x0A000026, 0x0A000065 }
        };
		
		if(entry->Hotkeys[0].IsPressed()) {
			bool index = *(u32 *)walkover1 == 0x0A000094 ? 0 : 1;

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
		static const u32 move1 = Region::AutoRegion(0x64E824, 0x64DD4C, 0x64D85C, 0x64D85C, 0x64D31C, 0x64D31C, 0x64CEC0, 0x64CEC0);
		static const u32 move2 = Region::AutoRegion(0x64E82C, 0x64DD54, 0x64D864, 0x64D864, 0x64D324, 0x64D324, 0x64CECC, 0x64CECC);
		static const u32 move3 = Region::AutoRegion(0x653154, 0x65267C, 0x65218C, 0x65218C, 0x651C4C, 0x651C4C, 0x6517F4, 0x6517F4);
		static const u32 move4 = Region::AutoRegion(0x653530, 0x652A58, 0x652568, 0x652568, 0x652028, 0x652028, 0x651BD0, 0x651BD0);
		static const u32 move5 = Region::AutoRegion(0x763ABC, 0x762AA0, 0x762AC4, 0x762A9C, 0x76225C, 0x762234, 0x761E04, 0x761E049);
		static const u32 moveoff = *(u32 *)move3;
		
		static const u32 MoveChanger[5] = { move1, move2, move3, move4, move5 };
		
		static const u32 MoveChangerPatch[2][5] = {
            { 0x03A00001, 0xE3A00001, 0xE1A00000, 0xE3A00000, 0xE3A00000 },
            { 0x03A00000, 0xE3A00000, moveoff, 0xEB00AFB7, 0xE3A00001 }
        };
		
		bool index = *(u32 *)move2 == 0xE3A00000 ? 0 : 1;
	
		if(entry->Hotkeys[0].IsPressed()) {
			if(index) 
                OSD::Notify("Movement Mode: Walking", Color::Green);	
		    else 
                OSD::Notify("Movement Mode: Swimming", Color::Blue);
				
			for(int i = 0; i < 5; ++i)
                Process::Patch(MoveChanger[i], MoveChangerPatch[index][i]);
        }
		
		if(!entry->IsActivated()) {
			for(int i = 0; i < 5; ++i)
                Process::Patch(MoveChanger[i], MoveChangerPatch[1][i]);
		}
    }
//Walk Particle	
	void Walkparticle(MenuEntry *entry) {
		static const u32 WalkParticlePatch = Region::AutoRegion(0x652694, 0x651BBC, 0x6516CC, 0x6516CC, 0x65118C, 0x65118C, 0x650D34, 0x650D34);
		static Hook hook;

		if(entry->WasJustActivated()) {
			hook.Initialize(WalkParticlePatch, (u32)SetWalkParticleID);
			hook.SetFlags(USE_LR_TO_RETURN);
		}

        if(entry->Hotkeys[0].IsPressed()) {
			if(Wrap::KB<u32>(Language->Get("WALK_PARTICLE_CHANGE_ENTER_ID"), true, 3, WalkParticleID, WalkParticleID)) {
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
					Animation::ExecuteAnimationWrapper(pos, 0x34, 1, 1, 1, 1, 0, x, y, true);
					OSD::Notify(Utils::Format("Teleported player %02X to you", pos));
				}
				else {
					for(u8 i = 0; i < 4; ++i) {
						Animation::ExecuteAnimationWrapper(i, 0x34, 1, 1, 1, 1, 0, x, y, true);
					}
					OSD::Notify("Teleported players to you");
				}
			}
		}
	}
//Player Visibility Changer	
	void onlineplayermod(MenuEntry *entry) {
		static const u32 visi1 = Region::AutoRegion(0x655E44, 0x65536C, 0x654E7C, 0x654E7C, 0x65493C, 0x65493C, 0x6544E4, 0x6544E4);
		static const u32 visi2 = Region::AutoRegion(0x67743C, 0x676964, 0x676474, 0x676474, 0x675F34, 0x675F34, 0x675ADC, 0x675ADC);
		static const u32 visi3 = Region::AutoRegion(0x68DC3C, 0x68D164, 0x68CC74, 0x68CC74, 0x68C734, 0x68C734, 0x68C2DC, 0x68C2DC);
		
		static const u32 VisiMod[3] = { visi1, visi2, visi3 };
		
		static const u32 VisiModPatch[3][3] = { 
            { 0xE3A01017, 0xE3A07006, 0xE1A00000 },
            { 0xE3A01016, 0xE3A07000, 0x1BFF021A },
			{ 0xE3A01017, 0xE1A07002, 0x1BFF021A }
        };
		
		if(entry->Hotkeys[0].IsPressed()) {
			int mode = 0;
			
			switch(*(u32 *)visi2) {
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
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(ID >= 15) {
			keyboard.SetError(Color::Red << Language->Get("SPEED_MOD_ERROR"));
			return;
		}
	}

//Definition for changable Speed of the Player Speed Changer
	float walkSpeed = 1;
//Player Speed Changer	
	void speedMod(MenuEntry *entry) {
		static const u32 sp1 = Region::AutoRegion(0x887880, 0x886878, 0x88670C, 0x88670C, 0x880B2C, 0x87FB2C, 0x87FADC, 0x87FADC);
		static const u32 sp2 = Region::AutoRegion(0x887888, 0x886880, 0x886714, 0x886714, 0x880B34, 0x87FB34, 0x87FB34, 0x87FB34);
		static const u32 sp3 = Region::AutoRegion(0x887958, 0x886950, 0x8867E4, 0x8867E4, 0x880C04, 0x87FC04, 0x87FC04, 0x87FC04);
		static const u32 sp4 = Region::AutoRegion(0x5D4C80, 0x5D41B0, 0x5D3CC8, 0x5D3CC8, 0x5D34FC, 0x5D34FC, 0x5D31D0, 0x5D31D0);
		static const u32 sp5 = Region::AutoRegion(0x8879B8, 0x8869B0, 0x886844, 0x886844, 0x880C64, 0x87FC64, 0x87FC64, 0x87FC64);
		static const u32 sp6 = Region::AutoRegion(0x887C68, 0x886C60, 0x886AF4, 0x886AF4, 0x880F14, 0x87FF14, 0x87FF14, 0x87FF14);
		static const u32 sp7 = Region::AutoRegion(0x94EF34, 0x94DF24, 0x94DF34, 0x94DF34, 0x947F34, 0x946F34, 0x946F34, 0x946F34);
		static const u32 sp8 = Region::AutoRegion(0x8878A4, 0x88689C, 0x886730, 0x886730, 0x880B50, 0x87FB50, 0x87FB50, 0x87FB50);
		
		if(!entry->IsActivated()) 
			walkSpeed = 1;
		
		Process::WriteFloat(sp1, walkSpeed);
		Process::WriteFloat(sp2, walkSpeed);
		Process::WriteFloat(sp3, walkSpeed);
		Process::WriteFloat(sp4, 1.8f * walkSpeed * 4096.0f); //swim speed
		Process::Write16(sp5, 0x16BC); //fast 0x20 rotate
		Process::Write16(sp6, 0xB5E); //fast 0x1F rotate
		Process::Write16(sp7, 0xB5E); //fast 0x1F rotate
		Process::Write8(sp8, 0x14 * walkSpeed); //slide
	}
//Player Speed Changer Keyboard
	void menuSpeedMod(MenuEntry *entry) {
		Keyboard kb(Language->Get("ENTER_ID"));
		kb.GetMessage() = Utils::Format(Language->Get("SPEED_MOD_SPEED").c_str(), 1);
		kb.IsHexadecimal(false);
		kb.SetMaxLength(2);
		kb.OnKeyboardEvent(SpeedCheck);
		kb.Open(walkSpeed);
	}	
//InputChangeEvent for Room Warper	
	void onRoomChange(Keyboard &k, KeyboardEvent &e) {
		std::string& input = k.GetInput();	
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(!IDList::RoomValid(ID & 0xFF)) {
			k.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}
		
		k.GetMessage() = Language->Get("ROOM_WARPING_ENTER_ID") << "\n\n" << IDList::GetRoomName(input != "" ? ID : 0);
	}
//Room Warper
	void roomWarp(MenuEntry *entry) {	
		if(entry->Hotkeys[0].IsPressed()) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify("Player needs to be loaded to use room teleport!");
				return;
			}		
			
			u8 val;
			Sleep(Milliseconds(100));
			if(Wrap::KB<u8>(Language->Get("ROOM_WARPING_ENTER_ID"), true, 2, val, 0, onRoomChange)) {		
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
