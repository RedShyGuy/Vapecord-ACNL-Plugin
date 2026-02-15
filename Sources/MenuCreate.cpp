#include "cheats.hpp"
#include "LibCtrpfExtras/MenuEntryExtras.hpp"
#include "LibCtrpfExtras/MenuFolderExtras.hpp"
#include "LibCtrpfExtras/HotkeyExtras.hpp"

namespace CTRPluginFramework {
    static MenuEntryExtras *EntryWithHotkey(MenuEntryExtras *entry, const std::vector<HotkeyExtras> &hotkeys) {
        if(entry != nullptr) {
            for(const HotkeyExtras &hotkey : hotkeys) {
				entry->Hotkeys += hotkey;
				entry->AppendHotkeyNameKey(hotkey.GetNameKey());
			}
        }
        return entry;
    }

	static MenuEntryExtras *EntryWithHotkey(MenuEntryExtras *entry, const HotkeyExtras &hotkey) {
        if(entry != nullptr) {
            entry->Hotkeys += hotkey;
			entry->AppendHotkeyNameKey(hotkey.GetNameKey());
        }
        return entry;
    }

/*This will load all the folders and entrys*/
    void InitMenu(PluginMenu *menu) {
	/////////////////////
	/*Save Codes Folder*/
	/////////////////////
		MenuFolderExtras *SAVEC = new MenuFolderExtras(FolderType::Save);
	    SAVEC->Append(new MenuEntryExtras(TextID::TOWN_NAME_CHANGER, nullptr, townnamechanger, TextID::TOWN_NAME_CHANGER_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::SAVE_BACKUP_NAME, nullptr, savebackup, TextID::SAVE_BACKUP_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::BULL_BOARD_DUMPER, nullptr, bullboard, TextID::BULL_BOARD_DUMPER_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::TREESIZE_NAME, nullptr, TreeSizeChanger, TextID::TREESIZE_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::CHANGE_NATIVE_NAME, nullptr, ChangeNativeFruit, TextID::CHANGE_NATIVE_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::PWP_UNLOCK_NAME, nullptr, PWPUnlock, TextID::PWP_UNLOCK_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::GRASS_CHANGER_NAME, nullptr, GrassChanger, TextID::GRASS_CHANGER_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::CARAVAN_SET, nullptr, caravanchange, TextID::CARAVAN_SET_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::CAMPING_SET, nullptr, SetCampingVillager, TextID::CAMPING_SET_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::SHOP_UPGRADE, nullptr, shopunlocks, TextID::SHOP_UPGRADE_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::HOUSE_EDITOR_NAME, nullptr, HouseChanger, TextID::HOUSE_EDITOR_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::BUILDING_MOD_NAME, nullptr, BuildingMod, TextID::BUILDING_MOD_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::FILL_MUSEUM_NAME, nullptr, CompleteMuseum, TextID::FILL_MUSEUM_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::MAYOR_PERMIT_NAME, Permit100, TextID::MAYOR_PERMIT_NOTE)),
		SAVEC->Append(new MenuEntryExtras(TextID::REAL_TIME_ACRE, MapEditor, TextID::REAL_TIME_ACRE_NOTE)),
		menu->Append(SAVEC);

	/////////////////////////
	/*Movement Codes Folder*/
	/////////////////////////
		MenuFolderExtras *MOVEC = new MenuFolderExtras(FolderType::Movement);
		MOVEC->Append(new MenuEntryExtras(TextID::CANT_PUSH, noPush, TextID::CANT_PUSH_NOTE)),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::COORD_MOD, coordinate, coordspeed, TextID::COORD_MOD_NOTE), {
			HotkeyExtras(Key::A, TextID::COORD_MOD_KEY1),
			HotkeyExtras(Key::DPadRight, TextID::COORD_MOD_KEY2),
			HotkeyExtras(Key::DPadLeft, TextID::COORD_MOD_KEY3),
			HotkeyExtras(Key::DPadDown, TextID::COORD_MOD_KEY4),
			HotkeyExtras(Key::DPadUp, TextID::COORD_MOD_KEY5)
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::MOON_JUMP, moonjump, TextID::MOON_JUMP_NOTE), {
			HotkeyExtras(Key::L | Key::DPadUp, TextID::MOON_JUMP_KEY1),
			HotkeyExtras(Key::L | Key::DPadDown, TextID::MOON_JUMP_KEY2)
		})),
		MOVEC->Append(new MenuEntryExtras(TextID::TOUCH_WARP, tch_warp, TextID::TOUCH_WARP_NOTE)),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::WALK_OVER, walkOver, TextID::WALK_OVER_NOTE), {
			HotkeyExtras(Key::L | Key::DPadUp, TextID::WALK_OVER)
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::MOVEMENT_CHANGE, MovementChanger, TextID::MOVEMENT_CHANGE_NOTE), {
			HotkeyExtras(Key::L | Key::B, TextID::MOVEMENT_CHANGE)
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::WALK_PARTICLE_CHANGE, Walkparticle, TextID::WALK_PARTICLE_CHANGE_NOTE), {
			HotkeyExtras(Key::L | Key::X, TextID::WALK_PARTICLE_CHANGE)
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::PLAYER_TELEPORT, stalk, TextID::PLAYER_TELEPORT_NOTE), {
			HotkeyExtras(Key::R | Key::DPadLeft, TextID::PLAYER_TELEPORT_KEY1),
			HotkeyExtras(Key::R | Key::DPadRight, TextID::PLAYER_TELEPORT_KEY2)
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::VISIBILITY_MOD, onlineplayermod, TextID::VISIBILITY_MOD_NOTE), {
			HotkeyExtras(Key::L | Key::A, TextID::VISIBILITY_MOD_KEY1)
		})),
		MOVEC->Append(new MenuEntryExtras(TextID::SPEED_MOD, speedMod, menuSpeedMod, TextID::SPEED_MOD_NOTE)),
		MOVEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::ROOM_WARPING, roomWarp, TextID::ROOM_WARPING_NOTE), {
			HotkeyExtras(Key::L | Key::X, TextID::ROOM_WARPING)
		})),
		MOVEC->Append(new MenuEntryExtras(TextID::SHOVEL_KNOCKBACK, shovelknockback, TextID::SHOVEL_KNOCKBACK_NOTE)),
		menu->Append(MOVEC);

	//////////////////////////
	/*Inventory Codes Folder*/
	//////////////////////////
		MenuFolderExtras *INVC = new MenuFolderExtras(FolderType::Inventory);
		INVC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::TEXT_2_ITEM, t2i, itemsearch, TextID::TEXT_2_ITEM_NOTE), {
			HotkeyExtras(Key::X | Key::DPadRight, TextID::TEXT_2_ITEM_KEY1),
			HotkeyExtras(Key::X |Key::DPadUp, TextID::TEXT_2_ITEM_KEY2),
			HotkeyExtras(Key::X | Key::DPadDown, TextID::TEXT_2_ITEM_KEY3),
		})),
		INVC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::DUPE_ITEMS, duplication, TextID::DUPE_ITEMS_NOTE), {
			HotkeyExtras(Key::R, TextID::DUPE_ITEMS_KEY1),
			HotkeyExtras(Key::R | Key::X, TextID::DUPE_ITEMS_KEY2)
		})),
		INVC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::CATALOG_TO_POCKET, catalog, TextID::CATALOG_TO_POCKET_NOTE), {
			HotkeyExtras(Key::L | Key::DPadRight, TextID::CATALOG_TO_POCKET_KEY1)
		})),
		INVC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::CHAT_T2I, chatt2i, TextID::CHAT_T2I_NOTE), {
			HotkeyExtras(Key::R | Key::DPadLeft, TextID::CHAT_T2I_KEY1)
		})),
		INVC->Append(new MenuEntryExtras(TextID::CLEAR_INV_NAME, nullptr, ClearInventory, TextID::CLEAR_INV_NOTE)),
		INVC->Append(new MenuEntryExtras(TextID::ITEM_SETTINGS, nullptr, itemsettings, TextID::ITEM_SETTINGS_NOTE)),
		INVC->Append(new MenuEntryExtras(TextID::SAVE_MENU_CHANGER, nullptr, MenuChanger, TextID::SAVE_MENU_CHANGER_NOTE)),
		INVC->Append(new MenuEntryExtras(TextID::GET_SET, nullptr, getset, TextID::GET_SET_NOTE)),
		INVC->Append(new MenuEntryExtras(TextID::CUSTOM_BUTTON, nullptr, SettingsButton, TextID::CUSTOM_BUTTON_NOTE)),
		menu->Append(INVC);

	///////////////////////
	/*Player Codes Folder*/
	///////////////////////
		MenuFolderExtras *PLAYC = new MenuFolderExtras(FolderType::Player);
	////////////////////////////
	/*Player Save Codes Folder*/
	////////////////////////////
		MenuFolderExtras *PSAVEC = new MenuFolderExtras(FolderType::Player, SubFolder::PlayerSave);
		PSAVEC->Append(new MenuEntryExtras(TextID::NAME_CHANGER, nullptr, NameChanger, TextID::NAME_CHANGER_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::PLAYER_APPEARANCE, nullptr, playermod, TextID::PLAYER_APPEARANCE_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::RANDOM_PLAYER, nullptr, randomoutfit, TextID::RANDOM_PLAYER_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::PLAYER_BACKUP_NAME, nullptr, playerbackup, TextID::PLAYER_BACKUP_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::TPC_MESSAGE, nullptr, tpcmessage, TextID::TPC_MESSAGE_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::TPC_SETTINGS, nullptr, tpc, TextID::TPC_SETTINGS_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::DESIGN_DUMP, nullptr, DesignDumper, TextID::DESIGN_DUMP_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::EMOTION_LIST, nullptr, emotelist, TextID::EMOTION_LIST_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::ENCY_LIST, nullptr, enzyklopedia, TextID::ENCY_LIST_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::DREAM_CODE, nullptr, comodifier, TextID::DREAM_CODE_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::DEBUG_MENU, nullptr, debug1, TextID::DEBUG_MENU_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::SONG_LIST_NAME, nullptr, FillSongs, TextID::SONG_LIST_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::FILL_CATALOG_NAME, nullptr, FillCatalog, TextID::FILL_CATALOG_NOTE)),
		PSAVEC->Append(new MenuEntryExtras(TextID::QR_MACHINE_NAME, nullptr, unlockqrmachine, TextID::QR_MACHINE_NOTE)),
		PLAYC->Append(PSAVEC);
		PLAYC->Append(new MenuEntryExtras(TextID::PLAYER_INFO, debug, TextID::PLAYER_INFO_NOTE)),
	//PLAYC->Append(new MenuEntryExtras(TextID::PLAYER_LOADER, nullptr, pLoaderEntry, TextID::PLAYER_LOADER_NOTE)),
		PLAYC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::NECK_POSITION, neckentry, TextID::NECK_POSITION_NOTE), {
			HotkeyExtras(Key::L | Key::DPadDown, TextID::NECK_POSITION)
		})),
		PLAYC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::CUSTOM_HAIR_EYE, App_ColorMod, SaveColor, TextID::CUSTOM_HAIR_EYE_NOTE), {
			HotkeyExtras(Key::R, TextID::CUSTOM_HAIR_EYE_KEY1)
		})),
		PLAYC->Append(new MenuEntryExtras(TextID::WEAR_HELMET, hatz, TextID::WEAR_HELMET_NOTE)),
		PLAYC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::FAINT, Faint, TextID::FAINT_NOTE), {
			HotkeyExtras(Key::R | Key::A, TextID::FAINT)
		})),
		PLAYC->Append(new MenuEntryExtras(TextID::PLAYERS_ON_MAP, map, TextID::PLAYERS_ON_MAP_NOTE)),

		PLAYC->Append(new MenuEntryExtras(TextID::NEVER_BEDHEAD, NeverBedHead, TextID::NEVER_BEDHEAD_NOTE)),
		menu->Append(PLAYC);

	//////////////////////////
	/*Animation Codes Folder*/
	//////////////////////////
		MenuFolderExtras *ANIMC = new MenuFolderExtras(FolderType::Animation);
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::PLAYER_SELECTOR, playerSelector, TextID::PLAYER_SELECTOR_NOTE), {
			HotkeyExtras(Key::L | Key::DPadRight, TextID::PLAYER_SELECTOR_KEY1),
			HotkeyExtras(Key::L | Key::DPadLeft, TextID::PLAYER_SELECTOR_KEY2)
		})),
		ANIMC->Append(new MenuEntryExtras(TextID::ANTI_ANIM, anticheat, TextID::ANTI_ANIM_NOTE)),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::ANIMATION_MOD, execAnim, TextID::ANIMATION_MOD_NOTE), {
			HotkeyExtras(Key::A | Key::DPadRight, TextID::ANIMATIONS_NOTE_HOTKEY1),
			HotkeyExtras(Key::A | Key::DPadLeft, TextID::ANIMATIONS_NOTE_HOTKEY2),
			HotkeyExtras(Key::A | Key::DPadUp, TextID::ANIMATIONS_NOTE_HOTKEY3),
			HotkeyExtras(Key::A | Key::B, TextID::ANIMATIONS_NOTE_HOTKEY4)
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::EMOTION_LOOP, inf_expression, TextID::EMOTION_LOOP_NOTE), {
			HotkeyExtras(Key::B, TextID::EMOTION_LOOP)
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::IDLE, idle, TextID::IDLE_NOTE), {
			HotkeyExtras(Key::R, TextID::IDLE)
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::SLOW_MO_ANIM, slmoanms, TextID::SLOW_MO_ANIM_NOTE), {
			HotkeyExtras(Key::L | Key::DPadLeft, TextID::SLOW_MO_ANIM)
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::ANIM_ON_ALL, doonall, TextID::ANIM_ON_ALL_NOTE), {
			HotkeyExtras(Key::R | Key::A, TextID::ANIM_ON_ALL_KEY1),
			HotkeyExtras(Key::R | Key::B, TextID::ANIM_ON_ALL_KEY2),
			HotkeyExtras(Key::R | Key::Y, TextID::ANIM_ON_ALL_KEY3)
		})),
		menu->Append(ANIMC);

	////////////////////////
	/*Seeding Codes Folder*/
	////////////////////////
		MenuFolderExtras *SEEDC = new MenuFolderExtras(FolderType::Seeding);
	////////////////////////
	/*Seed Codes SubFolder*/
	////////////////////////
		MenuFolderExtras *SEED1C = new MenuFolderExtras(FolderType::Seeding, SubFolder::Seed);
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras(TextID::PICK_SEEDER, pickseeder, TextID::PICK_SEEDER_NOTE), {
			HotkeyExtras(Key::B | Key::DPadLeft, TextID::PICK_SEEDER_KEY1),
			HotkeyExtras(Key::B | Key::DPadDown, TextID::PICK_SEEDER_KEY2),
			HotkeyExtras(Key::B | Key::DPadUp, TextID::PICK_SEEDER_KEY3),
			HotkeyExtras(Key::B | Key::DPadRight, TextID::PICK_SEEDER_KEY4),
			HotkeyExtras(Key::B | Key::L, TextID::PICK_SEEDER_KEY5)
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras(TextID::WALK_SEEDER, Walkseeder, TextID::WALK_SEEDER_NOTE), {
			HotkeyExtras(Key::R | Key::B, TextID::WALK_SEEDER)
		})),
		SEED1C->Append(new MenuEntryExtras(TextID::FIREWORK_SEEDER, nullptr, fireworkentry, TextID::FIREWORK_SEEDER_NOTE)),
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras(TextID::MAP_EDITOR, tileSelector, TextID::MAP_EDITOR_NOTE), {
			HotkeyExtras(Key::Start | Key::DPadUp, TextID::MAP_EDITOR_KEY1), //Toggle Editor
			HotkeyExtras(Key::DPadRight, TextID::MAP_EDITOR_KEY2),
			HotkeyExtras(Key::DPadLeft, TextID::MAP_EDITOR_KEY3),
			HotkeyExtras(Key::DPadDown, TextID::MAP_EDITOR_KEY4),
			HotkeyExtras(Key::DPadUp, TextID::MAP_EDITOR_KEY5),
			HotkeyExtras(Key::R, TextID::MAP_EDITOR_KEY6), //Cycle Size
			HotkeyExtras(Key::A, TextID::MAP_EDITOR_KEY7), //Place Item
			HotkeyExtras(Key::X, TextID::MAP_EDITOR_KEY8), //Remove Item
			HotkeyExtras(Key::Y, TextID::MAP_EDITOR_KEY9), //Copy Item
			HotkeyExtras(Key::L, TextID::MAP_EDITOR_KEY10) //Set Item ID
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntryExtras(TextID::ALL_SEEDER, everythingseeder, E_Seeder_KB, TextID::ALL_SEEDER_NOTE), {
			HotkeyExtras(Key::L, TextID::ALL_SEEDER_KEY1)
		})),
		SEEDC->Append(SEED1C);
	////////////////////////
	/*Drop Codes SubFolder*/
	////////////////////////
		MenuFolderExtras *DROPC = new MenuFolderExtras(FolderType::Seeding, SubFolder::Drop);
		DROPC->Append(new MenuEntryExtras(TextID::ITEM_SEQUENCER, nullptr, Entry_itemsequence, TextID::ITEM_SEQUENCER_NOTE)),
	    DROPC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::DROP_MODS, dropMod, TextID::DROP_MODS_NOTE), {
			HotkeyExtras(Key::A | Key::DPadRight, TextID::DROP_MODS_KEY1),
			HotkeyExtras(Key::A | Key::DPadLeft, TextID::DROP_MODS_KEY2),
			HotkeyExtras(Key::A | Key::DPadDown, TextID::DROP_MODS_KEY3),
			HotkeyExtras(Key::A | Key::DPadUp, TextID::DROP_MODS_KEY4)
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::DROP_ITEMS, instantDrop, TextID::DROP_ITEMS_NOTE), {
			HotkeyExtras(Key::Y | Key::DPadRight, TextID::DROP_ITEMS_KEY1),
			HotkeyExtras(Key::L | Key::DPadDown, TextID::DROP_ITEMS_KEY2)
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::AUTO_DROP, autoDrop, TextID::AUTO_DROP_NOTE), {
			HotkeyExtras(Key::Y | Key::DPadRight, TextID::AUTO_DROP_KEY1),
			HotkeyExtras(Key::Y | Key::DPadLeft, TextID::AUTO_DROP_KEY2)
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::TOUCH_DROP, touchDrop, TextID::TOUCH_DROP_NOTE), {
			HotkeyExtras(Key::Y | Key::DPadRight, TextID::TOUCH_DROP_KEY1)
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::SLOT_DROP, ShowInvSlotID, TextID::SLOT_DROP_NOTE), {
			HotkeyExtras(Key::R | Key::Y, TextID::SLOT_DROP_KEY1),
			HotkeyExtras(Key::R | Key::X, TextID::SLOT_DROP_KEY2)
		})),
		SEEDC->Append(DROPC);
	////////////////////////
	/*Tree Codes SubFolder*/
	////////////////////////
		MenuFolderExtras *TREEC = new MenuFolderExtras(FolderType::Seeding, SubFolder::Tree);
		TREEC->Append(new MenuEntryExtras(TextID::INF_FRUIT_TREE, fruitStays, TextID::INF_FRUIT_TREE_NOTE)),
		TREEC->Append(new MenuEntryExtras(TextID::AXE_TREE_SHAKE, shakechop, TextID::AXE_TREE_SHAKE_NOTE)),
		TREEC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::FRUIT_TREE_MOD, fruititemmod, TextID::FRUIT_TREE_MOD_NOTE), {
			HotkeyExtras(Key::A | Key::DPadLeft, TextID::FRUIT_TREE_MOD_KEY1)
		})),
		TREEC->Append(new MenuEntryExtras(TextID::INST_TREE_CHOP, instantchop, TextID::INST_TREE_CHOP_NOTE)),
		SEEDC->Append(TREEC),
		menu->Append(SEEDC);

	//////////////////////
	/*Money Codes Folder*/
	//////////////////////
		MenuFolderExtras *MONC = new MenuFolderExtras(FolderType::Money);
		MONC->Append(new MenuEntryExtras(TextID::WALLET, nullptr, wallet, TextID::WALLET_NOTE)),
		MONC->Append(new MenuEntryExtras(TextID::BANK, nullptr, bank, TextID::BANK_NOTE)),
		MONC->Append(new MenuEntryExtras(TextID::MEOW_COUPONS, nullptr, coupon, TextID::MEOW_COUPONS_NOTE)),
		MONC->Append(new MenuEntryExtras(TextID::BADGES, nullptr, badges, TextID::BADGES_NOTE)),
		MONC->Append(new MenuEntryExtras(TextID::MEDALS, nullptr, medals, TextID::MEDALS_NOTE)),
		MONC->Append(new MenuEntryExtras(TextID::TURNIPS, nullptr, turnips, TextID::TURNIPS_NOTE)),
		menu->Append(MONC);

	///////////////////////
	/*Island Codes Folder*/
	///////////////////////
		MenuFolderExtras *ISLC = new MenuFolderExtras(FolderType::Island);
		ISLC->Append(new MenuEntryExtras(TextID::UNLOCK_ISLAND, nullptr, UnlockIsland, TextID::UNLOCK_ISLAND_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::FILL_INV_ORE, bonusOre, TextID::FILL_INV_ORE_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::FILL_INV_FRUIT, instantFruit, TextID::FILL_INV_FRUIT_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::FREE_KAPPN, FreeKappn, TextID::FREE_KAPPN_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::ISLAND_SHOP_MOD, IslandShop, IslandSettings, TextID::ISLAND_SHOP_MOD_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::ALL_TOURS, alltour, TextID::ALL_TOURS_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::ISLAND_ACRE, acreMod, menuAcreMod, TextID::ISLAND_ACRE_NOTE)),
		ISLC->Append(new MenuEntryExtras(TextID::ISLAND_BUILDING, buildingMod, menuBuildingMod, TextID::ISLAND_BUILDING_NOTE)),
		ISLC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::ISLAND_SAVER, IslandSaver, TextID::ISLAND_SAVER_NOTE), {
			HotkeyExtras(Key::L | Key::Y, TextID::OPEN_MENU)
		})),
		menu->Append(ISLC);

	//////////////
	/*NPC Folder*/
	//////////////
		MenuFolderExtras *NPCC = new MenuFolderExtras(FolderType::NPC);
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::NPC_SELECTOR, NPCFunction, TextID::NPC_SELECTOR_NOTE), {
			HotkeyExtras(Key::L | Key::A, TextID::NPC_SELECTOR_KEY1)
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::NPC_ANIMATION,  NPCAnimation, NPCSetAnim, TextID::NPC_ANIMATION_NOTE), {
			HotkeyExtras(Key::L | Key::B, TextID::NPC_ANIMATION_KEY1)
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::NPC_COORDINATE, NPCCoordinates, TextID::NPC_COORDINATE_NOTE), {
			HotkeyExtras(Key::L, TextID::NPC_COORDINATE_KEY1),
			HotkeyExtras(Key::DPadRight, TextID::NPC_COORDINATE_KEY2),
			HotkeyExtras(Key::DPadLeft, TextID::NPC_COORDINATE_KEY3),
			HotkeyExtras(Key::DPadDown, TextID::NPC_COORDINATE_KEY4),
			HotkeyExtras(Key::DPadUp, TextID::NPC_COORDINATE_KEY5)
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::NPC_TELEPORT, NPCTeleportToYou, TextID::NPC_TELEPORT_NOTE), {
			HotkeyExtras(Key::L | Key::Y, TextID::NPC_TELEPORT_KEY1)
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::NPC_ROTATION, NPCRotate, TextID::NPC_ROTATION_NOTE), {
			HotkeyExtras(Key::L, TextID::NPC_ROTATION_KEY1)
		})),
		menu->Append(NPCC);

	////////////////////////////
	/*Environment Codes Folder*/
	////////////////////////////
		MenuFolderExtras *ENVC = new MenuFolderExtras(FolderType::Environment);
	/////////////////////
	/*Fish Codes Folder*/
	/////////////////////
		MenuFolderExtras *FISC = new MenuFolderExtras(FolderType::Environment, SubFolder::Fish);
		FISC->Append(new MenuEntryExtras(TextID::FISH_ALWAYS_BITE_NAME, FishAlwaysBiteRightAway, TextID::FISH_ALWAYS_BITE_NOTE)),
		FISC->Append(new MenuEntryExtras(TextID::FISH_CANT_SCARE_NAME, FishCantBeScared, TextID::FISH_CANT_SCARE_NOTE)),
		ENVC->Append(FISC);
	///////////////////////
	/*Insect Codes Folder*/
	///////////////////////
		MenuFolderExtras *INSC = new MenuFolderExtras(FolderType::Environment, SubFolder::Insect);
		INSC->Append(new MenuEntryExtras(TextID::SPAWN_INSECT, SpawnInsectEntry, SetInsectIdEntry, TextID::SPAWN_INSECT_NOTE)),
		INSC->Append(new MenuEntryExtras(TextID::INSECT_CANT_SCARE, InsectsCantBeScared, TextID::INSECT_CANT_SCARE_NOTE)),
		ENVC->Append(INSC);

		ENVC->Append(new MenuEntryExtras(TextID::DAYTIME, Daytime, TextID::DAYTIME_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::ALWAYS_AURORA_MOD, auroralights , TextID::ALWAYS_AURORA_MOD_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::UNBREAK_FLOWER, unbreakableflower, TextID::UNBREAK_FLOWER_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::WEATHER_MOD, nullptr, Weathermod , TextID::WEATHER_MOD_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::WATER_FLOWERS_NAME, nullptr, WaterAllFlowers, TextID::WATER_FLOWERS_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::WEED_REMOVER_NAME, nullptr, weedremover, TextID::WEED_REMOVER_NOTE)),
		ENVC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::GRASS_EDITOR, grasseditor, grasscomplete, TextID::GRASS_EDITOR_NOTE), {
			HotkeyExtras(Key::R | Key::DPadDown, TextID::GRASS_EDITOR_HOTKEY1),
			HotkeyExtras(Key::R | Key::DPadUp, TextID::GRASS_EDITOR_HOTKEY2),
			HotkeyExtras(Key::R | Key::DPadRight, TextID::GRASS_EDITOR_HOTKEY3)
		})),
		ENVC->Append(new MenuEntryExtras(TextID::KEEP_GRASS_STATE, KeepGrassState, TextID::KEEP_GRASS_STATE_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::BURIED_INSPECTOR, BuriedInspector, TextID::BURIED_INSPECTOR_NOTE)),
		ENVC->Append(new MenuEntryExtras(TextID::ITEMS_DONT_DISSAPPEAR, ItemsDontDissappearOnInvalidPositions, TextID::ITEMS_DONT_DISSAPPEAR_NOTE)),

		menu->Append(ENVC);

	//////////////////////
	/*Extra Codes Folder*/
	//////////////////////
		MenuFolderExtras *EXTC = new MenuFolderExtras(FolderType::Extra);
	/////////////////////
	/*Chat Codes Folder*/
	/////////////////////
		MenuFolderExtras *CHAC = new MenuFolderExtras(FolderType::Extra, SubFolder::Chat);
		CHAC->Append(new MenuEntryExtras(TextID::CHAT_DONT_DISSAPEAR, bubblesDisappear, TextID::CHAT_DONT_DISSAPEAR_NOTE)),
		CHAC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::CHATCOPYPASTE, ChatCopyPaste, TextID::CHATCOPYPASTE_NOTE), {
			HotkeyExtras(Key::L | Key::DPadRight, TextID::CHATCOPYPASTE_KEY1),
			HotkeyExtras(Key::L | Key::DPadUp, TextID::CHATCOPYPASTE_KEY2),
			HotkeyExtras(Key::L | Key::DPadDown, TextID::CHATCOPYPASTE_KEY3),
			HotkeyExtras(Key::L | Key::DPadLeft, TextID::CHATCOPYPASTE_KEY4)
		})),
		CHAC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::FORCE_CHAT, Forcesendchat, TextID::FORCE_CHAT_NOTE), {
			HotkeyExtras(Key::R, TextID::FORCE_CHAT)
		})),
		CHAC->Append(new MenuEntryExtras(TextID::CHAT_COMMANDS, chatCommands, TextID::CHAT_COMMANDS_NOTE)),
		CHAC->Append(new MenuEntryExtras(TextID::CHAT_BUTTON, ChatButton, TextID::CHAT_BUTTON_NOTE)),
		CHAC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::CHAT_TRANSLATION_OPTIONS, ChatTranslationOptions, TextID::CHAT_TRANSLATION_OPTIONS_NOTE), {
			HotkeyExtras(Key::X | Key::DPadLeft, TextID::CHAT_TRANSLATION_OPTIONS)
		})),
		EXTC->Append(CHAC);
	////////////////////
	/*Fun Codes Folder*/
	////////////////////
		MenuFolderExtras *FUNC = new MenuFolderExtras(FolderType::Extra, SubFolder::Fun);
		FUNC->Append(new MenuEntryExtras(TextID::SIZE_CODES, nullptr, sizecodes, TextID::SIZE_CODES_NOTE)),
		FUNC->Append(new MenuEntryExtras(TextID::T_POSE, tposeentry, TextID::T_POSE_NOTE)),
		FUNC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::TAKE_TPC_PIC, freezeframe, TextID::TAKE_TPC_PIC_NOTE), {
			HotkeyExtras(Key::DPadRight, TextID::TAKE_TPC_PIC_KEY1),
			HotkeyExtras(Key::A, TextID::TAKE_TPC_PIC_KEY2)
		})),
		FUNC->Append(new MenuEntryExtras(TextID::MAX_TURBO, maxturbo, TextID::MAX_TURBO_NOTE)),
		FUNC->Append(new MenuEntryExtras(TextID::MULTI_PRESS, asmpresses, TextID::MULTI_PRESS_NOTE)),
		FUNC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::ULTIMATE_POPPER, partypopper, TextID::ULTIMATE_POPPER_NOTE), {
			HotkeyExtras(Key::B | Key::DPadLeft, TextID::ULTIMATE_POPPER_KEY1)
		})),
	    FUNC->Append(new MenuEntryExtras(TextID::CAMERA_MOD, cameramod, TextID::CAMERA_MOD_NOTE)),
		EXTC->Append(FUNC);
		EXTC->Append(new MenuEntryExtras(TextID::SHOP_ALWAYS_OPEN_NAME, ShopsAlwaysOpen, TextID::SHOP_ALWAYS_OPEN_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::DISABLE_SAVE, nonesave, TextID::DISABLE_SAVE_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::DISABLE_ITEM_LOCKS, bypass, TextID::DISABLE_ITEM_LOCKS_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::CANT_FALL_HOLE, noTrap, TextID::CANT_FALL_HOLE_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::SET_SPOT_STATE_NAME, nullptr, SetSpotState, TextID::SET_SPOT_STATE_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::SEARCH_REPLACE_NAME, nullptr, SearchReplace, TextID::SEARCH_REPLACE_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::REMOVE_MAP_ITEMS_NAME, nullptr, RemoveItemsCheat, TextID::REMOVE_MAP_ITEMS_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::EDIT_PATTERN_NAME, editpattern, TextID::EDIT_PATTERN_NOTE)),
		EXTC->Append(new MenuEntryExtras(TextID::AMIIBO_SPOOFER, AmiiboSpoofer, TextID::AMIIBO_SPOOFER_NOTE)),
		EXTC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::TIME_TRAVEL, TimeTravel, TTKeyboard, TextID::TIME_TRAVEL_NOTE), {
			HotkeyExtras(Key::R | Key::DPadRight, TextID::TIME_FORWARD),
			HotkeyExtras(Key::R | Key::DPadLeft, TextID::TIME_BACKWARDS)
		})),
		menu->Append(EXTC);

	/////////////////////
	/*Misc Codes Folder*/
	/////////////////////
		MenuFolderExtras *MISC = new MenuFolderExtras(FolderType::Misc);
		MISC->Append(new MenuEntryExtras(TextID::TOOL_ANIM, nullptr, tooltype, TextID::TOOL_ANIM_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::GAME_TYPE, nullptr, mgtype, TextID::GAME_TYPE_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::RADIO_PLAYER, radioPlayer , TextID::RADIO_PLAYER_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::RELOAD_ROOM_NAME, nullptr, ReloadRoomCheat, TextID::RELOAD_ROOM_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::MORE_NUMBERS,  morenumberisland, TextID::MORE_NUMBERS_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::LARGE_FOV, fovlarge, TextID::LARGE_FOV_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::MOVE_FURN, roomSeeder, TextID::MOVE_FURN_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::WALK_TALK, walktalkentry, TextID::WALK_TALK_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::KEY_EXTEND, key_limit, TextID::KEY_EXTEND_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::CUSTOM_KEY, CustomKeyboard, TextID::CUSTOM_KEY_NOTE)),
		MISC->Append(EntryWithHotkey(new MenuEntryExtras(TextID::BEANS_PARTICLE, BeansParticleChanger, TextID::BEANS_PARTICLE_NOTE), {
			HotkeyExtras(Key::L | Key::DPadLeft, TextID::BEANS_PARTICLE)
		})),
		MISC->Append(new MenuEntryExtras(TextID::FAST_MODE, fast, TextID::FAST_MODE_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::FAST_TEXT_SPEED, fasttalk, TextID::FAST_TEXT_SPEED_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::FAST_GAME_SPEED, speedentry, TextID::FAST_GAME_SPEED_NOTE)),
		MISC->Append(new MenuEntryExtras(TextID::FAST_ISABELLE, fastisabelle, TextID::FAST_ISABELLE_NOTE)),
		menu->Append(MISC);

	////////////////////////
	/*Default Codes Folder*/
	////////////////////////
	MenuFolderExtras *DEFAULTC = new MenuFolderExtras(FolderType::Default);

	DEFAULTC->Append(new MenuEntryExtras(TextID::SEED_ITEM_LEGIT, nullptr, SeedItemLegitimacyEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::DROP_LAG_REMOVE, nullptr, OnlineDropLagRemoverEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::CHANGE_ROCKBREAK_PARTICLE, nullptr, ChangeRockbreakParticleEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::DROP_ITEMS_EVERYWHERE, nullptr, DropItemsEverywhereEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::IDLE_AFTER_TREE_SHAKE_OR_CUT, nullptr, IdleAfterTreeShakeOrCutEntry, TextID::NONE)),
	//DEFAULTC->Append(new MenuEntryExtras(TextID::DONT_MOVE_NPC_BACK_TO_ORIGINAL_POSITION, nullptr, DontMoveNPCBackToOriginalPositionEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::REPLACE_DROP_FUNCTIONS, nullptr, ReplaceDropFunctionsEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::PREVENT_PARTICLE_CRASH, nullptr, PreventParticleCrashEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::BYPASS_GAME_CHECKS, nullptr, BypassGameChecksEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::DISABLE_NON_SEED_ITEM_CHECK, nullptr, DisableNonSeedItemCheckEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::PATCH_DROP_FUNCTION, nullptr, PatchDropFunctionEntry, TextID::NONE)),

	DEFAULTC->Append(new MenuEntryExtras(TextID::CHECK_INVALID_BADGE, nullptr, CheckInvalidBadgeEntry, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::DISABLE_OPEN_SAVE_MENU_WITH_START_BUTTON, nullptr, DisableOpenSaveMenuWithStartButton, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::DISABLE_CATALOG_SEARCH_FUNCTION, nullptr, DisableCatalogSearchFunction, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_INVALID_PICKUP_CRASH, nullptr, FixInvalidPickupCrash, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_INVALID_DROP_PLANT_CRASH, nullptr, FixInvalidDropPlantCrash, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_INVALID_SPRITE_CRASH, nullptr, FixInvalidSpriteCrash, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_INVALID_GIVE_ITEM_CRASH, nullptr, FixInvalidGiveItemCrash, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_INVALID_HOLE_CRASH, nullptr, FixInvalidHoleCrash, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_INVALID_ITEM_CRASH, nullptr, FixInvalidItemCrash, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::CONVERT_FLOWER_FROM_SEED_ITEM_TO_NORMAL_ITEM, nullptr, ConvertFlowerFromSeedItemToNormalItem, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::SET_SEED_ITEM_NAMES, nullptr, SetSeedItemNames, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::SET_ITEM_REPLACEMENT_RULES, nullptr, SetItemReplacementRules, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::SET_DROP_RULES, nullptr, SetDropRules, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::SET_PLANT_RULES, nullptr, SetPlantRules, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::FIX_PARTICLES_IN_PUZZLE_LEAGUE, nullptr, FixParticlesInPuzzleLeague, TextID::NONE)),
	DEFAULTC->Append(new MenuEntryExtras(TextID::SET_CUSTOM_SPRITES_FOR_PRO_DESIGNS, nullptr, SetCustomSpritesForProDesigns, TextID::NONE)),
	menu->Append(DEFAULTC);

	////////////////////
	/*Dev Codes Folder*/
	////////////////////
	#if DEVMODE
		MenuFolderExtras *DEVC = new MenuFolderExtras(FolderType::Dev);
		DEVC->Append(new MenuEntryExtras(TextID::CUSTOM_DUMP, nullptr, customdump, TextID::CUSTOM_DUMP_NOTE)),
		DEVC->Append(new MenuEntryExtras(TextID::CALL_FUNCTION, FunctionsCaller, FunctionsCallerSettings, TextID::CALL_FUNCTION_NOTE)),
		DEVC->Append(new MenuEntryExtras(TextID::DISPLAY_SAVE_FLAG_DIFFERENCE, displaySaveFlagDifference, TextID::NONE)),
		DEVC->Append(new MenuEntryExtras(TextID::UNUSED_FALL_DOWN, falldownfishing, TextID::NONE)),
		DEVC->Append(new MenuEntryExtras(TextID::UNLOCK_CRO_REGION, unlockCroRegion, TextID::UNLOCK_CRO_REGION_NOTE)),
		DEVC->Append(new MenuEntryExtras(TextID::ACNH_CHEAT, acnh, TextID::NONE)),
		DEVC->Append(new MenuEntryExtras(TextID::PLAYER_DUMPER, nullptr, player_dumper, TextID::NONE)),
		DEVC->Append(new MenuEntryExtras(TextID::FISH_THROWER, FishThrower, TextID::NONE)),
		DEVC->Append(new MenuEntryExtras(TextID::WAND_ABILITY, wandability, TextID::WAND_ABILITY_NOTE)),
		DEVC->Append(new MenuEntryExtras(TextID::LIGHT_SWITCH_TOGGLER, lightswitch, TextID::LIGHT_SWITCH_TOGGLER_NOTE)),
		DEVC->Append(new MenuEntryExtras(TextID::EXPRESSION_MOD, FacialExpressionMod, SetFacialExpression, TextID::EXPRESSION_MOD_NOTE)),
		menu->Append(DEVC);
	#endif

		menu->Append(new MenuEntryExtras(TextID::PLUGIN_SETTINGS, nullptr, pluginSettingsEntry, TextID::NONE));
	}
}
