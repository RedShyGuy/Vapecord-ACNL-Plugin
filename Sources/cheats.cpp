#include "cheats.hpp"
#include "Files.h"
#include "Helpers/Game.hpp"
#include "Color.h"

namespace CTRPluginFramework {
	bool turbo = false;
	bool fovbool = false;
	bool saveMenuDisabled = false;

	const Color pColor[4] = {
		Color(pBlue), Color(pRed),		
		Color(pGreen), Color(pYellow),	
	};
}