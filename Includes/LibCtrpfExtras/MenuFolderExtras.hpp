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
        * \param folderType Type of the folder
        */
        MenuFolderExtras(const FolderType folderType, const SubFolder subFolder = SubFolder::None);
    
        /**
         * \brief Append a MenuEntryExtras object to this folder, will set the folder color to the entry
         * \param item The entry to append
         */
        void    Append(MenuEntryExtras *item);

        /**
         * \brief Append a MenuFolder object to this folder
         * \param item The folder to append
         */
        void    Append(MenuFolderExtras *item);

        /**
         * \brief Update the menu folders name and color
         */
        void    Update();

        FolderType  GetFolderType() const {
            return folderType;
        }

        static std::string GetFolderName(FolderType type) {
            static const std::unordered_map<FolderType, const char*> keys = {
                { FolderType::Save,        "SAVE_CODES" },
                { FolderType::Movement,    "MOVEMENT_CODES" },
                { FolderType::Inventory,   "INVENTORY_CODES" },
                { FolderType::Player,      "PLAYER_CODES" },
                { FolderType::Animation,   "ANIMATION_CODES" },
                { FolderType::Seeding,     "SEEDING_CODES" },
                { FolderType::Money,       "MONEY_CODES" },
                { FolderType::Island,      "ISLAND_CODES" },
                { FolderType::NPC,         "NPC_CODES" },
                { FolderType::Environment, "ENV_CODES" },
                { FolderType::Extra,       "EXTRA_CODES" },
                { FolderType::Misc,        "MISC_CODES" },
                { FolderType::Default,     "DEFAULT_CODES" },
                { FolderType::Dev,         "DEV_CODES" }
            };

            auto it = keys.find(type);
            return it != keys.end() ? Language::getInstance()->get(it->second) : "ERROR";
        }

        static std::string GetSubFolderName(FolderType parent, SubFolder sub) {
            if (sub == SubFolder::None) {
                return "";
            }

            switch (parent) {
                case FolderType::Player:
                    switch (sub) {
                        case SubFolder::PlayerSave: return Language::getInstance()->get("PLAYER_SAVE_CODES");
                        default: break;
                    }
                    break;

                case FolderType::Seeding:
                    switch (sub) {
                        case SubFolder::Seed: return Language::getInstance()->get("SEED_CODES");
                        case SubFolder::Drop: return Language::getInstance()->get("DROP_CODES");
                        case SubFolder::Tree: return Language::getInstance()->get("TREE_CODES");
                        default: break;
                    }
                    break;

                case FolderType::Environment:
                    switch (sub) {
                        case SubFolder::Fish: return Language::getInstance()->get("FISH_CODES");
                        case SubFolder::Insect: return Language::getInstance()->get("INSECT_CODES");
                        default: break;
                    }
                    break;

                case FolderType::Extra:
                    switch (sub) {
                        case SubFolder::Chat: return Language::getInstance()->get("CHAT_CODES");
                        case SubFolder::Fun: return Language::getInstance()->get("FUN_CODES");
                        default: break;
                    }
                    break;

                default:
                    break;
            }

            return "ERROR";
        }

        static Color GetFolderColor(FolderType type) {
            if (type >= FolderType::Save && type <= FolderType::Misc) {
                if (CustomColorsExist()) {
                    std::vector<ColorEntry> customColors = GetCustomColors();
                    for (const ColorEntry& entry : customColors) {
                        if (entry.folderType == type) {
                            return Color(entry.r, entry.g, entry.b);
                        }
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
        std::function<std::string()> NameGetter;
        FolderType folderType;
        SubFolder subFolder;

        static std::string setLanguageByKey(const std::string& langKey) {
            return Language::getInstance()->get(langKey);
        }
    };
}