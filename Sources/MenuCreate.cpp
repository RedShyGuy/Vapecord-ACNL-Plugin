#include "cheats.hpp"
#include "MenuPointers.hpp"

namespace CTRPluginFramework {
    static MenuEntry *EntryWithHotkey(MenuEntry *entry, const std::vector<Hotkey> &hotkeys) {
        if(entry != nullptr) {
            for(const Hotkey &hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }
        return(entry);
    }	
	
	static MenuEntry *EntryWithHotkey(MenuEntry *entry, const Hotkey &hotkey) {
        if(entry != nullptr) {
            entry->Hotkeys += hotkey;
            entry->SetArg(new std::string(entry->Name()));
            //entry->Name() += " " + hotkey.ToString();
            /*entry->Hotkeys.OnHotkeyChangeCallback([](MenuEntry *entry, int index) {
				UpdateAll();
            });*/
        }
        return(entry);
    }

	static void SetUp(MenuFolder *objfolder, bool isSubFolder = false) {
		if(objfolder != nullptr) {
			std::vector<EntryData *> objentryData;

			std::vector<MenuEntry *> entrys = objfolder->GetEntryList();
			
			OSD::Notify(Utils::Format("SetUp %d", entrys.size()));

			for(auto obj_entry : entrys) {
				if(obj_entry != nullptr) {
					std::vector<std::string> hotkeys;
					for(int i = 0; i < obj_entry->Hotkeys.Count(); ++i) 
						hotkeys.push_back(GetHotkeyName(obj_entry->Hotkeys[i]));

					EntryData *edata = new EntryData();
					edata->entry = obj_entry;
					edata->IndexColor = GetColorFromString(obj_entry->Name());
					edata->IndexName = RemoveColorFromString(obj_entry->Name());
					edata->IndexNote = obj_entry->Note();
					edata->IndexHotkeys = hotkeys;

					objentryData.push_back(edata);
				}
			}

			FolderData *data = new FolderData();
			data->folder = objfolder;
			data->IndexColor = GetColorFromString(objfolder->Name());
			data->IndexName = RemoveColorFromString(objfolder->Name());
			data->IndexNote = objfolder->Note();
			data->IsSubFolder = isSubFolder;
			data->entryData = objentryData;

			folderData.push_back(data);
		}
	}

/*This will load all the folders and entrys*/
    void InitMenu(PluginMenu *menu) {
	/////////////////////
	/*Save Codes Folder*/
	/////////////////////
		MenuFolder *SAVEC = new MenuFolder(FolderColors[0] << "SAVE_CODES");
	    SAVEC->Append(new MenuEntry(FolderColors[0] << "TOWN_NAME_CHANGER", nullptr, townnamechanger, "TOWN_NAME_CHANGER_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "SAVE_BACKUP_NAME", nullptr, savebackup, "SAVE_BACKUP_NOTE")),	
		SAVEC->Append(new MenuEntry(FolderColors[0] << "BULL_BOARD_DUMPER", nullptr, bullboard, "BULL_BOARD_DUMPER_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "TREESIZE_NAME", nullptr, TreeSizeChanger, "TREESIZE_NOTE")),	
		SAVEC->Append(new MenuEntry(FolderColors[0] << "CHANGE_NATIVE_NAME", nullptr, ChangeNativeFruit, "CHANGE_NATIVE_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "PWP_UNLOCK_NAME", nullptr, PWPUnlock, "PWP_UNLOCK_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "GRASS_CHANGER_NAME", nullptr, GrassChanger, "GRASS_CHANGER_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "CARAVAN_SET", nullptr, caravanchange, "CARAVAN_SET_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "CAMPING_SET", nullptr, SetCampingVillager, "CAMPING_SET_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "SHOP_UPGRADE", nullptr, shopunlocks, "SHOP_UPGRADE_NOTE")),
	//SAVEC->Append(new MenuEntry(FolderColors[0] << "HOUSE_EDITOR_NAME", nullptr, HouseChanger, "HOUSE_EDITOR_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "QR_MACHINE_NAME", nullptr, unlockqrmachine, "QR_MACHINE_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "BUILDING_MOD_NAME", nullptr, BuildingMod, "BUILDING_MOD_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "FILL_MUSEUM_NAME", nullptr, CompleteMuseum, "FILL_MUSEUM_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "MAYOR_PERMIT_NAME", Permit100, "MAYOR_PERMIT_NOTE")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "REAL_TIME_ACRE", MapEditor, "REAL_TIME_ACRE_NOTE")),
		menu->Append(SAVEC);
		
	/////////////////////////
	/*Movement Codes Folder*/
	/////////////////////////
		MenuFolder *MOVEC = new MenuFolder(FolderColors[1] << "MOVEMENT_CODES");
		MOVEC->Append(new MenuEntry(FolderColors[1] << "CANT_PUSH", noPush, "CANT_PUSH_NOTE")),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "COORD_MOD", coordinate, coordspeed, "COORD_MOD_NOTE"), {
			Hotkey(Key::A, "COORD_MOD_KEY1"), 
			Hotkey(Key::DPadRight, "COORD_MOD_KEY2"), 
			Hotkey(Key::DPadLeft, "COORD_MOD_KEY3"), 
			Hotkey(Key::DPadDown, "COORD_MOD_KEY4"), 
			Hotkey(Key::DPadUp, "COORD_MOD_KEY5") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "MOON_JUMP", moonjump, "MOON_JUMP_NOTE"), { 
			Hotkey(Key::L | Key::DPadUp, "MOON_JUMP_KEY1"), 
			Hotkey(Key::L | Key::DPadDown, "MOON_JUMP_KEY2") 
		})),
		MOVEC->Append(new MenuEntry(FolderColors[1] << "TOUCH_WARP", tch_warp, "TOUCH_WARP_NOTE")),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "WALK_OVER", walkOver, "WALK_OVER_NOTE"), { 
			Hotkey(Key::L | Key::DPadUp, "WALK_OVER") 
		})),		   
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "MOVEMENT_CHANGE", MovementChanger, "MOVEMENT_CHANGE_NOTE"), { 
			Hotkey(Key::L | Key::B, "MOVEMENT_CHANGE") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "WALK_PARTICLE_CHANGE", Walkparticle, "WALK_PARTICLE_CHANGE_NOTE"), { 
			Hotkey(Key::L | Key::X, "WALK_PARTICLE_CHANGE")
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "PLAYER_TELEPORT", stalk, "PLAYER_TELEPORT_NOTE"), { 
			Hotkey(Key::R | Key::DPadLeft, "PLAYER_TELEPORT_KEY1"), 
			Hotkey(Key::R | Key::DPadRight, "PLAYER_TELEPORT_KEY2") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "VISIBILITY_MOD", onlineplayermod, "VISIBILITY_MOD_NOTE"), { 
			Hotkey(Key::L | Key::A, "VISIBILITY_MOD_KEY1") 
		})),
		MOVEC->Append(new MenuEntry(FolderColors[1] << "SPEED_MOD", speedMod, menuSpeedMod, "SPEED_MOD_NOTE")),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "ROOM_WARPING", roomWarp, "ROOM_WARPING_NOTE"), { 
			Hotkey(Key::L | Key::X, "ROOM_WARPING") 
		})),
		MOVEC->Append(new MenuEntry(FolderColors[1] << "SHOVEL_KNOCKBACK", shovelknockback, "SHOVEL_KNOCKBACK_NOTE")),
		menu->Append(MOVEC);

	//////////////////////////
	/*Inventory Codes Folder*/
	//////////////////////////
		MenuFolder *INVC = new MenuFolder(FolderColors[2] << "INVENTORY_CODES");
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "TEXT_2_ITEM", t2i, "TEXT_2_ITEM_NOTE"), { 
			Hotkey(Key::X | Key::DPadRight, "TEXT_2_ITEM_KEY1"), 
			Hotkey(Key::X |Key::DPadUp, "TEXT_2_ITEM_KEY2"), 
			Hotkey(Key::X | Key::DPadDown, "TEXT_2_ITEM_KEY3"),
			Hotkey(Key::X | Key::DPadLeft, "TEXT_2_ITEM_KEY4"),
		})),
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "DUPE_ITEMS", duplication, "DUPE_ITEMS_NOTE"), { 
			Hotkey(Key::R, "DUPE_ITEMS_KEY1"), 
			Hotkey(Key::R | Key::X, "DUPE_ITEMS_KEY2") 
		})),
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "CATALOG_TO_POCKET", catalog, "CATALOG_TO_POCKET_NOTE"), { 
			Hotkey(Key::L | Key::DPadRight, "CATALOG_TO_POCKET_KEY1"), 
			Hotkey(Key::L | Key::Y, "CATALOG_TO_POCKET_KEY2") 
		})),
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "CHAT_T2I", chatt2i, "CHAT_T2I_NOTE"), { 
			Hotkey(Key::R | Key::DPadLeft, "CHAT_T2I_KEY1") 
		})),
		INVC->Append(new MenuEntry(FolderColors[2] << "CLEAR_INV_NAME", nullptr, ClearInventory, "CLEAR_INV_NOTE")),
		INVC->Append(new MenuEntry(FolderColors[2] << "ITEM_SETTINGS", nullptr, itemsettings, "ITEM_SETTINGS_NOTE")),
		INVC->Append(new MenuEntry(FolderColors[2] << "SAVE_MENU_CHANGER", nullptr, MenuChanger, "SAVE_MENU_CHANGER_NOTE")),
		INVC->Append(new MenuEntry(FolderColors[2] << "GET_SET", nullptr, getset, "GET_SET_NOTE")),
		INVC->Append(new MenuEntry(FolderColors[2] << "CUSTOM_BUTTON", nullptr, SettingsButton, "CUSTOM_BUTTON_NOTE")),
		menu->Append(INVC);

	///////////////////////
	/*Player Codes Folder*/
	///////////////////////
		MenuFolder *PLAYC = new MenuFolder(FolderColors[3] << "PLAYER_CODES");
	////////////////////////////
	/*Player Save Codes Folder*/
	////////////////////////////
		MenuFolder *PSAVEC = new MenuFolder(FolderColors[3] << "PLAYER_SAVE_CODES");
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "NAME_CHANGER", nullptr, NameChanger, "NAME_CHANGER_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "PLAYER_APPEARANCE", nullptr, playermod, "PLAYER_APPEARANCE_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "RANDOM_PLAYER", nullptr, randomoutfit, "RANDOM_PLAYER_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "PLAYER_BACKUP_NAME", nullptr, playerbackup, "PLAYER_BACKUP_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "TPC_MESSAGE", nullptr, tpcmessage, "TPC_MESSAGE_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "TPC_SETTINGS", nullptr, tpc, "TPC_SETTINGS_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "DESIGN_DUMP", nullptr, DesignDumper, "DESIGN_DUMP_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "EMOTION_LIST", nullptr, emotelist, "EMOTION_LIST_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "ENCY_LIST", nullptr, enzyklopedia, "ENCY_LIST_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "DREAM_CODE", nullptr, comodifier, "DREAM_CODE_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "DEBUG_MENU", nullptr, debug1, "DEBUG_MENU_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "SONG_LIST_NAME", nullptr, FillSongs, "SONG_LIST_NOTE")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "FILL_CATALOG_NAME", nullptr, FillCatalog, "FILL_CATALOG_NOTE")),
		PLAYC->Append(PSAVEC);
		PLAYC->Append(new MenuEntry(FolderColors[3] << "PLAYER_INFO", debug, "PLAYER_INFO_NOTE")),
	//PLAYC->Append(new MenuEntry(FolderColors[3] << "PLAYER_LOADER", nullptr, pLoaderEntry, "PLAYER_LOADER_NOTE")),
		PLAYC->Append(EntryWithHotkey(new MenuEntry(FolderColors[3] << "NECK_POSITION", neckentry, "NECK_POSITION_NOTE"), { 
			Hotkey(Key::L | Key::DPadDown, "NECK_POSITION") 
		})),
		PLAYC->Append(EntryWithHotkey(new MenuEntry(FolderColors[3] << "CUSTOM_HAIR_EYE", App_ColorMod, SaveColor, "CUSTOM_HAIR_EYE_NOTE"), { 
			Hotkey(Key::R, "CUSTOM_HAIR_EYE_KEY1")
		})),
		PLAYC->Append(new MenuEntry(FolderColors[3] << "WEAR_HELMET", hatz, "WEAR_HELMET_NOTE")),
		PLAYC->Append(EntryWithHotkey(new MenuEntry(FolderColors[3] << "FAINT", Faint, "FAINT_NOTE"), {
			Hotkey(Key::R | Key::A, "FAINT")
		})),
		PLAYC->Append(new MenuEntry(FolderColors[3] << "PLAYERS_ON_MAP", map, "PLAYERS_ON_MAP_NOTE")),

		PLAYC->Append(new MenuEntry(FolderColors[3] << "NEVER_BEDHEAD", NeverBedHead, "NEVER_BEDHEAD_NOTE")),
		menu->Append(PLAYC);

	//////////////////////////
	/*Animation Codes Folder*/
	//////////////////////////
		MenuFolder *ANIMC = new MenuFolder(FolderColors[4] << "ANIMATION_CODES");
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "PLAYER_SELECTOR", playerSelector, "PLAYER_SELECTOR_NOTE"), { 
			Hotkey(Key::L | Key::DPadRight, "PLAYER_SELECTOR_KEY1"), 
			Hotkey(Key::L | Key::DPadLeft, "PLAYER_SELECTOR_KEY2") 
		})),
		ANIMC->Append(new MenuEntry(FolderColors[4] << "ANTI_ANIM", anticheat, "ANTI_ANIM_NOTE")),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "ANIMATION_MOD", execAnim, "ANIMATION_MOD_NOTE"), { 
			Hotkey(Key::A | Key::DPadRight, "ANIMATIONS_NOTE_HOTKEY1"), 
			Hotkey(Key::A | Key::DPadLeft, "ANIMATIONS_NOTE_HOTKEY2"), 
			Hotkey(Key::A | Key::DPadUp, "ANIMATIONS_NOTE_HOTKEY3"), 
			Hotkey(Key::A | Key::B, "ANIMATIONS_NOTE_HOTKEY4") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "EMOTION_LOOP", inf_expression, "EMOTION_LOOP_NOTE"), { 
			Hotkey(Key::B, "EMOTION_LOOP") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "IDLE", idle, "IDLE_NOTE"), { 
			Hotkey(Key::R, "IDLE") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "SLOW_MO_ANIM", slmoanms, "SLOW_MO_ANIM_NOTE"), { 
			Hotkey(Key::L | Key::DPadLeft, "SLOW_MO_ANIM") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "ANIM_ON_ALL", doonall, "ANIM_ON_ALL_NOTE"), { 
			Hotkey(Key::R | Key::A, "ANIM_ON_ALL_KEY1"), 
			Hotkey(Key::R | Key::B, "ANIM_ON_ALL_KEY2"), 
			Hotkey(Key::R | Key::Y, "ANIM_ON_ALL_KEY3") 
		})),
		menu->Append(ANIMC);

	////////////////////////
	/*Seeding Codes Folder*/
	////////////////////////
		MenuFolder *SEEDC = new MenuFolder(FolderColors[5] << "SEEDING_CODES");
	////////////////////////
	/*Seed Codes SubFolder*/
	////////////////////////
		MenuFolder *SEED1C = new MenuFolder(FolderColors[5] << "SEED_CODES");
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "PICK_SEEDER", pickseeder, "PICK_SEEDER_NOTE"), { 
			Hotkey(Key::B | Key::DPadLeft, "PICK_SEEDER_KEY1"), 
			Hotkey(Key::B | Key::DPadDown, "PICK_SEEDER_KEY2"), 
			Hotkey(Key::B | Key::DPadUp, "PICK_SEEDER_KEY3"), 
			Hotkey(Key::B | Key::DPadRight, "PICK_SEEDER_KEY4"), 
			Hotkey(Key::B | Key::L, "PICK_SEEDER_KEY5") 
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "WALK_SEEDER", Walkseeder, "WALK_SEEDER_NOTE"), { 
			Hotkey(Key::R | Key::B, "WALK_SEEDER") 
		})),
		SEED1C->Append(new MenuEntry(FolderColors[5] << "FIREWORK_SEEDER", nullptr, fireworkentry, "FIREWORK_SEEDER_NOTE")),
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "MAP_EDITOR", tileSelector, "MAP_EDITOR_NOTE"), { 
			Hotkey(Key::Start | Key::DPadUp, "MAP_EDITOR_KEY1"), 
			Hotkey(Key::DPadRight, "MAP_EDITOR_KEY2"), 
			Hotkey(Key::DPadLeft, "MAP_EDITOR_KEY3"), 
			Hotkey(Key::DPadDown, "MAP_EDITOR_KEY4"), 
			Hotkey(Key::DPadUp, "MAP_EDITOR_KEY5"), 
			Hotkey(Key::L, "MAP_EDITOR_KEY6"), 
			Hotkey(Key::R, "MAP_EDITOR_KEY7"), 
			Hotkey(Key::Start | Key::DPadDown, "MAP_EDITOR_KEY8"), 
			Hotkey(Key::Start | Key::DPadLeft, "MAP_EDITOR_KEY9"), 
			Hotkey(Key::A, "MAP_EDITOR_KEY10"), 
			Hotkey(Key::Start | Key::DPadRight, "MAP_EDITOR_KEY11") 
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "ALL_SEEDER", everythingseeder, E_Seeder_KB, "ALL_SEEDER_NOTE"), { 
			Hotkey(Key::L, "ALL_SEEDER_KEY1") 
		})),
		SEEDC->Append(SEED1C);
	////////////////////////
	/*Drop Codes SubFolder*/
	////////////////////////
		MenuFolder *DROPC = new MenuFolder(FolderColors[5] << "DROP_CODES");
		DROPC->Append(new MenuEntry(FolderColors[5] << "ITEM_SEQUENCER", nullptr, Entry_itemsequence, "ITEM_SEQUENCER_NOTE")),
	    DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "DROP_MODS", dropMod, "DROP_MODS_NOTE"), { 
			Hotkey(Key::A | Key::DPadRight, "DROP_MODS_KEY1"), 
			Hotkey(Key::A | Key::DPadLeft, "DROP_MODS_KEY2"), 
			Hotkey(Key::A | Key::DPadDown, "DROP_MODS_KEY3"), 
			Hotkey(Key::A | Key::DPadUp, "DROP_MODS_KEY4") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "DROP_ITEMS", instantDrop, "DROP_ITEMS_NOTE"), { 
			Hotkey(Key::Y | Key::DPadRight, "DROP_ITEMS_KEY1"), 
			Hotkey(Key::L | Key::DPadDown, "DROP_ITEMS_KEY2") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "AUTO_DROP", autoDrop, "AUTO_DROP_NOTE"), { 
			Hotkey(Key::Y | Key::DPadRight, "AUTO_DROP_KEY1"), 
			Hotkey(Key::Y | Key::DPadLeft, "AUTO_DROP_KEY2") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "TOUCH_DROP", touchDrop, "TOUCH_DROP_NOTE"), { 
			Hotkey(Key::Y | Key::DPadRight, "TOUCH_DROP_KEY1") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "SLOT_DROP", ShowInvSlotID, "SLOT_DROP_NOTE"), { 
			Hotkey(Key::R | Key::Y, "SLOT_DROP_KEY1"), 
			Hotkey(Key::R | Key::X, "SLOT_DROP_KEY2") 
		})),
		SEEDC->Append(DROPC);
	////////////////////////
	/*Tree Codes SubFolder*/
	////////////////////////
		MenuFolder *TREEC = new MenuFolder(FolderColors[5] << "TREE_CODES");
		TREEC->Append(new MenuEntry(FolderColors[5] << "INF_FRUIT_TREE", fruitStays, "INF_FRUIT_TREE_NOTE")),
		TREEC->Append(new MenuEntry(FolderColors[5] << "AXE_TREE_SHAKE", shakechop, "AXE_TREE_SHAKE_NOTE")),
		TREEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "FRUIT_TREE_MOD", fruititemmod, "FRUIT_TREE_MOD_NOTE"), { 
			Hotkey(Key::A | Key::DPadLeft, "FRUIT_TREE_MOD_KEY1") 
		})),
		TREEC->Append(new MenuEntry(FolderColors[5] << "INST_TREE_CHOP", instantchop, "INST_TREE_CHOP_NOTE")),
		SEEDC->Append(TREEC);
		menu->Append(SEEDC);

	//////////////////////
	/*Money Codes Folder*/
	//////////////////////
		MenuFolder *MONC = new MenuFolder(FolderColors[6] << "MONEY_CODES");
		MONC->Append(new MenuEntry(FolderColors[6] << "WALLET", nullptr, wallet, "WALLET_NOTE")),
		MONC->Append(new MenuEntry(FolderColors[6] << "BANK", nullptr, bank, "BANK_NOTE")),
		MONC->Append(new MenuEntry(FolderColors[6] << "MEOW_COUPONS", nullptr, coupon, "MEOW_COUPONS_NOTE")),
		MONC->Append(new MenuEntry(FolderColors[6] << "BADGES", nullptr, badges, "BADGES_NOTE")),
		MONC->Append(new MenuEntry(FolderColors[6] << "MEDALS", nullptr, medals, "MEDALS_NOTE")),
		MONC->Append(new MenuEntry(FolderColors[6] << "TURNIPS", nullptr, turnips, "TURNIPS_NOTE")),
		menu->Append(MONC);

	///////////////////////
	/*Island Codes Folder*/
	///////////////////////
		MenuFolder *ISLC = new MenuFolder(FolderColors[7] << "ISLAND_CODES");
		ISLC->Append(new MenuEntry(FolderColors[7] << "UNLOCK_ISLAND", nullptr, UnlockIsland, "UNLOCK_ISLAND_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "FILL_INV_ORE", bonusOre, "FILL_INV_ORE_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "FILL_INV_FRUIT", instantFruit, "FILL_INV_FRUIT_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "HACKER_ISLAND", nullptr, Hackerisland, "HACKER_ISLAND_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "ISLAND_COUNTRY", nullptr, Countryspoof, "ISLAND_COUNTRY_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "FREE_KAPPN", FreeKappn, "FREE_KAPPN_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "ISLAND_SHOP_MOD", IslandShop, IslandSettings, "ISLAND_SHOP_MOD_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "ALL_TOURS", alltour, "ALL_TOURS_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "ISLAND_ACRE", acreMod, menuAcreMod, "ISLAND_ACRE_NOTE")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "ISLAND_BUILDING", buildingMod, menuBuildingMod, "ISLAND_BUILDING_NOTE")),
		menu->Append(ISLC);

	//////////////
	/*NPC Folder*/
	//////////////
		MenuFolder *NPCC = new MenuFolder(FolderColors[8] << "NPC_CODES");
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC_SELECTOR", NPCFunction, "NPC_SELECTOR_NOTE"), { 
			Hotkey(Key::L | Key::A, "NPC_SELECTOR_KEY1") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC_ANIMATION",  NPCAnimation, NPCSetAnim, "NPC_ANIMATION_NOTE"), { 
			Hotkey(Key::L | Key::B, "NPC_ANIMATION_KEY1") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC_COORDINATE", NPCCoordinates, "NPC_COORDINATE_NOTE"), { 
			Hotkey(Key::L, "NPC_COORDINATE_KEY1"),
			Hotkey(Key::DPadRight, "NPC_COORDINATE_KEY2"), 
			Hotkey(Key::DPadLeft, "NPC_COORDINATE_KEY3"), 
			Hotkey(Key::DPadDown, "NPC_COORDINATE_KEY4"), 
			Hotkey(Key::DPadUp, "NPC_COORDINATE_KEY5")
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC_TELEPORT", NPCTeleportToYou, "NPC_TELEPORT_NOTE"), { 
			Hotkey(Key::L | Key::Y, "NPC_TELEPORT_KEY1") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC_ROTATION", NPCRotate, "NPC_ROTATION_NOTE"), { 
			Hotkey(Key::L, "NPC_ROTATION_KEY1") 
		})),
		menu->Append(NPCC);

	////////////////////
	/*Fun Codes Folder*/
	////////////////////
		MenuFolder *FUNC = new MenuFolder(FolderColors[9] << "FUN_CODES"); 
		FUNC->Append(new MenuEntry(FolderColors[9] << "SIZE_CODES", nullptr, sizecodes, "SIZE_CODES_NOTE")),
		FUNC->Append(new MenuEntry(FolderColors[9] << "T_POSE", tposeentry, "T_POSE_NOTE")),
		FUNC->Append(EntryWithHotkey(new MenuEntry(FolderColors[9] << "TAKE_TPC_PIC", freezeframe, "TAKE_TPC_PIC_NOTE"), { 
			Hotkey(Key::L, "TAKE_TPC_PIC_KEY1"), 
			Hotkey(Key::A, "TAKE_TPC_PIC_KEY2") 
		})),
		FUNC->Append(new MenuEntry(FolderColors[9] << "MAX_TURBO", maxturbo, "MAX_TURBO_NOTE")),
		FUNC->Append(new MenuEntry(FolderColors[9] << "MULTI_PRESS", asmpresses, "MULTI_PRESS_NOTE")),
		FUNC->Append(EntryWithHotkey(new MenuEntry(FolderColors[9] << "ULTIMATE_POPPER", partypopper, "ULTIMATE_POPPER_NOTE"), { 
			Hotkey(Key::B | Key::DPadLeft, "ULTIMATE_POPPER_KEY1") 
		})),
	    FUNC->Append(new MenuEntry(FolderColors[9] << "CAMERA_MOD", cameramod, "CAMERA_MOD_NOTE")),
		menu->Append(FUNC);

	//////////////////////
	/*Extra Codes Folder*/
	//////////////////////
		MenuFolder *EXTC = new MenuFolder(FolderColors[10] << "EXTRA_CODES");
	/////////////////////
	/*Fish Codes Folder*/
	/////////////////////
		MenuFolder *FISC = new MenuFolder(FolderColors[10] << "FISH_CODES");
		FISC->Append(new MenuEntry(FolderColors[10] << "FISH_ALWAYS_BITE_NAME", FishAlwaysBiteRightAway, "FISH_ALWAYS_BITE_NOTE")),
		FISC->Append(new MenuEntry(FolderColors[10] << "FISH_CANT_SCARE_NAME", FishCantBeScared, "FISH_CANT_SCARE_NOTE")),
		EXTC->Append(FISC);
	/////////////////////
	/*Chat Codes Folder*/
	/////////////////////
		MenuFolder *CHAC = new MenuFolder(FolderColors[10] << "CHAT_CODES");
		CHAC->Append(new MenuEntry(FolderColors[10] << "CHAT_DONT_DISSAPEAR", bubblesDisappear, "CHAT_DONT_DISSAPEAR_NOTE")),
		CHAC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "CHATCOPYPASTE", ChatCopyPaste, "CHATCOPYPASTE_NOTE"), {
			Hotkey(Key::L | Key::DPadRight, "CHATCOPYPASTE_KEY1"),
			Hotkey(Key::L | Key::DPadUp, "CHATCOPYPASTE_KEY2"),
			Hotkey(Key::L | Key::DPadDown, "CHATCOPYPASTE_KEY3"),
			Hotkey(Key::L | Key::DPadLeft, "CHATCOPYPASTE_KEY4")
		})),
		CHAC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "FORCE_CHAT", Forcesendchat, "FORCE_CHAT_NOTE"), { 
			Hotkey(Key::R, "FORCE_CHAT") 
		})),
		EXTC->Append(CHAC);
		EXTC->Append(new MenuEntry(FolderColors[10] << "SHOP_ALWAYS_OPEN_NAME", ShopsAlwaysOpen, "SHOP_ALWAYS_OPEN_NOTE")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "DISABLE_SAVE", nonesave, "DISABLE_SAVE_NOTE")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "DISABLE_ITEM_LOCKS", bypass, "DISABLE_ITEM_LOCKS_NOTE")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "CANT_FALL_HOLE", noTrap, "CANT_FALL_HOLE_NOTE")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "SET_SPOT_STATE_NAME", nullptr, SetSpotState, "SET_SPOT_STATE_NOTE")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "SEARCH_REPLACE_NAME", nullptr, SearchReplace, "SEARCH_REPLACE_NOTE")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "REMOVE_TOWNITEMS_NAME", nullptr, RemoveItemsCheat, "REMOVE_TOWNITEMS_NOTE")),
	    EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "WATER_FLOWERS_NAME", WaterAllFlowers, "WATER_FLOWERS_NOTE"), { 
			Hotkey(Key::R | Key::DPadLeft, "WATER_FLOWRES_HOTKEY1") 
		})),
		EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "WEED_REMOVER_NAME", weedremover, "WEED_REMOVER_NOTE"), { 
			Hotkey(Key::L | Key::DPadRight, "WEED_REMOVER_HOTKEY1"), 
			Hotkey(Key::L | Key::DPadLeft, "WEED_REMOVER_HOTKEY2") 
		})),
		EXTC->Append(new MenuEntry(FolderColors[10] << "EDIT_PATTERN_NAME", editpattern, "EDIT_PATTERN_NOTE")),
		EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "GRASS_EDITOR", grasseditor, grasscomplete, "GRASS_EDITOR_NOTE"), { 
			Hotkey(Key::R | Key::DPadDown, "GRASS_EDITOR_HOTKEY1"), 
			Hotkey(Key::R | Key::DPadUp, "GRASS_EDITOR_HOTKEY2"), 
			Hotkey(Key::R | Key::DPadRight, "GRASS_EDITOR_HOTKEY3") 
		})),
		EXTC->Append(new MenuEntry(FolderColors[10] << "AMIIBO_SPOOFER", AmiiboSpoofer, "AMIIBO_SPOOFER_NOTE")),
		EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "TIME_TRAVEL", TimeTravel, TTKeyboard, "TIME_TRAVEL_NOTE"), { 
			Hotkey(Key::R | Key::DPadRight, "TIME_FORWARD"), 
			Hotkey(Key::R | Key::DPadLeft, "TIME_BACKWARDS") 
		})),
		menu->Append(EXTC);

	/////////////////////
	/*Misc Codes Folder*/
	/////////////////////
		MenuFolder *MISC = new MenuFolder(FolderColors[11] << "MISC_CODES");		
		MISC->Append(new MenuEntry(FolderColors[11] << "TOOL_ANIM", nullptr, tooltype, "TOOL_ANIM_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "GAME_TYPE", nullptr, mgtype, "GAME_TYPE_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "UNBREAK_FLOWER", unbreakableflower, "UNBREAK_FLOWER_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "WEATHER_MOD", nullptr, Weathermod , "WEATHER_MOD_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "ALWAYS_AURORA_MOD", auroralights , "ALWAYS_AURORA_MOD_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "DISABLECOMMAND", nullptr, disablecommands, "DISABLECOMMAND_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "RELOAD_ROOM_NAME", nullptr, ReloadRoomCheat, "RELOAD_ROOM_NOTE")),
		MISC->Append(EntryWithHotkey(new MenuEntry(FolderColors[11] << "QUICK_MENU", QuickMenuEntry, "QUICK_MENU_NOTE"), {
			Hotkey(Key::Y | Key::DPadDown, "QUICK_MENU")
		})),
		MISC->Append(new MenuEntry(FolderColors[11] << "MORE_NUMBERS",  morenumberisland, "MORE_NUMBERS_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "LARGE_FOV", fovlarge, "LARGE_FOV_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "MOVE_FURN", roomSeeder, "MOVE_FURN_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "WALK_TALK", walktalkentry, "WALK_TALK_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "KEY_EXTEND", key_limit, "KEY_EXTEND_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "CUSTOM_KEY", CustomKeyboard, "CUSTOM_KEY_NOTE")),
		MISC->Append(EntryWithHotkey(new MenuEntry(FolderColors[11] << "BEANS_PARTICLE", BeansParticleChanger, "BEANS_PARTICLE_NOTE"), { 
			Hotkey(Key::L | Key::DPadLeft, "BEANS_PARTICLE") 
		})),
		MISC->Append(new MenuEntry(FolderColors[11] << "DAYTIME", Daytime, "DAYTIME_NOTE")),    
		MISC->Append(EntryWithHotkey(new MenuEntry(FolderColors[11] << "FAST_MODE", fast, "FAST_MODE_NOTE"), { 
			Hotkey(Key::R | Key::DPadDown, "FAST_MODE")
		})),
		MISC->Append(new MenuEntry(FolderColors[11] << "FAST_TEXT_SPEED", fasttalk, "FAST_TEXT_SPEED_NOTE")),
		MISC->Append(new MenuEntry(FolderColors[11] << "FAST_GAME_SPEED", speedentry, "FAST_GAME_SPEED_NOTE")),
		menu->Append(MISC);

	////////////////////
	/*Dev Codes Folder*/
	////////////////////
		DEVC = new MenuFolder(Color(0xFF1A69FF) << "Dev Codes");
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Custom Dump", nullptr, customdump, "Lets you dump/restore custom dumps.")),
		DEVC->Append(EntryWithHotkey(new MenuEntry(Color(0xFF1A69FF) << "Call Function", FunctionsCaller, FunctionsCallerSettings, "Lets you call functions"), { 
			Hotkey(Key::R | Key::DPadRight, "Open Menu"), 
			Hotkey(Key::R | Key::DPadDown, "Execute Function") 
		})),
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
																			  FormatColor("%cBlue Wand: %cNULL\n", Color(0x0089FFFF), Color::White) +
																			  FormatColor("%cGreen Wand: %cRestores wilted flowers instantly\n", Color(0x2BFF00FF), Color::White) +
																			  FormatColor("%cPink Wand: %cNULL\n", Color(0xFF00E6FF), Color::White) +
																			  FormatColor("%cYellow Wand: %cNULL\n", Color(0xEFFF00FF), Color::White) +
																			  FormatColor("%cFlower Fairy Wand: %cGrows Tree instantly\n", Color(0xCD00FFFF), Color::White) +
																			  FormatColor("%cKiki and Lala Wand: %cNULL", Color(0xFF7700FF), Color::White))),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Light Switch Toggler", lightswitch, "Lets you toggle the light switch.\nHotkeys are:\nL + DPadUp : Toggle light in current room\nL + DPadRight : Set Room ID\nL + DPadLeft : Toggle light in set room")),
		//DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Custom Save Settings", CustomSaveScreen, "")),
		DEVC->Append(new MenuEntry(Color(0xFF1A69FF) << "Island Saver", nullptr, IslandSaver, "")),
		menu->Append(DEVC);

	/*Other Cheats*/
		menu->Append(new MenuEntry("Plugin Settings", nullptr, pluginsettings)),
		//menu->Append(new MenuEntry("Game Settings", nullptr, gamesettings)),
		menu->Append(new MenuEntry("Random Folder Colors", rainbow, "This will randomize the cheat folders colors.\nNote: If you randomize the color and reset the plugin, the color will not be the randomized one, it will instead stay at the color option you selected!"));
	
		SetUp(SAVEC);
		SetUp(MOVEC);
		SetUp(INVC);
		SetUp(PSAVEC, true);
		SetUp(PLAYC);
		SetUp(ANIMC);
		SetUp(SEED1C, true);
		SetUp(DROPC, true);
		SetUp(TREEC, true);
		SetUp(SEEDC);
		SetUp(MONC);
		SetUp(ISLC);
		SetUp(NPCC);
		SetUp(FUNC);
		SetUp(FISC, true);
		SetUp(CHAC, true);
		SetUp(EXTC);
		SetUp(MISC);
	}
}
