#include "Helpers/House.hpp"
#include "Helpers/Save.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
    namespace House {
        void LoadUpgradedRooms() {
            static const u8 mapPlayerRoomToPlayerIndex[26] = {
				0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1,
				2, 2, 2, 2, 2, 2,
				3, 3, 3, 3, 3, 3,
			};

			static const u8 mapPlayerRoomIndexToPlayerIndex[26] = {
				0, 0, 0, 1, 2, 3, 4, 5,
				0, 1, 2, 3, 4, 5,
				0, 1, 2, 3, 4, 5,
				0, 1, 2, 3, 4, 5,
			};

			for (u8 playerRoom = 2; playerRoom < 0x1A; ++playerRoom) {
                RoomData* roomData = GetRoomData(playerRoom);
				if (roomData) {
                    u32 data0 = Address(0x316F40).Call<u32>(roomData, 0, 0);
                    if (data0 != 0) {
                        u32 playerIndexByRoom = mapPlayerRoomToPlayerIndex[playerRoom];

					    u32 houseSaveOffset = Save::GetInstance()->Address(0x5D900);
                        u32 houseSaveFromGivenPlayerOffset = Address(0x30F770).Call<u32>(houseSaveOffset, playerIndexByRoom);

                        u32 playerRoomIndex = mapPlayerRoomIndexToPlayerIndex[playerRoom];
                        u32 roomSize = Address(0x6FB9E8).Call<u32>(houseSaveFromGivenPlayerOffset, playerRoomIndex); //get room "size"

                        Address(0x30CB54).Call<void>(data0, roomSize);
                        
                        Address(0x753938).Call<u32>(roomData, 0, 0);
                    }
				}
			}
        }

        void FinishedAllUpgrades(ACNL_Player* player) {
            player->PlayerFlags.Unknown117 = 1; //probably finished all debts
        }

        bool IsLeftRoomBuilt(ACNL_TownData* town, int playerIndex) {
            return town->PlayerHouse[playerIndex].LeftRoom.flags.RoomSize >= 2;
        }

        bool IsRightRoomBuilt(ACNL_TownData* town, int playerIndex) {
            return town->PlayerHouse[playerIndex].RightRoom.flags.RoomSize >= 2;
        }

        bool IsMiddleRoomFinished(ACNL_TownData* town, int playerIndex) {
            return town->PlayerHouse[playerIndex].MiddleRoom.flags.RoomSize == 4;
        }

        bool IsSecondRoomBuilt(ACNL_TownData* town, int playerIndex) {
            return town->PlayerHouse[playerIndex].SecondRoom.flags.RoomSize >= 2;
        }

        bool IsHouseFinished(ACNL_TownData* town, int playerIndex) {
            bool leftRoomFinished = town->PlayerHouse[playerIndex].LeftRoom.flags.RoomSize == 4;
            bool rightRoomFinished = town->PlayerHouse[playerIndex].RightRoom.flags.RoomSize == 4;
            bool backRoomFinished = town->PlayerHouse[playerIndex].BackRoom.flags.RoomSize == 4;
            bool basementRoomFinished = town->PlayerHouse[playerIndex].BasementRoom.flags.RoomSize == 4;
            bool secondRoomFinished = town->PlayerHouse[playerIndex].SecondRoom.flags.RoomSize == 4;
            bool mainRoomFinished = IsMiddleRoomFinished(town, playerIndex);

            return leftRoomFinished && rightRoomFinished && backRoomFinished && basementRoomFinished && secondRoomFinished && mainRoomFinished;
        }

        bool IsHouseBuilt(ACNL_TownData* town, int playerIndex) {
            return town->PlayerHouse[playerIndex].exterior1.HouseSize > 0 && town->PlayerHouse[playerIndex].exterior2.HouseSize > 0;
        }

        void BuildSecretStorage(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to build secret storage
            player->PlayerFlags.SecretStorageExplained = 1;
            player->PlayerFlags.BuildingSecretStorage = 1; //wants to build secret storage
        //Next day -> Upgrade was made
            player->PlayerFlags.BuildingSecretStorage = 0;
            player->PlayerFlags.UnlockedSecretStorage = 1;

            player->PlayerFlags.HouseUpgradeFinished = 1;

        //Gets set when you talk to tom nook after your secret storage was built
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //Talking to lottie
            player->PlayerFlags.Unknown382 = 1;
        //finished house editor tutorial
            player->PlayerFlags.UnlockedHouseEditor = 1;
        //pay loan
            Game::EncryptValue(&player->DebtAmount, 0);
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;
            player->PlayerFlags.Unknown382 = 0;

            /*
            TODO: Check out how to load the furniture editing tutorial with lottie to ask the player if he wants to load it
            */
        }

        void BuildHouse(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //paying tent payment
            player->PlayerFlags.TomNookToldYouFirstPayment = 1;
            player->PlayerFlags.TomNookDownFirstPayment = 1;

            town->PlayerHouse[playerIndex].exterior2.HouseSize = 1;
            town->PlayerHouse[playerIndex].exterior2.HouseBrick = 0;
            town->PlayerHouse[playerIndex].exterior2.HouseRoof = 0; //Red Roof = 0x00 || Blue Roof = 0x20 ||Yellow Roof = 0x21 ||Green Roof = 0x22
            town->PlayerHouse[playerIndex].exterior2.HouseDoor = 0;
            town->PlayerHouse[playerIndex].exterior2.HouseFence = 1;
            town->PlayerHouse[playerIndex].exterior2.HouseMailBox = 0;

            town->PlayerHouse[playerIndex].MiddleRoom.flags.IsRoomUpgrading = 1;
        //Next day -> House was built
            player->PlayerFlags.Unknown95 = 1;
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->TownFlags.HHSStandUnlocked = 1;
            town->TownFlags.Unknown10 = 1; //unsure what this is
            town->TownFlags.Unknown112 = 1; //unsure what this is

            town->PlayerHouse[playerIndex].MiddleRoom.flags.RoomSize = 2;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.IsRoomUpgrading = 0;

            town->PlayerHouse[playerIndex].exterior1.HouseSize = 1;
            town->PlayerHouse[playerIndex].exterior1.HouseBrick = 0;
            town->PlayerHouse[playerIndex].exterior1.HouseRoof = 0;
            town->PlayerHouse[playerIndex].exterior1.HouseDoor = 0;
            town->PlayerHouse[playerIndex].exterior1.HouseFence = 1;
            town->PlayerHouse[playerIndex].exterior1.HouseMailBox = 0;
        //Gets set when you talk to tom nook after your house was built
            player->PlayerFlags.HouseUpgradeFinished = 0;
            player->PlayerFlags.Unknown95 = 0;
            
            player->PlayerFlags.Unknown140 = 1;
            player->PlayerFlags.Unknown149 = 1;
        //pay loan
            Game::EncryptValue(&player->DebtAmount, 0);
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
            player->PlayerFlags.Unknown43 = 1;
            player->PlayerFlags.Unknown156 = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeMainRoomBarrier0State(playerIndex);
        }

        void UpgradeHouseFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Gets set when you enter Tom Nooks after your house was upgraded -> UpgradeHouseFirst (Should know him by now)
            player->PlayerFlags.LyleIntroduction = 1;    
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].exterior2.HouseSize = 2;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;
            player->PlayerFlags.Unknown149 = 0;
            player->PlayerFlags.Unknown150 = 1;
            player->PlayerFlags.Unknown157 = 1;

            town->PlayerHouse[playerIndex].exterior1.HouseSize = 2;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.RoomSize = 3;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //pay loan
            Game::EncryptValue(&player->DebtAmount, 0);
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeMainRoomBarrier1State(playerIndex);
        }

        void UpgradeHouseSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].exterior2.HouseSize = 3;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;
        
            town->PlayerHouse[playerIndex].exterior1.HouseSize = 3;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.RoomSize = 4;
            town->PlayerHouse[playerIndex].MiddleRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //pay loan
            Game::EncryptValue(&player->DebtAmount, 0);
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;	
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            if (IsHouseFinished(town, playerIndex)) {
                FinishedAllUpgrades(player);
            }

            LoadUpgradedRooms();
            Barrier::UpgradeMainRoomBarrier2State(playerIndex);
        }

        void BuildSecondRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].exterior2.HouseSize = 4;
            town->PlayerHouse[playerIndex].SecondRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].exterior1.HouseSize = 4;
            town->PlayerHouse[playerIndex].SecondRoom.flags.RoomSize = 2;
            town->PlayerHouse[playerIndex].SecondRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
            player->PlayerFlags.Unknown305 = 1;
        //pay loan
            Game::EncryptValue(&player->DebtAmount, 0);
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeBarrierUpstairs(playerIndex);
        }

        void UpgradeSecondRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].SecondRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].SecondRoom.flags.RoomSize = 3;
            town->PlayerHouse[playerIndex].SecondRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;
            
            LoadUpgradedRooms();
            Barrier::UpgradeUpstairsBarrier1State(playerIndex);
        }
        
        void UpgradeSecondRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].SecondRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].SecondRoom.flags.RoomSize = 4;
            town->PlayerHouse[playerIndex].SecondRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            if (IsHouseFinished(town, playerIndex)) {
                FinishedAllUpgrades(player);
            }

            LoadUpgradedRooms();
            Barrier::UpgradeUpstairsBarrier2State(playerIndex);
        }

        void BuildBasementRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].BasementRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].BasementRoom.flags.RoomSize = 2;
            town->PlayerHouse[playerIndex].BasementRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeBarrierDownstairs(playerIndex);
        }

        void UpgradeBasementRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].BasementRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].BasementRoom.flags.RoomSize = 3;
            town->PlayerHouse[playerIndex].BasementRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeDownStairsBarrier1State(playerIndex);
        }

        void UpgradeBasementRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].BasementRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].BasementRoom.flags.RoomSize = 4;
            town->PlayerHouse[playerIndex].BasementRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            if (IsHouseFinished(town, playerIndex)) {
                FinishedAllUpgrades(player);
            }

            LoadUpgradedRooms();
            Barrier::UpgradeDownStairsBarrier2State(playerIndex);
        }

        void BuildRightRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Sets proper house size
            u8 houseSize = 0;
            bool exteriorRenovationsUnlocked = false;
            if (IsLeftRoomBuilt(town, playerIndex)) {
                houseSize = 7; //Both are built
                exteriorRenovationsUnlocked = true;
            } else {
                houseSize = 5; //Only right room is/will be built
            }

        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].exterior2.HouseSize = houseSize;
            town->PlayerHouse[playerIndex].RightRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;
            player->PlayerFlags.ExteriorRenovationsUnlocked = exteriorRenovationsUnlocked;
            
            town->PlayerHouse[playerIndex].exterior1.HouseSize = houseSize;
            town->PlayerHouse[playerIndex].RightRoom.flags.RoomSize = 2;
            town->PlayerHouse[playerIndex].RightRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeBarrierRightRoom(playerIndex);
        }

        void UpgradeRightRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].RightRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].RightRoom.flags.RoomSize = 3;
            town->PlayerHouse[playerIndex].RightRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeRightRoomBarrier1State(playerIndex);
        }

        void UpgradeRightRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].RightRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].RightRoom.flags.RoomSize = 4;
            town->PlayerHouse[playerIndex].RightRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            if (IsHouseFinished(town, playerIndex)) {
                FinishedAllUpgrades(player);
            }

            LoadUpgradedRooms();
            Barrier::UpgradeRightRoomBarrier2State(playerIndex);
        }

        void BuildLeftRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Sets proper house size
            u8 houseSize = 0;
            bool exteriorRenovationsUnlocked = false;
            if (IsRightRoomBuilt(town, playerIndex)) {
                houseSize = 7; //Both are built
                exteriorRenovationsUnlocked = true;
            } else {
                houseSize = 6; //Only left room is/will be built
            }

        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].exterior2.HouseSize = houseSize;
            town->PlayerHouse[playerIndex].LeftRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.ExteriorRenovationsUnlocked = exteriorRenovationsUnlocked;
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].exterior1.HouseSize = houseSize;
            town->PlayerHouse[playerIndex].LeftRoom.flags.RoomSize = 2;
            town->PlayerHouse[playerIndex].LeftRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeBarrierLeftRoom(playerIndex);
        }

        void UpgradeLeftRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].LeftRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].LeftRoom.flags.RoomSize = 3;
            town->PlayerHouse[playerIndex].LeftRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeLeftRoomBarrier1State(playerIndex);
        }

        void UpgradeLeftRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].LeftRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].LeftRoom.flags.RoomSize = 4;
            town->PlayerHouse[playerIndex].LeftRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            if (IsHouseFinished(town, playerIndex)) {
                FinishedAllUpgrades(player);
            }

            LoadUpgradedRooms();
            Barrier::UpgradeLeftRoomBarrier2State(playerIndex);
        }

        void BuildBackRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].BackRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].BackRoom.flags.RoomSize = 2;
            town->PlayerHouse[playerIndex].BackRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeBarrierTopRoom(playerIndex);
        }

        void UpgradeBackRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].BackRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].BackRoom.flags.RoomSize = 3;
            town->PlayerHouse[playerIndex].BackRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            LoadUpgradedRooms();
            Barrier::UpgradeTopRoomBarrier1State(playerIndex);
        }

        void UpgradeBackRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex) {
        //Tell Tom Nook you want to expand
            town->PlayerHouse[playerIndex].BackRoom.flags.IsRoomUpgrading = 1;
        //Next day -> Upgrade was made
            player->PlayerFlags.HouseUpgradeFinished = 1;

            town->PlayerHouse[playerIndex].BackRoom.flags.RoomSize = 4;
            town->PlayerHouse[playerIndex].BackRoom.flags.IsRoomUpgrading = 0;
        //Gets set when you talk to tom nook after your house was upgraded
            player->PlayerFlags.HouseUpgradeFinished = 0;
        //after repaying loan in post office
            player->PlayerFlags.HouseLoanRepayed = 1;
        //Talking to tom nook to let him know you payed your loan
            player->PlayerFlags.HouseLoanRepayed = 0;

            if (IsHouseFinished(town, playerIndex)) {
                FinishedAllUpgrades(player);
            }

            LoadUpgradedRooms();
            Barrier::UpgradeTopRoomBarrier2State(playerIndex);
        }

        RoomData* GetRoomData(u8 playerRoom) {
            static Address getRoomAddr(0x2FE95C);
            return getRoomAddr.Call<RoomData*>(playerRoom); //unsure why its shifted by one
        }
    }
}