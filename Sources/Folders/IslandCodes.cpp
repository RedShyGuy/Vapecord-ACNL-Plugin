#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

extern "C" void PATCH_KappnBypass1(void);
extern "C" void PATCH_KappnBypass2(void);

namespace CTRPluginFramework {
//For Acre And Building Mod
	struct Building {
		u16 id;
		u8 x;
		u8 y;
	};

	struct Island {
		Building b[2];
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
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"));
		KB.Populate(cmnOpt);
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

//Fill Inv With Bonus Ore
	void bonusOre(MenuEntry *entry) {
		if(!GameHelper::IsInRoom(0x79) && !GameHelper::IsInRoom(0x73)) {
			MessageBox(Language->Get("ORE_FRUIT_ERROR")).SetClear(ClearScreen::Top)();
			return;
		}
		
		if(*(u32 *)Code::Isl2ndPointer == 0)
			return;
		
		u32 BonusOre = *(u32 *)Code::Isl2ndPointer + 0xC0;
		
		for(int i = 0; i < 16; ++i) 
			Inventory::WriteSlot(i, *(u16 *)BonusOre);

		MessageBox(Language->Get("BONUS_ORE_SPAWNED")).SetClear(ClearScreen::Top)();
	}
//Instant Fruit
	void instantFruit(MenuEntry *entry) {
		static const u32 InstantFruitPatch1 = Region::AutoRegion(0x76FBBC, 0x76EBA0, 0x76EBC4, 0x76EB9C, 0x76E35C, 0x76E334, 0x76DF04, 0x76DEDC);
		static const u32 InstantFruitPatch2 = Region::AutoRegion(0x76FC14, 0x76EBF8, 0x76EC1C, 0x76EBF4, 0x76E3B4, 0x76E38C, 0x76DF5C, 0x76DF34);

		if(entry->WasJustActivated()) {
		//This makes the amount of different fruits needed to 0 (example: peaches, lemon, apple)
			Process::Patch(InstantFruitPatch1, 0xE3A00000);
			Process::Patch(InstantFruitPatch1 + 4, 0xEA000002);
		//This makes the amount of fruits needed to 0 (example: 5 peaches)
			Process::Patch(InstantFruitPatch2, 0xE3A00000);
			Process::Patch(InstantFruitPatch2 + 4, 0xE1A00000);
		}
		else if(!entry->IsActivated()) {
			Process::Patch(InstantFruitPatch1, 0xE3A00001);
			Process::Patch(InstantFruitPatch1 + 4, 0x0A000002);

			Process::Patch(InstantFruitPatch2, 0x13A00003);
			Process::Patch(InstantFruitPatch2 + 4, 0x03A00005);
		}
	}
//Hacker Island Spoof	
	void Hackerisland(MenuEntry *entry)	{ 
		std::vector<std::string> cmnOpt =  { "" };

		bool IsON = *(u32 *)Code::country == 0xE3A000FF;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		int op = optKb.Open();
		if(op == -1)
			return;
		
		Process::Patch(Code::country, IsON ? 0xE1A00C20 : 0xE3A000FF);
		Hackerisland(entry);
	}
//InputChangeEvent For Country Spoof
	void onCountryChange(Keyboard &k, KeyboardEvent &e) {
		if(e.type == KeyboardEvent::CharacterRemoved || e.type == KeyboardEvent::CharacterAdded) {
			std::string s = k.GetInput();
			k.GetMessage() = "ID:\n\n" << IDList::SetCountryName(s != "" ? std::stoi(s, nullptr, 16) : 0);
		}
	}
//Country Spoof	
	void Countryspoof(MenuEntry *entry) {
		static const std::vector<std::string> countOpt = {
			Language->Get("VECTOR_COUNTY_CHANGE"), 
			Language->Get("VECTOR_DISABLE")
		};
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
        static u8 input = 0;
		optKb.Populate(countOpt);
		switch(optKb.Open()) {
			default: break;	
			case 0: 
				if(Wrap::KB<u8>(Language->Get("ISLAND_COUNTRY_SET_ID"), true, 2, input, 0, onCountryChange)) 
					Process::Patch(Code::country, 0xE3A00000 + input); 
			break;	
			case 1: 
				Process::Patch(Code::country, 0xE1A00C20); 
			break;	
		}
	}
//Defined u32 items for Island Shop Slot Mod
	static u32 ShopItem[4] = { 0x2018, 0x2018, 0x2018, 0x2018 };
//Keyboard for Island Shop Slot Mod
	void IslandSettings(MenuEntry *entry) {
		Keyboard SetItem("a");
		SetItem.DisplayTopScreen = true;
		SetItem.OnKeyboardEvent(ItemChange);

		for(int i = 0; i < 4; ++i) {
			SetItem.GetMessage() = Utils::Format(Language->Get("ISLAND_SHOP_MOD_ENTER_ID").c_str(), i + 1);
			s8 res = SetItem.Open(ShopItem[i]);
			if(res < 0)
				break;
		}
	}
//Island Shop
	void IslandShop(MenuEntry *entry) {	
		static const u32 IslandShopPointer = Region::AutoRegion(0x954238, 0x953228, 0x953238, 0x953238, 0x94D238, 0x94C238, 0x94C238, 0x94C238);
		if(*(u32 *)IslandShopPointer == 0)
			return;
		
		if(GameHelper::NextRoomCheck() == 0xA5 && GameHelper::RoomCheck() == 0x65) {
			for(int i = 0; i < 4; ++i) {
				Process::Write32(*(u32 *)IslandShopPointer + 0x10 + (i * 4), IDList::ItemValid(ShopItem[i], false) ? ShopItem[i] : 0x2018);
			}
		}
	}
//All Tours
	void alltour(MenuEntry *entry) {
		static const u32 TourPatch = Region::AutoRegion(0x76FCC0, 0x76ECA4, 0x76ECC8, 0x76ECA0, 0x76E460, 0x76E438, 0x76E008, 0x76DFE0);
		if(entry->WasJustActivated()) {
			Process::Patch(TourPatch, 0xE1A00000); //unsure? (still keeping it for safety)
			Process::Patch(TourPatch + 0x54, 0xE1A00000);  //Adds tour difficulty
			Process::Patch(TourPatch + 0xD0, 0xE1A00000); //Adds tour names and tour infos
			Process::Patch(TourPatch + 0x138, 0xE1A00000); //Adds tour time

			Process::Patch(TourPatch + 0xA8, 0xE2800001); //Adds all tours to be selectable
		}
		else if(!entry->IsActivated()) {
			Process::Patch(TourPatch, 0x0A000004);
			Process::Patch(TourPatch + 0x54, 0x0A000004);
			Process::Patch(TourPatch + 0xD0, 0x0A000004);
			Process::Patch(TourPatch + 0x138, 0x0A000004); 

			Process::Patch(TourPatch + 0xA8, 0x12800001);
		}
    }

//Island Acre Mod	
	void acreMod(MenuEntry *entry) {
		if(*(u32 *)Code::IslPointer == 0)
			return;
		
		u32 IslAcreOffset = *(u32 *)Code::IslPointer + 2;
		
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
			kb.Open(isl.acres[i], isl.acres[i]);
		}
	}
//Island Building Mod	
	void buildingMod(MenuEntry *entry) {
		if(*(u32 *)Code::IslPointer == 0)
			return;
		
		u32 islandBuildings = *(u32 *)Code::IslPointer + 0x1022;
		
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
			kb.GetMessage() = Language->Get("ISLAND_BUILDING_ENTER_ID") << " " << std::to_string(i + 1);
			kb.Open(isl.b[i].id, isl.b[i].id);
			kb.GetMessage() = Language->Get("ISLAND_BUILDING_ENTER_X") << " " << std::to_string(i + 1);
			kb.Open(isl.b[i].x, isl.b[i].x);
			kb.GetMessage() = Language->Get("ISLAND_BUILDING_ENTER_Y") << " " << std::to_string(i + 1);
			kb.Open(isl.b[i].y, isl.b[i].y);
		}
	}

	void FreeKappn(MenuEntry *entry) {
		static Hook hook1, hook2;
		static const u32 kappn1 = Region::AutoRegion(0x5DC048, 0x5DBD94, 0x5DB8AC, 0x5DB8AC, 0x5DB12C, 0x5DB12C, 0x5DADB4, 0x5DADB4);
		static const u32 kappn2 = Region::AutoRegion(0x5DAF98, 0x5DA4C8, 0x5D9FE0, 0x5D9FE0, 0x5D9814, 0x5D9814, 0x5D94E8, 0x5D94E8);

		if(entry->WasJustActivated()) {
			hook1.Initialize(kappn1, (u32)PATCH_KappnBypass1);
			hook2.Initialize(kappn2, (u32)PATCH_KappnBypass2);

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
}