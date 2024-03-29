#include "Helpers/PluginMenuData.hpp"
#include "Helpers/QuickMenu.hpp"
#include "Files.h"
#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
	MenuFolder *DEVC;

	std::vector<FolderData> PluginMenuData::folderData;
	std::vector<MenuEntry*> PluginMenuData::fullList;

	void PluginMenuData::SetUp(MenuFolder *objfolder, bool isSubFolder) {
		if(objfolder != nullptr) {
			folderData.push_back(FolderData());
			int pos = (folderData.size() - 1);

			folderData[pos].folder = objfolder;
			folderData[pos].IsSubFolder = isSubFolder;

			std::vector<MenuEntry *> entrys = objfolder->GetEntryList();
			for(auto objentry : entrys) {
				if(objentry != nullptr) 
					folderData[pos].entryData.push_back(objentry);
			}
		}
	}

	void PluginMenuData::UpdateAll(const Color arr[12]) {
		int previous = 0;

		for(int i = 0; i < folderData.size(); ++i) {
			if(folderData[i].IsSubFolder) {
				folderData[i].folder->NameColor() = arr[previous];

				for(int j = 0; j < folderData[i].entryData.size(); ++j) 
					folderData[i].entryData[j]->NameColor() = arr[previous];
			}
			else {
				folderData[i].folder->NameColor() = arr[previous];

				for(int j = 0; j < folderData[i].entryData.size(); ++j) 
					folderData[i].entryData[j]->NameColor() = arr[previous];

				previous++;
			}
		}

		PluginMenu::GetRunningInstance()->Reload(QuickMenu::obj_QuickMenu);
	}

	void PluginMenuData::SetVapecordStandardTheme(FwkSettings &settings) {
		FwkSettings::SetThemeDefault();
		settings.BackgroundSecondaryColor = Color::Black; 
		settings.BackgroundBorderColor = Color(255, 0, 247);
		settings.WindowTitleColor = Color::Yellow;
		settings.MenuSelectedItemColor = Color::Lime;
		settings.MenuUnselectedItemColor = Color::White;
		settings.Keyboard.KeyText = Color::Lime;
		settings.Keyboard.KeyTextPressed = Color::Lime;
		settings.Keyboard.Cursor = Color::Lime;
		settings.Keyboard.Input = Color::Lime;
		settings.CustomKeyboard.BackgroundSecondary = Color::Black;
		settings.CustomKeyboard.BackgroundBorder = Color(255, 0, 247);
		settings.CustomKeyboard.KeyBackground = Color::Black;
		settings.CustomKeyboard.KeyBackgroundPressed = Color(0x505050A0);
		settings.CustomKeyboard.KeyText = Color::White;
		settings.CustomKeyboard.KeyTextPressed = Color::White;
	}

	void PluginMenuData::SetFWK(FwkSettings &settings, const Color arr[25]) {
		settings.MainTextColor = arr[0];
		settings.WindowTitleColor = arr[1];
		settings.MenuSelectedItemColor = arr[2];
		settings.MenuUnselectedItemColor = arr[3];
		settings.BackgroundMainColor = arr[4];
		settings.BackgroundSecondaryColor = arr[5];
		settings.BackgroundBorderColor = arr[6];
		settings.Keyboard.Background = arr[7];
		settings.Keyboard.KeyBackground = arr[8];
		settings.Keyboard.KeyBackgroundPressed = arr[9];
		settings.Keyboard.KeyText = arr[10];	
		settings.Keyboard.KeyTextPressed = arr[11];
		settings.Keyboard.KeyTextDisabled = arr[12];
		settings.Keyboard.Cursor = arr[13];
		settings.Keyboard.Input = arr[14];
		settings.CustomKeyboard.BackgroundMain = arr[15];
		settings.CustomKeyboard.BackgroundSecondary = arr[16];
		settings.CustomKeyboard.BackgroundBorder = arr[17];
		settings.CustomKeyboard.KeyBackground = arr[18];
		settings.CustomKeyboard.KeyBackgroundPressed = arr[19];
		settings.CustomKeyboard.KeyText = arr[20];
		settings.CustomKeyboard.KeyTextPressed = arr[21];
		settings.CustomKeyboard.ScrollBarBackground = arr[22];
		settings.CustomKeyboard.ScrollBarThumb = arr[23];
		settings.Notifications.DefaultBackground = arr[24];
		settings.Notifications.DefaultForeground = arr[25];
	}

	void PluginMenuData::SaveEntrys(void) {
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(!menu)
			return;

		for(MenuEntry* mainEntry : menu->GetEntryList()) 
			fullList.push_back(mainEntry);

		for(MenuFolder *folder : menu->GetFolderList()) {
			if(folder->GetFolderList().size() > 0) {
				for(MenuFolder *subfolder : folder->GetFolderList()) {
					for(MenuEntry* sEntry : subfolder->GetEntryList()) 
						fullList.push_back(sEntry);
				}
			}

			for(MenuEntry* fEntry : folder->GetEntryList()) 
				fullList.push_back(fEntry);
		}
	}

	void PluginMenuData::GetEntrys(std::vector<MenuEntry *> entry) {
		entry = fullList;
	}

	void PluginMenuData::Init(void) {
		SaveEntrys();

		if(!File::Exists(Utils::Format(PATH_HIDECH, regionName.c_str()))) {
            File::Create(Utils::Format(PATH_HIDECH, regionName.c_str()));
            return;
        }

    //Pushes all available entrys (including hidden ones) with their ID to the data
        std::vector<MenuEntry *> allEntrys;
        GetEntrys(allEntrys);

    //reads file which holds ID data of the hidden entrys
        File f_quickm(Utils::Format(PATH_HIDECH, regionName.c_str()), File::READ);
        s16* buffer = new s16[f_quickm.GetSize()];
        f_quickm.Read(buffer, f_quickm.GetSize());

        for(int i = 0; i < f_quickm.GetSize(); ++i) {
            for(auto edata : allEntrys) {
            //look for ID in all available entrys and push its data into the quick menu
                if(edata->CheatID() == buffer[i]) {
                    edata->Hide();
                    break;
                }
            }
        }

        delete[] buffer;
	}
}