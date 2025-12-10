#include "cheats.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
//Fish Byte Always
	void FishAlwaysBiteRightAway(MenuEntry *entry) {
		static Address fishbite(0x1EA844);

		if(entry->WasJustActivated()) {
			fishbite.Patch(0xE3A0005A);
		}
		else if(!entry->IsActivated()) {
			fishbite.Unpatch();
		}
	}
//Fish Can't Be Scared
	void FishCantBeScared(MenuEntry *entry) {
		static Address fishscare(0x1EAB14);

		if(entry->WasJustActivated()) {
			fishscare.Patch(0xE3500001);
		}
		else if(!entry->IsActivated()) {
			fishscare.Unpatch();
		}
	}
}
