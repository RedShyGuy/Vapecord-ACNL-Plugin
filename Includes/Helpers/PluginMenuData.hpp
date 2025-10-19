#ifndef PLUGINMENUDATA_HPP
#define PLUGINMENUDATA_HPP

#include <CTRPluginFramework.hpp>
#include <functional> 
#include "LibCtrpfExtras/ColorExtras.hpp"

namespace CTRPluginFramework {
	extern MenuFolder *DEVC;

	class PluginMenuData {
		public:
			/**
			* \brief Set the VapecordStandardTheme for the FwkSettings
			*/
			static void SetVapecordStandardTheme(FwkSettings &settings);
	};
}

#endif