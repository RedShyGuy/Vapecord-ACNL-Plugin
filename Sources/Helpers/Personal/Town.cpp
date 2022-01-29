#include "Helpers/Town.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/Player.hpp"

namespace CTRPluginFramework {
	void TownName::UpdateReference(const std::string& tName, u8 tID1, u8 tID2) {
		u32 pAddress = Player::GetSaveOffset(4);
		if(pAddress == 0)
			return;
		
	//search all town ID references
		u32 tData[5], tCheck[5];
		Process::CopyMemory((void *)tData, (void *)(pAddress + 0x55BC), 20);

		for(u32 addr = Save::GetInstance()->Address(); addr < Save::GetInstance()->Address(SAVE_GARDENPLUS); addr += 2) {
			Process::CopyMemory((void *)tCheck, (void *)addr, 20);
			if(!std::equal(std::begin(tData), std::end(tData), std::begin(tCheck)))
				continue;

			if(tID1 != -1) {
				Process::Write8(addr, tID1);
				Process::Write8(addr + 1, tID2);
			}

			if(!tName.empty()) 
				Process::WriteString(addr + 2, tName, 16, StringFormat::Utf16);

			addr += 20;
		}
	}

	void TownName::Set(const std::string& name) {
		TownName::UpdateReference(name, -1);
	}
}