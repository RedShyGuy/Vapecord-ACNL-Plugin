#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"
#include "Helpers/Game.hpp"

#pragma pack(push, 1)

namespace CTRPluginFramework {
    namespace House {
        static const u8 MAIN_ROOM[4] = { 3, 9, 0xF, 0x15 };
        static const u8 UPSTAIRS[4] = { 4, 0xA, 0x10, 0x16 };
        static const u8 BASEMENT[4] = { 5, 0xB, 0x11, 0x17 };
        static const u8 RIGHT_ROOM[4] = { 6, 0xC, 0x12, 0x18 };
        static const u8 LEFT_ROOM[4] = { 7, 0xD, 0x13, 0x19 };
        static const u8 TOP_ROOM[4] = { 8, 0xE, 0x14, 0x1A };

        enum BarrierToUpgrade {
            State0 = 0,
            State1,
            State2,
            Upstairs,
            Left,
            Right,
            Top,
            Basement
        };

        //For First Room: 0x3106BA00 - 0x3106CCD0
        struct RoomData { //Size: 0x12D0
            u8 unk0[24]; //0 //Usually 0
            u32 unk1[2]; //0x18 //Usually 0xF
            u32 unk2; //0x20 //Jump to 0x50
            u32 unk3; //0x24 //Jump to 0x74
            u32 unk4[2]; //0x28 //Usually 0
            u32 unk5; //0x30 //Jump to 0xAA8
            u32 unk6; //0x34 //Jump to 0xEC0
            u32 unk7; //0x38 //Jump to -0x10 ?
            u32 unk8; //0x3C //Jump to 0x5C
            u32 unk9; //0x40 //Usually 9
            u32 unk10; //0x44 //Usually 0x14
            u32 unk11; //0x48 //Usually 0xC
            u32 unk12; //0x4C //Usually 1
            u32 visualSize; //0x50
            u32 unk14; //0x54 //Jump to 0xAA8
            u32 unk15; //0x58 //Jump to 0xEC0
            u32 unk16; //0x5C //Jump to 0x38
            u32 unk17; //0x60 //Jump to 0xA90
            u32 unk18; //0x64 //Usually 9
            u32 unk19; //0x68 //Usually 0xA24
            u32 unk20; //0x6C //Usually 0xA1C
            u32 unk21; //0x70 //Usually 1
            u32 unk22[7]; //0x74 //Usually 0
            u16 barrier[0x500]; //0x90 //Size of 0xA00 bytes
            u32 unk24; //0xA90 //Jump to 0x5C
            u32 unk25; //0xA94 //Jump to 0xEA8
            u32 unk26; //0xA98 //Usually 9
            u32 unk27; //0xA9C //Usually 0x408
            u32 unk28; //0xAA0 //Usually 0x400
            u32 unk29; //0xAA4 //Usually 1
            Item furnitureInRoom[256]; //0xAA8 //Array of furniture placed in room
            u32 unk31; //0xEA8 //Jump to 0xA90
            u32 unk32; //0xEAC //Jump to 0x12C0
            u32 unk33; //0xEB0 //Usually 9
            u32 unk34; //0xEB4 //Usually 0x408
            u32 unk35; //0xEB8 //Usually 0x400
            u32 unk36; //0xEBC //Usually 1
            Item furnitureOnFurniture[256]; //0xEC0 //Array of furniture placed on other furniture
            u32 unk38; //0x12C0 //Jump to 0xEA8
            u32 unk39; //0x12C4 //Jump to 0x1308?
            u32 unk40; //0x12C8 //Usually 9
            u32 unk41; //0x12CC //Usually 0x38
        };

        namespace Barrier {
            void UpgradeMainRoomBarrier0State(int playerIndex);
            void UpgradeMainRoomBarrier1State(int playerIndex);
            void UpgradeMainRoomBarrier2State(int playerIndex);
           
            void UpgradeRightRoomBarrier1State(int playerIndex);
            void UpgradeRightRoomBarrier2State(int playerIndex);

            void UpgradeLeftRoomBarrier1State(int playerIndex);
            void UpgradeLeftRoomBarrier2State(int playerIndex);

            void UpgradeTopRoomBarrier1State(int playerIndex);
            void UpgradeTopRoomBarrier2State(int playerIndex);

            void UpgradeDownStairsBarrier1State(int playerIndex);
            void UpgradeDownStairsBarrier2State(int playerIndex);

            void UpgradeUpstairsBarrier1State(int playerIndex);
            void UpgradeUpstairsBarrier2State(int playerIndex);

            /*
            Sets the barrier for the stairs that go upstairs (main room)
            */
            void UpgradeBarrierUpstairs(int playerIndex);
            /*
            Sets the barrier for the stairs that go downstairs (main room)
            */
            void UpgradeBarrierDownstairs(int playerIndex);
            /*
            Sets the barrier for the path that goes to the left room (main room)
            */
            void UpgradeBarrierLeftRoom(int playerIndex);
            /*
            Sets the barrier for the path that goes to the right room (main room)
            */
            void UpgradeBarrierRightRoom(int playerIndex);
            /*
            Sets the barrier for the path that goes to the top room (main room)
            */
            void UpgradeBarrierTopRoom(int playerIndex);
        }

        void BuildSecretStorage(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void BuildHouse(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeHouseFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeHouseSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void BuildSecondRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeSecondRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeSecondRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void BuildBasementRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeBasementRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeBasementRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void BuildRightRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeRightRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeRightRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void BuildLeftRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeLeftRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeLeftRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void BuildBackRoom(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeBackRoomFirst(ACNL_Player* player, ACNL_TownData* town, int playerIndex);
        void UpgradeBackRoomSecond(ACNL_Player* player, ACNL_TownData* town, int playerIndex);

        void FinishedAllUpgrades(ACNL_Player* player);
        bool IsHouseBuilt(ACNL_TownData* town, int playerIndex);

        bool IsMiddleRoomFinished(ACNL_TownData* town, int playerIndex);
        bool IsSecondRoomBuilt(ACNL_TownData* town, int playerIndex);

        RoomData* GetRoomData(u8 playerRoom);
    }
}

#pragma pack(pop)