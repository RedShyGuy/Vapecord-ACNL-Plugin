#include "cheats.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Town.hpp"
#include "Helpers/Wrapper.hpp"
#include "Address/Address.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/AnimData.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Converters.hpp"
#include "Helpers/NPC.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/GameStructs.hpp"
#include "Color.h"
#include "Files.h"

namespace CTRPluginFramework {
//Town Name Changer | player specific save code	
	void townnamechanger(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

        Keyboard keyboard(Language::getInstance()->get("TOWN_NAME_CHANGER_ENTER_NAME"));
        std::string input;
        keyboard.SetMaxLength(8);

        if(keyboard.Open(input) < 0) {
			return;
		}

		Town::EditName(input);
    }

//Save Backup and Restore | non player specific save code
	void savebackup(MenuEntry *entry) {
		static const std::vector<std::string> options = {
			Language::getInstance()->get("SAVE_DUMPER"), 
			Language::getInstance()->get("SAVE_RESTORE"),  
			Language::getInstance()->get("FILE_DELETE"),  
		};

		WrapLoc lock = { (u32 *)Address(0x2FB344).Call<u32>(), 0x89B00 };
		
		Keyboard KB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), options);

		switch(KB.Open()) {
			default: break;
			case 0: {
				std::string filename = "";
				Keyboard KB(Language::getInstance()->get("SAVE_DUMPER_DUMP"));

				if(KB.Open(filename) == -1) {
					return;
				}

				Wrap::Dump(Utils::Format(PATH_SAVE, Address::regionName.c_str()), filename, ".dat", &lock, nullptr);		
			} break;
			case 1: {
				if(Wrap::Restore(Utils::Format(PATH_SAVE, Address::regionName.c_str()), ".dat", Language::getInstance()->get("SAVE_RESTORE_SELECT"), nullptr, true, &lock, nullptr) == ExHandler::SUCCESS) {
					static Address fixfurno(0x6A6EE0);
					static Address fixfurno1 = fixfurno.MoveOffset(0x41C);

					fixfurno1.Patch(0xE1A00000);
					
					fixfurno.Call<void>();

					fixfurno1.Unpatch();
				} 
			} break;
			case 2: 
				Wrap::Delete(Utils::Format(PATH_SAVE, Address::regionName.c_str()), ".dat");
			break;
		}
	} 

//Bulletinboard message backup/restore | non player specific save code
	void bullboard(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		static const std::vector<std::string> bullsett = {
			Language::getInstance()->get("VECTOR_BULLETINDUMPER_BACKUP"),
			Language::getInstance()->get("VECTOR_BULLETINDUMPER_RESTORE"),
			Language::getInstance()->get("FILE_DELETE"),  
		};
		
		std::vector<std::string> backmessage;
		
		for(int i = 1; i <= 15; ++i) {
			backmessage.push_back(Utils::Format(Language::getInstance()->get("VECTOR_BULLETINDUMPER_MESSAGE").c_str(), i));
		}
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), bullsett);

		WrapLoc loc;
	
		switch(optKb.Open()) {
			default: break;
			case 0: {
				optKb.Populate(backmessage);

				int KBChoice = optKb.Open();
				if(KBChoice >= 0) {
					std::string filename = "";
					Keyboard KB(Language::getInstance()->get("BULLETIN_BOARD_DUMPER_DUMP"));

					if(KB.Open(filename) < 0) {
						return;
					}

					loc = { (u32 *)&town->BBoardMessages[KBChoice], sizeof(ACNL_BulletinBoardMessage) };
					Wrap::Dump(Utils::Format(PATH_BULLETIN, Address::regionName.c_str()), filename, ".dat", &loc, nullptr);
				}
			} break;
			
			case 1: {
				optKb.Populate(backmessage);
				int KBChoice = optKb.Open();
				if(KBChoice >= 0) {
					loc = { (u32 *)&town->BBoardMessages[KBChoice], sizeof(ACNL_BulletinBoardMessage) };
					Wrap::Restore(Utils::Format(PATH_BULLETIN, Address::regionName.c_str()), ".dat", Language::getInstance()->get("RESTORE_MESSAGE"), nullptr, true, &loc, nullptr); 
				}
			} break;
			
			case 2: 
				Wrap::Delete(Utils::Format(PATH_BULLETIN, Address::regionName.c_str()), ".dat");
			break;
		}
	}
//Tree Size Changer | non player specific save code
	void TreeSizeChanger(MenuEntry *entry) {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) {
			return;
		}

		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		std::vector<std::string> treesizevec;
		
		for(int i = 1; i <= 8; ++i) {
			treesizevec.push_back(Utils::Format(Language::getInstance()->get("TREESIZE_STATE").c_str(), i));
		}
		
		constexpr int played[8] = { 0, 5, 20, 50, 100, 180, 300, 500 };
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), treesizevec);

		int op = optKb.Open();
		if(op < 0) {
			return;
		}

		int intminutes = played[op] * 60;
		int intseconds = intminutes * 60;
		s64 sethours = intseconds;

		building->TownTreeSize = op;
		town->Playtime = sethours;
		town->DaysPlayed = played[op];
    }
//Change Native Fruit | non player specific save code	
	void ChangeNativeFruit(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		std::vector<std::string> fruitopt = {
            Language::getInstance()->get("NATIVE_FRUIT_APPLE"),
            Language::getInstance()->get("NATIVE_FRUIT_ORANGE"),
            Language::getInstance()->get("NATIVE_FRUIT_PEAR"),
            Language::getInstance()->get("NATIVE_FRUIT_PEACH"),
            Language::getInstance()->get("NATIVE_FRUIT_CHERRIE"),
            Language::getInstance()->get("NATIVE_FRUIT_COCONUT"),
            Language::getInstance()->get("NATIVE_FRUIT_DURIAN"),
            Language::getInstance()->get("NATIVE_FRUIT_LEMON"),
            Language::getInstance()->get("NATIVE_FRUIT_LYCHEE"),
            Language::getInstance()->get("NATIVE_FRUIT_MANGO"),
            Language::getInstance()->get("NATIVE_FRUIT_PERSIMMON"),
            Language::getInstance()->get("NATIVE_FRUIT_BANANA")
        };
		
		bool IsON;

		for(int i = 0; i < 12; ++i) {
			IsON = town->TownFruit.ID == 0x2000 + (i + 1);
			fruitopt[i] = IsON ? (Color(pGreen) << fruitopt[i]) : (fruitopt[i] = Color(pRed) << fruitopt[i]);
		}

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), fruitopt);

		int userChoice = optKb.Open();
		if(userChoice < 0) {
			return;
		}
		
		town->TownFruit.ID = 0x2000 + userChoice + 1;
		ChangeNativeFruit(entry);
	}

//Unlock All PWP | non player specific save code	
	void PWPUnlock(MenuEntry *entry) {	
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) {
			return;
		}

		static const std::vector<std::string> songopt = {
			Language::getInstance()->get("VECTOR_ENZY_FILL"),
			Language::getInstance()->get("VECTOR_ENZY_CLEAR"),
		};
			
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), songopt);

		switch(optKb.Open()) {
			default: break;
			case 0:
				for(int i = 0; i < 20 * 8; ++i) {
					building->UnlockedPWPs[i >> 5] |= (1 << (i & 0x1F));
				}
			break;
			case 1: 
				for(int i = 0; i < 20 * 8; ++i) {
					building->UnlockedPWPs[i >> 5] &= ~(1 << (i & 0x1F));
				}
			break;
		}
    }

//Grass Type Changer | non player specific save code		
	void GrassChanger(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		std::vector<std::string> grasstypevec = {
			Language::getInstance()->get("GRASS_CHANGER_TRIANGLE"),
            Language::getInstance()->get("GRASS_CHANGER_CIRCLE"),
            Language::getInstance()->get("GRASS_CHANGER_SQUARE")
		};	
		
		bool IsON;
		
		for(int i = 0; i < 3; ++i) {
			IsON = town->TownGrassType == i;
			grasstypevec[i] = IsON ? (Color(pGreen) << grasstypevec[i]) : (Color(pRed) << grasstypevec[i]);
		}
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), grasstypevec);

		int op = optKb.Open();
		if(op < 0) {
			return;
		}

		town->TownGrassType = op;
		GrassChanger(entry);
    }

	void caravanchange(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		int caravan = 0;

		std::vector<std::string> keyVec = {
			Language::getInstance()->get("CARAVAN_SET_LEFT"),
			Language::getInstance()->get("CARAVAN_SET_RIGHT")
		};

		Keyboard keyboard(Language::getInstance()->get("CARAVAN_SET_SELECT"), keyVec);

		int res = keyboard.Open(); //Pick caravan
		if(res < 0) {
			return;
		}

		caravan = res;

		keyboard.GetMessage() = std::string(Language::getInstance()->get("AMIIBO_SPOOFER_SPECIES"));
		keyVec.clear();

		NPC::PopulateRace(keyVec);

        keyboard.Populate(keyVec);

        res = keyboard.Open(); //Pick a species
        if(res < 0) {//User picked a species
			return;
		}

		keyboard.GetMessage() = std::string(Language::getInstance()->get("AMIIBO_SPOOFER_VILLAGER"));
		keyVec.clear();

		std::vector<PACKED_AmiiboInfo> amiiboVec;
		NPC::PopulateAmiibo((SpecieID)res, keyVec, amiiboVec, true, false);

		keyboard.Populate(keyVec);

		res = keyboard.Open(); //Pick villager based on species
		if(res < 0) {
			return;
		}

		const PACKED_AmiiboInfo& amiibo = amiiboVec[res];

		town->CampgroundCaravan[caravan] = amiibo.VID;
		OSD::Notify(Utils::Format("Set %s in caravan %1d", amiibo.Name.c_str(), caravan));
	}

	void SetCampingVillager(MenuEntry *entry) {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) {
			return;
		}

		std::vector<std::string> keyVec = {
			Language::getInstance()->get("CAMPING_SET_SET"),
			Language::getInstance()->get("CAMPING_SET_REMOVE")
		};

		Keyboard keyboard(Language::getInstance()->get("KEY_CHOOSE_OPTION"), keyVec);	

		int res = keyboard.Open();
       	if(res < 0) {
			return;
		}
		
		static Address SetNPCFunc(0x3081A8);
		static Address DeleteNPCFunc(0x30836C);

		if(res == 0) {
			Keyboard keyboard("a");
			keyboard.GetMessage() = std::string(Language::getInstance()->get("AMIIBO_SPOOFER_SPECIES"));
			keyVec.clear();

			NPC::PopulateRace(keyVec);
			keyVec.pop_back(); //Removes Special Characters from vec

			keyboard.Populate(keyVec);

			int res = keyboard.Open(); //Pick a species
			if(res < 0) {//User picked a species
				return;
			}

			keyboard.GetMessage() = std::string(Language::getInstance()->get("AMIIBO_SPOOFER_VILLAGER"));
			keyVec.clear();

			std::vector<PACKED_AmiiboInfo> amiiboVec;
			NPC::PopulateAmiibo((SpecieID)res, keyVec, amiiboVec, false, false);

			keyboard.Populate(keyVec);

			res = keyboard.Open(); //Pick villager based on species
			if(res < 0) {
				return;
			}

			const PACKED_AmiiboInfo& amiibo = amiiboVec[res];

			for(int i = 0; i < 56; ++i) {
				if(building->Buildings.Building[i].ID == 0xC6) {
					building->Buildings.Building[i].ID = 0x5F;
				}
			}

			u32 null[]{ 0 };
			u16 VID[]{ amiibo.VID };

			SetNPCFunc.Call<void>(&NPC::GetSaveData()->townID1, VID, null, &Town::GetSaveData()->TownData1); 
			OSD::Notify(Utils::Format("Set %s!", amiibo.Name.c_str()), Color::Green);

			if(Game::IsGameInRoom(0)) {
				Game::ReloadRoom();
			}
		}

		else if(res == 1) {
			for(int i = 0; i < 56; ++i) {
				if(building->Buildings.Building[i].ID == 0x5F) {
					building->Buildings.Building[i].ID = 0xC6;
				}
			}

			DeleteNPCFunc.Call<void>(&NPC::GetSaveData()->townID1);
			OSD::Notify("Camping Villager Removed!", Color::Red);

			if(Game::IsGameInRoom(0)) {
				Game::ReloadRoom();
			}
		}
	}

//Shop Unlocker
    void shopunlocks(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		ACNL_TownData *town = Town::GetSaveData();

		if(!player || !town) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> shopopt = {
			Language::getInstance()->get("VECTOR_SHOP_NOOK"), 	
			Language::getInstance()->get("VECTOR_SHOP_FORTUNE"), 
			Language::getInstance()->get("VECTOR_SHOP_DREAM"), 
			Language::getInstance()->get("VECTOR_SHOP_CLUB"), 	
			Language::getInstance()->get("VECTOR_SHOP_MUSEUM"), 
			Language::getInstance()->get("VECTOR_SHOP_SHAMPOODLE"), 
			Language::getInstance()->get("VECTOR_SHOP_KICKS") 
		};
		
		std::vector<std::string> nookopt = {
			Language::getInstance()->get("VECTOR_SHOP_CRANNY"),
			Language::getInstance()->get("VECTOR_SHOP_TT_MART"),
			Language::getInstance()->get("VECTOR_SHOP_SUPER_TT"),
			Language::getInstance()->get("VECTOR_SHOP_TIY"),
			Language::getInstance()->get("VECTOR_SHOP_TT_EMPORIUM")
		};

		static const int NooklingBellsSpent[5] {
			0, 12000, 25000, 50000, 100000
		};

		u8* UnlockStates[6] = {
			&town->FortuneTellerUnlockStatus,
			&town->DreamSuiteUnlockStatus,
			&town->ClubLOLUnlockState,
			&town->MuseumShopUnlockState,
			&town->ShampoodleUnlockStatus,
			&town->KickUnlockStatus
		};

		bool IsON = false;
		
		for(int i = 0; i < 6; ++i) { 
			IsON = *UnlockStates[i] != 0;
			shopopt[i + 1] = (IsON ? Color(pGreen) : Color(pRed)) << shopopt[i + 1];
		}
		
		Keyboard shopkb(Language::getInstance()->get("KEY_CHOOSE_STORE"), shopopt);

		int op = shopkb.Open();
		if(op < 0) {
			return;
		}

		switch(op) {
		//Nooklings
			case 0: {
				for(int i = 0; i < 5; ++i) {
					nookopt[i] = ((town->NooklingState == i) ? Color(pGreen) : Color(pRed)) << nookopt[i];
				}
				
				Keyboard nookkb(Language::getInstance()->get("KEY_CHOOSE_UPGRADE"), nookopt); 

				int nook = nookkb.Open();
				if(nook < 0) {
					return;
				}

				town->NooklingState = nook;
				town->NooklingStateUnknown = nook;
				Game::EncryptValue(&town->NooklingBellsSpent, NooklingBellsSpent[nook]);
				town->LeifUnlockStatus = (nook == 1 ? 2 : nook);
			} break;
		//Fortune Teller
			case 1: 
				town->FortuneTellerUnlockStatus = !town->FortuneTellerUnlockStatus;
			break;
		//Dream Suite
			case 2:
				town->DreamSuiteUnlockStatus = !town->DreamSuiteUnlockStatus;
			break;
		//Club LOL
			case 3:
				town->ClubLOLUnlockState = (town->ClubLOLUnlockState < 2) ? 2 : 0;

			//To remove shrunk if petition is not done
				if(!player->PlayerFlags.FinishedShrunkSignatures) {
					player->PlayerFlags.FinishedShrunkSignatures = 1;
				}
			break;
		//Museum Shop
			case 4:
				town->MuseumShopUnlockState = !town->MuseumShopUnlockState;
			break;
		//Shampoodle
			case 5:
				town->ShampoodleUnlockStatus = (town->ShampoodleUnlockStatus < 2) ? 2 : 0;
			break;
		//Kicks
			case 6:
				town->KickUnlockStatus = (town->KickUnlockStatus < 2) ? 2 : 0;
			break;
		}

		shopunlocks(entry);
	}

	void HouseChanger(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		ACNL_TownData *town = Town::GetSaveData();
		if(!player || !town) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		if(Game::GetOnlinePlayerCount() != 0) {		
			MessageBox(Language::getInstance()->get("ONLY_TOWN_ERROR")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};
		
		static const std::vector<std::string> HouseSet = {
			Language::getInstance()->get("HOUSE_EDITOR_EXT"),
			Language::getInstance()->get("HOUSE_EDITOR_INT"),
		};
		
		static const std::vector<std::string> HouseSettings = {
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SIZE"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_STYLE"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_DOORSHAPE"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_BRICK"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_ROOF"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_DOOR"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_FENCE"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_PAVEMENT"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_MAILBOX"),
		};
		
		static const std::vector<std::string> HouseInfo = {
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET1"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET2"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET3"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET4"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET5"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET6"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET7"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET8"),
			Language::getInstance()->get("HOUSE_EDITOR_HOUSE_SET9"),
		};
		
		static const std::vector<std::string> RoomSet = {
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_MIDDLE"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SECOND"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_BASEMENT"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_RIGHT"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_LEFT"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_BACK"),
		};
		
		static const std::vector<std::string> RoomInfo = {
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SET1"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SET2"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SET3"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SET4"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SET5"),
			Language::getInstance()->get("HOUSE_EDITOR_ROOM_SET6"),
		};
		
		static constexpr u8 ValidHouseValues[9][2] = {
			{ 0x00, 0x07 }, { 0x00, 0x03 }, { 0x00, 0x01 },
			{ 0x00, 0x1F }, { 0x00, 0x22 }, { 0x00, 0x15 },
			{ 0x00, 0x16 }, { 0x00, 0x08 }, { 0x00, 0x15 },
		};
		
		static constexpr u8 ValidRoomValues[6][2] = {
			{ 0, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 }, { 1, 4 },
		};
		
		for(int i = 0; i <= 3; ++i) {
			ACNL_Player *player = Player::GetSaveData(i);
			if(player) {
				if(Player::SaveExists(player)) {
					std::string str = "";
					Convert::U16_TO_STR(player->PlayerInfo.PlayerData.PlayerName, str);
					pV[i] = Player::GetColor(i) << str;
				}
			}
		}
		
		Keyboard pKB(Language::getInstance()->get("KEY_SELECT_PLAYER"), pV);
	
		int pChoice = pKB.Open();
		if((pChoice < 0) || (pV[pChoice] == Color::Silver << "-Empty-")) {
			return;
		}
		
		Keyboard hKB(Language::getInstance()->get("KEY_CHOOSE_OPTION"), HouseSet);
		
		int hChoice = hKB.Open();
		if(hChoice < 0) {
			return;
		}
	
	/*Exterior House Settings*/
		if(hChoice == 0) {
			Keyboard eKB(Language::getInstance()->get("HOUSE_EDITOR_SELECT_HOUSE"), HouseSettings);

			int eChoice = eKB.Open();
			if(eChoice < 0) {
				return;
			}
			
			u8 Value = 0;
			if(Wrap::KB<u8>(HouseInfo[eChoice], true, 2, Value, Value)) {
				if(!IDList::ValidID(Value, ValidHouseValues[eChoice][0], ValidHouseValues[eChoice][1])) {
					MessageBox(Language::getInstance()->get("INVALID_ID")).SetClear(ClearScreen::Top)();
					return;
				}

				switch(eChoice) {
					case 0:
						town->PlayerHouse[pChoice].exterior1.HouseSize = Value;
						town->PlayerHouse[pChoice].exterior2.HouseSize = Value;
					break;
					case 1:
						town->PlayerHouse[pChoice].exterior1.HouseStyle = Value;
						town->PlayerHouse[pChoice].exterior2.HouseStyle = Value;
					break;
					case 2:
						town->PlayerHouse[pChoice].exterior1.HouseDoorShape = Value;
						town->PlayerHouse[pChoice].exterior2.HouseDoorShape = Value;
					break;
					case 3:
						town->PlayerHouse[pChoice].exterior1.HouseBrick = Value;
						town->PlayerHouse[pChoice].exterior2.HouseBrick = Value;
					break;
					case 4:
						town->PlayerHouse[pChoice].exterior1.HouseRoof = Value;
						town->PlayerHouse[pChoice].exterior2.HouseRoof = Value;
					break;
					case 5:
						town->PlayerHouse[pChoice].exterior1.HouseDoor = Value;
						town->PlayerHouse[pChoice].exterior2.HouseDoor = Value;
					break;
					case 6:
						town->PlayerHouse[pChoice].exterior1.HouseFence = Value;
						town->PlayerHouse[pChoice].exterior2.HouseFence = Value;
					break;
					case 7:
						town->PlayerHouse[pChoice].exterior1.HousePavement = Value;
						town->PlayerHouse[pChoice].exterior2.HousePavement = Value;
					break;
					case 8:
						town->PlayerHouse[pChoice].exterior1.HouseMailBox = Value;
						town->PlayerHouse[pChoice].exterior2.HouseMailBox = Value;
					break;
				}
			}
			return;
		}
	
	/*Interior Room Size Settings*/
		Keyboard rKB(Language::getInstance()->get("HOUSE_EDITOR_SELECT_ROOM"), RoomSet);

		int rChoice = rKB.Open();
		if(rChoice < 0) {
			return;
		}
		
		u8 RoomSize = 0;
		if(Wrap::KB<u8>(RoomInfo[rChoice], true, 2, RoomSize, RoomSize)) {
			if(!IDList::ValidID(RoomSize, ValidRoomValues[rChoice][0], ValidRoomValues[rChoice][1])) {
				MessageBox(Language::getInstance()->get("INVALID_ID")).SetClear(ClearScreen::Top)();
				return;
			}

			switch(rChoice) {
				case 0:
					town->PlayerHouse[pChoice].MiddleRoom.flags.RoomSize = RoomSize;
				break;
				case 1:
					town->PlayerHouse[pChoice].SecondRoom.flags.RoomSize = RoomSize;
				break;
				case 2:
					town->PlayerHouse[pChoice].BasementRoom.flags.RoomSize = RoomSize;
				break;
				case 3:
					town->PlayerHouse[pChoice].RightRoom.flags.RoomSize = RoomSize;
				break;
				case 4:
					town->PlayerHouse[pChoice].LeftRoom.flags.RoomSize = RoomSize;
				break;
				case 5:
					town->PlayerHouse[pChoice].BackRoom.flags.RoomSize = RoomSize;
				break;
			}
		}
	}

//InputChangeEvent For Quick Menu
	void onBuildingChange(Keyboard &k, KeyboardEvent &e) {
		if(e.type == KeyboardEvent::CharacterRemoved || e.type == KeyboardEvent::CharacterAdded) {
			std::string s = k.GetInput();
			k.GetMessage() = "ID:\n\n" << IDList::GetBuildingName(!s.empty() ? std::stoi(s, nullptr, 16) : 0);
		}
	}

	void BuildingMod(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		static const std::vector<std::string> buildingOpt = {
			Language::getInstance()->get("QUICK_MENU_PLACE_AT_LOCATION"),
			Language::getInstance()->get("QUICK_MENU_MOVE_TO_LOCATION"),
			Language::getInstance()->get("QUICK_MENU_REMOVE_BUILDING"),
		};

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), buildingOpt);

		switch(optKb.Open()) {
			case 0:
				u8 id;
				if(Wrap::KB<u8>(Language::getInstance()->get("ENTER_ID"), 1, 2, id, 0, onBuildingChange)) {
					Game::PlaceBuilding(id);
				}
				break;
			case 1:
				Game::MoveBuilding();
				break;
			case 2:
				Game::RemoveBuilding();
				break;
			default: break;
		}
	}
/*
	bool IsVillagerBoxed(u32 VillagerData) {
		u8 flag = *(u8 *)(VillagerData + 0x24E4 + 0x2C);
		return (flag == 1);
	}

	std::vector<std::string> GetVillager(void) {
		std::vector<std::string> vill;
		u32 VillagerBase = GameHelper::GetGardenPlus(0x292A4 + 0x2C); //To jump directly to villager VID

		for(int i = 0; i < 10; i++) {
			for(const AmiiboInfo& amiibo : amiiboVillagers) {
				if(amiibo.VID == 0xFFFF) //non villager get skipped
					continue;

				if(amiibo.VID == *(u16 *)((VillagerBase) + (0x2518 * i))) 
					vill.push_back(amiibo.Name);
			}
			if(*(u16 *)((VillagerBase) + (0x2518 * i)) == 0xFFFF)
				vill.push_back(Color::Gray << "-empty-");
		}
		return vill;
	}

	void VillagerEditor(MenuEntry *entry) {
		std::vector<std::string> Options = {
			"(un)box villager",
		};

		Keyboard opKB("Select Option:", Options);
		int opChoice = opKB.Open();
		
		if(opChoice == -1) 
			return;
	//remove villager
		if(opChoice == 0) {
			Options.clear();
			std::vector<std::string> vill = GetVillager();
			for(int i = 0; i < 10; i++) {
				if(IsVillagerBoxed(GameHelper::GetGardenPlus(0x292A4) + (0x2518 * i)))
					Options.push_back(Color::Gray << "[" << vill[i] << "]");
				else
					Options.push_back(vill[i]);
			}

			opKB.Populate(Options);
			opChoice = opKB.Open();
			if(opChoice == -1 || vill[opChoice] == (Color::Gray << "-empty-")) 
				return;

			if(IsVillagerBoxed(GameHelper::GetGardenPlus(0x292A4) + (0x2518 * opChoice)))
				Process::Write8(GameHelper::GetGardenPlus(0x292A4) + (0x2518 * opChoice) + 0x24E4, 0);
			else
				Process::Write8(GameHelper::GetGardenPlus(0x292A4) + (0x2518 * opChoice) + 0x24E4, 1);
			//Process::Write32((u32)&FUN, 0x3110F4);
			//FUN(GameHelper::GetGardenPlus(0x292A4), opChoice, GameHelper::GetGardenPlus(0x292A4) + (0x2518 * opChoice), 0);

			//MessageBox("Boxed!").SetClear(ClearScreen::Top)();
		}
		VillagerEditor(entry);
	}*/
	//31F54C54
	//308cc8
	//0x31F50224, 5, 31F5BB9C, 0
	//3110F4(0x31F50224/*Villager Start*/, 4, 0x31F59684/*Villager Save*/, 0)

	/*VILLAGERS = 			0x292D0; //Villager Start
	VILLAGER_ID = 			0x00;
	VILLAGER_PERSONALITY = 	0x02;
	VILLAGER_STOREDLETTER = 0x17C4;
	VILLAGER_SHIRT = 		0x246E;
	VILLAGER_SONG = 		0x2472;
	VILLAGER_WALL = 		0x2476;
	VILLAGER_FLOOR = 		0x247A;
	VILLAGER_UMBRELLA = 	0x247E;
	VILLAGER_FURNITURE = 	0x2482;
	VILLAGER_CATCHPHRASE = 	0x24C6; 
	VILLAGER_STATUS = 		0x24E4; //1 = boxed | 2 = moving in

	VILLAGER_AFFINITY =		0x9A7
	


	31F5EA58 = If talked to villager streak is active (right nibble)

	31F5EA5B = Current Affinity
	hitting someone with a net can deplet friendship to 0 on a single day (6 friendship per hit)
	-> Either through pushing or hitting with net

	If talked to villager for first time on the day you get +1 friendship

	31F5EA5C = Total Affinity got for today through speaking => resets after day
	If talked to villager you get +2 of this (up to 14 (7 times per day))

	31F5EA5F = If talked to villager once today (x != 0 = bool) => resets after day (left nibble)



	VILLAGER_SIZE = 		0x2518; //Villager Size
	
	//Villager Start
	0x292D0

	//Villager Size
	0x2518

	0x2B7B4 //Boxed 1st Villager | 1 Bit Boolean
	0x2DCCC //Boxed 2nd Villager | 1 Bit Boolean
	
	
	//Move Away Villager Instantly
	//FUN_00308CC8(u32 VillagerOffset); 31F54C54   
	
	31F52768
	
	31F5273C
	31F6E3BC
	31F708D4
	
	
	31F5273C
	31F23DC8
	31F23DB8
	31F89138

//Delete save game
	thunk_FUN_00608660();
	FUN_00304a60(0x62,2,2,2,2);
	*/

	void CompleteMuseum(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		static const std::pair<u16, u16> Pairs[6] = { 
			{ 0x3130, 0x3173 }, //AnalyzedFossils 43
			{ 0x22E1, 0x2329 }, //Fish 48
			{ 0x232D, 0x234B }, //SeaCreatures 1E
			{ 0x228E, 0x22D6 }, //Insects 48
			{ 0x30D2, 0x30EB }, //RealPaintings 19
			{ 0x30EB, 0x30F3 } //RealStatues 8
		};

		static const u16 Addage[6] = {
			0, 0x43, 0x8B, 0xA9, 0xF1, 0x10A
		};

		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};

		static const std::vector<std::string> musOpt = {
			Language::getInstance()->get("VECTOR_ENZY_FILL"),
			Language::getInstance()->get("VECTOR_ENZY_CLEAR"),
		};

		for(int i = 0; i <= 3; ++i) {
			ACNL_Player *player = Player::GetSaveData(i);
			if(player) {
				if(Player::SaveExists(player)) {
					std::string str = "";
					Convert::U16_TO_STR(player->PlayerInfo.PlayerData.PlayerName, str);
					pV[i] = Player::GetColor(i) << str;
				}
			}
		}

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"), musOpt);
		int state = optKb.Open();
		if(state < 0) {
			return;
		}

		else if(state == 0) {
			optKb.Populate(pV);
			int player = optKb.Open();
			if(player < 0 && pV[player] == (Color::Silver << "-Empty-")) {
				return;
			}

			for(int j = 0; j < 6; ++j) {
				for(u16 i = Pairs[j].first; i < Pairs[j].second; ++i) {
					int field = Address(0x2FF76C).Call<int>(&i);
					town->MuseumDonations[Addage[j] + field] = player + 1;

					town->MuseumDonationDates[Addage[j] + field] = Game::GetCurrentDate();
				}
			}
		}

		else if(state == 1) {
			for(int j = 0; j < 6; ++j) {
				for(u16 i = Pairs[j].first; i < Pairs[j].second; ++i) {
					int field = Address(0x2FF76C).Call<int>(&i);
					town->MuseumDonations[Addage[j] + field] = 0;

					town->MuseumDonationDates[Addage[j] + field] = { 0, 0, 0 };
				}
			}
		}
	}

//Finish Mayor Permit | only for player 1	
	void Permit100(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			entry->Disable();
			return;
		}

		//player->PlayerFlags.FinishedFirstDay = 1; //not tested (might break something)
		player->PlayerFlags.PermitApproval = 1;
		player->PlayerFlags.MayorJobIntroduction = 1;
		player->PlayerFlags.KnowsPermitRequirements = 1;
		player->PlayerFlags.PermitFinished = 1;
		player->PlayerFlags.PermitApprovalArrived = 1;
		player->PlayerFlags.PermitIntroduction = 1;
		player->PlayerFlags.PWPExplained = 1;
		player->PlayerFlags.OrdinanceExplained = 1;
		player->PlayerFlags.PermitFinished = 1;

	//make all permit points ON
		player->PlayerFlags.Permit_Points1 = 1;
		player->PlayerFlags.Permit_Points2 = 1;
		player->PlayerFlags.Permit_Points3 = 1;
		player->PlayerFlags.Permit_Points4 = 1;
		player->PlayerFlags.Permit_Points5 = 1;
		player->PlayerFlags.Permit_Points6 = 1;
		player->PlayerFlags.Permit_Points7 = 1;
		player->PlayerFlags.Permit_Points8 = 1;
		player->PlayerFlags.Permit_Points9 = 1;
		player->PlayerFlags.Permit_Points10 = 1;

		entry->Disable();
    }

	std::string S_AcreID[4][5] = { "" };
	static constexpr int I_RectXPos[5] = { 70, 106, 142, 178, 214 };
	static constexpr int J_RectYPos[4] = { 54, 90, 126, 162 };
	static constexpr int Addage[4] = { 0x8, 0xF, 0x16, 0x1D };
//OSD for Map Editor
	bool MapDraw(const Screen &screen) {	
		if(!screen.IsTop) {
			for(int j = 0; j < 4; ++j) {
				for(int i = 0; i < 5; ++i) {
					screen.DrawSysfont(S_AcreID[j][i], I_RectXPos[i] + 9, J_RectYPos[j] + 9, Color::Black);
					screen.DrawRect(I_RectXPos[i], J_RectYPos[j], 36, 36, Color::Black, false);
				}
			}
		}
		return 1;
    }

	void GetAcreID(void) {
		if(!(Game::MapBoolCheck() && Game::IsGameInRoom(0) && Player::GetSaveOffset(4) != 0)) {
			return;
		}

		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		for(int j = 0; j < 4; ++j) {
			for(int i = 0; i < 5; ++i) {
				S_AcreID[j][i] = (Utils::Format("%02X", town->TownAcres[Addage[j] + i]));
			}
		}	
	}

	c_RGBA* acreArray = nullptr;
	u8 acreID = 0;

	void ShowAcre(void) {
		const Screen &Top = OSD::GetTopScreen();

		int XPos = 168, YPos = 88;
		int Pixels = 0, XResult = 64 + XPos, YResult = 64 + YPos;

		Top.DrawRect(XPos - 80, YPos - 30, XPos + 60, YPos + 80, Color::Black);

		for(int X = XPos; X < XResult; ++X) {
			for(int Y = YPos; Y < YResult; ++Y) {
				Color cPix = Color(acreArray[Pixels].R, acreArray[Pixels].G, acreArray[Pixels].B, acreArray[Pixels].A);
				if(cPix != Color(0, 0, 0, 0)) {
					Top.DrawPixel(X, Y, cPix);
				}

				Pixels++;
			}
		}
		
		Top.DrawSysfont(((acreID <= 0) ? "    " : "\uE052 ") + Utils::Format("Acre ID %02X", acreID) + ((acreID >= 0xD6) ? "  " : " \uE053"), 145, 70);
	
		Top.DrawSysfont("Do you want to set this Acre?", 100, 160);
		Top.DrawSysfont("Press \uE000 to confirm", 140, 180);
		Top.DrawSysfont("Press \uE001 to cancel", 140, 200);
	}

	bool SetAcre(u8 acreID) {
		delete[] acreArray;

		std::string test = Utils::Format(PATH_ACRE, acreID);
		File Test(test, File::READ);
		if(!Test.IsOpen()) {
			return 0;
		}

		acreArray = new c_RGBA[Test.GetSize() / sizeof(c_RGBA)];
		Test.Read(acreArray, Test.GetSize());
		Test.Close();
		return 1;
	}

	bool IsAcreOkay(u8& AcreID) {
		if(!IDList::ValidID(AcreID, 0, 0xD6)) {
			OSD::Notify("Invalid Acre ID!", Color::Red);
			return false;
		}

		if(IDList::IsHalfAcre(AcreID)) {
			OSD::Notify("Half Acre's can't be set at full acre spots!", Color::Red);
			return false;
		}

		if(!SetAcre(AcreID)) {
			OSD::Notify(Utils::Format(PATH_ACRE, AcreID) + " is missing", Color::Red);
			return 1;
		}

		Process::Pause();

		bool okay = false;	

		while(true) {
			Controller::Update();

			acreID = AcreID;
			ShowAcre();

			if(Controller::IsKeyDown(Key::A)) {
				okay = true;
				break;
			}
			if(Controller::IsKeyDown(Key::B)) {
				okay = false;
				break;
			}

			if(Controller::IsKeyPressed(Key::L) && AcreID > 0) {
				AcreID--;

				while(IDList::IsHalfAcre(AcreID)) {
					AcreID--;
				}

				if(!SetAcre(AcreID)) {
					OSD::Notify(Utils::Format(PATH_ACRE, AcreID) + " is missing", Color::Red);
					break;
				}
			}
			if(Controller::IsKeyPressed(Key::R) && AcreID < 0xD6) {
				AcreID++;

				while(IDList::IsHalfAcre(AcreID)) {
					AcreID++;
				}

				if(!SetAcre(AcreID)) {
					OSD::Notify(Utils::Format(PATH_ACRE, AcreID) + " is missing", Color::Red);
					break;
				}
			}

			OSD::SwapBuffers();
		}
		
		delete[] acreArray;
		acreArray = nullptr;

		Process::Play();

		return okay;
	}

//Map Function
	void AcreChange(int acre, u8 offset) {
		u8 AcreID = 0;
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		Keyboard Acre(Utils::Format(Language::getInstance()->get("MAP_EDITOR_TYPE_ID").c_str(), acre));
		if(Acre.Open(AcreID, AcreID) < 0) {
			return;
		}

		if(!IsAcreOkay(AcreID)) {
			return;
		}

		town->TownAcres[offset] = AcreID;

		OSD::Notify(Utils::Format("Acre %02d : %02X", acre, AcreID));

		u32 pInstance = PlayerClass::GetInstance()->Offset();
		u32 aInstance = Animation::GetAnimationInstance(pInstance, 0, 0, 0);

		AnimData data;
		data.Init(aInstance, pInstance, 4);
		data.Congrats_2A();
		data.ExecuteAnimation(0x2A);

		OSD::Notify("Save and quit your game to see your changes!");
	}

//Map Editor
	void MapEditor(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu += GetAcreID;
		}

		bool IsOkay = (Game::MapBoolCheck() && Game::IsGameInRoom(0) && Player::GetSaveData());
		static bool WasActivated = false;
		int res = 0;

		if(IsOkay) {
			if(Touch::IsDown()) {
				for(int j = 0; j < 4; ++j) {
					for(int i = 0; i < 5; ++i) {
						UIntRect rec(I_RectXPos[i], J_RectYPos[j], 36, 36);
						res++;

						if(!rec.Contains(Touch::GetPosition())) {
							continue;
						}
						
						AcreChange(res, Addage[j] + i);
					}
				}
			}
		}

		if(IsOkay && !WasActivated) {
			OSD::Run(MapDraw);
			WasActivated = true;
		}
		else if(!IsOkay && WasActivated) {
			OSD::Stop(MapDraw);
			WasActivated = false;
		}
		
		if(!entry->IsActivated()) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu -= GetAcreID;
			OSD::Stop(MapDraw);
		}
	}
}
