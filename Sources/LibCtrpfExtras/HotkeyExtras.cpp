#include "LibCtrpfExtras/HotkeyExtras.hpp"
#include "LibCtrpfExtras/UtilsExtras.hpp"

namespace CTRPluginFramework {
    HotkeyExtras::HotkeyExtras(u32 keys, const TextID &nameKey)
        : Hotkey(keys, UtilsExtras::setLanguageByKey(nameKey)) {
        this->NameKey = nameKey;
    }
}