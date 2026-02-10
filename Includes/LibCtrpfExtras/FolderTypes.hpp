#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    enum class FolderType : u8 {
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
        Misc,
        Default,
        Dev,
        None
    };

    enum class SubFolder : u8 {
        PlayerSave = 0,
        Seed,
        Drop,
        Tree,
        Fish,
        Insect,
        Chat,
        Fun,
        None
    };
}