#include "cheats.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/CROEditing.hpp"

#include "Helpers/IDList.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/ItemSequence.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/Player.hpp"
#include "Color.h"
#include "Files.h"

extern "C" void PATCH_KappnBypass1(void);
extern "C" void PATCH_KappnBypass2(void);

namespace CTRPluginFramework {
//For Acre And Building Mod
	struct sBuilding {
		u16 id;
		u8 x;
		u8 y;
	};

	struct Island {
		sBuilding b[2];
		u8 acres[16];
	};
	
	static Island isl { 
		0x69, 0x1F, 0x15, //Building 1
		0x6A, 0x1D, 0x15, //Building 2
		0xAE, 0xAF, 0xAF, 0xB0, //Acre ID's
		0xAD, 0xCB, 0xB9, 0xAC, //Acre ID's
		0xAD, 0xC7, 0xC1, 0xAC, //Acre ID's
		0xAB, 0xA9, 0xA9, 0xAA  //Acre ID's
	};

	void UnlockIsland(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Both)();
			return;
		}

		std::vector<std::string> cmnOpt =  {
			Language::getInstance()->get("UNLOCK_ISLAND_ISL"),
			Language::getInstance()->get("UNLOCK_ISLAND_CLUB")
		};

		bool IsON1 = player->PlayerFlags.UnlockedKappn == 1;
		bool IsON2 = player->PlayerFlags.HasClubTortimerMembership == 1;

		cmnOpt[0] = (IsON1 ? Color(pGreen) : Color(pRed)) << cmnOpt[0];
		cmnOpt[1] = (IsON2 ? Color(pGreen) : Color(pRed)) << cmnOpt[1];
		
		Keyboard KB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), cmnOpt);

		int op = KB.Open();
		if(op < 0) {
			return;
		}

		else if(op == 0) {
			player->PlayerFlags.UnlockedKappn = !IsON1;
			player->PlayerFlags.KnowKappn = !IsON1;
		}

		else if(op == 1) {
			player->PlayerFlags.HasClubTortimerMembership = !IsON2;
			player->PlayerFlags.ClubTortimerFirstAsked = !IsON2;
			player->PlayerFlags.ClubTortimerRulesExplained = !IsON2;
		}

		UnlockIsland(entry);
	}

//Instant Bonus Ore | CRO Patch
	void bonusOre(MenuEntry *entry) {
		CRO::Write<u16>("Tour", 0x424, 0x4600); //Disables check if Item is Ore
		CRO::Write<u16>("Tour", 0x444, 0x4600); //Disables check if Item is secret ore

		if(!entry->IsActivated()) {
			CRO::Write<u16>("Tour", 0x424, 0xD124);
        	CRO::Write<u16>("Tour", 0x444, 0xD001);
		}
	}
//Instant Fruit | CRO Patch
	void instantFruit(MenuEntry *entry) {
		CRO::Write<u32>("Kotobuki", 0x17F4, 0xE1500000);

		if(!entry->IsActivated()) {
			CRO::Write<u32>("Kotobuki", 0x17F4, 0xE1500005);
		}
	}

//Defined u32 items for Island Shop Slot Mod
	Item ShopItem[4] = { {0x2018, 0}, {0x2018, 0}, {0x2018, 0}, {0x2018, 0} };
//Keyboard for Island Shop Slot Mod
	void IslandSettings(MenuEntry *entry) {
		Keyboard SetItem("a");
		SetItem.DisplayTopScreen = true;
		SetItem.OnKeyboardEvent(ItemChange);

		for(int i = 0; i < 4; ++i) {
			SetItem.GetMessage() = Utils::Format(Language::getInstance()->get("ISLAND_SHOP_MOD_ENTER_ID").c_str(), i + 1);

			int res = SetItem.Open(*(u32 *)&ShopItem[i]);
			if(res < 0) {
				break;
			}
		}
	}
//Island Shop
	void IslandShop(MenuEntry *entry) {	
		static const Address IslandShopPointer(0x954238);
		if(*(u32 *)IslandShopPointer.addr == 0) {
			return;
		}
		
		if(Game::NextRoomCheck() == 0xA5 && Game::IsGameInRoom(0x65)) {
			for(int i = 0; i < 4; ++i) {
				Process::Write32(*(u32 *)IslandShopPointer.addr + 0x10 + (i * 4), ShopItem[i].isValid(false) ? *(u32 *)&ShopItem[i] : 0x2018);
			}
		}
	}
	
//All Tours
	void alltour(MenuEntry *entry) {
		static const Address TourPatch(0x76FCC0);
		if(entry->WasJustActivated()) {
			Process::Patch(TourPatch.addr, 0xE1A00000); //unsure? (still keeping it for safety)
			Process::Patch(TourPatch.addr + 0x54, 0xE1A00000);  //Adds tour difficulty
			Process::Patch(TourPatch.addr + 0xD0, 0xE1A00000); //Adds tour names and tour infos
			Process::Patch(TourPatch.addr + 0x138, 0xE1A00000); //Adds tour time

			Process::Patch(TourPatch.addr + 0xA8, 0xE2800001); //Adds all tours to be selectable
		}
		else if(!entry->IsActivated()) {
			Process::Patch(TourPatch.addr, 0x0A000004);
			Process::Patch(TourPatch.addr + 0x54, 0x0A000004);
			Process::Patch(TourPatch.addr + 0xD0, 0x0A000004);
			Process::Patch(TourPatch.addr + 0x138, 0x0A000004); 

			Process::Patch(TourPatch.addr + 0xA8, 0x12800001);
		}
    }

//Island Acre Mod	
	void acreMod(MenuEntry *entry) {
		if(*(u32 *)Address(0x953708).addr == 0) {
			return;
		}
		
		u32 IslAcreOffset = *(u32 *)Address(0x953708).addr + 2; //0x953708
		
		for(u8 i = 0; i < 16; ++i) {
			Process::Write8(IslAcreOffset + i * 2, isl.acres[i]);
		}
	}
//Island Acre Mod Keyboard
	void menuAcreMod(MenuEntry *entry) {
		Keyboard kb(Language::getInstance()->get("ENTER_ID"));
		kb.SetMaxLength(2);
		kb.IsHexadecimal(true);

		for(u8 i = 0; i < 16; ++i) {
			kb.GetMessage() = Utils::Format(Language::getInstance()->get("ISLAND_ACRE_ENTER_ID").c_str(), i + 1);

			kb.Open(isl.acres[i], isl.acres[i]);
		}
	}
//Island Building Mod	
	void buildingMod(MenuEntry *entry) {
		if(*(u32 *)Address(0x953708).addr == 0) {
			return;
		}
		
		u32 islandBuildings = *(u32 *)Address(0x953708).addr + 0x1022;
		
		for(u8 i = 0; i < 2; ++i) {
			Process::Write16(islandBuildings + i * 4, isl.b[i].id);
			Process::Write8(islandBuildings + 2 + i * 4, isl.b[i].x);
			Process::Write8(islandBuildings + 3 + i * 4, isl.b[i].y);
		}
	}
//Island Building Mod Keyboard
	void menuBuildingMod(MenuEntry *entry) {
		Keyboard kb(Language::getInstance()->get("ENTER_ID"));
		kb.SetMaxLength(2);
		kb.IsHexadecimal(true);

		for(u8 i = 0; i < 2; ++i) {
			kb.GetMessage() = Language::getInstance()->get("ISLAND_BUILDING_ENTER_ID") << Utils::Format(" %d", i + 1);
			kb.Open(isl.b[i].id, isl.b[i].id);

			kb.GetMessage() = Language::getInstance()->get("ISLAND_BUILDING_ENTER_X") << Utils::Format(" %d", i + 1);
			kb.Open(isl.b[i].x, isl.b[i].x);

			kb.GetMessage() = Language::getInstance()->get("ISLAND_BUILDING_ENTER_Y") << Utils::Format(" %d", i + 1);
			kb.Open(isl.b[i].y, isl.b[i].y);
		}
	}

	void FreeKappn(MenuEntry *entry) {
		static Hook hook1, hook2;
		static const Address kappn1(0x5DC048);
		static const Address kappn2(0x5DAF98);

		if(entry->WasJustActivated()) {
			hook1.Initialize(kappn1.addr, (u32)PATCH_KappnBypass1);
			hook2.Initialize(kappn2.addr, (u32)PATCH_KappnBypass2);

		  	hook1.SetFlags(USE_LR_TO_RETURN);
			hook2.SetFlags(USE_LR_TO_RETURN);

			hook1.Enable();	
			hook2.Enable();	
		}

		else if(!entry->IsActivated()) {
			hook1.Disable();	
			hook2.Disable();	
		}
	}

	void RestoreIsland(std::vector<Item> &fileData) {
		bool ItemSequenceWasON = false;

		if(ItemSequence::Enabled()) {
			ItemSequenceWasON = true;
			ItemSequence::Switch(false);
		}

		if(!bypassing) {
			Dropper::DropItemLock(true);
		}

		u32 count = 0;
		u32 x = 0x10, y = 0x10;
		s32 nextItem = -1;

		bool res = true;
		while(res) {
			while(res) {
				if(Game::GetItemAtWorldCoords(x, y)) {
					nextItem++;
					if(*Game::GetItemAtWorldCoords(x, y) != fileData[nextItem]) {
						if(Dropper::PlaceItemWrapper(1, ReplaceEverything, &fileData[nextItem], &fileData[nextItem], x, y, 0, 0, 0, 0, 0, 0x3D, 0xA5, false)) {
							count++;
							if(count % 300 == 0) {
								Sleep(Milliseconds(500));
							}
						}
					}
				}
				else {
					res = false;
				}
				
				y++;
			}
			res = true;
			
			y = 0x10;
			x++;
			if(!Game::GetItemAtWorldCoords(x, y)) {
				res = false;
			}
		}
		
		OSD::Notify(Utils::Format("%d %s", count, "items placed!"));

	//OFF
		if(!bypassing) {
			Dropper::DropItemLock(false);
		}

		if(ItemSequenceWasON) {
			ItemSequence::Switch(true);
		}
	}

	void IslandSaver(MenuEntry *entry) {
		if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			if(!PlayerClass::GetInstance()->IsLoaded()) {
				MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
				return;
			}
			if(!Game::IsGameInRoom(0x68)) {
				MessageBox(Language::getInstance()->get("ISLAND_SAVER_NO")).SetClear(ClearScreen::Top)();
				return;
			}

			Keyboard KB(Language::getInstance()->get("ISLAND_SAVER_DUMPER_DUMP"), std::vector<std::string>{ Language::getInstance()->get("ISLAND_SAVER_BACKUP_ISLAND"), Language::getInstance()->get("ISLAND_SAVER_RESTORE_ISLAND"), Language::getInstance()->get("FILE_DELETE") });
			int index = KB.Open();
			switch(index) {
				default: break;
				case 0: {
					std::vector<u32> dumpVec;
			
					for (u32 x = 0x10; x <= 0x2F; x++)
						for (u32 y = 0x10; y <= 0x2F; y++) {
							Item* atCoords = Game::GetItemAtWorldCoords(x, y);
							dumpVec.push_back((atCoords->Flags * 0x10000) ^ atCoords->ID);
						}
						
					WrapLoc backupLoc = WrapLoc{ dumpVec.data(), static_cast<int>(dumpVec.size() * sizeof(u32)) };
					
					std::string filename = "";
					Keyboard KB(Language::getInstance()->get("ISLAND_SAVER_NAME_BACKUP"));

					if(KB.Open(filename) == -1) {
						return;
					}

					Wrap::Dump(Utils::Format(PATH_ISLAND, Address::regionName.c_str()), filename, ".dat", &backupLoc, nullptr);
				} break;

				case 1: {
					size_t arrSize = 0x400;//number of elements in array
					u32 fileData[arrSize];
					std::vector<Item> IslandItems;
					if (!fileData) {
						OSD::Notify("Failed to accolate memory");
						return;
					}
					
					std::string filename = "restoredump";
					WrapLoc restoreLoc = WrapLoc{ fileData, static_cast<int>(arrSize * sizeof(u32)) };
					
					if(Wrap::Restore(Utils::Format(PATH_ISLAND, Address::regionName.c_str()), ".dat", Language::getInstance()->get("SAVE_RESTORE_SELECT"), nullptr, false, &restoreLoc, nullptr) == ExHandler::SUCCESS) {
						for(size_t i = 0; i < arrSize; i++) {
							IslandItems.push_back({ static_cast<u16>(fileData[i] & 0xFFFF), static_cast<u16>(fileData[i] >> 16) });
						}
						RestoreIsland(IslandItems);
					}
				} break;
				case 2: {
					Wrap::Delete(Utils::Format(PATH_ISLAND, Address::regionName.c_str()), ".dat");
				} break;
			}
		}
	}
}