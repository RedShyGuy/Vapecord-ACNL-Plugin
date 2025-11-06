#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/Wrapper.hpp"
#include "Color.h"
#include "Helpers/IDList.hpp"
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
//Infinite Fruit Tree
	void fruitStays(MenuEntry *entry) {
		static const Address fruitstay(0x5972CC);

		if(entry->WasJustActivated()) 
			Process::Patch(fruitstay.addr, 0xEA000000);
		else if(!entry->IsActivated())
			Process::Patch(fruitstay.addr, 0xE1A01006);
	}
//Axe Tree Shake	
	void shakechop(MenuEntry *entry) {
		static const Address shake1(0x5971D4);
		static const Address shake2(0x5971DC);
		static const Address shake3(0x5971E4);
		static const Address shake4(0x5971EC);
			
		if(entry->WasJustActivated()) {
			Process::Patch(shake1.addr, 0xE1A00000);
			Process::Patch(shake2.addr, 0xE1A00000);
			Process::Patch(shake3.addr, 0x1A00001B);
			Process::Patch(shake4.addr, 0xEA000080);	
		}
		else if(!entry->IsActivated()) {
			Process::Patch(shake1.addr, 0x0A000008);
			Process::Patch(shake2.addr, 0x0A00005B);
			Process::Patch(shake3.addr, 0x0A00001B);
			Process::Patch(shake4.addr, 0x0A000080);	
		}
    }
//Fruit Tree Item Modifier	
	void fruititemmod(MenuEntry *entry) {
		static const Address fruitmod(0x2FE6A0);
		
		static Item val = {0x2018, 0};
		if(entry->WasJustActivated()) {
			Process::Patch(fruitmod.addr, 0xE59F0020); 
            Process::Write32(fruitmod.addr + 0xC, 0xE3500000); 
			Process::Patch(fruitmod.addr + 0x28, *(u32 *)&val);
		}
		
		if(entry->Hotkeys[0].IsDown()) {
			if(Wrap::KB<u32>(Language::getInstance()->get("ENTER_ID"), true, 8, *(u32 *)&val, *(u32 *)&val, TextItemChange))
				Process::Patch(fruitmod.addr + 0x28, *(u32 *)&val);
		}
		
		if(!entry->IsActivated()) {
			Process::Patch(fruitmod.addr, 0xE59F1020);
			Process::Patch(fruitmod.addr + 0xC, 0xE1510002);
			Process::Patch(fruitmod.addr + 0x28, 0x172B);
		}
	}
//Instant Tree Chop	
	void instantchop(MenuEntry *entry) { 
		static const Address instchop(0x59945C);
		if(entry->WasJustActivated()) 
			Process::Patch(instchop.addr, 0xE1A00000); 
		else if(!entry->IsActivated()) 
			Process::Patch(instchop.addr, 0xCA000005); 
	}
}
