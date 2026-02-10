#include "LibCtrpfExtras/MenuEntryExtras.hpp"
#include "LibCtrpfExtras/MenuFolderExtras.hpp"
#include "LibCtrpfExtras/UtilsExtras.hpp"

namespace CTRPluginFramework {
    MenuEntryExtras::MenuEntryExtras(const TextID &nameKey, FuncPointer gameFunc, FuncPointer menuFunc, const TextID &noteKey)
        : MenuEntry(UtilsExtras::setLanguageByKey(nameKey), gameFunc, menuFunc, noteKey != TextID::NONE ? UtilsExtras::setLanguageByKey(noteKey) : "") {
        this->nameKey = nameKey;
        this->noteKey = noteKey;
    }

    MenuEntryExtras::MenuEntryExtras(const TextID &nameKey, FuncPointer gameFunc, const TextID &noteKey)
        : MenuEntry(UtilsExtras::setLanguageByKey(nameKey), gameFunc, noteKey != TextID::NONE ? UtilsExtras::setLanguageByKey(noteKey) : "") {
        this->nameKey = nameKey;
        this->noteKey = noteKey;
    }

    void MenuEntryExtras::Update() {
        this->Name() = this->color << UtilsExtras::setLanguageByKey(nameKey);
        this->Note() = UtilsExtras::setLanguageByKey(noteKey);
    }
}