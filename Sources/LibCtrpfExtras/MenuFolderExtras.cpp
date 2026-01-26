#include "LibCtrpfExtras/MenuFolderExtras.hpp"

namespace CTRPluginFramework {
    MenuFolderExtras::MenuFolderExtras(const FolderType folderType, const SubFolder subFolder)
        : MenuFolder(GetFolderColor(folderType) << (subFolder != SubFolder::None ? GetSubFolderName(folderType, subFolder) : GetFolderName(folderType)), "") {
        this->folderType = folderType;
        this->subFolder = subFolder;
    }

    void MenuFolderExtras::Append(MenuEntryExtras *item) {
        item->SetColor(GetFolderColor(folderType));
        item->Update();
        MenuFolder::Append(item);
    }

    void MenuFolderExtras::Append(MenuFolderExtras *item) {
        MenuFolder::Append(item);
    }

    void MenuFolderExtras::Update() {
        this->Name() = GetFolderColor(folderType) << (subFolder != SubFolder::None ? GetSubFolderName(folderType, subFolder) : GetFolderName(folderType));
    }
}