#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

u8 FruitCount = 0;

extern "C" bool __IsFruit(u32 ItemID) {
	return CTRPluginFramework::IDList::ValidID(*(u32 *)ItemID, 0x2001, 0x2012);
}

extern "C" void __AddFruit(void) {
	if(FruitCount <= 10)
		FruitCount++;
}

extern "C" void CHECK_IfFruit(void);

namespace CTRPluginFramework {
	/*
	ACNHCheats are currently unused
	*/

    u32 itembury = 0;
	
	/*bool IfFlagOrTree(void) {
	//If item is a tree, write item model and name correctly and return 1 so its digable
		if((*(u16 *)itembury >= 0x0007 && *(u16 *)itembury <= 0x007B)) {
			Process::Write32(0x67A2F4, *(u32 *)itembury);
			
			Process::Write32(i + 0x1F5CC, 0x000E000E);
			Process::Write32(i + 0x1F5D0, 0x00040000);
			Process::Write32(i + 0x1F5D4, 0xCD030100);
			Process::WriteString(i + 0x1F5D8, IDList::GetSeedName(*(u32 *)itembury), StringFormat::Utf16);
			
		//doesnt work yet, should write tree to inv :/
			GameHelper::SetItem(&(*(u32 *)itembury));
			FruitCount--;
			return 1;
		}
	}*/
	
	int RandomCrystall(void) {
		FruitCount--;
		return Utils::Random(0, 5); //returns random crystal
	}

    bool IsTree(u32 ItemIDAddress) {
        if(FruitCount > 0) {
            if(IDList::ValidID(*(u16 *)ItemIDAddress, 7, 0x7B)) {
                FruitCount--;
                return 1;
            }
        }
        return *(bool *)(ItemIDAddress + 2) >> 0xF;
    }
	
	bool FruitDisplay(const Screen &screen) {	
		if(!PlayerClass::GetInstance()->IsLoaded())
			return 0;
		
		if(screen.IsTop) {	
			screen.DrawRect(0, 0, 85, 18, Color(0xF0E68CFF));
			screen.DrawSysfont(Utils::Format("Fruit: %d|10", FruitCount), 0, 0, Color::Black);
		}	
		return 1;
	}	

    static bool WasEnabled = false;

//acnh cheat
	void acnh(MenuEntry *entry) {
		/*static Hook hookTree;
		static Hook hookTreeBury;
		
		if(!PlayerClass::GetInstance()->IsLoaded())
			return;
		
		//if anim is bury check if tree
		if(*PlayerClass::GetInstance()->GetAnimation() == 0x53 && (*(u16 *)itembury >= 7 && *(u16 *)itembury <= 0x7B)) 
			Process::Write32(0x9B5268, *(u16 *)itembury);
		
		if(FruitCount != 0) {			
			//enables patch so item model gets showed
			Process::Write32(0x676214, 0xE3A01050);
		}
		else {	
			//enables patch so item model gets showed
			Process::Write32(0x676214, 0xE3A0104F);
		}
		
		if(!entry->IsActivated()) {
			//enables patch so item model gets showed
			Process::Write32(0x676214, 0xE3A0104F);
		}*/

		static Hook fruitHook, buryHook, crystallHook;

		if(entry->WasJustActivated()) {
			OSD::Run(FruitDisplay);

			fruitHook.Initialize(0x650ED4, (u32)CHECK_IfFruit);
			fruitHook.SetFlags(USE_LR_TO_RETURN);
			fruitHook.Enable(); //Makes counter go up if fruit eaten

            buryHook.Initialize(0x599980, (u32)IsTree);
            buryHook.SetFlags(USE_LR_TO_RETURN);

            crystallHook.Initialize(0x599AB0, (u32)RandomCrystall);
            crystallHook.SetFlags(USE_LR_TO_RETURN);
		}

        if(FruitCount > 0 && !WasEnabled) {
            Process::Patch(0x599AA0, 0xE1A00000); //All Rocks breakable
            buryHook.Enable(); //All Trees digable
            crystallHook.Enable(); //Random Crystall when rock brocken
            WasEnabled = true;
        }
        else if(FruitCount <= 0 && WasEnabled) {
            Process::Patch(0x599AA0, 0x0A000022);
            buryHook.Disable();
            crystallHook.Disable();
            WasEnabled = false;
        }

		if(!entry->IsActivated()) {
			OSD::Stop(FruitDisplay);

            buryHook.Disable();
			fruitHook.Disable();
            crystallHook.Disable();

            Process::Patch(0x599AA0, 0x0A000022);
		}
	}
}