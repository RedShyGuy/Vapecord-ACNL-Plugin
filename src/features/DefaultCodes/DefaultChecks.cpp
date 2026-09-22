#include <CTRPluginFramework.hpp>
#include "core/infrastructure/Address.hpp"
#include "core/hooks/Checks.hpp"
#include "core/game_api/Game.hpp"
#include "core/infrastructure/Language.hpp"
#include <initializer_list>

extern "C" void SetProperParticle(void);

extern "C" bool __IsPuzzleLeagueRoom() {
    u8 roomID = (u8)CTRPluginFramework::Game::GetRoom();
    return roomID == 0x9E;
}

namespace CTRPluginFramework {
    void SetHook(Hook &hook, u32 address, u32 callback, u32 flags) {
		hook.Initialize(address, callback);
		hook.SetFlags(flags);
		hook.Enable();
	}

    static std::string FormatToggleStateOption(bool isEnabled) {
        return (isEnabled ? Color::Green : Color::Red)
            << Language::getInstance()->get(isEnabled ? TextID::VECTOR_ENABLED : TextID::VECTOR_DISABLED);
    }

    static bool ConfirmDefaultCodesDisable(void) {
        return MessageBox(
            Language::getInstance()->get(TextID::DEFAULT_CODES_DISABLE_WARNING),
            DialogType::DialogYesNo
        )();
    }

    int OptionKeyboard(bool isEnabled) {
        const std::vector<std::string> cmnOpt = { FormatToggleStateOption(isEnabled) };

        Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), cmnOpt);
        if (optKb.Open() < 0) {
            return -1;
        }

        if (isEnabled && !ConfirmDefaultCodesDisable()) {
            return -1;
        }

        return isEnabled ? 0 : 1;
    }

    void SetHooksEnabled(bool enable, std::initializer_list<Hook *> hooks) {
        for (Hook *hook : hooks) {
            if (hook == nullptr) {
                continue;
            }

            if (enable) {
                hook->Enable();
            }
            else {
                hook->Disable();
            }
        }
    }

    static Hook CheckInvalidBadgeHook1, CheckInvalidBadgeHook2;
    static Hook SaveButtonCheck;
    static Hook CatalogPHook1, CatalogPHook2;
    static Hook IPHook;
    static Hook InvDropHook, InvPlantHook;
    static Hook ISHook;
    static Hook InvalidGiveItemHook;
    static Hook IHHook;
    static Hook IIHook;
    static Hook CFHook;
    static Hook HoveredNameHook;
    static Hook NameHook;
    static Hook ReplaceHook;
    static Hook DropHook1, DropHook2, DropHook3, DropHook4;
    static Hook PlantHook1, PlantHook2, PlantHook3, PlantHook4;
    static Hook ParticleHook1, ParticleHook2;
    static Hook OnProDesignHook;

    void EnableAllChecks() {
        SetHook(CheckInvalidBadgeHook1, Address(0x6AF954).addr, (u32)CheckInvalidBadge, USE_LR_TO_RETURN);
        SetHook(CheckInvalidBadgeHook2, Address(0x6AECA0).addr, (u32)CheckInvalidBadge, USE_LR_TO_RETURN);

        SetHook(SaveButtonCheck, Address(0x1A0980).addr - 0x10, (u32)IsSTARTDown, USE_LR_TO_RETURN);

        SetHook(CatalogPHook1, Address(0x21C408).addr, (u32)CatalogPatch_Keyboard, USE_LR_TO_RETURN);
        SetHook(CatalogPHook2, Address(0x21C0AC).addr, (u32)CatalogPatch_SearchFunction, USE_LR_TO_RETURN);

        SetHook(IPHook, Address(0x59A258).addr, (u32)InvalidPickStop, USE_LR_TO_RETURN);

        SetHook(InvDropHook, Address(0x597850).addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
        SetHook(InvPlantHook, Address(0x597724).addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);

        SetHook(ISHook, Address(0x670EA0).addr, (u32)InvalidSpriteStop, USE_LR_TO_RETURN);

        SetHook(InvalidGiveItemHook, Address(0x2339CC).addr, (u32)InvalidGiveItem, USE_LR_TO_RETURN);

        SetHook(IHHook, Address(0x5980F4).addr, (u32)InvalidHoleStop, USE_LR_TO_RETURN);

        SetHook(IIHook, Address(0x72511C).addr, (u32)InvalidItemStop, USE_LR_TO_RETURN);

        SetHook(CFHook, Address(0x323514).addr, (u32)ConvertFlower, USE_LR_TO_RETURN);

        SetHook(HoveredNameHook, Address(0x19CE64).addr, (u32)SetHoveredItemName, USE_LR_TO_RETURN);
        SetHook(NameHook, Address(0x19C498).addr, (u32)SetItemName, USE_LR_TO_RETURN);

        SetHook(ReplaceHook, Address(0x165528).addr, (u32)IsItemReplaceable, USE_LR_TO_RETURN);

        SetHook(DropHook1, Address(0x19B66C).addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
        SetHook(DropHook2, Address(0x19C044).addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
        SetHook(DropHook3, Address(0x19CFF0).addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
        SetHook(DropHook4, Address(0x2AEA64).addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);

        SetHook(PlantHook1, Address(0x19B688).addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
        SetHook(PlantHook2, Address(0x19B97C).addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
        SetHook(PlantHook3, Address(0x2AEAC4).addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
        SetHook(PlantHook4, Address(0x2AFD24).addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);

        SetHook(ParticleHook1, Address(0x5506D4).addr, (u32)SetProperParticle, USE_LR_TO_RETURN);
        SetHook(ParticleHook2, Address(0x5509CC).addr, (u32)SetProperParticle, USE_LR_TO_RETURN);

        SetHook(OnProDesignHook, Address(0x2B9110).addr, (u32)SetProDesignStyle, USE_LR_TO_RETURN);
    }

    void DisableAllChecks() {
        CheckInvalidBadgeHook1.Disable();
        CheckInvalidBadgeHook2.Disable();
        SaveButtonCheck.Disable();
        CatalogPHook1.Disable();
        CatalogPHook2.Disable();
        IPHook.Disable();
        InvDropHook.Disable();
        InvPlantHook.Disable();
        ISHook.Disable();
        InvalidGiveItemHook.Disable();
        IHHook.Disable();
        IIHook.Disable();
        CFHook.Disable();
        HoveredNameHook.Disable();
        NameHook.Disable();
        ReplaceHook.Disable();
        DropHook1.Disable();
        DropHook2.Disable();
        DropHook3.Disable();
        DropHook4.Disable();
        PlantHook1.Disable();
        PlantHook2.Disable();
        PlantHook3.Disable();
        PlantHook4.Disable();
        ParticleHook1.Disable();
        ParticleHook2.Disable();
        OnProDesignHook.Disable();
    }

    void CheckInvalidBadgeEntry(MenuEntry *entry) {
        int res = OptionKeyboard(CheckInvalidBadgeHook1.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &CheckInvalidBadgeHook1, &CheckInvalidBadgeHook2 });
    }

    void DisableOpenSaveMenuWithStartButton(MenuEntry *entry) {
        int res = OptionKeyboard(SaveButtonCheck.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &SaveButtonCheck });
    }

    void DisableCatalogSearchFunction(MenuEntry *entry) {
        int res = OptionKeyboard(CatalogPHook1.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &CatalogPHook1, &CatalogPHook2 });
    }

    void FixInvalidPickupCrash(MenuEntry *entry) {
        int res = OptionKeyboard(IPHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &IPHook });
    }

    void FixInvalidDropPlantCrash(MenuEntry *entry) {
        int res = OptionKeyboard(InvDropHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &InvDropHook, &InvPlantHook });
    }

    void FixInvalidSpriteCrash(MenuEntry *entry) {
        int res = OptionKeyboard(ISHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &ISHook });
    }

    void FixInvalidGiveItemCrash(MenuEntry *entry) {
        int res = OptionKeyboard(InvalidGiveItemHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &InvalidGiveItemHook });
    }

    void FixInvalidHoleCrash(MenuEntry *entry) {
        int res = OptionKeyboard(IHHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &IHHook });
    }

    void FixInvalidItemCrash(MenuEntry *entry) {
        int res = OptionKeyboard(IIHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &IIHook });
    }

    void ConvertFlowerFromSeedItemToNormalItem(MenuEntry *entry) {
        int res = OptionKeyboard(CFHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &CFHook });
    }

    void SetSeedItemNames(MenuEntry *entry) {
        int res = OptionKeyboard(HoveredNameHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &HoveredNameHook, &NameHook });
    }

    void SetItemReplacementRules(MenuEntry *entry) {
        int res = OptionKeyboard(ReplaceHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &ReplaceHook });
    }

    void SetDropRules(MenuEntry *entry) {
        int res = OptionKeyboard(DropHook1.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &DropHook1, &DropHook2, &DropHook3, &DropHook4 });
    }

    void SetPlantRules(MenuEntry *entry) {
        int res = OptionKeyboard(PlantHook1.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &PlantHook1, &PlantHook2, &PlantHook3, &PlantHook4 });
    }

    void FixParticlesInPuzzleLeague(MenuEntry *entry) {
		int res = OptionKeyboard(ParticleHook1.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &ParticleHook1, &ParticleHook2 });
    }

    /*Not SeedItems*/
    void SetCustomSpritesForProDesigns(MenuEntry *entry) {
        int res = OptionKeyboard(OnProDesignHook.IsEnabled());
        if(res < 0) {
            return;
        }

        SetHooksEnabled(res == 1, { &OnProDesignHook });
    }

    /*
		static Hook suspendHook;
		static Address suspendAddress(0x124EB8);
		SetHook(suspendHook, suspendAddress.addr, (u32)SuspendCallBack, USE_LR_TO_RETURN);
		*/

		/*static Hook titleHook, textHook;
		static const warningTXT(0x2F319C);
		SetHook(titleHook, warningTXT.addr, (u32)SetTitle, USE_LR_TO_RETURN);
		SetHook(textHook, warningTXT.addr + 0xA8, (u32)SetText, USE_LR_TO_RETURN);*/

		/*static Hook hook;
		static const u32 address(0x323424, 0, 0, 0, 0, 0, 0, 0);
		hook.Initialize(address, (u32)func);
		hook.SetFlags(USE_LR_TO_RETURN);
		hook.Enable();*/

		/*static Hook ButtonCheck;
		static const u32 KeyPressedFunc(0x304A14, 0, 0, 0, 0, 0, 0, 0);
		ButtonCheck.Initialize(KeyPressedFunc, (u32)IsKeyDown);
		ButtonCheck.SetFlags(0);
		ButtonCheck.Enable();*/

	/*bool IsKeyDown(u32 key) {
	//custom instruction to check for ZL and ZR and the CStick
		if(Controller::IsKeyDown(Key::ZL) && key == 0x300000) return 1;
		if(Controller::IsKeyDown(Key::ZR) && key == 0x500000) return 1;
		if(Controller::IsKeyDown(Key::CStickRight) && key == 0x8000000) return 1;
		if(Controller::IsKeyDown(Key::CStickLeft) && key == 0x4000000) return 1;
		if(Controller::IsKeyDown(Key::CStickUp) && key == 0x1000000) return 1;
		if(Controller::IsKeyDown(Key::CStickDown) && key == 0x2000000) return 1;

	//this is the standard function to check for keys, exactly copied from the game
		int iVar1;

		static const u32 KeyCheck(0x5CF1AC, 0, 0, 0, 0, 0, 0, 0);

		static FUNCTION func(KeyCheck);
		iVar1 = func.Call<int>();

		if(iVar1 == 0) {
			static const u32 KeyPointer(0x9762F4, 0, 0, 0, 0, 0, 0, 0);
			if(*(u32 *)(*(u32 *)(KeyPointer) + 0xD0) == 0)
				iVar1 = 0;
			else
				iVar1 = **(u32 **)(*(u32 *)(KeyPointer) + 0xD8);

			key = *(u32 *)(iVar1 + 0x110) & key;
			if(key != 0)
				key = 1;
		}
		else
			key = 0;

		return key;
	}*/
}