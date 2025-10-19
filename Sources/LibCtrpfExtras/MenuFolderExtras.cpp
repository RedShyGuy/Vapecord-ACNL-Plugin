#include "LibCtrpfExtras/MenuFolderExtras.hpp"

namespace CTRPluginFramework
{
    MenuFolderExtras::MenuFolderExtras(const std::string& nameKey, const FolderType folderType, const std::string &noteKey)
        : MenuFolder(GetFolderColor(folderType) << setLanguageByKey(nameKey), noteKey != "" ? setLanguageByKey(noteKey) : "")
    {
        this->color = GetFolderColor(folderType);
    }

    void MenuFolderExtras::Append(MenuEntryExtras *item) const {
        item->Name() = color << item->Name();
        MenuFolder::Append(item);
    }

    void MenuFolderExtras::Append(MenuFolderExtras *item) const {
        MenuFolder::Append(item);
    }
}