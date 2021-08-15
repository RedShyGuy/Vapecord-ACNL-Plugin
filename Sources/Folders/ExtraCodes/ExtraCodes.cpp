#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
//Shops Always Open
	void ShopsAlwaysOpen(MenuEntry *entry) {
		static const u32 shopretail = Region::AutoRegion(0x309348, 0x309430, 0x309344, 0x309344, 0x3093BC, 0x3093BC, 0x30935C, 0x30935C);
		static const u32 shopnookling = Region::AutoRegion(0x711B14, 0x710FC4, 0x710B1C, 0x710AF4, 0x7102C8, 0x7102A0, 0x70FE70, 0x70FE70);
		static const u32 shopgarden = Region::AutoRegion(0x711BCC, 0x71107C, 0x710BD4, 0x710BAC, 0x710380, 0x710358, 0x70FF28, 0x70FF28);
		static const u32 shopables = Region::AutoRegion(0x713EB0, 0x713360, 0x712EB8, 0x712E90, 0x712664, 0x71263C, 0x71220C, 0x71220C);
		static const u32 shopshampoodle = Region::AutoRegion(0x71D42C, 0x71C774, 0x71C434, 0x71C40C, 0x71BBE0, 0x71BBB8, 0x71B788, 0x71B788);
		static const u32 shopkicks = Region::AutoRegion(0x71184C, 0x710CFC, 0x710854, 0x71082C, 0x710000, 0x70FFD8, 0x70FBA8, 0x70FBA8);   
		static const u32 shopnooks = Region::AutoRegion(0x71F654, 0x71E99C, 0x71E65C, 0x71E634, 0x71DE08, 0x71DDE0, 0x71D9B0, 0x71D9B0);
		static const u32 shopkatrina = Region::AutoRegion(0x718098, 0x717548, 0x7170A0, 0x717078, 0x71684C, 0x716824, 0x7163F4 ,0x7163F4);
		static const u32 shopredd = Region::AutoRegion(0x718444, 0x7178F4, 0x71744C, 0x717424, 0x716BF8, 0x716BD0, 0x7167A0, 0x7167A0);
		
		const u32 ShopOpen[9] = { shopretail, shopnookling, shopgarden, shopables, shopshampoodle, shopkicks, shopnooks, shopkatrina, shopredd };

        std::vector<std::string> cmnOpt =  { "" };

		bool IsON = *(u32 *)ShopOpen[0] == 0xE3A00001;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;
			
		for(int i = 0; i < 9; ++i)
			Process::Patch(ShopOpen[i], *(u32 *)ShopOpen[i] == 0xE3A00001 ? 0xE3A00000 : 0xE3A00001);
			
		ShopsAlwaysOpen(entry);
    }

//Disable Save Menus
	void nonesave(MenuEntry *entry) {
		std::vector<std::string> cmnOpt =  { "" };

		bool IsON = *(u32 *)Code::nosave == 0xE1A00000;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		if(op < 0)
			return;

		Process::Patch(Code::nosave, IsON ? 0xE8900006 : 0xE1A00000);
		save = !save;
		nonesave(entry);
	}

//Disable Item Locks /*Credits to Nico*/
	void bypass(MenuEntry *entry) {
		std::vector<std::string> cmnOpt =  { "" };	

		cmnOpt[0] = bypassing ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		
		if(op < 0)
			return;
		
		GameHelper::DropItemLock(!bypassing);
		bypassing = !bypassing;
		bypass(entry);
	}
//Can't Fall In Holes Or Pitfalls /*Credits to Nico*/
	void noTrap(MenuEntry *entry) {
		static const u32 notraps1 = Region::AutoRegion(0x65A668, 0x659B90, 0x6596A0, 0x6596A0, 0x659160, 0x659160, 0x658D08, 0x658D08);
		static const u32 notraps2 = Region::AutoRegion(0x6789E4, 0x677F0C, 0x677A1C, 0x677A1C, 0x6774DC, 0x6774DC, 0x677084, 0x677084);
		
		std::vector<std::string> cmnOpt =  { "" };

		bool IsON = *(u32 *)notraps1 == 0xEA000014;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();
		
		if(op < 0)
			return;
			
		Process::Patch(notraps1, *(u32 *)notraps1 == 0xEA000014 ? 0x1A000014 : 0xEA000014);
		Process::Patch(notraps2, *(u32 *)notraps2 == 0xEA00002D ? 0x1A00002D : 0xEA00002D);
		noTrap(entry);
	}

	bool wasFF = false;
//Show Mail Text	
	void mailtext(MenuEntry *entry) {
		if(!wasFF) {
			if(Player::GetMailSlot() != 0xFF) {
                MessageBox(GameHelper::GetMailText(Player::GetMailSlot())).SetClear(ClearScreen::Top)();
                wasFF = true;
			}
		}
		
		if(wasFF) 
			if(Player::GetMailSlot() == 0xFF) 
				wasFF = false;
	}
//Water All Flowers	
	void WaterAllFlowers(MenuEntry *entry) {	
		if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			u32 x = 0x10, y = 0x10;
			bool res = true;
			
			while(res) {
				while(res) {
					if((u32)GameHelper::GetItemAtWorldCoords(x, y) != 0) 
						GameHelper::WaterFlower(x, y);
					else 
						res = false;

					y++;
				}
				
				res = true;
				y = 0x10;
				x++;
				if((u32)GameHelper::GetItemAtWorldCoords(x, y) == 0) 
					res = false;
			}
			OSD::Notify("Success!");
		}
    }
//Weed Remover
	void weedremover(MenuEntry *entry) {	
		const std::vector<std::string> weedopt = {
			Language->Get("WEED_REMOVER_OFF"), 
			Language->Get("WEED_REMOVER_ON")
		};
		
		static int size = 400;
		if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			Sleep(Milliseconds(100));
			Keyboard KB(Language->Get("WEED_REMOVER_KEY"));
			KB.Populate(weedopt);
			switch(KB.Open()) {
				case 0: size = 5000; break;
				case 1: 
				default: size = 300; break;
			}
		}
		
		if(Controller::IsKeysPressed(entry->Hotkeys[1].GetKeys())) {
			int res = Dropper::Search_Replace(size, { 0x7C, 0x7D, 0x7E, 0x7F, 0xCC, 0xF8 }, Code::Pointer7FFE, 0x3D, false, "Weed Removed!", true);
			if(res == -1)
				OSD::Notify("Your player needs to be loaded!", Color::Red);
			else if(res == -2) 
				OSD::Notify("Only works outdoors!", Color::Red);
		}
	}
//Edit Every Pattern
	void editpattern(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0)
			return;
		
		std::string PlayerName = "", TownName = "";
		Process::ReadString(PlayerPTR::Pointer(0x55A8), PlayerName, 0x10, StringFormat::Utf16);
		Process::ReadString(PlayerPTR::Pointer(0x55BE), TownName, 0x10, StringFormat::Utf16);
		
		for(int i = 0; i < 10; ++i) 
			Player::SetDesign(i, "", *(u16 *)PlayerPTR::Pointer(0x55A6), PlayerName, *(u8 *)PlayerPTR::Pointer(0x55BA), *(u16 *)PlayerPTR::Pointer(0x55BC), TownName, *(u32 *)PlayerPTR::Pointer(0x55D0), 0xFF, 0xFF);

		entry->Disable();
	}
	
	static const int _GrassTile[64] = {
		0,	1,	4,	5,	16,	17,	20,	21,
		2,	3,	6,	7,	18,	19,	22,	23,
		8,	9,	12,	13,	24,	25,	28,	29,
		10,	11,	14,	15,	26,	27,	30,	31,
		32,	33,	36,	37,	48,	49,	52,	53,
		34,	35,	38,	39,	50,	51,	54,	55,
		40,	41,	44,	45,	56,	57,	60,	61,
		42,	43,	46,	47,	58,	59,	62,	63
	};
/*Calculations copied from the ACNL Web Save Editor, credits goes to the creator*/
	u32 GetTileOffset(int x, int y) {
		const int Add = 64 * ((y / 8) * 8 * 2 + (x / 8)) + _GrassTile[(y % 8) * 8 + (x % 8)];
		const u32 GrassStart = *(u32 *)(GameHelper::GetCurrentMap() + 0x28);
		return (GrassStart + Add);
	}
	
	void grasscomplete(MenuEntry *entry) {		
		const std::vector<std::string> GrassKB {
			Language->Get("GRASS_EDITOR_FILL"),
			Language->Get("GRASS_EDITOR_CLEAR")
		};
		
		if(!GameHelper::IsInRoom(0)) {
			MessageBox(Color::Red << Language->Get("ONLY_TOWN_ERROR")).SetClear(ClearScreen::Top)();
			return;
		}
		
		const u32 GrassStart = *(u32 *)(GameHelper::GetCurrentMap() + 0x28);
		Keyboard KB(Language->Get("GRASS_EDITOR_KB1") << "\n" << Color(0x228B22FF) << 
					Language->Get("GRASS_EDITOR_KB2")  << "\n" << Color(0xCD853FFF) << 
					Language->Get("GRASS_EDITOR_KB3"));
					
		KB.Populate(GrassKB);
		switch(KB.Open()) {
			case 0:
				std::memset((void *)GrassStart, 0xFFFFFFFF, 0x2800);
				GameHelper::ReloadRoom();
			break;
			case 1:
				std::memset((void *)GrassStart, 0, 0x2800);
				GameHelper::ReloadRoom();
			break;
			default: break;
		}
	}
	
//remove/add grass!!!! :))))))
	void grasseditor(MenuEntry *entry) {
		static bool opt = false;
		static u8 type = 0;

		if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			if(!GameHelper::IsInRoom(0)) {
				OSD::Notify("Error: Only Works In Town!", Color::Red);
				return;
			}
			
			u32 x, y;
			if(PlayerClass::GetInstance()->GetWorldCoords(&x, &y)) {
				Process::Write8(GetTileOffset(x, y), type);
				OSD::Notify(Utils::Format("Changed Grass at: X%02X|Y%02X", (u8)x, (u8)y));
			}
		}
		
		if(Controller::IsKeysPressed(entry->Hotkeys[1].GetKeys())) 
			GameHelper::ReloadRoom();
		
		if(Controller::IsKeysPressed(entry->Hotkeys[2].GetKeys())) {
			switch(opt) {
				case 0:
					type = 0xFF;
					OSD::Notify("Grass: Fill-Mode Active!", Color(0x228B22FF));
					opt = true;
				break;
				case 1:
					type = 0;
					OSD::Notify("Grass: Clear-Mode Active!", Color(0xCD853FFF));
					opt = false;
				break;
			}
		}
	}
	
	const int TimeMax[5] = { 60, 24, 30, 12, 50 };
	static int CurrTime = 0;
	
	bool CheckTimeInput(const void *input, std::string &error) {
		const std::string TimeMode[5] = { 
			Language->Get("TIME_MINUTE"), 
			Language->Get("TIME_HOUR"), 
			Language->Get("TIME_DAY"), 
			Language->Get("TIME_MONTH"), 
			Language->Get("TIME_YEAR") 
		};

        int in = *static_cast<const int *>(input);
        if(in >= TimeMax[CurrTime]) {
			error = Utils::Format(Language->Get("TIME_ERROR").c_str(), (TimeMax[CurrTime] - 1), TimeMode[CurrTime].c_str());
            return 0;
        }

        return 1;
    }
	
	void TTKeyboard(MenuEntry *entry) {
		const std::string TimeMode[5] = { 
			Language->Get("TIME_MINUTE"), 
			Language->Get("TIME_HOUR"), 
			Language->Get("TIME_DAY"), 
			Language->Get("TIME_MONTH"), 
			Language->Get("TIME_YEAR") 
		};

		std::vector<std::string> TTKB {
			Language->Get("TIME_BACKWARDS"),
			Language->Get("TIME_FORWARD")
		};
		
		u8 timedat[5] = { 0, 0, 0, 0, 0 };
		Keyboard KB("", TTKB);
		s8 ch = KB.Open();
		if(ch < 0)
			return;
		
		for(int i = 0; i < 5; ++i) {			
			Keyboard KBS(Utils::Format(Language->Get("TIME_KB1").c_str(), TimeMode[i].c_str()));
			KBS.IsHexadecimal(false);
			CurrTime = i;
			KBS.SetCompareCallback(CheckTimeInput);
			s8 cho = KBS.Open(timedat[i]);
			if(cho < 0)
				return;
		}
		GameHelper::SetCurrentTime(ch, timedat[0], timedat[1], timedat[2], timedat[3], timedat[4]);
	}
	
	void TimeTravel(MenuEntry *entry) {
		static u32 PressedTicks = 0;
		int minute = 1;
		
		if(Controller::IsKeysDown(entry->Hotkeys[0].GetKeys()) || Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			PressedTicks++;
			if((PressedTicks < 50 ? (PressedTicks % 8) == 1 : (PressedTicks % 3) == 1) || PressedTicks > 100) 
				GameHelper::SetCurrentTime(true, minute, 0, 0, 0, 0);
		}
		
		if(Controller::IsKeysDown(entry->Hotkeys[1].GetKeys()) || Controller::IsKeysPressed(entry->Hotkeys[1].GetKeys())) {
			PressedTicks++;
			if((PressedTicks < 50 ? (PressedTicks % 8) == 1 : (PressedTicks % 3) == 1) || PressedTicks > 100) 
				GameHelper::SetCurrentTime(false, minute, 0, 0, 0, 0);
		}
	//somehow doesnt work always?
		if(Controller::IsKeysReleased(entry->Hotkeys[0].GetKeys()) || Controller::IsKeysReleased(entry->Hotkeys[1].GetKeys())) 
			PressedTicks = 0;
	}
}