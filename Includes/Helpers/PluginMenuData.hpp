#ifndef PLUGINMENUDATA_HPP
#define PLUGINMENUDATA_HPP

#include <CTRPluginFramework.hpp>
#include <functional> 

namespace CTRPluginFramework {
	extern MenuFolder *DEVC;

	struct HotkeyDat {
		std::vector<std::string> Name;
		int Pos;
	};

	struct EntryData {
		MenuEntry *entry;
		Color IndexColor;
		std::string IndexName;
		std::string IndexNote;
		int CogID; //-1 if no cog cheat
		std::vector<std::string> IndexHotkeys;
	};

	struct FolderData {
		MenuFolder *folder;
		Color IndexColor;
		std::string IndexName;
		std::string IndexNote;
		u32 IsSubFolder;
		std::vector<EntryData> entryData;
	};

	using FuncPointer = void (*)(MenuEntry *);

	class PluginMenuData {
		public:
			/**
			* \brief Sets up PluginMenuData for folders (call this right after you init the menu)
			*/
			static void SetUp(MenuFolder *objfolder, bool isSubFolder = false);

			/**
			* \brief Removes the color of a string (only if the color is at the beginning)
			*/
			static std::string RemoveColorFromString(const std::string &str);

			/**
			* \brief Printf for Colors (add %c to string)
			*/
			static std::string FormatColor(const char *fmt, ...);

			/**
			* \brief Get the color of the given string (only if the color is at the beginning)
			*/
			static Color GetColorFromString(const std::string &str);

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

			/**
			* \brief Refreshes PluginMenuData strings
			*/
			static void UpdateAll(void);

			static std::vector<FolderData> folderData;
	};
}

#endif