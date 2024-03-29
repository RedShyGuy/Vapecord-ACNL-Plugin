#include "Config.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "Files.h"

namespace CTRPluginFramework {
    static const Color c_Colorful[12] = {
		Color(0xDC143CFF), Color(0x98FB98FF), Color(0xFF7F50FF), Color(0x8A2BE2FF), 
		Color(0x1E90FFFF), Color(0x00FA9AFF), Color(0xFF69B4FF), Color(0xDAA520FF), 
		Color(0x00FA9AFF), Color(0xD2691EFF), Color(0xE10034FF), Color(0x20B2AAFF)
	};

	static const Color c_Luxury[12] = {
		Color(0xFFAA00FF), Color(0xFFFFFFFF), Color(0xFFAA00FF), Color(0xFFFFFFFF),
		Color(0xFFAA00FF), Color(0xFFFFFFFF), Color(0xFFAA00FF), Color(0xFFFFFFFF), 
		Color(0xFFAA00FF), Color(0xFFFFFFFF), Color(0xFFAA00FF), Color(0xFFFFFFFF)
	};

	static const Color c_Lite[12] = {
		Color(0xFFFFFFFF), Color(0xFFFFFFFF), Color(0xFFFFFFFF), Color(0xFFFFFFFF), 
		Color(0xFFFFFFFF), Color(0xFFFFFFFF), Color(0xFFFFFFFF), Color(0xFFFFFFFF), 
		Color(0xFFFFFFFF), Color(0xFFFFFFFF), Color(0xFFFFFFFF), Color(0xFFFFFFFF)
	};

    void ColorChange(Keyboard& keyboard, KeyboardEvent& event) {
        std::string& input = keyboard.GetMessage();
        
        if(event.type != KeyboardEvent::SelectionChanged)
            return;

		switch(event.selectedIndex) {
            default:
                input = "Which Version do you want to use?";
            break;
            case 0: 
                input = (c_Colorful[0] << "Colorful Version\n\n" << c_Colorful[1] << "All " << c_Colorful[2] << "Cheat-Names " << c_Colorful[3] << "are " << c_Colorful[4] << "colorful");
            break;
            case 1:
                input = (c_Lite[0] << "Lite Version\n\n" << c_Lite[1] << "All " << c_Lite[2] << "Cheat-Names " << c_Lite[3] << "are " << c_Lite[4] << "white");
            break;
            case 2:
                input = (c_Luxury[0] << "Luxury Version\n\n" << c_Luxury[1] << "Enjoy " << c_Luxury[2] << "luxurious " << c_Luxury[3] << "colors");
            break;
            case 3:
                input = (Color(0xFF4F22FF) << "Custom Version\n\n" << Color(0xFFCC22FF) << "Your " << Color(0x2AFF22FF) << "own " << Color(0x2282FFFF) << "colors");
            break;
        }
	}

    void SetupColors(bool SetInMenu) {
        std::vector<std::string> v_Color = {
			"Colorful Mode", //todo: set language
			"Lite Mode", //todo: set language 
			"Luxury Mode", //todo: set language
			"Custom Mode", //todo: set language
		};

        u8 u_byte = f_Color::NoMode;
		ReadConfig(CONFIG::Color, u_byte);

        if(u_byte >= f_Color::MaxColor) //If byte is no mode reset
			u_byte = f_Color::NoMode;

        if(u_byte == f_Color::NoMode || SetInMenu) { //if byte has no mode or re-choose in menu let user choose one
            static const std::string s_Color = "Which Version do you want to use?"; //todo: set language
            Keyboard k_Color(s_Color, v_Color);
			k_Color.CanAbort(false);
            k_Color.OnKeyboardEvent(ColorChange);
            
            switch(k_Color.Open()) {
				case 0:
					u_byte = f_Color::ColorMode; //COLORFUL MODE ON
				break;
				case 1:
					u_byte = f_Color::LiteMode; //LITE MODE ON
				break;
				case 2:
					u_byte = f_Color::LuxuryMode; //LUXURY MODE ON
				break;			
				case 3:
					u_byte = f_Color::CustomMode; //CUSTOM MODE ON
                break;  
            }

			WriteConfig(CONFIG::Color, u_byte); //write color mode
        }

    //loads mode to entrys
        switch(u_byte) {
            case f_Color::ColorMode:
                PluginMenuData::UpdateAll(c_Colorful); 
            break;
            case f_Color::LiteMode:
                PluginMenuData::UpdateAll(c_Lite);
            break;
            case f_Color::LuxuryMode:
                PluginMenuData::UpdateAll(c_Luxury);
            break;			
            case f_Color::CustomMode:
                if(!File::Exists(PATH_CUSTOMCOLOR)) {
                    MessageBox(Utils::Format("Error 606\nYou need the color.txt for the custom color mode to work!\nGet more info and help on the Discord Server: %s", DISCORDINV)).SetClear(ClearScreen::Top)();
                    SetupColors(true); //redo color choosing
                }
                colorparser->Parse(PATH_CUSTOMCOLOR);

                static const Color c_Custom[] = {
                    Color(std::stoul(colorparser->Get("SAVECODES"), 0, 16)), Color(std::stoul(colorparser->Get("MOVEMENTCODES"), 0, 16)), 
                    Color(std::stoul(colorparser->Get("INVENTORYCODES"), 0, 16)), Color(std::stoul(colorparser->Get("PLAYERCODES"), 0, 16)), 
                    Color(std::stoul(colorparser->Get("ANIMATIONCODES"), 0, 16)), Color(std::stoul(colorparser->Get("SEEDINGCODES"), 0, 16)), 
                    Color(std::stoul(colorparser->Get("MONEYCODES"), 0, 16)), Color(std::stoul(colorparser->Get("ISLANDCODES"), 0, 16)), 
                    Color(std::stoul(colorparser->Get("NPCCODES"), 0, 16)), Color(std::stoul(colorparser->Get("FUNCODES"), 0, 16)), 
					Color(std::stoul(colorparser->Get("EXTRACODES"), 0, 16)), Color(std::stoul(colorparser->Get("MISCCODES"), 0, 16))
                };
                PluginMenuData::UpdateAll(c_Custom);
            break;  
        }
    }

    const Color *GetFWK(void) {
		if(!File::Exists(PATH_FWKCOLOR)) 
			return nullptr;

		fwkparser->Parse(PATH_FWKCOLOR);

		static const Color c_Custom[26] = {
			Color(std::stoul(fwkparser->Get("MainTextColor"), 0, 16)), Color(std::stoul(fwkparser->Get("WindowTitleColor"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("MenuSelectedItemColor"), 0, 16)), Color(std::stoul(fwkparser->Get("MenuUnselectedItemColor"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("BackgroundMainColor"), 0, 16)), Color(std::stoul(fwkparser->Get("BackgroundSecondaryColor"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("BackgroundBorderColor"), 0, 16)), Color(std::stoul(fwkparser->Get("Keyboard_Background"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("Keyboard_KeyBackground"), 0, 16)), Color(std::stoul(fwkparser->Get("Keyboard_KeyBackgroundPressed"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("Keyboard_KeyText"), 0, 16)), Color(std::stoul(fwkparser->Get("Keyboard_KeyTextPressed"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("Keyboard_KeyTextDisabled"), 0, 16)), Color(std::stoul(fwkparser->Get("Keyboard_Cursor"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("Keyboard_Input"), 0, 16)), Color(std::stoul(fwkparser->Get("CustomKeyboard_BackgroundMain"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("CustomKeyboard_BackgroundSecondary"), 0, 16)), Color(std::stoul(fwkparser->Get("CustomKeyboard_BackgroundBorder"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("CustomKeyboard_KeyBackground"), 0, 16)), Color(std::stoul(fwkparser->Get("CustomKeyboard_KeyBackgroundPressed"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("CustomKeyboard_KeyText"), 0, 16)), Color(std::stoul(fwkparser->Get("CustomKeyboard_KeyTextPressed"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("CustomKeyboard_ScrollBarBackground"), 0, 16)), Color(std::stoul(fwkparser->Get("CustomKeyboard_ScrollBarThumb"), 0, 16)), 
			Color(std::stoul(fwkparser->Get("Notifications_DefaultBackground"), 0, 16)), Color(std::stoul(fwkparser->Get("Notifications_DefaultForeground"), 0, 16))
		};

		return c_Custom;
	}

	bool WrapFWK(void) {
		FwkSettings &settings = FwkSettings::Get();

		const Color* c_fwk = GetFWK();

		if(c_fwk == nullptr)
			return 0;

		PluginMenuData::SetFWK(settings, c_fwk);
		return 1;
	}

	void CustomFWK(bool SetInMenu) {
        u8 u_byte = fwk_Color::NoFWK;
		ReadConfig(CONFIG::FWKColor, u_byte);

        if(u_byte >= fwk_Color::MaxFWK) //If byte is no mode reset
			u_byte = fwk_Color::NoFWK;

        if(SetInMenu) {
			std::vector<std::string> FWKOpt {
				"Custom Colors",
				"Standard Colors",
			};

            Keyboard fwk_Lang("Lets you change the Main Colors of the plugin, to set custom ones change them in the FWKColors.txt!", FWKOpt);
			fwk_Lang.CanAbort(false);
            switch(fwk_Lang.Open()) {
				case 0:
					u_byte = fwk_Color::FWK_Custom; 		
				break;
				case 1:
					u_byte = fwk_Color::NoFWK; 
				break;
            }
        }

		if(u_byte == fwk_Color::FWK_Custom) {	
			if(!WrapFWK()) {
				MessageBox(Utils::Format("Error 101\nYou need the FWKColors.txt for the custom main color mode to work!\nGet more info and help on the Discord Server: %s\nStandard Colors will load now!", DISCORDINV)).SetClear(ClearScreen::Top)();

				WriteConfig(CONFIG::FWKColor, fwk_Color::NoFWK);
			}
			else {
				WriteConfig(CONFIG::FWKColor, fwk_Color::FWK_Custom);
			}
		}
		else {
			FwkSettings &settings = FwkSettings::Get();
			PluginMenuData::SetVapecordStandardTheme(settings);

			WriteConfig(CONFIG::FWKColor, fwk_Color::NoFWK);
		}
    }
}
