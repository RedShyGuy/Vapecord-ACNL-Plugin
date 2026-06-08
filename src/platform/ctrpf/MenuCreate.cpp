#include "features/cheats.hpp"
#include "platform/ctrpf/MenuEntryIds.hpp"
#include "platform/ctrpf/UtilsExtras.hpp"
#include "core/infrastructure/Language.hpp"
#include "platform/ctrpf/Plugin_Color.hpp"
#include "platform/ctrpf/FolderTypes.hpp"
#include "platform/ctrpf/FolderTypeTextMap.hpp"
#include "core/Pretendo/PIALogger.hpp"
#include "Files.h"

#include <unordered_map>

namespace CTRPluginFramework {
	struct NamedHotkey {
		Hotkey hotkey;
		TextID nameKey;

		NamedHotkey(u32 keys, const TextID &nameKey)
			: hotkey(keys, UtilsExtras::setLanguageByKey(nameKey)), nameKey(nameKey)
		{
		}
	};

	struct FolderInfo {
		FolderType folderType;
		SubFolder subFolder;
		std::string customNote;
	};

	static std::unordered_map<MenuFolder *, FolderInfo> g_folderInfos;

	static std::string GetFolderName(FolderType type) {
		return Language::getInstance()->get(GetFolderTextId(type));
	}

	static std::string GetSubFolderName(FolderType parent, SubFolder sub) {
		return Language::getInstance()->get(GetSubFolderTextId(parent, sub));
	}

	static std::string GetFolderNote(FolderType type) {
		TextID noteKey = GetFolderNoteTextId(type);
		return noteKey != TextID::NONE ? Language::getInstance()->get(noteKey) : "";
	}

	static std::string GetSubFolderNote(FolderType parent, SubFolder sub) {
		TextID noteKey = GetSubFolderNoteTextId(parent, sub);
		return noteKey != TextID::NONE ? Language::getInstance()->get(noteKey) : "";
	}

	static std::string ResolveFolderNote(FolderType folderType, SubFolder subFolder, const std::string &customNote = "") {
		if (!customNote.empty())
			return customNote;

		return subFolder != SubFolder::None ? GetSubFolderNote(folderType, subFolder) : GetFolderNote(folderType);
	}

	static Color GetFolderColor(FolderType type) {
		if (type >= FolderType::Save && type <= FolderType::Misc) {
			if (CustomColorsExist()) {
				std::vector<ColorEntry> customColors = GetCustomColors();
				for (const ColorEntry& entry : customColors) {
					if (entry.folderType == type)
						return Color(entry.r, entry.g, entry.b);
				}
			}
		}

		for (const ColorEntry& entry : defaultColors) {
			if (entry.folderType == type)
				return Color(entry.r, entry.g, entry.b);
		}

		return Color::White;
	}

	static MenuFolder *CreateFolder(FolderType folderType, SubFolder subFolder = SubFolder::None, const std::string &note = "") {
		MenuFolder *folder = new MenuFolder(GetFolderColor(folderType) << (subFolder != SubFolder::None ? GetSubFolderName(folderType, subFolder) : GetFolderName(folderType)), ResolveFolderNote(folderType, subFolder, note));
		folder->SetColor(GetFolderColor(folderType));
		g_folderInfos[folder] = { folderType, subFolder, note };
		return folder;
	}

	bool TryGetFolderInfo(MenuFolder *folder, FolderType &folderType, SubFolder &subFolder) {
		auto it = g_folderInfos.find(folder);
		if (it == g_folderInfos.end())
			return false;

		folderType = it->second.folderType;
		subFolder = it->second.subFolder;
		return true;
	}

	static std::string ResolveTextByKey(u32 key) {
		return Language::getInstance()->get(static_cast<TextID>(key));
	}

	static MenuEntry *CreateEntry(const TextID &nameKey, void(*gameFunc)(MenuEntry*), void(*menuFunc)(MenuEntry*), const TextID &noteKey = TextID::NONE, unsigned int uuID = 0) {
		MenuEntry *entry = new MenuEntry(
			UtilsExtras::setLanguageByKey(nameKey),
			gameFunc,
			menuFunc,
			noteKey != TextID::NONE ? UtilsExtras::setLanguageByKey(noteKey) : "",
			uuID
		);

		entry->SetNameKey(static_cast<u32>(nameKey));
		entry->SetNoteKey(noteKey != TextID::NONE ? static_cast<u32>(noteKey) : 0);
		return entry;
	}

	static MenuEntry *CreateEntry(const TextID &nameKey, void(*gameFunc)(MenuEntry*), const TextID &noteKey = TextID::NONE, unsigned int uuID = 0) {
		MenuEntry *entry = new MenuEntry(
			UtilsExtras::setLanguageByKey(nameKey),
			gameFunc,
			noteKey != TextID::NONE ? UtilsExtras::setLanguageByKey(noteKey) : "",
			uuID
		);

		entry->SetNameKey(static_cast<u32>(nameKey));
		entry->SetNoteKey(noteKey != TextID::NONE ? static_cast<u32>(noteKey) : 0);
		return entry;
	}

	static MenuEntry *EntryWithHotkey(MenuEntry *entry, const std::vector<NamedHotkey> &hotkeys) {
        if(entry != nullptr) {
			for(const NamedHotkey &hotkey : hotkeys) {
				entry->Hotkeys += hotkey.hotkey;
				entry->AppendHotkeyNameKey(static_cast<u32>(hotkey.nameKey));
			}
        }
        return entry;
    }

	static MenuEntry *EntryWithHotkey(MenuEntry *entry, const NamedHotkey &hotkey) {
        if(entry != nullptr) {
			entry->Hotkeys += hotkey.hotkey;
			entry->AppendHotkeyNameKey(static_cast<u32>(hotkey.nameKey));
        }
        return entry;
    }

/*This will load all the folders and entrys*/
    void InitMenu(PluginMenu *menu) {
		MenuEntry::SetLocalizationCallback(ResolveTextByKey);
		PluginMenu::SetFolderRefreshCallback([](MenuFolder *folder) {
			auto it = g_folderInfos.find(folder);
			if (it == g_folderInfos.end())
				return;

			const FolderInfo &info = it->second;
			folder->SetColor(GetFolderColor(info.folderType));
			folder->SetName(GetFolderColor(info.folderType) << (info.subFolder != SubFolder::None ? GetSubFolderName(info.folderType, info.subFolder) : GetFolderName(info.folderType)));
			folder->Note() = ResolveFolderNote(info.folderType, info.subFolder, info.customNote);
		});
	/////////////////////
	/*Save Codes Folder*/
	/////////////////////
		MenuFolder *SAVEC = CreateFolder(FolderType::Save);
	    SAVEC->Append(CreateEntry(TextID::TOWN_NAME_CHANGER, nullptr, townnamechanger, TextID::TOWN_NAME_CHANGER_NOTE, MenuEntryId::SaveTownNameChanger)),
		SAVEC->Append(CreateEntry(TextID::SAVE_BACKUP_NAME, nullptr, savebackup, TextID::SAVE_BACKUP_NOTE, MenuEntryId::SaveBackup)),
		SAVEC->Append(CreateEntry(TextID::BULL_BOARD_DUMPER, nullptr, bullboard, TextID::BULL_BOARD_DUMPER_NOTE, MenuEntryId::SaveBullBoardDumper)),
		SAVEC->Append(CreateEntry(TextID::TREESIZE_NAME, nullptr, TreeSizeChanger, TextID::TREESIZE_NOTE, MenuEntryId::SaveTreeSizeChanger)),
		SAVEC->Append(CreateEntry(TextID::CHANGE_NATIVE_NAME, nullptr, ChangeNativeFruit, TextID::CHANGE_NATIVE_NOTE, MenuEntryId::SaveChangeNativeFruit)),
		SAVEC->Append(CreateEntry(TextID::PWP_UNLOCK_NAME, nullptr, PWPUnlock, TextID::PWP_UNLOCK_NOTE, MenuEntryId::SavePwpUnlock)),
		SAVEC->Append(CreateEntry(TextID::GRASS_CHANGER_NAME, nullptr, GrassChanger, TextID::GRASS_CHANGER_NOTE, MenuEntryId::SaveGrassChanger)),
		SAVEC->Append(CreateEntry(TextID::CARAVAN_SET, nullptr, caravanchange, TextID::CARAVAN_SET_NOTE, MenuEntryId::SaveCaravanSet)),
		SAVEC->Append(CreateEntry(TextID::CAMPING_SET, nullptr, SetCampingVillager, TextID::CAMPING_SET_NOTE, MenuEntryId::SaveCampingSet)),
		SAVEC->Append(CreateEntry(TextID::SHOP_UPGRADE, nullptr, shopunlocks, TextID::SHOP_UPGRADE_NOTE, MenuEntryId::SaveShopUpgrade)),
		SAVEC->Append(CreateEntry(TextID::HOUSE_EDITOR_NAME, nullptr, HouseChanger, TextID::HOUSE_EDITOR_NOTE, MenuEntryId::SaveHouseEditor)),
		SAVEC->Append(CreateEntry(TextID::BUILDING_MOD_NAME, nullptr, BuildingMod, TextID::BUILDING_MOD_NOTE, MenuEntryId::SaveBuildingMod)),
		SAVEC->Append(CreateEntry(TextID::FILL_MUSEUM_NAME, nullptr, CompleteMuseum, TextID::FILL_MUSEUM_NOTE, MenuEntryId::SaveFillMuseum)),
		SAVEC->Append(CreateEntry(TextID::MAYOR_PERMIT_NAME, nullptr, Permit100, TextID::MAYOR_PERMIT_NOTE, MenuEntryId::SaveMayorPermit)),
		SAVEC->Append(CreateEntry(TextID::REAL_TIME_ACRE, MapEditor, TextID::REAL_TIME_ACRE_NOTE, MenuEntryId::SaveRealTimeAcre)),
		menu->Append(SAVEC);

	/////////////////////////
	/*Movement Codes Folder*/
	/////////////////////////
		MenuFolder *MOVEC = CreateFolder(FolderType::Movement);
		MOVEC->Append(CreateEntry(TextID::CANT_PUSH, noPush, TextID::CANT_PUSH_NOTE, MenuEntryId::MovementCantPush));
		MenuEntry *coordModEntry = EntryWithHotkey(CreateEntry(TextID::COORD_MOD, coordinate, coordspeed, TextID::COORD_MOD_NOTE, MenuEntryId::MovementCoordMod), {
			NamedHotkey(Key::A | Key::DPadRight, TextID::COORD_MOD_KEY2),
			NamedHotkey(Key::A | Key::DPadLeft, TextID::COORD_MOD_KEY3),
			NamedHotkey(Key::A | Key::DPadDown, TextID::COORD_MOD_KEY4),
			NamedHotkey(Key::A | Key::DPadUp, TextID::COORD_MOD_KEY5)
		});
		coordModEntry->SetSavedValueApplyCallback(CoordSpeedApplySaved);
		MOVEC->Append(coordModEntry);

		if (System::IsNew3DS()) {
			MenuEntry *cStickCoordModEntry = CreateEntry(TextID::CSTICK_COORD_MOD, cStickCoordinate, coordspeed, TextID::CSTICK_COORD_MOD_NOTE, MenuEntryId::MovementCstickCoordMod);
			cStickCoordModEntry->SetSavedValueApplyCallback(CoordSpeedApplySaved);
			MOVEC->Append(cStickCoordModEntry);
		}

		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::MOON_JUMP, moonjump, TextID::MOON_JUMP_NOTE, MenuEntryId::MovementMoonJump), {
			NamedHotkey(Key::L | Key::DPadUp, TextID::MOON_JUMP_KEY1),
			NamedHotkey(Key::L | Key::DPadDown, TextID::MOON_JUMP_KEY2)
		}));
		MOVEC->Append(CreateEntry(TextID::TOUCH_WARP, tch_warp, TextID::TOUCH_WARP_NOTE, MenuEntryId::MovementTouchWarp));
		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::WALK_OVER, walkOver, TextID::WALK_OVER_NOTE, MenuEntryId::MovementWalkOver), {
			NamedHotkey(Key::L | Key::DPadUp, TextID::WALK_OVER)
		}));
		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::MOVEMENT_CHANGE, MovementChanger, TextID::MOVEMENT_CHANGE_NOTE, MenuEntryId::MovementChange), {
			NamedHotkey(Key::L | Key::B, TextID::MOVEMENT_CHANGE)
		}));
		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::WALK_PARTICLE_CHANGE, Walkparticle, TextID::WALK_PARTICLE_CHANGE_NOTE, MenuEntryId::MovementWalkParticleChange), {
			NamedHotkey(Key::L | Key::X, TextID::WALK_PARTICLE_CHANGE)
		}));
		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::PLAYER_TELEPORT, stalk, TextID::PLAYER_TELEPORT_NOTE, MenuEntryId::MovementPlayerTeleport), {
			NamedHotkey(Key::R | Key::DPadLeft, TextID::PLAYER_TELEPORT_KEY1),
			NamedHotkey(Key::R | Key::DPadRight, TextID::PLAYER_TELEPORT_KEY2)
		}));
		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::VISIBILITY_MOD, onlineplayermod, TextID::VISIBILITY_MOD_NOTE, MenuEntryId::MovementVisibilityMod), {
			NamedHotkey(Key::L | Key::A, TextID::VISIBILITY_MOD_KEY1)
		}));
		MenuEntry *speedModEntry = CreateEntry(TextID::SPEED_MOD, speedMod, menuSpeedMod, TextID::SPEED_MOD_NOTE, MenuEntryId::MovementSpeedMod);
		speedModEntry->SetSavedValueApplyCallback(SpeedModApplySaved);
		MOVEC->Append(speedModEntry);
		MOVEC->Append(EntryWithHotkey(CreateEntry(TextID::ROOM_WARPING, roomWarp, TextID::ROOM_WARPING_NOTE, MenuEntryId::MovementRoomWarping), {
			NamedHotkey(Key::L | Key::X, TextID::ROOM_WARPING)
		}));
		MOVEC->Append(CreateEntry(TextID::SHOVEL_KNOCKBACK, shovelknockback, TextID::SHOVEL_KNOCKBACK_NOTE, MenuEntryId::MovementShovelKnockback));
		menu->Append(MOVEC);

	//////////////////////////
	/*Inventory Codes Folder*/
	//////////////////////////
		MenuFolder *INVC = CreateFolder(FolderType::Inventory);
		INVC->Append(EntryWithHotkey(CreateEntry(TextID::TEXT_2_ITEM, t2i, TextID::TEXT_2_ITEM_NOTE, MenuEntryId::InventoryText2Item), {
			NamedHotkey(Key::X | Key::DPadRight, TextID::TEXT_2_ITEM_KEY1),
			NamedHotkey(Key::X | Key::DPadDown, TextID::TEXT_2_ITEM_KEY3),
		}));
		INVC->Append(EntryWithHotkey(CreateEntry(TextID::DUPE_ITEMS, duplication, TextID::DUPE_ITEMS_NOTE, MenuEntryId::InventoryDupeItems), {
			NamedHotkey(Key::R, TextID::DUPE_ITEMS_KEY1),
			NamedHotkey(Key::R | Key::Y, TextID::DUPE_ITEMS_KEY2)
		}));
		INVC->Append(EntryWithHotkey(CreateEntry(TextID::CATALOG_TO_POCKET, catalog, TextID::CATALOG_TO_POCKET_NOTE, MenuEntryId::InventoryCatalogToPocket), {
			NamedHotkey(Key::L | Key::DPadRight, TextID::CATALOG_TO_POCKET_KEY1)
		}));
		INVC->Append(EntryWithHotkey(CreateEntry(TextID::CHAT_T2I, chatt2i, TextID::CHAT_T2I_NOTE, MenuEntryId::InventoryChatT2I), {
			NamedHotkey(Key::R | Key::DPadLeft, TextID::CHAT_T2I_KEY1)
		}));
		INVC->Append(CreateEntry(TextID::SEARCH_ITEM, nullptr, itemsearch, TextID::SEARCH_ITEM_NOTE, MenuEntryId::InventorySearchItem));
		INVC->Append(CreateEntry(TextID::CLEAR_INV_NAME, nullptr, ClearInventory, TextID::CLEAR_INV_NOTE, MenuEntryId::InventoryClearInventory));
		MenuEntry *itemSettingsEntry = CreateEntry(TextID::ITEM_SETTINGS, nullptr, itemsettings, TextID::ITEM_SETTINGS_NOTE, MenuEntryId::InventoryItemSettings);
		itemSettingsEntry->SetSavedValueApplyCallback(ItemSettingsApplySaved);
		INVC->Append(itemSettingsEntry);
		MenuEntry *saveMenuChanger = CreateEntry(TextID::SAVE_MENU_CHANGER, nullptr, MenuChanger, TextID::SAVE_MENU_CHANGER_NOTE, MenuEntryId::InventorySaveMenuChanger);
		saveMenuChanger->SetSavedValueApplyCallback(MenuChangerApplySaved);
		INVC->Append(saveMenuChanger);
		INVC->Append(CreateEntry(TextID::GET_SET, nullptr, getset, TextID::GET_SET_NOTE, MenuEntryId::InventoryGetSet));
		MenuEntry *customButtonEntry = CreateEntry(TextID::CUSTOM_BUTTON, nullptr, SettingsButton, TextID::CUSTOM_BUTTON_NOTE, MenuEntryId::InventoryCustomButton);
		customButtonEntry->SetSavedValueApplyCallback(CustomButtonApplySaved);
		INVC->Append(customButtonEntry);
		INVC->Append(CreateEntry(TextID::HIDE_DESIGN_OUTFITS, HideDesignOutfits, TextID::HIDE_DESIGN_OUTFITS_NOTE, MenuEntryId::InventoryHideDesignOutfits));
		menu->Append(INVC);

	///////////////////////
	/*Player Codes Folder*/
	///////////////////////
		MenuFolder *PLAYC = CreateFolder(FolderType::Player);
	////////////////////////////
	/*Player Save Codes Folder*/
	////////////////////////////
		MenuFolder *PSAVEC = CreateFolder(FolderType::Player, SubFolder::PlayerSave);
		PSAVEC->Append(CreateEntry(TextID::NAME_CHANGER, nullptr, NameChanger, TextID::NAME_CHANGER_NOTE, MenuEntryId::PlayerSaveNameChanger));
		PSAVEC->Append(CreateEntry(TextID::PLAYER_APPEARANCE, nullptr, playermod, TextID::PLAYER_APPEARANCE_NOTE, MenuEntryId::PlayerSaveAppearance));
		PSAVEC->Append(CreateEntry(TextID::RANDOM_PLAYER, nullptr, randomoutfit, TextID::RANDOM_PLAYER_NOTE, MenuEntryId::PlayerSaveRandomPlayer));
		PSAVEC->Append(CreateEntry(TextID::PLAYER_BACKUP_NAME, nullptr, playerbackup, TextID::PLAYER_BACKUP_NOTE, MenuEntryId::PlayerSaveBackup));
		PSAVEC->Append(CreateEntry(TextID::TPC_MESSAGE, nullptr, tpcmessage, TextID::TPC_MESSAGE_NOTE, MenuEntryId::PlayerSaveTPCMessage));
		PSAVEC->Append(CreateEntry(TextID::TPC_SETTINGS, nullptr, tpc, TextID::TPC_SETTINGS_NOTE, MenuEntryId::PlayerSaveTPCSettings));
		PSAVEC->Append(CreateEntry(TextID::DESIGN_DUMP, nullptr, DesignDumper, TextID::DESIGN_DUMP_NOTE, MenuEntryId::PlayerSaveDesignDump));
		PSAVEC->Append(CreateEntry(TextID::EMOTION_LIST, nullptr, emotelist, TextID::EMOTION_LIST_NOTE, MenuEntryId::PlayerSaveEmotionList));
		PSAVEC->Append(CreateEntry(TextID::ENCY_LIST, nullptr, enzyklopedia, TextID::ENCY_LIST_NOTE, MenuEntryId::PlayerSaveEncyList));
		PSAVEC->Append(CreateEntry(TextID::DREAM_CODE, nullptr, comodifier, TextID::DREAM_CODE_NOTE, MenuEntryId::PlayerSaveDreamCode));
		PSAVEC->Append(CreateEntry(TextID::DEBUG_MENU, nullptr, debug1, TextID::DEBUG_MENU_NOTE, MenuEntryId::PlayerSaveDebugMenu));
		PSAVEC->Append(CreateEntry(TextID::SONG_LIST_NAME, nullptr, FillSongs, TextID::SONG_LIST_NOTE, MenuEntryId::PlayerSaveSongList));
		PSAVEC->Append(CreateEntry(TextID::FILL_CATALOG_NAME, nullptr, FillCatalog, TextID::FILL_CATALOG_NOTE, MenuEntryId::PlayerSaveFillCatalog));
		PSAVEC->Append(CreateEntry(TextID::QR_MACHINE_NAME, nullptr, unlockqrmachine, TextID::QR_MACHINE_NOTE, MenuEntryId::PlayerSaveQRMachine));
		PLAYC->Append(PSAVEC);
		PLAYC->Append(CreateEntry(TextID::PLAYER_INFO, debug, TextID::PLAYER_INFO_NOTE, MenuEntryId::PlayerInfo));
	//PLAYC->Append(CreateEntry(TextID::PLAYER_LOADER, nullptr, pLoaderEntry, TextID::PLAYER_LOADER_NOTE)),
		PLAYC->Append(CreateEntry(TextID::EDIT_PATTERN_NAME, nullptr, editpattern, TextID::EDIT_PATTERN_NOTE, MenuEntryId::ExtraEditPattern));
		MenuEntry *customHairEyeEntry = CreateEntry(TextID::CUSTOM_HAIR_EYE, nullptr, SaveColor, TextID::CUSTOM_HAIR_EYE_NOTE, MenuEntryId::PlayerCustomHairEye);
		customHairEyeEntry->SetSavedValueApplyCallback(AppColorModApplySaved);
		PLAYC->Append(customHairEyeEntry);
		PLAYC->Append(CreateEntry(TextID::WEAR_HELMET, hatz, TextID::WEAR_HELMET_NOTE, MenuEntryId::PlayerWearHelmet));
		PLAYC->Append(EntryWithHotkey(CreateEntry(TextID::FAINT, Faint, TextID::FAINT_NOTE, MenuEntryId::PlayerFaint), {
			NamedHotkey(Key::R | Key::A, TextID::FAINT)
		}));
		PLAYC->Append(EntryWithHotkey(CreateEntry(TextID::FISH_THROWER, FishThrower, TextID::FISH_THROWER_NOTE, MenuEntryId::PlayerFishThrower), {
			NamedHotkey(Key::B | Key::DPadLeft, TextID::FISH_THROWER_KEY1),
		}));
		PLAYC->Append(CreateEntry(TextID::PLAYERS_ON_MAP, map, TextID::PLAYERS_ON_MAP_NOTE, MenuEntryId::PlayerOnMap));

		PLAYC->Append(CreateEntry(TextID::NEVER_BEDHEAD, NeverBedHead, TextID::NEVER_BEDHEAD_NOTE, MenuEntryId::PlayerNeverBedhead));
		menu->Append(PLAYC);

	//////////////////////////
	/*Animation Codes Folder*/
	//////////////////////////
		MenuFolder *ANIMC = CreateFolder(FolderType::Animation);
		ANIMC->Append(EntryWithHotkey(CreateEntry(TextID::PLAYER_SELECTOR, playerSelector, TextID::PLAYER_SELECTOR_NOTE, MenuEntryId::AnimationPlayerSelector), {
			NamedHotkey(Key::L | Key::DPadRight, TextID::PLAYER_SELECTOR_KEY1),
			NamedHotkey(Key::L | Key::DPadLeft, TextID::PLAYER_SELECTOR_KEY2)
		})),
		ANIMC->Append(CreateEntry(TextID::ANTI_ANIM, anticheat, TextID::ANTI_ANIM_NOTE, MenuEntryId::AnimationAntiAnim)),
		ANIMC->Append(EntryWithHotkey(CreateEntry(TextID::ANIMATION_MOD, execAnim, TextID::ANIMATION_MOD_NOTE, MenuEntryId::AnimationMod), {
			NamedHotkey(Key::A | Key::DPadRight, TextID::ANIMATIONS_NOTE_HOTKEY1),
			NamedHotkey(Key::A | Key::DPadLeft, TextID::ANIMATIONS_NOTE_HOTKEY2),
			NamedHotkey(Key::A | Key::DPadUp, TextID::ANIMATIONS_NOTE_HOTKEY3),
			NamedHotkey(Key::A | Key::B, TextID::ANIMATIONS_NOTE_HOTKEY4)
		})),
		ANIMC->Append(EntryWithHotkey(CreateEntry(TextID::EMOTION_LOOP, inf_expression, TextID::EMOTION_LOOP_NOTE, MenuEntryId::AnimationEmotionLoop), {
			NamedHotkey(Key::B, TextID::EMOTION_LOOP)
		})),
		ANIMC->Append(EntryWithHotkey(CreateEntry(TextID::IDLE, idle, TextID::IDLE_NOTE, MenuEntryId::AnimationIdle), {
			NamedHotkey(Key::R, TextID::IDLE)
		})),
		ANIMC->Append(EntryWithHotkey(CreateEntry(TextID::ANIM_ON_ALL, doonall, TextID::ANIM_ON_ALL_NOTE, MenuEntryId::AnimationOnAll), {
			NamedHotkey(Key::R | Key::A, TextID::ANIM_ON_ALL_KEY1),
			NamedHotkey(Key::R | Key::B, TextID::ANIM_ON_ALL_KEY2),
			NamedHotkey(Key::R | Key::Y, TextID::ANIM_ON_ALL_KEY3)
		}));
		{
			MenuEntry *toolAnimEntry = CreateEntry(TextID::TOOL_ANIM, nullptr, tooltype, TextID::TOOL_ANIM_NOTE, MenuEntryId::MiscToolAnim);
			toolAnimEntry->SetSavedValueApplyCallback(ToolTypeApplySaved);
			ANIMC->Append(toolAnimEntry);
		}
		menu->Append(ANIMC);

	////////////////////////
	/*Seeding Codes Folder*/
	////////////////////////
		MenuFolder *SEEDC = CreateFolder(FolderType::Seeding);
	////////////////////////
	/*Seed Codes SubFolder*/
	////////////////////////
		MenuFolder *SEED1C = CreateFolder(FolderType::Seeding, SubFolder::Seed);
		SEED1C->Append(EntryWithHotkey(CreateEntry(TextID::PICK_SEEDER, pickseeder, TextID::PICK_SEEDER_NOTE, MenuEntryId::SeedPickSeeder), {
			NamedHotkey(Key::B | Key::DPadLeft, TextID::PICK_SEEDER_KEY1),
			NamedHotkey(Key::B | Key::DPadDown, TextID::PICK_SEEDER_KEY2),
			NamedHotkey(Key::B | Key::DPadUp, TextID::PICK_SEEDER_KEY3),
			NamedHotkey(Key::B | Key::DPadRight, TextID::PICK_SEEDER_KEY4),
			NamedHotkey(Key::B | Key::L, TextID::PICK_SEEDER_KEY5)
		})),
		SEED1C->Append(EntryWithHotkey(CreateEntry(TextID::WALK_SEEDER, Walkseeder, TextID::WALK_SEEDER_NOTE, MenuEntryId::SeedWalkSeeder), {
			NamedHotkey(Key::R | Key::B, TextID::WALK_SEEDER)
		})),
		SEED1C->Append(CreateEntry(TextID::FIREWORK_SEEDER, nullptr, fireworkentry, TextID::FIREWORK_SEEDER_NOTE, MenuEntryId::SeedFireworkSeeder)),
		SEED1C->Append(EntryWithHotkey(CreateEntry(TextID::MAP_EDITOR, tileSelector, TextID::MAP_EDITOR_NOTE, MenuEntryId::SeedMapEditor), {
			NamedHotkey(Key::Start | Key::DPadUp, TextID::MAP_EDITOR_KEY1), //Toggle Editor
			NamedHotkey(Key::DPadRight, TextID::MAP_EDITOR_KEY2),
			NamedHotkey(Key::DPadLeft, TextID::MAP_EDITOR_KEY3),
			NamedHotkey(Key::DPadDown, TextID::MAP_EDITOR_KEY4),
			NamedHotkey(Key::DPadUp, TextID::MAP_EDITOR_KEY5),
			NamedHotkey(Key::R, TextID::MAP_EDITOR_KEY6), //Cycle Size
			NamedHotkey(Key::A, TextID::MAP_EDITOR_KEY7), //Place Item
			NamedHotkey(Key::X, TextID::MAP_EDITOR_KEY8), //Remove Item
			NamedHotkey(Key::Y, TextID::MAP_EDITOR_KEY9), //Copy Item
			NamedHotkey(Key::L, TextID::MAP_EDITOR_KEY10) //Set Item ID
		})),
		SEED1C->Append(EntryWithHotkey(CreateEntry(TextID::ALL_SEEDER, everythingseeder, E_Seeder_KB, TextID::ALL_SEEDER_NOTE, MenuEntryId::SeedAllSeeder), {
			NamedHotkey(Key::L, TextID::ALL_SEEDER_KEY1)
		})),
		SEEDC->Append(SEED1C);
	////////////////////////
	/*Drop Codes SubFolder*/
	////////////////////////
		MenuFolder *DROPC = CreateFolder(FolderType::Seeding, SubFolder::Drop);
		MenuEntry *itemSequencerEntry = CreateEntry(TextID::ITEM_SEQUENCER, nullptr, Entry_itemsequence, TextID::ITEM_SEQUENCER_NOTE, MenuEntryId::DropItemSequencer);
		itemSequencerEntry->SetSavedValueApplyCallback(ItemSequencerApplySaved);
		DROPC->Append(itemSequencerEntry);
	    DROPC->Append(EntryWithHotkey(CreateEntry(TextID::DROP_MODS, dropMod, TextID::DROP_MODS_NOTE, MenuEntryId::DropMods), {
			NamedHotkey(Key::A | Key::DPadRight, TextID::DROP_MODS_KEY1),
			NamedHotkey(Key::A | Key::DPadLeft, TextID::DROP_MODS_KEY2),
			NamedHotkey(Key::A | Key::DPadDown, TextID::DROP_MODS_KEY3),
			NamedHotkey(Key::A | Key::DPadUp, TextID::DROP_MODS_KEY4)
		})),
		DROPC->Append(EntryWithHotkey(CreateEntry(TextID::DROP_ITEMS, instantDrop, TextID::DROP_ITEMS_NOTE, MenuEntryId::DropItems), {
			NamedHotkey(Key::Y | Key::DPadRight, TextID::DROP_ITEMS_KEY1),
			NamedHotkey(Key::L | Key::DPadDown, TextID::DROP_ITEMS_KEY2)
		})),
		DROPC->Append(EntryWithHotkey(CreateEntry(TextID::AUTO_DROP, autoDrop, TextID::AUTO_DROP_NOTE, MenuEntryId::DropAutoDrop), {
			NamedHotkey(Key::Y | Key::DPadRight, TextID::AUTO_DROP_KEY1),
			NamedHotkey(Key::Y | Key::DPadLeft, TextID::AUTO_DROP_KEY2)
		})),
		DROPC->Append(EntryWithHotkey(CreateEntry(TextID::TOUCH_DROP, touchDrop, TextID::TOUCH_DROP_NOTE, MenuEntryId::DropTouchDrop), {
			NamedHotkey(Key::Y | Key::DPadRight, TextID::TOUCH_DROP_KEY1)
		})),
		DROPC->Append(EntryWithHotkey(CreateEntry(TextID::SLOT_DROP, ShowInvSlotID, TextID::SLOT_DROP_NOTE, MenuEntryId::DropSlotDrop), {
			NamedHotkey(Key::R | Key::Y, TextID::SLOT_DROP_KEY1),
			NamedHotkey(Key::R | Key::X, TextID::SLOT_DROP_KEY2)
		})),
		SEEDC->Append(DROPC);
	////////////////////////
	/*Tree Codes SubFolder*/
	////////////////////////
		MenuFolder *TREEC = CreateFolder(FolderType::Seeding, SubFolder::Tree);
		TREEC->Append(CreateEntry(TextID::INF_FRUIT_TREE, fruitStays, TextID::INF_FRUIT_TREE_NOTE, MenuEntryId::TreeInfFruit)),
		TREEC->Append(CreateEntry(TextID::AXE_TREE_SHAKE, shakechop, TextID::AXE_TREE_SHAKE_NOTE, MenuEntryId::TreeAxeShake)),
		TREEC->Append(EntryWithHotkey(CreateEntry(TextID::FRUIT_TREE_MOD, fruititemmod, TextID::FRUIT_TREE_MOD_NOTE, MenuEntryId::TreeFruitMod), {
			NamedHotkey(Key::A | Key::DPadLeft, TextID::FRUIT_TREE_MOD_KEY1)
		})),
		TREEC->Append(CreateEntry(TextID::INST_TREE_CHOP, instantchop, TextID::INST_TREE_CHOP_NOTE, MenuEntryId::TreeInstChop)),
		SEEDC->Append(TREEC),
		menu->Append(SEEDC);

	//////////////////////
	/*Money Codes Folder*/
	//////////////////////
		MenuFolder *MONC = CreateFolder(FolderType::Money);
		MONC->Append(CreateEntry(TextID::WALLET, nullptr, wallet, TextID::WALLET_NOTE, MenuEntryId::MoneyWallet)),
		MONC->Append(CreateEntry(TextID::BANK, nullptr, bank, TextID::BANK_NOTE, MenuEntryId::MoneyBank)),
		MONC->Append(CreateEntry(TextID::MEOW_COUPONS, nullptr, coupon, TextID::MEOW_COUPONS_NOTE, MenuEntryId::MoneyMeowCoupons)),
		MONC->Append(CreateEntry(TextID::BADGES, nullptr, badges, TextID::BADGES_NOTE, MenuEntryId::MoneyBadges)),
		MONC->Append(CreateEntry(TextID::MEDALS, nullptr, medals, TextID::MEDALS_NOTE, MenuEntryId::MoneyMedals)),
		MONC->Append(CreateEntry(TextID::TURNIPS, nullptr, turnips, TextID::TURNIPS_NOTE, MenuEntryId::MoneyTurnips)),
		menu->Append(MONC);

	///////////////////////
	/*Island Codes Folder*/
	///////////////////////
		MenuFolder *ISLC = CreateFolder(FolderType::Island);
		ISLC->Append(CreateEntry(TextID::UNLOCK_ISLAND, nullptr, UnlockIsland, TextID::UNLOCK_ISLAND_NOTE, MenuEntryId::IslandUnlock)),
		ISLC->Append(CreateEntry(TextID::FILL_INV_ORE, bonusOre, TextID::FILL_INV_ORE_NOTE, MenuEntryId::IslandFillInvOre)),
		ISLC->Append(CreateEntry(TextID::FILL_INV_FRUIT, instantFruit, TextID::FILL_INV_FRUIT_NOTE, MenuEntryId::IslandFillInvFruit)),
		ISLC->Append(CreateEntry(TextID::FREE_KAPPN, FreeKappn, TextID::FREE_KAPPN_NOTE, MenuEntryId::IslandFreeKappn)),
		ISLC->Append(CreateEntry(TextID::ISLAND_SHOP_MOD, IslandShop, IslandSettings, TextID::ISLAND_SHOP_MOD_NOTE, MenuEntryId::IslandShopMod)),
		ISLC->Append(CreateEntry(TextID::ALL_TOURS, alltour, TextID::ALL_TOURS_NOTE, MenuEntryId::IslandAllTours)),
		ISLC->Append(CreateEntry(TextID::ISLAND_ACRE, acreMod, menuAcreMod, TextID::ISLAND_ACRE_NOTE, MenuEntryId::IslandAcre)),
		ISLC->Append(CreateEntry(TextID::ISLAND_BUILDING, buildingMod, menuBuildingMod, TextID::ISLAND_BUILDING_NOTE, MenuEntryId::IslandBuilding)),
		ISLC->Append(CreateEntry(TextID::ISLAND_SAVER, nullptr, IslandSaver, TextID::ISLAND_SAVER_NOTE, MenuEntryId::IslandSaver)),
		ISLC->Append(EntryWithHotkey(CreateEntry(TextID::ISLAND_ONLINE_OPTIONS, IslandOnlineOptions, TextID::ISLAND_ONLINE_OPTIONS_NOTE, MenuEntryId::IslandOnlineOptions), {
			NamedHotkey(Key::Y | Key::DPadUp, TextID::ISLAND_ONLINE_OPTIONS)
		}));
		menu->Append(ISLC);

	//////////////
	/*NPC Folder*/
	//////////////
		MenuFolder *NPCC = CreateFolder(FolderType::NPC);
		NPCC->Append(CreateEntry(TextID::NPC_SELECTOR, nullptr, NPCFunction, TextID::NPC_SELECTOR_NOTE, MenuEntryId::NpcSelector));
		MenuEntry *npcAnimEntry = EntryWithHotkey(CreateEntry(TextID::NPC_ANIMATION,  NPCAnimation, NPCSetAnim, TextID::NPC_ANIMATION_NOTE, MenuEntryId::NpcAnimation), {
			NamedHotkey(Key::L | Key::B, TextID::NPC_ANIMATION_KEY1)
		});
		npcAnimEntry->SetSavedValueApplyCallback(NPCSetAnimApplySaved);
		NPCC->Append(npcAnimEntry);
		NPCC->Append(EntryWithHotkey(CreateEntry(TextID::NPC_COORDINATE, NPCCoordinates, TextID::NPC_COORDINATE_NOTE, MenuEntryId::NpcCoordinate), {
			NamedHotkey(Key::L | Key::DPadRight, TextID::NPC_COORDINATE_KEY2),
			NamedHotkey(Key::L | Key::DPadLeft, TextID::NPC_COORDINATE_KEY3),
			NamedHotkey(Key::L | Key::DPadDown, TextID::NPC_COORDINATE_KEY4),
			NamedHotkey(Key::L | Key::DPadUp, TextID::NPC_COORDINATE_KEY5)
		}));
		NPCC->Append(EntryWithHotkey(CreateEntry(TextID::NPC_TELEPORT, NPCTeleportToYou, TextID::NPC_TELEPORT_NOTE, MenuEntryId::NpcTeleport), {
			NamedHotkey(Key::L | Key::Y, TextID::NPC_TELEPORT_KEY1)
		}));
		NPCC->Append(EntryWithHotkey(CreateEntry(TextID::NPC_ROTATION, NPCRotate, TextID::NPC_ROTATION_NOTE, MenuEntryId::NpcRotation), {
			NamedHotkey(Key::L, TextID::NPC_ROTATION_KEY1)
		}));
		menu->Append(NPCC);

	////////////////////////////
	/*Environment Codes Folder*/
	////////////////////////////
		MenuFolder *ENVC = CreateFolder(FolderType::Environment);
	/////////////////////
	/*Fish Codes Folder*/
	/////////////////////
		MenuFolder *FISC = CreateFolder(FolderType::Environment, SubFolder::Fish);
		FISC->Append(CreateEntry(TextID::FISH_ALWAYS_BITE_NAME, FishAlwaysBiteRightAway, TextID::FISH_ALWAYS_BITE_NOTE, MenuEntryId::EnvFishAlwaysBite));
		FISC->Append(CreateEntry(TextID::FISH_CANT_SCARE_NAME, FishCantBeScared, TextID::FISH_CANT_SCARE_NOTE, MenuEntryId::EnvFishCantScare));
		ENVC->Append(FISC);
	///////////////////////
	/*Insect Codes Folder*/
	///////////////////////
		MenuFolder *INSC = CreateFolder(FolderType::Environment, SubFolder::Insect);
		MenuEntry *spawnInsect = CreateEntry(TextID::SPAWN_INSECT, SpawnInsectEntry, SetInsectIdEntry, TextID::SPAWN_INSECT_NOTE, MenuEntryId::EnvInsectSpawn);
		spawnInsect->SetSavedValueApplyCallback(InsectSetIdApplySaved);
		INSC->Append(spawnInsect);
		INSC->Append(CreateEntry(TextID::INSECT_CANT_SCARE, InsectsCantBeScared, TextID::INSECT_CANT_SCARE_NOTE, MenuEntryId::EnvInsectCantScare));
		ENVC->Append(INSC);

		ENVC->Append(CreateEntry(TextID::DAYTIME, Daytime, TextID::DAYTIME_NOTE, MenuEntryId::EnvironmentDaytime));
		ENVC->Append(CreateEntry(TextID::ALWAYS_AURORA_MOD, auroralights , TextID::ALWAYS_AURORA_MOD_NOTE, MenuEntryId::EnvironmentAlwaysAurora));
		ENVC->Append(CreateEntry(TextID::UNBREAK_FLOWER, unbreakableflower, TextID::UNBREAK_FLOWER_NOTE, MenuEntryId::EnvironmentUnbreakFlower));
		{
			MenuEntry *weatherMod = CreateEntry(TextID::WEATHER_MOD, nullptr, Weathermod , TextID::WEATHER_MOD_NOTE, MenuEntryId::EnvironmentWeatherMod);
			weatherMod->SetSavedValueApplyCallback(WeatherModApplySaved);
			ENVC->Append(weatherMod);
		}
		ENVC->Append(CreateEntry(TextID::SEARCH_REPLACE_NAME, nullptr, SearchReplace, TextID::SEARCH_REPLACE_NOTE, MenuEntryId::ExtraSearchReplace)),
		ENVC->Append(CreateEntry(TextID::REMOVE_MAP_ITEMS_NAME, nullptr, RemoveItemsCheat, TextID::REMOVE_MAP_ITEMS_NOTE, MenuEntryId::ExtraRemoveMapItems)),
		ENVC->Append(CreateEntry(TextID::WATER_FLOWERS_NAME, nullptr, WaterAllFlowers, TextID::WATER_FLOWERS_NOTE, MenuEntryId::EnvironmentWaterFlowers));
		ENVC->Append(CreateEntry(TextID::WEED_REMOVER_NAME, nullptr, weedremover, TextID::WEED_REMOVER_NOTE, MenuEntryId::EnvironmentWeedRemover));
		ENVC->Append(EntryWithHotkey(CreateEntry(TextID::GRASS_EDITOR, grasseditor, grasscomplete, TextID::GRASS_EDITOR_NOTE, MenuEntryId::EnvironmentGrassEditor), {
			NamedHotkey(Key::R | Key::DPadDown, TextID::GRASS_EDITOR_HOTKEY1),
			NamedHotkey(Key::R | Key::DPadUp, TextID::GRASS_EDITOR_HOTKEY2),
			NamedHotkey(Key::R | Key::DPadRight, TextID::GRASS_EDITOR_HOTKEY3)
		}));
		ENVC->Append(CreateEntry(TextID::KEEP_GRASS_STATE, KeepGrassState, TextID::KEEP_GRASS_STATE_NOTE, MenuEntryId::EnvironmentKeepGrassState));
		ENVC->Append(CreateEntry(TextID::BURIED_INSPECTOR, BuriedInspector, TextID::BURIED_INSPECTOR_NOTE, MenuEntryId::EnvironmentBuriedInspector));
		ENVC->Append(CreateEntry(TextID::ITEMS_DONT_DISSAPPEAR, ItemsDontDissappearOnInvalidPositions, TextID::ITEMS_DONT_DISSAPPEAR_NOTE, MenuEntryId::EnvironmentItemsDontDissapear));
		{
			MenuEntry *setFoliage = CreateEntry(TextID::SET_FOLIAGE_SEASON, nullptr, SetFoliageSeason, TextID::SET_FOLIAGE_SEASON_NOTE, MenuEntryId::EnvironmentSetFoliageSeason);
			setFoliage->SetSavedValueApplyCallback(SetFoliageSeasonApplySaved);
			ENVC->Append(setFoliage);
		}
		menu->Append(ENVC);

	//////////////////////
	/*Extra Codes Folder*/
	//////////////////////
		MenuFolder *EXTC = CreateFolder(FolderType::Extra);
	/////////////////////
	/*Chat Codes Folder*/
	/////////////////////
		MenuFolder *CHAC = CreateFolder(FolderType::Extra, SubFolder::Chat);
		CHAC->Append(CreateEntry(TextID::CHAT_DONT_DISSAPEAR, bubblesDisappear, TextID::CHAT_DONT_DISSAPEAR_NOTE, MenuEntryId::ChatDontDissapear)),
		CHAC->Append(EntryWithHotkey(CreateEntry(TextID::FORCE_CHAT, Forcesendchat, TextID::FORCE_CHAT_NOTE, MenuEntryId::ChatForce), {
			NamedHotkey(Key::R, TextID::FORCE_CHAT)
		})),
		CHAC->Append(CreateEntry(TextID::CHAT_COMMANDS, chatCommands, TextID::CHAT_COMMANDS_NOTE, MenuEntryId::ChatCommands)),
		CHAC->Append(CreateEntry(TextID::CHAT_BUTTON, ChatButton, TextID::CHAT_BUTTON_NOTE, MenuEntryId::ChatButton)),
		CHAC->Append(EntryWithHotkey(CreateEntry(TextID::CHAT_TRANSLATION_OPTIONS, ChatTranslationOptions, TextID::CHAT_TRANSLATION_OPTIONS_NOTE, MenuEntryId::ChatTranslationOptions), {
			NamedHotkey(Key::X | Key::DPadLeft, TextID::CHAT_TRANSLATION_OPTIONS)
		})),
		CHAC->Append(CreateEntry(TextID::KEY_EXTEND, key_limit, TextID::KEY_EXTEND_NOTE, MenuEntryId::ChatKeyExtend)),
		CHAC->Append(CreateEntry(TextID::CUSTOM_KEY, CustomKeyboard, TextID::CUSTOM_KEY_NOTE, MenuEntryId::ChatCustomKey)),
		CHAC->Append(CreateEntry(TextID::MORE_NUMBERS,  morenumberisland, TextID::MORE_NUMBERS_NOTE, MenuEntryId::ChatMoreNumbers)),
		EXTC->Append(CHAC);
	////////////////////
	/*Fun Codes Folder*/
	////////////////////
		MenuFolder *FUNC = CreateFolder(FolderType::Extra, SubFolder::Fun);
		{
			MenuEntry *sizeCodesEntry = CreateEntry(TextID::SIZE_CODES, nullptr, sizecodes, TextID::SIZE_CODES_NOTE, MenuEntryId::FunSizeCodes);
			sizeCodesEntry->SetSavedValueApplyCallback(SizeCodesApplySaved);
			FUNC->Append(sizeCodesEntry);
		}
		FUNC->Append(CreateEntry(TextID::T_POSE, tposeentry, TextID::T_POSE_NOTE, MenuEntryId::FunTPose)),
		FUNC->Append(EntryWithHotkey(CreateEntry(TextID::TAKE_TPC_PIC, freezeframe, TextID::TAKE_TPC_PIC_NOTE, MenuEntryId::FunTakeTpcPic), {
			NamedHotkey(Key::DPadRight, TextID::TAKE_TPC_PIC_KEY1),
			NamedHotkey(Key::A, TextID::TAKE_TPC_PIC_KEY2)
		})),
		FUNC->Append(CreateEntry(TextID::MAX_TURBO, maxturbo, TextID::MAX_TURBO_NOTE, MenuEntryId::FunMaxTurbo)),
		FUNC->Append(CreateEntry(TextID::MULTI_PRESS, asmpresses, TextID::MULTI_PRESS_NOTE, MenuEntryId::FunMultiPress)),
		FUNC->Append(EntryWithHotkey(CreateEntry(TextID::ULTIMATE_POPPER, partypopper, TextID::ULTIMATE_POPPER_NOTE, MenuEntryId::FunUltimatePopper), {
			NamedHotkey(Key::B | Key::DPadLeft, TextID::ULTIMATE_POPPER_KEY1)
		})),
		FUNC->Append(EntryWithHotkey(CreateEntry(TextID::BEANS_PARTICLE, BeansParticleChanger, TextID::BEANS_PARTICLE_NOTE, MenuEntryId::MiscBeansParticle), {
			NamedHotkey(Key::L | Key::DPadLeft, TextID::BEANS_PARTICLE)
		})),
	    FUNC->Append(CreateEntry(TextID::CAMERA_MOD, cameramod, TextID::CAMERA_MOD_NOTE, MenuEntryId::FunCameraMod)),
		EXTC->Append(FUNC);
		EXTC->Append(CreateEntry(TextID::SHOP_ALWAYS_OPEN_NAME, ShopsAlwaysOpen, TextID::SHOP_ALWAYS_OPEN_NOTE, MenuEntryId::ExtraShopsAlwaysOpen)),
		EXTC->Append(CreateEntry(TextID::DISABLE_SAVE, nonesave, TextID::DISABLE_SAVE_NOTE, MenuEntryId::ExtraDisableSave)),
		EXTC->Append(CreateEntry(TextID::DISABLE_ITEM_LOCKS, bypass, TextID::DISABLE_ITEM_LOCKS_NOTE, MenuEntryId::ExtraDisableItemLocks)),
		EXTC->Append(CreateEntry(TextID::CANT_FALL_HOLE, noTrap, TextID::CANT_FALL_HOLE_NOTE, MenuEntryId::ExtraCantFallHole)),
		EXTC->Append(CreateEntry(TextID::SET_SPOT_STATE_NAME, nullptr, SetSpotState, TextID::SET_SPOT_STATE_NOTE, MenuEntryId::ExtraSetSpotState)),
		EXTC->Append(CreateEntry(TextID::AMIIBO_SPOOFER, AmiiboSpoofer, TextID::AMIIBO_SPOOFER_NOTE, MenuEntryId::ExtraAmiiboSpoofer)),
		EXTC->Append(EntryWithHotkey(CreateEntry(TextID::TIME_TRAVEL, TimeTravel, TTKeyboard, TextID::TIME_TRAVEL_NOTE, MenuEntryId::ExtraTimeTravel), {
			NamedHotkey(Key::R | Key::DPadRight, TextID::TIME_FORWARD),
			NamedHotkey(Key::R | Key::DPadLeft, TextID::TIME_BACKWARDS)
		})),
		menu->Append(EXTC);

	/////////////////////
	/*Misc Codes Folder*/
	/////////////////////
		MenuFolder *MISC = CreateFolder(FolderType::Misc);
		{
			MenuEntry *keepConnEntry = CreateEntry(TextID::KEEP_CONNECTION, nullptr, KeepConnectionToggle, TextID::KEEP_CONNECTION_NOTE, MenuEntryId::MiscKeepConnection);
			keepConnEntry->SetSavedValueApplyCallback(KeepConnectionApplySaved);
			MISC->Append(keepConnEntry);
		}
		{
			MenuEntry *gameTypeEntry = CreateEntry(TextID::GAME_TYPE, nullptr, mgtype, TextID::GAME_TYPE_NOTE, MenuEntryId::MiscGameType);
			gameTypeEntry->SetSavedValueApplyCallback(GameTypeApplySaved);
			MISC->Append(gameTypeEntry);
		}
		MISC->Append(CreateEntry(TextID::RADIO_PLAYER, radioPlayer , TextID::RADIO_PLAYER_NOTE, MenuEntryId::MiscRadioPlayer)),
		MISC->Append(CreateEntry(TextID::RELOAD_ROOM_NAME, nullptr, ReloadRoomCheat, TextID::RELOAD_ROOM_NOTE, MenuEntryId::MiscReloadRoom)),
		MISC->Append(CreateEntry(TextID::LARGE_FOV, fovlarge, TextID::LARGE_FOV_NOTE, MenuEntryId::MiscLargeFov)),
		MISC->Append(CreateEntry(TextID::MOVE_FURN, roomSeeder, TextID::MOVE_FURN_NOTE, MenuEntryId::MiscMoveFurn)),
		MISC->Append(CreateEntry(TextID::WALK_TALK, walktalkentry, TextID::WALK_TALK_NOTE, MenuEntryId::MiscWalkTalk)),
		MISC->Append(CreateEntry(TextID::FAST_MODE, fast, TextID::FAST_MODE_NOTE, MenuEntryId::MiscFastMode)),
		MISC->Append(CreateEntry(TextID::FAST_TEXT_SPEED, fasttalk, TextID::FAST_TEXT_SPEED_NOTE, MenuEntryId::MiscFastTextSpeed)),
		MISC->Append(CreateEntry(TextID::FAST_GAME_SPEED, speedentry, TextID::FAST_GAME_SPEED_NOTE, MenuEntryId::MiscFastGameSpeed)),
		MISC->Append(CreateEntry(TextID::FAST_ISABELLE, fastisabelle, TextID::FAST_ISABELLE_NOTE, MenuEntryId::MiscFastIsabelle)),
		menu->Append(MISC);

	////////////////////////
	/*Default Codes Folder*/
	////////////////////////
	MenuFolder *DEFAULTC = CreateFolder(FolderType::Default);

	DEFAULTC->Append(CreateEntry(TextID::SEED_ITEM_LEGIT, nullptr, SeedItemLegitimacyEntry, TextID::NONE, MenuEntryId::DefaultSeedItemLegit)),
	DEFAULTC->Append(CreateEntry(TextID::DROP_LAG_REMOVE, nullptr, OnlineDropLagRemoverEntry, TextID::NONE, MenuEntryId::DefaultDropLagRemove)),
	DEFAULTC->Append(CreateEntry(TextID::CHANGE_ROCKBREAK_PARTICLE, nullptr, ChangeRockbreakParticleEntry, TextID::NONE, MenuEntryId::DefaultChangeRockbreakParticle)),
	DEFAULTC->Append(CreateEntry(TextID::DROP_ITEMS_EVERYWHERE, nullptr, DropItemsEverywhereEntry, TextID::NONE, MenuEntryId::DefaultDropItemsEverywhere)),
	DEFAULTC->Append(CreateEntry(TextID::IDLE_AFTER_TREE_SHAKE_OR_CUT, nullptr, IdleAfterTreeShakeOrCutEntry, TextID::NONE, MenuEntryId::DefaultIdleAfterTreeShakeOrCut)),
	//DEFAULTC->Append(CreateEntry(TextID::DONT_MOVE_NPC_BACK_TO_ORIGINAL_POSITION, nullptr, DontMoveNPCBackToOriginalPositionEntry, TextID::NONE)),
	DEFAULTC->Append(CreateEntry(TextID::REPLACE_DROP_FUNCTIONS, nullptr, ReplaceDropFunctionsEntry, TextID::NONE, MenuEntryId::DefaultReplaceDropFunctions)),
	DEFAULTC->Append(CreateEntry(TextID::PREVENT_PARTICLE_CRASH, nullptr, PreventParticleCrashEntry, TextID::NONE, MenuEntryId::DefaultPreventParticleCrash)),
	DEFAULTC->Append(CreateEntry(TextID::BYPASS_GAME_CHECKS, nullptr, BypassGameChecksEntry, TextID::NONE, MenuEntryId::DefaultBypassGameChecks)),
	DEFAULTC->Append(CreateEntry(TextID::DISABLE_NON_SEED_ITEM_CHECK, nullptr, DisableNonSeedItemCheckEntry, TextID::NONE, MenuEntryId::DefaultDisableNonSeedItemCheck)),
	DEFAULTC->Append(CreateEntry(TextID::PATCH_DROP_FUNCTION, nullptr, PatchDropFunctionEntry, TextID::NONE, MenuEntryId::DefaultPatchDropFunction)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_PRETENO_ISLAND_SESSION, nullptr, FixPretendoOnlineIslandSessionEntry, TextID::NONE, MenuEntryId::DefaultFixPretendoOnlineIslandSession)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_PRETENDO_FIND_SESSION, nullptr, FixPretendoFindSessionByOwnerCallEntry, TextID::NONE, MenuEntryId::DefaultFixPretendoFindSessionByOwnerCall)),

	DEFAULTC->Append(CreateEntry(TextID::CHECK_INVALID_BADGE, nullptr, CheckInvalidBadgeEntry, TextID::NONE, MenuEntryId::DefaultCheckInvalidBadge)),
	DEFAULTC->Append(CreateEntry(TextID::DISABLE_OPEN_SAVE_MENU_WITH_START_BUTTON, nullptr, DisableOpenSaveMenuWithStartButton, TextID::NONE, MenuEntryId::DefaultDisableOpenSaveMenuWithStart)),
	DEFAULTC->Append(CreateEntry(TextID::DISABLE_CATALOG_SEARCH_FUNCTION, nullptr, DisableCatalogSearchFunction, TextID::NONE, MenuEntryId::DefaultDisableCatalogSearch)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_INVALID_PICKUP_CRASH, nullptr, FixInvalidPickupCrash, TextID::NONE, MenuEntryId::DefaultFixInvalidPickupCrash)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_INVALID_DROP_PLANT_CRASH, nullptr, FixInvalidDropPlantCrash, TextID::NONE, MenuEntryId::DefaultFixInvalidDropPlantCrash)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_INVALID_SPRITE_CRASH, nullptr, FixInvalidSpriteCrash, TextID::NONE, MenuEntryId::DefaultFixInvalidSpriteCrash)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_INVALID_GIVE_ITEM_CRASH, nullptr, FixInvalidGiveItemCrash, TextID::NONE, MenuEntryId::DefaultFixInvalidGiveItemCrash)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_INVALID_HOLE_CRASH, nullptr, FixInvalidHoleCrash, TextID::NONE, MenuEntryId::DefaultFixInvalidHoleCrash)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_INVALID_ITEM_CRASH, nullptr, FixInvalidItemCrash, TextID::NONE, MenuEntryId::DefaultFixInvalidItemCrash)),
	DEFAULTC->Append(CreateEntry(TextID::CONVERT_FLOWER_FROM_SEED_ITEM_TO_NORMAL_ITEM, nullptr, ConvertFlowerFromSeedItemToNormalItem, TextID::NONE, MenuEntryId::DefaultConvertFlowerSeedToNormal)),
	DEFAULTC->Append(CreateEntry(TextID::SET_SEED_ITEM_NAMES, nullptr, SetSeedItemNames, TextID::NONE, MenuEntryId::DefaultSetSeedItemNames)),
	DEFAULTC->Append(CreateEntry(TextID::SET_ITEM_REPLACEMENT_RULES, nullptr, SetItemReplacementRules, TextID::NONE, MenuEntryId::DefaultSetItemReplacementRules)),
	DEFAULTC->Append(CreateEntry(TextID::SET_DROP_RULES, nullptr, SetDropRules, TextID::NONE, MenuEntryId::DefaultSetDropRules)),
	DEFAULTC->Append(CreateEntry(TextID::SET_PLANT_RULES, nullptr, SetPlantRules, TextID::NONE, MenuEntryId::DefaultSetPlantRules)),
	DEFAULTC->Append(CreateEntry(TextID::FIX_PARTICLES_IN_PUZZLE_LEAGUE, nullptr, FixParticlesInPuzzleLeague, TextID::NONE, MenuEntryId::DefaultFixParticlesPuzzleLeague)),
	DEFAULTC->Append(CreateEntry(TextID::SET_CUSTOM_SPRITES_FOR_PRO_DESIGNS, nullptr, SetCustomSpritesForProDesigns, TextID::NONE, MenuEntryId::DefaultSetCustomSpritesProDesigns)),
	menu->Append(DEFAULTC);

	////////////////////
	/*Dev Codes Folder*/
	////////////////////
	#if DEVMODE
		MenuFolder *DEVC = CreateFolder(FolderType::Dev);
		DEVC->Append(CreateEntry(TextID::CUSTOM_DUMP, nullptr, customdump, TextID::CUSTOM_DUMP_NOTE, MenuEntryId::DevCustomDump)),
		DEVC->Append(CreateEntry(TextID::CALL_FUNCTION, FunctionsCaller, FunctionsCallerSettings, TextID::CALL_FUNCTION_NOTE, MenuEntryId::DevCallFunction)),
		DEVC->Append(CreateEntry(TextID::DISPLAY_SAVE_FLAG_DIFFERENCE, displaySaveFlagDifference, TextID::NONE, MenuEntryId::DevDisplaySaveFlagDifference)),
		DEVC->Append(CreateEntry(TextID::UNUSED_FALL_DOWN, falldownfishing, TextID::NONE, MenuEntryId::DevUnusedFallDown)),
		DEVC->Append(CreateEntry(TextID::UNLOCK_CRO_REGION, unlockCroRegion, TextID::UNLOCK_CRO_REGION_NOTE, MenuEntryId::DevUnlockCroRegion)),
		DEVC->Append(CreateEntry(TextID::ACNH_CHEAT, acnh, TextID::NONE, MenuEntryId::DevAcnhCheat)),
		DEVC->Append(CreateEntry(TextID::PLAYER_DUMPER, nullptr, player_dumper, TextID::NONE, MenuEntryId::DevPlayerDumper)),
		DEVC->Append(CreateEntry(TextID::WAND_ABILITY, wandability, TextID::WAND_ABILITY_NOTE, MenuEntryId::DevWandAbility)),
		DEVC->Append(CreateEntry(TextID::LIGHT_SWITCH_TOGGLER, lightswitch, TextID::LIGHT_SWITCH_TOGGLER_NOTE, MenuEntryId::DevLightSwitchToggler)),
		DEVC->Append(CreateEntry(TextID::EXPRESSION_MOD, FacialExpressionMod, SetFacialExpression, TextID::EXPRESSION_MOD_NOTE, MenuEntryId::DevExpressionMod)),
		DEVC->Append(CreateEntry(TextID::PRIVATE_ISLAND, PrivateIsland, TextID::NONE, MenuEntryId::DevPrivateIsland)),
		menu->Append(DEVC);
	#endif

		menu->Append(CreateEntry(TextID::PLUGIN_SETTINGS, nullptr, pluginSettingsEntry, TextID::NONE, MenuEntryId::PluginSettings));
	
	///////////////////
	/*Pretendo Folder*/
	///////////////////
		MenuFolder *PRET = CreateFolder(FolderType::Pretendo);
	////////////////////////////////
	/*Players In Session SubFolder*/
	////////////////////////////////
	/*
		if (getPiaLoggerStatus() == PatternStatus::Active) {
			MenuFolder *PLAYSESS = CreateFolder(FolderType::Pretendo, SubFolder::PlayersInSession);
			PLAYSESS->OnAction = onPiaPlayersFolderAction;
			PRET->Append(PLAYSESS);
		}
	*/
	////////////////////////////////
	/*PIA logger Options SubFolder*/
	////////////////////////////////
	///*
		MenuFolder *PIALOG = CreateFolder(FolderType::Pretendo, SubFolder::PIALoggerOptions);
		PRET->Append(CreateEntry(TextID::PLAYERS_IN_SESSION, nullptr, PretendoPlayersInSessionEntry, TextID::NONE, MenuEntryId::PretendoPlayersInSession)),
	    PIALOG->OnAction = onPiaLoggerOptionsFolderAction;
		PRET->Append(PIALOG);
		
		menu->Append(PRET);
	//*/
	}
}



