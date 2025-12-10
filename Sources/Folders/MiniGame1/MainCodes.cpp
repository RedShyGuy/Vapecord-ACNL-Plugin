#include "cheats.hpp"
#include "Address/Address.hpp"
#include "Helpers/CROEditing.hpp"

namespace CTRPluginFramework {
/*
    u32 GetMiniGame1Data(u32 add) {
        static const 0x8049094; //MiniGame1(0xB09000) + 0x1FA2C
        return *(u32 *)(obj_data) + add;
    }

//count for how many lines need to be cleared (0x330A35F0)
    int GetCurrentRowRequirement(void) {
        return *(int *)GetMiniGame1Data(0x2F34);
    }
*/

    void InstantWin(MenuEntry *entry) {
        CRO::Write<u32>("MiniGame1", 0x1F880, 0xE5C4B024);
        CRO::Write<u32>("MiniGame1", 0x1F884, 0xEA00003C);

        if(!entry->IsActivated()) {
            CRO::Write<u32>("MiniGame1", 0x1F880, 0x15C4B024);
            CRO::Write<u32>("MiniGame1", 0x1F884, 0x1A00003C);
        }
    }

    //0x23C0C8 = Init MiniGame1
    //0x600B70 = Close MiniGame1
}