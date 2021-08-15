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
		if(!GameHelper::IsInRoom(0x77)) {
			MessageBox(Language->Get("ORE_FRUIT_ERROR")).SetClear(ClearScreen::Top)();
			return;
		}
		
		if(*(u32 *)Code::Isl2ndPointer == 0)
			return;
		
		u32 Fruit = *(u32 *)Code::Isl2ndPointer + 0xC0;
		u32 val = GameHelper::GetOnlinePlayerIndex();
		if(val <= 3 && val >= 0) {
			for(int i = 0; i < 5; ++i) {
				for(int j = 0; j < 5; ++j) {
					Inventory::WriteSlot((i * 3 + j), *(u16 *)(Fruit + 2 * (val * 3 + i)));
				}
			}
		}
		MessageBox(Language->Get("FRUIT_SPAWNED")).SetClear(ClearScreen::Top)();
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
		
		Process::Write32(Code::country, IsON ? 0xE1A00C20 : 0xE3A000FF);
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
					Process::Write32(Code::country, 0xE3A00000 + input); 
			break;	
			case 1: 
				Process::Write32(Code::country, 0xE1A00C20); 
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
		if(!GameHelper::IsInRoom(0x67)) 
			return;
		
		static const u32 toury = Region::AutoRegion(0x95D734, 0x95C714, 0x95C72C, 0x95C72C, 0x95672C, 0x95572C, 0x95572C, 0x95572C);
		if(*(u32 *)toury == 0) 
			return;
		
		for(int i = 0; i < 64; ++i)					
			Process::Write8(*(u32 *)toury + 0x10 + (1 * i), 1);
		
		entry->Disable();
    }
//Island Acre Mod	
	void acreMod(MenuEntry *entry) {
		if(*(u32 *)Code::IslPointer == 0)
			return;
		
		u32 IslAcreOffset = *(u32 *)Code::IslPointer + 0x2;
		
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