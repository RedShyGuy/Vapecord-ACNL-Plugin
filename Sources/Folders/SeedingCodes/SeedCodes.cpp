#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

extern "C" void PATCH_EverythingSeed(void);
extern "C" void PATCH_SnakeSpeed(void);
extern "C" void PATCH_PickupSeed(void);

extern "C" bool __IsPickSnakeID(u16 snakeID) {
	static const u16 snakeIDArr[19] = { 0x01, 0x25, 0x28, 0x3E, 0x3F, 0x47, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F, 0x5F, 0x60, 0xA6, 0xA7, 0xD0, 0xDA, 0xDB };
	return std::find(std::begin(snakeIDArr), std::end(snakeIDArr), snakeID) != std::end(snakeIDArr);
}

u32 EverythingSeederItemID = 0x80007FFE;
u32 PickupSeederItemID = 0x7FFE;

namespace CTRPluginFramework {
//Pickup Seeder
	void pickseeder(MenuEntry *entry) {
		static const u32 HoleSpeedPatch = Region::AutoRegion(0x65256C, 0x651A94, 0x6515A4, 0x6515A4, 0x651064, 0x651064, 0x650C0C, 0x650C0C);
		static const u32 AlwaysPickup = Region::AutoRegion(0x59A0C4, 0x5995DC, 0x59910C, 0x59910C, 0x5989FC, 0x5989FC, 0x5986D0, 0x5986D0);
		static const u32 PickupDropID = Region::AutoRegion(0x598D9C, 0x5982B4, 0x597DE4, 0x597DE4, 0x5976D4, 0x5976D4, 0x5973A8, 0x5973A8);

		static const u32 PickupAnimID = Region::AutoRegion(0x65D410, 0x65C938, 0x65C448, 0x65C448, 0x65BF08, 0x65BF08, 0x65BAB0, 0x65BAB0);

		static const u32 PickupInvFull = Region::AutoRegion(0x599FA8, 0x5994C0, 0x598FF0, 0x598FF0, 0x5988E0, 0x5988E0, 0x5985B4, 0x5985B4);
		static const u32 pickmode3 = Region::AutoRegion(0x599FD0, 0x5994E8, 0x599018, 0x599018, 0x598908, 0x598908, 0x5985DC, 0x5985DC);
		static const u32 pickmode4 = Region::AutoRegion(0x59A1C0, 0x5996D8, 0x599208, 0x599208, 0x598AF8, 0x598AF8, 0x5987CC, 0x5987CC);
		static const u32 pickmode5 = Region::AutoRegion(0x59A24C, 0x599764, 0x599294, 0x599294, 0x598B84, 0x598B84, 0x598858, 0x598858);

		static const u32 pickmode6 = Region::AutoRegion(0x661894, 0x660DBC, 0x6608CC, 0x6608CC, 0x66038C, 0x66038C, 0x65FF34, 0x65FF34);
		static const u32 pickmode7 = Region::AutoRegion(0x661898, 0x660DC0, 0x6608D0, 0x6608D0, 0x660390, 0x660390, 0x65FF38, 0x65FF38);
		static const u32 pickmode8 = Region::AutoRegion(0x66189C, 0x660DC4, 0x6608D4, 0x6608D4, 0x660394, 0x660394, 0x65FF3C, 0x65FF3C);
		static const u32 pickmode9 = Region::AutoRegion(0x6618C0, 0x660DE8, 0x6608F8, 0x6608F8, 0x6603B8, 0x6603B8, 0x65FF60, 0x65FF60);

		const u32 PickMode[10] = { PickupDropID, PickupInvFull, pickmode3, pickmode4, pickmode5, pickmode6, pickmode7, pickmode8, pickmode9 };
		
		static const u32 ModePatch[6][9] = {
			{ 0xE3A00001, 0xEA000008, 0xE3A00001, 0xE3A04001, 0xE3A00001, 0xE5840000, 0xE5C48004, 0xE5CA9013, 0xE3A0103D },
			{ 0xE3A00003, 0xEA000008, 0xE3A00003, 0xE3A04003, 0xE3A00003, 0xE5840000, 0xE5C48004, 0xE5CA9013, 0xE3A01040 },
			{ 0xE3A0000A, 0xEA000008, 0xE3A0000A, 0xE3A0400A, 0xE3A0000A, 0xE5840002, 0xE5C48000, 0xE5CA900F, 0xE3A01061 },
			{ 0xE3A00012, 0xEA000008, 0xE3A00012, 0xE3A04012, 0xE3A00012, 0xE5840002, 0xE5C48000, 0xE5CA900F, 0xE3A0105A },
			{ 0xE3A00013, 0xEA000008, 0xE3A00013, 0xE3A04013, 0xE3A00013, 0xE5840002, 0xE5C48000, 0xE5CA900F, 0xE3A0104F },
			{ 0xE3A00007, 0x0A000008, 0xE1A00004, 0xE3A04001, 0xE1A00004, 0xE5840000, 0xE5C48004, 0xE5CA9013, 0xE3A0103C },
		};

		static const u32 PickupAuto1 = Region::AutoRegion(0x67CCB8, 0x67C1E0, 0x67BCF0, 0x67BCF0, 0x67B7B0, 0x67B7B0, 0x67B358, 0x67B358);
		static const u32 PickupAuto2 = Region::AutoRegion(0x59A0D0, 0x5995E8, 0x599118, 0x599118, 0x598A08, 0x598A08, 0x5986DC, 0x5986DC);
		static const u32 PickupAuto3 = Region::AutoRegion(0x59A1BC, 0x5996D4, 0x599204, 0x599204, 0x598AF4, 0x598AF4, 0x5987C8, 0x5987C8);

		static const u32 AutoPick[3] = { PickupAuto1, PickupAuto2, PickupAuto3 };

		static const u32 AutoPatch[2][3] = {
            { 0xE1A00000, 0xE1A00000, 0xE1A00000 },
            { 0x0A000039, 0x1A000041, 0x0A000006 }
        };

		static const u32 snakeFunc = Region::AutoRegion(0x68E5F4, 0x68DB1C, 0x68D62C, 0x68D62C, 0x68D0EC, 0x68D0EC, 0x68CC94, 0x68CC94);
		static Hook speedHook, pickItemHook, closeholeItemHook;

		if(entry->WasJustActivated()) {
			Process::Patch(pickmode5, 0xE3A00001);

			Process::Patch(AlwaysPickup, 0xEA00003D);
			Process::Patch(PickupAnimID, 0xE3A01006);

			speedHook.Initialize(snakeFunc + 0x20, (u32)PATCH_SnakeSpeed);
			pickItemHook.Initialize((Code::PickupItem - 0x3C), (u32)PATCH_PickupSeed);
			closeholeItemHook.Initialize((Code::CloseHoleItem - 0x40), (u32)PATCH_PickupSeed);

			pickItemHook.SetFlags(USE_LR_TO_RETURN);
			closeholeItemHook.SetFlags(USE_LR_TO_RETURN);

			pickItemHook.Enable();
			closeholeItemHook.Enable();
		}
		
		if(entry->Hotkeys[0].IsPressed()) {
			Sleep(Milliseconds(100));
			Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, PickupSeederItemID, PickupSeederItemID, ItemChange);
        }	
		
	//Switches Seed Item to Remove Item
		if(Controller::IsKeysPressed(entry->Hotkeys[1].GetKeys())) {
			if(!pickItemHook.IsEnabled()) {
				pickItemHook.Enable();
				closeholeItemHook.Enable();
				OSD::Notify(Color::Magenta << "Seed Item" << Color::Green << " Enabled"); //Enabled Seeding
			}
			else {
				pickItemHook.Disable();
				closeholeItemHook.Disable();
				OSD::Notify(Color::Turquoise << "Remove Item" << Color::Green << " Enabled"); //Disabled Seeding
			}
        }
		
	//Pickup Speed Part
		if(Controller::IsKeysPressed(entry->Hotkeys[2].GetKeys())) {
			if(!speedHook.IsEnabled()) {
				Process::Patch(HoleSpeedPatch, 0xEA000015);
				speedHook.Enable();	
			}
			else {
				Process::Patch(HoleSpeedPatch, 0x0A000015);
				speedHook.Disable();
			}
				
			OSD::Notify("Seed Speed" << (speedHook.IsEnabled() ? Color::Green << " Enabled" : Color::Red << " Disabled"));
		}
		
	//Switches Modes of Pickup
		if(Controller::IsKeysPressed(entry->Hotkeys[3].GetKeys())) {
			static int Mode = 0, Index = 0;
			
			switch(Mode) {
				default: break;
				case 0:
					Mode++;
					Index = 0;
					OSD::Notify("Seed Type: Pickup", Color::Red);
				break;
				case 1:
					Mode++;
					Index = 1;
					OSD::Notify("Seed Type: Pluck", Color::Orange);
				break;
				case 2:
					Mode++;
					Index = 2;
					OSD::Notify("Seed Type: FlipDrop", Color::Green);
				break;
				case 3:
					Mode++;
					Index = 3;
					OSD::Notify("Seed Type: RockBreak", Color::Blue);
				break;
				case 4:
					Mode = 0;
					Index = 4;
					OSD::Notify("Seed Type: Dig Hole", Color::Purple);
				break;
			}
			
			for(int i = 0; i < 9; ++i)
                Process::Patch(PickMode[i], ModePatch[Index][i]);
		}	
		
	//toggles Auto Pickup
		if(Controller::IsKeysPressed(entry->Hotkeys[4].GetKeys()))	{
			bool IsON = *(u32 *)PickupAuto1 == 0x0A000039 ? 0 : 1;
			
			for(int i = 0; i < 3; ++i)
                Process::Patch(AutoPick[i], AutoPatch[IsON][i]);

            OSD::Notify("Auto-Pickup: " << (IsON ? Color::Red << "OFF" : Color::Green << "ON"));
		}	
	
		if(!entry->IsActivated()) {
		//Disables Speed Patch
			Process::Patch(HoleSpeedPatch, 0x0A000015);
			speedHook.Disable();

		//Disable Pick Mode	
			for(int i = 0; i < 9; ++i) 
                Process::Patch(PickMode[i], ModePatch[5][i]);

		//Disables AutoPick
			for(int i = 0; i < 3; ++i)
                Process::Patch(AutoPick[i], AutoPatch[1][i]);

		//Disable Always Pickup	
			Process::Patch(AlwaysPickup, 0x0A000044);
			Process::Patch(PickupAnimID, 0xE3A01054);

		//Disables Item Patch
            pickItemHook.Disable();
			closeholeItemHook.Disable();
        }
    }
//Walking Seeder
	void Walkseeder(MenuEntry *entry) {
		static bool set = false;
		if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {	
			set = !set;
			OSD::Notify("Delete Items " << (set ? Color::Green << "ON" : Color::Red << "OFF"));
			Process::Patch(Code::UnbreakableFlower, set ? 0x1A000015 : Code::v_UnbreakableFlower);
		}	

		if(!entry->IsActivated()) 
            Process::Patch(Code::UnbreakableFlower, Code::v_UnbreakableFlower);
    }
//Firework Seeder
	void fireworkentry(MenuEntry *entry) {
		static const u32 firework = Region::AutoRegion(0x597870, 0x596D88, 0x5968B8, 0x5968B8, 0x5961A8, 0x5961A8, 0x595E7C, 0x595E7C);
		
		const std::vector<std::string> fireOpt = {
			Language->Get("VECTOR_FIREWORK_CHANGE_ID"),
			Language->Get("VECTOR_FIREWORK_SPAWN"),
			Language->Get("VECTOR_DISABLE")
		};
		
		u16 input = 0;
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(fireOpt);
		switch(optKb.Open()) {
			default: break;
			case 0: {
				if(Wrap::KB<u16>(Language->Get("FIREWORK_SEEDER_ENTER_ID"), true, 4, input, input, ItemChange)) 
					Process::Patch(firework, input);
			} break;
			case 1: Inventory::WriteSlot(0, 0x339F); break;
			case 2: Process::Patch(firework, 0x33A0); break;
		}
	}
//OSD for Map Editor	
	bool editorID(const Screen &screen) { 
		if(screen.IsTop) 
			screen.Draw(Utils::Format("ID: %08X", dropitem), 320, 220, Color::White); 

		return 1;
	}

	static u8 size;

	void ParticleCallBack(void) {
		float *coords = PlayerClass::GetInstance()->GetCoordinates();
		if(coords == nullptr)
			return;

		float particleCoords[3]{ 0, coords[1], 0 };
		for(s8 i = -size; i <= size; ++i) {
			for(s8 j = -size; j <= size; ++j) {
				particleCoords[0] = (selectedX + j) * 0x20 + 0x10;
				particleCoords[2] = (selectedY + i) * 0x20 + 0x10;
				GameHelper::Particles(0x20C, particleCoords);
				//0x20C = Orange Light
				//0x214 = Blue Light
			}
		}
	}

//Map Editor
	void tileSelector(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded())  {
			if(MapEditorActive)
				goto OFF;

			return; //if player is not loaded return
		}
		
		static u32 keyPressedTicks = 0, DPadKeyPressedTicks = 0;
		static bool removal = false;
			
		if(!MapEditorActive) //If Map Editor is OFF get Coords
			PlayerClass::GetInstance()->GetWorldCoords(&selectedX, &selectedY);
		
		if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			if(!MapEditorActive) { //Map Editor ON
				PluginMenu *menu = PluginMenu::GetRunningInstance();
				*menu += ParticleCallBack;

				fovbool = true;
				
				if(!save) //If No Save is OFF switch it ON
					Process::Patch(Code::nosave, 0xE1A00000);
				
				OSD::Run(editorID);	
				
				Process::Patch(Code::unlockcamera, 0xE8BD81F0);
				OSD::Notify("Map Editor " << Color::Green << "ON");
				MapEditorActive = true;

				*(float *)(Camera::GetInstance() + 4) = (float)(selectedX * 0x20 + 0x10);
				Camera::AddToY(90.0f);
				*(float *)(Camera::GetInstance() + 0xC) = (float)(selectedY * 0x20 + 0x70);
				Camera::AddToYRotation(0x700);
			}
			
			else { //Map Editor OFF
			OFF:
				PluginMenu *menu = PluginMenu::GetRunningInstance();
				*menu -= ParticleCallBack;

				fovbool = false;
				
				if(!save) //If No Save is OFF switch it back OFF
					Process::Patch(Code::nosave, 0xE8900006);
				
				OSD::Stop(editorID);
				
				Process::Patch(Code::unlockcamera, 0xE2805C01);
				OSD::Notify("Map Editor " << Color::Red << "OFF");
				MapEditorActive = false;		
			}
		}
		
		if(MapEditorActive) {	
			if(Controller::IsKeysDown(entry->Hotkeys[1].GetKeys()) || Controller::IsKeysPressed(entry->Hotkeys[1].GetKeys())) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedX += 1;
					Camera::AddToX(32.0f);
				}
			}
			
			if(Controller::IsKeysDown(entry->Hotkeys[2].GetKeys()) || Controller::IsKeysPressed(entry->Hotkeys[2].GetKeys())) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedX -= 1;
					Camera::AddToX(-32.0f);
				}
			}
			
			if(Controller::IsKeysDown(entry->Hotkeys[3].GetKeys()) || Controller::IsKeysPressed(entry->Hotkeys[3].GetKeys())) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedY += 1;
					Camera::AddToZ(32.0f);
				}
			}
			
			if(Controller::IsKeysDown(entry->Hotkeys[4].GetKeys()) || Controller::IsKeysPressed(entry->Hotkeys[4].GetKeys())) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedY -= 1;
					Camera::AddToZ(-32.0f);
				}
			}
			
			if(Controller::IsKeysReleased(entry->Hotkeys[5].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[6].GetKeys())) 
				keyPressedTicks = 0;
			
			if(Controller::IsKeysReleased(entry->Hotkeys[1].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[2].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[3].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[4].GetKeys())) 
				DPadKeyPressedTicks = 0;
			
			if(Controller::IsKeysDown(entry->Hotkeys[5].GetKeys())) {
				keyPressedTicks++;
				if((keyPressedTicks < 90 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 220)
					dropitem = (dropitem - 1 == 0x1FFF ? 0xFD : dropitem - 1) % 0x4000;
			}
			
			if(Controller::IsKeysDown(entry->Hotkeys[6].GetKeys())) {
				keyPressedTicks++;
				if((keyPressedTicks < 90 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 220) 
					dropitem = (dropitem + 1 == 0xFE ? 0x2000 : dropitem + 1) % 0x4000;
			}
			
			if(Controller::IsKeysPressed(entry->Hotkeys[7].GetKeys())) {
				size++;
				if(size >= 4) 
					size = 0;
				
				OSD::Notify("Size set to " << std::to_string(size));
			}
			
			if(Controller::IsKeysPressed(entry->Hotkeys[8].GetKeys())) {
				removal = !removal;

				OSD::Notify("Removal mode " << (removal ? Color::Green << "ON" : Color::Red << "OFF")); 
			}
			
			if(turbo ? Controller::IsKeysDown(entry->Hotkeys[9].GetKeys()) : Controller::IsKeysPressed(entry->Hotkeys[9].GetKeys())) {//Key::A
				u32 pItem = (u32)GameHelper::GetItemAtWorldCoords(selectedX, selectedY);
				
				if(pItem == 0) 
					return;
				
				for(s8 i = -size; i <= size; ++i) {
					for(s8 j = -size; j <= size; ++j) {
						if(!removal) 
							Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitem, &dropitem, (selectedX + j), (selectedY + i), 0, 0, 0, 0, 0, waitAnim, 0xA5);
						else 
							GameHelper::TrampleAt((selectedX + j), (selectedY + i)); 
					}
				}
			}
			
			if(Controller::IsKeysPressed(entry->Hotkeys[10].GetKeys())) 
				Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, dropitem, dropitem, ItemChange);
		}
		
		if(!entry->IsActivated()) { 
			fovbool = false;
			OSD::Stop(editorID);
			Process::Patch(Code::unlockcamera, 0xE2805C01);
				
			if(!save) //If No Save is OFF switch it back OFF
				Process::Patch(Code::nosave, 0xE8900006);
		}
	}

	void E_Seeder_KB(MenuEntry *entry) {
		Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, EverythingSeederItemID, EverythingSeederItemID, ItemChange);
	}

	void everythingseeder(MenuEntry *entry) {
		static bool active = false;
		static Hook hook1, hook2;

		if(entry->WasJustActivated()) {
			hook1.Initialize(Code::PlaceItemOffset + 0x58, (u32)PATCH_EverythingSeed);
			hook2.Initialize(Code::PlaceItemOffset + 0x64, (u32)PATCH_EverythingSeed);
		}
		
		if(entry->Hotkeys[0].IsDown() && !active) {
			if(!IDList::ItemValid(EverythingSeederItemID))
				EverythingSeederItemID = 0x2018;

			if((EverythingSeederItemID & 0xFFFF) == 0x7FFE)
				EverythingSeederItemID = 0x80007FFE;

			hook1.Enable();
			hook2.Enable();
			active = true; //Patch is written
		}
		
		if(!entry->Hotkeys[0].IsDown() && active) {
			hook1.Disable();
			hook2.Disable();
			active = false; //Patch is not written
		}

		if(!entry->IsActivated()) {
			hook1.Disable();
			hook2.Disable();
		}
	}
}
