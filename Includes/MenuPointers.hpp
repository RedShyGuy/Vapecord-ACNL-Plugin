#ifndef MENUPOINTERS_HPP
#define MENUPOINTERS_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
//MenuEntry Names for Color Changer/Language Changer
	extern MenuFolder *SAVEC; //SAVE_CODES
	extern MenuFolder *PSAVEC; //PLAYER_SAVE_CODES
	extern MenuFolder *MOVEC; //MOVEMENT_CODES
	extern MenuFolder *INVC; //INVENTORY_CODES
	extern MenuFolder *PLAYC; //PLAYER_CODES
	extern MenuFolder *ANIMC; //ANIMATION_CODES
	extern MenuFolder *SEEDC; //SEEDING_CODES
	extern MenuFolder *SEED1C; //SEED_CODES
	extern MenuFolder *DROPC; //DROP_CODES
	extern MenuFolder *TREEC; //TREE_CODES
	extern MenuFolder *MONC; //MONEY_CODES
	extern MenuFolder *ISLC; //ISLAND_CODES
	extern MenuFolder *NPCC; //NPC_CODES
	extern MenuFolder *FUNC; //FUN_CODES
	extern MenuFolder *EXTC; //EXTRA_CODES
	extern MenuFolder *FISC; //FISH_CODES
	extern MenuFolder *CHAC; //CHAT_CODES
	extern MenuFolder *MISC; //MISC_CODES
	extern MenuFolder *DEVC; //DEV_CODES

	extern Color FolderColors[12];

	struct HotkeyDat {
		std::vector<std::string> Name;
		int Pos;
	};

//Functions
	std::string RemoveColorFromString(const std::string &str);
	std::string FormatColor(const char *fmt, ...);
	Color GetColorFromString(const std::string &str);
	void SetVapecordStandardTheme(FwkSettings &settings);
	void SetFWK(FwkSettings &settings, const Color arr[25]);
	void UpdateAll(const Color arr[12]);
	void UpdateAll(void);
	void SaveCodesUpdate(Color color);
	void MovementCodesUpdate(Color color);
	void InventoryCodesUpdate(Color color);
	void PlayerCodesUpdate(Color color);
	void AnimationCodesUpdate(Color color);
	void SeedingCodesUpdate(Color color);
	void MoneyCodesUpdate(Color color);
	void IslandCodesUpdate(Color color);
	void NPCCodesUpdate(Color color);
	void FunCodesUpdate(Color color);
	void ExtraCodesUpdate(Color color);
	void MiscCodesUpdate(Color color);
}

#endif