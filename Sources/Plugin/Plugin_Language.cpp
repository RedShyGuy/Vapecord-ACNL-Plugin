#include "Config.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "Color.h"
#include "Files.h"
#include "Language.hpp"

namespace CTRPluginFramework {	
    void SetupLanguage(bool SetInMenu) {
		f_Language language = f_Language::NoLang;
		ReadLanguage(language);

		if(language >= f_Language::MaxLang) {//If byte is no language reset
			language = f_Language::NoLang;
		}

		static const std::vector<std::pair<std::string, std::string>> languages = {
			{"jp", "Japanese"},
			{"en", "English"},
			{"fr", "French"},
			{"de", "German"},
			{"it", "Italian"},
			{"es", "Spanish"},
			{"kr", "Korean"}
		};

        if(language == f_Language::NoLang || SetInMenu) { //if byte has no mode or re-choose in menu let user choose one
            if (!File::Exists(PATH_LANGUAGE_BIN)) {
				MessageBox(Utils::Format("Error 577\nThe language.bin is missing. The Plugin can not work without it!\nGet more info and help on the Discord Server: %s\nGame will be closed now!", DISCORDINV)).SetClear(ClearScreen::Top)();
				Process::ReturnToHomeMenu();
			}
			
			std::vector<std::string> values;
			for (const auto& pair : languages) {
				values.push_back(pair.second);
			}

            Keyboard keyboard("Which language do you want to use?", values);
			keyboard.CanAbort(false);

			int sel = keyboard.Open();
			if (sel >= 0 && sel < (int)languages.size()) {
            	language = static_cast<f_Language>(sel + 1);
			}

			WriteLanguage(language); //write language mode
        }

 		int index = (int)language - 1;
		if (index < 0 || index >= (int)languages.size() ||
        	!Language::getInstance()->loadFromBinary(PATH_LANGUAGE_BIN, languages[index].first.c_str())) {
			MessageBox(Utils::Format("Error 404\nThe language.bin is missing the selected language\nGet more info and help on the Discord Server: %s", DISCORDINV)).SetClear(ClearScreen::Top)();

			WriteLanguage(f_Language::NoLang); 
			SetupLanguage(true); //redo language choosing
		}
    }
}