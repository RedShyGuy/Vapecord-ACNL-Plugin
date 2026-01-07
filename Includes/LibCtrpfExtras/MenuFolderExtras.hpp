#pragma once

#include <CTRPluginFramework.hpp>
#include "Language.hpp"
#include "MenuEntryExtras.hpp"
#include "Plugin/Plugin_Color.hpp"
#include "LibCtrpfExtras/FolderTypes.hpp"
#include "Files.h"

namespace CTRPluginFramework {
    class MenuFolderExtras : public MenuFolder {
    public:
        /**
        * \brief Sets up a MenuFolder with the language key as the name
        * \param nameKey Name key
        * \param folderType Type of the folder
        * \param noteKey Note key for the folder (Optional)
        */
        MenuFolderExtras(const std::string &nameKey, const FolderType folderType, const std::string &noteKey = "");
    
        /**
         * \brief Append a MenuEntryExtras object to this folder, will set the folder color to the entry
         * \param item The entry to append
         */
        void    Append(MenuEntryExtras *item) const;

        /**
         * \brief Append a MenuFolder object to this folder
         * \param item The folder to append
         */
        void    Append(MenuFolderExtras *item) const;

        static std::string GetFolderName(FolderType type) {
            switch (type) {
                case FolderType::Save:          return Language::getInstance()->get("SAVE_CODES");
                case FolderType::Movement:      return Language::getInstance()->get("MOVEMENT_CODES");
                case FolderType::Inventory:     return Language::getInstance()->get("INVENTORY_CODES");
                case FolderType::Player:        return Language::getInstance()->get("PLAYER_CODES");
                case FolderType::Animation:     return Language::getInstance()->get("ANIMATION_CODES");
                case FolderType::Seeding:       return Language::getInstance()->get("SEEDING_CODES");
                case FolderType::Money:         return Language::getInstance()->get("MONEY_CODES");
                case FolderType::Island:        return Language::getInstance()->get("ISLAND_CODES");
                case FolderType::NPC:           return Language::getInstance()->get("NPC_CODES");
                case FolderType::Environment:   return Language::getInstance()->get("ENV_CODES");
                case FolderType::Extra:         return Language::getInstance()->get("EXTRA_CODES");
                case FolderType::Misc:          return Language::getInstance()->get("MISC_CODES");
            }
            return "ERROR"; // fallback
        }

        static Color GetFolderColor(FolderType type) {
            if (type < FolderType::Save || type > FolderType::Misc) {
                return Color::White; // invalid type
            }

            if (CustomColorsExist()) {
                std::vector<ColorEntry> customColors = GetCustomColors();
                for (const ColorEntry& entry : customColors) {
                    if (entry.folderType == type) {
                        return Color(entry.r, entry.g, entry.b);
                    }
                }
            }

            for (const ColorEntry& entry : defaultColors) {
                if (entry.folderType == type) {
                    return Color(entry.r, entry.g, entry.b);
                }
            }
            return Color::White; // fallback
        }
    private:
        Color color;
        
        static std::string setLanguageByKey(const std::string& langKey) {
            return Language::getInstance()->get(langKey);
        }
    };
}