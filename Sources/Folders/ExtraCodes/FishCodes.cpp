#include "cheats.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
//Fish Byte Always
	void FishAlwaysBiteRightAway(MenuEntry *entry) {
		static const Address fishbite("FISHBITE");
		if(entry->WasJustActivated()) 
			Process::Patch(fishbite.addr, 0xE3A0005A);
		else if(!entry->IsActivated()) 
			Process::Patch(fishbite.addr, 0xE0800100);
	}
//Fish Can't Be Scared
	void FishCantBeScared(MenuEntry *entry) {
		static const Address fishscare("FISHSCARE");
		if(entry->WasJustActivated()) 
			Process::Patch(fishscare.addr, 0xE3500001);
		else if(!entry->IsActivated()) 
			Process::Patch(fishscare.addr, 0xE3500000);
	}
}
