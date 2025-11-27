#include "cheats.hpp"

#include "Helpers/Wrapper.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/GameStructs.hpp"
#include "RuntimeContext.hpp"

extern "C" void PATCH_EverythingSeed(void);
extern "C" void PATCH_SnakeSpeed(void);
extern "C" void PATCH_PickupSeed(void);

extern "C" bool __IsPickSnakeID(u16 snakeID) {
	static const u16 snakeIDArr[19] = { 0x01, 0x25, 0x28, 0x3E, 0x3F, 0x47, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4F, 0x5F, 0x60, 0xA6, 0xA7, 0xD0, 0xDA, 0xDB };
	return std::find(std::begin(snakeIDArr), std::end(snakeIDArr), snakeID) != std::end(snakeIDArr);
}

CTRPluginFramework::Item EverythingSeederItemID = {0x7FFE, 0x8000};
CTRPluginFramework::Item PickupSeederItemID = {0x7FFE, 0};

namespace CTRPluginFramework {
//Pickup Seeder
	void pickseeder(MenuEntry *entry) {
		static Address HoleSpeedPatch(0x65256C);
		static Address AlwaysPickup(0x59A0C4);
		static Address PickupDropID(0x598D9C);

		static Address PickupAnimID(0x65D410);

		static Address PickupInvFull(0x599FA8);
		static Address pickmode3(0x599FD0);
		static Address pickmode4(0x59A1C0);
		static Address pickmode5(0x59A24C);

		static Address pickmode6(0x661894);
		static Address pickmode7(0x661898);
		static Address pickmode8(0x66189C);
		static Address pickmode9(0x6618C0);

		static Address PickupAuto1(0x67CCB8);
		static Address PickupAuto2(0x59A0D0);
		static Address PickupAuto3(0x59A1BC);

		static Address snakeFunc(0x68E5F4);

		static Address pickItem = pickmode5.MoveOffset(-0x20);
		static Address closeholeItem = PickupDropID.MoveOffset(-0x40);

		static Address PickMode[9] = {
			PickupDropID, PickupInvFull, pickmode3, pickmode4,
			pickmode5, pickmode6, pickmode7, pickmode8, pickmode9
		};
		
		static Address AutoPick[3] = { 
			PickupAuto1, PickupAuto2, PickupAuto3 
		};

		static Hook speedHook, pickItemHook, closeholeItemHook;

		static const u32 ModePatch[6][9] = {
			{ 0xE3A00001, 0xEA000008, 0xE3A00001, 0xE3A04001, 0xE3A00001, 0xE5840000, 0xE5C48004, 0xE5CA9013, 0xE3A0103D },
			{ 0xE3A00003, 0xEA000008, 0xE3A00003, 0xE3A04003, 0xE3A00003, 0xE5840000, 0xE5C48004, 0xE5CA9013, 0xE3A01040 },
			{ 0xE3A0000A, 0xEA000008, 0xE3A0000A, 0xE3A0400A, 0xE3A0000A, 0xE5840002, 0xE5C48000, 0xE5CA900F, 0xE3A01061 },
			{ 0xE3A00012, 0xEA000008, 0xE3A00012, 0xE3A04012, 0xE3A00012, 0xE5840002, 0xE5C48000, 0xE5CA900F, 0xE3A0105A },
			{ 0xE3A00013, 0xEA000008, 0xE3A00013, 0xE3A04013, 0xE3A00013, 0xE5840002, 0xE5C48000, 0xE5CA900F, 0xE3A0104F },
			{ 0xE3A00007, 0x0A000008, 0xE1A00004, 0xE3A04001, 0xE1A00004, 0xE5840000, 0xE5C48004, 0xE5CA9013, 0xE3A0103C },
		};

		static const u32 AutoPatch[2][3] = {
            { 0xE1A00000, 0xE1A00000, 0xE1A00000 },
            { 0x0A000039, 0x1A000041, 0x0A000006 }
        };

		if(entry->WasJustActivated()) {
			pickmode5.Patch(0xE3A00001);
			AlwaysPickup.Patch(0xEA00003D);
			PickupAnimID.Patch(0xE3A01006);

			speedHook.Initialize(snakeFunc.addr + 0x20, (u32)PATCH_SnakeSpeed);
			pickItemHook.Initialize(pickItem.addr, (u32)PATCH_PickupSeed);
			closeholeItemHook.Initialize(closeholeItem.addr, (u32)PATCH_PickupSeed);

			pickItemHook.SetFlags(USE_LR_TO_RETURN);
			closeholeItemHook.SetFlags(USE_LR_TO_RETURN);

			pickItemHook.Enable();
			closeholeItemHook.Enable();
		}
		
		if(entry->Hotkeys[0].IsPressed()) {
			Wrap::KB<u32>(Language::getInstance()->get("ENTER_ID"), true, 8, *(u32 *)&PickupSeederItemID, *(u32 *)&PickupSeederItemID, ItemChange);
        }	
		
	//Switches Seed Item to Remove Item
		if(entry->Hotkeys[1].IsPressed()) {
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
		if(entry->Hotkeys[2].IsPressed()) {
			if(!speedHook.IsEnabled()) {
				HoleSpeedPatch.Patch(0xEA000015);
				speedHook.Enable();	
			}
			else {
				HoleSpeedPatch.Unpatch();
				speedHook.Disable();
			}
			
			OSD::Notify("Seed Speed" << (speedHook.IsEnabled() ? Color::Green << " Enabled" : Color::Red << " Disabled"));
		}
		
	//Switches Modes of Pickup
		if(entry->Hotkeys[3].IsPressed()) {
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
			
			for(int i = 0; i < 9; ++i) {
				PickMode[i].Patch(ModePatch[Index][i]);
			}
		}	
		
	//toggles Auto Pickup
		if(entry->Hotkeys[4].IsPressed())	{
			bool IsON = *(u32 *)PickupAuto1.addr != PickupAuto1.origVal;
			
			for(int i = 0; i < 3; ++i) {
				AutoPick[i].Patch(AutoPatch[IsON][i]);
			}

            OSD::Notify("Auto-Pickup: " << (IsON ? Color::Red << "OFF" : Color::Green << "ON"));
		}	
	
		if(!entry->IsActivated()) {
		//Disables Speed Patch
			HoleSpeedPatch.Unpatch();
			speedHook.Disable();

		//Disable Pick Mode	
			for(int i = 0; i < 9; ++i) {
				PickMode[i].Unpatch();
			}

		//Disables AutoPick
			for(int i = 0; i < 3; ++i) {
				AutoPick[i].Unpatch();
			}

		//Disable Always Pickup	
			AlwaysPickup.Unpatch();
			PickupAnimID.Unpatch();

		//Disables Item Patch
            pickItemHook.Disable();
			closeholeItemHook.Disable();
        }
    }
//Walking Seeder
	void Walkseeder(MenuEntry *entry) {
		static Address walkseed(0x597F64);
		static bool set = false;

		if(entry->Hotkeys[0].IsPressed()) {	
			if (!set) {
				walkseed.Patch(0xEA000014);
				OSD::Notify("Delete Items " << Color::Green << "ON");
				set = true;
			} 
			else {
				walkseed.Unpatch();
				OSD::Notify("Delete Items " << Color::Red << "OFF");
				set = false;
			}
		}	

		if(!entry->IsActivated()) {
			walkseed.Unpatch();
			set = false;
		}
    }
//Firework Seeder
	void fireworkentry(MenuEntry *entry) {
		static Address firework(0x597870);
		
		const std::vector<std::string> fireOpt = {
			Language::getInstance()->get("VECTOR_FIREWORK_CHANGE_ID"),
			Language::getInstance()->get("VECTOR_FIREWORK_SPAWN"),
			Language::getInstance()->get("VECTOR_DISABLE")
		};
		
		Item input = {0x7FFE, 0};
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), fireOpt);
		
		switch(optKb.Open()) {
			default: break;
			case 0: {
				if(Wrap::KB<u16>(Language::getInstance()->get("FIREWORK_SEEDER_ENTER_ID"), true, 4, *(u16 *)&input, *(u16 *)&input, ItemChange)) {
					firework.Patch(*(u16 *)&input);
				}
			} break;
			case 1: 
				Inventory::WriteSlot(0, {0x339F, 0}); 
			break;
			case 2: 
				firework.Unpatch();
			break;
		}
	}
//OSD for Map Editor
	bool editorID(const Screen &screen) { 
		if(screen.IsTop) {
			screen.Draw(Utils::Format("ID: %08X", dropitem), 320, 220, Color::White); 
		}

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
				Game::Particles(0x20C, particleCoords);
				//0x20C = Orange Light
				//0x214 = Blue Light
			}
		}
	}

//Map Editor
	void tileSelector(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded())  {
			if(MapEditorActive) {
				goto OFF;
			}

			return; //if player is not loaded return
		}
		
		static u32 keyPressedTicks = 0, DPadKeyPressedTicks = 0;
		static bool removal = false;
			
		if(!MapEditorActive) {//If Map Editor is OFF get Coords
			PlayerClass::GetInstance()->GetWorldCoords(&selectedX, &selectedY);
		}
		
		if(entry->Hotkeys[0].IsPressed()) {
			if(!MapEditorActive) { //Map Editor ON
				PluginMenu *menu = PluginMenu::GetRunningInstance();
				*menu += ParticleCallBack;

				RuntimeContext::getInstance()->setFov(true);
				
				if(!RuntimeContext::getInstance()->isSaveMenuDisabled()) {//If No Save is OFF switch it ON
					Process::Patch(Address(0x1A0980).addr, 0xE1A00000);
				}
				
				OSD::Run(editorID);	
				
				Process::Patch(Address(0x1A51C8).addr, 0xE8BD81F0);
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

				RuntimeContext::getInstance()->setFov(false);
				
				if(!RuntimeContext::getInstance()->isSaveMenuDisabled()) {//If No Save is OFF switch it back OFF
					Process::Patch(Address(0x1A0980).addr, 0xE8900006);
				}
				
				OSD::Stop(editorID);
				
				Process::Patch(Address(0x1A51C8).addr, 0xE2805C01);
				OSD::Notify("Map Editor " << Color::Red << "OFF");
				MapEditorActive = false;		
			}
		}
		
		if(MapEditorActive) {	
			if(entry->Hotkeys[1].IsDown() || entry->Hotkeys[1].IsPressed()) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedX += 1;
					Camera::AddToX(32.0f);
				}
			}
			
			if(entry->Hotkeys[2].IsDown() || entry->Hotkeys[2].IsPressed()) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedX -= 1;
					Camera::AddToX(-32.0f);
				}
			}
			
			if(entry->Hotkeys[3].IsDown() || entry->Hotkeys[3].IsPressed()) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedY += 1;
					Camera::AddToZ(32.0f);
				}
			}
			
			if(entry->Hotkeys[4].IsDown() || entry->Hotkeys[4].IsPressed()) {
				DPadKeyPressedTicks++;
				if((DPadKeyPressedTicks < 50 ? (DPadKeyPressedTicks % 8) == 1 : (DPadKeyPressedTicks % 3) == 1) || DPadKeyPressedTicks > 100) {
					selectedY -= 1;
					Camera::AddToZ(-32.0f);
				}
			}
			
			if(Controller::IsKeysReleased(entry->Hotkeys[5].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[6].GetKeys())) {
				keyPressedTicks = 0;
			}
			
			if(Controller::IsKeysReleased(entry->Hotkeys[1].GetKeys()) || 
				Controller::IsKeysReleased(entry->Hotkeys[2].GetKeys()) || 
					Controller::IsKeysReleased(entry->Hotkeys[3].GetKeys()) || 
						Controller::IsKeysReleased(entry->Hotkeys[4].GetKeys())) {
				DPadKeyPressedTicks = 0;
			}
				
			if(entry->Hotkeys[5].IsDown()) {
				keyPressedTicks++;
				if((keyPressedTicks < 90 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 220) {
					dropitem.ID = (dropitem.ID - 1 == 0x1FFF ? 0xFD : dropitem.ID - 1) % 0x4000;
				}
			}
			
			if(entry->Hotkeys[6].IsDown()) {
				keyPressedTicks++;
				if((keyPressedTicks < 90 ? (keyPressedTicks % 10) == 1 : (keyPressedTicks % 3) == 1) || keyPressedTicks > 220) {
					dropitem.ID = (dropitem.ID + 1 == 0xFE ? 0x2000 : dropitem.ID + 1) % 0x4000;
				}
			}
			
			if(entry->Hotkeys[7].IsPressed()) {
				size++;
				if(size >= 4) {
					size = 0;
				}
				
				OSD::Notify(Utils::Format("Size set to %d", size));
			}
			
			if(entry->Hotkeys[8].IsPressed()) {
				removal = !removal;

				OSD::Notify("Removal mode " << (removal ? Color::Green << "ON" : Color::Red << "OFF")); 
			}
			
			if(RuntimeContext::getInstance()->isTurbo() ? entry->Hotkeys[9].IsDown() : entry->Hotkeys[9].IsPressed()) {//Key::A
				Item *pItem = Game::GetItemAtWorldCoords(selectedX, selectedY);
				
				if(!pItem) {
					return;
				}
				
				for(s8 i = -size; i <= size; ++i) {
					for(s8 j = -size; j <= size; ++j) {
						if(!removal) {
							Dropper::PlaceItemWrapper(DropType, ItemIDToReplace, &dropitem, &dropitem, (selectedX + j), (selectedY + i), 0, 0, 0, 0, 0, waitAnim, 0xA5);
						}
						else {
							Game::TrampleAt((selectedX + j), (selectedY + i)); 
						}
					}
				}
			}
			
			if(entry->Hotkeys[10].IsPressed()) {
				Wrap::KB<u32>(Language::getInstance()->get("ENTER_ID"), true, 8, *(u32 *)&dropitem, *(u32 *)&dropitem, ItemChange);
			}
		}
		
		if(!entry->IsActivated()) { 
			RuntimeContext::getInstance()->setFov(false);
			OSD::Stop(editorID);
			Process::Patch(Address(0x1A51C8).addr, 0xE2805C01);
				
			if(!RuntimeContext::getInstance()->isSaveMenuDisabled()) {//If No Save is OFF switch it back OFF
				Process::Patch(Address(0x1A0980).addr, 0xE8900006);
			}
		}
	}

	void E_Seeder_KB(MenuEntry *entry) {
		Wrap::KB<u32>(Language::getInstance()->get("ENTER_ID"), true, 8, *(u32 *)&EverythingSeederItemID, *(u32 *)&EverythingSeederItemID, ItemChange);
	}

	void everythingseeder(MenuEntry *entry) {
		static bool active = false;
		static Hook hook1, hook2;

		if(entry->WasJustActivated()) {
			hook1.Initialize(Address(0x59FC7C).addr + 0x58, (u32)PATCH_EverythingSeed);
			hook2.Initialize(Address(0x59FC7C).addr + 0x64, (u32)PATCH_EverythingSeed);
		}
		
		if(entry->Hotkeys[0].IsDown() && !active) {
			if(!EverythingSeederItemID.isValid()) {
				EverythingSeederItemID = {0x2018, 0};
			}

			if(EverythingSeederItemID.ID == 0x7FFE) {
				EverythingSeederItemID = {0x7FFE, 0x8000};
			}

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
