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
#include "House/House.hpp"
#include "House/Exterior.hpp"
#include "Helpers/GameStructs.hpp"
#include "Color.h"
#include "Files.h"

namespace CTRPluginFramework {
//Town Name Changer | player specific save code	
	void townnamechanger(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

        Keyboard keyboard(Language::getInstance()->get(TextID::TOWN_NAME_CHANGER_ENTER_NAME));
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
			Language::getInstance()->get(TextID::SAVE_DUMPER), 
			Language::getInstance()->get(TextID::SAVE_RESTORE),  
			Language::getInstance()->get(TextID::FILE_DELETE),  
		};

		WrapLoc lock = { (u32 *)Address(0x2FB344).Call<u32>(), 0x89B00 };
		
		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), options);

		switch(KB.Open()) {
			default: break;
			case 0: {
				std::string filename = "";
				Keyboard KB(Language::getInstance()->get(TextID::SAVE_DUMPER_DUMP));

				if(KB.Open(filename) == -1) {
					return;
				}

				Wrap::Dump(Utils::Format(PATH_SAVE, Address::regionName.c_str()), filename, ".dat", &lock, nullptr);		
			} break;
			case 1: {
				if(Wrap::Restore(Utils::Format(PATH_SAVE, Address::regionName.c_str()), ".dat", Language::getInstance()->get(TextID::SAVE_RESTORE_SELECT), nullptr, true, &lock, nullptr) == ExHandler::SUCCESS) {
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
			Language::getInstance()->get(TextID::VECTOR_BULLETINDUMPER_BACKUP),
			Language::getInstance()->get(TextID::VECTOR_BULLETINDUMPER_RESTORE),
			Language::getInstance()->get(TextID::FILE_DELETE),  
		};
		
		std::vector<std::string> backmessage;
		
		for(int i = 1; i <= 15; ++i) {
			backmessage.push_back(Utils::Format(Language::getInstance()->get(TextID::VECTOR_BULLETINDUMPER_MESSAGE).c_str(), i));
		}
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), bullsett);

		WrapLoc loc;
	
		switch(optKb.Open()) {
			default: break;
			case 0: {
				optKb.Populate(backmessage);

				int KBChoice = optKb.Open();
				if(KBChoice >= 0) {
					std::string filename = "";
					Keyboard KB(Language::getInstance()->get(TextID::BULLETIN_BOARD_DUMPER_DUMP));

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
					Wrap::Restore(Utils::Format(PATH_BULLETIN, Address::regionName.c_str()), ".dat", Language::getInstance()->get(TextID::RESTORE_MESSAGE), nullptr, true, &loc, nullptr); 
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
			treesizevec.push_back(Utils::Format(Language::getInstance()->get(TextID::TREESIZE_STATE).c_str(), i));
		}
		
		constexpr int played[8] = { 0, 5, 20, 50, 100, 180, 300, 500 };
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), treesizevec);

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
            Language::getInstance()->get(TextID::NATIVE_FRUIT_APPLE),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_ORANGE),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_PEAR),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_PEACH),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_CHERRIE),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_COCONUT),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_DURIAN),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_LEMON),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_LYCHEE),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_MANGO),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_PERSIMMON),
            Language::getInstance()->get(TextID::NATIVE_FRUIT_BANANA)
        };
		
		bool IsON;

		for(int i = 0; i < 12; ++i) {
			IsON = town->TownFruit.ID == 0x2000 + (i + 1);
			fruitopt[i] = IsON ? (Color(pGreen) << fruitopt[i]) : (fruitopt[i] = Color(pRed) << fruitopt[i]);
		}

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), fruitopt);

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
			Language::getInstance()->get(TextID::VECTOR_ENZY_FILL),
			Language::getInstance()->get(TextID::VECTOR_ENZY_CLEAR),
		};
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), songopt);
		int res = optKb.Open();
		if (res < 0) {
			return;
		}

		bool unlock = (res == 0);

		building->UnlockedPWPs.DefaultPWPs = unlock;
        building->UnlockedPWPs.Topiaries = unlock;
        building->UnlockedPWPs.DreamSuite = unlock;
        building->UnlockedPWPs.MuseumRenovation = unlock;
        building->UnlockedPWPs.Cafe = unlock;
        building->UnlockedPWPs.ResetCenter = unlock;
        building->UnlockedPWPs.PerfectTownPWPs = unlock;
        building->UnlockedPWPs.StationReconstruction = unlock;
        building->UnlockedPWPs.DrinkingFountain = unlock;
        building->UnlockedPWPs.GarbageCan = unlock;
        building->UnlockedPWPs.FlowerBed = unlock;
        building->UnlockedPWPs.OutdoorChair = unlock;
        building->UnlockedPWPs.FlowerArch = unlock;
        building->UnlockedPWPs.FairyTaleClock = unlock;
        building->UnlockedPWPs.FairyTaleBench = unlock;
        building->UnlockedPWPs.FairyTaleStreetlight = unlock;
        building->UnlockedPWPs.FairyTaleBridge = unlock;
        building->UnlockedPWPs.MetalBench = unlock;
        building->UnlockedPWPs.RoundStreetlight = unlock;
        building->UnlockedPWPs.IlluminatedHeart = unlock;
        building->UnlockedPWPs.IlluminatedClock = unlock;
        building->UnlockedPWPs.IlluminatedTree = unlock;
        building->UnlockedPWPs.Bell = unlock;
        building->UnlockedPWPs.ArchwaySculpture = unlock;
        building->UnlockedPWPs.StatueFountain = unlock;
        building->UnlockedPWPs.HotSpring = unlock;
        building->UnlockedPWPs.Streetlight = unlock;
        building->UnlockedPWPs.IlluminatedArch = unlock;
        building->UnlockedPWPs.Tower = unlock;
        building->UnlockedPWPs.ModernClock = unlock;
        building->UnlockedPWPs.ModernBench = unlock;
        building->UnlockedPWPs.ModernStreetlight = unlock;
        building->UnlockedPWPs.Scarecrow = unlock;
        building->UnlockedPWPs.Geyser = unlock;
        building->UnlockedPWPs.Windmill = unlock;
        building->UnlockedPWPs.WoodBench = unlock;
        building->UnlockedPWPs.WisteriaTrellis = unlock;
        building->UnlockedPWPs.LogBench = unlock;
        building->UnlockedPWPs.BusStop = unlock;
        building->UnlockedPWPs.PicnicBlanket = unlock;
        building->UnlockedPWPs.BalloonArch = unlock;
        building->UnlockedPWPs.TireToy = unlock;
        building->UnlockedPWPs.PileOfPipes = unlock;
        building->UnlockedPWPs.CampingCot = unlock;
        building->UnlockedPWPs.JungleGym = unlock;
        building->UnlockedPWPs.Sandbox = unlock;
        building->UnlockedPWPs.Hammock = unlock;
        building->UnlockedPWPs.WaterPump = unlock;
        building->UnlockedPWPs.InstrumentShelter = unlock;
        building->UnlockedPWPs.Torch = unlock;
        building->UnlockedPWPs.FirePit = unlock;
        building->UnlockedPWPs.SolarPanel = unlock;
        building->UnlockedPWPs.BlueBench = unlock;
        building->UnlockedPWPs.TrafficSignal = unlock;
        building->UnlockedPWPs.StadiumLight = unlock;
        building->UnlockedPWPs.VideoScreen = unlock;
        building->UnlockedPWPs.WoodenBridge = unlock;
        building->UnlockedPWPs.ZenGarden = unlock;
        building->UnlockedPWPs.ZenBell = unlock;
        building->UnlockedPWPs.RackOfRice = unlock;
        building->UnlockedPWPs.DrillingRig = unlock;
        building->UnlockedPWPs.ZenClock = unlock;
        building->UnlockedPWPs.ZenBench = unlock;
        building->UnlockedPWPs.ZenStreetlight = unlock;
        building->UnlockedPWPs.Sphinx = unlock;
        building->UnlockedPWPs.TotemPole = unlock;
        building->UnlockedPWPs.ParabolicAntenna = unlock;
        building->UnlockedPWPs.MoaiStatue = unlock;
        building->UnlockedPWPs.Stonehenge = unlock;
        building->UnlockedPWPs.Pyramid = unlock;
        building->UnlockedPWPs.CubeSculpture = unlock;
        building->UnlockedPWPs.ChairSculpture = unlock;
        building->UnlockedPWPs.PoliceStations = unlock;
        building->UnlockedPWPs.Lighthouse = unlock;
        building->UnlockedPWPs.BrickBridge = unlock;
        building->UnlockedPWPs.ModernBridge = unlock;
        building->UnlockedPWPs.StoneTablet = unlock;
        building->UnlockedPWPs.WindTurbine = unlock;
        building->UnlockedPWPs.CautionSign = unlock;
        building->UnlockedPWPs.YieldSign = unlock;
        building->UnlockedPWPs.FortuneTellersShop = unlock;
    }

//Grass Type Changer | non player specific save code		
	void GrassChanger(MenuEntry *entry) {
		ACNL_TownData *town = Town::GetSaveData();
		if(!town) {
			return;
		}

		std::vector<std::string> grasstypevec = {
			Language::getInstance()->get(TextID::GRASS_CHANGER_TRIANGLE),
            Language::getInstance()->get(TextID::GRASS_CHANGER_CIRCLE),
            Language::getInstance()->get(TextID::GRASS_CHANGER_SQUARE)
		};	
		
		bool IsON;
		
		for(int i = 0; i < 3; ++i) {
			IsON = town->TownGrassType == i;
			grasstypevec[i] = IsON ? (Color(pGreen) << grasstypevec[i]) : (Color(pRed) << grasstypevec[i]);
		}
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), grasstypevec);

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
			Language::getInstance()->get(TextID::CARAVAN_SET_LEFT),
			Language::getInstance()->get(TextID::CARAVAN_SET_RIGHT)
		};

		Keyboard keyboard(Language::getInstance()->get(TextID::CARAVAN_SET_SELECT), keyVec);

		int res = keyboard.Open(); //Pick caravan
		if(res < 0) {
			return;
		}

		caravan = res;

		keyboard.GetMessage() = std::string(Language::getInstance()->get(TextID::AMIIBO_SPOOFER_SPECIES));
		keyVec.clear();

		NPC::PopulateRace(keyVec);

        keyboard.Populate(keyVec);

        res = keyboard.Open(); //Pick a species
        if(res < 0) {//User picked a species
			return;
		}

		keyboard.GetMessage() = std::string(Language::getInstance()->get(TextID::AMIIBO_SPOOFER_VILLAGER));
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
		OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::CARAVAN_SET).c_str(), amiibo.Name.c_str(), caravan));
	}

	void SetCampingVillager(MenuEntry *entry) {
		ACNL_BuildingData *building = Building::GetSaveData();
		if(!building) {
			return;
		}

		std::vector<std::string> keyVec = {
			Language::getInstance()->get(TextID::CAMPING_SET_SET),
			Language::getInstance()->get(TextID::CAMPING_SET_REMOVE)
		};

		Keyboard keyboard(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), keyVec);	

		int res = keyboard.Open();
       	if(res < 0) {
			return;
		}
		
		static Address SetNPCFunc(0x3081A8);
		static Address DeleteNPCFunc(0x30836C);

		if(res == 0) {
			Keyboard keyboard("a");
			keyboard.GetMessage() = std::string(Language::getInstance()->get(TextID::AMIIBO_SPOOFER_SPECIES));
			keyVec.clear();

			NPC::PopulateRace(keyVec);
			keyVec.pop_back(); //Removes Special Characters from vec

			keyboard.Populate(keyVec);

			int res = keyboard.Open(); //Pick a species
			if(res < 0) {//User picked a species
				return;
			}

			keyboard.GetMessage() = std::string(Language::getInstance()->get(TextID::AMIIBO_SPOOFER_VILLAGER));
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
			OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::CAMPING_VILLAGER_SET).c_str(), amiibo.Name.c_str()), Color::Green);

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
			OSD::Notify(Language::getInstance()->get(TextID::CAMPING_VILLAGER_REMOVED), Color::Red);

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
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> shopopt = {
			Language::getInstance()->get(TextID::VECTOR_SHOP_NOOK), 	
			Language::getInstance()->get(TextID::VECTOR_SHOP_FORTUNE), 
			Language::getInstance()->get(TextID::VECTOR_SHOP_DREAM), 
			Language::getInstance()->get(TextID::VECTOR_SHOP_CLUB), 	
			Language::getInstance()->get(TextID::VECTOR_SHOP_MUSEUM), 
			Language::getInstance()->get(TextID::VECTOR_SHOP_SHAMPOODLE), 
			Language::getInstance()->get(TextID::VECTOR_SHOP_KICKS) 
		};
		
		std::vector<std::string> nookopt = {
			Language::getInstance()->get(TextID::VECTOR_SHOP_CRANNY),
			Language::getInstance()->get(TextID::VECTOR_SHOP_TT_MART),
			Language::getInstance()->get(TextID::VECTOR_SHOP_SUPER_TT),
			Language::getInstance()->get(TextID::VECTOR_SHOP_TIY),
			Language::getInstance()->get(TextID::VECTOR_SHOP_TT_EMPORIUM)
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
		
		Keyboard shopkb(Language::getInstance()->get(TextID::KEY_CHOOSE_STORE), shopopt);

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
				
				Keyboard nookkb(Language::getInstance()->get(TextID::KEY_CHOOSE_UPGRADE), nookopt); 

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

	void ReloadRoomIfInTownOrRoom() {
		for (u8 roomId = 3; roomId < 0x1B; roomId++) {
			if (Game::IsGameInRoom(roomId)) {
				Game::ReloadRoom();
				return;
			}
		}

		if(Game::IsGameInRoom(0)) {
			Game::ReloadRoom();
		}
	}

	int SelectPlayerToEditHouse() {
		std::vector<std::string> pV = {
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
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
		
		Keyboard pKB(Language::getInstance()->get(TextID::KEY_SELECT_PLAYER), pV);
	
		int pChoice = pKB.Open();
		if((pChoice < 0) || (pV[pChoice] == Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY))) {
			return -1;
		}

		return pChoice;
	}

	void LoadHouseEditingTutorial(bool justUnlocked) {
		if (!justUnlocked) {
			if (Game::GetGameMode() != Game::GameMode::OFFLINE) {
				MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_ONLY_OFFLINE)).SetClear(ClearScreen::Top)();
			}
		}

		if((MessageBox(justUnlocked ? Language::getInstance()->get(TextID::HOUSE_EDITOR_SECRET_STORAGE_UNLOCKED) : "", Language::getInstance()->get(TextID::HOUSE_EDITOR_SECRET_STORAGE_RELOAD), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			Game::TeleportToRoom(0xA3, 0, 0, 0);
		}
	}

	enum UpgradeOption : int {
		Upgrade_Middle = 0,
		Upgrade_Second,
		Upgrade_Basement,
		Upgrade_Right,
		Upgrade_Left,
		Upgrade_Back,
		Unlock_SecretStorage
	};

	std::string GetUpgradeOptionName(UpgradeOption option) {
		switch (option) {
			case Upgrade_Middle: return Language::getInstance()->get(TextID::HOUSE_EDITOR_ROOM_MIDDLE);
			case Upgrade_Second: return Language::getInstance()->get(TextID::HOUSE_EDITOR_ROOM_SECOND);
			case Upgrade_Basement: return Language::getInstance()->get(TextID::HOUSE_EDITOR_ROOM_BASEMENT);
			case Upgrade_Right: return Language::getInstance()->get(TextID::HOUSE_EDITOR_ROOM_RIGHT);
			case Upgrade_Left: return Language::getInstance()->get(TextID::HOUSE_EDITOR_ROOM_LEFT);
			case Upgrade_Back: return Language::getInstance()->get(TextID::HOUSE_EDITOR_ROOM_BACK);
			case Unlock_SecretStorage: return Language::getInstance()->get(TextID::HOUSE_EDITOR_SECRET_STORAGE);
			default: return "";
		}
	}

	std::string GetUpgradeOptionAction(UpgradeOption option, u8 size) {
		if (option == Unlock_SecretStorage) {
			return Language::getInstance()->get(TextID::HOUSE_EDITOR_UNLOCK);
		} else {
			return (size < 2 ? Language::getInstance()->get(TextID::HOUSE_EDITOR_BUILD) : Language::getInstance()->get(TextID::HOUSE_EDITOR_UPGRADE));
		}
	}

	void UpgradeHouse(ACNL_Player *player, ACNL_TownData *town, int playerIndex) {
		struct Option { 
			u8 size; 
			UpgradeOption upgradeOption; 
		};
		std::vector<Option> options;

		u8 middleRoomSize = town->PlayerHouse[playerIndex].MiddleRoom.flags.RoomSize;
		u8 secondRoomSize = town->PlayerHouse[playerIndex].SecondRoom.flags.RoomSize;
		u8 basementRoomSize = town->PlayerHouse[playerIndex].BasementRoom.flags.RoomSize;
		u8 rightRoomSize = town->PlayerHouse[playerIndex].RightRoom.flags.RoomSize;
		u8 leftRoomSize = town->PlayerHouse[playerIndex].LeftRoom.flags.RoomSize;
		u8 backRoomSize = town->PlayerHouse[playerIndex].BackRoom.flags.RoomSize;
		bool secretStorageBuilt = player->PlayerFlags.UnlockedSecretStorage;

		if (middleRoomSize < 4) {
			options.push_back({ middleRoomSize, Upgrade_Middle });
		}

		bool middleDone = House::IsMiddleRoomFinished(town, playerIndex);
		bool secondBuilt = House::IsSecondRoomBuilt(town, playerIndex);

		if (secondRoomSize < 4 && middleDone) {
			options.push_back({ secondRoomSize, Upgrade_Second });
		}

		if (basementRoomSize < 4 && middleDone && secondBuilt) {
			options.push_back({ basementRoomSize, Upgrade_Basement });
		}

		if (rightRoomSize < 4 && middleDone && secondBuilt) {
			options.push_back({ rightRoomSize, Upgrade_Right });
		}

		if (leftRoomSize < 4 && middleDone && secondBuilt) {
			options.push_back({ leftRoomSize, Upgrade_Left });
		}

		if (backRoomSize < 4 && middleDone && secondBuilt) {
			options.push_back({ backRoomSize, Upgrade_Back });
		}

		if (!secretStorageBuilt && secondBuilt) {
			options.push_back({ 0, Unlock_SecretStorage });
		}

		if (options.empty()) {
			MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_ALL_FINISHED)).SetClear(ClearScreen::Top)();
			return;
		}

		std::vector<std::string> labels;
		labels.reserve(options.size());
		for (const auto &o : options)  {
			std::string action = GetUpgradeOptionAction(o.upgradeOption, o.size);
			std::string name = GetUpgradeOptionName(o.upgradeOption);
			labels.push_back(action + " " + name);
		}

		Keyboard upgradeKB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), labels);
		int idx = upgradeKB.Open();
		if (idx < 0) {
			return;
		}

		const Option chosen = options[idx];
		bool wasUpgraded = false;

		using RoomOp = void(*)(ACNL_Player*, ACNL_TownData*, int);
		struct RoomOps { 
			RoomOp build; 
			RoomOp up1; 
			RoomOp up2; 
		};

		static const RoomOps ops[6] = {
			/* 0: Main */	  { nullptr,                  House::UpgradeHouseFirst,        House::UpgradeHouseSecond },	
			/* 1: Second */   { House::BuildSecondRoom,   House::UpgradeSecondRoomFirst,   House::UpgradeSecondRoomSecond },
			/* 2: Basement */ { House::BuildBasementRoom, House::UpgradeBasementRoomFirst, House::UpgradeBasementRoomSecond },
			/* 3: Right */    { House::BuildRightRoom,    House::UpgradeRightRoomFirst,    House::UpgradeRightRoomSecond },
			/* 4: Left */     { House::BuildLeftRoom,     House::UpgradeLeftRoomFirst,     House::UpgradeLeftRoomSecond },
			/* 5: Back */     { House::BuildBackRoom,     House::UpgradeBackRoomFirst,     House::UpgradeBackRoomSecond },
		};

		switch (chosen.upgradeOption) {
			case Upgrade_Middle: case Upgrade_Second: case Upgrade_Basement: case Upgrade_Right: case Upgrade_Left: case Upgrade_Back: {
				const RoomOps &ro = ops[chosen.upgradeOption];
				if (chosen.size < 2 && ro.build) { 
					ro.build(player, town, playerIndex); 
					wasUpgraded = true; 
				}
				else if (chosen.size == 2 && ro.up1) { 
					ro.up1(player, town, playerIndex); 
					wasUpgraded = true; 
				}
				else if (chosen.size == 3 && ro.up2) { 
					ro.up2(player, town, playerIndex); 
					wasUpgraded = true; 
				}
				break;
			}
			case Unlock_SecretStorage:
				House::BuildSecretStorage(player, town, playerIndex);
				LoadHouseEditingTutorial(true);
				return;
		}

		if (wasUpgraded) {
			MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_UPGRADE_SUCCESS), labels[idx]).SetClear(ClearScreen::Top)();
			ReloadRoomIfInTownOrRoom();
		} else {
			MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_UPGRADE_FAIL)).SetClear(ClearScreen::Top)();
		}
	}

	void FinishHouse(ACNL_Player *player, ACNL_TownData *town, int playerIndex) {
		if (House::IsHouseFinished(town, playerIndex)) {
			MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_ALREADY_FINISHED)).SetClear(ClearScreen::Top)();
			return;
		}

		House::FinishHouse(player, town, playerIndex);
		MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_FINISH_SUCCESS)).SetClear(ClearScreen::Top)();
		ReloadRoomIfInTownOrRoom();
	}

	void EditExterior(ACNL_TownData *town, int playerIndex) {
		std::vector<std::string> options = {
			Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_DOOR),
			Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_BRICK),
			Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_FENCE),
			Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_MAILBOX),
			Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_PAVEMENT),
			Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_ROOF)
		};

		if (House::IsExteriorMaxSize(town, playerIndex)) {
			options.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_HOUSE_STYLE));
		}

		Keyboard extKB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), options);
		int extChoice = extKB.Open();
		if (extChoice < 0) {
			return;
		}

		using ExteriorList = std::vector<ExteriorOption>(*)();
		static const ExteriorList listOps[7] = {
			Exterior::GetDoorOptions,
			Exterior::GetExteriorOptions,
			Exterior::GetFenceOptions,
			Exterior::GetMailboxOptions,
			Exterior::GetPavementOptions,
			Exterior::GetRoofOptions,
			Exterior::GetFrameworkOptions,
		};

		std::vector<ExteriorOption> extOptions = listOps[extChoice]();
		std::vector<std::string> extType;
		for (ExteriorOption ext : extOptions) {
			u16 rawItemId = ext.rawItemId;
			std::string name = Item(rawItemId).GetName();
			extType.push_back(name);
		}

		Keyboard chooseKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), extType);
		int chosenId = chooseKb.Open();
		if (chosenId < 0) {
			return;
		}
		
		using ExteriorOp = void(*)(ACNL_TownData*, int, u8);
		static const ExteriorOp ops[7] = {
			Exterior::SetDoorStyle,
			Exterior::SetExteriorStyle,
        	Exterior::SetFenceStyle,
        	Exterior::SetMailboxStyle,
        	Exterior::SetPavementStyle,
        	Exterior::SetRoofStyle,
			Exterior::SetFrameworkStyle,
		};

		if (extChoice == 0) {
			bool res = MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_DOOR_CUSTOM_LABEL), Language::getInstance()->get(TextID::HOUSE_EDITOR_DOOR_CUSTOM_NOTE), DialogType::DialogYesNo).SetClear(ClearScreen::Top)();
			Exterior::SetDoorArched(town, playerIndex, res);
		}

		ops[extChoice](town, playerIndex, extOptions[chosenId].exteriorId);
		MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_EXTERIOR_SUCCESS)).SetClear(ClearScreen::Top)();
		ReloadRoomIfInTownOrRoom();
	}

	enum HouseAction {
		Build,
		Upgrade,
		Finish,
		Edit,
		Tutorial
	};

	void HouseChanger(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		ACNL_TownData *town = Town::GetSaveData();
		if(!player || !town) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		if(Game::GetOnlinePlayerCount() != 0) {		
			MessageBox(Language::getInstance()->get(TextID::ONLY_TOWN_ERROR)).SetClear(ClearScreen::Top)();
			return;
		}
		
		int playerIndex = SelectPlayerToEditHouse();
		if(playerIndex < 0) {
			return;
		}

		player = Player::GetSaveData(playerIndex);
		if (!player->PlayerFlags.FinishedFirstDay || !player->PlayerFlags.HouseLocationChosen) {
			MessageBox(Utils::Format(Language::getInstance()->get(TextID::HOUSE_EDITOR_PLAYER_NOT_READY).c_str(), player->PlayerInfo.PlayerData.GetName().c_str())).SetClear(ClearScreen::Top)();
			return;
		}

		std::vector<std::string> houseEditorOptions;
		std::vector<HouseAction> actions;

		if (!House::IsHouseBuilt(town, playerIndex)) {
			houseEditorOptions.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_BUILD_HOUSE));
			actions.push_back(HouseAction::Build);

			houseEditorOptions.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_FINISH_HOUSE));
			actions.push_back(HouseAction::Finish);
		} else if (!House::IsHouseFinished(town, playerIndex)) {
			houseEditorOptions.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_UPGRADE_HOUSE));
			actions.push_back(HouseAction::Upgrade);

			houseEditorOptions.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_FINISH_HOUSE));
			actions.push_back(HouseAction::Finish);
		}

		if (House::IsHouseBuilt(town, playerIndex)) {
			houseEditorOptions.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_EDIT_EXTERIOR));
			actions.push_back(HouseAction::Edit);
		}

		if (player->PlayerFlags.UnlockedSecretStorage) {
			houseEditorOptions.push_back(Language::getInstance()->get(TextID::HOUSE_EDITOR_LOAD_TUTORIAL));
			actions.push_back(HouseAction::Tutorial);
		}

		Keyboard hKB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), houseEditorOptions);
		
		int hChoice = hKB.Open();
		if(hChoice < 0) {
			return;
		}

		switch (actions[hChoice]) {
			case HouseAction::Build:
				House::BuildHouse(player, town, playerIndex);
				MessageBox(Language::getInstance()->get(TextID::HOUSE_EDITOR_BUILT_HOUSE_SUCCESS)).SetClear(ClearScreen::Top)();
				ReloadRoomIfInTownOrRoom();
				break;
			case HouseAction::Upgrade:
				UpgradeHouse(player, town, playerIndex);
				break;
			case HouseAction::Finish:
				FinishHouse(player, town, playerIndex);
				break;
			case HouseAction::Edit:
				EditExterior(town, playerIndex);
				break;
			case HouseAction::Tutorial:
				LoadHouseEditingTutorial(false);
				break;
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
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		static const std::vector<std::string> buildingOpt = {
			Language::getInstance()->get(TextID::QUICK_MENU_PLACE_AT_LOCATION),
			Language::getInstance()->get(TextID::QUICK_MENU_MOVE_TO_LOCATION),
			Language::getInstance()->get(TextID::QUICK_MENU_REMOVE_BUILDING),
		};

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), buildingOpt);

		switch(optKb.Open()) {
			case 0:
				u8 id;
				if(Wrap::KB<u8>(Language::getInstance()->get(TextID::ENTER_ID), 1, 2, id, 0, onBuildingChange)) {
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
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
			Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY),
		};

		static const std::vector<std::string> musOpt = {
			Language::getInstance()->get(TextID::VECTOR_ENZY_FILL),
			Language::getInstance()->get(TextID::VECTOR_ENZY_CLEAR),
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

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), musOpt);
		int state = optKb.Open();
		if(state < 0) {
			return;
		}

		else if(state == 0) {
			optKb.Populate(pV);
			int player = optKb.Open();
			if(player < 0 && pV[player] == (Color::Silver << Language::getInstance()->get(TextID::SAVE_PLAYER_EMPTY))) {
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
		if(!(Game::IsMapOpened() && Game::IsGameInRoom(0) && Player::GetSaveOffset(4) != 0)) {
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
		
		Top.DrawSysfont(((acreID <= 0) ? "    " : "\uE052 ") + Utils::Format(Language::getInstance()->get(TextID::ACRE_EDITOR_ACRE).c_str(), acreID) + ((acreID >= 0xD6) ? "  " : " \uE053"), 145, 70);
	
		Top.DrawSysfont(Language::getInstance()->get(TextID::ACRE_EDITOR_SET), 100, 160);
		Top.DrawSysfont(Language::getInstance()->get(TextID::ACRE_EDITOR_CONFIRM), 140, 180);
		Top.DrawSysfont(Language::getInstance()->get(TextID::ACRE_EDITOR_CANCEL), 140, 200);
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
			OSD::Notify(Language::getInstance()->get(TextID::ACRE_EDITOR_INVALID), Color::Red);
			return false;
		}

		if(IDList::IsHalfAcre(AcreID)) {
			OSD::Notify(Language::getInstance()->get(TextID::ACRE_EDITOR_HALF_ACRE_ERROR), Color::Red);
			return false;
		}

		if(!SetAcre(AcreID)) {
			OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::ACRE_EDITOR_PATH_MISSING).c_str(), Utils::Format(PATH_ACRE, AcreID)), Color::Red);
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
					OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::ACRE_EDITOR_PATH_MISSING).c_str(), Utils::Format(PATH_ACRE, AcreID)), Color::Red);
					break;
				}
			}
			if(Controller::IsKeyPressed(Key::R) && AcreID < 0xD6) {
				AcreID++;

				while(IDList::IsHalfAcre(AcreID)) {
					AcreID++;
				}

				if(!SetAcre(AcreID)) {
					OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::ACRE_EDITOR_PATH_MISSING).c_str(), Utils::Format(PATH_ACRE, AcreID)), Color::Red);
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

		Keyboard Acre(Utils::Format(Language::getInstance()->get(TextID::MAP_EDITOR_TYPE_ID).c_str(), acre));
		if(Acre.Open(AcreID, AcreID) < 0) {
			return;
		}

		if(!IsAcreOkay(AcreID)) {
			return;
		}

		town->TownAcres[offset] = AcreID;

		OSD::Notify(Utils::Format(Language::getInstance()->get(TextID::ACRE_EDITOR_ACRE_2).c_str(), acre, AcreID));

		u32 pInstance = PlayerClass::GetInstance()->Offset();
		u32 aInstance = Animation::GetAnimationInstance(pInstance, 0, 0, 0);

		AnimData data;
		data.Init(aInstance, pInstance, 4);
		data.Congrats_2A();
		data.ExecuteAnimation(0x2A);

		OSD::Notify(Language::getInstance()->get(TextID::ACRE_EDITOR_SAVE_QUIT));
	}

//Map Editor
	void MapEditor(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu += GetAcreID;
		}

		bool IsOkay = (Game::IsMapOpened() && Game::IsGameInRoom(0) && Player::GetSaveData());
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
