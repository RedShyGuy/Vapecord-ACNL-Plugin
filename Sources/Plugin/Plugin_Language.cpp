#include "Config.hpp"
#include "Color.h"
#include "Files.h"
#include "Language.hpp"

namespace CTRPluginFramework {	
	void SetLanguageEntry(MenuEntry *entry) {
		SetupLanguage(true);
	}

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
			MessageBox("Successfully set new language, please restart the game to see changes.").SetClear(ClearScreen::Top)();
		}
    }
}