#include "House/Exterior.hpp"

namespace CTRPluginFramework {
    void Exterior::SetDoorArched(ACNL_TownData* town, int playerIndex, bool isArched) {
        town->PlayerHouse[playerIndex].exterior1.HouseDoorShape = !isArched; //1 = square | 0 = arched
        town->PlayerHouse[playerIndex].exterior2.HouseDoorShape = !isArched; //1 = square | 0 = arched
    }

    void Exterior::SetDoorStyle(ACNL_TownData* town, int playerIndex, u8 doorId) {
        town->PlayerHouse[playerIndex].exterior1.HouseDoor = doorId;
        town->PlayerHouse[playerIndex].exterior2.HouseDoor = doorId;
    }

    void Exterior::SetExteriorStyle(ACNL_TownData* town, int playerIndex, u8 exteriorId) {
        town->PlayerHouse[playerIndex].exterior1.HouseBrick = exteriorId;
        town->PlayerHouse[playerIndex].exterior2.HouseBrick = exteriorId;
    }

    void Exterior::SetFenceStyle(ACNL_TownData* town, int playerIndex, u8 fenceId) {
        town->PlayerHouse[playerIndex].exterior1.HouseFence = fenceId;
        town->PlayerHouse[playerIndex].exterior2.HouseFence = fenceId;
    }

    void Exterior::SetFrameworkStyle(ACNL_TownData* town, int playerIndex, u8 frameworkId) {
        town->PlayerHouse[playerIndex].exterior1.HouseStyle = frameworkId;
        town->PlayerHouse[playerIndex].exterior2.HouseStyle = frameworkId;
    }

    void Exterior::SetMailboxStyle(ACNL_TownData* town, int playerIndex, u8 mailboxId) {
        town->PlayerHouse[playerIndex].exterior1.HouseMailBox = mailboxId;
        town->PlayerHouse[playerIndex].exterior2.HouseMailBox = mailboxId;
    }

    void Exterior::SetPavementStyle(ACNL_TownData* town, int playerIndex, u8 pavementId) {
        town->PlayerHouse[playerIndex].exterior1.HousePavement = pavementId;
        town->PlayerHouse[playerIndex].exterior2.HousePavement = pavementId;
    }

    void Exterior::SetRoofStyle(ACNL_TownData* town, int playerIndex, u8 roofId) {
        town->PlayerHouse[playerIndex].exterior1.HouseRoof = roofId;
        town->PlayerHouse[playerIndex].exterior2.HouseRoof = roofId;
    }

    std::vector<ExteriorOption> Exterior::GetDoorOptions() {
        std::vector<ExteriorOption> doors;
        for (int i = 0; i < 0x15; ++i) {
            doors.push_back(ExteriorOption{ static_cast<u16>(0x3452 + i), static_cast<u8>(i) });
        }
        //Golden door missing, cause golden door only has arched variant as the other one is unused (so i am just adding the arched one as the name)
        doors.push_back(ExteriorOption{ static_cast<u16>(0x3451), 0x15 });
        return doors;
    }

    std::vector<ExteriorOption> Exterior::GetExteriorOptions() {
        std::vector<ExteriorOption> exteriors;
        for (int i = 0; i < 0x20; ++i) {
            exteriors.push_back(ExteriorOption{ static_cast<u16>(0x34A1 + i), static_cast<u8>(i) });
        }
        return exteriors;
    }

    std::vector<ExteriorOption> Exterior::GetFenceOptions() {
        std::vector<ExteriorOption> fences;
        for (int i = 0; i < 0x17; ++i) {
            fences.push_back(ExteriorOption{ static_cast<u16>(0x348A + i), static_cast<u8>(i) });
        }
        return fences;
    }

    std::vector<ExteriorOption> Exterior::GetFrameworkOptions() {
        std::vector<ExteriorOption> frameworks;
        for (int i = 0; i < 4; ++i) {
            frameworks.push_back(ExteriorOption{ static_cast<u16>(0x34CA + i), static_cast<u8>(i) });
        }
        return frameworks;
    }

    std::vector<ExteriorOption> Exterior::GetMailboxOptions() {
        std::vector<ExteriorOption> mailboxes;
        for (int i = 0; i < 0x16; ++i) {
            mailboxes.push_back(ExteriorOption{ static_cast<u16>(0x3426 + i), static_cast<u8>(i) });
        }
        return mailboxes;
    }

    std::vector<ExteriorOption> Exterior::GetPavementOptions() {
        std::vector<ExteriorOption> pavements;
        for (int i = 0; i < 9; ++i) {
            pavements.push_back(ExteriorOption{ static_cast<u16>(0x34C1 + i), static_cast<u8>(i) });
        }
        return pavements;
    }

    std::vector<ExteriorOption> Exterior::GetRoofOptions() {
        std::vector<ExteriorOption> roofs;
        for (int i = 0; i < 0x23; ++i) {
            roofs.push_back(ExteriorOption{ static_cast<u16>(0x3467 + i), static_cast<u8>(i) });
        }
        return roofs;
    }
}