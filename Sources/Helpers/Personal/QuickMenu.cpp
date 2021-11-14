#include "Helpers/QuickMenu.hpp"
#include "MenuPointers.hpp"

namespace CTRPluginFramework {
    std::vector<MenuEntry*> TestEntry;

    void QuickMenu::Init() {
        
    }

    void QuickMenu::ListEntrys(std::vector<MenuEntry*> &quickmenu) {
        quickmenu = TestEntry;
    }

    void QuickMenu::AddEntry(MenuEntry* entry) {
        TestEntry.push_back(entry);
    }

    void QuickMenu::ListAvailableCogEntrys(std::vector<MenuEntry*> &cogEntrys) {
        PluginMenu *menu = PluginMenu::GetRunningInstance();
        if(menu == nullptr) //if menu somehow isn't loaded
            return;

        cogEntrys.clear();
        std::vector<MenuEntry *> AllEntrys;

        std::vector<MenuFolder *> Folders = menu->GetFolderList();
        std::vector<MenuEntry *> Entrys = menu->GetEntryList();
        std::vector<MenuFolder *> SubFolders;

        for(MenuEntry *entry : Entrys)
            AllEntrys.push_back(entry);

        for(MenuFolder *folder : Folders) {
            SubFolders = folder->GetFolderList();
            for(MenuFolder *subfolder : SubFolders) {
                Entrys = subfolder->GetEntryList();
                for(MenuEntry *entry : Entrys)
                    AllEntrys.push_back(entry);
            }

            Entrys = folder->GetEntryList();
            for(MenuEntry *entry : Entrys) 
                AllEntrys.push_back(entry);
        }

        for(MenuEntry *entry : AllEntrys) {
            if(GetGameFunc(entry) == nullptr && GetMenuFunc(entry) != nullptr && entry->IsVisible())
                cogEntrys.push_back(entry);
        }
    }
}