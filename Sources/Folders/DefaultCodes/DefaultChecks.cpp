#include <CTRPluginFramework.hpp>
#include "Address/Address.hpp"
#include "Helpers/Checks.hpp"
#include "Helpers/Game.hpp"

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

    void DisableOpenSaveMenuWithStartButton(MenuEntry *entry) {
        static Hook SaveButtonCheck;

        if(entry->WasJustActivated()) {
            SetHook(SaveButtonCheck, Address(0x1A0980).addr - 0x10, (u32)IsSTARTDown, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            SaveButtonCheck.Disable();
        }
    }

    void DisableCatalogSearchFunction(MenuEntry *entry) {
        static Hook CatalogPHook1, CatalogPHook2;
		static const Address CatalogPOffset1(0x21C408);
		static const Address CatalogPOffset2(0x21C0AC);

        if(entry->WasJustActivated()) {
            SetHook(CatalogPHook1, CatalogPOffset1.addr, (u32)CatalogPatch_Keyboard, USE_LR_TO_RETURN);
		    SetHook(CatalogPHook2, CatalogPOffset2.addr, (u32)CatalogPatch_SearchFunction, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            CatalogPHook1.Disable();
            CatalogPHook2.Disable();
        }
    }

    void FixInvalidPickupCrash(MenuEntry *entry) {
        static Hook IPHook;	
		static const Address IPOffset(0x59A258);

        if(entry->WasJustActivated()) {
            SetHook(IPHook, IPOffset.addr, (u32)InvalidPickStop, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            IPHook.Disable();
        }
    }

    void FixInvalidDropPlantCrash(MenuEntry *entry) {
        static Hook InvDropHook, InvPlantHook;
		static const Address InvDropOffset(0x597850);
		static const Address InvPlantOffset(0x597724);

        if(entry->WasJustActivated()) {
            SetHook(InvDropHook, InvDropOffset.addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
            SetHook(InvPlantHook, InvPlantOffset.addr, (u32)InvalidDropStop, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            InvDropHook.Disable();
            InvPlantHook.Disable();
        }
    }

    void FixInvalidSpriteCrash(MenuEntry *entry) {
        static Hook ISHook;
		static const Address ISOffset(0x670EA0);

        if(entry->WasJustActivated()) {
		    SetHook(ISHook, ISOffset.addr, (u32)InvalidSpriteStop, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            ISHook.Disable();
        }
    }

    void FixInvalidGiveItemCrash(MenuEntry *entry) {
        static Hook InvalidGiveItemHook;
		static const Address InvalidGiveItemAddress(0x2339CC);

        if(entry->WasJustActivated()) {
		    SetHook(InvalidGiveItemHook, InvalidGiveItemAddress.addr, (u32)InvalidGiveItem, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            InvalidGiveItemHook.Disable();
        }
    }

    void FixInvalidHoleCrash(MenuEntry *entry) {
        static Hook IHHook;
		static const Address IHOffset(0x5980F4);

        if(entry->WasJustActivated()) {
		    SetHook(IHHook, IHOffset.addr, (u32)InvalidHoleStop, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            IHHook.Disable();
        }
    }

    void FixInvalidItemCrash(MenuEntry *entry) {
        static Hook IIHook;
		static const Address IIOffset(0x72511C);

        if(entry->WasJustActivated()) {
		    SetHook(IIHook, IIOffset.addr, (u32)InvalidItemStop, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            IIHook.Disable();
        }
    }

    void ConvertFlowerFromSeedItemToNormalItem(MenuEntry *entry) {
        static Hook CFHook;
		static const Address CFOffset(0x323514);

        if(entry->WasJustActivated()) {
		    SetHook(CFHook, CFOffset.addr, (u32)ConvertFlower, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            CFHook.Disable();
        }
    }

    void SetSeedItemNames(MenuEntry *entry) {
        static Hook NameHook;
		static const Address NameWrite(0x19C498);

        if(entry->WasJustActivated()) {
		    SetHook(NameHook, NameWrite.addr, (u32)NameFunc, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            NameHook.Disable();
        }
    }

    void SetItemReplacementRules(MenuEntry *entry) {
        static Hook ReplaceHook;
		static const Address init(0x165528);

        if(entry->WasJustActivated()) {
		    SetHook(ReplaceHook, init.addr, (u32)IsItemReplaceable, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            ReplaceHook.Disable();
        }
    }

    void SetDropRules(MenuEntry *entry) {
        static Hook DropHook1, DropHook2, DropHook3, DropHook4;
		static const Address Drop1(0x19B66C);
		static const Address Drop2(0x19C044);
		static const Address Drop3(0x19CFF0);
		static const Address Drop4(0x2AEA64);

        if(entry->WasJustActivated()) {
            SetHook(DropHook1, Drop1.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
            SetHook(DropHook2, Drop2.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
            SetHook(DropHook3, Drop3.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
            SetHook(DropHook4, Drop4.addr, (u32)IsItemDroppable, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            DropHook1.Disable();
            DropHook2.Disable();
            DropHook3.Disable();
            DropHook4.Disable();
        }
    }

    void SetPlantRules(MenuEntry *entry) {
        static Hook PlantHook1, PlantHook2, PlantHook3, PlantHook4;
		static const Address Plant1(0x19B688);
		static const Address Plant2(0x19B97C);
		static const Address Plant3(0x2AEAC4);
		static const Address Plant4(0x2AFD24);

        if(entry->WasJustActivated()) {
            SetHook(PlantHook1, Plant1.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
            SetHook(PlantHook2, Plant2.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
            SetHook(PlantHook3, Plant3.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
            SetHook(PlantHook4, Plant4.addr, (u32)IsItemPlantable, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            PlantHook1.Disable();
            PlantHook2.Disable();
            PlantHook3.Disable();
            PlantHook4.Disable();
        }
    }

    void FixParticlesInPuzzleLeague(MenuEntry *entry) {
		static Hook ParticleHook1, ParticleHook2;
        static const Address partc1(0x5506D4);
		static const Address partc2(0x5509CC);

        if(entry->WasJustActivated()) {
            SetHook(ParticleHook1, partc1.addr, (u32)SetProperParticle, USE_LR_TO_RETURN);
            SetHook(ParticleHook2, partc2.addr, (u32)SetProperParticle, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            ParticleHook1.Disable();
            ParticleHook2.Disable();
        }
    }

    void SetCustomSpritesForSeedItemsAndProDesigns(MenuEntry *entry) {
        static Hook OnProDesignHook;
		static const Address setItemIcon(0x2B9110);

        if(entry->WasJustActivated()) {
		    SetHook(OnProDesignHook, setItemIcon.addr, (u32)SetProDesignStyle, USE_LR_TO_RETURN);
        }
        else if(!entry->IsActivated()) {
            OnProDesignHook.Disable();
        }
    }

    /*
		static Hook suspendHook;
		static const Address suspendAddress(0x124EB8);
		SetHook(suspendHook, suspendAddress.addr, (u32)SuspendCallBack, USE_LR_TO_RETURN);
		*/

		/*static Hook titleHook, textHook;
		static const Address warningTXT(0x2F319C);
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