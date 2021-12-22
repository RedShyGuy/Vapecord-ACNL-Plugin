#include "cheats.hpp"
#include "Helpers/PlayerClass.hpp"
#include "TextFileParser.hpp"
#include "Helpers/Town.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/PlayerPTR.hpp"
#include "Helpers/AnimData.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/NPC.hpp"
#include "Color.h"
#include "Files.h"

namespace CTRPluginFramework {
//Town Name Changer | player specific save code	
	void townnamechanger(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			Sleep(Milliseconds(100));
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

        Keyboard keyboard(Language->Get("TOWN_NAME_CHANGER_ENTER_NAME"));
        std::string input;
        keyboard.SetMaxLength(8);

		Sleep(Milliseconds(100));
        if(keyboard.Open(input) < 0) 
			return;

		TownName::Set(input);
    }

//Save Backup and Restore | non player specific save code
	void savebackup(MenuEntry *entry) {
		static const std::vector<std::string> options = {
			Language->Get("SAVE_DUMPER"), 
			Language->Get("SAVE_RESTORE"),  
			Language->Get("FILE_DELETE"),  
		};
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"), options);

		Sleep(Milliseconds(100));
		switch(KB.Open()) {
			default: break;
			case 0: {
				std::string filename = "";
				Keyboard KB(Language->Get("SAVE_DUMPER_DUMP"));

				Sleep(Milliseconds(100));
				if(KB.Open(filename) == -1)
					return;

				Wrap::Dump(Utils::Format(PATH_SAVE, regionName.c_str()), filename, ".dat", WrapLoc{ Save::GetInstance()->Address(), 0x89B00 }, WrapLoc{ (u32)-1, (u32)-1 });		
			} break;
			case 1: {
				if(Wrap::Restore(Utils::Format(PATH_SAVE, regionName.c_str()), ".dat", Language->Get("SAVE_RESTORE_SELECT"), nullptr, true, WrapLoc{ Save::GetInstance()->Address(), 0x89B00 }, WrapLoc{ (u32)-1, (u32)-1 }) == ExHandler::SUCCESS) {
					static Address fixfurno(0x6A6EE0, 0x6A6408, 0x6A5F18, 0x6A5F18, 0x6A59B0, 0x6A59B0, 0x6A5558, 0x6A5558);	
					u32 orig[1] = { 0 };

					Process::Patch(fixfurno.addr + 0x41C, 0xE1A00000, orig);
								
					fixfurno.Call<void>();

					Process::Patch(fixfurno.addr + 0x41C, orig[0]);
				} 
			} break;
			case 2: 
				Wrap::Delete(Utils::Format(PATH_SAVE, regionName.c_str()), ".dat");
			break;
		}
	} 

//Bulletinboard message backup/restore | non player specific save code
	void bullboard(MenuEntry *entry) {		
		static const std::vector<std::string> bullsett = {
			Language->Get("VECTOR_BULLETINDUMPER_BACKUP"),
			Language->Get("VECTOR_BULLETINDUMPER_RESTORE"),
			Language->Get("FILE_DELETE"),  
		};
		
		std::vector<std::string> backmessage;
		
		for(int i = 1; i <= 15; ++i)
			backmessage.push_back(Utils::Format(Language->Get("VECTOR_BULLETINDUMPER_MESSAGE").c_str(), i));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), bullsett);
	
		Sleep(Milliseconds(100));
		switch(optKb.Open()) {
			default: break;
			case 0: {
				optKb.Populate(backmessage);

				Sleep(Milliseconds(100));
				s8 KBChoice = optKb.Open();
				if(KBChoice >= 0) {
					std::string filename = "";
					Keyboard KB(Language->Get("BULLETIN_BOARD_DUMPER_DUMP"));

					Sleep(Milliseconds(100));
					if(KB.Open(filename) < 0)
						return;

					Wrap::Dump(Utils::Format(PATH_BULLETIN, regionName.c_str()), filename, ".dat", WrapLoc{ Player::GetBulletin(KBChoice), 0x1AC }, WrapLoc{ (u32)-1, (u32)-1 });
				}
			} break;
			
			case 1: {
				optKb.Populate(backmessage);

				Sleep(Milliseconds(100));
				s8 KBChoice = optKb.Open();
				if(KBChoice >= 0)
					Wrap::Restore(Utils::Format(PATH_BULLETIN, regionName.c_str()), ".dat", Language->Get("RESTORE_MESSAGE"), nullptr, true, WrapLoc{ Player::GetBulletin(KBChoice), 0x1AC }, WrapLoc{ (u32)-1, (u32)-1 }); 
			} break;
			
			case 2: 
				Wrap::Delete(Utils::Format(PATH_BULLETIN, regionName.c_str()), ".dat");
			break;
		}
	}
//Tree Size Changer | non player specific save code
	void TreeSizeChanger(MenuEntry *entry) {
		std::vector<std::string> treesizevec;
		
		for(int i = 1; i <= 8; ++i)
			treesizevec.push_back(Utils::Format(Language->Get("TREESIZE_STATE").c_str(), i));
		
		constexpr int played[8] = { 0, 5, 20, 50, 100, 180, 300, 500 };
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), treesizevec);
		
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
		
		Save::GetInstance()->Write<u8>(0x4BE86, op);
		GameHelper::SetPlayedHours(played[op]); 
		GameHelper::SetPlayedDays(played[op]); 
    }
//Change Native Fruit | non player specific save code	
	void ChangeNativeFruit(MenuEntry *entry) {
		std::vector<std::string> fruitopt = {
            Language->Get("NATIVE_FRUIT_APPLE"),
            Language->Get("NATIVE_FRUIT_ORANGE"),
            Language->Get("NATIVE_FRUIT_PEAR"),
            Language->Get("NATIVE_FRUIT_PEACH"),
            Language->Get("NATIVE_FRUIT_CHERRIE"),
            Language->Get("NATIVE_FRUIT_COCONUT"),
            Language->Get("NATIVE_FRUIT_DURIAN"),
            Language->Get("NATIVE_FRUIT_LEMON"),
            Language->Get("NATIVE_FRUIT_LYCHEE"),
            Language->Get("NATIVE_FRUIT_MANGO"),
            Language->Get("NATIVE_FRUIT_PERSIMMON"),
            Language->Get("NATIVE_FRUIT_BANANA")
        };
		
		bool IsON;

		for(int i = 0; i < 12; ++i) {
			IsON = *(u8 *)Save::GetInstance()->Address(0x6223A) == (i + 1);
			fruitopt[i] = IsON ? (Color(pGreen) << fruitopt[i]) : (fruitopt[i] = Color(pRed) << fruitopt[i]);
		}

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), fruitopt);

		Sleep(Milliseconds(100));
		s8 userChoice = optKb.Open();
		if(userChoice < 0)
			return;
		
		Save::GetInstance()->Write<u8>(0x6223A, userChoice + 1);
		
		ChangeNativeFruit(entry);
	}
//Unlock All PWP | non player specific save code	
	void PWPUnlock(MenuEntry *entry) {	
		static const std::vector<std::string> songopt = {
			Language->Get("VECTOR_ENZY_FILL"),
			Language->Get("VECTOR_ENZY_CLEAR"),
		};
			
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), songopt);
		
		Sleep(Milliseconds(100));
		switch(optKb.Open()) {
			default: break;
			case 0:
				std::memset((void *)Save::GetInstance()->Address(0x50330), 0xFFFFFFFF, 0x18);
			break;
			case 1: 
				std::memset((void *)Save::GetInstance()->Address(0x50330), 0, 0x18);
			break;
		}
    }

//Grass Type Changer | non player specific save code		
	void GrassChanger(MenuEntry *entry) {
		std::vector<std::string> grasstypevec = {
			Language->Get("GRASS_CHANGER_TRIANGLE"),
            Language->Get("GRASS_CHANGER_CIRCLE"),
            Language->Get("GRASS_CHANGER_SQUARE")
		};	
		
		bool IsON;
		
		for(int i = 0; i < 3; ++i) {
			IsON = *(u8 *)Save::GetInstance()->Address(0x53481) == i;
			grasstypevec[i] = IsON ? (Color(pGreen) << grasstypevec[i]) : (Color(pRed) << grasstypevec[i]);
		}
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), grasstypevec);

		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
	
		Save::GetInstance()->Write<u8>(0x53481, op);
		GrassChanger(entry);
    }

	void caravanchange(MenuEntry *entry) {
		int caravan = 0;

		std::vector<std::string> keyVec = {
			Language->Get("CARAVAN_SET_LEFT"),
			Language->Get("CARAVAN_SET_RIGHT")
		};

		Keyboard keyboard(Language->Get("CARAVAN_SET_SELECT"), keyVec);

		Sleep(Milliseconds(100));
		s8 res = keyboard.Open(); //Pick caravan
		if(res < 0)
			return;

		caravan = res;

		keyboard.GetMessage() = std::string(Language->Get("AMIIBO_SPOOFER_SPECIES"));
		keyVec.clear();

		NPC::PopulateRace(keyVec);

        keyboard.Populate(keyVec);

		Sleep(Milliseconds(100));
        res = keyboard.Open(); //Pick a species
        if(res < 0) //User picked a species
			return;

		keyboard.GetMessage() = std::string(Language->Get("AMIIBO_SPOOFER_VILLAGER"));
		keyVec.clear();

		std::vector<PACKED_AmiiboInfo> amiiboVec;
		NPC::PopulateAmiibo((SpecieID)res, keyVec, amiiboVec, true, false);

		keyboard.Populate(keyVec);

		Sleep(Milliseconds(100));
		res = keyboard.Open(); //Pick villager based on species
		if(res < 0)
			return;

		const PACKED_AmiiboInfo& amiibo = amiiboVec[res];

		Save::GetInstance()->Write<u16>(0x6AE60 + (2 * caravan), amiibo.VID);
		OSD::Notify(Utils::Format("Set %s in caravan %1d", amiibo.Name.c_str(), caravan));
	}

	void SetCampingVillager(MenuEntry *entry) {
		std::vector<std::string> keyVec = {
			Language->Get("CAMPING_SET_SET"),
			Language->Get("CAMPING_SET_REMOVE")
		};

		Keyboard keyboard(Language->Get("KEY_CHOOSE_OPTION"), keyVec);	

		Sleep(Milliseconds(100));
		s8 res = keyboard.Open();
       	if(res < 0)
			return;
		
		static const u32 B_Removal = Save::GetInstance()->Address(0x4BE88);
		static Address SetNPCFunc(0x3081A8, 0x308380, 0x308234, 0x308234, 0x3081C8, 0x3081C8, 0x3081DC, 0x3081DC);
		static Address DeleteNPCFunc(0x30836C, 0x3084D0, 0x308384, 0x308384, 0x308474, 0x308474, 0x30832C, 0x30832C);

		if(res == 0) {
			Keyboard keyboard("a");
			keyboard.GetMessage() = std::string(Language->Get("AMIIBO_SPOOFER_SPECIES"));
			keyVec.clear();

			NPC::PopulateRace(keyVec);
			keyVec.pop_back(); //Removes Special Characters from vec

			keyboard.Populate(keyVec);

			Sleep(Milliseconds(100));
			s8 res = keyboard.Open(); //Pick a species
			if(res < 0) //User picked a species
				return;

			keyboard.GetMessage() = std::string(Language->Get("AMIIBO_SPOOFER_VILLAGER"));
			keyVec.clear();

			std::vector<PACKED_AmiiboInfo> amiiboVec;
			NPC::PopulateAmiibo((SpecieID)res, keyVec, amiiboVec, false, false);

			keyboard.Populate(keyVec);

			Sleep(Milliseconds(100));
			res = keyboard.Open(); //Pick villager based on species
			if(res < 0)
				return;

			const PACKED_AmiiboInfo& amiibo = amiiboVec[res];

			for(int i = 0; i < 56; ++i) {
				if(*(u8 *)(B_Removal + (i * 4)) == 0xC6)
					*(u8 *)(B_Removal + (i * 4)) = 0x5F;
			}

			u32 null[]{ 0 };
			u16 VID[]{ amiibo.VID };

			SetNPCFunc.Call<void>(Save::GetInstance()->Address(0x292A4 + 0x17676), VID, null, Save::GetInstance()->Address(0x621B8)); 
			OSD::Notify(Utils::Format("Set %s!", amiibo.Name.c_str()), Color::Green);

			if(GameHelper::IsInRoom(0))
				GameHelper::ReloadRoom();
		}

		else if(res == 1) {
			for(int i = 0; i < 56; ++i) {
				if(*(u8 *)(B_Removal + (i * 4)) == 0x5F)
					*(u8 *)(B_Removal + (i * 4)) = 0xC6;
			}

			DeleteNPCFunc.Call<void>(Save::GetInstance()->Address(0x292A4 + 0x17676));
			OSD::Notify("Camping Villager Removed!", Color::Red);

			if(GameHelper::IsInRoom(0))
				GameHelper::ReloadRoom();
		}
	}

//get if specific nook is unlocked
	bool IsNookUnlocked(int value) {
		return(*(u32 *)Save::GetInstance()->Address(0x62264) / 0x0101 == value);
	}
//Help File For Shop Unlocker
	void nook_common(int value) {
		Save::GetInstance()->Write<u32>(0x62264, 0x0101 * value);
		Save::GetInstance()->Write<u8>(0x666F4, (value == 1 ? 2 : value));
	}
//Shop Unlocker
    void shopunlocks(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			Sleep(Milliseconds(100));
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> shopopt = {
			Language->Get("VECTOR_SHOP_NOOK"), 	
			Language->Get("VECTOR_SHOP_FORTUNE"), 
			Language->Get("VECTOR_SHOP_DREAM"), 
			Language->Get("VECTOR_SHOP_CLUB"), 	
			Language->Get("VECTOR_SHOP_MUSEUM"), 
			Language->Get("VECTOR_SHOP_SHAMPOODLE"), 
			Language->Get("VECTOR_SHOP_KICKS") 
		};
		
		std::vector<std::string> nookopt = {
			Language->Get("VECTOR_SHOP_CRANNY"),
			Language->Get("VECTOR_SHOP_TT_MART"),
			Language->Get("VECTOR_SHOP_SUPER_TT"),
			Language->Get("VECTOR_SHOP_TIY"),
			Language->Get("VECTOR_SHOP_TT_EMPORIUM")
		};
		
		const u32 Shops[7] = {
			0,		 //just for loop
			0x6ADA4, //Fortune Teller
			0x6ADA2, //Dream Suite
			0x6AD82, //Club LOL
			0x6ACBC, //Museum Shop
			0x6ADB4, //Shampoodle
			0x6682C, //Kicks
		};
		
		const int ShopValues[7] = {
			0,		//Just for loop
			1,		//Fortune Teller
			1,		//Dream Suite
			2,		//Club LOL
			1,		//Museum Shop
			2,		//Shampoodle
			2,		//Kicks
		};

		bool IsON;
		
		for(int i = 1; i < 7; ++i) { 
			IsON = *(bool *)Save::GetInstance()->Address(Shops[i]) != 0;
			shopopt[i] = (IsON ? Color(pGreen) : Color(pRed)) << shopopt[i];
		}
		
		Keyboard shopkb(Language->Get("KEY_CHOOSE_STORE"), shopopt);

		Sleep(Milliseconds(100));
		s8 op = shopkb.Open();
		if(op < 0)
			return;
		
	//op 0 which is a special case cause nook has multiple states
		if(op == 0) {
			for(int i = 0; i < 5; ++i) { 
				nookopt[i] = (IsNookUnlocked(i) ? Color(pGreen) : Color(pRed)) << nookopt[i];
			}
			
			Keyboard nookkb(Language->Get("KEY_CHOOSE_UPGRADE"), nookopt); 

			Sleep(Milliseconds(100));
			s8 nook = nookkb.Open();
			if(nook < 0)
				return;
				
			nook_common(nook);
			shopunlocks(entry);
			return;
		}
		
		Save::GetInstance()->Write<u8>(Shops[op], (*(bool *)Save::GetInstance()->Address(Shops[op]) != 0 ? 0 : ShopValues[op]));
	
	//op 3 is also special cause it removes shrunk if true
		if(op == 3) {
			if(*(bool *)Save::GetInstance()->Address(Shops[op]) != 0) {
			//To remove shrunk if he is in room 1
				u8 byte = *(u8 *)PlayerPTR::Pointer(0x5714);
				Process::Write8(PlayerPTR::Pointer(0x5714), byte | 0x4);
			}
		}
		shopunlocks(entry);
	}

	void HouseChanger(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			Sleep(Milliseconds(100));
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		if(GameHelper::GetOnlinePlayerCount() != 0) {	
			Sleep(Milliseconds(100));		
			MessageBox(Language->Get("ONLY_TOWN_ERROR")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};
		
		static const std::vector<std::string> HouseSet = {
			Language->Get("HOUSE_EDITOR_EXT"),
			Language->Get("HOUSE_EDITOR_INT"),
		};
		
		static const std::vector<std::string> HouseSettings = {
			Language->Get("HOUSE_EDITOR_HOUSE_SIZE"),
			Language->Get("HOUSE_EDITOR_HOUSE_STYLE"),
			Language->Get("HOUSE_EDITOR_HOUSE_DOORSHAPE"),
			Language->Get("HOUSE_EDITOR_HOUSE_BRICK"),
			Language->Get("HOUSE_EDITOR_HOUSE_ROOF"),
			Language->Get("HOUSE_EDITOR_HOUSE_DOOR"),
			Language->Get("HOUSE_EDITOR_HOUSE_FENCE"),
			Language->Get("HOUSE_EDITOR_HOUSE_PAVEMENT"),
			Language->Get("HOUSE_EDITOR_HOUSE_MAILBOX"),
		};
		
		static const std::vector<std::string> HouseInfo = {
			Language->Get("HOUSE_EDITOR_HOUSE_SET1"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET2"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET3"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET4"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET5"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET6"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET7"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET8"),
			Language->Get("HOUSE_EDITOR_HOUSE_SET9"),
		};
		
		static const std::vector<std::string> RoomSet = {
			Language->Get("HOUSE_EDITOR_ROOM_MIDDLE"),
			Language->Get("HOUSE_EDITOR_ROOM_SECOND"),
			Language->Get("HOUSE_EDITOR_ROOM_BASEMENT"),
			Language->Get("HOUSE_EDITOR_ROOM_RIGHT"),
			Language->Get("HOUSE_EDITOR_ROOM_LEFT"),
			Language->Get("HOUSE_EDITOR_ROOM_BACK"),
		};
		
		static const std::vector<std::string> RoomInfo = {
			Language->Get("HOUSE_EDITOR_ROOM_SET1"),
			Language->Get("HOUSE_EDITOR_ROOM_SET2"),
			Language->Get("HOUSE_EDITOR_ROOM_SET3"),
			Language->Get("HOUSE_EDITOR_ROOM_SET4"),
			Language->Get("HOUSE_EDITOR_ROOM_SET5"),
			Language->Get("HOUSE_EDITOR_ROOM_SET6"),
		};
		
		static constexpr u8 ValidHouseValues[9][2] = {
			{ 0, 7 },
			{ 0, 3 },
			{ 0, 1 },
			{ 0, 0x1F },
			{ 0, 0x22 },
			{ 0, 0x15 },
			{ 0, 0x16 },
			{ 0, 8 },
			{ 0, 0x15 },
		};
		
		static constexpr u8 ValidRoomValues[6][2] = {
			{ 0, 4 },
			{ 1, 4 },
			{ 1, 4 },
			{ 1, 4 },
			{ 1, 4 },
			{ 1, 4 },
		};
		
		for(int i = 0; i <= 3; ++i) {
			u32 pO = Player::GetSpecificSave(i);
			if(*(u16 *)(pO + 0x55A6) != 0) {
				std::string pS = "";
				Process::ReadString((pO + 0x55A8), pS, 0x10, StringFormat::Utf16);
				pV[i] = pColor[i] << pS;
			}
		}
		
		Keyboard pKB(Language->Get("KEY_SELECT_PLAYER"), pV);
	
		Sleep(Milliseconds(100));
		s8 pChoice = pKB.Open();
		if((pChoice < 0) || (pV[pChoice] == Color::Silver << "-Empty-")) 
			return;
		
		Keyboard hKB(Language->Get("KEY_CHOOSE_OPTION"), HouseSet);
		
		Sleep(Milliseconds(100));
		s8 hChoice = hKB.Open();
		if(hChoice < 0)
			return;
	
	/*Exterior House Settings*/
		if(hChoice == 0) {
			Keyboard eKB(Language->Get("HOUSE_EDITOR_SELECT_HOUSE"), HouseSettings);

			Sleep(Milliseconds(100));
			s8 eChoice = eKB.Open();
			if(eChoice < 0)
				return;
			
			u8 Value = 0;
		//Gets Save Offset for house base
			u32 ExteriorBase = Save::GetInstance()->Address(0x5D904 + (0x1228 * pChoice));

			if(Wrap::KB<u8>(HouseInfo[eChoice], true, 2, Value, *(u8 *)(ExteriorBase + eChoice))) {
				if(!IDList::ValidID(Value, ValidHouseValues[eChoice][0], ValidHouseValues[eChoice][1])) {
					Sleep(Milliseconds(100));
					MessageBox(Language->Get("INVALID_ID")).SetClear(ClearScreen::Top)();
					return;
				}
				Process::Write8(ExteriorBase + eChoice, Value);
				Process::Write8(ExteriorBase + (9 + eChoice), Value);
			}
			return;
		}
	
	/*Interior Room Size Settings*/
		Keyboard rKB(Language->Get("HOUSE_EDITOR_SELECT_ROOM"), RoomSet);

		Sleep(Milliseconds(100));
		s8 rChoice = rKB.Open();
		if(rChoice < 0)
			return;
		
		u8 RoomSize = 0;
		
		u32 InteriorSizeBase = Save::GetInstance()->Address(0x5D936 + (0x1228 * pChoice) + (0x302 * rChoice));
	
		if(Wrap::KB<u8>(RoomInfo[rChoice], true, 2, RoomSize, *(u8 *)(InteriorSizeBase))) {
			if(!IDList::ValidID(RoomSize, ValidRoomValues[rChoice][0], ValidRoomValues[rChoice][1])) {
				Sleep(Milliseconds(100));
				MessageBox(Language->Get("INVALID_ID")).SetClear(ClearScreen::Top)();
				return;
			}
			Process::Write8(InteriorSizeBase, RoomSize);
		}
	}

//Unlock QR Machine | half player specific save code	
	void unlockqrmachine(MenuEntry *entry) {	
		if(Player::GetSaveOffset(4) == 0) {
			Sleep(Milliseconds(100));
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> cmnOpt =  { "" };

		bool IsON1 = *(u16 *)Save::GetInstance()->Address(0x621D4) == 0x8000;
		bool IsON2 = *(u16 *)PlayerPTR::Pointer(0x5710) >= 0x955;
		
		cmnOpt[0] = (IsON1 && IsON2) ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), cmnOpt);

		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
		
		Save::GetInstance()->Write<u16>(0x621D4, (IsON1 ? 0 : 0x8000));
		Process::Write16(PlayerPTR::Pointer(0x5710), IsON2 ? 0x880 : 0x955);
		unlockqrmachine(entry);
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
			Sleep(Milliseconds(100));
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		static const std::vector<std::string> buildingOpt = {
			Language->Get("QUICK_MENU_PLACE_AT_LOCATION"),
			Language->Get("QUICK_MENU_MOVE_TO_LOCATION"),
			Language->Get("QUICK_MENU_REMOVE_BUILDING"),
		};

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), buildingOpt);

		Sleep(Milliseconds(100));
		switch(optKb.Open()) {
			case 0:
				u8 id;
				if(Wrap::KB<u8>(Language->Get("ENTER_ID"), 1, 2, id, 0, onBuildingChange)) {
					GameHelper::PlaceBuilding(id);
				}
				break;
			case 1:
				GameHelper::MoveBuilding();
				break;
			case 2:
				GameHelper::RemoveBuilding();
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

//Finish Mayor Permit | only for player 1	
	void Permit100(MenuEntry *entry) {
		if(Player::GetSaveOffset(0) == 0) {
			entry->Disable();
			return;
		}
		Process::Write8(PlayerPTR::Pointer(0x571A), (*(u8 *)(PlayerPTR::Pointer(0x571A)) & 1) | 0xC8);
		entry->Disable();
    }

	std::string S_AcreID[4][5] = { "" };
	static constexpr int I_RectXPos[5] = { 70, 106, 142, 178, 214 };
	static constexpr int J_RectYPos[4] = { 54, 90, 126, 162 };
	static constexpr int Addage[4] = { 0x00, 0x0E, 0x1C, 0x2A };

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
		if(!(GameHelper::MapBoolCheck() && GameHelper::IsInRoom(0) && Player::GetSaveOffset(4) != 0)) 
			return;

		static u32 townacre = Save::GetInstance()->Address(0x53494);

		for(int j = 0; j < 4; ++j) 
			for(int i = 0; i < 5; ++i) 
				S_AcreID[j][i] = (Utils::Format("%02X", *(u8 *)((townacre + Addage[j]) + (i * 2))));
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
				if(cPix != Color(0, 0, 0, 0))
					Top.DrawPixel(X, Y, cPix);

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
		if(!Test.IsOpen())
			return 0;

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
		
		Process::Play();
		delete[] acreArray;

		return okay;
	}

//Map Function
	void AcreChange(int acre, u8 offset) {
		u8 AcreID = 0;
		u32 townacre = Save::GetInstance()->Address(0x53494); 

		Keyboard Acre(Utils::Format(Language->Get("MAP_EDITOR_TYPE_ID").c_str(), acre));
		Sleep(Milliseconds(200));
		if(Acre.Open(AcreID, AcreID) < 0) 
			return;

		Sleep(Milliseconds(100));
		if(!IsAcreOkay(AcreID)) 
			return;

		Process::Write8(townacre + offset, AcreID);
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

		bool IsOkay = (GameHelper::MapBoolCheck() && GameHelper::IsInRoom(0) && Player::GetSaveOffset(4) != 0);
		static bool WasActivated = false;
		int res = 0;

		if(IsOkay) {
			if(Touch::IsDown()) {
				for(int j = 0; j < 4; ++j) {
					for(int i = 0; i < 5; ++i) {
						UIntRect rec(I_RectXPos[i], J_RectYPos[j], 36, 36);
						res++;

						if(!rec.Contains(Touch::GetPosition()))
							continue;
						
						AcreChange(res, Addage[j] + (i * 2));
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
