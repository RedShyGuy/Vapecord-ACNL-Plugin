#include "Helpers/Town.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/NPC.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Converters.hpp"
#include "RegionCodes.hpp"

namespace CTRPluginFramework {
	ACNL_TownData *Town::GetSaveData(void) {
		u32 *addr = (u32 *)(Code::GardenPlus.Call<u32>() + 0x53424);
		return (ACNL_TownData *)addr;
	}

	void GetPlayerTownDataReferences(int index, std::vector<TownID *> &townIDs) {
		ACNL_Player *player = Player::GetSaveData(index);
		if(!player)
			return;

		for(int i = 0; i < 10; ++i)
			townIDs.push_back(&player->Patterns[i].CreatorData.TownData);

		townIDs.push_back(&player->PlayerInfo.TownData);
		townIDs.push_back(&player->TownData2);

		for(int i = 0; i < 10; ++i)
			townIDs.push_back(&player->Letters[i].Reciever.TownData);

		townIDs.push_back(&player->UnkBuffer1.TownData);

		townIDs.push_back(&player->UnkBuffer6.Mini1.TownData1);
		townIDs.push_back(&player->UnkBuffer6.Mini1.TownData2);
		townIDs.push_back(&player->UnkBuffer6.Mini2.TownData1);
		townIDs.push_back(&player->UnkBuffer6.Mini2.TownData2);
	}

	void GetVillagerTownDataReferences(std::vector<TownID *> &townIDs) {
		ACNL_VillagerData *villager = NPC::GetSaveData();
		if(!villager)
			return;

		for(int i = 0; i < 10; ++i) {
			townIDs.push_back(&villager->Villager[i].Mini1.TownData1);
			townIDs.push_back(&villager->Villager[i].Mini1.TownData2);

			townIDs.push_back(&villager->Villager[i].Pattern.CreatorData.TownData);

			townIDs.push_back(&villager->Villager[i].TownID1);

			for(int j = 0; j < 16; ++j) {
				townIDs.push_back(&villager->Villager[i].Home[j].unknownPID.TownData);
				townIDs.push_back(&villager->Villager[i].Home[j].UnkTownID);
			}

			for(int g = 0; g < 5; ++g) {
				townIDs.push_back(&villager->Villager[i].Letter[g].Reciever.TownData);
			}
		}

		townIDs.push_back(&villager->townID1);
		townIDs.push_back(&villager->townID2);
	}

	void GetBuildingTownDataReferences(std::vector<TownID *> &townIDs) {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building)
			return;

		townIDs.push_back(&building->UnkPattern1.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern2.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern3.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern4.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern5.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern6.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern7.CreatorData.TownData);
		townIDs.push_back(&building->UnkPattern8.CreatorData.TownData);
	}

	void GetTownTownDataReferences(std::vector<TownID *> &townIDs) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town)
			return;

		townIDs.push_back(&town->TownData1);

		for(int i = 0; i < 8; ++i) 
			townIDs.push_back(&town->AbleDisplayPattern[i].CreatorData.TownData);
	
		for(int i = 0; i < 4; ++i) 
			townIDs.push_back(&town->UnknownPID1[i].TownData);

		for(int i = 0; i < 8; ++i)
			townIDs.push_back(&town->UnknownPattern[i].CreatorData.TownData);

		for(int i = 0; i < 4; ++i)
			townIDs.push_back(&town->Exhibit[i].UnkPattern1.CreatorData.TownData);

		for(int i = 0; i < 3; ++i)
			townIDs.push_back(&town->UnknownPID2[i].TownData);

		townIDs.push_back(&town->TownFlag.CreatorData.TownData);

		for(int i = 0; i < 10; ++i) {
			townIDs.push_back(&town->Player1Letters[i].Reciever.TownData);
			townIDs.push_back(&town->Player2Letters[i].Reciever.TownData);
			townIDs.push_back(&town->Player3Letters[i].Reciever.TownData);
			townIDs.push_back(&town->Player4Letters[i].Reciever.TownData);
		}
	}

	void WrapTownDataReferences(std::vector<TownID *> &townIDs) {
		townIDs.clear();

	//player struct references
		for(int i = 0; i < 4; ++i)
			GetPlayerTownDataReferences(i, townIDs);

		GetVillagerTownDataReferences(townIDs);

		GetBuildingTownDataReferences(townIDs);

		GetTownTownDataReferences(townIDs);
	}

	void Town::EditName(const std::string& name) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player)
			return;

		std::vector<TownID *> townIDs;
		WrapTownDataReferences(townIDs);

		TownID oldTownID = player->PlayerInfo.TownData;
		Convert::STR_TO_U16(name, player->PlayerInfo.TownData.DataTownName);
		TownID newTownID = player->PlayerInfo.TownData;

		for(int i = 0; i < townIDs.size(); ++i) {
			if(*(TownID *)townIDs[i] == oldTownID) 
				*(TownID *)townIDs[i] = newTownID;
		}
	}
}