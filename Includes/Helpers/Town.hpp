#ifndef TOWN_HPP
#define TOWN_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace TownName {
		void	Set(const std::string& name);
		void	UpdateReference(const std::string& tName, u8 tID1, u8 tID2 = -1);
	}
}
#endif