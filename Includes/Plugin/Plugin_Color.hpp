#pragma once

#include <CTRPluginFramework.hpp>
#include "LibCtrpfExtras/FolderTypes.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
    #pragma pack(push, 1)
    struct ColorEntry {
		FolderType folderType;
		u8 r;
		u8 g;
		u8 b;
	};
    #pragma pack(pop)

    static const std::vector<ColorEntry> defaultColors = {
        { FolderType::Save, 0x78, 0x50, 0xC8 }, // Deep purple
        { FolderType::Movement, 0x3C, 0xC8, 0x64 }, // Green
        { FolderType::Inventory, 0xC8, 0x8C, 0x3C }, // Brown-orange
        { FolderType::Player, 0x46, 0x82, 0xFF }, // Blue
        { FolderType::Animation, 0xFF, 0x5A, 0x96 }, // Pink
        { FolderType::Seeding, 0x5A, 0xAA, 0x5A }, // Natural green
        { FolderType::Money, 0xFF, 0xC8, 0x46 }, // Gold
        { FolderType::Island, 0x32, 0xC8, 0xB4 }, // Turquoise
        { FolderType::NPC, 0xDC, 0x50, 0x50 }, // Red
        { FolderType::Environment, 0x64, 0xA0, 0x78 }, // Muted green
        { FolderType::Extra, 0x96, 0x96, 0xAA }, // Neutral grey-blue
        { FolderType::Misc, 0x5A, 0x5A, 0x5A }, // Dark grey
        { FolderType::Default, 0xFF, 0xFF, 0xFF }, // White
        { FolderType::Dev, 0xFF, 0x1A, 0x69 }  // Vibrant red
    };

    void WriteCustomColors(const std::vector<ColorEntry>& colors);
    std::vector<ColorEntry> GetCustomColors();
    std::vector<ColorEntry> GetDefaultColors();
    bool CustomColorsExist();

	void EditColors();
}