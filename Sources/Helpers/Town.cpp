#include "Helpers/Town.hpp"
#include "Helpers/Player.hpp"
#include "Address/Address.hpp"
#include "Helpers/NPC.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Converters.hpp"
#include "Helpers/Save.hpp"

namespace CTRPluginFramework {
	ACNL_TownData *Town::GetSaveData(void) {
		u32 *addr = (u32 *)(Address(0x2FB344).Call<u32>() + 0x53424);
		return (ACNL_TownData *)addr;
	}

	void Town::EditName(const std::string& name) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return;
		}

		u32 GardenPlus = Save::GetInstance()->Address();

		TownID oldTownID = player->PlayerInfo.TownData;
		Convert::STR_TO_U16(name, player->PlayerInfo.TownData.DataTownName);
		TownID newTownID = player->PlayerInfo.TownData;

		for(u32 addr = GardenPlus; addr < (GardenPlus + SAVE_GARDENPLUS); addr++) {
			if(*(TownID *)addr == oldTownID) {
				*(TownID *)addr = newTownID;
			}
		}
	}
}