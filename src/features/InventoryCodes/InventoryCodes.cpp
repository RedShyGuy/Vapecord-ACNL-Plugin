#include "features/cheats.hpp"
#include "core/game_api/Game.hpp"
#include "core/checks/IDChecks.hpp"
#include "core/game_api/Inventory.hpp"
#include "core/game_api/PlayerClass.hpp"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/game_api/Player.hpp"
#include "core/game_api/GameKeyboard.hpp"
#include "core/game_api/Animation.hpp"
#include "core/RuntimeContext.hpp"
#include "core/HUD.hpp"

#include "Color.h"
#include "Files.h"

#define MAXCOUNT 25

namespace CTRPluginFramework {
	void itemsearch(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red).SetClear(ClearScreen::Top)();
			return;
		}

		std::string input = "";
		Keyboard KB(Language::getInstance()->get(TextID::TEXT_2_ITEM_SEARCH_KB2));
		if (KB.Open(input) < 0) {
			return;
		}

		std::vector<ItemNamePack> itemEntries = std::vector<ItemNamePack>();

		if(!Item::searchAllByKeyword(input, itemEntries)) {
			MessageBox(Language::getInstance()->get(TextID::TEXT_2_ITEM_SEARCH_ERR3), Color::Red).SetClear(ClearScreen::Top)();
			return;
		}

		const int resultCount = static_cast<int>(itemEntries.size());
		const int pageSize = MAXCOUNT;
		const int totalPages = (resultCount + pageSize - 1) / pageSize;
		int currentPage = 0;
		int selectedIndex = -1;

		while(true) {
			const int pageStart = currentPage * pageSize;
			const int pageEnd = std::min(pageStart + pageSize, resultCount);
			std::vector<std::string> pageOptions;
			const bool hasPreviousPage = currentPage > 0;
			const bool hasNextPage = currentPage + 1 < totalPages;

			if(hasPreviousPage) {
				pageOptions.emplace_back(Color(0x5AA9FFFF) << Language::getInstance()->get(TextID::TEXT_2_ITEM_SEARCH_PREVIOUS_PAGE));
			}

			for(int index = pageStart; index < pageEnd; ++index) {
				pageOptions.push_back(itemEntries[index].name);
			}

			if(hasNextPage) {
				pageOptions.emplace_back(Color(0xFFC857FF) << Language::getInstance()->get(TextID::TEXT_2_ITEM_SEARCH_NEXT_PAGE));
			}

			std::string keyboardTitle = Utils::Format(
				Language::getInstance()->get(TextID::TEXT_2_ITEM_SEARCH_PAGE_TITLE).c_str(),
				currentPage + 1,
				totalPages
			);

			Keyboard selectKB(keyboardTitle, pageOptions);
			const int pageChoice = selectKB.Open();
			if(pageChoice < 0) {
				return;
			}

			if(hasPreviousPage && pageChoice == 0) {
				--currentPage;
				continue;
			}

			if(hasNextPage && pageChoice == static_cast<int>(pageOptions.size()) - 1) {
				++currentPage;
				continue;
			}

			selectedIndex = pageStart + pageChoice - (hasPreviousPage ? 1 : 0);
			break;
		}

		Item item(itemEntries[selectedIndex].ID);

		u8 slot = 0;
		if(!Inventory::GetNextItem({0x7FFE, 0}, slot)) {
			MessageBox(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INV_FULL),  Color::Red).SetClear(ClearScreen::Top)();
			return;
		}

		if(!item.isValid(false)) {
			MessageBox(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INVALID),  Color::Red).SetClear(ClearScreen::Top)();
			return;
		}

		Inventory::WriteSlot(slot, item);
		MessageBox(Utils::Format(Language::getInstance()->get(TextID::INVENTORY_SEARCH_SET).c_str(), item.ID, slot + 1)).SetClear(ClearScreen::Top)();
	}

//Text to Item
	void t2i(MenuEntry *entry) {
		Item val;
		ACNL_Player *player = Player::GetSaveData();

		if(entry->Hotkeys[0].IsPressed()) {
			if(!player) {
				HUD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
				return;
			}

			u8 slot = 0;
			if(!Inventory::GetNextItem({0x7FFE, 0}, slot)) {
				HUD::Notify(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INV_FULL), Color::Red);
				return;
			}

			if(PluginUtils::Input::PromptNumber<u32>({ Language::getInstance()->get(TextID::ENTER_ID), true, 8, *(u32 *)&val, TextItemChange }, *(u32 *)&val)) {
				Inventory::WriteSlot(slot, val);
			}
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			if(!player) {
				HUD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
				return;
			}

			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				Item *item = Game::GetItemAtWorldCoords(x, y);
				if(item) {
					u8 slot = 0;
					if(!Inventory::GetNextItem({0x7FFE, 0}, slot)) {
						HUD::Notify(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INV_FULL), Color::Red);
						return;
					}

					Inventory::WriteSlot(slot, *item);
					HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::INVENTORY_T2I_SET).c_str(), *(u32 *)item));
				}
			}
		}
	}

//Duplicate Items
	void duplication(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return;
		}

		if(entry->Hotkeys[0].IsPressed()) {
			Inventory::WriteSlot(1, player->Inventory[0], player->InventoryItemLocks[0]);
		}

		else if(entry->Hotkeys[1].IsPressed()) {
			for(int i = 0; i <= 0xF; ++i) {
				Inventory::WriteSlot(i, player->Inventory[0], player->InventoryItemLocks[0]);
			}
		}
	}

	void CatalogGetItem(u32 invData, u32 u0, u32 u1) {
		static Address getCurrentCatalogItem(0x692FD0);

		u32 catalogItemPtr = getCurrentCatalogItem.Call<u32>();
		Item CurrentItem = *(Item *)(catalogItemPtr + 4);

		if(Game::SetItem(&CurrentItem)) {
			std::string itemName = CurrentItem.GetName();
			HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::INVENTORY_CATALOG_SET_ITEM).c_str(), itemName.c_str(), CurrentItem.ID));
		}
		else {
			HUD::Notify(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INV_FULL), Color::Red);
		}

		static Address argData(0x8499E4);

		const HookContext &curr = HookContext::GetCurrent();
		static Address func = Address::decodeARMBranch(curr.targetAddress, curr.overwrittenInstr);
		func.Call<void>(invData, *(u32 *)argData.addr, *(u32 *)(argData.addr + 4));
	}

	static bool isCatalogOpen = false;
//Catalog To Pockets
	void catalog(MenuEntry *entry) {
		static Hook catalogHook;

		static Address AllItemsBuyable(0x21B3AC);
		static Address AllItemsHavePrices(0x21AB60);

		static Address cHook(0x21B4B0);

		if(entry->WasJustActivated()) {
			catalogHook.Initialize(cHook.addr, (u32)CatalogGetItem);
			catalogHook.SetFlags(USE_LR_TO_RETURN);

			catalogHook.Enable();

			AllItemsBuyable.Patch(0xE3A00000);
			AllItemsHavePrices.Patch(0xE3A00000);
		}

		if(entry->Hotkeys[0].IsPressed()) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				HUD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
				return;
			}

		//if no menu is opened
			if(Game::BaseInvPointer() == 0) {
				Game::Catalog();
				return;
			}
		}

		if(Inventory::GetCurrent() == 0x7C && !isCatalogOpen) {
			isCatalogOpen = true;
		}

		if(Inventory::GetCurrent() != 0x7C && isCatalogOpen) {
			Animation::Idle();
			isCatalogOpen = false;
		}

		if(!entry->IsActivated()) {
			catalogHook.Disable();

			AllItemsBuyable.Unpatch();
			AllItemsHavePrices.Unpatch();

			isCatalogOpen = false;
		}
	}
//Chat Text2Item
	void chatt2i(MenuEntry *entry) {
		if(!entry->Hotkeys[0].IsPressed()) {
			return;
		}

		if(!PlayerClass::GetInstance()->IsLoaded()) {
			HUD::Notify(Language::getInstance()->get(TextID::SAVE_PLAYER_NO), Color::Red);
			return;
		}

		if(!GameKeyboard::IsOpen()) {
			HUD::Notify(Language::getInstance()->get(TextID::CHAT_TEXT_2_I_OPEN_KEYBOARD), Color::Red);
			return;
		}

		if(GameKeyboard::IsEmpty()) {
			HUD::Notify(Language::getInstance()->get(TextID::CHAT_TEXT_2_I_KEYBOARD_EMPTY), Color::Red);
			return;
		}

		std::string chatStr = "";
		Item itemID;

		if(!GameKeyboard::Copy(chatStr, 0, 0x16)) {
			HUD::Notify(Language::getInstance()->get(TextID::CHAT_TEXT_2_I_COPY_ERROR), Color::Red);
			return;
		}

		if(!GameKeyboard::ConvertToItemID(chatStr, itemID)) {
			HUD::Notify(Language::getInstance()->get(TextID::CHAT_TEXT_2_I_INVALID), Color::Red);
			return;
		}

		u8 slot = 0;
		if(!Inventory::GetNextItem({0x7FFE, 0}, slot)) {
			HUD::Notify(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INV_FULL), Color::Red);
			return;
		}

		if(!itemID.isValid(false)) {
			HUD::Notify(Language::getInstance()->get(TextID::INVENTORY_SEARCH_INVALID), Color::Red);
			return;
		}

		Inventory::WriteSlot(slot, itemID);

		std::string itemName = itemID.GetName();
		HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::CHAT_TEXT_2_I_SPAWNED).c_str(), itemName.c_str(), itemID));
	}
//Clear Inventory
	void ClearInventory(MenuEntry *entry) {
		if(!Player::GetSaveData()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		if((MessageBox(Language::getInstance()->get(TextID::REMOVE_INV_WARNING), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			for(int i = 0; i <= 0xF; ++i) {
				Inventory::WriteSlot(i, Item{ 0x7FFE, 0 });
			}
		}
	}

//Item Settings
	static void ApplyItemSettingsMask(u32 mask) {
		static Address showoff(0x19BA78);
		static Address infinite1(0x19C574);
		static Address infinite2(0x19C4D0);
		static Address eat(0x19C1F0);

		if(mask & 0x1) {
			showoff.Patch(0xE1A00000);
		}
		else {
			showoff.Unpatch();
		}

		if(mask & 0x2) {
			infinite1.Patch(0xE2805A00);
			infinite2.Patch(0xE2805A00);
		}
		else {
			infinite1.Unpatch();
			infinite2.Unpatch();
		}

		if(mask & 0x4) {
			eat.Patch(0xE1A00000);
		}
		else {
			eat.Unpatch();
		}
	}

	static u32 GetItemSettingsMask(void) {
		static Address showoff(0x19BA78);
		static Address infinite1(0x19C574);
		static Address eat(0x19C1F0);

		u32 mask = 0;
		if(*(u32 *)showoff.addr != showoff.origVal) {
			mask |= 0x1;
		}
		if(*(u32 *)infinite1.addr != infinite1.origVal) {
			mask |= 0x2;
		}
		if(*(u32 *)eat.addr != eat.origVal) {
			mask |= 0x4;
		}

		return mask;
	}

	void ItemSettingsApplySaved(MenuEntry *entry, u32 savedValue) {
		(void)entry;
		ApplyItemSettingsMask(savedValue & 0x7);
	}

	void itemsettings(MenuEntry *entry) {
		while(true) {
			u32 currentMask = GetItemSettingsMask();

			std::vector<std::string> itemsettopt = {
				Language::getInstance()->get(TextID::VECTOR_ITEMSETTINGS_SHOWOFF),
				Language::getInstance()->get(TextID::VECTOR_ITEMSETTINGS_INFINITE),
				Language::getInstance()->get(TextID::VECTOR_ITEMSETTINGS_EAT),
			};

			itemsettopt[0] = ((currentMask & 0x1) ? Color(pGreen) : Color(pRed)) << itemsettopt[0];
			itemsettopt[1] = ((currentMask & 0x2) ? Color(pGreen) : Color(pRed)) << itemsettopt[1];
			itemsettopt[2] = ((currentMask & 0x4) ? Color(pGreen) : Color(pRed)) << itemsettopt[2];

			Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_ITEM_SETTINGS), itemsettopt);
			const int op = optKb.Open();
			if(op < 0 || op > 2) {
				return;
			}

			u32 nextMask = currentMask ^ (1u << op);
			ApplyItemSettingsMask(nextMask);
			entry->SetSavedValue(nextMask);
		}
	}

	namespace {
		void GetCustomView(Keyboard& keyboard, KeyboardEvent& event) {
			if(event.type != KeyboardEvent::SelectionChanged) {
				return;
			}

			int index = event.selectedIndex;

			std::vector<std::string> f_file, f_Dir, f_all;
			std::vector<bool> isDir;
			File file;

			if(PluginUtils::Backup::restoreDirectory.ListDirectories(f_Dir) == Directory::OPResult::NOT_OPEN) {
				return;
			}

			if(PluginUtils::Backup::restoreDirectory.ListFiles(f_file, ".inv") == Directory::OPResult::NOT_OPEN)  {
				return;
			}

			if(f_Dir.empty() && f_file.empty()) {
				return;
			}

			for(const std::string& str : f_Dir) {
				f_all.push_back(str);
				isDir.push_back(true);
			}

			for(const std::string& str : f_file) {
				f_all.push_back(str);
				isDir.push_back(false);
			}

			if(index == -1) {
				return;
			}

			std::string& input = keyboard.GetMessage();

		//if directory return
			if(isDir[index]) {
				return;
			}

			input.clear();

			if(PluginUtils::Backup::restoreDirectory.OpenFile(file, f_all[index], File::READ) != 0) {
				return; //error opening file
			}

			std::string Sets[16];
			Item SetItem[16];
			std::vector<Item> OnlyItem;
			file.Read(&SetItem, sizeof(SetItem));

			for(int i = 0; i < 16; ++i) {
				if(SetItem[i].ID != 0x7FFE) {
					OnlyItem.push_back(SetItem[i]);
				}
			}

			for(int i = 0; i < 11; ++i) {
				if(i >= OnlyItem.size()) {
					return;
				}

				Sets[i] = OnlyItem[i].GetName();

				input += Color(0x40FF40FF) << Utils::Format("%08X | ", OnlyItem[i]) << Color(0xFFFDD0FF) << Sets[i] << "\n";
			}
			input += "etc...";
			file.Flush();
			file.Close();
		}

		static OperationResult RestorePresetSet(const std::vector<MemoryRange> &ranges) {
			std::vector<std::string> folders;
			Directory presetDirectory(PATH_PRESET);

			if(presetDirectory.ListDirectories(folders) == Directory::OPResult::NOT_OPEN || folders.empty()) {
				MessageBox(Language::getInstance()->get(TextID::RESTORE_NOFILES)).SetClear(ClearScreen::Top)();
				return OperationResult::ListingFailed;
			}

			Keyboard categoryKb(Language::getInstance()->get(TextID::GET_SET_PRESET_SELECT), folders);
			const int folderChoice = categoryKb.Open();
			if(folderChoice < 0 || folderChoice >= static_cast<int>(folders.size())) {
				return OperationResult::Aborted;
			}

			return PluginUtils::Backup::RestoreMemory(
				Utils::Format("%s/%s", PATH_PRESET, folders[folderChoice].c_str()),
				".inv",
				Language::getInstance()->get(TextID::GET_SET_PRESET_SELECT),
				ranges,
				PluginUtils::Backup::RestoreOptions{ GetCustomView, false }
			);
		}

		static bool ConfirmInventorySetRestore(void) {
			return MessageBox(
				Language::getInstance()->get(TextID::GET_SET_RESTORE_WARNING),
				DialogType::DialogYesNo
			).SetClear(ClearScreen::Top)();
		}
	};

	void getset(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		const std::vector<std::string> setopt = {
			Language::getInstance()->get(TextID::VECTOR_GETSET_FURN),
			Language::getInstance()->get(TextID::VECTOR_GETSET_CUSTOM),
		};

		const std::vector<std::string> custinvopt = {
			Language::getInstance()->get(TextID::VECTOR_GETSET_CUSTOM_BACKUP),
			Language::getInstance()->get(TextID::VECTOR_GETSET_CUSTOM_RESTORE),
			Language::getInstance()->get(TextID::FILE_DELETE),
		};

		MemoryRange LocInv = { (u32 *)player->Inventory, sizeof(player->Inventory) };
		MemoryRange LocLock = { (u32 *)player->InventoryItemLocks, sizeof(player->InventoryItemLocks) };

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_INVENTORY_SET_OPTION), setopt);

		switch(optKb.Open()) {
			default: return;
			case 0: {
				if(!ConfirmInventorySetRestore()) {
					return;
				}

				if(RestorePresetSet({ LocInv, LocLock }) == OperationResult::Success) {
					Inventory::ReloadIcons();
					MessageBox(Language::getInstance()->get(TextID::GET_SET_RESTORE_SUCCESS)).SetClear(ClearScreen::Top)();
				}
			} return;

			case 1: {
				Keyboard customKb(Language::getInstance()->get(TextID::KEY_CHOOSE_CUSTOM_SET_ACTION), custinvopt);

				switch(customKb.Open()) {
					default: return;
					case 0: {
						std::string filename = "";
						Keyboard KB(Language::getInstance()->get(TextID::GET_SET_DUMP));
						if(KB.Open(filename) == -1) {
							return;
						}

						PluginUtils::Backup::DumpMemory(
							Utils::Format(PATH_ITEMSET, Address::regionName.c_str()),
							filename,
							".inv",
							{ LocInv, LocLock }
						);
					} return;

					case 1: {
						if(!ConfirmInventorySetRestore()) {
							return;
						}

						if(PluginUtils::Backup::RestoreMemory(
							Utils::Format(PATH_ITEMSET, Address::regionName.c_str()),
							".inv",
							Language::getInstance()->get(TextID::GET_SET_RESTORE),
							{ LocInv, LocLock },
							PluginUtils::Backup::RestoreOptions{ GetCustomView, false }
						) == OperationResult::Success) {
							Inventory::ReloadIcons();
							MessageBox(Language::getInstance()->get(TextID::GET_SET_RESTORE_SUCCESS)).SetClear(ClearScreen::Top)();
						}
					} return;

					case 2:
						PluginUtils::Backup::DeleteBackup(
							Utils::Format(PATH_ITEMSET, Address::regionName.c_str()),
							".inv"
						);
					return;
				}
			}
		}
	}

	void HideDesignOutfits(MenuEntry *entry) {
		if (entry->WasJustActivated()) {
			RuntimeContext::getInstance()->setDesignOutfitsLegit(false);
		} else if (!entry->IsActivated()) {
			RuntimeContext::getInstance()->setDesignOutfitsLegit(true);
		}
	}
}
