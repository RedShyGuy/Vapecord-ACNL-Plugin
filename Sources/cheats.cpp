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
		if(timer.HasTimePassed(Milliseconds(500))) {  
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

		if(PluginMenu::GetRunningInstance()->RainbowState())
			RainbowEntrys(ttime);
	}
}