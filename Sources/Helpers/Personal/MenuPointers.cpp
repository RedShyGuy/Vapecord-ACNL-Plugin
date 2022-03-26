#include <cstdarg>
#include "MenuPointers.hpp"
#include "TextFileParser.hpp"
#include "Helpers/QuickMenu.hpp"

namespace CTRPluginFramework {
//MenuEntry Names for Color Changer/Language Changer
	MenuFolder *SAVEC; //SAVE_CODES
	MenuFolder *MOVEC; //MOVEMENT_CODES	
	MenuFolder *INVC; //INVENTORY_CODES	
	MenuFolder *PLAYC; //PLAYER_CODES
	MenuFolder *PSAVEC; //PLAYER_SAVE_CODES
	MenuFolder *ANIMC; //ANIMATION_CODES
	MenuFolder *SEEDC; //SEEDING_CODES
	MenuFolder *SEED1C; //SEED_CODES
	MenuFolder *DROPC; //DROP_CODES
	MenuFolder *TREEC; //TREE_CODES
	MenuFolder *MONC; //MONEY_CODES
	MenuFolder *ISLC; //ISLAND_CODES
	MenuFolder *NPCC; //NPC_CODES
	MenuFolder *FUNC; //FUN_CODES
	MenuFolder *EXTC; //EXTRA_CODES
	MenuFolder *FISC; //FISH_CODES
	MenuFolder *CHAC; //CHAT_CODES
	MenuFolder *MISC; //MISC_CODES
//Dev Codes Folder
	MenuFolder *DEVC; //DEV_CODES

	Color FolderColors[12] = {
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0),
		Color(220, 20, 60, 0)
	};

	FuncPointer GetGameFunc(MenuEntry *entry) {
		return *(FuncPointer *)(*(u32 *)((u32)entry + 0x14) + 0x5C);
	}

	FuncPointer GetMenuFunc(MenuEntry *entry) {
		return *(FuncPointer *)(*(u32 *)((u32)entry + 0x14) + 0x60);
	}

//Functions for easier color changes
	void UpdateAll(const Color arr[12]) {
		for(int i = 0; i < (sizeof(FolderColors) / sizeof(FolderColors[0])); ++i)
			FolderColors[i] = arr[i];

		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(menu == nullptr) //if menu didn't load yet
			return;

		SaveCodesUpdate(FolderColors[0]);
		MovementCodesUpdate(FolderColors[1]);
		InventoryCodesUpdate(FolderColors[2]);
		PlayerCodesUpdate(FolderColors[3]);
		AnimationCodesUpdate(FolderColors[4]);
		SeedingCodesUpdate(FolderColors[5]);
		MoneyCodesUpdate(FolderColors[6]);
		IslandCodesUpdate(FolderColors[7]);
		NPCCodesUpdate(FolderColors[8]);
		FunCodesUpdate(FolderColors[9]);
		ExtraCodesUpdate(FolderColors[10]);
		MiscCodesUpdate(FolderColors[11]);
	}
//Update only text
	void UpdateAll(void) {
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(menu == nullptr) //if menu didn't load yet
			return;

		std::vector<MenuFolder *> Folders = menu->GetFolderList();

		SaveCodesUpdate(GetColorFromString(Folders[0]->Name()));
		MovementCodesUpdate(GetColorFromString(Folders[1]->Name()));
		InventoryCodesUpdate(GetColorFromString(Folders[2]->Name()));
		PlayerCodesUpdate(GetColorFromString(Folders[3]->Name()));
		AnimationCodesUpdate(GetColorFromString(Folders[4]->Name()));
		SeedingCodesUpdate(GetColorFromString(Folders[5]->Name()));
		MoneyCodesUpdate(GetColorFromString(Folders[6]->Name()));
		IslandCodesUpdate(GetColorFromString(Folders[7]->Name()));
		NPCCodesUpdate(GetColorFromString(Folders[8]->Name()));
		FunCodesUpdate(GetColorFromString(Folders[9]->Name()));
		ExtraCodesUpdate(GetColorFromString(Folders[10]->Name()));
		MiscCodesUpdate(GetColorFromString(Folders[11]->Name()));
	}

	void SetVapecordStandardTheme(FwkSettings &settings) {
		FwkSettings::SetThemeDefault();
		settings.BackgroundSecondaryColor = Color::Black; 
		settings.BackgroundBorderColor = Color(255, 0, 247);
		settings.WindowTitleColor = Color::Yellow;
		settings.MenuSelectedItemColor = Color::Lime;
		settings.MenuUnselectedItemColor = Color::White;
		settings.Keyboard.KeyText = Color::Lime;
		settings.Keyboard.KeyTextPressed = Color::Lime;
		settings.Keyboard.Cursor = Color::Lime;
		settings.Keyboard.Input = Color::Lime;
		settings.CustomKeyboard.BackgroundSecondary = Color::Black;
		settings.CustomKeyboard.BackgroundBorder = Color(255, 0, 247);
		settings.CustomKeyboard.KeyBackground = Color::Black;
		settings.CustomKeyboard.KeyBackgroundPressed = Color(0x505050A0);
		settings.CustomKeyboard.KeyText = Color::White;
		settings.CustomKeyboard.KeyTextPressed = Color::White;
	}

	void SetFWK(FwkSettings &settings, const Color arr[25]) {
		settings.MainTextColor = arr[0];
		settings.WindowTitleColor = arr[1];
		settings.MenuSelectedItemColor = arr[2];
		settings.MenuUnselectedItemColor = arr[3];
		settings.BackgroundMainColor = arr[4];
		settings.BackgroundSecondaryColor = arr[5];
		settings.BackgroundBorderColor = arr[6];
		settings.Keyboard.Background = arr[7];
		settings.Keyboard.KeyBackground = arr[8];
		settings.Keyboard.KeyBackgroundPressed = arr[9];
		settings.Keyboard.KeyText = arr[10];	
		settings.Keyboard.KeyTextPressed = arr[11];
		settings.Keyboard.KeyTextDisabled = arr[12];
		settings.Keyboard.Cursor = arr[13];
		settings.Keyboard.Input = arr[14];
		settings.CustomKeyboard.BackgroundMain = arr[15];
		settings.CustomKeyboard.BackgroundSecondary = arr[16];
		settings.CustomKeyboard.BackgroundBorder = arr[17];
		settings.CustomKeyboard.KeyBackground = arr[18];
		settings.CustomKeyboard.KeyBackgroundPressed = arr[19];
		settings.CustomKeyboard.KeyText = arr[20];
		settings.CustomKeyboard.KeyTextPressed = arr[21];
		settings.CustomKeyboard.ScrollBarBackground = arr[22];
		settings.CustomKeyboard.ScrollBarThumb = arr[23];
		settings.Notifications.DefaultBackground = arr[24];
		settings.Notifications.DefaultForeground = arr[25];
	}

	std::string FormatColor(const char *fmt, ...) {
		std::string res;

		va_list list;
		va_start(list, fmt);
		while(*fmt != '\0') {
			if(*fmt != '%') {
				res += *fmt;
				fmt++;
				continue;
			}

			fmt++;

			if(*fmt == '\0') 
				break;

			if(*fmt == 'c') {
				void* p = va_arg(list, void *);

				u32 RawColor = (u32)p;

				char strColor[5] = { 
					0x1B, 
					std::max((u8)1, (u8)(RawColor & 0x000000FF)), 
					std::max((u8)1, (u8)((RawColor & 0x0000FF00) >> 8)),
					std::max((u8)1, (u8)((RawColor & 0x00FF0000) >> 16)), 
				};
				res += strColor[0];
				res += strColor[1];
				res += strColor[2];
				res += strColor[3];
       		}      

			fmt++;
		}

		return res;
	}

	Color GetColorFromString(const std::string &str) {
		std::vector<char> r_char(str.begin(), str.end());

		if((u8)r_char[0] == 0x1B) { //if colorcode
			u32 c_code = ((u8)r_char[1] << 24 | (u8)r_char[2] << 16 | (u8)r_char[3] << 8 | 0xFF);
			return Color(c_code);
		}
		return Color(0xFFFFFFFF);
	}

	std::string RemoveColorFromString(const std::string &str) {
		std::vector<char> r_char(str.begin(), str.end());
		if((u8)r_char[0] == 0x1B) { //if colorcode
			return str.substr(4);
		}
		return str;
	}

	enum Folders_ {
		SaveCodes = 0,
		MovementCodes,
		InventoryCodes,
		PlayerSaveCodes,
		PlayerCodes,
		AnimationCodes,
		SeedCodes,
		SeedingCodes,
		DropCodes,
		TreeCodes,
		MoneyCodes,
		IslandCodes,
		NPCCodes,
		FunCodes,
		FishCodes,
		ChatCodes,
		ExtraCodes,
		MiscCodes
	};

	std::vector<MenuEntry *> MenuEntrys[18];
	bool loadedEntry = false;

	void LoadEntrys(void) {
		MenuEntrys[SaveCodes] = SAVEC->GetEntryList();
		MenuEntrys[MovementCodes] = MOVEC->GetEntryList();
		MenuEntrys[InventoryCodes] = INVC->GetEntryList();
		MenuEntrys[PlayerSaveCodes] = PSAVEC->GetEntryList();
		MenuEntrys[PlayerCodes] = PLAYC->GetEntryList();
		MenuEntrys[AnimationCodes] = ANIMC->GetEntryList();
		//MenuEntrys[SeedCodes] = SEEDC->GetEntryList();
		MenuEntrys[SeedingCodes] = SEED1C->GetEntryList();
		MenuEntrys[DropCodes] = DROPC->GetEntryList();
		MenuEntrys[TreeCodes] = TREEC->GetEntryList();
		MenuEntrys[MoneyCodes] = MONC->GetEntryList();
		MenuEntrys[IslandCodes] = ISLC->GetEntryList();
		MenuEntrys[NPCCodes] = NPCC->GetEntryList();
		MenuEntrys[FunCodes] = FUNC->GetEntryList();
		MenuEntrys[FishCodes] = FISC->GetEntryList();
		MenuEntrys[ChatCodes] = CHAC->GetEntryList();
		MenuEntrys[ExtraCodes] = EXTC->GetEntryList();
		MenuEntrys[MiscCodes] = MISC->GetEntryList();
		loadedEntry = true;
	}

	void UpdateEntry(MenuEntry *entry, const std::string& name, const std::string& note) {
		if(entry == nullptr)
			return;

		std::string hotkey = "";
		int count = entry->Hotkeys.Count();

		if(count == 1) //If Entry has only 1 Hotkey append string of it to name
			hotkey = entry->Hotkeys[0].ToString();

		entry->Name() = name + " " + hotkey;
		entry->Note() = note;
		entry->RefreshNote();
	} 

	void UpdateHotkey(int pos, HotkeyDat data) {
		std::vector<MenuEntry *> F_entrys = MenuEntrys[pos];

		int count = F_entrys[data.Pos]->Hotkeys.Count();

		if(count == 0)
			return;

		for(int i = 0; i < data.Name.size(); ++i)
			F_entrys[data.Pos]->Hotkeys[i] = Language->Get(data.Name[i]);
	}

	void SaveCodesUpdate(Color color) {	
		std::vector<MenuEntry *> Entrys = MenuEntrys[SaveCodes];

		const static std::string Names[2][15] = {
			{ "TOWN_NAME_CHANGER", "SAVE_BACKUP_NAME", "BULL_BOARD_DUMPER", 
			"TREESIZE_NAME", "CHANGE_NATIVE_NAME", "PWP_UNLOCK_NAME", 
			"GRASS_CHANGER_NAME", "CARAVAN_SET", "CAMPING_SET", 
			"SHOP_UPGRADE", /*"HOUSE_EDITOR_NAME", */"QR_MACHINE_NAME", 
			"BUILDING_MOD_NAME", "FILL_MUSEUM_NAME", "MAYOR_PERMIT_NAME", 
			"REAL_TIME_ACRE" },

			{ "TOWN_NAME_CHANGER_NOTE", "SAVE_BACKUP_NOTE", "BULL_BOARD_DUMPER_NOTE", 
			"TREESIZE_NOTE", "CHANGE_NATIVE_NOTE", "PWP_UNLOCK_NOTE", 
			"GRASS_CHANGER_NOTE", "CARAVAN_SET_NOTE", "CAMPING_SET_NOTE", 
			"SHOP_UPGRADE_NOTE", /*"HOUSE_EDITOR_NOTE", */"QR_MACHINE_NOTE", 
			"BUILDING_MOD_NOTE", "FILL_MUSEUM_NOTE", "MAYOR_PERMIT_NOTE", 
			"REAL_TIME_ACRE_NOTE" },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));
	
		SAVEC->Name() = Color(color) << Language->Get("SAVE_CODES");
	}
	
	void MovementCodesUpdate(Color color) {	
		std::vector<MenuEntry *> Entrys = MenuEntrys[MovementCodes];	
		const static std::string Names[2][12] = {
			{ "CANT_PUSH", "COORD_MOD", "MOON_JUMP", "TOUCH_WARP", 
			"WALK_OVER", "MOVEMENT_CHANGE", "WALK_PARTICLE_CHANGE", "PLAYER_TELEPORT", 
			"VISIBILITY_MOD", "SPEED_MOD", "ROOM_WARPING", "SHOVEL_KNOCKBACK" },

			{ "CANT_PUSH_NOTE", "COORD_MOD_NOTE", "MOON_JUMP_NOTE", "TOUCH_WARP_NOTE", 
			"WALK_OVER_NOTE", "MOVEMENT_CHANGE_NOTE", "WALK_PARTICLE_CHANGE_NOTE", "PLAYER_TELEPORT_NOTE", 
			"VISIBILITY_MOD_NOTE", "SPEED_MOD_NOTE", "ROOM_WARPING_NOTE", "SHOVEL_KNOCKBACK_NOTE" },
		};

		const static HotkeyDat Hotkeys[8] = {
			{ { "COORD_MOD_KEY1", "COORD_MOD_KEY2", "COORD_MOD_KEY3", "COORD_MOD_KEY4", "COORD_MOD_KEY5"}, 1 },
			{ { "MOON_JUMP_KEY1", "MOON_JUMP_KEY2" }, 2 },
			{ { "WALK_OVER" }, 4 },
			{ { "MOVEMENT_CHANGE" }, 5 },
			{ { "WALK_PARTICLE_CHANGE" }, 6 },
			{ { "PLAYER_TELEPORT_KEY1", "PLAYER_TELEPORT_KEY2" }, 7 },
			{ { "VISIBILITY_MOD_KEY1" }, 8 },
			{ { "ROOM_WARPING" }, 10 },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		for(int i = 0; i < 8; ++i) 
			UpdateHotkey(MovementCodes, Hotkeys[i]);

		MOVEC->Name() = Color(color) << Language->Get("MOVEMENT_CODES");
	}
	
	void InventoryCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[InventoryCodes];

		const static std::string Names[2][9] = {
			{ "TEXT_2_ITEM", "DUPE_ITEMS", "CATALOG_TO_POCKET", "CHAT_T2I", 
			"CLEAR_INV_NAME", "ITEM_SETTINGS", "SAVE_MENU_CHANGER", "GET_SET", 
			"CUSTOM_BUTTON" },

			{ "TEXT_2_ITEM_NOTE", "DUPE_ITEMS_NOTE", "CATALOG_TO_POCKET_NOTE", 
			"CHAT_T2I_NOTE", "CLEAR_INV_NOTE", "ITEM_SETTINGS_NOTE", "SAVE_MENU_CHANGER_NOTE", 
			"GET_SET_NOTE", "CUSTOM_BUTTON_NOTE" },
		};

		const static HotkeyDat Hotkeys[4] = {
			{ { "TEXT_2_ITEM_KEY1", "TEXT_2_ITEM_KEY2", "TEXT_2_ITEM_KEY3", "TEXT_2_ITEM_KEY4" }, 0 },
			{ { "DUPE_ITEMS_KEY1", "DUPE_ITEMS_KEY2" }, 1 },
			{ { "CATALOG_TO_POCKET_KEY1", "CATALOG_TO_POCKET_KEY2" }, 2 },
			{ { "CHAT_T2I_KEY1" }, 3 },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		for(int i = 0; i < 4; ++i) 
			UpdateHotkey(InventoryCodes, Hotkeys[i]);

		INVC->Name() = Color(color) << Language->Get("INVENTORY_CODES");
	}
	
	void PlayerCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys1 = MenuEntrys[PlayerSaveCodes];
		std::vector<MenuEntry *> Entrys2 = MenuEntrys[PlayerCodes];

		const static std::string Names1[2][13] = {
			{ "NAME_CHANGER", "PLAYER_APPEARANCE", "RANDOM_PLAYER", 
			"PLAYER_BACKUP_NAME", "TPC_MESSAGE", "TPC_SETTINGS", "DESIGN_DUMP", 
			"EMOTION_LIST", "ENCY_LIST", "DREAM_CODE", "DEBUG_MENU", 
			"SONG_LIST_NAME", "FILL_CATALOG_NAME",  },

			{ "NAME_CHANGER_NOTE", "PLAYER_APPEARANCE_NOTE", "RANDOM_PLAYER_NOTE", 
			"PLAYER_BACKUP_NOTE", "TPC_MESSAGE_NOTE", "TPC_SETTINGS_NOTE", "DESIGN_DUMP_NOTE", 
			"EMOTION_LIST_NOTE", "ENCY_LIST_NOTE", "DREAM_CODE_NOTE", "DEBUG_MENU_NOTE", 
			"SONG_LIST_NOTE", "FILL_CATALOG_NOTE",  },
		};

		const static std::string Names2[2][7] = {
			{ "PLAYER_INFO", /*"PLAYER_LOADER",*/ "NECK_POSITION", 
			"CUSTOM_HAIR_EYE", "WEAR_HELMET", "FAINT", 
			"PLAYERS_ON_MAP", "NEVER_BEDHEAD" },

			{ "PLAYER_INFO_NOTE", /*"PLAYER_LOADER_NOTE",*/ "NECK_POSITION_NOTE", 
			"CUSTOM_HAIR_EYE_NOTE", "WEAR_HELMET_NOTE", "FAINT_NOTE", 
			"PLAYERS_ON_MAP_NOTE", "NEVER_BEDHEAD_NOTE" },
		};

		const static HotkeyDat Hotkeys[3] = {
			{ { "NECK_POSITION" }, 1 },
			{ { "CUSTOM_HAIR_EYE_KEY1" }, 2 },
			{ { "FAINT" }, 4 },
		};

		for(int i = 0; i < Entrys1.size(); ++i) 
			UpdateEntry(Entrys1[i], Color(color) << Language->Get(Names1[0][i]), Language->Get(Names1[1][i]));
		for(int i = 0; i < Entrys2.size(); ++i) 
			UpdateEntry(Entrys2[i], Color(color) << Language->Get(Names2[0][i]), Language->Get(Names2[1][i]));

		for(int i = 0; i < 3; ++i) 
			UpdateHotkey(PlayerCodes, Hotkeys[i]);

		PLAYC->Name() = Color(color) << Language->Get("PLAYER_CODES");
		PSAVEC->Name() = Color(color) << Language->Get("PLAYER_SAVE_CODES");
	}
	
	void AnimationCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[AnimationCodes];

		const static std::string Names[2][7] = {
			{ "PLAYER_SELECTOR", "ANTI_ANIM", "ANIMATION_MOD", "EMOTION_LOOP", 
			"IDLE", "SLOW_MO_ANIM", "ANIM_ON_ALL" },

			{ "PLAYER_SELECTOR_NOTE", "ANTI_ANIM_NOTE", "ANIMATION_MOD_NOTE", 
			"EMOTION_LOOP_NOTE", "IDLE_NOTE", "SLOW_MO_ANIM_NOTE", "ANIM_ON_ALL_NOTE" },
		};

		const static HotkeyDat Hotkeys[6] = {
			{ { "PLAYER_SELECTOR_KEY1", "PLAYER_SELECTOR_KEY2" }, 0 },
			{ { "ANIMATIONS_NOTE_HOTKEY1", "ANIMATIONS_NOTE_HOTKEY2", "ANIMATIONS_NOTE_HOTKEY3", "ANIMATIONS_NOTE_HOTKEY4" }, 2 },
			{ { "EMOTION_LOOP" }, 3 },
			{ { "IDLE" }, 4 },
			{ { "SLOW_MO_ANIM" }, 5 },
			{ { "ANIM_ON_ALL_KEY1", "ANIM_ON_ALL_KEY2", "ANIM_ON_ALL_KEY3" }, 6 },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		for(int i = 0; i < 6; ++i) 
			UpdateHotkey(AnimationCodes, Hotkeys[i]);

		ANIMC->Name() = Color(color) << Language->Get("ANIMATION_CODES");	
	}
	
	void SeedingCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys1 = MenuEntrys[SeedingCodes];
		std::vector<MenuEntry *> Entrys2 = MenuEntrys[DropCodes];
		std::vector<MenuEntry *> Entrys3 = MenuEntrys[TreeCodes];

		const static std::string Names1[2][5] = {
			{ "PICK_SEEDER", "WALK_SEEDER", "FIREWORK_SEEDER", "MAP_EDITOR", 
			"ALL_SEEDER" },

			{ "PICK_SEEDER_NOTE", "WALK_SEEDER_NOTE", "FIREWORK_SEEDER_NOTE", 
			"MAP_EDITOR_NOTE", "ALL_SEEDER_NOTE" },
		};
		const static std::string Names2[2][6] = {
			{ "ITEM_SEQUENCER", "DROP_MODS", "DROP_ITEMS", "AUTO_DROP", 
			"TOUCH_DROP", "SLOT_DROP" },

			{ "ITEM_SEQUENCER_NOTE", "DROP_MODS_NOTE", "DROP_ITEMS_NOTE", 
			"AUTO_DROP_NOTE", "TOUCH_DROP_NOTE", "SLOT_DROP_NOTE" },
		};
		const static std::string Names3[2][4] = {
			{ "INF_FRUIT_TREE", "AXE_TREE_SHAKE", "FRUIT_TREE_MOD", "INST_TREE_CHOP" },

			{ "INF_FRUIT_TREE_NOTE", "AXE_TREE_SHAKE_NOTE", "FRUIT_TREE_MOD_NOTE", 
			"INST_TREE_CHOP_NOTE" },
		};

		const static HotkeyDat Hotkeys1[4] = {
			{ { "PICK_SEEDER_KEY1", "PICK_SEEDER_KEY2", "PICK_SEEDER_KEY3", "PICK_SEEDER_KEY4", "PICK_SEEDER_KEY5" }, 0 },
			{ { "WALK_SEEDER" }, 1 },
			{ { "MAP_EDITOR_KEY1", "MAP_EDITOR_KEY2", "MAP_EDITOR_KEY3", "MAP_EDITOR_KEY4", "MAP_EDITOR_KEY5", "MAP_EDITOR_KEY6", "MAP_EDITOR_KEY7", "MAP_EDITOR_KEY8", "MAP_EDITOR_KEY9", "MAP_EDITOR_KEY10", "MAP_EDITOR_KEY11" }, 3 },
			{ { "ALL_SEEDER_KEY1" }, 4 }
		};

		const static HotkeyDat Hotkeys2[5] = {
			{ { "DROP_MODS_KEY1", "DROP_MODS_KEY2", "DROP_MODS_KEY3", "DROP_MODS_KEY4" }, 1 },
			{ { "DROP_ITEMS_KEY1", "DROP_ITEMS_KEY2" }, 2 },
			{ { "AUTO_DROP_KEY1", "AUTO_DROP_KEY2" }, 3 },
			{ { "TOUCH_DROP_KEY1" }, 4 },
			{ { "SLOT_DROP_KEY1", "SLOT_DROP_KEY2" }, 5 },
		};

		const static HotkeyDat Hotkeys3[1] = {
			{ { "FRUIT_TREE_MOD_KEY1" }, 2 },
		};

		for(int i = 0; i < Entrys1.size(); ++i) 
			UpdateEntry(Entrys1[i], Color(color) << Language->Get(Names1[0][i]), Language->Get(Names1[1][i]));
		for(int i = 0; i < Entrys2.size(); ++i) 
			UpdateEntry(Entrys2[i], Color(color) << Language->Get(Names2[0][i]), Language->Get(Names2[1][i]));
		for(int i = 0; i < Entrys3.size(); ++i) 
			UpdateEntry(Entrys3[i], Color(color) << Language->Get(Names3[0][i]), Language->Get(Names3[1][i]));

		for(int i = 0; i < 4; ++i) 
			UpdateHotkey(SeedingCodes, Hotkeys1[i]);
		for(int i = 0; i < 5; ++i) 
			UpdateHotkey(DropCodes, Hotkeys2[i]);
		for(int i = 0; i < 1; ++i) 
			UpdateHotkey(TreeCodes, Hotkeys3[i]);

		SEEDC->Name() = Color(color) << Language->Get("SEEDING_CODES");
		SEED1C->Name() = Color(color) << Language->Get("SEED_CODES");
		DROPC->Name() = Color(color) << Language->Get("DROP_CODES");
		TREEC->Name() = Color(color) << Language->Get("TREE_CODES");
	}
	
	void MoneyCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[MoneyCodes];

		const static std::string Names[2][6] = {
			{ "WALLET", "BANK", "MEOW_COUPONS", "BADGES", 
			"MEDALS", "TURNIPS" },

			{ "WALLET_NOTE", "BANK_NOTE", "MEOW_COUPONS_NOTE", 
			"BADGES_NOTE", "MEDALS_NOTE", "TURNIPS_NOTE" },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		MONC->Name() = Color(color) << Language->Get("MONEY_CODES");
	}
	
	void IslandCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[IslandCodes];

		const static std::string Names[2][10] = {
			{ "UNLOCK_ISLAND", "FILL_INV_ORE", "FILL_INV_FRUIT", 
			"HACKER_ISLAND", "ISLAND_COUNTRY", "FREE_KAPPN", 
			"ISLAND_SHOP_MOD", "ALL_TOURS", "ISLAND_ACRE", 
			"ISLAND_BUILDING" },

			{ "UNLOCK_ISLAND_NOTE", "FILL_INV_ORE_NOTE", "FILL_INV_FRUIT_NOTE", 
			"HACKER_ISLAND_NOTE", "ISLAND_COUNTRY_NOTE", "FREE_KAPPN_NOTE", 
			"ISLAND_SHOP_MOD_NOTE", "ALL_TOURS_NOTE", "ISLAND_ACRE_NOTE", 
			"ISLAND_BUILDING_NOTE" },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		ISLC->Name() = Color(color) << Language->Get("ISLAND_CODES");
	}

	void NPCCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[NPCCodes];

		const static std::string Names[2][5] = {
			{ "NPC_SELECTOR", "NPC_ANIMATION", "NPC_COORDINATE", 
			"NPC_TELEPORT", "NPC_ROTATION" },

			{ "NPC_SELECTOR_NOTE", "NPC_ANIMATION_NOTE", "NPC_COORDINATE_NOTE", 
			"NPC_TELEPORT_NOTE", "NPC_ROTATION_NOTE" },
		};

		const static HotkeyDat Hotkeys[5] = {
			{ { "NPC_SELECTOR_KEY1" }, 0 },
			{ { "NPC_ANIMATION_KEY1" }, 1 },
			{ { "NPC_COORDINATE_KEY1", "NPC_COORDINATE_KEY2", "NPC_COORDINATE_KEY3", "NPC_COORDINATE_KEY4", "NPC_COORDINATE_KEY5" }, 2 },
			{ { "NPC_TELEPORT_KEY1" }, 3 },
			{ { "NPC_ROTATION_KEY1" }, 4 }
		};

		for(int i = 0; i < Entrys.size(); ++i)
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		for(int i = 0; i < 5; ++i) 
			UpdateHotkey(NPCCodes, Hotkeys[i]);

		NPCC->Name() = Color(color) << Language->Get("NPC_CODES");
	}
	
	void FunCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[FunCodes];

		const static std::string Names[2][7] = {
			{ "SIZE_CODES", "T_POSE", "TAKE_TPC_PIC", 
			"MAX_TURBO", "MULTI_PRESS", "ULTIMATE_POPPER", 
			"CAMERA_MOD" },

			{ "SIZE_CODES_NOTE", "T_POSE_NOTE", "TAKE_TPC_PIC_NOTE", 
			"MAX_TURBO_NOTE", "MULTI_PRESS_NOTE", "ULTIMATE_POPPER_NOTE", 
			"CAMERA_MOD_NOTE" },
		};

		const static HotkeyDat Hotkeys[2] = {
			{ { "TAKE_TPC_PIC_KEY1", "TAKE_TPC_PIC_KEY2" }, 2 },
			{ { "ULTIMATE_POPPER_KEY1" }, 5 },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		for(int i = 0; i < 2; ++i) 
			UpdateHotkey(FunCodes, Hotkeys[i]);

		FUNC->Name() = Color(color) << Language->Get("FUN_CODES");
	}
	
	void ExtraCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys1 = MenuEntrys[FishCodes];
		std::vector<MenuEntry *> Entrys2 = MenuEntrys[ChatCodes];
		std::vector<MenuEntry *> Entrys3 = MenuEntrys[ExtraCodes];

		const static std::string Names1[2][2] = {
			{ "FISH_ALWAYS_BITE_NAME", "FISH_CANT_SCARE_NAME" },

			{ "FISH_ALWAYS_BITE_NOTE", "FISH_CANT_SCARE_NOTE" },
		};

		const static std::string Names2[2][3] = {
			{ "CHAT_DONT_DISSAPEAR", "CHATCOPYPASTE", "FORCE_CHAT" },

			{ "CHAT_DONT_DISSAPEAR_NOTE", "CHATCOPYPASTE_NOTE", "FORCE_CHAT_NOTE" },
		};

		const static std::string Names3[2][18] = {
			{ "SHOP_ALWAYS_OPEN_NAME", "DISABLE_SAVE", "DISABLE_ITEM_LOCKS", 
			"CANT_FALL_HOLE", "SET_SPOT_STATE_NAME", "SEARCH_REPLACE_NAME", 
			"REMOVE_TOWNITEMS_NAME", "WATER_FLOWERS_NAME", "WEED_REMOVER_NAME", 
			"EDIT_PATTERN_NAME", "GRASS_EDITOR", "AMIIBO_SPOOFER", 
			"TIME_TRAVEL" },

			{ "SHOP_ALWAYS_OPEN_NOTE", "DISABLE_SAVE_NOTE", "DISABLE_ITEM_LOCKS_NOTE", 
			"CANT_FALL_HOLE_NOTE", "SET_SPOT_STATE_NOTE", "SEARCH_REPLACE_NOTE", 
			"REMOVE_TOWNITEMS_NOTE", "WATER_FLOWERS_NOTE", "WEED_REMOVER_NOTE", 
			"EDIT_PATTERN_NOTE", "GRASS_EDITOR_NOTE", "AMIIBO_SPOOFER_NOTE", 
			"TIME_TRAVEL_NOTE" },
		};

		const static HotkeyDat Hotkeys1[2] = {
			{ { "CHATCOPYPASTE_KEY1", "CHATCOPYPASTE_KEY2", "CHATCOPYPASTE_KEY3", "CHATCOPYPASTE_KEY4" }, 1 },
			{ { "FORCE_CHAT" }, 2 },
		};

		const static HotkeyDat Hotkeys2[4] = {
			{ { "WATER_FLOWRES_HOTKEY1" }, 7 },
			{ { "WEED_REMOVER_HOTKEY1", "WEED_REMOVER_HOTKEY2" }, 8 },
			{ { "GRASS_EDITOR_HOTKEY1", "GRASS_EDITOR_HOTKEY2", "GRASS_EDITOR_HOTKEY3" }, 10 },
			{ { "TIME_FORWARD", "TIME_BACKWARDS" }, 12 },
		};

		for(int i = 0; i < Entrys1.size(); ++i) 
			UpdateEntry(Entrys1[i], Color(color) << Language->Get(Names1[0][i]), Language->Get(Names1[1][i]));
		for(int i = 0; i < Entrys2.size(); ++i) 
			UpdateEntry(Entrys2[i], Color(color) << Language->Get(Names2[0][i]), Language->Get(Names2[1][i]));
		for(int i = 0; i < Entrys3.size(); ++i) 
			UpdateEntry(Entrys3[i], Color(color) << Language->Get(Names3[0][i]), Language->Get(Names3[1][i]));

		for(int i = 0; i < 2; ++i) 
			UpdateHotkey(ChatCodes, Hotkeys1[i]);
		for(int i = 0; i < 4; ++i) 
			UpdateHotkey(ExtraCodes, Hotkeys2[i]);

		FISC->Name() = Color(color) << Language->Get("FISH_CODES");
		CHAC->Name() = Color(color) << Language->Get("CHAT_CODES");
		EXTC->Name() = Color(color) << Language->Get("EXTRA_CODES");
	}
	
	void MiscCodesUpdate(Color color) {
		std::vector<MenuEntry *> Entrys = MenuEntrys[MiscCodes];

		const static std::string Names[2][19] = {
			{ "TOOL_ANIM", "GAME_TYPE", "UNBREAK_FLOWER", 
			"WEATHER_MOD", "ALWAYS_AURORA_MOD", "DISABLECOMMAND", 
			"RELOAD_ROOM_NAME", "QUICK_MENU", "MORE_NUMBERS", 
			"LARGE_FOV", "MOVE_FURN", "WALK_TALK", 
			"KEY_EXTEND", "CUSTOM_KEY", "BEANS_PARTICLE", 
			"DAYTIME", "FAST_MODE", "FAST_TEXT_SPEED", 
			"FAST_GAME_SPEED" },

			{ "TOOL_ANIM_NOTE", "GAME_TYPE_NOTE", "UNBREAK_FLOWER_NOTE", 
			"WEATHER_MOD_NOTE", "ALWAYS_AURORA_MOD_NOTE", "DISABLECOMMAND_NOTE", 
			"RELOAD_ROOM_NOTE", "QUICK_MENU_NOTE", "MORE_NUMBERS_NOTE", 
			"LARGE_FOV_NOTE", "MOVE_FURN_NOTE", "WALK_TALK_NOTE", 
			"KEY_EXTEND_NOTE", "CUSTOM_KEY_NOTE", "BEANS_PARTICLE_NOTE", 
			"DAYTIME_NOTE", "FAST_MODE_NOTE", "FAST_TEXT_SPEED_NOTE", 
			"FAST_GAME_SPEED_NOTE" },
		};

		const static HotkeyDat Hotkeys[3] = {	
			{ { "QUICK_MENU" }, 7 },
			{ { "BEANS_PARTICLE" }, 14 },
			{ { "FAST_MODE" }, 16 },
		};

		for(int i = 0; i < Entrys.size(); ++i) 
			UpdateEntry(Entrys[i], Color(color) << Language->Get(Names[0][i]), Language->Get(Names[1][i]));

		for(int i = 0; i < 3; ++i) 
			UpdateHotkey(MiscCodes, Hotkeys[i]);
	
		MISC->Name() = Color(color) << Language->Get("MISC_CODES");
	}
}