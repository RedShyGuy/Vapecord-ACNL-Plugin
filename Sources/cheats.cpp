#include "cheats.hpp"
#include "Files.h"
#include "Helpers/PluginMenuData.hpp"
#include "Helpers/Game.hpp"
#include "RegionCodes.hpp"
#include "Color.h"

namespace CTRPluginFramework {
	bool turbo = false;
	bool fovbool = false;
	bool save = false;

	const std::string g_Empty = "-Empty-";

	const Color pColor[4] = {
		Color(pBlue), Color(pRed),		
		Color(pGreen), Color(pYellow),	
	};

	bool RainBowON = false;

/*
Randomizes colors of Menu Folders
*/
	void RainbowEntrys(Time time) {
		static Clock timer(time);
		if(timer.HasTimePassed(Milliseconds(500)) && RainBowON) {  
			Color color[12] = {
				Color::Rainbow(), Color::Rainbow(), Color::Rainbow(), Color::Rainbow(),
				Color::Rainbow(), Color::Rainbow(), Color::Rainbow(), Color::Rainbow(),
				Color::Rainbow(), Color::Rainbow(), Color::Rainbow(), Color::Rainbow()
			};
			
			PluginMenuData::UpdateAll(color);
			timer.Restart();
		}
	}

	void OnNewFrameCallback(Time ttime) {
		SendPlayerData(ttime);

		RainbowEntrys(ttime);

		const Screen& BottomScreen = OSD::GetBottomScreen();

		int coordx = 20, coordy1 = 0, coordy2 = 218;

		BottomScreen.DrawSysfont("Discord: " + std::string(DISCORDINV), coordx + 5, coordy1 + 3);

		BottomScreen.DrawSysfont("Plugin made by Lukas#4444", coordx + 5, coordy2 + 3);
	}

/*
Toggles the random color entry
*/
	void rainbow(MenuEntry *entry) {
		if(entry->WasJustActivated())
			RainBowON = true;
		else if(!entry->IsActivated())
			RainBowON = false;
	}
}