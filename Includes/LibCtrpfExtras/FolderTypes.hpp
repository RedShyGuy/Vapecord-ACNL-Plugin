#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    enum FolderType : u8 {
        Save = 0,
        Movement,
        Inventory,
        Player,
        Animation,
        Seeding,
        Money,
        Island,
        NPC,
        Environment,
        Extra,
        Misc
    };
}