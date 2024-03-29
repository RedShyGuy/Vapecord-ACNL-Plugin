#include "Config.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "Helpers/KeySequence.hpp"
#include "Helpers/Address.hpp"
#include "cheats.hpp"
#include "Files.h"

namespace CTRPluginFramework {
	static const u8 CONFIG_V = 0xC9;

	void WriteConfig(CONFIG config, u8 byte) {
		File file(Utils::Format(CONFIGNAME, regionName.c_str()), File::WRITE);
		file.Seek((s64)config, File::SeekPos::SET);
		file.Write(&byte, 1);
	}

	void ReadConfig(CONFIG config, u8 &byte) {
		File file(Utils::Format(CONFIGNAME, regionName.c_str()), File::READ);
		file.Seek((s64)config, File::SeekPos::SET);
		file.Read(&byte, 1);
	}

	void ResetConfig(void) {
		u8 save[8] = { 0, 0, 0, 0, 0, 0, 0, CONFIG_V };

		File w_file(Utils::Format(CONFIGNAME, regionName.c_str()), File::WRITE);
		w_file.Write(save, sizeof(save));
	}

	void ClearConfig(void) {
		File file(Utils::Format(CONFIGNAME, regionName.c_str()), File::TRUNCATE);
	}

//check for config file
	void CheckForCONFIG(void) {
		if(!Directory::IsExists(V_STANDARD)) Directory::Create(V_STANDARD);
		if(!Directory::IsExists(V_DATA)) Directory::Create(V_DATA);

		if(!Directory::IsExists(Utils::Format(V_DIRECTORY, regionName.c_str()))) 
			Directory::Create(Utils::Format(V_DIRECTORY, regionName.c_str()));

		if(File::Exists(Utils::Format(CONFIGNAME, regionName.c_str()))) {
			u8 u_byte = 0;
			ReadConfig(CONFIG::Version, u_byte);

			if(u_byte != CONFIG_V) {//if not new config
				OSD::Notify("plugin.bin version mismatch | file will now be updated", Color::Orange);
				ClearConfig();
			}
			else return;
		}

		File::Create(Utils::Format(CONFIGNAME, regionName.c_str()));
		ResetConfig();
	}
	
//Start Message to ask to backup
	void StartingMsg(void) {
        u8 u_byte = false;
		ReadConfig(CONFIG::Info, u_byte);

        if(!u_byte) {
			static const std::string question = "Note:\n"
												"This Plugin can use seeding cheats, which can be used to make games of others unplayable, be wise and respectful using those."
												"This Plugin is still in developement, be aware that you might find bugs/glitches."
												"It is recommended to make a backup of your save game before using this plugin.\n"
												"Do you want to backup your save game now?";

			static const std::string back = "Name the backup you would like to create:";
            if((MessageBox(question, DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
				std::string filename = "";
				Keyboard KB(back);
				if(KB.Open(filename) == -1)
					goto forward;

				WrapLoc loc = { (u32 *)Save::GetInstance()->Address(), 0x89B00 };
				Wrap::Dump(Utils::Format(PATH_SAVE, regionName.c_str()), filename, ".dat", &loc, nullptr); 
            }

		forward:
			WriteConfig(CONFIG::Info, true);
        }
    }

//Dev Folder
	bool IsDevModeUsable(void) {
		u8 u_byte = false;
		ReadConfig(CONFIG::DevMode, u_byte);

		if(u_byte) {
			if(!DEVC->IsVisible()) 
				DEVC->Show();
		}
		else {
			if(DEVC->IsVisible()) 
				DEVC->Hide();	
		}
		return DEVC->IsVisible();
	}

	void devcallback(void) {
		static KeySequence buttoncombo({ Key::DPadUp, Key::DPadUp, Key::DPadDown, Key::DPadDown, Key::DPadLeft, Key::DPadRight, Key::DPadLeft, Key::DPadRight, Key::B, Key::A });

		if(buttoncombo() && !DEVC->IsVisible()) {
			if((MessageBox("Yay you found the secret menu", "Do you want to launch the developer mode?\nDev-Mode will enable new Cheats that are meant for developers or are still in developement!", DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
				MessageBox("Dev-Mode is now active!").SetClear(ClearScreen::Top)();	

				WriteConfig(CONFIG::DevMode, true);
				
				IsDevModeUsable();
			}	
		}
	}

	void resetSettings(bool SetInMenu) {
		if((MessageBox("Reset V-Settings?", "Do you really want to erase your V-Data?\nThis will erase your color and language selections.", DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			if(DEVC->IsVisible()) 
				DEVC->Hide();	

			ResetConfig();

			MessageBox("Erased Data!").SetClear(ClearScreen::Top)();	
			Sleep(Milliseconds(100));
		}
	}

	void cheatsVisibility(bool SetInMenu) {
		std::vector<MenuEntry*> entrys;
		PluginMenuData::GetEntrys(entrys);

		std::vector<std::string> nameList;		
		for(MenuEntry *aEntry : entrys)
			nameList.push_back(aEntry->Name());

		Keyboard KB("Select cheat to hide/unhide:", nameList);
		int choice = KB.Open();
		if(choice < 0)
			return;

		if(entrys.at(choice)->IsVisible()) {
			entrys.at(choice)->Hide();
		
			MessageBox("Erased Data!").SetClear(ClearScreen::Top)();	
			Sleep(Milliseconds(100));
		}
		else {
			entrys.at(choice)->Show();
			
			MessageBox("Erased Data!").SetClear(ClearScreen::Top)();	
			Sleep(Milliseconds(100));
		}
	}
}