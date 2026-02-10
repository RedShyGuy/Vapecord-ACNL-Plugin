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
        MenuEntryExtras(const TextID &nameKey, FuncPointer gameFunc, FuncPointer menuFunc, const TextID &noteKey = TextID::NONE);

        /**
        * \brief Sets up a MenuEntry with the language key as the name
        * \param nameKey Name key
        * \param gameFunc Game function for the entry
        * \param noteKey Note Key for the entry (Optional)
        */
        MenuEntryExtras(const TextID &nameKey, FuncPointer gameFunc, const TextID &noteKey = TextID::NONE);

        void    SetColor(const Color &color) {
            this->color = color;
        }

        void    Update();

        void    AppendHotkeyNameKey(const TextID &nameKey) {
            hotkeyNameKeys.push_back(nameKey);
        }

        std::vector<TextID> GetHotkeyNameKeys() const {
            return hotkeyNameKeys;
        }
    private:
        std::vector<TextID> hotkeyNameKeys;
        TextID nameKey;
        TextID noteKey;
        Color color;
    };
}