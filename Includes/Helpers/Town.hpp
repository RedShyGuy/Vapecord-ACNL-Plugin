#ifndef TOWN_HPP
#define TOWN_HPP

#include <CTRPluginFramework.hpp>
#include <Helpers/GameStructs.hpp>

namespace CTRPluginFramework {
	namespace Town {
		ACNL_TownData *GetSaveData(void);
		void 		   EditName(const std::string& name);
	}
}
#endif