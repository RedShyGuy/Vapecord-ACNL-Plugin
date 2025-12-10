#include "LibCtrpfExtras/MenuEntryExtras.hpp"

namespace CTRPluginFramework
{
    MenuEntryExtras::MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, FuncPointer menuFunc, const std::string &noteKey)
        : MenuEntry(setLanguageByKey(nameKey), gameFunc, menuFunc, noteKey != "" ? setLanguageByKey(noteKey) : "")
    {}

    MenuEntryExtras::MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, const std::string &noteKey)
        : MenuEntry(setLanguageByKey(nameKey), gameFunc, noteKey != "" ? setLanguageByKey(noteKey) : "") 
    {}
}