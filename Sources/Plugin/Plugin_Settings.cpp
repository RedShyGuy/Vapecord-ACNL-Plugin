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
	
	u32 decrypt(u64 money) {
        u32 enc = (money & 0xFFFFFFFF);
        u16 adjust = ((money >> 32) & 0xFFFF);
        u8 shift_val = ((money >> 48) & 0xFF);
        u8 chk = ((money >> 56) & 0xFF);

        if((((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF) != chk) 
			return 0;
        
        u8 left_shift = ((0x1C - shift_val) & 0xFF);
        u8 right_shift = 0x20 - left_shift;

        if(left_shift >= 0x20) 
            return 0 + (enc << right_shift) - (adjust + 0x8F187432);

        return(enc << left_shift) + (enc >> right_shift) - (adjust + 0x8F187432);
    }
	
	static bool isOpenDev = false;
	
	void devcallback(void) {
		bool passwordcorrect = false;
	//Now that the source code is public you can finally see what a impossible check I made to access the dev menu lol
		if(Controller::IsKeysPressed(Key::L + Key::ZL + Key::DPadLeft + Key::A + Key::DPadDown)) {
			if(GameHelper::IsInRoom(0x5B)) { 
				if(*PlayerClass::GetInstance()->GetAnimation() == 6) {
					if(Inventory::ReadSlot(0) == 0x4000335B) {
						Sleep(Milliseconds(100));
						GameHelper::OpenMenu(0x35);
						
						yay:
						if(passwordcorrect) {
							Animation::ExecuteAnimationWrapper(4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
				}
			}
		}	

		if(Inventory::GetCurrent() == 0x35 && GameHelper::IsInRoom(0x5A)) { //if menu opened
			UIntVector Position = Touch::GetPosition();
			//If Press A Button is clicked
			if(Position.x >= 200 && Position.x <= 300 && Position.y >= 205 && Position.y <= 235) {
				u32 PassWord = *(u32 *)(*(u32 *)Code::InvMenu + 0x45C);
				if((int)PassWord == decrypt(0xD1070029C3E0FC78)) {
					Sleep(Milliseconds(100));
					MessageBox("Password correct!").SetClear(ClearScreen::Top)();
					passwordcorrect = true;
					goto yay;
				}
				else {
					Sleep(Milliseconds(100));
					MessageBox("Password incorrect!").SetClear(ClearScreen::Top)();
					passwordcorrect = false;
					Animation::ExecuteAnimationWrapper(4, 0xF, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					Sleep(Seconds(1));
					GameHelper::OpenMenu(0x35);
				}						
			}
		}
		
		if(Inventory::GetCurrent() == 0x35 && !isOpenDev) 
			isOpenDev = true;
		
		if(Inventory::GetCurrent() != 0x35 && isOpenDev) {
			Animation::Idle();
			isOpenDev = false;				
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