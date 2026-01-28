#pragma once

#include <CTRPluginFramework.hpp>
#include <functional>
#include "Language.hpp"
#include "TextID.hpp"

namespace CTRPluginFramework {
    class HotkeyExtras : public Hotkey {
    public:
        /**
         * \brief Create a new Hotkey
         * \param keys The default keys of this hotkey
         * \param nameKey The key of the name for this hotkey \n
         * Appear on the note and on the Hotkey selector when the user want to edit an hotkey
         */
        HotkeyExtras(u32 keys, const TextID &nameKey);

        TextID GetNameKey() const {
            return NameKey;
        }
    private:
        TextID NameKey;
    };
}