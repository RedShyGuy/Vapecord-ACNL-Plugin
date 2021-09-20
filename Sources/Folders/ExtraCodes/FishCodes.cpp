#include "cheats.hpp"
#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
//Fish Byte Always
	void FishAlwaysBiteRightAway(MenuEntry *entry) {
		static const Address fishbite(0x1EA844, 0x1EA288, 0x1EA864, 0x1EA864, 0x1EA7A0, 0x1EA7A0, 0x1EA76C, 0x1EA76C);
		if(entry->WasJustActivated()) 
			Process::Patch(fishbite.addr, 0xE3A0005A);
		else if(!entry->IsActivated()) 
			Process::Patch(fishbite.addr, 0xE0800100);
	}
//Fish Can't Be Scared
	void FishCantBeScared(MenuEntry *entry) {
		static const Address fishscare(0x1EAB14, 0x1EA558, 0x1EAB34, 0x1EAB34, 0x1EAA70, 0x1EAA70, 0x1EAA3C, 0x1EAA3C);
		if(entry->WasJustActivated()) 
			Process::Patch(fishscare.addr, 0xE3500001);
		else if(!entry->IsActivated()) 
			Process::Patch(fishscare.addr, 0xE3500000);
	}
}
