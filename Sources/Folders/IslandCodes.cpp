#include "cheats.hpp"
#include "Helpers/Game.hpp"
#include "TextFileParser.hpp"
#include "Helpers/CROEditing.hpp"
#include "RegionCodes.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/ItemSequence.hpp"
#include "Helpers/Dropper.hpp"
#include "Helpers/Save.hpp"
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
		const u32 ClubTortimer = Save::GetInstance()->Address(0x57BF);
		const u32 Island = Save::GetInstance()->Address(0x57B2);
		u8 value1 = 0, value2 = 0;

		std::vector<std::string> cmnOpt =  {
			Language->Get("UNLOCK_ISLAND_ISL"),
			Language->Get("UNLOCK_ISLAND_CLUB")
		};

		Set::ReadNibble(Island, value1, false);
		Set::ReadNibble(ClubTortimer, value2, true);

		bool IsON;

		IsON = value1 >= 8;

		cmnOpt[0] = (IsON ? Color(pGreen) : Color(pRed)) << cmnOpt[0];
			
		IsON = value2 >= 4;

		cmnOpt[1] = (IsON ? Color(pGreen) : Color(pRed)) << cmnOpt[1];
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"), cmnOpt);

		Sleep(Milliseconds(100));
		s8 op = KB.Open();
		if(op < 0)
			return;

		if(op == 0) {
			if(value1 < 8) //under 8 is locked and everything above is unlocked
				Set::WriteNibble(Island, 8, false);
			else //if above or 8 Island is unlocked
				Set::WriteNibble(Island, 0, false);

			UnlockIsland(entry);
		}

		if(op == 1) {
			if(value2 < 4) //below 4 is locked club tortimer | everything above is unlocked
				Set::WriteNibble(ClubTortimer, 4, true); 
			else //if above or 4 club tortimer is unlocked
				Set::WriteNibble(ClubTortimer, 0, true);

			UnlockIsland(entry);
		}
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
//Hacker Island Spoof	
	void Hackerisland(MenuEntry *entry)	{ 
		std::vector<std::string> cmnOpt =  { "" };

		bool IsON = *(u32 *)Code::country.addr == 0xE3A000FF;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), cmnOpt);

		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
		
		Process::Patch(Code::country.addr, IsON ? 0xE1A00C20 : 0xE3A000FF);
		Hackerisland(entry);
	}
//InputChangeEvent For Country Spoof
	void onCountryChange(Keyboard &k, KeyboardEvent &e) {
		if(e.type == KeyboardEvent::CharacterRemoved || e.type == KeyboardEvent::CharacterAdded) {
			std::string s = k.GetInput();
			k.GetMessage() = "ID:\n\n" << IDList::SetCountryName(!s.empty() ? std::stoi(s, nullptr, 16) : 0);
		}
	}
//Country Spoof	
	void Countryspoof(MenuEntry *entry) {
		static const std::vector<std::string> countOpt = {
			Language->Get("VECTOR_COUNTY_CHANGE"), 
			Language->Get("VECTOR_DISABLE")
		};
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), countOpt);
        static u8 input = 0;
		
		Sleep(Milliseconds(100));
		switch(optKb.Open()) {
			default: break;	
			case 0: 
				if(Wrap::KB<u8>(Language->Get("ISLAND_COUNTRY_SET_ID"), true, 2, input, 0, onCountryChange)) 
					Process::Patch(Code::country.addr, 0xE3A00000 + input); 
			break;	
			case 1: 
				Process::Patch(Code::country.addr, 0xE1A00C20); 
			break;	
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
			SetItem.GetMessage() = Utils::Format(Language->Get("ISLAND_SHOP_MOD_ENTER_ID").c_str(), i + 1);

			Sleep(Milliseconds(100));
			s8 res = SetItem.Open(*(u32 *)&ShopItem[i]);
			if(res < 0)
				break;
		}
	}
//Island Shop
	void IslandShop(MenuEntry *entry) {	
		static const Address IslandShopPointer(0x954238, 0x953228, 0x953238, 0x953238, 0x94D238, 0x94C238, 0x94C238, 0x94C238);
		if(*(u32 *)IslandShopPointer.addr == 0)
			return;
		
		if(GameHelper::NextRoomCheck() == 0xA5 && GameHelper::RoomCheck() == 0x65) {
			for(int i = 0; i < 4; ++i) {
				Process::Write32(*(u32 *)IslandShopPointer.addr + 0x10 + (i * 4), IDList::ItemValid(ShopItem[i], false) ? *(u32 *)&ShopItem[i] : 0x2018);
			}
		}
	}
	
//All Tours
	void alltour(MenuEntry *entry) {
		static const Address TourPatch(0x76FCC0, 0x76ECA4, 0x76ECC8, 0x76ECA0, 0x76E460, 0x76E438, 0x76E008, 0x76DFE0);
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
		if(*(u32 *)Code::IslPointer.addr == 0)
			return;
		
		u32 IslAcreOffset = *(u32 *)Code::IslPointer.addr + 2; //0x953708
		
		for(u8 i = 0; i < 16; ++i) 
			Process::Write8(IslAcreOffset + i * 2, isl.acres[i]);
	}
//Island Acre Mod Keyboard
	void menuAcreMod(MenuEntry *entry) {
		Keyboard kb(Language->Get("ENTER_ID"));
		kb.SetMaxLength(2);
		kb.IsHexadecimal(true);
		for(u8 i = 0; i < 16; ++i) {
			kb.GetMessage() = Utils::Format(Language->Get("ISLAND_ACRE_ENTER_ID").c_str(), i + 1);

			Sleep(Milliseconds(100));
			kb.Open(isl.acres[i], isl.acres[i]);
		}
	}
//Island Building Mod	
	void buildingMod(MenuEntry *entry) {
		if(*(u32 *)Code::IslPointer.addr == 0)
			return;
		
		u32 islandBuildings = *(u32 *)Code::IslPointer.addr + 0x1022;
		
		for(u8 i = 0; i < 2; ++i) {
			Process::Write16(islandBuildings + i * 4, isl.b[i].id);
			Process::Write8(islandBuildings + 2 + i * 4, isl.b[i].x);
			Process::Write8(islandBuildings + 3 + i * 4, isl.b[i].y);
		}
	}
//Island Building Mod Keyboard
	void menuBuildingMod(MenuEntry *entry) {
		Keyboard kb(Language->Get("ENTER_ID"));
		kb.SetMaxLength(2);
		kb.IsHexadecimal(true);
		for(u8 i = 0; i < 2; ++i) {
			kb.GetMessage() = Language->Get("ISLAND_BUILDING_ENTER_ID") << Utils::Format(" %d", i + 1);
			Sleep(Milliseconds(100));
			kb.Open(isl.b[i].id, isl.b[i].id);
			kb.GetMessage() = Language->Get("ISLAND_BUILDING_ENTER_X") << Utils::Format(" %d", i + 1);
			Sleep(Milliseconds(100));
			kb.Open(isl.b[i].x, isl.b[i].x);
			kb.GetMessage() = Language->Get("ISLAND_BUILDING_ENTER_Y") << Utils::Format(" %d", i + 1);
			Sleep(Milliseconds(100));
			kb.Open(isl.b[i].y, isl.b[i].y);
		}
	}

	void FreeKappn(MenuEntry *entry) {
		static Hook hook1, hook2;
		static const Address kappn1(0x5DC048, 0x5DB578, 0x5DB090, 0x5DB090, 0x5DA910, 0x5DA910, 0x5DA598, 0x5DA598);
		static const Address kappn2(0x5DAF98, 0x5DA4C8, 0x5D9FE0, 0x5D9FE0, 0x5D9814, 0x5D9814, 0x5D94E8, 0x5D94E8);

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

	void RestoreIsland(u32 fileData) {
		bool ItemSequenceWasON = false;

		if(ItemSequence::Enabled()) {
			ItemSequenceWasON = true;
			ItemSequence::Switch(false);
		}

		if(!bypassing) 
			Dropper::DropItemLock(true);

		u32 count = 0;
		u32 x = 0x10, y = 0x10;
		static int nextItem = 0;

		bool res = true;
		while(res) {
			while(res) {
				if(GameHelper::GetItemAtWorldCoords(x, y)) {
					Item var = *(Item *)(fileData + (nextItem++ * 4));
					if(Dropper::PlaceItemWrapper(1, ReplaceEverything, &var, &var, x, y, 0, 0, 0, 0, 0, 6, 0xA5, false)) {
						count++;
						if(count % 300 == 0) 
							Sleep(Milliseconds(500));
					}
				}
				else 
					res = false;

				y++;
			}
			res = true;
			
			y = 0x10;
			x++;
			if(!GameHelper::GetItemAtWorldCoords(x, y)) 
				res = false;
		}		
		
		OSD::Notify(Utils::Format("%d %s", count, "items placed!"));

	//OFF
		if(!bypassing) 
			Dropper::DropItemLock(false);	

		if(ItemSequenceWasON)
			ItemSequence::Switch(true);
	}

	void IslandSaver(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			Sleep(Milliseconds(100));
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		if(!GameHelper::IsInRoom(0x68)) {
			Sleep(Milliseconds(100));
			MessageBox("You need to be on the island for this cheat to work").SetClear(ClearScreen::Top)();
			return;
		}

		Item *startPos = GameHelper::GetItemAtWorldCoords(0x10, 0x10);
		Item *endPos = GameHelper::GetItemAtWorldCoords(0x2F, 0x2F);

		Keyboard KB("a", std::vector<std::string>{ "Backup Island", "Restore Island", "Delete Files" });
		s8 index = KB.Open();
		switch(index) {
			default: break;
			case 0: {
				std::string filename = "";
				Keyboard KB("Name the island backup:");

				Sleep(Milliseconds(100));
				if(KB.Open(filename) == -1)
					return;

				//Wrap::Dump(Utils::Format(PATH_ISLAND, regionName.c_str()), filename, ".dat", WrapLoc{ *(u32 *)startPos, *(u32 *)(endPos - startPos) }, WrapLoc{ (u32)-1, (u32)-1 });
			} break;
			case 1: {
				u32 islandFileData = 0xA00000;
				//Wrap::Restore(Utils::Format(PATH_ISLAND, regionName.c_str()), ".dat", "", nullptr, false, WrapLoc{ (u32)islandFileData, 0x1000 }, WrapLoc{ (u32)-1, (u32)-1 });

				RestoreIsland(islandFileData);
			} break;
			case 2: {
				//Wrap::Delete(Utils::Format(PATH_ISLAND, regionName.c_str()), ".dat");
			} break;
		}
	}
}