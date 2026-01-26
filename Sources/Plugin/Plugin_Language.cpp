#include "Config.hpp"
#include "Color.h"
#include "Files.h"
#include "Language.hpp"
#include "Address/Address.hpp"
#include "LibCtrpfExtras/PluginMenuExtras.hpp"

namespace CTRPluginFramework {	
    void SetupLanguage(bool SetInMenu) {
		std::string language = "";
		ReadLanguage(language);

		if (!File::Exists(PATH_LANGUAGE_BIN)) {
			MessageBox(Utils::Format("Error 577\nThe language.bin is missing. The Plugin can not work without it!\nGet more info and help on the Discord Server: %s\nGame will be closed now!", DISCORDINV)).SetClear(ClearScreen::Top)();
			Process::ReturnToHomeMenu();
			return;
		}

		auto languages = Language::getInstance()->listAvailableLanguages(PATH_LANGUAGE_BIN);
		if (languages.empty()) {
			MessageBox(Utils::Format("Error 578\nThe language.bin is empty or corrupted!\nGet more info and help on the Discord Server: %s\nGame will be closed now!", DISCORDINV)).SetClear(ClearScreen::Top)();
			Process::ReturnToHomeMenu();
			return;
		}

		bool languageExists = std::find(languages.begin(), languages.end(), language) != languages.end();

        if(!languageExists || SetInMenu) {
			std::vector<std::string> values;
			for (const auto& pair : languages) {
				values.push_back(pair.fullName);
			}

            Keyboard keyboard("Which language do you want to use?", values);
			keyboard.CanAbort(SetInMenu);

			int sel = keyboard.Open();
			if (sel < 0) {
				return;
			}

			if (sel < (int)languages.size()) {
            	language = languages[sel].shortName;
			}

			if (!WriteLanguage(language)) { //write language mode to file
				MessageBox(Utils::Format("Error 608\nCouldn't save chosen language.\nGet more info and help on the Discord Server: %s", DISCORDINV)).SetClear(ClearScreen::Top)();
			} 
			languageExists = true;
        }

		if (!languageExists || !Language::getInstance()->loadFromBinary(PATH_LANGUAGE_BIN, language.c_str())) {
			MessageBox(Utils::Format("Error 605\nCouldn't load the language.\nGet more info and help on the Discord Server: %s", DISCORDINV)).SetClear(ClearScreen::Top)();

			DeleteLanguage();
			SetupLanguage(false); //redo language choosing
		}

		if (SetInMenu) {
			PluginMenuExtras::Update();
			MessageBox("Successfully set new language!").SetClear(ClearScreen::Top)();
		}
    }

	void DeleteLanguage(void) {
		File::Remove(Utils::Format(CONFIGNAME, Address::regionName.c_str()));
	}

	bool WriteLanguage(const std::string& langCode) {
		File file;
		if (File::Open(file, Utils::Format(CONFIGNAME, Address::regionName.c_str()), File::CREATE | File::WRITE) != File::SUCCESS) {
			return false;
		}

		u8 len = static_cast<u8>(langCode.size());
		if (file.Write(&len, sizeof(len)) != File::SUCCESS) {
			file.Close();
			return false;
		}

		if (file.Write(langCode.data(), len) != File::SUCCESS) {
			file.Close();
			return false;
		}

		file.Close();
		return true;
	}

	bool ReadLanguage(std::string &outLang) {
		File file;
		if (File::Open(file, Utils::Format(CONFIGNAME, Address::regionName.c_str()), File::READ) != File::SUCCESS) {
			return false;
		}

		u8 len = 0;
		if (file.Read(&len, sizeof(len)) != File::SUCCESS) {
			file.Close();
			return false;
		}

		if (len == 0) {
			file.Close();
			return false;
		}

		outLang.resize(len);
		if (file.Read(&outLang[0], len) != File::SUCCESS) {
			file.Close();
			return false;
		}

		file.Close();
		return true;
	}

	void CheckForLanguageFile(void) {
		if(!Directory::IsExists(V_STANDARD)) {
			Directory::Create(V_STANDARD);
		}

		if(!Directory::IsExists(V_DATA)) {
			Directory::Create(V_DATA);
		}

		if(!Directory::IsExists(Utils::Format(V_DIRECTORY, Address::regionName.c_str()))) {
			Directory::Create(Utils::Format(V_DIRECTORY, Address::regionName.c_str()));
		}

		if(!File::Exists(Utils::Format(CONFIGNAME, Address::regionName.c_str()))) {
			File::Create(Utils::Format(CONFIGNAME, Address::regionName.c_str()));
		}
	} 
}