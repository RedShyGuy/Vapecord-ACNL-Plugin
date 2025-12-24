#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"
#include "Helpers/Game.hpp"

#pragma pack(push, 1)

namespace CTRPluginFramework {
    struct ExteriorOption {
		u16 rawItemId;
		u8 exteriorId;
	};

    namespace Exterior {
        void SetDoorArched(ACNL_TownData* town, int playerIndex, bool isArched);
        void SetDoorStyle(ACNL_TownData* town, int playerIndex, u8 doorId);
        void SetExteriorStyle(ACNL_TownData* town, int playerIndex, u8 exteriorId);
        void SetFenceStyle(ACNL_TownData* town, int playerIndex, u8 fenceId);
        void SetFrameworkStyle(ACNL_TownData* town, int playerIndex, u8 frameworkId);
        void SetMailboxStyle(ACNL_TownData* town, int playerIndex, u8 mailboxId);
        void SetPavementStyle(ACNL_TownData* town, int playerIndex, u8 pavementId);
        void SetRoofStyle(ACNL_TownData* town, int playerIndex, u8 roofId);

        std::vector<ExteriorOption> GetDoorOptions();
        std::vector<ExteriorOption> GetExteriorOptions();
        std::vector<ExteriorOption> GetFenceOptions();
        std::vector<ExteriorOption> GetFrameworkOptions();
        std::vector<ExteriorOption> GetMailboxOptions();
        std::vector<ExteriorOption> GetPavementOptions();
        std::vector<ExteriorOption> GetRoofOptions();
    }
}

#pragma pack(pop)