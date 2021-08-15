#ifndef TOWN_HPP
#define TOWN_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace TownName {
		void	Set(const std::string& name);
		void	UpdateReference(const std::string& tName, s8 tID1, s8 tID2 = -1);
	}
}
#endif