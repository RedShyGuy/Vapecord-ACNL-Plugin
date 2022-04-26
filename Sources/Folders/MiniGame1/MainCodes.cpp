#include "cheats.hpp"
#include "TextFileParser.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/CROEditing.hpp"

namespace CTRPluginFramework {
    void InstantWin(MenuEntry *entry) {
        CRO::Write<u32>("MiniGame1", 0x1F880, 0xE5C4B024);
        CRO::Write<u32>("MiniGame1", 0x1F884, 0xEA00003C);

        if(!entry->IsActivated()) {
            CRO::Write<u32>("MiniGame1", 0x1F880, 0x15C4B024);
            CRO::Write<u32>("MiniGame1", 0x1F884, 0x1A00003C);
        }
    }

    
}