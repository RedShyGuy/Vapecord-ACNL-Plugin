#pragma once

#include <CTRPluginFramework.hpp>
#include "Language.hpp"
#include "MenuEntryExtras.hpp"
#include "Plugin/Plugin_Color.hpp"
#include "LibCtrpfExtras/FolderTypes.hpp"
#include "Files.h"
#include "TextID.hpp"

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
            static const std::unordered_map<FolderType, const TextID> keys = {
                { FolderType::Save,        TextID::SAVE_CODES },
                { FolderType::Movement,    TextID::MOVEMENT_CODES },
                { FolderType::Inventory,   TextID::INVENTORY_CODES },
                { FolderType::Player,      TextID::PLAYER_CODES },
                { FolderType::Animation,   TextID::ANIMATION_CODES },
                { FolderType::Seeding,     TextID::SEEDING_CODES },
                { FolderType::Money,       TextID::MONEY_CODES },
                { FolderType::Island,      TextID::ISLAND_CODES },
                { FolderType::NPC,         TextID::NPC_CODES },
                { FolderType::Environment, TextID::ENV_CODES },
                { FolderType::Extra,       TextID::EXTRA_CODES },
                { FolderType::Misc,        TextID::MISC_CODES },
                { FolderType::Default,     TextID::DEFAULT_CODES },
                { FolderType::Dev,         TextID::DEV_CODES }
            };

            auto it = keys.find(type);
            return it != keys.end() ? Language::getInstance()->get(it->second) : Language::getInstance()->get(TextID::NONE);
        }

        static std::string GetSubFolderName(FolderType parent, SubFolder sub) {
            TextID textID = TextID::NONE;

            if (sub != SubFolder::None) {
                switch (parent) {
                    case FolderType::Player:
                        switch (sub) {
                            case SubFolder::PlayerSave: textID = TextID::PLAYER_SAVE_CODES;
                            default: break;
                        }
                        break;

                    case FolderType::Seeding:
                        switch (sub) {
                            case SubFolder::Seed: textID = TextID::SEED_CODES;
                            case SubFolder::Drop: textID = TextID::DROP_CODES;
                            case SubFolder::Tree: textID = TextID::TREE_CODES;
                            default: break;
                        }
                        break;

                    case FolderType::Environment:
                        switch (sub) {
                            case SubFolder::Fish: textID = TextID::FISH_CODES;
                            case SubFolder::Insect: textID = TextID::INSECT_CODES;
                            default: break;
                        }
                        break;

                    case FolderType::Extra:
                        switch (sub) {
                            case SubFolder::Chat: textID = TextID::CHAT_CODES;
                            case SubFolder::Fun: textID = TextID::FUN_CODES;
                            default: break;
                        }
                        break;

                    default:
                        break;
                }
            }

            return Language::getInstance()->get(textID);
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
        std::vector<TextID> NameKeys;
        FolderType folderType;
        SubFolder subFolder;
    };
}