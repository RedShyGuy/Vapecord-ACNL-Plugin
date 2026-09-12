#include "features/cheats.hpp"
#include "core/game_api/Game.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/game_api/Inventory.hpp"
#include "core/game_api/Player.hpp"
#include "core/infrastructure/Address.hpp"
#include "core/game_api/Animation.hpp"
#include "core/game_api/AnimData.hpp"
#include "core/game_api/Dropper.hpp"
#include "core/hooks/GameLoopHook.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/checks/IDChecks.hpp"
#include "core/RuntimeContext.hpp"
#include "Color.h"
#include "Files.h"
#include "core/HUD.hpp"

#include <array>
#include <optional>

u32 BGR_HairVal = 0;
u32 BGR_EyeVal = 0;
extern "C" void BGRHook(void);
extern "C" void SetPlayerIconCoordinates(void);

namespace CTRPluginFramework {
	namespace {
		constexpr u8 CustomHairColorId = 0x00;
		constexpr u8 CustomEyeColorId = 0x01;
		bool g_appColorsLoaded = false;
		bool g_appColorModEnabled = false;
		Hook g_appColorHook;
		bool g_appColorHookInitialized = false;

		bool EnsureCompatibleCustomColorId(ACNL_Player *player, bool hairColor) {
			if(player == nullptr) {
				MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
				return false;
			}

			u8 &currentId = hairColor ? player->PlayerAppearance.PlayerFeatures.HairColor : player->PlayerAppearance.PlayerFeatures.EyeColor;
			const u8 requiredId = hairColor ? CustomHairColorId : CustomEyeColorId;
			if(currentId == requiredId) {
				return true;
			}

			const auto *lang = Language::getInstance();
			const TextID labelId = hairColor ? TextID::CUSTOM_HAIR_ID_FIX_LABEL : TextID::CUSTOM_EYE_ID_FIX_LABEL;
			const TextID noteId = hairColor ? TextID::CUSTOM_HAIR_ID_FIX_NOTE : TextID::CUSTOM_EYE_ID_FIX_NOTE;

			if(!(MessageBox(
				lang->get(labelId),
				Utils::Format(lang->get(noteId).c_str(), currentId),
				DialogType::DialogYesNo
			)).SetClear(ClearScreen::Top)()) {
				return false;
			}

			currentId = requiredId;
			Player::UpdateStyle();
			return true;
		}

		bool EnsureCompatibleCustomColors(ACNL_Player *player) {
			return EnsureCompatibleCustomColorId(player, true) && EnsureCompatibleCustomColorId(player, false);
		}

		std::vector<std::string> strings1 = { "", "", "", "", "", "" };
		HUD::TagColor pColors[4] = { HUD::Blue, HUD::Red, HUD::Green, HUD::Orange };

		void GetPlayerInfoData(void) {
			u8 pIndex = Game::GetOnlinePlayerIndex();
			if(!PlayerClass::GetInstance(pIndex)->IsLoaded() || !Player::GetSaveData(pIndex)) {
				return;
			}

		//gets coordinates
			float *pCoords = PlayerClass::GetInstance(pIndex)->GetCoordinates();
			if(!pCoords) {
				return;
			}

		//Gets world coords
			if(!MapEditorActive) {
				PlayerClass::GetInstance(pIndex)->GetWorldCoords(&selectedX, &selectedY);
			}

		//gets item standing on
			Item *pItem = Game::GetItemAtWorldCoords(selectedX, selectedY);
			if(!pItem) {
				return;
			}

		//gets inv item
			u8 slot = 0;
			if(Inventory::GetSelectedSlot(slot)) {
				Inventory::ReadSlot(slot, itemslotid);
			}
			else {
				itemslotid = ReplaceEverything;
			}

			u8 menuID = Inventory::GetCurrent();

			strings1[0] = (Utils::Format(Language::getInstance()->get(TextID::PLAYER_INFO_WORLD_COORDS).c_str(), (u8)(selectedX & 0xFF), (u8)(selectedY & 0xFF)));
			strings1[1] = (Utils::Format(Language::getInstance()->get(TextID::PLAYER_INFO_ANIMATION).c_str(), *PlayerClass::GetInstance(pIndex)->GetAnimation(), *PlayerClass::GetInstance(pIndex)->GetSnake()));
			strings1[2] = (Language::getInstance()->get(TextID::PLAYER_INFO_ITEM_STANDING) << " " << (pItem->ID != 0 ? Utils::Format("%08X", *(u32 *)pItem) : Language::getInstance()->get(TextID::PLAYER_INFO_ITEM_NA)) << (Game::GetLockedSpotIndex(selectedX, selectedY) != 0xFFFFFFFF ? Language::getInstance()->get(TextID::PLAYER_INFO_ITEM_LOCKED) : ""));
			strings1[3] = (itemslotid != ReplaceEverything) ? Utils::Format(Language::getInstance()->get(TextID::INVENTORY_T2I_SET).c_str(), itemslotid) : Language::getInstance()->get(TextID::PLAYER_INFO_INV_NO_SLOT);
			strings1[4] = menuID != 0xFF ? Utils::Format(Language::getInstance()->get(TextID::PLAYER_INFO_MENU_ID).c_str(), menuID) : Language::getInstance()->get(TextID::PLAYER_INFO_MENU_ID_NO);
			strings1[5] = (Utils::Format(Language::getInstance()->get(TextID::PLAYER_INFO_ROOM).c_str(), Player::GetRoom(pIndex)));
		}

		bool debugHUD() {
			u8 pIndex = Game::GetOnlinePlayerIndex();
			if(!PlayerClass::GetInstance(pIndex)->IsLoaded()) {
				return true;
			}

			HUD::Text text = pColors[pIndex] << Utils::Format(Language::getInstance()->get(TextID::PLAYER_INFO_PLAYER).c_str(), pIndex + 1) << HUD::White;

			for (int i = 0; i < 6; ++i) {
				if(!strings1.at(i).empty()) {
					text << "\n";
					text << strings1.at(i);
				}
			}

			HUD::Draw(0.0f, 0.0f, text);

			return true;
		}

		u32 SwapRgbBgr(u32 value) {
			u8 byteshift[3] = { 0 };

		//shifts into bytes
			byteshift[0] = ((value & 0x00FF0000) >> 16);
			byteshift[1] = ((value & 0x0000FF00) >> 8);
			byteshift[2] = ((value & 0x000000FF));

		//reshifts back into integer
			return (byteshift[2] << 16) + (byteshift[1] << 8) + byteshift[0];
		}

		u32 RainbowBGR() {
			u8 ShiftB = Utils::Random(0, 255);
			u8 ShiftG = Utils::Random(0, 255);
			u8 ShiftR = Utils::Random(0, 255);

			return (ShiftB << 16 | ShiftG << 8 | ShiftR);
		}

		static u32 rval1 = 0, rval2 = 0;

		struct App_Colors {
			u32 Hair_BGR;
			u32 Eye_BGR;
		};

		bool LoadAppColors(bool notify = false) {
			const std::string path = Utils::Format(PATH_COLOR, Address::regionName.c_str());
			if(!File::Exists(path)) {
				return false;
			}

			App_Colors oldColor;
			File f_color(path, File::READ);
			f_color.Read(&oldColor, 8);
			f_color.Flush();
			f_color.Close();

			rval1 = oldColor.Hair_BGR;
			rval2 = oldColor.Eye_BGR;
			g_appColorsLoaded = true;

			if(notify) {
				HUD::Notify(Language::getInstance()->get(TextID::COLOR_MOD_PLAYER_LOADED), Color::Orange);
			}

			return true;
		}

		void EnsureAppColorsLoaded(void) {
			if(!g_appColorsLoaded) {
				LoadAppColors();
			}
		}

		void SaveAppColors(void) {
			const std::string path = Utils::Format(PATH_COLOR, Address::regionName.c_str());
			if(!File::Exists(path)) {
				File::Create(path);
			}

			App_Colors newColor = { rval1, rval2 };

			File f_color(path, File::WRITE);
			f_color.Write(&newColor, 8);
			f_color.Flush();
			f_color.Close();

			g_appColorsLoaded = true;
		}

		void SyncAppColorValues(void) {
			BGR_HairVal = rval1;
			BGR_EyeVal = rval2;
		}

		void EnableAppColorMod(bool notifyLoaded = false) {
			if(notifyLoaded) {
				LoadAppColors(true);
			}
			else {
				EnsureAppColorsLoaded();
			}

			if(!g_appColorHookInitialized) {
				static const Address address(0x4A33C8);
				g_appColorHook.Initialize(address.addr, (u32)BGRHook);
				g_appColorHook.SetFlags(USE_LR_TO_RETURN);
				g_appColorHookInitialized = true;
			}

			SyncAppColorValues();
			g_appColorHook.Enable();
			g_appColorModEnabled = true;
		}

		void DisableAppColorMod(void) {
			if(g_appColorModEnabled) {
				g_appColorHook.Disable();
				g_appColorModEnabled = false;
			}
		}

		std::string FormatStoredColorOption(TextID labelId, u32 storedBgr) {
			u32 rgb = SwapRgbBgr(storedBgr);
			return Language::getInstance()->get(labelId)
				<< Color((rgb << 8) | 0xFF)
				<< Utils::Format(" [%06X]", rgb & 0xFFFFFF);
		}

		bool SetAppColorModEnabled(MenuEntry *entry, bool enabled, bool validatePlayerAppearance) {
			(void)entry;

			if(enabled) {
				if(validatePlayerAppearance) {
					ACNL_Player *player = Player::GetSaveData();
					if(!EnsureCompatibleCustomColors(player)) {
						return false;
					}
				}

				EnableAppColorMod();

				entry->SetSavedValue(1u);
				return true;
			}

			DisableAppColorMod();

			entry->ClearSavedValue();
			return true;
		}

		bool EditStoredColor(u32 &storedBgr, TextID promptId) {
			u32 rgbValue = SwapRgbBgr(storedBgr);
			Keyboard kb(Language::getInstance()->get(promptId));
			kb.IsHexadecimal(true);
			kb.SetMaxLength(6);

			if(kb.Open(rgbValue) < 0) {
				return false;
			}

			storedBgr = SwapRgbBgr(rgbValue);
			g_appColorsLoaded = true;
			SyncAppColorValues();
			return true;
		}
	}

	void debug(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			HUD::Run(debugHUD);
		}
		else if(!entry->IsActivated()) {
			HUD::Stop(debugHUD);
			return;
		}

		GetPlayerInfoData();
	}

//player loader | for now disabled
/*
	void pLoaderEntry(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Both)();
			return;
		}

		if(GameHelper::GetOnlinePlayerCount() != 0) {
			MessageBox("Only works offline!").SetClear(ClearScreen::Both)();
			return;
		}

		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};

		for(int i = 0; i <= 3; ++i) {
			u32 pO = Player::GetSpecificSave(i);
			if(*(u16 *)(pO + 0x55A6) != 0) {
				std::string pS = "";
				Process::ReadString((pO + 0x55A8), pS, 0x10, StringFormat::Utf16);
				pV[i] = Player::GetColor(i) << pS;
			}
		}

		Keyboard pKB(Language::getInstance()->get("KEY_SELECT_PLAYER"), pV);

		int pChoice = pKB.Open();
		if((pChoice >= 0) && (pV[pChoice] != Color::Silver << "-Empty-"))
			Player::Load(pChoice);
	}
*/

	void SaveColor(MenuEntry *entry) {
		EnsureAppColorsLoaded();
		auto *lang = Language::getInstance();

		while(true) {
			const std::vector<std::string> options = {
				g_appColorModEnabled
					? (Color(pGreen) << lang->get(TextID::VECTOR_ENABLED))
					: (Color(pRed) << lang->get(TextID::VECTOR_DISABLED)),
				FormatStoredColorOption(TextID::CUSTOM_SET_HAIR, rval1),
				FormatStoredColorOption(TextID::CUSTOM_SET_EYE, rval2),
				lang->get(TextID::CUSTOM_HAIR_EYE_KEY1),
				lang->get(TextID::CUSTOM_SAVE),
			};

			Keyboard KB(lang->get(TextID::CUSTOM_HAIR_EYE_CHOOSE_ACTION), options);
			const int choice = KB.Open();
			if(choice < 0) {
				return;
			}

			switch(choice) {
				case 0:
					SetAppColorModEnabled(entry, !g_appColorModEnabled, true);
					break;

				case 1:
					if(EnsureCompatibleCustomColorId(Player::GetSaveData(), true) && EditStoredColor(rval1, TextID::CUSTOM_ENTER_HAIR)) {
						SetAppColorModEnabled(entry, true, false);
					}
					break;

				case 2:
					if(EnsureCompatibleCustomColorId(Player::GetSaveData(), false) && EditStoredColor(rval2, TextID::CUSTOM_ENTER_EYE)) {
						SetAppColorModEnabled(entry, true, false);
					}
					break;

				case 3:
					if(EnsureCompatibleCustomColors(Player::GetSaveData())) {
						rval1 = RainbowBGR();
						rval2 = RainbowBGR();
						g_appColorsLoaded = true;
						SetAppColorModEnabled(entry, true, false);
					}
					break;

				case 4:
					SaveAppColors();
					MessageBox(lang->get(TextID::CUSTOM_FILE_SAVED)).SetClear(ClearScreen::Top)();
					break;

				default:
					break;
			}
		}
	}

	void AppColorModApplySaved(MenuEntry *entry, u32 savedValue) {
		(void)entry;

		if(savedValue != 0) {
			EnableAppColorMod();
		}
	}

//Wear Helmet And Accessory /*Credits to Levi*/
	void hatz(MenuEntry *entry) {
		static Address hatwear(0x68C630);

		if(entry->WasJustActivated()) {
			hatwear.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			hatwear.Unpatch();
		}
	}

//Faint
	void Faint(MenuEntry *entry) {
        if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			u32 x, y;
			if(PlayerClass::GetInstance(Game::GetOnlinePlayerIndex())->GetWorldCoords(&x, &y)) {
				Animation::ExecuteAnimationWrapper(Game::GetOnlinePlayerIndex(), 0x9D, { 0, 0 }, 0, 0, 0, 0, x, y, 0, 0);
			}
		}
    }

//Thanks to Jay for original code, and thanks to Gokiro for finding out
//that the function needs to be called from the game's thread
	void FishThrower(MenuEntry *entry) {
		static Address getFacingPosition(0x76330C);
		static Address toInsectFishID(0x6d9ba8);
		static Address bsShowMgrInitFishFromPacket(0x6da6e0);
		static Address bsShowMgrInitFish(0x6da7fc);
		static Address bsInsectFieldMgrInstance(0x950534);
		static Address playerReleaseFish(0x5c2dac);
		static Address spoofPacket(0x628B54);
		static Address sendPacket(0x625488);

		class FishThrow {
		public:
			static std::optional<FishThrow>& GetInstance() {
				static std::optional<FishThrow> instance {};
				return instance;
			}

			void OpenOptions() {
				auto* lang = Language::getInstance();
				const std::vector<std::string> options = {
					lang->get(TextID::FISH_THROWER_OPTION_ITEM),
					lang->get(TextID::FISH_THROWER_OPTION_RANDOM) + ": " + (random ? Color::Green << lang->get(TextID::STATE_ON) : Color::Red << lang->get(TextID::STATE_OFF)),
					lang->get(TextID::FISH_THROWER_OPTION_DISTANCE) + Utils::Format(": %.1f", distance),
					lang->get(TextID::FISH_THROWER_OPTION_PUT_IN_HAND) + ": " + (putInHand ? Color::Green << lang->get(TextID::STATE_ON) : Color::Red << lang->get(TextID::STATE_OFF)),
				};

				Keyboard KB(Language::getInstance()->get(TextID::FISH_THROWER_CHOOSE_ACTION), options);
				if (int choice = KB.Open(); choice == 0) {
					u32 id = item;
					if (PluginUtils::Input::PromptNumber<u32>({ lang->get(TextID::ENTER_ID), true, 8, id, TextItemChange }, id)) {
						item = id;
					}
				}
				else if (choice == 1) {
					random = !random;
				}
				else if (choice == 2) {
					PluginUtils::Input::PromptNumber<float>({ "", false, 3, distance }, distance);
				}
				else if (choice == 3) {
					putInHand = !putInHand;
				}
			}

			void Update(MenuEntry *entry) {
				auto& hotkey = entry->Hotkeys[0];

				if (hotkey.IsPressed()) {
					keyDownTime = Ticks(svcGetSystemTick());
				}

				if (keyDownTime.AsTicks() != 0) {
					bool hasPassed = (Ticks(svcGetSystemTick()) - keyDownTime) >= Seconds(0.25f);
					if (hotkey.IsDown()) {
						if (hasPassed) {
							if (!RuntimeContext::getInstance()->isTurbo()) {
								keyDownTime = Ticks(0);
							}
							GameLoopHook::GetInstance()->Add([] {
								if (auto& instance = GetInstance()) {
									instance->GameHook();
								}
								return true;
							});
						}
					}
					else {
						if (!hasPassed) {
							OpenOptions();
						}
						keyDownTime = Ticks(0);
					}
				}
			}

		private:
			void GameHook() {
				auto* player = PlayerClass::GetInstance();
				if (!player->IsLoaded()) {
					return;
				}

				std::array<float, 3> pos;
				getFacingPosition.Call<void>(player->Offset(), pos.data(), *(u16*)(player->Offset(0x2e)), false, distance);

				const Item useItem = random ? Item(Utils::Random(0x22e1, 0x234b)) : item;
				const auto isFish = [useItem] { return useItem.ID >= 0x22e1 && useItem.ID <= 0x234b; };

				//items in hand are not visible for others when indoors or when using anything other than fish
				if (putInHand) {

					//prevent crash
					if (Player::IsIndoors() && useItem.ID < 0x2000) {
						return;
					}

					const u8 currentPlayer = Game::GetOnlinePlayerIndex();
					const u8 realPlayer = Game::GetActualPlayerIndex();
					if (currentPlayer != realPlayer) {
						struct PACKED ShowPacket {
							ShowPacket() = default;
							ShowPacket(const Item& item, const std::array<float, 3>& pos) :
								cmd(1),
								id(toInsectFishID.Call<u32>(&item)),
								x(static_cast<u32>(pos[0]) * 0.25f),
								z(static_cast<u32>(pos[2]) * 0.25f) {
							}

							u8 cmd : 2; //0: stop showing fish; 1: start showing fish
							u32 id : 7;
							u32 x : 10;
							u32 z : 10;
						};
						static_assert(sizeof(ShowPacket) == 4);

						const ShowPacket pkt(useItem, pos);
						bsShowMgrInitFishFromPacket.Call<void>(currentPlayer, &pkt);

						u32 oldValue;
						Process::Patch(spoofPacket.addr, 0xE3A01000 + currentPlayer, &oldValue);
						sendPacket.Call<void>(0x3d, 4, &pkt, sizeof(pkt));
						Process::Patch(spoofPacket.addr, oldValue);
					}
					else {
						bsShowMgrInitFish.Call<void>(&useItem, &pos);
					}
				}
				else {

					//prevent crash
					if (!isFish() && *(void**)bsInsectFieldMgrInstance.addr == nullptr) {
						return;
					}

					playerReleaseFish.Call<bool>(&useItem, pos.data(), 0);
				}
			}

			Time keyDownTime;
			Item item {0x2324, 0};
			float distance = 100.0f;
			bool putInHand = false;
			bool random = false;
		};

		if (entry->WasJustActivated()) {
			FishThrow::GetInstance().emplace();
		}
		if (entry->IsActivated()) {
			FishThrow::GetInstance()->Update(entry);
		}
		else {
			FishThrow::GetInstance().reset();
		}
	}

	c_RGBA* playerIcon[4] = { nullptr, nullptr, nullptr, nullptr };

	void LoadPlayerIcons(void) {
		for(int i = 0; i < 4; ++i) {
			if(PlayerClass::GetInstance(i)->IsLoaded() && playerIcon[i] == nullptr) {
				std::string filestr = Utils::Format(PATH_PICON, i);
				File file(filestr, File::READ);
				if(!file.IsOpen()) {
					return;
				}

				playerIcon[i] = new c_RGBA[file.GetSize() / sizeof(c_RGBA)];
				file.Read(playerIcon[i], file.GetSize());
				file.Close();
			}
			else if(!PlayerClass::GetInstance(i)->IsLoaded() && playerIcon[i] != nullptr) {
				delete[] playerIcon[i];
				playerIcon[i] = nullptr;
			}
		}
	}

//OSD For Show Players On The Map
	bool players(const Screen &screen) {
		if(screen.IsTop || !Game::IsMapOpened()) {
			return 0;
		}

		u32 XPos, YPos;

		for(int i = 0; i < 4; ++i) {
			if(PlayerClass::GetInstance(i)->IsLoaded()) {
				PlayerClass::GetInstance(i)->PlayerClass::CalculateMapCoordinates(XPos, YPos);

				if(playerIcon[i] != nullptr) {
					int Pixels = 0, XResult = 9 + XPos, YResult = 15 + YPos;

					for(int X = XPos; X < XResult; ++X) {
						for(int Y = YPos; Y < YResult; ++Y) {
							Color cPix = Color(playerIcon[i][Pixels].R, playerIcon[i][Pixels].G, playerIcon[i][Pixels].B, playerIcon[i][Pixels].A);
							if(cPix != Color(0, 0, 0, 0) && playerIcon[i][Pixels].A == 0xFF) {
								screen.DrawPixel(X, Y, cPix);
							}

							Pixels++;
						}
					}
				}
				else {
					screen.DrawRect(XPos - 1, YPos + 1, 6, 6, Player::GetColor(i));
				}
			}
		}

		return 1;
	}

//Show Players On The Map
	void map(MenuEntry *entry) {
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		static const Address writePatch(0x2215B0);
		static Hook hook;

		if(entry->WasJustActivated()) {
			hook.Initialize(writePatch.addr, (u32)SetPlayerIconCoordinates);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();

			u32 pInstance = PlayerClass::GetInstance()->Offset();
			if(pInstance != 0 && Game::IsMapOpened()) {
				u32 aInstance = Animation::GetAnimationInstance(pInstance, 0, 0, 0);

				AnimData data;
				data.Init(aInstance, pInstance, 4);
				data.Congrats_2A();
				data.ExecuteAnimation(0x2A);
			}

			*menu += LoadPlayerIcons;
			OSD::Run(players);
		}

		else if(!entry->IsActivated()) {
			hook.Disable();

			*menu -= LoadPlayerIcons;
			for(int i = 0; i < 4; ++i) {
				delete[] playerIcon[i];
				playerIcon[i] = nullptr;
			}

			OSD::Stop(players);
		}
	}

	void NeverBedHead(MenuEntry *entry) {
		static Address antiBedHead(0x20C798);

		if(entry->WasJustActivated()) {
			antiBedHead.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			antiBedHead.Unpatch();
		}
	}

	void editpattern(MenuEntry *entry) {
		for(int i = 0; i < 10; ++i) {
			Player::StealDesign(i);
		}

		MessageBox(Color::Orange << Language::getInstance()->get(TextID::PATTERN_STOLEN)).SetClear(ClearScreen::Top)();
	}
}