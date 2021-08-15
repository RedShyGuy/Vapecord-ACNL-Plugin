#include "cheats.hpp"

namespace CTRPluginFramework {

	int UI_Pos = 0;
	c_RGBA* logoArray = nullptr;
	bool logoExists = true;

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
		int Pixels = 0,
			XLocation = 40, YLocation = 10,
			XResult = 50 + XLocation, YResult = 50 + YLocation;

		if(Splash.IsTop) {
			if(logoExists) {
				for(int X = XLocation; X < XResult; ++X) {
					for(int Y = YLocation; Y < YResult; ++Y) {
						Color cPix = Color(logoArray[Pixels].R, logoArray[Pixels].G, logoArray[Pixels].B, logoArray[Pixels].A);
						if(cPix != Color(0, 0, 0, 0))
							Splash.DrawPixel(X, Y, cPix);
							
						Pixels++;
					}
				}
			}
			
			Splash.DrawSysfont("Loading Plugin " << GetLoad(), XLocation - 33, logoExists ? (YLocation + 42) : (YLocation), Color(255, 0, 247));
		}
		return true;
	}
}