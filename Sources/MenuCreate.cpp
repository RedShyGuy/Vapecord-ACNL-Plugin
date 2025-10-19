#include "cheats.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "LibCtrpfExtras/MenuEntryExtras.hpp"
#include "LibCtrpfExtras/MenuFolderExtras.hpp"
#include "LibCtrpfExtras/HotkeyExtras.hpp"

namespace CTRPluginFramework {
    static MenuEntryExtras *EntryWithHotkey(MenuEntryExtras *entry, const std::vector<HotkeyExtras> &hotkeys) {
        if(entry != nullptr) {
            for(const HotkeyExtras &hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return entry;
    }	
	
	static MenuEntryExtras *EntryWithHotkey(MenuEntryExtras *entry, const HotkeyExtras &hotkey) {
        if(entry != nullptr) {
            entry->Hotkeys += hotkey;
        }
        return entry;
    }

/*This will load all the folders and entrys*/
    void InitMenu(PluginMenu *menu) {
	/////////////////////
	/*Save Codes Folder*/
	/////////////////////
		MenuFolderExtras *SAVEC = new MenuFolderExtras("SAVE_CODES", FolderType::Save);
	    SAVEC->Append(new MenuEntryExtras("TOWN_NAME_CHANGER", nullptr, townnamechanger, "TOWN_NAME_CHANGER_NOTE")),
		SAVEC->Append(new MenuEntryExtras("SAVE_BACKUP_NAME", nullptr, savebackup, "SAVE_BACKUP_NOTE")),	
		SAVEC->Append(new MenuEntryExtras("BULL_BOARD_DUMPER", nullptr, bullboard, "BULL_BOARD_DUMPER_NOTE")),
		SAVEC->Append(new MenuEntryExtras("TREESIZE_NAME", nullptr, TreeSizeChanger, "TREESIZE_NOTE")),	
		SAVEC->Append(new MenuEntryExtras("CHANGE_NATIVE_NAME", nullptr, ChangeNativeFruit, "CHANGE_NATIVE_NOTE")),
		SAVEC->Append(new MenuEntryExtras("PWP_UNLOCK_NAME", nullptr, PWPUnlock, "PWP_UNLOCK_NOTE")),
		SAVEC->Append(new MenuEntryExtras("GRASS_CHANGER_NAME", nullptr, GrassChanger, "GRASS_CHANGER_NOTE")),
		SAVEC->Append(new MenuEntryExtras("CARAVAN_SET", nullptr, caravanchange, "CARAVAN_SET_NOTE")),
		SAVEC->Append(new MenuEntryExtras("CAMPING_SET", nullptr, SetCampingVillager, "CAMPING_SET_NOTE")),
		SAVEC->Append(new MenuEntryExtras("SHOP_UPGRADE", nullptr, shopunlocks, "SHOP_UPGRADE_NOTE")),
	//SAVEC->Append(new MenuEntryExtras("HOUSE_EDITOR_NAME", nullptr, HouseChanger, "HOUSE_EDITOR_NOTE")),
		SAVEC->Append(new MenuEntryExtras("QR_MACHINE_NAME", nullptr, unlockqrmachine, "QR_MACHINE_NOTE")),
		SAVEC->Append(new MenuEntryExtras("BUILDING_MOD_NAME", nullptr, BuildingMod, "BUILDING_MOD_NOTE")),
		SAVEC->Append(new MenuEntryExtras("FILL_MUSEUM_NAME", nullptr, CompleteMuseum, "FILL_MUSEUM_NOTE")),
		SAVEC->Append(new MenuEntryExtras("MAYOR_PERMIT_NAME", Permit100, "MAYOR_PERMIT_NOTE")),
		SAVEC->Append(new MenuEntryExtras("REAL_TIME_ACRE", MapEditor, "REAL_TIME_ACRE_NOTE")),
		menu->Append(SAVEC);
		
	/////////////////////////
	/*Movement Codes Folder*/
	/////////////////////////
		MenuFolderExtras *MOVEC = new MenuFolderExtras("MOVEMENT_CODES", FolderType::Movement);
		MOVEC->Append(new MenuEntryExtras("CANT_PUSH", noPush, "CANT_PUSH_NOTE")),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("COORD_MOD", coordinate, coordspeed, "COORD_MOD_NOTE"), {
			HotkeyExtras(Key::A, "COORD_MOD_KEY1"), 
			HotkeyExtras(Key::DPadRight, "COORD_MOD_KEY2"), 
			HotkeyExtras(Key::DPadLeft, "COORD_MOD_KEY3"), 
			HotkeyExtras(Key::DPadDown, "COORD_MOD_KEY4"), 
			HotkeyExtras(Key::DPadUp, "COORD_MOD_KEY5") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("MOON_JUMP", moonjump, "MOON_JUMP_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadUp, "MOON_JUMP_KEY1"), 
			HotkeyExtras(Key::L | Key::DPadDown, "MOON_JUMP_KEY2") 
		})),
		MOVEC->Append(new MenuEntryExtras("TOUCH_WARP", tch_warp, "TOUCH_WARP_NOTE")),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("WALK_OVER", walkOver, "WALK_OVER_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadUp, "WALK_OVER") 
		})),		   
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("MOVEMENT_CHANGE", MovementChanger, "MOVEMENT_CHANGE_NOTE"), { 
			HotkeyExtras(Key::L | Key::B, "MOVEMENT_CHANGE") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("WALK_PARTICLE_CHANGE", Walkparticle, "WALK_PARTICLE_CHANGE_NOTE"), { 
			HotkeyExtras(Key::L | Key::X, "WALK_PARTICLE_CHANGE")
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("PLAYER_TELEPORT", stalk, "PLAYER_TELEPORT_NOTE"), { 
			HotkeyExtras(Key::R | Key::DPadLeft, "PLAYER_TELEPORT_KEY1"), 
			HotkeyExtras(Key::R | Key::DPadRight, "PLAYER_TELEPORT_KEY2") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("VISIBILITY_MOD", onlineplayermod, "VISIBILITY_MOD_NOTE"), { 
			HotkeyExtras(Key::L | Key::A, "VISIBILITY_MOD_KEY1") 
		})),
		MOVEC->Append(new MenuEntryExtras("SPEED_MOD", speedMod, menuSpeedMod, "SPEED_MOD_NOTE")),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras("ROOM_WARPING", roomWarp, "ROOM_WARPING_NOTE"), { 
			HotkeyExtras(Key::L | Key::X, "ROOM_WARPING") 
		})),
		MOVEC->Append(new MenuEntryExtras("SHOVEL_KNOCKBACK", shovelknockback, "SHOVEL_KNOCKBACK_NOTE")),
		menu->Append(MOVEC);

	//////////////////////////
	/*Inventory Codes Folder*/
	//////////////////////////
		MenuFolderExtras *INVC = new MenuFolderExtras("INVENTORY_CODES", FolderType::Inventory);
		INVC->Append(EntryWithHotkey(new MenuEntryExtras("TEXT_2_ITEM", t2i, "TEXT_2_ITEM_NOTE"), { 
			HotkeyExtras(Key::X | Key::DPadRight, "TEXT_2_ITEM_KEY1"), 
			HotkeyExtras(Key::X |Key::DPadUp, "TEXT_2_ITEM_KEY2"), 
			HotkeyExtras(Key::X | Key::DPadDown, "TEXT_2_ITEM_KEY3"),
			HotkeyExtras(Key::X | Key::DPadLeft, "TEXT_2_ITEM_KEY4"),
		})),
		INVC->Append(EntryWithHotkey(new MenuEntryExtras("DUPE_ITEMS", duplication, "DUPE_ITEMS_NOTE"), { 
			HotkeyExtras(Key::R, "DUPE_ITEMS_KEY1"), 
			HotkeyExtras(Key::R | Key::X, "DUPE_ITEMS_KEY2") 
		})),
		INVC->Append(EntryWithHotkey(new MenuEntryExtras("CATALOG_TO_POCKET", catalog, "CATALOG_TO_POCKET_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadRight, "CATALOG_TO_POCKET_KEY1"), 
			HotkeyExtras(Key::L | Key::Y, "CATALOG_TO_POCKET_KEY2") 
		})),
		INVC->Append(EntryWithHotkey(new MenuEntryExtras("CHAT_T2I", chatt2i, "CHAT_T2I_NOTE"), { 
			HotkeyExtras(Key::R | Key::DPadLeft, "CHAT_T2I_KEY1") 
		})),
		INVC->Append(new MenuEntryExtras("CLEAR_INV_NAME", nullptr, ClearInventory, "CLEAR_INV_NOTE")),
		INVC->Append(new MenuEntryExtras("ITEM_SETTINGS", nullptr, itemsettings, "ITEM_SETTINGS_NOTE")),
		INVC->Append(new MenuEntryExtras("SAVE_MENU_CHANGER", nullptr, MenuChanger, "SAVE_MENU_CHANGER_NOTE")),
		INVC->Append(new MenuEntryExtras("GET_SET", nullptr, getset, "GET_SET_NOTE")),
		INVC->Append(new MenuEntryExtras("CUSTOM_BUTTON", nullptr, SettingsButton, "CUSTOM_BUTTON_NOTE")),
		menu->Append(INVC);

	///////////////////////
	/*Player Codes Folder*/
	///////////////////////
		MenuFolderExtras *PLAYC = new MenuFolderExtras("PLAYER_CODES", FolderType::Player);
	////////////////////////////
	/*Player Save Codes Folder*/
	////////////////////////////
		MenuFolderExtras *PSAVEC = new MenuFolderExtras("PLAYER_SAVE_CODES", FolderType::Player);
		PSAVEC->Append(new MenuEntryExtras("NAME_CHANGER", nullptr, NameChanger, "NAME_CHANGER_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("PLAYER_APPEARANCE", nullptr, playermod, "PLAYER_APPEARANCE_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("RANDOM_PLAYER", nullptr, randomoutfit, "RANDOM_PLAYER_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("PLAYER_BACKUP_NAME", nullptr, playerbackup, "PLAYER_BACKUP_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("TPC_MESSAGE", nullptr, tpcmessage, "TPC_MESSAGE_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("TPC_SETTINGS", nullptr, tpc, "TPC_SETTINGS_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("DESIGN_DUMP", nullptr, DesignDumper, "DESIGN_DUMP_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("EMOTION_LIST", nullptr, emotelist, "EMOTION_LIST_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("ENCY_LIST", nullptr, enzyklopedia, "ENCY_LIST_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("DREAM_CODE", nullptr, comodifier, "DREAM_CODE_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("DEBUG_MENU", nullptr, debug1, "DEBUG_MENU_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("SONG_LIST_NAME", nullptr, FillSongs, "SONG_LIST_NOTE")),
		PSAVEC->Append(new MenuEntryExtras("FILL_CATALOG_NAME", nullptr, FillCatalog, "FILL_CATALOG_NOTE")),
		PLAYC->Append(PSAVEC);
		PLAYC->Append(new MenuEntryExtras("PLAYER_INFO", debug, "PLAYER_INFO_NOTE")),
	//PLAYC->Append(new MenuEntryExtras("PLAYER_LOADER", nullptr, pLoaderEntry, "PLAYER_LOADER_NOTE")),
		PLAYC->Append(EntryWithHotkey(new MenuEntryExtras("NECK_POSITION", neckentry, "NECK_POSITION_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadDown, "NECK_POSITION") 
		})),
		PLAYC->Append(EntryWithHotkey(new MenuEntryExtras("CUSTOM_HAIR_EYE", App_ColorMod, SaveColor, "CUSTOM_HAIR_EYE_NOTE"), { 
			HotkeyExtras(Key::R, "CUSTOM_HAIR_EYE_KEY1")
		})),
		PLAYC->Append(new MenuEntryExtras("WEAR_HELMET", hatz, "WEAR_HELMET_NOTE")),
		PLAYC->Append(EntryWithHotkey(new MenuEntryExtras("FAINT", Faint, "FAINT_NOTE"), {
			HotkeyExtras(Key::R | Key::A, "FAINT")
		})),
		PLAYC->Append(new MenuEntryExtras("PLAYERS_ON_MAP", map, "PLAYERS_ON_MAP_NOTE")),

		PLAYC->Append(new MenuEntryExtras("NEVER_BEDHEAD", NeverBedHead, "NEVER_BEDHEAD_NOTE")),
		menu->Append(PLAYC);

	//////////////////////////
	/*Animation Codes Folder*/
	//////////////////////////
		MenuFolderExtras *ANIMC = new MenuFolderExtras("ANIMATION_CODES", FolderType::Animation);
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras("PLAYER_SELECTOR", playerSelector, "PLAYER_SELECTOR_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadRight, "PLAYER_SELECTOR_KEY1"), 
			HotkeyExtras(Key::L | Key::DPadLeft, "PLAYER_SELECTOR_KEY2") 
		})),
		ANIMC->Append(new MenuEntryExtras("ANTI_ANIM", anticheat, "ANTI_ANIM_NOTE")),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras("ANIMATION_MOD", execAnim, "ANIMATION_MOD_NOTE"), { 
			HotkeyExtras(Key::A | Key::DPadRight, "ANIMATIONS_NOTE_HOTKEY1"), 
			HotkeyExtras(Key::A | Key::DPadLeft, "ANIMATIONS_NOTE_HOTKEY2"), 
			HotkeyExtras(Key::A | Key::DPadUp, "ANIMATIONS_NOTE_HOTKEY3"), 
			HotkeyExtras(Key::A | Key::B, "ANIMATIONS_NOTE_HOTKEY4") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras("EMOTION_LOOP", inf_expression, "EMOTION_LOOP_NOTE"), { 
			HotkeyExtras(Key::B, "EMOTION_LOOP") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras("IDLE", idle, "IDLE_NOTE"), { 
			HotkeyExtras(Key::R, "IDLE") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras("SLOW_MO_ANIM", slmoanms, "SLOW_MO_ANIM_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadLeft, "SLOW_MO_ANIM") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras("ANIM_ON_ALL", doonall, "ANIM_ON_ALL_NOTE"), { 
			HotkeyExtras(Key::R | Key::A, "ANIM_ON_ALL_KEY1"), 
			HotkeyExtras(Key::R | Key::B, "ANIM_ON_ALL_KEY2"), 
			HotkeyExtras(Key::R | Key::Y, "ANIM_ON_ALL_KEY3") 
		})),
		menu->Append(ANIMC);

	////////////////////////
	/*Seeding Codes Folder*/
	////////////////////////
		MenuFolderExtras *SEEDC = new MenuFolderExtras("SEEDING_CODES", FolderType::Seeding);
	////////////////////////
	/*Seed Codes SubFolder*/
	////////////////////////
		MenuFolderExtras *SEED1C = new MenuFolderExtras("SEED_CODES", FolderType::Seeding);
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras("PICK_SEEDER", pickseeder, "PICK_SEEDER_NOTE"), { 
			HotkeyExtras(Key::B | Key::DPadLeft, "PICK_SEEDER_KEY1"), 
			HotkeyExtras(Key::B | Key::DPadDown, "PICK_SEEDER_KEY2"), 
			HotkeyExtras(Key::B | Key::DPadUp, "PICK_SEEDER_KEY3"), 
			HotkeyExtras(Key::B | Key::DPadRight, "PICK_SEEDER_KEY4"), 
			HotkeyExtras(Key::B | Key::L, "PICK_SEEDER_KEY5") 
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras("WALK_SEEDER", Walkseeder, "WALK_SEEDER_NOTE"), { 
			HotkeyExtras(Key::R | Key::B, "WALK_SEEDER") 
		})),
		SEED1C->Append(new MenuEntryExtras("FIREWORK_SEEDER", nullptr, fireworkentry, "FIREWORK_SEEDER_NOTE")),
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras("MAP_EDITOR", tileSelector, "MAP_EDITOR_NOTE"), { 
			HotkeyExtras(Key::Start | Key::DPadUp, "MAP_EDITOR_KEY1"), 
			HotkeyExtras(Key::DPadRight, "MAP_EDITOR_KEY2"), 
			HotkeyExtras(Key::DPadLeft, "MAP_EDITOR_KEY3"), 
			HotkeyExtras(Key::DPadDown, "MAP_EDITOR_KEY4"), 
			HotkeyExtras(Key::DPadUp, "MAP_EDITOR_KEY5"), 
			HotkeyExtras(Key::L, "MAP_EDITOR_KEY6"), 
			HotkeyExtras(Key::R, "MAP_EDITOR_KEY7"), 
			HotkeyExtras(Key::Start | Key::DPadDown, "MAP_EDITOR_KEY8"), 
			HotkeyExtras(Key::Start | Key::DPadLeft, "MAP_EDITOR_KEY9"), 
			HotkeyExtras(Key::A, "MAP_EDITOR_KEY10"), 
			HotkeyExtras(Key::Start | Key::DPadRight, "MAP_EDITOR_KEY11") 
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras("ALL_SEEDER", everythingseeder, E_Seeder_KB, "ALL_SEEDER_NOTE"), { 
			HotkeyExtras(Key::L, "ALL_SEEDER_KEY1") 
		})),
		SEEDC->Append(SEED1C);
	////////////////////////
	/*Drop Codes SubFolder*/
	////////////////////////
		MenuFolderExtras *DROPC = new MenuFolderExtras("DROP_CODES", FolderType::Seeding);
		DROPC->Append(new MenuEntryExtras("ITEM_SEQUENCER", nullptr, Entry_itemsequence, "ITEM_SEQUENCER_NOTE")),
	    DROPC->Append(EntryWithHotkey(new MenuEntryExtras("DROP_MODS", dropMod, "DROP_MODS_NOTE"), { 
			HotkeyExtras(Key::A | Key::DPadRight, "DROP_MODS_KEY1"), 
			HotkeyExtras(Key::A | Key::DPadLeft, "DROP_MODS_KEY2"), 
			HotkeyExtras(Key::A | Key::DPadDown, "DROP_MODS_KEY3"), 
			HotkeyExtras(Key::A | Key::DPadUp, "DROP_MODS_KEY4") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras("DROP_ITEMS", instantDrop, "DROP_ITEMS_NOTE"), { 
			HotkeyExtras(Key::Y | Key::DPadRight, "DROP_ITEMS_KEY1"), 
			HotkeyExtras(Key::L | Key::DPadDown, "DROP_ITEMS_KEY2") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras("AUTO_DROP", autoDrop, "AUTO_DROP_NOTE"), { 
			HotkeyExtras(Key::Y | Key::DPadRight, "AUTO_DROP_KEY1"), 
			HotkeyExtras(Key::Y | Key::DPadLeft, "AUTO_DROP_KEY2") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras("TOUCH_DROP", touchDrop, "TOUCH_DROP_NOTE"), { 
			HotkeyExtras(Key::Y | Key::DPadRight, "TOUCH_DROP_KEY1") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras("SLOT_DROP", ShowInvSlotID, "SLOT_DROP_NOTE"), { 
			HotkeyExtras(Key::R | Key::Y, "SLOT_DROP_KEY1"), 
			HotkeyExtras(Key::R | Key::X, "SLOT_DROP_KEY2") 
		})),
		SEEDC->Append(DROPC);
	////////////////////////
	/*Tree Codes SubFolder*/
	////////////////////////
		MenuFolderExtras *TREEC = new MenuFolderExtras("TREE_CODES", FolderType::Seeding);
		TREEC->Append(new MenuEntryExtras("INF_FRUIT_TREE", fruitStays, "INF_FRUIT_TREE_NOTE")),
		TREEC->Append(new MenuEntryExtras("AXE_TREE_SHAKE", shakechop, "AXE_TREE_SHAKE_NOTE")),
		TREEC->Append(EntryWithHotkey(new MenuEntryExtras("FRUIT_TREE_MOD", fruititemmod, "FRUIT_TREE_MOD_NOTE"), { 
			HotkeyExtras(Key::A | Key::DPadLeft, "FRUIT_TREE_MOD_KEY1") 
		})),
		TREEC->Append(new MenuEntryExtras("INST_TREE_CHOP", instantchop, "INST_TREE_CHOP_NOTE")),
		SEEDC->Append(TREEC),
		menu->Append(SEEDC);

	//////////////////////
	/*Money Codes Folder*/
	//////////////////////
		MenuFolderExtras *MONC = new MenuFolderExtras("MONEY_CODES", FolderType::Money);
		MONC->Append(new MenuEntryExtras("WALLET", nullptr, wallet, "WALLET_NOTE")),
		MONC->Append(new MenuEntryExtras("BANK", nullptr, bank, "BANK_NOTE")),
		MONC->Append(new MenuEntryExtras("MEOW_COUPONS", nullptr, coupon, "MEOW_COUPONS_NOTE")),
		MONC->Append(new MenuEntryExtras("BADGES", nullptr, badges, "BADGES_NOTE")),
		MONC->Append(new MenuEntryExtras("MEDALS", nullptr, medals, "MEDALS_NOTE")),
		MONC->Append(new MenuEntryExtras("TURNIPS", nullptr, turnips, "TURNIPS_NOTE")),
		menu->Append(MONC);

	///////////////////////
	/*Island Codes Folder*/
	///////////////////////
		MenuFolderExtras *ISLC = new MenuFolderExtras("ISLAND_CODES", FolderType::Island);
		ISLC->Append(new MenuEntryExtras("UNLOCK_ISLAND", nullptr, UnlockIsland, "UNLOCK_ISLAND_NOTE")),
		ISLC->Append(new MenuEntryExtras("FILL_INV_ORE", bonusOre, "FILL_INV_ORE_NOTE")),
		ISLC->Append(new MenuEntryExtras("FILL_INV_FRUIT", instantFruit, "FILL_INV_FRUIT_NOTE")),
		ISLC->Append(new MenuEntryExtras("HACKER_ISLAND", nullptr, Hackerisland, "HACKER_ISLAND_NOTE")),
		ISLC->Append(new MenuEntryExtras("ISLAND_COUNTRY", nullptr, Countryspoof, "ISLAND_COUNTRY_NOTE")),
		ISLC->Append(new MenuEntryExtras("FREE_KAPPN", FreeKappn, "FREE_KAPPN_NOTE")),
		ISLC->Append(new MenuEntryExtras("ISLAND_SHOP_MOD", IslandShop, IslandSettings, "ISLAND_SHOP_MOD_NOTE")),
		ISLC->Append(new MenuEntryExtras("ALL_TOURS", alltour, "ALL_TOURS_NOTE")),
		ISLC->Append(new MenuEntryExtras("ISLAND_ACRE", acreMod, menuAcreMod, "ISLAND_ACRE_NOTE")),
		ISLC->Append(new MenuEntryExtras("ISLAND_BUILDING", buildingMod, menuBuildingMod, "ISLAND_BUILDING_NOTE")),
		ISLC->Append(EntryWithHotkey(new MenuEntryExtras("ISLAND_SAVER", IslandSaver, "ISLAND_SAVER_NOTE"), { 
			HotkeyExtras(Key::L | Key::Y, "Open Menu")
		})),
		menu->Append(ISLC);

	//////////////
	/*NPC Folder*/
	//////////////
		MenuFolderExtras *NPCC = new MenuFolderExtras("NPC_CODES", FolderType::NPC);
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras("NPC_SELECTOR", NPCFunction, "NPC_SELECTOR_NOTE"), { 
			HotkeyExtras(Key::L | Key::A, "NPC_SELECTOR_KEY1") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras("NPC_ANIMATION",  NPCAnimation, NPCSetAnim, "NPC_ANIMATION_NOTE"), { 
			HotkeyExtras(Key::L | Key::B, "NPC_ANIMATION_KEY1") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras("NPC_COORDINATE", NPCCoordinates, "NPC_COORDINATE_NOTE"), { 
			HotkeyExtras(Key::L, "NPC_COORDINATE_KEY1"),
			HotkeyExtras(Key::DPadRight, "NPC_COORDINATE_KEY2"), 
			HotkeyExtras(Key::DPadLeft, "NPC_COORDINATE_KEY3"), 
			HotkeyExtras(Key::DPadDown, "NPC_COORDINATE_KEY4"), 
			HotkeyExtras(Key::DPadUp, "NPC_COORDINATE_KEY5")
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras("NPC_TELEPORT", NPCTeleportToYou, "NPC_TELEPORT_NOTE"), { 
			HotkeyExtras(Key::L | Key::Y, "NPC_TELEPORT_KEY1") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras("NPC_ROTATION", NPCRotate, "NPC_ROTATION_NOTE"), { 
			HotkeyExtras(Key::L, "NPC_ROTATION_KEY1") 
		})),
		menu->Append(NPCC);

	////////////////////
	/*Fun Codes Folder*/
	////////////////////
		MenuFolderExtras *FUNC = new MenuFolderExtras("FUN_CODES", FolderType::Fun); 
		FUNC->Append(new MenuEntryExtras("SIZE_CODES", nullptr, sizecodes, "SIZE_CODES_NOTE")),
		FUNC->Append(new MenuEntryExtras("T_POSE", tposeentry, "T_POSE_NOTE")),
		FUNC->Append(EntryWithHotkey(new MenuEntryExtras("TAKE_TPC_PIC", freezeframe, "TAKE_TPC_PIC_NOTE"), { 
			HotkeyExtras(Key::L, "TAKE_TPC_PIC_KEY1"), 
			HotkeyExtras(Key::A, "TAKE_TPC_PIC_KEY2") 
		})),
		FUNC->Append(new MenuEntryExtras("MAX_TURBO", maxturbo, "MAX_TURBO_NOTE")),
		FUNC->Append(new MenuEntryExtras("MULTI_PRESS", asmpresses, "MULTI_PRESS_NOTE")),
		FUNC->Append(EntryWithHotkey(new MenuEntryExtras("ULTIMATE_POPPER", partypopper, "ULTIMATE_POPPER_NOTE"), { 
			HotkeyExtras(Key::B | Key::DPadLeft, "ULTIMATE_POPPER_KEY1") 
		})),
	    FUNC->Append(new MenuEntryExtras("CAMERA_MOD", cameramod, "CAMERA_MOD_NOTE")),
		menu->Append(FUNC);

	//////////////////////
	/*Extra Codes Folder*/
	//////////////////////
		MenuFolderExtras *EXTC = new MenuFolderExtras("EXTRA_CODES", FolderType::Extra);
	/////////////////////
	/*Fish Codes Folder*/
	/////////////////////
		MenuFolderExtras *FISC = new MenuFolderExtras("FISH_CODES", FolderType::Extra);
		FISC->Append(new MenuEntryExtras("FISH_ALWAYS_BITE_NAME", FishAlwaysBiteRightAway, "FISH_ALWAYS_BITE_NOTE")),
		FISC->Append(new MenuEntryExtras("FISH_CANT_SCARE_NAME", FishCantBeScared, "FISH_CANT_SCARE_NOTE")),
		EXTC->Append(FISC);
	/////////////////////
	/*Chat Codes Folder*/
	/////////////////////
		MenuFolderExtras *CHAC = new MenuFolderExtras("CHAT_CODES", FolderType::Extra);
		CHAC->Append(new MenuEntryExtras("CHAT_DONT_DISSAPEAR", bubblesDisappear, "CHAT_DONT_DISSAPEAR_NOTE")),
		CHAC->Append(EntryWithHotkey(new MenuEntryExtras("CHATCOPYPASTE", ChatCopyPaste, "CHATCOPYPASTE_NOTE"), {
			HotkeyExtras(Key::L | Key::DPadRight, "CHATCOPYPASTE_KEY1"),
			HotkeyExtras(Key::L | Key::DPadUp, "CHATCOPYPASTE_KEY2"),
			HotkeyExtras(Key::L | Key::DPadDown, "CHATCOPYPASTE_KEY3"),
			HotkeyExtras(Key::L | Key::DPadLeft, "CHATCOPYPASTE_KEY4")
		})),
		CHAC->Append(EntryWithHotkey(new MenuEntryExtras("FORCE_CHAT", Forcesendchat, "FORCE_CHAT_NOTE"), { 
			HotkeyExtras(Key::R, "FORCE_CHAT") 
		})),
		CHAC->Append(new MenuEntryExtras("ShowChatMessage", ShowChatMessage, "ShowChatMessage_NOTE")),
		CHAC->Append(new MenuEntryExtras("ChatButton", ChatButton, "ChatButton_NOTE")),
		EXTC->Append(CHAC);
		EXTC->Append(new MenuEntryExtras("SHOP_ALWAYS_OPEN_NAME", ShopsAlwaysOpen, "SHOP_ALWAYS_OPEN_NOTE")),
		EXTC->Append(new MenuEntryExtras("DISABLE_SAVE", nonesave, "DISABLE_SAVE_NOTE")),
		EXTC->Append(new MenuEntryExtras("DISABLE_ITEM_LOCKS", bypass, "DISABLE_ITEM_LOCKS_NOTE")),
		EXTC->Append(new MenuEntryExtras("CANT_FALL_HOLE", noTrap, "CANT_FALL_HOLE_NOTE")),
		EXTC->Append(new MenuEntryExtras("SET_SPOT_STATE_NAME", nullptr, SetSpotState, "SET_SPOT_STATE_NOTE")),
		EXTC->Append(new MenuEntryExtras("SEARCH_REPLACE_NAME", nullptr, SearchReplace, "SEARCH_REPLACE_NOTE")),
		EXTC->Append(new MenuEntryExtras("REMOVE_MAP_ITEMS_NAME", nullptr, RemoveItemsCheat, "REMOVE_MAP_ITEMS_NOTE")),
	    EXTC->Append(EntryWithHotkey(new MenuEntryExtras("WATER_FLOWERS_NAME", WaterAllFlowers, "WATER_FLOWERS_NOTE"), { 
			HotkeyExtras(Key::R | Key::DPadLeft, "WATER_FLOWRES_HOTKEY1") 
		})),
		EXTC->Append(EntryWithHotkey(new MenuEntryExtras("WEED_REMOVER_NAME", weedremover, "WEED_REMOVER_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadRight, "WEED_REMOVER_HOTKEY1"), 
			HotkeyExtras(Key::L | Key::DPadLeft, "WEED_REMOVER_HOTKEY2") 
		})),
		EXTC->Append(new MenuEntryExtras("EDIT_PATTERN_NAME", editpattern, "EDIT_PATTERN_NOTE")),
		EXTC->Append(EntryWithHotkey(new MenuEntryExtras("GRASS_EDITOR", grasseditor, grasscomplete, "GRASS_EDITOR_NOTE"), { 
			HotkeyExtras(Key::R | Key::DPadDown, "GRASS_EDITOR_HOTKEY1"), 
			HotkeyExtras(Key::R | Key::DPadUp, "GRASS_EDITOR_HOTKEY2"), 
			HotkeyExtras(Key::R | Key::DPadRight, "GRASS_EDITOR_HOTKEY3") 
		})),
		EXTC->Append(new MenuEntryExtras("AMIIBO_SPOOFER", AmiiboSpoofer, "AMIIBO_SPOOFER_NOTE")),
		EXTC->Append(EntryWithHotkey(new MenuEntryExtras("TIME_TRAVEL", TimeTravel, TTKeyboard, "TIME_TRAVEL_NOTE"), { 
			HotkeyExtras(Key::R | Key::DPadRight, "TIME_FORWARD"), 
			HotkeyExtras(Key::R | Key::DPadLeft, "TIME_BACKWARDS") 
		})),
		EXTC->Append(new MenuEntryExtras("BURIED_INSPECTOR", BuriedInspector, "BURIED_INSPECTOR_NOTE")),
		menu->Append(EXTC);

	/////////////////////
	/*Misc Codes Folder*/
	/////////////////////
		MenuFolderExtras *MISC = new MenuFolderExtras("MISC_CODES", FolderType::Misc);		
		MISC->Append(new MenuEntryExtras("TOOL_ANIM", nullptr, tooltype, "TOOL_ANIM_NOTE")),
		MISC->Append(new MenuEntryExtras("GAME_TYPE", nullptr, mgtype, "GAME_TYPE_NOTE")),
		MISC->Append(new MenuEntryExtras("UNBREAK_FLOWER", unbreakableflower, "UNBREAK_FLOWER_NOTE")),
		MISC->Append(new MenuEntryExtras("WEATHER_MOD", nullptr, Weathermod , "WEATHER_MOD_NOTE")),
		MISC->Append(new MenuEntryExtras("ALWAYS_AURORA_MOD", auroralights , "ALWAYS_AURORA_MOD_NOTE")),
		MISC->Append(new MenuEntryExtras("DISABLECOMMAND", nullptr, disablecommands, "DISABLECOMMAND_NOTE")),
		MISC->Append(new MenuEntryExtras("RELOAD_ROOM_NAME", nullptr, ReloadRoomCheat, "RELOAD_ROOM_NOTE")),
		MISC->Append(new MenuEntryExtras("MORE_NUMBERS",  morenumberisland, "MORE_NUMBERS_NOTE")),
		MISC->Append(new MenuEntryExtras("LARGE_FOV", fovlarge, "LARGE_FOV_NOTE")),
		MISC->Append(new MenuEntryExtras("MOVE_FURN", roomSeeder, "MOVE_FURN_NOTE")),
		MISC->Append(new MenuEntryExtras("WALK_TALK", walktalkentry, "WALK_TALK_NOTE")),
		MISC->Append(new MenuEntryExtras("KEY_EXTEND", key_limit, "KEY_EXTEND_NOTE")),
		MISC->Append(new MenuEntryExtras("CUSTOM_KEY", CustomKeyboard, "CUSTOM_KEY_NOTE")),
		MISC->Append(EntryWithHotkey(new MenuEntryExtras("BEANS_PARTICLE", BeansParticleChanger, "BEANS_PARTICLE_NOTE"), { 
			HotkeyExtras(Key::L | Key::DPadLeft, "BEANS_PARTICLE") 
		})),
		MISC->Append(new MenuEntryExtras("DAYTIME", Daytime, "DAYTIME_NOTE")),    
		MISC->Append(EntryWithHotkey(new MenuEntryExtras("FAST_MODE", fast, "FAST_MODE_NOTE"), { 
			HotkeyExtras(Key::R | Key::DPadDown, "FAST_MODE")
		})),
		MISC->Append(new MenuEntryExtras("FAST_TEXT_SPEED", fasttalk, "FAST_TEXT_SPEED_NOTE")),
		MISC->Append(new MenuEntryExtras("FAST_GAME_SPEED", speedentry, "FAST_GAME_SPEED_NOTE")),
		MISC->Append(new MenuEntryExtras("FAST_ISABELLE", fastisabelle, "FAST_ISABELLE_NOTE")),
		menu->Append(MISC);

	////////////////////
	/*Dev Codes Folder*/
	////////////////////
		DEVC = new MenuFolder(Color(0xFF1A69FF) << "Dev Codes");
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Custom Dump", nullptr, customdump, "Lets you dump/restore custom dumps.")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Call Function", FunctionsCaller, FunctionsCallerSettings, "Lets you call functions")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "MSG Box", msgboxtest, "")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Unused Fall Down", falldownfishing, "")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Analyze Fossils", Analyzer, "")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Test Cheat", islanditems, "")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "ACNH Cheat", acnh, "")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Friend Code Test", PlayerLoader, "")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Player Dumper", nullptr, player_dumper, "")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Test", valuedisplayer, "")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Extra Save Restore", nullptr, RestoreAll, "")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Fish Thrower", FishThrower, "")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Wand Ability", wandability, "Gives the different wands abilitys.\n" +
																			  ColorExtras::Format("%cBlue Wand: %cNULL\n", Color(0x0089FFFF), Color::White) +
																			  ColorExtras::Format("%cGreen Wand: %cRestores wilted flowers instantly\n", Color(0x2BFF00FF), Color::White) +
																			  ColorExtras::Format("%cPink Wand: %cNULL\n", Color(0xFF00E6FF), Color::White) +
																			  ColorExtras::Format("%cYellow Wand: %cNULL\n", Color(0xEFFF00FF), Color::White) +
																			  ColorExtras::Format("%cFlower Fairy Wand: %cGrows Tree instantly\n", Color(0xCD00FFFF), Color::White) +
																			  ColorExtras::Format("%cKiki and Lala Wand: %cNULL", Color(0xFF7700FF), Color::White))),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Light Switch Toggler", lightswitch, "Lets you toggle the light switch.\nHotkeys are:\nL + DPadUp : Toggle light in current room\nL + DPadRight : Set Room ID\nL + DPadLeft : Toggle light in set room")),
		DEVC->Append(new MenuEntry("EXPRESSION_MOD", FacialExpressionMod, SetFacialExpression, "EXPRESSION_MOD_NOTE")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Custom Save Settings", CustomSaveScreen, "")),
		menu->Append(DEVC);
	}
}
