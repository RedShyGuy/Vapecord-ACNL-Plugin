#include "Helpers/Game.hpp"

namespace CTRPluginFramework {

	int UI_Pos = 0;

	std::string GetLoad(void) {
		switch(UI_Pos) {
			case 0: return "\uE020";
			case 1: return "\uE021";		
			case 2: return "\uE022";		
			case 3: return "\uE023";		
			case 4: return "\uE024";		
			case 5: return "\uE025";
			case 6: return "\uE026";
			case 7: return "\uE027";
		}
		return "";
	}

	bool OSD_SplashScreen(const Screen &Splash) {
		if(Splash.IsTop) {
			Splash.DrawSysfont("Waiting for game to load " << GetLoad(), 7, 10, Color(255, 0, 247));
		}
			
		return true;
	}
}