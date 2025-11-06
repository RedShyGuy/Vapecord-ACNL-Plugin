#include "cheats.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/ItemReader.hpp"

#include "Color.h"
#include "Files.h"

#define MAXCOUNT 25

namespace CTRPluginFramework {
	void ItemListCallBack(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();

		if(event.type == KeyboardEvent::CharacterRemoved) {
			keyboard.SetError(Language::getInstance()->get("TEXT_2_ITEM_SEARCH_ERR1"));
			return;
		}

		if(input.size() < 3) {
			keyboard.SetError(Language::getInstance()->get("TEXT_2_ITEM_SEARCH_ERR2"));
			return;
		}

		std::vector<ItemReader::Entry> itemEntries = ItemReader::getInstance()->searchAllByName(input);

		if(itemEntries.empty()) {
			keyboard.SetError(Language::getInstance()->get("TEXT_2_ITEM_SEARCH_ERR3"));
			return;
		}

		int size = itemEntries.size();
		if(size > MAXCOUNT) {
			keyboard.SetError(Utils::Format(Language::getInstance()->get("TEXT_2_ITEM_SEARCH_ERR4").c_str(), size));
			return;
		}

		std::vector<std::string> names;
		names.reserve(itemEntries.size());
		for (const auto& e : itemEntries) {
			names.push_back(e.name);
		}

		Keyboard KB(Language::getInstance()->get("TEXT_2_ITEM_SEARCH_KB"), names);
		int result = KB.Open();
		if(result < 0) {
			input.clear();
			return;
		}

		ItemReader::Entry match = itemEntries[result];
		
		input.clear();
		u8 slot = 0;
		if(!Inventory::GetNextItem({0x7FFE, 0}, slot)) {
			keyboard.Close();
			OSD::Notify("Inventory Full!",  Color::Red);
			return;
		}
			
		if(!IDList::ItemValid(match.item, false)) {
			keyboard.Close();
			OSD::Notify("Invalid Item!",  Color::Red);
			return;
		}	

		Inventory::WriteSlot(slot, match.item);
		OSD::Notify(Utils::Format("Set Item %04X to slot %d", match.item.ID, slot));
	}

//Text to Item
	void t2i(MenuEntry *entry) {
		Item val;
		ACNL_Player *player = Player::GetSaveData();

		if(entry->Hotkeys[0].IsPressed()) {
			if(!player) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			Inventory::ReadSlot(0, val);
			if(Wrap::KB<u32>(Language::getInstance()->get("ENTER_ID"), true, 8, *(u32 *)&val, *(u32 *)&val, TextItemChange)) 		
				Inventory::WriteSlot(0, val);
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {
			if(!player) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			if(Wrap::KB<u32>(Language::getInstance()->get("TEXT_2_ITEM_SET"), true, 8, *(u32 *)&val, 0x7FFE, TextItemChange)) {
				for(u16 i = 0; i < 0x10; ++i) {
					Item item = {(u16)(val.ID + i), 0};
					Inventory::WriteSlot(i, item);
				}
			}
		} 
		
		else if(entry->Hotkeys[2].IsPressed()) {
			if(!player) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				Item *item = GameHelper::GetItemAtWorldCoords(x, y);
				if(item) {
					Inventory::WriteSlot(0, *item);
					OSD::Notify(Utils::Format("Item ID: %08X", *(u32 *)item));
				}
			}
		}

		else if(entry->Hotkeys[3].IsPressed()) {
			if(!player) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			if(!ItemReader::getInstance()->isLoaded()) {
				OSD::Notify("Error: item.txt missing!", Color::Red);
				return;
			}
			std::string input = "";
			Keyboard KB(Language::getInstance()->get("TEXT_2_ITEM_SEARCH_KB2"));
			KB.OnKeyboardEvent(ItemListCallBack);
			KB.Open(input);
		}
	}
//Duplicate Items
	void duplication(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) 
			return;

		if(entry->Hotkeys[0].IsPressed()) {
			Inventory::WriteSlot(1, player->Inventory[0], player->InventoryItemLocks[0]);
		}
		else if(entry->Hotkeys[1].IsPressed()) {
			for(int i = 0; i <= 0xF; ++i) 
				Inventory::WriteSlot(i, player->Inventory[0], player->InventoryItemLocks[0]);
		}
	}

	void CatalogGetItem(u32 invData) {
		Item CurrentItem = *(Item *)(invData + 0x3B9C - 0x28);

		if(GameHelper::SetItem(&CurrentItem)) {
			std::string itemName = ItemReader::getInstance()->get(CurrentItem);
			OSD::Notify(Utils::Format("Spawned Item: %s (%04X)", itemName.c_str(), CurrentItem.ID));
		}
		else
			OSD::Notify("Inventory Full!");

		static Address argData(0x8499E4);

		static Address restoreButton(0x81825C);
		restoreButton.Call<void>(invData, *(u32 *)argData.addr, *(u32 *)(argData.addr + 4));
	}

	static bool isCatalogOpen = false;
//Catalog To Pockets
	void catalog(MenuEntry *entry) {
		static Hook catalogHook;
		static Address AllItemsBuyable(0x70E494);
		static Address cHook(0x21B4B0);

		if(entry->WasJustActivated()) {
			catalogHook.Initialize(cHook.addr, (u32)CatalogGetItem);
			catalogHook.SetFlags(USE_LR_TO_RETURN);

			catalogHook.Enable();

			Process::Patch(AllItemsBuyable.addr, 0xE3A00000);
			Process::Patch(AllItemsBuyable.addr + 4, 0xEA00000B);
		}

		if(entry->Hotkeys[0].IsPressed()) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				OSD::Notify("Player needs to be loaded!");
				return;
			}
			
		//if no menu is opened 
			if(GameHelper::BaseInvPointer() == 0) {	
				GameHelper::Catalog();
				return;
			}	
		}
		
		if(Inventory::GetCurrent() == 0x7C && !isCatalogOpen) 
			isCatalogOpen = true;
		
		if(Inventory::GetCurrent() != 0x7C && isCatalogOpen) {
			Animation::Idle();
			isCatalogOpen = false;
		}
		
		if(!entry->IsActivated()) {
			catalogHook.Disable();
			Process::Patch(AllItemsBuyable.addr, 0x03A00001);
			Process::Patch(AllItemsBuyable.addr + 4, 0x0A00000B);

			isCatalogOpen = false;
		}
	}
//Chat Text2Item
	void chatt2i(MenuEntry *entry) {
		if(!entry->Hotkeys[0].IsPressed())
			return;
		
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			OSD::Notify("Player needs to be loaded!", Color::Red);
			return;
		}
		
		if(!GameKeyboard::IsOpen()) {
			OSD::Notify("Open your Keyboard!", Color::Red);
			return;
		}

		if(GameKeyboard::IsEmpty()) {
			OSD::Notify("Keyboard is empty!", Color::Red);
			return;
		}
		
		std::string chatStr = "";
		Item itemID;

		if(!GameKeyboard::Copy(chatStr, 0, 0x16)) {
			OSD::Notify("Somehow couldn't copy!", Color::Red);
			return;
		}

		if(!GameKeyboard::ConvertToItemID(chatStr, itemID)) {
			OSD::Notify("Invalid Character!", Color::Red);
			return;
		}
		
		u8 slot = 0;
		if(!Inventory::GetNextItem({0x7FFE, 0}, slot)) {
			OSD::Notify("Inventory Full!", Color::Red);
			return;
		}
			
		if(!IDList::ItemValid(itemID, false)) {
			OSD::Notify("Invalid Item!", Color::Red);
			return;
		}
		
		Inventory::WriteSlot(slot, itemID);

		std::string itemName = ItemReader::getInstance()->get(itemID);
		OSD::Notify(Utils::Format("Spawned Item: %s (%08X)", itemName.c_str(), itemID));
	}
//Clear Inventory
	void ClearInventory(MenuEntry *entry) {
		if(!Player::GetSaveData()) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		if((MessageBox(Language::getInstance()->get("REMOVE_INV_WARNING"), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			for(int i = 0; i <= 0xF; ++i)
				Inventory::WriteSlot(i, Item{ 0x7FFE, 0 });
		}
	}

//Item Settings	
	void itemsettings(MenuEntry *entry) {
		static const Address showoff(0x19BA78);
		static const Address infinite1(0x19C574);
		static const Address infinite2(0x19C4D0);
		static const Address eat(0x19C1F0);
		
		std::vector<std::string> itemsettopt = {
			Language::getInstance()->get("VECTOR_ITEMSETTINGS_SHOWOFF"),
			Language::getInstance()->get("VECTOR_ITEMSETTINGS_INFINITE"),
			Language::getInstance()->get("VECTOR_ITEMSETTINGS_EAT"),
		};
		
		static const u32 settings[3] = {
			showoff.addr, infinite1.addr, eat.addr
		};
		
		static constexpr u32 settingsvalue[2][3] = {
			{ 0xE1A00000, 0xE2805A00, 0xE1A00000 },
			{ 0x1A000012, 0xE2805A06, 0x0A000009 },
		};

		bool IsON;
		
		for(int i = 0; i < 3; ++i) { 
			IsON = *(u32 *)settings[i] == settingsvalue[0][i];
			itemsettopt[i] = IsON ? (Color(pGreen) << itemsettopt[i]) : (Color(pRed) << itemsettopt[i]);
		}
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), itemsettopt);

		int op = optKb.Open();
		if(op < 0)
			return;
			
		if(op == 1) {
			Process::Patch(infinite1.addr, *(u32 *)infinite1.addr == 0xE2805A06 ? 0xE2805A00 : 0xE2805A06);
			Process::Patch(infinite2.addr, *(u32 *)infinite2.addr == 0xE2805A06 ? 0xE2805A00 : 0xE2805A06);
			itemsettings(entry);
			return;
		}
			
		Process::Patch(settings[op], *(u32 *)settings[op] == settingsvalue[0][op] ? settingsvalue[1][op] : settingsvalue[0][op]);
		itemsettings(entry);
	}

	static bool IsMenuPatchOpen = false;
	static u8 CurrentMenu = 0xFF;
	const u8 Menus[8] = { 0x2E, 0x37, 0x38, 0x3D, 0x79, 0x89, 0x00, 0xFF };

	void Callback_MenuPatch(void) {
		if(Inventory::GetCurrent() == CurrentMenu && !IsMenuPatchOpen) 
			IsMenuPatchOpen = true;
		
		if(Inventory::GetCurrent() != CurrentMenu && IsMenuPatchOpen) {
			Animation::Idle();
			IsMenuPatchOpen = false;
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu -= Callback_MenuPatch; //delete itself
		}
	}

	void Hook_MenuPatch(void) {
		GameHelper::OpenMenu(CurrentMenu);
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		*menu += Callback_MenuPatch;
	}
	
//Menu Changer
	void MenuChanger(MenuEntry *entry) {
		static Hook hook;
		
		std::vector<std::string> menuopt = {
			Language::getInstance()->get("VECTOR_SAVEMENU_DATETIME"),
			Language::getInstance()->get("VECTOR_SAVEMENU_BELLPOINT_DEPO"),
			Language::getInstance()->get("VECTOR_SAVEMENU_BELLPOINT_WITHDRAW"),
			Language::getInstance()->get("VECTOR_SAVEMENU_LOCKER"),
			Language::getInstance()->get("VECTOR_SAVEMENU_TOWNTUNE"),
			Language::getInstance()->get("VECTOR_SAVEMENU_HOUSESTORAGE"),
			Language::getInstance()->get("VECTOR_SAVEMENU_CUSTOM"),
			Language::getInstance()->get("VECTOR_DISABLE")
		};

		bool IsON;
		
		for(int i = 0; i < 6; ++i) { 
			IsON = CurrentMenu == Menus[i];
			menuopt[i] = (IsON ? Color(pGreen) :  Color(pRed)) << menuopt[i];
		}
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), menuopt);

		int dChoice = optKb.Open();
		if(dChoice < 0)
			return;

		hook.Initialize(Address(0x1A0980).addr + 8, (u32)Hook_MenuPatch);
		hook.SetFlags(USE_LR_TO_RETURN);

	//If Custom Menu is chosen
		if(dChoice == 6) {
			if(Wrap::KB<u8>(Language::getInstance()->get("SAVE_MENU_CHANGER_ENTER_ID"), true, 2, CurrentMenu, 0)) {
				if(IDList::MenuValid(CurrentMenu))
					hook.Enable();
				else {
					hook.Disable();
					MessageBox(Language::getInstance()->get("INVALID_ID")).SetClear(ClearScreen::Top)();
				}
			}
			return;
		}
		
		CurrentMenu = Menus[dChoice];
		if(CurrentMenu == 0xFF) {
			hook.Disable();
			return;
		}
		hook.Enable();
		MenuChanger(entry);
	}

	void GetCustomView(Keyboard& keyboard, KeyboardEvent& event) {
		if(event.type != KeyboardEvent::SelectionChanged)
            return;

		int index = event.selectedIndex;

		std::vector<std::string> f_file, f_Dir, f_all;
		std::vector<bool> isDir;
		File file;

		if(Wrap::restoreDIR.ListDirectories(f_Dir) == Directory::OPResult::NOT_OPEN)
			return;

		if(Wrap::restoreDIR.ListFiles(f_file, ".inv") == Directory::OPResult::NOT_OPEN) 
			return;

		if(f_Dir.empty() && f_file.empty())
			return;

		for(const std::string& str : f_Dir) {
			f_all.push_back(str);
			isDir.push_back(true);
		}

		for(const std::string& str : f_file) {
			f_all.push_back(str);
			isDir.push_back(false);
		}

		if(index == -1)
			return;

		std::string& input = keyboard.GetMessage();
		input.clear();

	//if directory return
		if(isDir[index])
			return;

		if(Wrap::restoreDIR.OpenFile(file, f_all[index], File::READ) != 0) 
			return; //error opening file

		std::string Sets[16];
		Item SetItem[16];
		std::vector<Item> OnlyItem;
		file.Read(&SetItem, sizeof(SetItem));

		for(int i = 0; i < 16; ++i) {
			if(SetItem[i].ID != 0x7FFE)
				OnlyItem.push_back(SetItem[i]);
		}

		for(int i = 0; i < 11; ++i) {
			if(i >= OnlyItem.size())
				return;

			std::string str = ItemReader::getInstance()->get(OnlyItem[i]);
			Sets[i] = str;

			input += Color(0x40FF40FF) << Utils::Format("%08X | ", OnlyItem[i]) << Color(0xFFFDD0FF) << Sets[i] << "\n";
		}
		input += "etc...";
		file.Flush();
		file.Close();
	}
//Get Set
	void getset(MenuEntry *entry) { 
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
	
		static const std::vector<std::string> setopt = {
			Language::getInstance()->get("VECTOR_GETSET_FURN"),
			Language::getInstance()->get("VECTOR_GETSET_CUSTOM"),
		};
	
		static const std::vector<std::string> custinvopt = {
			Language::getInstance()->get("VECTOR_GETSET_CUSTOM_BACKUP"),
			Language::getInstance()->get("VECTOR_GETSET_CUSTOM_RESTORE"),
			Language::getInstance()->get("FILE_DELETE"),  
		};

		WrapLoc LocInv = { (u32 *)player->Inventory, sizeof(player->Inventory) };
		WrapLoc LocLock = { (u32 *)player->InventoryItemLocks, sizeof(player->InventoryItemLocks) };

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), setopt);

		switch(optKb.Open()) {
			default: return;
			case 0: {
				Wrap::Restore(PATH_PRESET, ".inv", Language::getInstance()->get("GET_SET_RESTORE"), GetCustomView, false, &LocInv, &LocLock, nullptr); 
				Inventory::ReloadIcons();
			} return;

			case 1: {
				optKb.Populate(custinvopt);

				switch(optKb.Open()) {
					default: return;
					case 0: {
						std::string filename = "";
						Keyboard KB(Language::getInstance()->get("GET_SET_DUMP"));
						if(KB.Open(filename) == -1)
							return;

						Wrap::Dump(Utils::Format(PATH_ITEMSET, Address::regionName.c_str()), filename, ".inv", &LocInv, &LocLock, nullptr);
					} return;
					
					case 1: {			
						Wrap::Restore(Utils::Format(PATH_ITEMSET, Address::regionName.c_str()), ".inv", Language::getInstance()->get("GET_SET_RESTORE"), GetCustomView, true, &LocInv, &LocLock, nullptr); 
						Inventory::ReloadIcons();
					} return;
					
					case 2: 
						Wrap::Delete(Utils::Format(PATH_ITEMSET, Address::regionName.c_str()), ".inv");
					return;
				}
			}
		}
	}
}
