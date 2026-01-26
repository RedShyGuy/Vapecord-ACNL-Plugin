#pragma once

#include <CTRPluginFramework.hpp>
#include <functional>
#include "LibCtrpfExtras/FolderTypes.hpp"
#include "LibCtrpfExtras/HotkeyExtras.hpp"
#include "Language.hpp"

namespace CTRPluginFramework {
    class MenuEntryExtras : public MenuEntry { 
        using FuncPointer = void(*)(MenuEntry*);
    public:
        /**
        * \brief Sets up a MenuEntry with the language key as the name
        * \param nameKey Name key
        * \param gameFunc Game function for the entry
        * \param menuFunc Menu function for the entry
        * \param noteKey Note Key for the entry (Optional)
        */
        MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, FuncPointer menuFunc, const std::string &noteKey = "");

        /**
        * \brief Sets up a MenuEntry with the language key as the name
        * \param nameKey Name key
        * \param gameFunc Game function for the entry
        * \param noteKey Note Key for the entry (Optional)
        */
        MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, const std::string &noteKey = "");

        void    SetColor(const Color &color) {
            this->color = color;
        }

        void    Update();

        void    AppendHotkeyNameKey(const std::string &nameKey) {
            hotkeyNameKeys.push_back(nameKey);
        }

        std::vector<std::string> GetHotkeyNameKeys() const {
            return hotkeyNameKeys;
        }
    private:
        std::vector<std::string> hotkeyNameKeys;

        std::function<std::string()> NameGetter;
        std::function<std::string()> NoteGetter;
        Color color;

        static std::string setLanguageByKey(const std::string& langKey) {
            return Language::getInstance()->get(langKey);
        }
    };
}