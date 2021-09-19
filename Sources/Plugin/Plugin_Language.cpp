#include "cheats.hpp"

namespace CTRPluginFramework {
	const std::string LanguageTXT[6] {
		"Vapecord/Language/japanese.txt",
		"Vapecord/Language/english.txt",
		"Vapecord/Language/french.txt",
		"Vapecord/Language/german.txt",
		"Vapecord/Language/italian.txt",
		"Vapecord/Language/spanish.txt"
	};

    void SetupLanguage(bool SetInMenu) {
        std::vector<std::string> v_Lang = {
			"Japanese", 
			"English", 
			"French",
			"German",
            "Italian",
            "Spanish"
		};

		u8 u_byte = f_Language::NoLang;
		ReadConfig(CONFIG::Language, u_byte);

		if(u_byte >= f_Language::MaxLang) //If byte is no language reset
			u_byte = f_Language::NoLang;

        if(u_byte == f_Language::NoLang || SetInMenu) { //if byte has no mode or re-choose in menu let user choose one
            static const std::string s_Lang = "Which language do you want to use?\n" << Color(pRed) << 
                                                "Red " << "means the file doesn't exist!\n" << Color(pGreen) << 
                                                "Green " << "means the file exists!"; //todo: set language

            int found = 0;
		//If File is missing it gets red if not it gets green
			for(int i = 0; i <= 5; ++i) {
				if(!File::Exists(LanguageTXT[i])) 
					v_Lang[i] = Color(pRed) << v_Lang[i];
				else {
					v_Lang[i] = Color(pGreen) << v_Lang[i];
					found += 1;
				}
			}
		//If no file was found do not even show the menu and just output error screen | abort | will prevent user to have to force shut down 3DS		
			if(found == 0) {
				Sleep(Milliseconds(100));
				MessageBox(Utils::Format("Error 505\nYou do not have any language files in your plugin directory. The Plugin can not work without one!\nGet more info and help on the Discord Server: %s\nGame will be closed now!", DISCORDINV)).SetClear(ClearScreen::Top)();
				Process::ReturnToHomeMenu();
			}

            Keyboard k_Lang(s_Lang, v_Lang);
			k_Lang.CanAbort(false);
            
            switch(k_Lang.Open()) {
				case 0:
					u_byte = f_Language::JapaneseLang; //Japanese MODE ON
				break;
				case 1:
					u_byte = f_Language::EnglishLang; //English MODE ON
				break;
				case 2:
					u_byte = f_Language::FrenchLang; //French MODE ON
				break;			
				case 3:
					u_byte = f_Language::GermanLang; //German MODE ON
                break;  
                case 4:
                    u_byte = f_Language::ItalianLang; //Italian MODE ON
                break;
                case 5:
                    u_byte = f_Language::SpanishLang; //Spanish MODE ON
                break;
            }

			WriteConfig(CONFIG::Language, u_byte); //write language mode
        }

		int pos = (u_byte - 1);

		if(!File::Exists(LanguageTXT[pos])) {
			Sleep(Milliseconds(100));
			MessageBox(Utils::Format("Error 404\nYou need the correct language text file for the plugin to work\nGet more info and help on the Discord Server: %s", DISCORDINV)).SetClear(ClearScreen::Top)();

			WriteConfig(CONFIG::Language, f_Language::NoLang); 

			SetupLanguage(true); //redo language choosing
		}

		Language->Parse(LanguageTXT[pos]);

		UpdateAll();
    }
}