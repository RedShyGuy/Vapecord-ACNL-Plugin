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
            entry->Name() += " " + hotkey.ToString();
            entry->Hotkeys.OnHotkeyChangeCallback([](MenuEntry *entry, int index) {
				UpdateAll();
            });
        }
        return(entry);
    }

/*This will load all the folders and entrys*/
    void InitMenu(PluginMenu *menu) {
	/////////////////////
	/*Save Codes Folder*/
	/////////////////////
		SAVEC = new MenuFolder(FolderColors[0] << "Save Codes");
	    SAVEC->Append(new MenuEntry(FolderColors[0] << "Town Name Changer", nullptr, townnamechanger, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Save Options", nullptr, savebackup, "note")),	
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Bulleting Board Dumper", nullptr, bullboard, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Tree Size", nullptr, TreeSizeChanger, "note")),	
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Change Native Fruit", nullptr, ChangeNativeFruit, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Unlock PWP's", nullptr, PWPUnlock, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Grass Type Changer", nullptr, GrassChanger, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Caravan Changer", nullptr, caravanchange, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Camping Villager Changer", nullptr, SetCampingVillager, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Shop Unlocker", nullptr, shopunlocks, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "House Editor", nullptr, HouseChanger, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Unlock QR Machine", nullptr, unlockqrmachine, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Building Modifier", nullptr, BuildingMod, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Mayor Permit 100%", Permit100, "note")),
		SAVEC->Append(new MenuEntry(FolderColors[0] << "Real Time Acre Editor", MapEditor, "note")),
		menu->Append(SAVEC);

	/////////////////////////
	/*Movement Codes Folder*/
	/////////////////////////
		MOVEC = new MenuFolder(FolderColors[1] << "Movement Codes");
		MOVEC->Append(new MenuEntry(FolderColors[1] << "Other Players Can't Push You", nullptr, noPush, "note")),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Coordinate Modifier", coordinate, coordspeed, "note"), {
			Hotkey(Key::A, "Key"), 
			Hotkey(Key::DPadRight, "Key2"), 
			Hotkey(Key::DPadLeft, "Key3"), 
			Hotkey(Key::DPadDown, "Key4"), 
			Hotkey(Key::DPadUp, "Key5") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Moon Jump", moonjump, "note"), { 
			Hotkey(Key::L | Key::DPadUp, "Key"), 
			Hotkey(Key::L | Key::DPadDown, "Key2") 
		})),
		MOVEC->Append(new MenuEntry(FolderColors[1] << "Touch Warp", tch_warp, "note")),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Walk Over Things", walkOver, "note"), { 
			Hotkey(Key::L | Key::DPadUp, "Key") 
		})),		   
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Movement Changer", MovementChanger, "note"), { 
			Hotkey(Key::L | Key::B, "Key") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Walk Particle Changer", Walkparticle, "note"), { 
			Hotkey(Key::L | Key::X, "Key")
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Player Teleporter", stalk, "note"), { 
			Hotkey(Key::R | Key::DPadLeft, "Key"), 
			Hotkey(Key::R | Key::DPadRight, "Key2") 
		})),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Visibility Modifier", onlineplayermod, "note"), { 
			Hotkey(Key::L | Key::A, "Key") 
		})),
		MOVEC->Append(new MenuEntry(FolderColors[1] << "Speed Modifier", speedMod, menuSpeedMod, "note")),
		MOVEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[1] << "Room Warping", roomWarp, "note"), { 
			Hotkey(Key::L | Key::X, "Key") 
		})),
		menu->Append(MOVEC);

	//////////////////////////
	/*Inventory Codes Folder*/
	//////////////////////////
		INVC = new MenuFolder(FolderColors[2] << "Inventory Codes");
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "Text To Items", t2i, "note"), { 
			Hotkey(Key::X | Key::DPadRight, "Key"), 
			Hotkey(Key::X |Key::DPadUp, "Key2"), 
			Hotkey(Key::X | Key::DPadDown, "Key3"),
			Hotkey(Key::X | Key::DPadLeft, "Key4"),
		})),
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "Duplicate Items", duplication, "note"), { 
			Hotkey(Key::R, "Key"), 
			Hotkey(Key::R | Key::X, "Key2") 
		})),
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "Catalog To Pockets", catalog, "note"), { 
			Hotkey(Key::L | Key::DPadRight, "Key"), 
			Hotkey(Key::L | Key::Y, "Key2") 
		})),
		INVC->Append(EntryWithHotkey(new MenuEntry(FolderColors[2] << "Chat Text2Item", chatt2i, "note"), { 
			Hotkey(Key::R | Key::DPadLeft, "Key") 
		})),
		INVC->Append(new MenuEntry(FolderColors[2] << "Clear Inventory", nullptr, ClearInventory, "note")),
		INVC->Append(new MenuEntry(FolderColors[2] << "Item Settings", nullptr, itemsettings, "note")),
		INVC->Append(new MenuEntry(FolderColors[2] << "Design Menu Changer", nullptr, MenuChanger, "note")),
		INVC->Append(new MenuEntry(FolderColors[2] << "Get Set", nullptr, getset, "note")),
		INVC->Append(new MenuEntry(FolderColors[2] << "Custom Buttons", nullptr, SettingsButton, "note")),
		menu->Append(INVC);

	///////////////////////
	/*Player Codes Folder*/
	///////////////////////
		PLAYC = new MenuFolder(FolderColors[3] << "Player Codes");
	////////////////////////////
	/*Player Save Codes Folder*/
	////////////////////////////
		PSAVEC = new MenuFolder(FolderColors[3] << "Player Save Codes");
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Player Name Changer", nullptr, NameChanger, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Player Appearance", nullptr, playermod, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Random Player", nullptr, randomoutfit, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Player Backup", nullptr, playerbackup, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "TPC Message Changer", nullptr, tpcmessage, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "TPC Settings", nullptr, tpc, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Design Dumper", nullptr, DesignDumper, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Emotion List", nullptr, emotelist, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Encyclopedia List", nullptr, enzyklopedia, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Dream Code Modifier", nullptr, comodifier, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Census Menu", nullptr, debug1, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Song List", nullptr, FillSongs, "note")),
		PSAVEC->Append(new MenuEntry(FolderColors[3] << "Fill Catalog", nullptr, FillCatalog, "note")),
		PLAYC->Append(PSAVEC);
		PLAYC->Append(new MenuEntry(FolderColors[3] << "Player Info", nullptr, debug, "note")),
	//PLAYC->Append(new MenuEntry(FolderColors[3] << "Player Loader", nullptr, pLoaderEntry, "note")),
		PLAYC->Append(EntryWithHotkey(new MenuEntry(FolderColors[3] << "Neck Position", neckentry, "note"), { 
			Hotkey(Key::L | Key::DPadDown, "Key") 
		})),
		PLAYC->Append(EntryWithHotkey(new MenuEntry(FolderColors[3] << "Custom Hair&Eye Colors", App_ColorMod, SaveColor, "note"), { 
			Hotkey(Key::R, "Key")
		})),
		PLAYC->Append(new MenuEntry(FolderColors[3] << "Wear Helmet And Accessory", hatz, "note")),
		PLAYC->Append(EntryWithHotkey(new MenuEntry(FolderColors[3] << "Player Faint", Faint, "note"), {
			Hotkey(Key::R | Key::A, "Key")
		})),
		PLAYC->Append(new MenuEntry(FolderColors[3] << "Show Players On The Map", map, "note")),

		PLAYC->Append(new MenuEntry(FolderColors[3] << "Never Bedhead", NeverBedHead, "note")),
		menu->Append(PLAYC);

	//////////////////////////
	/*Animation Codes Folder*/
	//////////////////////////
		ANIMC = new MenuFolder(FolderColors[4] << "Animation Codes");
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "Player Selector", playerSelector, "note"), { 
			Hotkey(Key::L | Key::DPadRight, "Key"), 
			Hotkey(Key::L | Key::DPadLeft, "Key2") 
		})),
		ANIMC->Append(new MenuEntry(FolderColors[4] << "Anti Animation", anticheat, "note")),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "Animation Modifier", execAnim, "note"), { 
			Hotkey(Key::A | Key::DPadRight, "Key"), 
			Hotkey(Key::A | Key::DPadLeft, "Key2"), 
			Hotkey(Key::A | Key::DPadUp, "Key3"), 
			Hotkey(Key::A | Key::B, "Key4") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "Emotion Loop", inf_expression, "note"), { 
			Hotkey(Key::B, "Key") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "Idle", idle, "note"), { 
			Hotkey(Key::R, "Key") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "Slow-Motion Animations", slmoanms, "note"), { 
			Hotkey(Key::L | Key::DPadLeft, "Key") 
		})),
		ANIMC->Append(EntryWithHotkey(new MenuEntry(FolderColors[4] << "Set Animations On Everyone", doonall, "note"), { 
			Hotkey(Key::R | Key::A, "Key"), 
			Hotkey(Key::R | Key::B, "Key2"), 
			Hotkey(Key::R | Key::Y, "Key3") 
		})),
		menu->Append(ANIMC);

	////////////////////////
	/*Seeding Codes Folder*/
	////////////////////////
		SEEDC = new MenuFolder(FolderColors[5] << "Seeding Codes");
	////////////////////////
	/*Seed Codes SubFolder*/
	////////////////////////
		SEED1C = new MenuFolder(FolderColors[5] << "Seed Codes");
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "PickUp Seeder", pickseeder, "note"), { 
			Hotkey(Key::B | Key::DPadLeft, "Key"), 
			Hotkey(Key::B | Key::DPadDown, "Key2"), 
			Hotkey(Key::B | Key::DPadUp, "Key3"), 
			Hotkey(Key::B | Key::DPadRight, "Key4"), 
			Hotkey(Key::B | Key::L, "Key5") 
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Walking Remover", Walkseeder, "note"), { 
			Hotkey(Key::R | Key::B, "Key") 
		})),
		SEED1C->Append(new MenuEntry(FolderColors[5] << "Firework Seeder", nullptr, fireworkentry, "note")),
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Map Editor", tileSelector, "note"), { 
			Hotkey(Key::Start | Key::DPadUp, "Key"), 
			Hotkey(Key::DPadRight, "Key2"), 
			Hotkey(Key::DPadLeft, "Key3"), 
			Hotkey(Key::DPadDown, "Key4"), 
			Hotkey(Key::DPadUp, "Key5"), 
			Hotkey(Key::L, "Key6"), 
			Hotkey(Key::R, "Key7"), 
			Hotkey(Key::Start | Key::DPadDown, "Key8"), 
			Hotkey(Key::Start | Key::DPadLeft, "Key9"), 
			Hotkey(Key::A, "Key10"), 
			Hotkey(Key::Start | Key::DPadRight, "Key11") 
		})),
		SEED1C->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Everything Seeder", everythingseeder, E_Seeder_KB, "note"), { 
			Hotkey(Key::L, "Key") 
		})),
		SEEDC->Append(SEED1C);
	////////////////////////
	/*Drop Codes SubFolder*/
	////////////////////////
		DROPC = new MenuFolder(FolderColors[5] << "Drop Codes");
		DROPC->Append(new MenuEntry(FolderColors[5] << "Item Sequencer", nullptr, Entry_itemsequence, "note")),
	    DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Drop Modifiers", dropMod, "note"), { 
			Hotkey(Key::A | Key::DPadRight, "Key"), 
			Hotkey(Key::A | Key::DPadLeft, "Key2"), 
			Hotkey(Key::A | Key::DPadDown, "Key3"), 
			Hotkey(Key::A | Key::DPadUp, "Key4") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Drop Items", instantDrop, "note"), { 
			Hotkey(Key::Y | Key::DPadRight, "Key"), 
			Hotkey(Key::L | Key::DPadDown, "Key2") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Auto Drop", autoDrop, "note"), { 
			Hotkey(Key::Y | Key::DPadRight, "Key"), 
			Hotkey(Key::Y | Key::DPadLeft, "Key2") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Touch Drop", touchDrop, "note"), { 
			Hotkey(Key::Y | Key::DPadRight, "Key") 
		})),
		DROPC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Slot Drop", ShowInvSlotID, "note"), { 
			Hotkey(Key::R | Key::Y, "Key"), 
			Hotkey(Key::R | Key::X, "Key2") 
		})),
		SEEDC->Append(DROPC);
	////////////////////////
	/*Tree Codes SubFolder*/
	////////////////////////
		TREEC = new MenuFolder(FolderColors[5] << "Tree Codes");
		TREEC->Append(new MenuEntry(FolderColors[5] << "Infinite Fruit Tree", nullptr, fruitStays, "note")),
		TREEC->Append(new MenuEntry(FolderColors[5] << "Axe Tree Shake", nullptr, shakechop, "note")),
		TREEC->Append(EntryWithHotkey(new MenuEntry(FolderColors[5] << "Fruit Tree Item Modifier", fruititemmod, "note"), { 
			Hotkey(Key::A | Key::DPadLeft, "Key") 
		})),
		TREEC->Append(new MenuEntry(FolderColors[5] << "Instant Tree Chop", instantchop, "note")),
		SEEDC->Append(TREEC);
		menu->Append(SEEDC);

	//////////////////////
	/*Money Codes Folder*/
	//////////////////////
		MONC = new MenuFolder(FolderColors[6] << "Money Codes");
		MONC->Append(new MenuEntry(FolderColors[6] << "Wallet", nullptr, wallet, "note")),
		MONC->Append(new MenuEntry(FolderColors[6] << "Bank", nullptr, bank, "note")),
		MONC->Append(new MenuEntry(FolderColors[6] << "Meow Coupons", nullptr, coupon, "note")),
		MONC->Append(new MenuEntry(FolderColors[6] << "Badges", nullptr, badges, "note")),
		MONC->Append(new MenuEntry(FolderColors[6] << "Medals", nullptr, medals, "note")),
		MONC->Append(new MenuEntry(FolderColors[6] << "Turnips", nullptr, turnips, "note")),
		menu->Append(MONC);

	///////////////////////
	/*Island Codes Folder*/
	///////////////////////
		ISLC = new MenuFolder(FolderColors[7] << "Island Codes");
		ISLC->Append(new MenuEntry(FolderColors[7] << "Unlock Island", nullptr, UnlockIsland, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Always Bonus Ore", bonusOre, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Instant Fruit Finish", instantFruit, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Hacker Island Spoof", nullptr, Hackerisland, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Island Country Spoof", nullptr, Countryspoof, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Free Kappn Rides", FreeKappn, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Island Shop Slot Modifier", IslandShop, IslandSettings, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "All Tours Selectable", alltour, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Island Acre Modifier", acreMod, menuAcreMod, "note")),
		ISLC->Append(new MenuEntry(FolderColors[7] << "Island Building Modifier", buildingMod, menuBuildingMod, "note")),
		menu->Append(ISLC);

	//////////////
	/*NPC Folder*/
	//////////////
		NPCC = new MenuFolder(FolderColors[8] << "NPC Codes");
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC Selector", NPCFunction, "note"), { 
			Hotkey(Key::L | Key::A, "Key") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC Animation Modifier",  NPCAnimation, NPCSetAnim, "note"), { 
			Hotkey(Key::L | Key::B, "Key") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC Coordinates Modifier", NPCCoordinates, "note"), { 
			Hotkey(Key::L, "Key"),
			Hotkey(Key::DPadRight, "Key2"), 
			Hotkey(Key::DPadLeft, "Key3"), 
			Hotkey(Key::DPadDown, "Key4"), 
			Hotkey(Key::DPadUp, "Key5")
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC Teleport To You", NPCTeleportToYou, "note"), { 
			Hotkey(Key::L | Key::Y, "Key") 
		})),
		NPCC->Append(EntryWithHotkey(new MenuEntry(FolderColors[8] << "NPC Rotation Modifier", NPCRotate, "note"), { 
			Hotkey(Key::L, "Key") 
		})),
		menu->Append(NPCC);

	////////////////////
	/*Fun Codes Folder*/
	////////////////////
		FUNC = new MenuFolder(FolderColors[9] << "Fun Codes"); 
		FUNC->Append(new MenuEntry(FolderColors[9] << "Size Codes", nullptr, sizecodes, "note")),
		FUNC->Append(new MenuEntry(FolderColors[9] << "T-Pose", nullptr, tposeentry, "note")),
		FUNC->Append(EntryWithHotkey(new MenuEntry(FolderColors[9] << "Take TPC Pic", freezeframe, "note"), { 
			Hotkey(Key::L, "Key"), 
			Hotkey(Key::A, "Key2") 
		})),
		FUNC->Append(new MenuEntry(FolderColors[9] << "Max Turbo Presses", maxturbo, "note")),
		FUNC->Append(new MenuEntry(FolderColors[9] << "Multi-Presses", asmpresses, "note")),
		FUNC->Append(EntryWithHotkey(new MenuEntry(FolderColors[9] << "Ultimate Party Popper", partypopper, "note"), { 
			Hotkey(Key::B | Key::DPadLeft, "Key") 
		})),
	    FUNC->Append(new MenuEntry(FolderColors[9] << "Camera Mod", cameramod, "note")),
		menu->Append(FUNC);

	//////////////////////
	/*Extra Codes Folder*/
	//////////////////////
		EXTC = new MenuFolder(FolderColors[10] << "Extra Codes");
	/////////////////////
	/*Fish Codes Folder*/
	/////////////////////
		FISC = new MenuFolder(FolderColors[10] << "Fish Codes");
		FISC->Append(new MenuEntry(FolderColors[10] << "Fish Bite Always", FishAlwaysBiteRightAway, "note")),
		FISC->Append(new MenuEntry(FolderColors[10] << "Fish Can't Be Scared", FishCantBeScared, "note")),
		EXTC->Append(FISC);
	/////////////////////
	/*Chat Codes Folder*/
	/////////////////////
		CHAC = new MenuFolder(FolderColors[10] << "Chat Codes");
		CHAC->Append(new MenuEntry(FolderColors[10] << "Chat Bubbles Don't Disappear", bubblesDisappear, "note")),
		CHAC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "Chat Copy/Paste", ChatCopyPaste, "note"), {
			Hotkey(Key::L | Key::DPadRight, "Key"),
			Hotkey(Key::L | Key::DPadUp, "Key2"),
			Hotkey(Key::L | Key::DPadDown, "Key3"),
			Hotkey(Key::L | Key::DPadLeft, "Key4")
		})),
		CHAC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "Force Send Chat", Forcesendchat, "note"), { 
			Hotkey(Key::R, "Key") 
		})),
		EXTC->Append(CHAC);
		EXTC->Append(new MenuEntry(FolderColors[10] << "Shops Always Open", nullptr, ShopsAlwaysOpen, "note")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Disable Save Menus", nullptr, nonesave, "note")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Disable Item Locks", nullptr, bypass, "note")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Can't Fall In Holes Or Pitfalls", nullptr, noTrap, "note")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Set Spot State", nullptr, SetSpotState, "note")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Search And Replace", nullptr, SearchReplace, "note")),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Remove All Town Items", nullptr, RemoveItemsCheat, "note")),
	    EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "Water All Flowers", WaterAllFlowers, "note"), { 
			Hotkey(Key::R | Key::DPadLeft, "Key") 
		})),
		EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "Weed Remover", weedremover, "note"), { 
			Hotkey(Key::L | Key::DPadRight, "Key"), 
			Hotkey(Key::L | Key::DPadLeft, "Key2") 
		})),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Edit Every Pattern", editpattern, "note")),
		EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "Grass Editor", grasseditor, grasscomplete, "note"), { 
			Hotkey(Key::R | Key::DPadDown, "Key"), 
			Hotkey(Key::R | Key::DPadUp, "Key2"), 
			Hotkey(Key::R | Key::DPadRight, "Key3") 
		})),
		EXTC->Append(new MenuEntry(FolderColors[10] << "Amiibo Spoofer", AmiiboSpoofer, "note")),
		EXTC->Append(EntryWithHotkey(new MenuEntry(FolderColors[10] << "Time Travel", TimeTravel, TTKeyboard, "note"), { 
			Hotkey(Key::R | Key::DPadRight, "Key"), 
			Hotkey(Key::R | Key::DPadLeft, "Key2") 
		})),
		menu->Append(EXTC);

	/////////////////////
	/*Misc Codes Folder*/
	/////////////////////
		MISC = new MenuFolder(FolderColors[11] << "Misc Codes");		
		MISC->Append(new MenuEntry(FolderColors[11] << "Change Tool Animation", nullptr, tooltype, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Change Gametype", nullptr, mgtype, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Unbreakable Flowers", nullptr, unbreakableflower, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Weather Modifier", nullptr, Weathermod , "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Always Aurora Lights", auroralights , "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Disable Non-Hacker Commands", nullptr, disablecommands, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Reload Room", nullptr, ReloadRoomCheat, "note")),
		MISC->Append(EntryWithHotkey(new MenuEntry(FolderColors[11] << "Quick Menu", QuickMenuEntry, "note"), {
			Hotkey(Key::Y | Key::DPadDown, "Key")
		})),
		MISC->Append(new MenuEntry(FolderColors[11] << "More Than 3 Number On Island",  morenumberisland, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Large FOV", fovlarge, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Move Furniture", roomSeeder, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Can Walk When Talk", walktalkentry, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Keyboard Extender", key_limit, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Custom Keyboard Symbols", CustomKeyboard, "note")),
		MISC->Append(EntryWithHotkey(new MenuEntry(FolderColors[11] << "Beans Particle Changer", BeansParticleChanger, "note"), { 
			Hotkey(Key::L | Key::DPadLeft, "Key") 
		})),
		MISC->Append(new MenuEntry(FolderColors[11] << "Always Daytime", Daytime, "note")),    
		MISC->Append(EntryWithHotkey(new MenuEntry(FolderColors[11] << "Fast Mode", fast, "note"), { 
			Hotkey(Key::R | Key::DPadDown, "Key")
		})),
		MISC->Append(new MenuEntry(FolderColors[11] << "Fast Text Speed", fasttalk, "note")),
		MISC->Append(new MenuEntry(FolderColors[11] << "Fast Game Speed", speedentry, "note")),
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
	}
}
