#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
//Wrapper Stuff
	static u8 AnimID = 6;
	static u32 ItemID = 0x2001;
	static u16 SnakeID = 1;
	static u8 EmoteID = 1;
	static u16 SoundID = 0x660;
	static u8 AppearanceID[3] = {0, 0, 0};
	static int setmode = 0;
//Speed
	static bool speedmode = false;

	void PSelector_Set(u8 pIndex) {
		Process::Patch(Code::a_GetOnlinePlayerIndex + 4, 0xE3A00000 + pIndex);
		Process::Patch(Code::a_GetOnlinePlayerIndex + 8, 0xE12FFF1E);
	}
	void PSelector_OFF(void) {
		Process::Patch(Code::a_GetOnlinePlayerIndex + 4, 0xE5900000);
		Process::Patch(Code::a_GetOnlinePlayerIndex + 8, Code::v_GetOnlinePlayerIndex);
	}
	bool PSelector_ON(void) {
		return (*(u32 *)Code::a_GetOnlinePlayerIndex != 0xE5900000);
	}

//check to make player selector better
	void PlayerSelectCheck(void) {
		if(!PSelector_ON())
			return;
		
		u8 pIndex = GameHelper::GetOnlinePlayerIndex();
	//If player is not loaded or loading screen started, switch off the code
		if(!PlayerClass::GetInstance()->IsLoaded() || !PlayerClass::GetInstance(pIndex)->IsLoaded() || GameHelper::LoadRoomBool()) 
			PSelector_OFF();
	} 
	
//Player Selector
	void playerSelector(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu += PlayerSelectCheck;
		}

		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};	
		
		if(entry->Hotkeys[0].IsPressed()) {
			for(int i = 0; i <= 3; ++i) {
				if(PlayerClass::GetInstance(i)->IsLoaded()) 
					pV[i] = pColor[i] << "Player: " << std::to_string(i);
			}
			
			Keyboard pKB(Language->Get("KEY_SELECT_PLAYER"));
			pKB.Populate(pV);
			s8 pChoice = pKB.Open();
			
			if(pChoice >= 0) {
				if(pV[pChoice] != Color::Silver << "-Empty-") {
					PSelector_Set(pChoice);
					OSD::Notify(Utils::Format("Controlling Player: %02X Enabled!", pChoice));
				}
				else
					OSD::Notify("Error: Player doesn't exist!", Color::Red);
			}
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {
			if(PSelector_ON()) {
				OSD::Notify(Utils::Format("Controlling Player: %02X Disabled!", *(u8 *)(Code::playselector + 0x10)));
				PSelector_OFF();
				return;
			}
			OSD::Notify("Error: No Player Is Selected!", Color::Red);
		}
		
		if(!entry->IsActivated()) {
			PSelector_OFF();
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu -= PlayerSelectCheck;
		}	
	}

	u32 AntiAnimCheck(u32 pID) {
		u32 pInstance = PlayerClass::GetInstance(pID)->Offset();
		if(pInstance == PlayerClass::GetInstance()->Offset()) 
			return 0;

		return pInstance; 
	}
	
//Disable Force Animation on yourself
	void anticheat(MenuEntry *entry) { 
		static Hook AntiHook;
		if(entry->WasJustActivated()) {	
			static const u32 AntiAddress = Region::AutoRegion(0x6786D4, 0x677BFC, 0x67770C, 0x67770C, 0x6771CC, 0x6771CC, 0x676D74, 0x676D74);
			AntiHook.Initialize(AntiAddress, (u32)AntiAnimCheck);
			AntiHook.SetFlags(USE_LR_TO_RETURN);
			AntiHook.Enable();
			IfForceAllowed = false;
		}
		
		else if(!entry->IsActivated()) {
			AntiHook.Disable();
			IfForceAllowed = true;
        }
	}
	
	void AnimChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(!IDList::AnimationValid((ID & 0xFF))) {
			keyboard.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}
	}
	void SnakeChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u16 ID = StringToHex<u16>(input, 0xFFFF);
		if(!IDList::SnakeValid((ID & 0xFFFF))) {
			keyboard.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}
	}
	void EmotionChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(!IDList::EmotionValid((ID & 0xFF))) {
			keyboard.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}
	}
	void MusicChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u16 ID = StringToHex<u16>(input, 0xFFFF);
		if(!IDList::MusicValid((ID & 0xFFFF))) {
			keyboard.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}
	}
	
//Animation Modifier
	void execAnim(MenuEntry *entry) {	
		static u8 offsetX, offsetY;
		static u32 wX, wY;
		
		static int mode = 0;
		
		if(entry->Hotkeys[0].IsPressed()) {	
			switch(mode) {
				default: break;
				case 0:
					OSD::Notify("Animation Mode", Color::Red);
					mode++;
					setmode = 1;
				break;
				case 1:
					OSD::Notify("Tool Mode", Color::Orange);
					mode++;
					setmode = 2;
				break;
				case 2:
					OSD::Notify("Snake Mode", Color::Lime);
					mode++;
					setmode = 3;
				break;
				case 3:
					OSD::Notify("Emotion Mode", Color::Cyan);
					mode++;
					setmode = 4;
				break;
				case 4:
					OSD::Notify("Sound Mode", Color::Magenta);
					mode++;
					setmode = 5;
				break;
				case 5:
					OSD::Notify("Appearance Mode", Color::Yellow);
					mode = mode - 5;
					setmode = 6;
				break;
			}
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {	
			switch(setmode) {
				case 0: return;
				case 1: 
					Wrap::KB<u8>(Language->Get("ANIMATIONS_ANIM_NOTE"), true, 2, AnimID, AnimID, AnimChange);
				break;
				case 2:
					Wrap::KB<u32>(Language->Get("ANIMATIONS_TOOL_NOTE"), true, 8, ItemID, ItemID, ItemChange);
				break;
				case 3:
					Wrap::KB<u16>(Language->Get("ANIMATIONS_SNAKE_NOTE"), true, 3, SnakeID, SnakeID, SnakeChange);
				break;
				case 4:
					Wrap::KB<u8>(Language->Get("ANIMATIONS_EMOTE_NOTE"), true, 2, EmoteID, EmoteID, EmotionChange);
				break;
				case 5:
					Wrap::KB<u16>(Language->Get("ANIMATIONS_SOUND_NOTE"), true, 3, SoundID, SoundID, MusicChange);
				break;
				case 6: {
					//They cant really crash so no valid check
					Wrap::KB<u8>(Language->Get("ANIMATIONS_APPEAR_NOTE1"), true, 2, AppearanceID[0], AppearanceID[0]);
					Sleep(Milliseconds(100));
					Wrap::KB<u8>(Language->Get("ANIMATIONS_APPEAR_NOTE2"), true, 2, AppearanceID[1], AppearanceID[1]);
					Sleep(Milliseconds(100));
					Wrap::KB<u8>(Language->Get("ANIMATIONS_APPEAR_NOTE3"), true, 2, AppearanceID[2], AppearanceID[2]);
				} break;
			}
		}
		
		else if(entry->Hotkeys[2].IsPressed()) {
			speedmode = !speedmode;
			OSD::Notify("Speed Mode " << (speedmode ? Color::Green << "ON" : Color::Red << "OFF"));
			
		}
		
		if(entry->Hotkeys[3].IsPressed()) 
			PlayerClass::GetInstance(GameHelper::GetOnlinePlayerIndex())->GetWorldCoords(&wX, &wY);
		
		if(speedmode ? entry->Hotkeys[3].IsDown() :entry->Hotkeys[3].IsPressed()) {//Key::A + B
			animExecuting = true;
			
			switch(setmode) {
				case 0: return;
				case 1: //Animation
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), IDList::AnimationValid(AnimID) ? AnimID : 6, IDList::ItemValid(ItemID) ? ItemID : 0x2001, EmoteID, SnakeID, SoundID, 0, wX + offsetX, wY + offsetY, 0, AppearanceID);
				break;
				case 2: //Tool
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0x38, IDList::ItemValid(ItemID) ? ItemID : 0x2001, 0, 0, 0, 0, wX + offsetX, wY + offsetY, 0, AppearanceID);
				break;
				case 3: //Snake
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0xC5, 0, 0, IDList::SnakeValid(SnakeID) ? SnakeID : 1, 0, 0, wX + offsetX, wY + offsetY, 0, 0);
				break;
				case 4: //Emote
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0xAF, 0, IDList::EmotionValid(EmoteID) ? EmoteID : 1, 0, 0, 0, wX + offsetX, wY + offsetY, 0, 0);
				break;
				case 5: //Sound
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0xC4, 0, 0, 0, IDList::MusicValid(SoundID) ? SoundID : 0x660, 0, wX + offsetX, wY + offsetY, 0, 0);
					Sleep(Milliseconds(100));
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0x06, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				break;
				case 6: //Appearance
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0xB9, 0x2001, 0, 0, 0, 0, wX + offsetX, wY + offsetY, 0, AppearanceID);
					Sleep(Seconds(2));
					Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0x06, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				break;
			}
			
			if(Controller::IsKeyDown(Key::CPadRight)) offsetX++;
			if(Controller::IsKeyDown(Key::CPadLeft)) offsetX--;
			if(Controller::IsKeyDown(Key::CPadDown)) offsetY++;
			if(Controller::IsKeyDown(Key::CPadUp)) offsetY--;
		}
		
		else {
			animExecuting = false;
			offsetX = 0;
			offsetY = 0;
		}
	}
//Emotion Loop
	void inf_expression(MenuEntry *entry) {
		static const u32 infex = Region::AutoRegion(0x65E9B0, 0x65DED8, 0x65D9E8, 0x65D9E8, 0x65D4A8, 0x65D4A8, 0x65D050, 0x65D050);
        if(entry->Hotkeys[0].IsDown()) 
			Process::Patch(infex, 0xE3A010FF);
		if(!entry->Hotkeys[0].IsDown()) 
			Process::Patch(infex, 0xE1D010B0);
	}
//Idle Animation 
	void idle(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsDown()) 
			Animation::Idle();
	}
//Slow Motion Animations
	void slmoanms(MenuEntry *entry) {
		static const u32 slo1 = Region::AutoRegion(0x654578, 0x653AA0, 0x6535B0, 0x6535B0, 0x653070, 0x653070, 0x652C18, 0x652C18);
		static const u32 slo2 = Region::AutoRegion(0x652C10, 0x652138, 0x651C48, 0x651C48, 0x651708, 0x651708, 0x6512B0, 0x6512B0);
		static const u32 slo3 = Region::AutoRegion(0x887880, 0x886878, 0x88670C, 0x88670C, 0x880B2C, 0x87FB2C, 0x87FBE0, 0x87FBE0);
		const u32 SlowAnim[2] = { slo2, slo3 };
		
		static const float SlowAnimPatch[2][2] = {
            { 8.0, 6.0 },
            { 1.0, 1.0 }
        };
		
		if(entry->Hotkeys[0].IsPressed()) {//Key::L + Key::DPadLeft
			bool index = *(u32 *)slo1 == 0x0A000004 ? false : true;

			Animation::Idle();
			OSD::Notify("Slow Animations " << (index ? Color::Red << "OFF" : Color::Green << "ON"));
			
			for(int i = 0; i < 2; ++i)
                Process::WriteFloat(SlowAnim[i], SlowAnimPatch[index][i]);

			Process::Patch(slo1, index ? 0x0A000004 : 0xE3A00001);
		}

		if(!entry->IsActivated()) {
			for(int i = 0; i < 2; ++i)
                Process::WriteFloat(SlowAnim[i], SlowAnimPatch[1][i]);

			Process::Patch(slo1, 0x0A000004);
        }
	}
//Set Animation On Everyone
	void doonall(MenuEntry *entry) {
		u8 DATAIndexRandom = Utils::Random(0, 3);
		static u32 wX, wY;
		
		if(entry->Hotkeys[0].IsPressed()) 
			PlayerClass::GetInstance()->GetWorldCoords(&wX, &wY);
		
		if(speedmode ? entry->Hotkeys[0].IsDown() : entry->Hotkeys[0].IsPressed()) {		
			animExecuting = true;
			for(u8 i = 0; i < 4; i++) {
				switch(setmode) {
					case 0: return;
					case 1: //Animation
						Animation::ExecuteAnimationWrapper(i, IDList::AnimationValid(AnimID) ? AnimID : 0x06, IDList::ItemValid(ItemID) ? ItemID : 0x2001, EmoteID, SnakeID, SoundID, 0, wX, wY, 1, AppearanceID);
					break;
					case 2: //Tool
						Animation::ExecuteAnimationWrapper(i, 0x38, IDList::ItemValid(ItemID) ? ItemID : 0x2001, 0, 0, 0, 0, wX, wY, 1, AppearanceID);
					break;
					case 3: //Snake
						Animation::ExecuteAnimationWrapper(i, 0xC5, 0, 0, IDList::SnakeValid(SnakeID) ? SnakeID : 0x001, 0, 0, wX, wY, 1, 0);
					break;
					case 4: //Emote
						Animation::ExecuteAnimationWrapper(i, 0xAF, 0, IDList::EmotionValid(EmoteID) ? EmoteID : 0x01, 0, 0, 0, wX, wY, 1, 0);
					break;
					case 5: //Sound
						Animation::ExecuteAnimationWrapper(i, 0xC4, 0, 0, 0, IDList::MusicValid(SoundID) ? SoundID : 0x0660, 0, wX, wY, 1, 0);
						Sleep(Milliseconds(100));
						Animation::ExecuteAnimationWrapper(i, 0x06, 0, 0, 0, 0, 0, 0, 0, 1, 0);
					break;
					case 6: //Appearance
						Animation::ExecuteAnimationWrapper(i, 0xB9, 0x2001, 0, 0, 0, 0, wX, wY, 1, AppearanceID);
						Sleep(Seconds(1));
						Animation::ExecuteAnimationWrapper(i, 0x06, 0, 0, 0, 0, 0, 0, 0, 1, 0);
					break;
				}
			}
		}
		
		if(speedmode ? entry->Hotkeys[1].IsDown() : entry->Hotkeys[1].IsPressed()) {
			for(u8 i = 0; i < 4; ++i) 
				Animation::ExecuteAnimationWrapper(i, 6, 0, 0, 0, 0, 0, 0, 0, 1, 0);
		}
		
		if(entry->Hotkeys[2].IsDown()) {
			if(!PlayerClass::GetInstance()->IsLoaded())
				return;

			Process::Patch(Code::AnimConditionPatch, 0xE1A00000); 
			Process::Patch(Code::IndexChange, 0xE3A01000 + DATAIndexRandom);
			
			for(u8 i = 0; i < 4; ++i) {
				u32 playerInstance = PlayerClass::GetInstance()->Offset();
				u32 animInstance = Animation::GetAnimationInstance(PlayerClass::GetInstance()->Offset(), 0, 0, 0);
				AnimData data;
				data.Init(animInstance, playerInstance, i);
				data.MoonJump_C4();
				
				if(GameHelper::GetOnlinePlayerIndex() == GameHelper::GetActualPlayerIndex()) 
					data.ExecuteAnimation(0xC4);
				else 
					Animation::SendAnimPacket(i, animInstance, 0xC4, Player::GetRoom(i), i);
			}
        } 
		if(!entry->Hotkeys[2].IsDown()) {
            Process::Patch(Code::AnimConditionPatch, 0x1A000017);
			Process::Patch(Code::IndexChange, 0xE5D11268);
		}
	}
}
