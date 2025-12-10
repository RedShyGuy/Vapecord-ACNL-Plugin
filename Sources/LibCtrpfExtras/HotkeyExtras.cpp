#include "LibCtrpfExtras/HotkeyExtras.hpp"

namespace CTRPluginFramework
{
    HotkeyExtras::HotkeyExtras(u32 keys, const std::string &nameKey)
        : Hotkey(keys, setLanguageByKey(nameKey))
    {}
}