#ifndef PLUGINMENUDATA_HPP
#define PLUGINMENUDATA_HPP

#include <CTRPluginFramework.hpp>
#include <functional> 

namespace CTRPluginFramework {
	extern MenuFolder *DEVC;

	struct FolderData {
		MenuFolder *folder;
		u32 IsSubFolder;
		std::vector<MenuEntry *> entryData;
	};

	class PluginMenuData {
		public:
			/**
			* \brief Sets up PluginMenuData for folders (call this right after you init the menu)
			*/
			static void SetUp(MenuFolder *objfolder, bool isSubFolder = false);

			/**
			* \brief Set the VapecordStandardTheme for the FwkSettings
			*/
			static void SetVapecordStandardTheme(FwkSettings &settings);

			/**
			* \brief Sets a custom Theme for the FwkSettings
			*/
			static void SetFWK(FwkSettings &settings, const Color arr[25]);

			/**
			* \brief Refreshes PluginMenuData colors and strings
			*/
			static void UpdateAll(const Color arr[12]);

			static void SaveEntrys(void);
			static void GetEntrys(std::vector<MenuEntry *> entry);
			static void Init(void);
			static std::vector<MenuEntry*> fullList;

			static std::vector<FolderData> folderData;
	};
}

#endif