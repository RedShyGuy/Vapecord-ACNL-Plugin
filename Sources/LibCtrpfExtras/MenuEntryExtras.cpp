#include "LibCtrpfExtras/MenuEntryExtras.hpp"
#include "LibCtrpfExtras/MenuFolderExtras.hpp"

namespace CTRPluginFramework {
    MenuEntryExtras::MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, FuncPointer menuFunc, const std::string &noteKey)
        : MenuEntry(setLanguageByKey(nameKey), gameFunc, menuFunc, noteKey != "" ? setLanguageByKey(noteKey) : "") {
        this->NameGetter = [nameKey]() {
            return setLanguageByKey(nameKey);
        };

        this->NoteGetter = [noteKey]() {
            return setLanguageByKey(noteKey);
        };
    }

    MenuEntryExtras::MenuEntryExtras(const std::string &nameKey, FuncPointer gameFunc, const std::string &noteKey)
        : MenuEntry(setLanguageByKey(nameKey), gameFunc, noteKey != "" ? setLanguageByKey(noteKey) : "") {
        this->NameGetter = [nameKey]() {
            return setLanguageByKey(nameKey);
        };

        this->NoteGetter = [noteKey]() {
            return setLanguageByKey(noteKey);
        };
    }

    void MenuEntryExtras::Update() {
        this->Name() = this->color << this->NameGetter();
        this->Note() = this->NoteGetter();
    }
}