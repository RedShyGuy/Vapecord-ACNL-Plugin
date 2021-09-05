#include <CTRPluginFramework.hpp>
#include <fstream>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"
#include "MenuPointers.hpp"

#define MAXCOUNT 25

namespace CTRPluginFramework {
	void TextItemChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		u32 ID = StringToHex<u32>(input, 0xFFFF);

		if(!IDList::ItemValid((ID & 0xFFFFFFFF), false)) {
			keyboard.GetMessage() = "";
			keyboard.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}

		keyboard.GetMessage() = ItemIDSearch(ID & 0xFFFF);
	}

	Item* ItemList = new Item();
//reserver data into pointer so search doesnt take so long
	void ReserveItemData(Item* out) {
		File file(ITEMLIST, File::READ);
		if(!file.IsOpen()) {
			ItemFileExists = false;
			return;
		}

		std::string line;
		LineReader reader(file);

		u32 lineNumber = 0;
		int count = 0;

	//Read all lines in file
		for(; reader(line); lineNumber++) {
		//If line is empty, skip it
			if(line.empty())
				continue;

			std::string lowcaseInput(line);
			for(char& c : lowcaseInput)
				c = std::tolower(c);

			std::string Name = lowcaseInput.substr(5, 30); //lets make max 30 for now
			std::string SID = lowcaseInput.substr(0, 4); 
			u16 ID = StringToHex<u16>(SID, 0xFFFF);
			out->Name.push_back(Name);
			out->ID.push_back(ID);
			ItemFileLenght++; //adds to file lenght to know how many items are in it
		}
	}

	int ItemSearch(const std::string& match, Item& out) {
		int count = 0;
	//Read our file until the last line
		for(int i = 0; i < ItemFileLenght; ++i) {
			auto namePos = ItemList->Name[i].find(match);
			if(namePos != std::string::npos) {
				out.Name.push_back(ItemList->Name[i]);
				out.ID.push_back(ItemList->ID[i]);
				count++;
			}
		}

		return count;
	}

	std::string ItemIDSearch(u16 ItemID) {
		if(!ItemFileExists)
			return "";

	//Read our file until the last line
		for(int i = 0; i < ItemFileLenght; ++i) {
			if(ItemList->ID[i] == ItemID) {
				return ItemList->Name[i];
			}
		}

		return "???";
	}

	void ItemListCallBack(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();

		std::string lowcaseInput(input);
		for(char& c : lowcaseInput)
			c = std::tolower(c);

		if(event.type == KeyboardEvent::CharacterRemoved) {
			keyboard.SetError(Language->Get("TEXT_2_ITEM_SEARCH_ERR1"));
			return;
		}

		if(lowcaseInput.size() < 3) {
			keyboard.SetError(Language->Get("TEXT_2_ITEM_SEARCH_ERR2"));
			return;
		}

		Item match;
		int res = ItemSearch(lowcaseInput, match);

		if(res == 0) {
			keyboard.SetError(Language->Get("TEXT_2_ITEM_SEARCH_ERR3"));
			return;
		}

		if(res > MAXCOUNT) {
			keyboard.SetError(Utils::Format(Language->Get("TEXT_2_ITEM_SEARCH_ERR4").c_str(), res));
			return;
		}

		Keyboard KB(Language->Get("TEXT_2_ITEM_SEARCH_KB"), match.Name);
		s8 kres = KB.Open();
		if(kres < 0) {
			input.clear();
			return;
		}
		
		input.clear();
		u8 slot = 0;
		if(Inventory::GetNextItem(0x7FFE, slot) == 0xFFFFFFFF) {
			keyboard.Close();
			OSD::Notify("Inventory Full!",  Color::Red);
			return;
		}
			
		if(!IDList::ItemValid(match.ID[kres], false)) {
			keyboard.Close();
			OSD::Notify("Invalid Item!",  Color::Red);
			return;
		}	
		Inventory::WriteSlot(slot, match.ID[kres]);
		OSD::Notify(Utils::Format("Set Item %4x to slot %d", match.ID[kres], slot));
	}

//Text to Item
	void t2i(MenuEntry *entry) {
		static u32 val = 0;
		if(entry->Hotkeys[0].IsPressed()) {
			if(Player::GetSaveOffset(4) == 0) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			Inventory::ReadSlot(0, val);
			if(Wrap::KB<u32>(Language->Get("ENTER_ID"), true, 8, val, val, TextItemChange)) 		
				Inventory::WriteSlot(0, val);
		}
		
		else if(entry->Hotkeys[1].IsPressed()) {
			if(Player::GetSaveOffset(4) == 0) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			if(Wrap::KB<u32>(Language->Get("TEXT_2_ITEM_SET"), true, 8, val, 0x7FFE, TextItemChange)) {
				for(int i = 0; i < 0x10; ++i) 
					Inventory::WriteSlot(i, val + i);
			}
		} 
		
		else if(entry->Hotkeys[2].IsPressed()) {
			if(Player::GetSaveOffset(4) == 0) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				val = (u32)GameHelper::GetItemAtWorldCoords(x, y);
				if(val != 0) {
					Inventory::WriteSlot(0, *(u32 *)val);
					OSD::Notify(Utils::Format("Item ID: %08X (%08X)", *(u32 *)val, val));
				}
			}
		}

		else if(entry->Hotkeys[3].IsPressed()) {
			if(Player::GetSaveOffset(4) == 0) {
				OSD::Notify("Error: Player needs to be loaded!", Color::Red);
				return;
			}

			if(!ItemFileExists) {
				OSD::Notify("Error: item.txt missing!", Color::Red);
				return;
			}
			std::string input;
			Keyboard KB(Language->Get("TEXT_2_ITEM_SEARCH_KB2"));
			KB.OnKeyboardEvent(ItemListCallBack);
			KB.Open(input);
		}
	}
//Duplicate Items
	void duplication(MenuEntry *entry) {	
		u32 dupe = 0x7FFE;
		if(entry->Hotkeys[0].IsPressed()) {
			Inventory::ReadSlot(0, dupe);
			Inventory::WriteSlot(1, dupe, Inventory::GetLock(0));
		}
		else if(entry->Hotkeys[1].IsPressed()) {
			Inventory::ReadSlot(0, dupe);
			for(int i = 0; i <= 0xF; ++i) 
				Inventory::WriteSlot(i, dupe, Inventory::GetLock(0));
		}
	}

	static bool catact = true;
//OSD for the Catalog To Pockets
	bool catalogosd(const Screen &screen) {
		if(screen.IsTop) {			
			screen.Draw("1. Press L + DPadRight to open catalog", 0, 0);
			screen.Draw("2. Search for wanted item", 0, 10);
			screen.Draw("3. Press L + DPadRight to get item", 0, 20);
			screen.Draw("(To remove this note press L + Y)", 0, 30);		
		}
		return 1;
	}
	
	u16 GetCurrentCatalogItem() {
		if(GameHelper::BaseInvPointer() == 0) 
			return 0x7FFE;
		
		return *(u16 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x3B9C);
	}
	static bool isCatalogOpen = false;
//Catalog To Pockets
	void catalog(MenuEntry *entry) {
		if(entry->WasJustActivated()) 
			OSD::Run(catalogosd);
		
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
		//if catalog is opened get item
			if(Inventory::GetCurrent() == 0x7C) {
				u32 CurrentItem = GetCurrentCatalogItem();
				if(CurrentItem != 0x7FFE) {
					if(GameHelper::SetItem(&CurrentItem))
						OSD::Notify(Utils::Format("Spawned Item: %04X", CurrentItem));
					else
						OSD::Notify("Inventory Full!");
				}
			}
		}
		
		if(Inventory::GetCurrent() == 0x7C && !isCatalogOpen) 
			isCatalogOpen = true;
		
		if(Inventory::GetCurrent() != 0x7C && isCatalogOpen) {
			Animation::Idle();
			isCatalogOpen = false;
		}
		
		if(entry->Hotkeys[1].IsPressed()) {
			if(catact) {
				OSD::Stop(catalogosd);
				catact = false;
				return;
			}

			OSD::Run(catalogosd); 
			catact = true;
		}
		
		if(!entry->IsActivated()) {
			isCatalogOpen = false;
			OSD::Stop(catalogosd);
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
		u32 itemID = 0;

		if(!GameKeyboard::Copy(chatStr, 0, 0x16)) {
			OSD::Notify("Somehow couldn't copy!", Color::Red);
			return;
		}

		if(!GameKeyboard::ConvertToItemID(chatStr, itemID)) {
			OSD::Notify("Invalid Character!", Color::Red);
			return;
		}
		
		u8 slot = 0;
		if(Inventory::GetNextItem(0x7FFE, slot) == 0xFFFFFFFF) {
			OSD::Notify("Inventory Full!", Color::Red);
			return;
		}
			
		if(!IDList::ItemValid(itemID, false)) {
			OSD::Notify("Invalid Item!", Color::Red);
			return;
		}
		
		Inventory::WriteSlot(slot, itemID);		
		OSD::Notify(Utils::Format("Spawned Item: %08X", itemID));
	}
//Item Settings	
	void itemsettings(MenuEntry *entry) {
		static const u32 showoff = Region::AutoRegion(0x19BA78, 0x19B4C0, 0x19BA98, 0x19BA98, 0x19B9D8, 0x19B9D8, 0x19B9D8, 0x19B9D8);
		static const u32 infinite1 = Region::AutoRegion(0x19C574, 0x19BFBC, 0x19C594, 0x19C594, 0x19C4D4, 0x19C4D4, 0x19C4D4, 0x19C4D4);
		static const u32 infinite2 = Region::AutoRegion(0x19C4D0, 0x19BF18, 0x19C4F0, 0x19C4F0, 0x19C430, 0x19C430, 0x19C430, 0x19C430);
		static const u32 eat = Region::AutoRegion(0x19C1F0, 0x19BC38, 0x19C210, 0x19C210, 0x19C150, 0x19C150, 0x19C150, 0x19C150);
		
		std::vector<std::string> itemsettopt = {
			Language->Get("VECTOR_ITEMSETTINGS_SHOWOFF"),
			Language->Get("VECTOR_ITEMSETTINGS_INFINITE"),
			Language->Get("VECTOR_ITEMSETTINGS_EAT"),
		};
		
		static const u32 settings[3] = {
			showoff, infinite1, eat
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
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(itemsettopt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
			
		if(op == 1) {
			Process::Patch(infinite1, *(u32 *)infinite1 == 0xE2805A06 ? 0xE2805A00 : 0xE2805A06);
			Process::Patch(infinite2, *(u32 *)infinite2 == 0xE2805A06 ? 0xE2805A00 : 0xE2805A06);
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
			Language->Get("VECTOR_SAVEMENU_DATETIME"),
			Language->Get("VECTOR_SAVEMENU_BELLPOINT_DEPO"),
			Language->Get("VECTOR_SAVEMENU_BELLPOINT_WITHDRAW"),
			Language->Get("VECTOR_SAVEMENU_LOCKER"),
			Language->Get("VECTOR_SAVEMENU_TOWNTUNE"),
			Language->Get("VECTOR_SAVEMENU_HOUSESTORAGE"),
			Language->Get("VECTOR_SAVEMENU_CUSTOM"),
			Language->Get("VECTOR_DISABLE")
		};

		bool IsON;
		
		for(int i = 0; i < 6; ++i) { 
			IsON = CurrentMenu == Menus[i];
			menuopt[i] = (IsON ? Color(pGreen) :  Color(pRed)) << menuopt[i];
		}
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(menuopt);
		Sleep(Milliseconds(100));
		s8 dChoice = optKb.Open();
		
		if(dChoice < 0)
			return;

		hook.Initialize(Code::nosave + 8, (u32)Hook_MenuPatch);
		hook.SetFlags(USE_LR_TO_RETURN);

	//If Custom Menu is chosen
		if(dChoice == 6) {
			if(Wrap::KB<u8>(Language->Get("SAVE_MENU_CHANGER_ENTER_ID"), true, 2, CurrentMenu, 0)) {
				if(IDList::MenuValid(CurrentMenu))
					hook.Enable();
				else {
					hook.Disable();
					MessageBox(Language->Get("INVALID_ID")).SetClear(ClearScreen::Top)();
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

	std::string path = "";
	std::string type = "";
	int lenght = -1;

	void GetCustomView(Keyboard& keyboard, KeyboardEvent& event) {
		if(event.type != KeyboardEvent::SelectionChanged)
            return;

		int index = event.selectedIndex;

		Directory dir(path);
		std::vector<std::string> f_list;
		File file;

		if(dir.ListFiles(f_list, type) == Directory::OPResult::NOT_OPEN)
			return;

		if(index == -1)
			return;

		if(dir.OpenFile(file, f_list[index], File::READ) != 0) 
			return; //error opening file

		std::string& input = keyboard.GetMessage();
		input.clear();

		std::string Sets[16];
		u32 SetItem[16] = { 0 };
		std::vector<u32> OnlyItem;
		file.Read(&SetItem, sizeof(SetItem));

		for(int i = 0; i < 16; ++i) {
			if(SetItem[i] != 0x7FFE)
				OnlyItem.push_back(SetItem[i]);
		}

		for(int i = 0; i < 11; ++i) {
			if(i >= OnlyItem.size())
				return;

			Sets[i] = IDList::GetSeedName(OnlyItem[i]);
			input += Color(0x40FF40FF) << Utils::Format("%4X | ", OnlyItem[i]) << Color(0xFFFDD0FF) << Sets[i] << "\n";
		}
		input += "etc...";
		file.Flush();
		file.Close();
	}
//Get Set	
	void getset(MenuEntry *entry) { 
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
	
		static const std::vector<std::string> setopt = {
			Language->Get("VECTOR_GETSET_FURN"),
			Language->Get("VECTOR_GETSET_ITEM"),
			Language->Get("VECTOR_GETSET_CUSTOM"),
		};
	
		static const std::vector<std::string> custinvopt = {
			Language->Get("VECTOR_GETSET_CUSTOM_BACKUP"),
			Language->Get("VECTOR_GETSET_CUSTOM_RESTORE"),
			Language->Get("FILE_DELETE"),  
		};

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(setopt);
		Sleep(Milliseconds(100));
		switch(optKb.Open()) {
			default: return;
			case 0: {
				path = PATH_PRESET;
				type = ".preSF";
				Wrap::Restore(path, type, Language->Get("GET_SET_RESTORE"), GetCustomView, false, WrapLoc{ PlayerPTR::Pointer(0x6BD0), 0x64 }, WrapLoc{ PlayerPTR::Pointer(0x6C10), 0x10 }, WrapLoc{ (u32)-1, (u32)-1 }); 
				Inventory::ReloadIcons();
			} return;

			case 1: {
				path = PATH_PRESET;
				type = ".preSI";
				Wrap::Restore(path, type, Language->Get("GET_SET_RESTORE"), GetCustomView, false, WrapLoc{ PlayerPTR::Pointer(0x6BD0), 0x64 }, WrapLoc{ PlayerPTR::Pointer(0x6C10), 0x10 }, WrapLoc{ (u32)-1, (u32)-1 }); 
				Inventory::ReloadIcons();
			} return;

			case 2: {
				optKb.Populate(custinvopt);
				switch(optKb.Open()) {
					default: return;
					case 0: {
						std::string filename = "";
						Keyboard KB(Language->Get("GET_SET_DUMP"));
						if(KB.Open(filename) == -1)
							return;

						Wrap::Dump(PATH_ITEMSET, filename, ".inv", WrapLoc{ PlayerPTR::Pointer(0x6BD0), 0x64 }, WrapLoc{ PlayerPTR::Pointer(0x6C10), 0x10 }, WrapLoc{ (u32)-1, (u32)-1 });
					} return;
					
					case 1: {			
						path = PATH_ITEMSET;
						type = ".inv";
						Wrap::Restore(path, type, Language->Get("GET_SET_RESTORE"), GetCustomView, true, WrapLoc{ PlayerPTR::Pointer(0x6BD0), 0x64 }, WrapLoc{ PlayerPTR::Pointer(0x6C10), 0x10 }, WrapLoc{ (u32)-1, (u32)-1 }); 
						Inventory::ReloadIcons();
					} return;
					
					case 2: 
						Wrap::Delete(PATH_ITEMSET, ".inv");
					return;
				}
			}
		}
	}
}
