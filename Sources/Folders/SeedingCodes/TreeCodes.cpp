#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/Wrapper.hpp"
#include "Color.h"
#include "Helpers/IDList.hpp"
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
//Infinite Fruit Tree
	void fruitStays(MenuEntry *entry) {
		static Address fruitstay(0x5972CC);

		if(entry->WasJustActivated()) {
			fruitstay.Patch(0xEA000000);
		}
		else if(!entry->IsActivated()) {
			fruitstay.Unpatch();
		}
	}
//Axe Tree Shake	
	void shakechop(MenuEntry *entry) {
		static Address shake1(0x5971D4);
		static Address shake2(0x5971DC);
		static Address shake3(0x5971E4);
		static Address shake4(0x5971EC);
		
		if(entry->WasJustActivated()) {
			shake1.Patch(0xE1A00000);
			shake2.Patch(0xE1A00000);
			shake3.Patch(0x1A00001B);
			shake4.Patch(0xEA000080);
		}
		else if(!entry->IsActivated()) {
			shake1.Unpatch();
			shake2.Unpatch();
			shake3.Unpatch();
			shake4.Unpatch();
		}
    }
//Fruit Tree Item Modifier	
	void fruititemmod(MenuEntry *entry) {
		static Address fruitmod(0x2FE6A0);
		static Address fruitmod2 = fruitmod.MoveOffset(0xC);
		static Address fruitmod3 = fruitmod.MoveOffset(0x28);
		
		static Item val = {0x2018, 0};

		if(entry->WasJustActivated()) {
			fruitmod.Patch(0xE59F0020);
			fruitmod2.Patch(0xE3500000);
			fruitmod3.Patch(*(u32 *)&val);
		}
		
		if(entry->Hotkeys[0].IsDown()) {
			if(Wrap::KB<u32>(Language::getInstance()->get("ENTER_ID"), true, 8, *(u32 *)&val, *(u32 *)&val, TextItemChange)) {
				fruitmod3.Patch(*(u32 *)&val);
			}
		}
		
		if(!entry->IsActivated()) {
			fruitmod.Unpatch();
			fruitmod2.Unpatch();
			fruitmod3.Unpatch();
		}
	}
//Instant Tree Chop	
	void instantchop(MenuEntry *entry) { 
		static Address instchop(0x59945C);
		if(entry->WasJustActivated()) {
			instchop.Patch(0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			instchop.Unpatch();
		}
	}
}
