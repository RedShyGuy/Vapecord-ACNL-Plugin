#include "Helpers/QuickMenu.hpp"
#include "MenuPointers.hpp"
#include "Files.h"
#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
    quickMenuData QuickMenu::g_quickMenu; //Holds data of non hidden entrys, with their ID and all

    std::vector<s8> QuickMenu::q_quickMenuFile; //will hold ID of an entry, obtained from a file

    quickMenuData QuickMenu::data; //Holds actual data of entrys, with their ID and all

//this initializes the quick menu from file
    void QuickMenu::Init() {
        if(!File::Exists(Utils::Format(PATH_QUICKM, regionName.c_str()))) {
            File::Create(Utils::Format(PATH_QUICKM, regionName.c_str()));
            return;
        }

    //Pushes all available entrys (including hidden ones) with their ID to the data
        std::vector<s8> empty;
        QuickMenu::ListAvailableCogEntrys(data.entry, empty);
        empty.clear();

        for(int i = 0; i < data.entry.size(); ++i)
            data.ID.push_back(i);

        OSD::Notify(Utils::Format("Data Size : %04X", data.entry.size()));
        OSD::Notify(Utils::Format("ID Size : %04X", data.ID.size()));

    //reads file which holds ID data of the hidden entrys
        File f_quickm(Utils::Format(PATH_QUICKM, regionName.c_str()), File::READ);
        s8* buffer = new s8[f_quickm.GetSize()];
        f_quickm.Read(buffer, f_quickm.GetSize());
        
        for(int i = 0; i < f_quickm.GetSize(); ++i) {
            q_quickMenuFile.push_back(buffer[i]);
        }

        OSD::Notify(Utils::Format("QM File Size : %04X", f_quickm.GetSize()));
        OSD::Notify(Utils::Format("QM Buffer Size : %04X", q_quickMenuFile.size()));

    //reads through all entry ID's and hides all entrys which have their ID inside of the file 
        for(int i = 0; i <= data.entry.size(); ++i) {
            for(int j = 0; j < q_quickMenuFile.size(); ++j) {
                if(q_quickMenuFile[j] == data.ID[i]) {
                    data.entry[i]->Hide();

                    g_quickMenu.entry.push_back(data.entry[i]);
                    g_quickMenu.ID.push_back(data.ID[i]);

                    data.entry.erase(data.entry.begin() + i);
                    data.ID.erase(data.ID.begin() + i);
                }
            }
        }

        delete[] buffer;
    }

//This will list all the entrys with their ID of the Quick Menu
    void QuickMenu::ListEntrys(quickMenuData &quickmenu) {
        quickmenu = g_quickMenu;
    }

//This will add a entry with their ID to the Quick Menu
    void QuickMenu::AddEntry(MenuEntry* entry, s8 ID) {
        g_quickMenu.entry.push_back(entry);
        g_quickMenu.ID.push_back(ID);

        for(int i = 0; i < data.entry.size(); ++i) {
            if(data.ID[i] == ID) {
                data.entry.erase(data.entry.begin() + i);
                data.ID.erase(data.ID.begin() + i);
            }
        }

        if(!File::Exists(Utils::Format(PATH_QUICKM, regionName.c_str()))) 
            File::Create(Utils::Format(PATH_QUICKM, regionName.c_str()));

        File f_quickm(Utils::Format(PATH_QUICKM, regionName.c_str()), File::WRITE);
        f_quickm.Write(g_quickMenu.ID.data(), g_quickMenu.ID.size());

        entry->Hide();
    }

//This will remove a entry from the Quick Menu
    void QuickMenu::RemoveEntry(MenuEntry* entry, s8 ID) {
        data.entry.push_back(entry);
        data.ID.push_back(ID);

        for(int i = 0; i < g_quickMenu.entry.size(); ++i) {
            if(g_quickMenu.ID[i] == ID) {
                g_quickMenu.entry.erase(g_quickMenu.entry.begin() + i);
                g_quickMenu.ID.erase(g_quickMenu.ID.begin() + i);
            }
        }

        if(!File::Exists(Utils::Format(PATH_QUICKM, regionName.c_str()))) 
            File::Create(Utils::Format(PATH_QUICKM, regionName.c_str()));

        File f_quickm(Utils::Format(PATH_QUICKM, regionName.c_str()), File::WRITE | File::TRUNCATE);
        f_quickm.Write(g_quickMenu.ID.data(), g_quickMenu.ID.size());

        entry->Show();
    }

//This lists all available cog entrys of the plugin (ignoring hidden ones)
    void QuickMenu::ListAvailableCogEntrys(std::vector<MenuEntry*> &cogEntrys, std::vector<s8> &ID) {
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

        if(!data.ID.empty()) 
            ID = data.ID;
    }
}