#include "features/cheats.hpp"
#include "core/HUD.hpp"
#include "core/game_api/Game.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/checks/IDChecks.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/game_api/Animation.hpp"
#include "core/game_api/AnimData.hpp"
#include "core/game_api/Player.hpp"

extern "C" void PATCH_ToolAnim(void);

u8 toolTypeAnimID = 6;

namespace CTRPluginFramework {
//Wrapper Stuff
	u8 a_AnimID = 6;
	Item a_ItemID = {0x2001, 0};
	u16 a_SnakeID = 1;
	u8 a_EmoteID = 1;
	u16 a_SoundID = 0x660;
	u8 a_AppearanceID[3] = {0, 0, 0};
	int setmode = 0;
//Speed
	bool speedmode = false;

	bool IsPlayerSelectEnabled = false;

	void TogglePlayerSelect(u8 pIndex) {
		static Address playerSelect(0x305EF0);
		static Address playerSelect2 = playerSelect.MoveOffset(4);

		if (pIndex < 4) {
			playerSelect.Patch(0xE3A00000 + pIndex);
			playerSelect2.Patch(0xE12FFF1E);
			IsPlayerSelectEnabled = true;
		}
		else {
			playerSelect.Unpatch();
			playerSelect2.Unpatch();
			IsPlayerSelectEnabled = false;
		}
	}

//check to make player selector better
	void PlayerSelectCheck(void) {
		if(!IsPlayerSelectEnabled) {
			return;
		}

		u8 pIndex = Game::GetOnlinePlayerIndex();
	//If player is not loaded or loading screen started, switch off the code
		if(!PlayerClass::GetInstance()->IsLoaded() || !PlayerClass::GetInstance(pIndex)->IsLoaded() || Game::IsRoomLoading()) {
			TogglePlayerSelect(4);
		}
	}

//Player Selector
	void playerSelector(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu += PlayerSelectCheck;
		}

		std::vector<std::string> pV = {
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
		};

		std::vector<bool> validPlayerOption = { false, false, false, false };

		if(entry->Hotkeys[0].IsPressed()) {
			for(int i = 0; i <= 3; ++i) {
				if(PlayerClass::GetInstance(i)->IsLoaded()) {
					pV[i] = Player::GetColor(i) << Language::getInstance()->get(TextID::PLAYER_SELECTOR_PLAYER) << std::to_string(i + 1);
					validPlayerOption[i] = true;
				}
			}

			Keyboard pKB(Language::getInstance()->get(TextID::KEY_SELECT_PLAYER), pV);

			int pChoice = pKB.Open();
			if(pChoice >= 0) {
				if(validPlayerOption[pChoice]) {
					TogglePlayerSelect(pChoice);
					HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::PLAYER_SELECT_CONTROLLING_ENABLED).c_str(), pChoice + 1));
				}
				else {
					HUD::Notify(Language::getInstance()->get(TextID::PLAYER_SELECT_PLAYER_NOT_EXISTS), Color::Red);
				}
			}
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			if(IsPlayerSelectEnabled) {
				HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::PLAYER_SELECT_CONTROLLING_DISABLED).c_str(), *(u8 *)(Address(0x75F010).addr + 0x10) + 1));
				TogglePlayerSelect(4);
				return;
			}
			HUD::Notify(Language::getInstance()->get(TextID::PLAYER_SELECT_NOT_SELECTED), Color::Red);
		}

		if(!entry->IsActivated()) {
			TogglePlayerSelect(4);
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu -= PlayerSelectCheck;
		}
	}

	void AntiAnimCheck(const u8 PlayerIndex, const u8 *Data, const u32 Unused, const u32 Flag, const u32 RoomID) {
        if (PlayerIndex != Game::GetActualPlayerIndex()) {
			HookContext& ctx = HookContext::GetCurrent();
            ctx.OriginalFunction<void, u8, const u8 *, u32, u32, u32>(PlayerIndex, Data, Unused, Flag, RoomID);
        }
    }

	void AntiDropCheck(const u8 PlayerIndex, const u8* Data) {
		const u8 DropPlayerIndex = Data[12] & 3;
		if (DropPlayerIndex != Game::GetActualPlayerIndex()) {
			HookContext& ctx = HookContext::GetCurrent();
			ctx.OriginalFunction<void, u8, const u8*>(PlayerIndex, Data);
		}
	}

//Disable Force Animation on yourself | Thanks to Gokiro
	void anticheat(MenuEntry *Entry) {
        static Hook AnimHook1, AnimHook2, AnimHook3, AnimHook4, InvalidDropCheckHook;
		static Address anim1(0x185EF8), anim2(0x185F28), anim3(0x185F58), anim4(0x185F88), drop(0x1832D8);

        if (Entry->WasJustActivated()) {
            AnimHook1.InitializeForMitm(anim1.addr, (u32)&AntiAnimCheck);
            AnimHook1.Enable();
            AnimHook2.InitializeForMitm(anim2.addr, (u32)&AntiAnimCheck);
            AnimHook2.Enable();
            AnimHook3.InitializeForMitm(anim3.addr, (u32)&AntiAnimCheck);
            AnimHook3.Enable();
            AnimHook4.InitializeForMitm(anim4.addr, (u32)&AntiAnimCheck);
            AnimHook4.Enable();

			InvalidDropCheckHook.InitializeForMitm(drop.addr, (u32)AntiDropCheck);
        	InvalidDropCheckHook.Enable();

			IfForceAllowed = false;
        }
        else if (!Entry->IsActivated()) {
            AnimHook1.Disable();
            AnimHook2.Disable();
            AnimHook3.Disable();
            AnimHook4.Disable();
			InvalidDropCheckHook.Disable();

			IfForceAllowed = true;
        }
    }

	void AnimChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(!IDChecks::AnimationValid((ID & 0xFF), Game::GetOnlinePlayerIndex())) {
			keyboard.SetError(Color::Red << Language::getInstance()->get(TextID::INVALID_ID));
			return;
		}
	}

	void SnakeChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		u16 ID = StringToHex<u16>(input, 0xFFFF);
		if(!IDChecks::SnakeValid((ID & 0xFFFF))) {
			keyboard.SetError(Color::Red << Language::getInstance()->get(TextID::INVALID_ID));
			return;
		}
	}

	void EmotionChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		u8 ID = StringToHex<u8>(input, 0xFF);
		if(!IDChecks::EmotionValid((ID & 0xFF))) {
			keyboard.SetError(Color::Red << Language::getInstance()->get(TextID::INVALID_ID));
			return;
		}
	}

	void MusicChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		u16 ID = StringToHex<u16>(input, 0xFFFF);
		if(!IDChecks::MusicValid((ID & 0xFFFF))) {
			keyboard.SetError(Color::Red << Language::getInstance()->get(TextID::INVALID_ID));
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
					HUD::Notify(Language::getInstance()->get(TextID::ANIM_MOD_ANIM_MODE), Color::Red);
					mode++;
					setmode = 1;
				break;
				case 1:
					HUD::Notify(Language::getInstance()->get(TextID::ANIM_MOD_TOOL_MODE), Color::Orange);
					mode++;
					setmode = 2;
				break;
				case 2:
					HUD::Notify(Language::getInstance()->get(TextID::ANIM_MOD_SNAKE_MODE), Color::Lime);
					mode++;
					setmode = 3;
				break;
				case 3:
					HUD::Notify(Language::getInstance()->get(TextID::ANIM_MOD_EMOTION_MODE), Color::Cyan);
					mode++;
					setmode = 4;
				break;
				case 4:
					HUD::Notify(Language::getInstance()->get(TextID::ANIM_MOD_SOUND_MODE), Color::Magenta);
					mode++;
					setmode = 5;
				break;
				case 5:
					HUD::Notify(Language::getInstance()->get(TextID::ANIM_MOD_APPEARANCE_MODE), Color::Yellow);
					mode = mode - 5;
					setmode = 6;
				break;
			}
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			switch(setmode) {
				case 0: return;
				case 1:
					PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::ANIMATIONS_ANIM_NOTE), true, 2, a_AnimID, AnimChange }, a_AnimID);
				break;
				case 2:
					PluginUtils::Input::PromptNumber<u32>({ Language::getInstance()->get(TextID::ANIMATIONS_TOOL_NOTE), true, 8, *(u32 *)&a_ItemID, ItemChange }, *(u32 *)&a_ItemID);
				break;
				case 3:
					PluginUtils::Input::PromptNumber<u16>({ Language::getInstance()->get(TextID::ANIMATIONS_SNAKE_NOTE), true, 3, a_SnakeID, SnakeChange }, a_SnakeID);
				break;
				case 4:
					PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::ANIMATIONS_EMOTE_NOTE), true, 2, a_EmoteID, EmotionChange }, a_EmoteID);
				break;
				case 5:
					PluginUtils::Input::PromptNumber<u16>({ Language::getInstance()->get(TextID::ANIMATIONS_SOUND_NOTE), true, 3, a_SoundID, MusicChange }, a_SoundID);
				break;
				case 6: {
					//They cant really crash so no valid check
					PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::ANIMATIONS_APPEAR_NOTE1), true, 2, a_AppearanceID[0] }, a_AppearanceID[0]);
					PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::ANIMATIONS_APPEAR_NOTE2), true, 2, a_AppearanceID[1] }, a_AppearanceID[1]);
					PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::ANIMATIONS_APPEAR_NOTE3), true, 2, a_AppearanceID[2] }, a_AppearanceID[2]);
				} break;
			}
		}

		else if(entry->Hotkeys[2].IsPressed()) {
			speedmode = !speedmode;
			HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::ANIM_MOD_SPEED_MODE).c_str(), (speedmode ? HUD::Green << Language::getInstance()->get(TextID::STATE_ON) : HUD::Red << Language::getInstance()->get(TextID::STATE_OFF))));
		}

		if(entry->Hotkeys[3].IsPressed()) {
			PlayerClass::GetInstance(Game::GetOnlinePlayerIndex())->GetWorldCoords(&wX, &wY);
		}

		if(speedmode ? entry->Hotkeys[3].IsDown() :entry->Hotkeys[3].IsPressed()) {//Key::A + B
			switch(setmode) {
				case 0: return;
				case 1: //Animation
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), IDChecks::AnimationValid(a_AnimID, Game::GetOnlinePlayerIndex()) ? a_AnimID : 6, a_ItemID.isValid() ? a_ItemID : Item{0x2001, 0}, a_EmoteID, a_SnakeID, a_SoundID, 0, wX + offsetX, wY + offsetY, 0, a_AppearanceID);
				break;
				case 2: //Tool
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0x38, a_ItemID.isValid() ? a_ItemID : Item{0x2001, 0}, 0, 0, 0, 0, wX + offsetX, wY + offsetY, 0, a_AppearanceID);
				break;
				case 3: //Snake
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0xC5, {0, 0}, 0, IDChecks::SnakeValid(a_SnakeID) ? a_SnakeID : 1, 0, 0, wX + offsetX, wY + offsetY, 0, 0);
				break;
				case 4: //Emote
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0xAF, {0, 0}, IDChecks::EmotionValid(a_EmoteID) ? a_EmoteID : 1, 0, 0, 0, wX + offsetX, wY + offsetY, 0, 0);
				break;
				case 5: //Sound
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0xC4, {0, 0}, 0, 0, IDChecks::MusicValid(a_SoundID) ? a_SoundID : 0x660, 0, wX + offsetX, wY + offsetY, 0, 0);
					Sleep(Milliseconds(100));
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0x06, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0);
				break;
				case 6: //Appearance
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0xB9, {0x2001, 0}, 0, 0, 0, 0, wX + offsetX, wY + offsetY, 0, a_AppearanceID);
					Sleep(Seconds(2));
					Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0x06, {0, 0}, 0, 0, 0, 0, 0, 0, 0, 0);
				break;
			}

			if(Controller::IsKeyDown(Key::CPadRight)) {
				offsetX++;
			}
			if(Controller::IsKeyDown(Key::CPadLeft)) {
				offsetX--;
			}
			if(Controller::IsKeyDown(Key::CPadDown)) {
				offsetY++;
			}
			if(Controller::IsKeyDown(Key::CPadUp)) {
				offsetY--;
			}
		}

		else {
			offsetX = 0;
			offsetY = 0;
		}
	}
//Emotion Loop
	void inf_expression(MenuEntry *entry) {
		static Address infex(0x65E9B0);

        if(entry->Hotkeys[0].IsDown()) {
			infex.Patch(0xE3A010FF);
		}
		if(!entry->Hotkeys[0].IsDown()) {
			infex.Unpatch();
		}
	}
//Idle Animation
	void idle(MenuEntry *entry) {
		if(entry->Hotkeys[0].IsDown()) {
			Animation::Idle();
		}
	}

//Set Animation On Everyone
	void doonall(MenuEntry *entry) {
		static Address doonall1(0x677504);
		static Address doonall2(0x628B54);

		u8 DATAIndexRandom = Utils::Random(0, 3);
		static u32 wX, wY;

		if(entry->Hotkeys[0].IsPressed()) {
			PlayerClass::GetInstance()->GetWorldCoords(&wX, &wY);
		}

		if(speedmode ? entry->Hotkeys[0].IsDown() : entry->Hotkeys[0].IsPressed()) {
			for(u8 i = 0; i < 4; i++) {
				switch(setmode) {
					case 0: return;
					case 1: //Animation
						Animation::ExecuteAnimationWrapper(i, IDChecks::AnimationValid(a_AnimID, i) ? a_AnimID : 0x06, a_ItemID.isValid() ? a_ItemID : Item{0x2001, 0}, a_EmoteID, a_SnakeID, a_SoundID, 0, wX, wY, 1, a_AppearanceID);
					break;
					case 2: //Tool
						Animation::ExecuteAnimationWrapper(i, 0x38, a_ItemID.isValid() ? a_ItemID : Item{0x2001, 0}, 0, 0, 0, 0, wX, wY, 1, a_AppearanceID);
					break;
					case 3: //Snake
						Animation::ExecuteAnimationWrapper(i, 0xC5, {0, 0}, 0, IDChecks::SnakeValid(a_SnakeID) ? a_SnakeID : 0x001, 0, 0, wX, wY, 1, 0);
					break;
					case 4: //Emote
						Animation::ExecuteAnimationWrapper(i, 0xAF, {0, 0}, IDChecks::EmotionValid(a_EmoteID) ? a_EmoteID : 0x01, 0, 0, 0, wX, wY, 1, 0);
					break;
					case 5: //Sound
						Animation::ExecuteAnimationWrapper(i, 0xC4, {0, 0}, 0, 0, IDChecks::MusicValid(a_SoundID) ? a_SoundID : 0x0660, 0, wX, wY, 1, 0);
						Sleep(Milliseconds(100));
						Animation::ExecuteAnimationWrapper(i, 0x06, {0, 0}, 0, 0, 0, 0, 0, 0, 1, 0);
					break;
					case 6: //Appearance
						Animation::ExecuteAnimationWrapper(i, 0xB9, {0x2001, 0}, 0, 0, 0, 0, wX, wY, 1, a_AppearanceID);
						Sleep(Seconds(1));
						Animation::ExecuteAnimationWrapper(i, 0x06, {0, 0}, 0, 0, 0, 0, 0, 0, 1, 0);
					break;
				}
			}
		}

		if(speedmode ? entry->Hotkeys[1].IsDown() : entry->Hotkeys[1].IsPressed()) {
			for(u8 i = 0; i < 4; ++i) {
				Animation::ExecuteAnimationWrapper(i, 6, {0, 0}, 0, 0, 0, 0, 0, 0, 1, 0);
			}
		}

		if(entry->Hotkeys[2].IsDown()) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				return;
			}

			doonall1.Patch(0xE1A00000);
			doonall2.Patch(0xE3A01000 + DATAIndexRandom);

			for(u8 i = 0; i < 4; ++i) {
				u32 playerInstance = PlayerClass::GetInstance()->Offset();
				u32 animInstance = Animation::GetAnimationInstance(PlayerClass::GetInstance()->Offset(), 0, 0, 0);
				AnimData data;
				data.Init(animInstance, playerInstance, i);
				data.MoonJump_C4();

				if(Game::GetOnlinePlayerIndex() == Game::GetActualPlayerIndex()) {
					data.ExecuteAnimation(0xC4);
				}
				else {
					Animation::SendAnimPacket(i, animInstance, 0xC4, Player::GetRoom(i), i);
				}
			}
        }
		if(!entry->Hotkeys[2].IsDown()) {
            doonall1.Unpatch();
			doonall2.Unpatch();
		}
	}

	namespace {
		Hook g_toolTypeHook;
		bool g_toolTypeHookInitialized = false;

		void EnsureToolTypeHookInitialized(void) {
			if(g_toolTypeHookInitialized) {
				return;
			}

			g_toolTypeHook.Initialize(Address(0x64DB90).addr + 0x10, (u32)PATCH_ToolAnim);
			g_toolTypeHook.SetFlags(USE_LR_TO_RETURN);
			g_toolTypeHookInitialized = true;
		}

		void ApplyToolTypeAnimId(u8 animId) {
			toolTypeAnimID = animId;

			if(toolTypeAnimID == 0) {
				if(g_toolTypeHookInitialized) {
					g_toolTypeHook.Disable();
				}
				return;
			}

			if(!IDChecks::AnimationValid(toolTypeAnimID)) {
				toolTypeAnimID = 6;
			}

			EnsureToolTypeHookInitialized();
			g_toolTypeHook.Enable();
		}
	}

	void ToolTypeApplySaved(MenuEntry *entry, u32 savedValue) {
		(void)entry;
		ApplyToolTypeAnimId(static_cast<u8>(savedValue & 0xFF));
	}

//Change Tool Animation
	void tooltype(MenuEntry *entry) {
		if(PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::TOOL_ANIM_ENTER_ANIM), true, 2, toolTypeAnimID }, toolTypeAnimID)) {
			ApplyToolTypeAnimId(toolTypeAnimID);
			entry->SetSavedValue(toolTypeAnimID);
		}
	}
}
