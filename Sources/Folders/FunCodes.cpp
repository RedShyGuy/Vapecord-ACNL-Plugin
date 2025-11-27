#include "cheats.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Game.hpp"

#include "Helpers/Wrapper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/IDList.hpp"
#include "Color.h"
#include "Helpers/GameStructs.hpp"
#include "RuntimeContext.hpp"

extern "C" void PATCH_PartyPop(void);
extern "C" void CHECK_Individual(void);

CTRPluginFramework::Item PartyPopperTool = {0x336A, 0};

u32 c_eyeID = 0;
u32 c_mouthID = 0;

namespace CTRPluginFramework {
//Size Codes
	void sizecodes(MenuEntry *entry) {
		static Address player(0x1ACE00);
		static Address bug(0x1F557C);
		static Address npc(0x2042BC);
		static Address effect(0x550A80);
		static Address shadow(0x28F3A4);
		static Address town(0x52E9D0);
		static Address horplayer(0x5680F8);
		static Address vertplayer(0x567FF4);
		static Address head(0x568064);
		static Address corrupt(0x47E3F0);
		
		static const std::vector<std::string> sizeopt = {
			Language::getInstance()->get("VECTOR_SIZE_PLAYER"),
			Language::getInstance()->get("VECTOR_SIZE_BUGFISH"),
			Language::getInstance()->get("VECTOR_SIZE_NPC"),
			Language::getInstance()->get("VECTOR_SIZE_EFFECT"),
			Language::getInstance()->get("VECTOR_SIZE_SHADOW"),
			Language::getInstance()->get("VECTOR_SIZE_TOWN"),
			Language::getInstance()->get("VECTOR_SIZE_HORI"),
			Language::getInstance()->get("VECTOR_SIZE_VERT"),
			Language::getInstance()->get("VECTOR_SIZE_HEAD"),
			Language::getInstance()->get("VECTOR_SIZE_CORRUPT"),
			Language::getInstance()->get("VECTOR_SIZE_RESET")
		};
		
		std::vector<std::string> sizesopt = {
			Language::getInstance()->get("VECTOR_SIZE_BIGGER"),
			Language::getInstance()->get("VECTOR_SIZE_DEFAULT"),
			Language::getInstance()->get("VECTOR_SIZE_SMALLER"),
			"Custom"
		};
		
		static Address sizeAddresses[10] = { 
			player, bug, npc, effect, shadow, 
			town, horplayer, vertplayer, head, corrupt 
		};
		
		static constexpr float sizes[3] = { 2.0, 1.0, 0.5 };

		bool IsON;

		float size = 0.0;
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), sizeopt);

		int op = optKb.Open();
		if(op < 0) {
			return;
		}
			
		else if(op <= 9) {
			for(int i = 0; i < 3; ++i) {
				IsON = *(float *)sizeAddresses[op].addr == sizes[i];
				sizesopt[i] = IsON ? (Color(pGreen) << sizesopt[i]) : (Color(pRed) << sizesopt[i]);	
			}
			
			optKb.Populate(sizesopt);
			int op2 = optKb.Open();
			if(op2 < 0) {
				return;
			}
			else if(op2 == 3) {
				if(optKb.Open(size, size) >= 0) {
					sizeAddresses[op].WriteFloat(size);
				}
			}
			else {
				sizeAddresses[op].WriteFloat(sizes[op2]);
			}

			sizecodes(entry);
			return;
		}
		else if(op == 10) {
			for(int i = 0; i < 10; i++) {
				sizeAddresses[i].Unpatch();
			}
		}
    }
//T-Pose
	void tposeentry(MenuEntry *entry) { 
		static Address tpose(0x73C290);

		if(entry->WasJustActivated()) {
			tpose.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			tpose.Unpatch();
		}
	}

//OSD For Take TPC Picture
	bool tpcoverlay(const Screen &screen) {
		if(screen.IsTop) {
			screen.DrawRect(154, 50, 90, 6, Color::White); //top bar
			screen.DrawRect(154, 50, 6, 130, Color::White); //left bar
			screen.DrawRect(154, 177, 90, 6, Color::White); //bottom bar
			screen.DrawRect(240, 50, 6, 133, Color::White); //right bar
		}
		return 1;
	}

//Take TPC Picture
	void freezeframe(MenuEntry *entry) {
		static Address freeze(0x54DBE8);

		if(entry->Hotkeys[0].IsPressed()) {
			ACNL_Player *player = Player::GetSaveData();
			if(!player) {
				OSD::Notify("Player needs to be loaded!", Color::Red);
				return;
			}

			static Address CreateTPC(0x5B3594);
			static const Address TPCPoint(0x954F10);

			CreateTPC.Call<void>(*(u32 *)TPCPoint.addr, &player->HasTPCPic);
			Game::PlaySound(0x4A7);

			player->PlayerFlags.HasTPCPicture = true;
		}
	
		if(entry->Hotkeys[1].IsDown()) {
			if(entry->Hotkeys[1].IsPressed()) {
				freeze.Patch(0xE3A000FF);
			}

			OSD::Run(tpcoverlay);
		}
		
		if(!entry->Hotkeys[1].IsDown()) {
			OSD::Stop(tpcoverlay);
			freeze.Unpatch();
		}

		if(!entry->IsActivated()) {
			freeze.Unpatch();
		}
    }

//Max Turbo Presses
	void maxturbo(MenuEntry *entry) { 
		u32 maxT = *(u32 *)Address(0x95D3FC).addr - 0x31C;

        Sleep(Seconds(0.0085F));
        Process::Write8(maxT, 0); //abxy
        Process::Write8(maxT + 2, 0); //dpad
    }

//Multi-presses
	void asmpresses(MenuEntry *entry) { 
		static Address press(0x5C5BEC);

		if(entry->WasJustActivated()) {
			press.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			press.Unpatch();
		}
	}
//Ultimate Party Popper	
	void partypopper(MenuEntry *entry) {
		static Address PartySnakeSpeed(0x67F008);
		static Address party2(0x662D9C);
		static Address party3(0x67BBC8);
		static Address PartyItemID(0x671874);
		static Address PartyEffect(0x671880);
		
		static u16 PartyEffectID = 0x20A;

		Process::Write8(*(u32 *)Address(0x95D3FC).addr - 0x31C, 0); //Multi Presses

		static Hook hook1, hook2;

		if(entry->WasJustActivated()) {
			hook1.Initialize(PartyItemID.addr - 0x1E0, (u32)PATCH_PartyPop);
			hook2.Initialize(PartyItemID.addr - 0x1B4, (u32)PATCH_PartyPop);

			hook1.SetFlags(USE_LR_TO_RETURN);
			hook2.SetFlags(USE_LR_TO_RETURN);
		}

		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return;
		}

		if(entry->Hotkeys[0].IsPressed()) {
			Wrap::KB<u16>(Language::getInstance()->get("ULTIMATE_PARTY_POPPER_ENTER_EFFECT"), true, 3, PartyEffectID, PartyEffectID);
		}

		if(player->HeldItem.ID == 0x336A) {
			player->HeldItem.ID = 0x336A;
			PartyEffect.Patch(PartyEffectID);

			PartySnakeSpeed.WriteFloat(8.0);
			party2.WriteFloat(2.0);
			party3.WriteFloat(2.0);

			hook1.Enable();
			hook2.Enable();
		}
   
		if(!entry->IsActivated() || player->HeldItem.ID != 0x336A) {
			PartyEffect.Unpatch();
			PartySnakeSpeed.Unpatch();
			party2.Unpatch();
			party3.Unpatch();

			hook1.Disable();
			hook2.Disable();
		}
	}
	
	void cameramod(MenuEntry *entry) {
    //pointers & addresses
		static Address cameraAsm(0x764504);
        static Address rotationAsm(0x1A3230);
		static Address rotationAsm2 = rotationAsm.MoveOffset(0xC);
        static Address camerapan(0x1A2058);

    //variables
        static bool isPatched = false;
		static bool isOn = false;

		static const u16 OrigVal[2] = { *(u16 *)(Camera::GetInstance() + 0x12C), *(u16 *)(Camera::GetInstance() + 0x12E) };
		
		static Clock time;
        Time delta = time.Restart();

        float speed = 400.0f * delta.AsSeconds();
        u16 difference = 0x1000 * delta.AsSeconds();

		float *coord = Camera::GetCoordinates();

		if(entry->WasJustActivated()) {
			camerapan.Patch(0xE3A00000); //disables camera panning
		}
	
        if(Camera::GetInstance() != 0) {
        //check if you're outside
            if(!RuntimeContext::getInstance()->isIndoors()) {
                if(Game::IsGameInRoom(1)) {
					rotationAsm.Unpatch();
					rotationAsm2.Unpatch();
                }
                else {
					rotationAsm.Patch(0xE1A00000);
					rotationAsm2.Patch(0xE1A00000);
                }
            }
            else {
				rotationAsm.Unpatch();
				rotationAsm2.Unpatch();
            }
			
            if(Controller::IsKeyDown(Key::R)) {
                if(Controller::IsKeyDown(Key::CPadUp)) {
					*(u16 *)(Camera::GetInstance() + 0x12C) += difference;
				}
                    
                if(Controller::IsKeyDown(Key::CPadDown)) {
					*(u16 *)(Camera::GetInstance() + 0x12C) -= difference;
				}
                   
                if(Controller::IsKeyDown(Key::CPadLeft)) {
					*(u16 *)(Camera::GetInstance() + 0x12E) += difference;
				}
                    
                if(Controller::IsKeyDown(Key::CPadRight)) {
					*(u16 *)(Camera::GetInstance() + 0x12E) -= difference;
				}
            }
		//Stop/follow camera from moving
            if(Controller::IsKeysPressed(Key::R + Key::X)) {
				switch(isPatched) {
					case 0: goto patch;
					case 1: goto unpatch;
				}
			}
			
		//lock/unlock player
            if(Controller::IsKeysPressed(Key::R + Key::Y)) {
				switch(isOn) {
					case 0: 
						OSD::Notify("Player: " << Color::Red << "Locked"); 
						Animation::ExecuteAnimationWrapper(4, 0xF, {0, 0}, 0, 0, 0, 0, 0, 0, 0);
						isOn = true;
					break;
					case 1: 
						OSD::Notify("Player: " << Color::Green << "Unlocked");
						Animation::ExecuteAnimationWrapper(4, 6, {0, 0}, 0, 0, 0, 0, 0, 0, 0);
						isOn = false;
					break;
				}
			}

			if(Controller::IsKeyDown(Key::B)) {
				if(!PlayerClass::GetInstance()->IsLoaded()) {
					return;
				}
					
				if(coord == nullptr) {
					return;
				}

				if(Controller::IsKeyDown(Key::DPadUp)) {
					coord[2] -= speed;
					goto patch;
				}

				if(Controller::IsKeyDown(Key::DPadRight)) {
					coord[0] += speed;
					goto patch;
				}

				if(Controller::IsKeyDown(Key::DPadDown)) {
					coord[2] += speed;
					goto patch;
				}

				if(Controller::IsKeyDown(Key::DPadLeft)) {
					coord[0] -= speed;
					goto patch;
				}

				if(Controller::IsKeyDown(Key::L)) {
					coord[1] -= speed;
					goto patch;
				}

				if(Controller::IsKeyDown(Key::R)) {
					coord[1] += speed;
					goto patch;
				}
			}
            return;
        patch:
            if(!isPatched) {
            //disable camera following
				OSD::Notify("Camera following: " << Color::Red << "OFF"); 
				cameraAsm.Patch(0xEA000020);
                isPatched = true;
            }
            return;
        unpatch:
            if(isPatched) {
			//reenable camera followig
				OSD::Notify("Camera following: " << Color::Green << "ON"); 
				cameraAsm.Unpatch();
                isPatched = false;
            }
        }

		if(!entry->IsActivated()) {
			if(coord != nullptr) {
				coord = PlayerClass::GetInstance()->GetCoordinates();
			}

			Animation::ExecuteAnimationWrapper(4, 6, {0, 0}, 0, 0, 0, 0, 0, 0, 0); //idles player
			cameraAsm.Unpatch();
			rotationAsm.Unpatch();
            rotationAsm2.Unpatch();
			camerapan.Unpatch();
			*(u16 *)(Camera::GetInstance() + 0x12C) = OrigVal[0];
			*(u16 *)(Camera::GetInstance() + 0x12E) = OrigVal[1];
			return;
		}
    }

	extern "C" void SetEyeExpression(void);
	extern "C" void SetMouthExpression(void);

	extern "C" bool isIndividual(u32 individualData) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			return false;
		}

		static Address convertData(0x27231C);
		u32 ownData = convertData.Call<u32>(PlayerClass::GetInstance()->Offset(0x1B4)); //gets actual data of player

		return (ownData == individualData);
	}

	void SetFacialExpression(MenuEntry *entry) {
		static const std::vector<std::string> options = {
			"Eye Expression", "Mouth Expression"
		};
		
		Keyboard KB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), options);
		int res = KB.Open();
		if(res < 0) {
			return;
		}

		KB.GetMessage() = Language::getInstance()->get("ENTER_ID");
		KB.IsHexadecimal(true);
		KB.Open(res == 0 ? c_eyeID : c_mouthID, res == 0 ? c_eyeID : c_mouthID);
	}

	void FacialExpressionMod(MenuEntry *entry) {
		static Hook eyeHook, mouthHook;

		static Address eyeMod(0x31DDE4);
		static Address mouthMod(0x31DF40);

		if(entry->WasJustActivated()) {
			eyeHook.Initialize(eyeMod.addr, (u32)SetEyeExpression);
			eyeHook.SetFlags(USE_LR_TO_RETURN);
			eyeHook.Enable();

			mouthHook.Initialize(mouthMod.addr, (u32)SetMouthExpression);
			mouthHook.SetFlags(USE_LR_TO_RETURN);
			mouthHook.Enable();
		}

		else if(!entry->IsActivated()) {
			eyeHook.Disable();
			mouthHook.Disable();
		}
	}

//Growing 1 Trees	
	static const u16 Growing_Trees[79] { 
		0x22, 0x23, 0x24, 0x25, //tree
		0x27, 0x28, 0x29, 0x2A, //cedar
		0x2C, 0x2D, 0x2E, 0x2F, //coconut palm tree
		0x31, 0x32, 0x33, 0x34, //banana palm tree
		0x36, 0x37, 0x38,		//bamboo
		0x3A, 0x3B, 0x3C, 0x3D, //apple tree
		0x3F, 0x40, 0x41, 0x42, //orange tree
		0x44, 0x45, 0x46, 0x47, //pear tree
		0x49, 0x4A, 0x4B, 0x4C, //peaches tree
		0x4E, 0x4F, 0x50, 0x51, //cherry tree
		0x53, 0x54, 0x55, 0x56, //durian tree
		0x58, 0x59, 0x5A, 0x5B, //lemon tree
		0x5D, 0x5E, 0x5F, 0x60, //lychee tree
		0x62, 0x63, 0x64, 0x65, //mango tree
		0x67, 0x68, 0x69, 0x6A, //persimmons tree
		0x6C, 0x6D, 0x6E, 0x6F, //money tree
		0x80, 0x81, 			//holly bush
		0x83, 0x84, 			//pink azalea bush
		0x86, 0x87, 			//white azalea bush
		0x89, 0x8A, 			//blue hydrangea bush
		0x8C, 0x8D, 			//pink hydrangea bush
		0x8F, 0x90, 			//red hibiscus bush
		0x92, 0x93, 			//yellow hibiscus bush
		0x95, 0x96				//sweet olive bush
	};
	//549
	//550
	//581/583
	
//wand abilitys
	void wandability(MenuEntry *entry) {
	//If A is not pressed return
		if(!Controller::IsKeyPressed(Key::A)) {
			return;
		}

	//If player is not loaded return
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			return;
		}

	//If player is not idling or running return
		if(*PlayerClass::GetInstance()->GetAnimation() != 6 && *PlayerClass::GetInstance()->GetAnimation() != 0xD) {
			return;
		}

		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return;
		}
		
		switch(player->HeldItem.ID) {
			default: break;
		/*If Blue Wand*/
			case 0x3398: {

			} break;
		/*If Green Wand | Restores wilted flowers instantly*/
			case 0x3399: {
				if(!bypassing) {
					Dropper::DropItemLock(true);
				}

				u32 x, y;
			//If no World Coords found | Either Player not loaded or other error
				if(!PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
					return;
				}

			//If Item is 0 return | 0 means no item-offset
				if(!Game::GetItemAtWorldCoords(x, y)) {
					return;
				}

				Item item = *Game::GetItemAtWorldCoords(x, y);

			//If ID is not wilted flower return
				if(!IDList::ValidID(item.ID, 0xCE, 0xF7)) {
					return;
				}

			//Removes Wilted Flower	
				Game::RemoveItems(true, x, y, 1, 1, false, false, false);
			//Some neat particles for a nice effect | do it 20 times to spam it	
				for(int i = 0; i <= 20; ++i) 
					Game::Particles(0x10B, PlayerClass::GetInstance()->GetCoordinates(x, y));	

				//GameHelper::PlaySound(0);

			//Places Fixed Flower
				item.ID -= 0x2F; //Jumps to fixed flower 
				Dropper::PlaceItemWrapper(0xC, ReplaceEverything, &item, &item, x, y, 0, 0, 0, 0, 0, 0x5C, 0xA5, false);

				if(!bypassing) {
					Dropper::DropItemLock(false);
				}
			} break;
		/*If Pink Wand*/
			case 0x339A: {

			} break;
		/*If Yellow Wand*/
			case 0x339B: {

			} break;
		/*If Flower Fairy Wand*/
			case 0x339C: {
				if(!bypassing) {
					Dropper::DropItemLock(true);
				}
					
				u32 x, y;
			//If no World Coords found | Either Player not loaded or other error
				if(!PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
					return;
				}

			//If Item is 0 return | 0 means no item-offset
				if(!Game::GetItemAtWorldCoords(x, y)) {
					return;
				}

				Item item = *Game::GetItemAtWorldCoords(x, y);

			//If no growing tree found return	
				if(!(std::find(std::begin(Growing_Trees), std::end(Growing_Trees), item.ID) != std::end(Growing_Trees))) {
					return;
				}
				
			//Removes growing tree
				Game::RemoveItems(true, x, y, 1, 1, false, false, false);	

			//Some neat particles for a nice effect | do it 20 times to spam it	
				for(int i = 0; i <= 20; ++i) {
					Game::Particles(0x19A, PlayerClass::GetInstance()->GetCoordinates(x, y));	
				}
								
			//Places Grown Tree
				item.ID += 1; //Jumps to next growth level
				Dropper::PlaceItemWrapper(0xC, ReplaceEverything, &item, &item, x, y, 0, 0, 0, 0, 0, 0x5C, 0xA5, false);

				if(!bypassing) {
					Dropper::DropItemLock(false);
				}
			} break;
		/*If Kiki and Lala Wand | Stores outfits for you and lets you select them*/
			case 0x339D: {

			} break;
		}
	}
}
