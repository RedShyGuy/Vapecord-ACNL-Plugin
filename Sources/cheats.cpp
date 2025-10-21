#include "cheats.hpp"
#include "Files.h"
#include "Helpers/PluginMenuData.hpp"
#include "Helpers/Game.hpp"
#include "Color.h"

namespace CTRPluginFramework {
	bool turbo = false;
	bool fovbool = false;
	bool save = false;

	const Color pColor[4] = {
		Color(pBlue), Color(pRed),		
		Color(pGreen), Color(pYellow),	
	};

	const std::string g_Empty = "-Empty-";

	void OnNewFrameCallback(Time ttime) {
		SendPlayerData(ttime);
	}
}