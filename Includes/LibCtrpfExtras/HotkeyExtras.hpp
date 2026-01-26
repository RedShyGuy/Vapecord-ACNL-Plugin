#pragma once

#include <CTRPluginFramework.hpp>
#include <functional>
#include "Language.hpp"

namespace CTRPluginFramework {
    class HotkeyExtras : public Hotkey {
    public:
        /**
         * \brief Create a new Hotkey
         * \param keys The default keys of this hotkey
         * \param nameKey The key of the name for this hotkey \n
         * Appear on the note and on the Hotkey selector when the user want to edit an hotkey
         */
        HotkeyExtras(u32 keys, const std::string &nameKey);

        std::string GetNameKey() const {
            return NameKey;
        }
    private:
        std::string NameKey;

        static std::string setLanguageByKey(const std::string& langKey) {
            Language* lang = Language::getInstance();
            return lang->get(langKey);
        }
    };
}