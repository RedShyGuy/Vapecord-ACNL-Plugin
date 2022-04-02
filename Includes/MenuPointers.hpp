#ifndef MENUPOINTERS_HPP
#define MENUPOINTERS_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	extern MenuFolder *DEVC;

	extern Color FolderColors[12];

	struct HotkeyDat {
		std::vector<std::string> Name;
		int Pos;
	};

	struct EntryData {
		MenuEntry *entry;
		Color IndexColor;
		std::string IndexName;
		std::string IndexNote;
		std::vector<std::string> IndexHotkeys;
	};

	struct FolderData {
		MenuFolder *folder;
		Color IndexColor;
		std::string IndexName;
		std::string IndexNote;
		bool IsSubFolder;

		std::vector<EntryData *> entryData;
	};

	extern std::vector<FolderData *> folderData;

	using FuncPointer = void (*)(MenuEntry *);

	/**
	* \brief Get the gamefunction of this entry
	*/
	FuncPointer GetGameFunc(MenuEntry *entry);
	/**
	* \brief Get the menu funuction of this entry
	*/
	FuncPointer GetMenuFunc(MenuEntry *entry);

	std::string GetHotkeyName(Hotkey hotkey);

//Functions
	std::string RemoveColorFromString(const std::string &str);
	std::string FormatColor(const char *fmt, ...);
	Color GetColorFromString(const std::string &str);
	void SetVapecordStandardTheme(FwkSettings &settings);
	void SetFWK(FwkSettings &settings, const Color arr[25]);
	void UpdateAll(const Color arr[12]);
	void UpdateAll(void);
}

#endif