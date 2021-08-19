#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"

namespace CTRPluginFramework {
	static constexpr int CONFIG_V = 0xC3; //config revs
	static constexpr int DEV_V = 0xE3; //devmode 

//check for config file
	void CheckForCONFIG(void) {
		u8 u_byte = 0;
		if(File::Exists(CONFIGNAME)) {
			File r_file(CONFIGNAME, File::READ);
			r_file.Seek((s64)CONFIG::Version, File::SeekPos::SET);
			r_file.Read(&u_byte, 1);
			if(u_byte != CONFIG_V) {//if not new config
				OSD::Notify("plugin.bin version mismatch | file will now be updated", Color::Orange);
				File::Remove(CONFIGNAME);
				r_file.Flush();
        		r_file.Close();
			}
			else {
				r_file.Flush();
        		r_file.Close();
				return;
			}
		}
		File::Create(CONFIGNAME);
		File w_file(CONFIGNAME, File::WRITE);
		w_file.Seek((s64)CONFIG::Version, File::SeekPos::SET);
		u_byte = CONFIG_V;
		w_file.Write(&u_byte, 1);
		w_file.Flush();
        w_file.Close();
	}
	
//Start Message to ask to backup
	void StartingMsg(void) {
        u8 u_byte = 0;
		File file(CONFIGNAME, File::RW);
		file.Seek((s64)CONFIG::Info, File::SeekPos::SET);
		file.Read(&u_byte, 1);
        if(u_byte != 1) {
			static const std::string question = "Note:\n"
												#if SEEDING_MODE 
												"This Plugin can use seeding cheats, which can be used to make games of others unplayable, be wise and respectful using those."
												#endif
												"This Plugin is still in developement, be aware that you might find bugs/glitches."
												"It is recommended to make a backup of your save game before using this plugin.\n"
												"Do you want to backup your save game now?";

			static const std::string back = "Name the backup you would like to create:";
            if((MessageBox(question, DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
				std::string filename = "";
				Keyboard KB(back);
				if(KB.Open(filename) == -1)
					return;

				Wrap::Dump(PATH_SAVE, filename, ".dat", WrapLoc{ Save::GetInstance()->Address(), 0x89B00 }, WrapLoc{ (u32)-1, (u32)-1 }); 
            }
			u_byte = 1;
			file.Seek((s64)CONFIG::Info, File::SeekPos::SET);
			file.Write(&u_byte, 1);
			file.Flush();
			file.Close();
			return;
        }
        file.Flush();
        file.Close();
    }

//Dev Folder
	bool IsDevModeUsable(void) {
		u8 u_byte = 0;
		File file(CONFIGNAME, File::READ);
		file.Seek((s64)CONFIG::DevMode, File::SeekPos::SET);
		file.Read(&u_byte, 1);
		if(u_byte == DEV_V) {
			if(!DEVC->IsVisible()) 
				DEVC->Show();
		}
		else {
			if(DEVC->IsVisible()) 
				DEVC->Hide();	
		}
		file.Flush();
        file.Close();
		return DEVC->IsVisible();
	}

	void devcallback(void) {
		static KeySequence buttoncombo({ Key::DPadUp, Key::DPadUp, Key::DPadDown, Key::DPadDown, Key::DPadLeft, Key::DPadRight, Key::DPadLeft, Key::DPadRight, Key::B, Key::A });

		if(buttoncombo()) {
			Sleep(Milliseconds(100));
			if((MessageBox("Yay you found the secret menu", "Do you want to launch the developer mode?\nDev-Mode will enable new Cheats that are meant for developers or are still in developement!", DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
				Sleep(Milliseconds(100));
				MessageBox("Dev-Mode is now active!").SetClear(ClearScreen::Top)();	
				
				u8 u_byte = 0;
				File file(CONFIGNAME, File::WRITE);
				file.Seek((s64)CONFIG::DevMode, File::SeekPos::SET);
				u_byte = DEV_V;
				file.Write(&u_byte, 1);
				file.Flush();			
				file.Close();
				
				IsDevModeUsable();
			}	
		}
	}		
	
	const std::vector<std::string> pluginstgns = {
		"Set Color Mode",
		"Set Language",
		"Set Standard Colors",
		"Reset Settings",
	};
//Plugin settings which can be used any time
	void pluginsettings(MenuEntry *entry) {
		Keyboard colkb("Select Option:");
		colkb.Populate(pluginstgns);
		switch(colkb.Open()) {
			case 0:
				SetupColors(true);
			break;
			case 1:
				SetupLanguage(true);
			break;
			case 2:
				CustomFWK(true);
			break;
			case 3: {
				if(DEVC->IsVisible()) 
					DEVC->Hide();	

				File w_file(CONFIGNAME, File::RW);

				int save[5] = { 0, 0, 0, 0, CONFIG_V };

				w_file.Write(save, sizeof(save) / sizeof(int));
				w_file.Flush();
				w_file.Close();

				MessageBox("Settings resetted!").SetClear(ClearScreen::Top)();
			} break;
			default: break;
		}
	}
}