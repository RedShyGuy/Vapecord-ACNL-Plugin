#include <cstdarg>
#include "Helpers/PluginMenuData.hpp"
#include "TextFileParser.hpp"
#include "Helpers/QuickMenu.hpp"

namespace CTRPluginFramework {
	MenuFolder *DEVC;

	std::vector<FolderData> PluginMenuData::folderData;

	std::string PluginMenuData::GetHotkeyName(Hotkey hotkey) {
		char *str = (char *)*(u32 *)((u32)&hotkey + 4);
		return std::string(str);
	}

	static int obj_CogID = 0;

	void PluginMenuData::SetUp(MenuFolder *objfolder, bool isSubFolder) {
		if(objfolder != nullptr) {
			folderData.push_back(FolderData());
			int pos = (folderData.size() - 1);

			folderData[pos].folder = objfolder;
			folderData[pos].IndexColor = GetColorFromString(objfolder->Name());
			folderData[pos].IndexName = RemoveColorFromString(objfolder->Name());
			folderData[pos].IndexNote = objfolder->Note();
			folderData[pos].IsSubFolder = isSubFolder;

			std::vector<MenuEntry *> entrys = objfolder->GetEntryList();
			for(auto objentry : entrys) {
				if(objentry != nullptr) {
					folderData[pos].entryData.push_back(EntryData());
					int pos2 = (folderData[pos].entryData.size() - 1);

					folderData[pos].entryData[pos2].entry = objentry;
					folderData[pos].entryData[pos2].IndexColor = GetColorFromString(objentry->Name());
					folderData[pos].entryData[pos2].IndexName = RemoveColorFromString(objentry->Name());
					folderData[pos].entryData[pos2].IndexNote = objentry->Note();
					
					for(int i = 0; i < objentry->Hotkeys.Count(); ++i) 
						folderData[pos].entryData[pos2].IndexHotkeys.push_back(GetHotkeyName(objentry->Hotkeys[i]));

					if(objentry->GetGameFunc() == nullptr && objentry->GetMenuFunc() != nullptr) {
						folderData[pos].entryData[pos2].CogID = obj_CogID;
						obj_CogID++;
					}
					else
						folderData[pos].entryData[pos2].CogID = -1;
				}
			}
		}
	}

	void PluginMenuData::UpdateAll(const Color arr[12]) {
		int previous = 0;

		for(int i = 0; i < folderData.size(); ++i) {
			if(folderData[i].IsSubFolder) {
				folderData[i].IndexColor = arr[previous];

				for(int j = 0; j < folderData[i].entryData.size(); ++j) 
					folderData[i].entryData[j].IndexColor = arr[previous];
			}
			else {
				folderData[i].IndexColor = arr[previous];

				for(int j = 0; j < folderData[i].entryData.size(); ++j) 
					folderData[i].entryData[j].IndexColor = arr[previous];

				previous++;
			}
		}
	
		UpdateAll();
	}
//Update
	void PluginMenuData::UpdateAll(void) {
		for(auto fdata : folderData) {
			for(auto edata : fdata.entryData) {
				
				std::string hotkey = "";
				int count = edata.entry->Hotkeys.Count();

				if(count == 1) {//If Entry has only 1 Hotkey append string of it to name
					hotkey = edata.entry->Hotkeys[0].ToString();
					edata.entry->Hotkeys[0] = Language->Get(edata.IndexHotkeys[0]);
				}
				else {
					for(int i = 0; i < count; ++i) 
						edata.entry->Hotkeys[i] = Language->Get(edata.IndexHotkeys[i]);
				}

				edata.entry->Name() = edata.IndexColor << Language->Get(edata.IndexName) + " " + hotkey;
				edata.entry->Note() = Language->Get(edata.IndexNote);
				edata.entry->RefreshNote();

			//update quick menu
				if(edata.CogID != -1) {
					for(auto qdata : QuickMenu::obj_QuickMenu) {
					//if entry is stored in the quick menu, hide it
						if(edata.CogID == qdata.CogID) {
							edata.entry->Hide();
							goto hidden;
						}
					}

					edata.entry->Show();
				}
				hidden: continue;
			}

			fdata.folder->Name() = fdata.IndexColor << Language->Get(fdata.IndexName);
			fdata.folder->Note() = Language->Get(fdata.IndexNote);
		}
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

	std::string PluginMenuData::FormatColor(const char *fmt, ...) {
		std::string res;

		va_list list;
		va_start(list, fmt);
		while(*fmt != '\0') {
			if(*fmt != '%') {
				res += *fmt;
				fmt++;
				continue;
			}

			fmt++;

			if(*fmt == '\0') 
				break;

			if(*fmt == 'c') {
				void* p = va_arg(list, void *);

				u32 RawColor = (u32)p;

				char strColor[5] = { 
					0x1B, 
					std::max((u8)1, (u8)(RawColor & 0x000000FF)), 
					std::max((u8)1, (u8)((RawColor & 0x0000FF00) >> 8)),
					std::max((u8)1, (u8)((RawColor & 0x00FF0000) >> 16)), 
				};
				res += strColor[0];
				res += strColor[1];
				res += strColor[2];
				res += strColor[3];
       		}      

			fmt++;
		}

		return res;
	}

	Color PluginMenuData::GetColorFromString(const std::string &str) {
		std::vector<char> r_char(str.begin(), str.end());

		if((u8)r_char[0] == 0x1B) { //if colorcode
			u32 c_code = ((u8)r_char[1] << 24 | (u8)r_char[2] << 16 | (u8)r_char[3] << 8 | 0xFF);
			return Color(c_code);
		}
		return Color(0xFFFFFFFF);
	}

	std::string PluginMenuData::RemoveColorFromString(const std::string &str) {
		std::vector<char> r_char(str.begin(), str.end());
		if((u8)r_char[0] == 0x1B) { //if colorcode
			return str.substr(4);
		}
		return str;
	}
}