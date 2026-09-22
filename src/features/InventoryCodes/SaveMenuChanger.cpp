#include "features/cheats.hpp"
#include <CTRPluginFramework.hpp>
#include "core/checks/IDChecks.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/infrastructure/Language.hpp"
#include "core/infrastructure/TextID.hpp"
#include "core/infrastructure/Address.hpp"
#include "core/game_api/Player.hpp"
#include "core/game_api/Game.hpp"
#include "core/game_api/Inventory.hpp"
#include "core/game_api/Animation.hpp"
#include "Color.h"
#include "core/HUD.hpp"

namespace CTRPluginFramework {
    namespace {
		static bool IsMenuPatchOpen = false;
		static u8 CurrentMenu = 0xFF;
		static Hook menuPatchHook;
		static bool menuPatchHookInitialized = false;
		const u8 Menus[8] = {
			0x2E, 0x37, 0x38, 0x3D, 0x79, 0x89, 0x00, 0xFF
		};

		void Callback_MenuPatch(void);
		void Hook_MenuPatch(u32 r0, u32 r1, u32 r3);

		static void EnsureMenuPatchHookInitialized(void) {
			if(menuPatchHookInitialized) {
				return;
			}

			menuPatchHook.Initialize(Address(0x1A0980).addr + 8, (u32)Hook_MenuPatch);
			menuPatchHook.SetFlags(USE_LR_TO_RETURN);
			menuPatchHookInitialized = true;
		}

		static void ApplyMenuChangerSelection(u8 menuId) {
			EnsureMenuPatchHookInitialized();

			if(menuId != 0xFF && !IDChecks::MenuValid(menuId)) {
				CurrentMenu = 0xFF;
				menuPatchHook.Disable();
				return;
			}

			CurrentMenu = menuId;
			if(CurrentMenu == 0xFF) {
				menuPatchHook.Disable();
				return;
			}

			menuPatchHook.Enable();
		}

		static void PersistAndApplyMenuChangerSelection(MenuEntry *entry, u8 menuId) {
			ApplyMenuChangerSelection(menuId);
			entry->SetSavedValue(CurrentMenu);
		}

		static std::vector<std::string> BuildMenuChangerOptions(void) {
			std::vector<std::string> menuopt = {
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_DATETIME),
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_BELLPOINT_DEPO),
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_BELLPOINT_WITHDRAW),
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_LOCKER),
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_TOWNTUNE),
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_HOUSESTORAGE),
				Language::getInstance()->get(TextID::VECTOR_SAVEMENU_CUSTOM),
				Language::getInstance()->get(TextID::VECTOR_DISABLE)
			};

			bool hasPresetSelection = false;
			for(int i = 0; i < 6; ++i) {
				const bool isOn = CurrentMenu == Menus[i];
				hasPresetSelection = hasPresetSelection || isOn;
				menuopt[i] = (isOn ? Color(pGreen) : Color(pRed)) << menuopt[i];
			}

			const bool customIsOn = (!hasPresetSelection && CurrentMenu != 0xFF);
			menuopt[6] = (customIsOn ? Color(pGreen) : Color(pRed)) << menuopt[6];
			menuopt[7] = (CurrentMenu == 0xFF ? Color(pGreen) : Color(pRed)) << menuopt[7];

			return menuopt;
		}

		static void HandleCustomMenuSelection(MenuEntry *entry) {
			u8 customMenu = CurrentMenu;

			if(!PluginUtils::Input::PromptNumber<u8>({ Language::getInstance()->get(TextID::SAVE_MENU_CHANGER_ENTER_ID), true, 2, 0 }, customMenu)) {
				return;
			}

			if(IDChecks::MenuValid(customMenu)) {
				PersistAndApplyMenuChangerSelection(entry, customMenu);
				return;
			}

			PersistAndApplyMenuChangerSelection(entry, 0xFF);
			MessageBox(Language::getInstance()->get(TextID::INVALID_ID)).SetClear(ClearScreen::Top)();
		}

		void Callback_MenuPatch(void) {
			if(Inventory::GetCurrent() == CurrentMenu && !IsMenuPatchOpen) {
				IsMenuPatchOpen = true;
			}

			if(Inventory::GetCurrent() != CurrentMenu && IsMenuPatchOpen) {
				Animation::Idle();
				IsMenuPatchOpen = false;
				PluginMenu *menu = PluginMenu::GetRunningInstance();
				*menu -= Callback_MenuPatch; //delete itself
			}
		}

		void Hook_MenuPatch(u32 r0, u32 r1, u32 r3) {
			u8 roomId = Player::GetRoom(4);
			if (roomId == 0xA1 || roomId == 0xA2 || (roomId >= 0x92 && roomId <= 0x97)) {
				HUD::Notify(Language::getInstance()->get(TextID::MENU_CHANGER_INVALID_ROOM), Color::Red);

				const HookContext &curr = HookContext::GetCurrent();
				static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
				func.Call<void>(r0, r1, r3);
			}
			else {
				Game::OpenMenu(CurrentMenu);
				PluginMenu *menu = PluginMenu::GetRunningInstance();
				*menu += Callback_MenuPatch;
			}
		}
	}

    void MenuChangerApplySaved(MenuEntry *entry, u32 savedValue) {
        (void)entry;
        ApplyMenuChangerSelection(static_cast<u8>(savedValue));
    }

    //Menu Changer
		void MenuChanger(MenuEntry *entry) {
			while(true) {
				Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_SAVE_MENU), BuildMenuChangerOptions());

				const int dChoice = optKb.Open();
				if(dChoice < 0) {
					return;
				}

				if(dChoice == 6) {
					HandleCustomMenuSelection(entry);
					return;
				}

				if(dChoice < 0 || dChoice >= 8) {
					return;
				}

				PersistAndApplyMenuChangerSelection(entry, Menus[dChoice]);

				if(CurrentMenu == 0xFF) {
					return;
				}
			}
		}
};