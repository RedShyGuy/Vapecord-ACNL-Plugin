#pragma once

#include <CTRPluginFramework.hpp>
#include "Language.hpp"
#include "MenuEntryExtras.hpp"

namespace CTRPluginFramework {
    enum class FolderType {
        Save,
        Movement,
        Inventory,
        Player,
        Animation,
        Seeding,
        Money,
        Island,
        NPC,
        Fun,
        Extra,
        Misc
    };

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
    private:
        Color color;
        
        static std::string setLanguageByKey(const std::string& langKey) {
            return Language::getInstance()->get(langKey);
        }

        static Color GetFolderColor(FolderType type) {
            switch (type) {
                case FolderType::Save:      return Color(150, 100, 255);  // Violett
                case FolderType::Movement:  return Color(80, 220, 120);   // Bright green
                case FolderType::Inventory: return Color(255, 160, 70);   // Orange
                case FolderType::Player:    return Color(70, 180, 255);   // Bright blue
                case FolderType::Animation: return Color(255, 80, 180);   // Magenta
                case FolderType::Seeding:   return Color(110, 200, 100);  // Green
                case FolderType::Money:     return Color(255, 215, 60);   // Golden yellow
                case FolderType::Island:    return Color(60, 220, 200);   // Cyan
                case FolderType::NPC:       return Color(255, 100, 100);  // Warm red
                case FolderType::Fun:       return Color(240, 120, 255);  // Pink purple
                case FolderType::Extra:     return Color(150, 160, 180);  // Grey blue
                case FolderType::Misc:      return Color(100, 100, 100);  // Dark grey
            }
            return Color::White; // fallback
        }
    };
}