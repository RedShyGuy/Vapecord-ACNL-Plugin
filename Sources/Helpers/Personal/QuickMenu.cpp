#include "Helpers/QuickMenu.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "Files.h"
#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
    std::vector<MenuEntry *> QuickMenu::obj_QuickMenu;

//this initializes the quick menu from file
    void QuickMenu::Init() {
        if(!File::Exists(Utils::Format(PATH_QUICKM, regionName.c_str()))) {
            File::Create(Utils::Format(PATH_QUICKM, regionName.c_str()));
            return;
        }

    //Pushes all available entrys (including hidden ones) with their ID to the data
        std::vector<MenuEntry *> cogEntrys;
        ListAvailableCogEntrys(cogEntrys);

    //reads file which holds ID data of the hidden entrys
        File f_quickm(Utils::Format(PATH_QUICKM, regionName.c_str()), File::READ);
        s8* buffer = new s8[f_quickm.GetSize()];
        f_quickm.Read(buffer, f_quickm.GetSize());
        
        for(int i = 0; i < f_quickm.GetSize(); ++i) {
            for(auto edata : cogEntrys) {
            //look for ID in all available entrys and push its data into the quick menu
                if(edata->CogID() == buffer[i]) {
                    obj_QuickMenu.push_back(edata);
                    break;
                }
            }
        }

        PluginMenu::GetRunningInstance()->Reload(QuickMenu::obj_QuickMenu);

        delete[] buffer;
    }

    void QuickMenu::UpdateFile(void) {
        File::Remove(Utils::Format(PATH_QUICKM, regionName.c_str()));
        File::Create(Utils::Format(PATH_QUICKM, regionName.c_str()));

        std::vector<s8> ID;
        for(auto qdata : obj_QuickMenu) 
            ID.push_back(qdata->CogID());

        File f_quickm(Utils::Format(PATH_QUICKM, regionName.c_str()), File::WRITE);
        f_quickm.Write(ID.data(), ID.size());
    }

//This will add a entry with their ID to the Quick Menu
    void QuickMenu::AddEntry(MenuEntry *entry) {
        std::vector<MenuEntry *> cogEntrys;
        ListAvailableCogEntrys(cogEntrys);
       
        for(auto edata : cogEntrys) {
        //adds new entry to quick menu if it exists
            if(edata == entry) {
                obj_QuickMenu.push_back(entry);
                break;
            }
        }

        UpdateFile();

        PluginMenu::GetRunningInstance()->Reload(QuickMenu::obj_QuickMenu);
    }

//This will remove a entry from the Quick Menu
    void QuickMenu::RemoveEntry(MenuEntry *entry) {
        for(int i = 0; i < obj_QuickMenu.size(); ++i) {
        //if entry is in quick menu, it will be removed
            if(obj_QuickMenu[i]->CogID() == entry->CogID()) {
                obj_QuickMenu.erase(obj_QuickMenu.begin() + i);
                break;
            }
        }
     
        entry->Show();

        UpdateFile();
        
        PluginMenu::GetRunningInstance()->Reload(QuickMenu::obj_QuickMenu);
    }

//This lists all available cog entrys of the plugin (ignoring hidden ones)
    void QuickMenu::ListAvailableCogEntrys(std::vector<MenuEntry *> &data) {
        for(auto obj_folder : PluginMenuData::folderData) {
            for(auto obj_entry : obj_folder.entryData) {
                if(obj_entry->CogID() != -1 && obj_entry->IsVisible()) 
                    data.push_back(obj_entry);
            }
        }
    }
}