#include "cheats.hpp"

namespace CTRPluginFramework {
//Name Changer | Player specific save code
	void NameChanger(MenuEntry* entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		Keyboard keyboard(Language->Get("NAME_CHANGER_ENTER_NAME"));
		std::string input;
		keyboard.SetMaxLength(8);
		if(keyboard.Open(input) < 0) 
			return;

		PlayerName::Set(input);
	}

//Player Appearance Changer	
	void playermod(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Both)();
			return;
		}	

		static const u8 ValidID[5][2] = {
			{ 0, 0x21 },
			{ 0, 0xF },
			{ 0, 0xB },
			{ 0, 5 },
		};

		static const u16 ValidID2[6][2] = {
			{ 0x280B, 0x28F3 },
			{ 0x28F5, 0x295B },
			{ 0x2493, 0x26F5 },
			{ 0x26F8, 0x2776 },
			{ 0x2777, 0x279E },
			{ 0x279F, 0x27E5 }
		};
		
		static const std::vector<std::string> playeropt = {
			Language->Get("VECTOR_PLAYER_MOD_HAIR_STYLE"),
			Language->Get("VECTOR_PLAYER_MOD_HAIR_COLOR"),
			Language->Get("VECTOR_PLAYER_MOD_EYE_STYLE"),
			Language->Get("VECTOR_PLAYER_MOD_EYE_COLOR"),
			
			Language->Get("VECTOR_PLAYER_MOD_GENDER"),
			Language->Get("VECTOR_PLAYER_MOD_TAN"),
			Language->Get("VECTOR_PLAYER_MOD_OUTFIT")
		};
		
		static const std::vector<std::string> genderopt = {
			Language->Get("VECTOR_PLAYER_MOD_GENDER_MALE"), 
			Language->Get("VECTOR_PLAYER_MOD_GENDER_FEMALE"),
		};
		
		static const std::vector<std::string> tanopt = {
			Language->Get("VECTOR_PLAYER_MOD_TAN_DARK"),
			Language->Get("VECTOR_PLAYER_MOD_TAN_TAN"),
			Language->Get("VECTOR_PLAYER_MOD_TAN_FAIR"),
			Language->Get("VECTOR_PLAYER_MOD_TAN_CUSTOM"),
		};
		
		static const std::vector<std::string> outfitplayeropt = {
			Language->Get("VECTOR_OUTFIT_HEADGEAR"), 
			Language->Get("VECTOR_OUTFIT_GLASSES"), 
			Language->Get("VECTOR_OUTFIT_SHIRT"), 
			Language->Get("VECTOR_OUTFIT_PANTS"), 
			Language->Get("VECTOR_OUTFIT_SOCKS"), 
			Language->Get("VECTOR_OUTFIT_SHOES")
		};

		u8 ID = 0;
		u16 ID2 = 0;

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(playeropt);
		Sleep(Milliseconds(100));

		s8 choice = optKb.Open();
		if(choice < 0)
			return;
			
	//Standard Face Appearance Change
		if(choice < 4) {
			KeyRange::Set({ ValidID[choice][0], ValidID[choice][1] });
			if(Wrap::KB<u8>(Language->Get("ENTER_ID") << Utils::Format("%02X -> %02X", ValidID[choice][0], ValidID[choice][1]), true, 2, ID, ID, ValidKeyboardCheck)) {
				PlayerPTR::Write8(4 + choice, ID);
				goto update;
			}
		}
	//Gender Change
		if(choice == 4) {
			optKb.Populate(genderopt);
			s8 res = optKb.Open();
			if(res < 0)
				return;

			PlayerName::UpdateReference(4, "", res);
		}
				
		if(choice == 5) {
			optKb.Populate(tanopt);
			switch(optKb.Open()) {
				default: break;	
				case 0: Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 8, 0xF); goto tanupdate;
				case 1: Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 8, 0xA); goto tanupdate;
				case 2: Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 8, 0); goto tanupdate;
				case 3: {
					u8 val = 0;
					if(Wrap::KB<u8>(Language->Get("PLAYER_APPEARANCE_TAN_LEVEL") << "0x00 -> 0x0F", false, 2, val, 0)) 
						Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 8, val);
				} goto tanupdate;
			}
		}
			
		if(choice == 6) {
			optKb.Populate(outfitplayeropt);

			s8 res = optKb.Open();
			if(res < 0)
				return;

			KeyRange::Set({ ValidID[choice][0], ValidID[choice][1] });
			if(Wrap::KB<u16>(Language->Get("ENTER_ID") << Utils::Format("%04X -> %04X", ValidID2[choice][0], ValidID2[choice][1]), true, 4, ID2, ID2, ValidKeyboardCheck)) {
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0xA + (choice * 4), ID2);

				u32 add = Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0xA;
				Player::WriteOutfit(GameHelper::GetOnlinePlayerIndex(), *(u16 *)(add), *(u16 *)(add + 4), *(u16 *)(add + 8), *(u16 *)(add + 0xC), *(u16 *)(add + 0x10), *(u16 *)(add + 0x14));
			}
		}

		update:
			Player::UpdateStyle();
		return;
		
		tanupdate:
			Player::UpdateTan();
	}

//Random Outfit
	void randomoutfit(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Both)();
			return;
		}
	
		static const std::vector<std::string> randomopt = {
			Language->Get("VECTOR_RANDOM_OUTFIT"),
			Language->Get("VECTOR_RANDOM_PLAYER")
		};

		Keyboard randkb(Language->Get("KEY_RANDOMIZE_PLAYER"));
		randkb.Populate(randomopt);
		switch(randkb.Open()) {
			default: break;			
			case 0: {
				Player::WriteOutfit(GameHelper::GetOnlinePlayerIndex(), Utils::Random(0x280B, 0x28F3), Utils::Random(0x28F5, 0x295B), Utils::Random(0x2493, 0x26F5), Utils::Random(0x26F8, 0x2776), Utils::Random(0x2777, 0x279E), Utils::Random(0x279F, 0x27E5));
			} break;
			case 1: {
				Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 4, Utils::Random(0, 0x21)); //HairStyle
				Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 5, Utils::Random(0, 0xF)); //HairColor
				Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 6, Utils::Random(0, 4)); //EyeStyle
				Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 7, Utils::Random(0, 4)); //EyeColor
				Process::Write8(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 8, Utils::Random(0, 0xF)); //Tan
				
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0xA, Utils::Random(0x280B, 0x28F3)); //Headwear
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0xE, Utils::Random(0x28F5, 0x295B)); //Glasses
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0x12, Utils::Random(0x2493, 0x26F5)); //Shirt
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0x1A, Utils::Random(0x26F8, 0x2776)); //Pants
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0x1E, Utils::Random(0x2777, 0x279E)); //Socks
				Process::Write16(Player::GetSaveOffset(GameHelper::GetOnlinePlayerIndex()) + 0x22, Utils::Random(0x279F, 0x27E5)); //Shoes

			//Reloads player style
				Player::UpdateStyle();
			} break;	
		}
	}

//Player Backup/Restore
	void playerbackup(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Both)();
			return;
		}
		
		static const std::vector<std::string> backopt = {
			Language->Get("VECTOR_RANDOM_BACKUP"),
			Language->Get("VECTOR_RANDOM_RESTORE"),
			Language->Get("FILE_DELETE"),  
		};

		Keyboard backkb(Language->Get("KEY_CHOOSE_OPTION"));
		backkb.Populate(backopt);
		switch(backkb.Open()) {
			default: break;		
			case 0: {
				std::string filename = "";
				Keyboard KB(Language->Get("RANDOM_PLAYER_DUMP"));
				if(KB.Open(filename) == -1)
					return;

				Wrap::Dump(PATH_PLAYER, filename, ".player", WrapLoc{ Player::GetSaveOffset(4), 0xA480 }, WrapLoc{ (u32)-1, (u32)-1 });
			} break;
			case 1: {
				Wrap::Restore(PATH_PLAYER, ".player", Language->Get("RANDOM_PLAYER_RESTORE"), nullptr, true, WrapLoc{ Player::GetSaveOffset(4), 0xA480 }, WrapLoc{ (u32)-1, (u32)-1 }); 
				Player::UpdateTan();
				Player::UpdateStyle();
			} break;	
			case 2: 
				Wrap::Delete(PATH_PLAYER, ".player");
			break;
		}
	}

//TPC Message Changer | Player specific save code
	void tpcmessage(MenuEntry* entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> alignments = {
			Language->Get("TPC_MESSAGE_ALLIGN_NORMAL"), 
			Language->Get("TPC_MESSAGE_ALLIGN_EMPTY")
		};

		Keyboard KB(Language->Get("TPC_MESSAGE_ALLIGN"));
		KB.Populate(alignments);

		std::string input;

		switch(KB.Open()) {
			default: return;
			case 0:
				KB.GetMessage() = Language->Get("TPC_MESSAGE_ENTER_NAME");
				KB.SetMaxLength(26);
				if(KB.Open(input) >= 0) 
					Process::WriteString(Player::GetSaveOffset(4) + 0x6B38, input, 0x20, StringFormat::Utf16);
			break;
			case 1:
				Process::Write16(Player::GetSaveOffset(4) + 0x6B38, 0x000E);
			break;
		}
	}

//TPC Image Dumper | non player specific save code
	void tpc(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		static const std::vector<std::string> g_player = {
			Language->Get("VECTOR_PLAYER_1"), 
			Language->Get("VECTOR_PLAYER_2"), 
			Language->Get("VECTOR_PLAYER_3"), 
			Language->Get("VECTOR_PLAYER_4"), 
		};
		
		static const std::vector<std::string> tpcselectopt = {
			Language->Get("VECTOR_TPCDUMP_DUMP"),
			Language->Get("VECTOR_TPCDUMP_RESTORE"),
			Language->Get("FILE_DELETE"),  
		};
			
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"));
		KB.Populate(tpcselectopt);
		
		switch(KB.Open()) {
			default: break;
			
			case 0: {
				Keyboard PKB(Language->Get("KEY_SELECT_PLAYER"));
				PKB.Populate(g_player);
				
				s8 index = PKB.Open();
				if(index < 0)
					return;
					
				if(Player::GetSaveOffset(index) != 0) {
					std::string filename = "";
					Keyboard KB(Language->Get("TPC_DUMPER_NAME"));
					if(KB.Open(filename) < 0)
						return;

					Wrap::Dump(PATH_TPC, filename, ".jpg", WrapLoc{ Player::GetSaveOffset(index) + 0x5738, 0x1400 }, WrapLoc{ (u32)-1, (u32)-1 });
				}
			} break;
			
			case 1: 
				Wrap::Restore(PATH_TPC, ".jpg", Language->Get("TPC_DUMPER_RESTORE"), nullptr, true, WrapLoc{ PlayerPTR::Pointer(0x5738), 0x1400 }, WrapLoc{ (u32)-1, (u32)-1 });
			break;
			
			case 2: 
				Wrap::Delete(PATH_TPC, ".jpg");
			break;
		}
	}

	u32 GetRealSlot(u8 slot, int pIndex) {
		u32 sData = Player::GetSaveOffset(pIndex);
		u32 DesignSlot = sData + 0x548C;
		
		u8 correctslot = *(u8 *)(DesignSlot + slot);
		return correctslot;
	}
//get correct save for design
	u32 GetDesignSave(u8 slot, int pIndex) {
		return Player::GetDesign(GetRealSlot(slot, pIndex), pIndex);
	}
//dump designs | player specific save code
	void DesignDumper(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> designslots;
		
		for(int i = 1; i <= 10; ++i)
			designslots.push_back(Utils::Format(Language->Get("VECTOR_DESIGN").c_str(), i));
		
		static const std::vector<std::string> designselect = {
			Language->Get("VECTOR_DESIGNDUMP_DUMP"), 
			Language->Get("VECTOR_DESIGNDUMP_RESTORE"), 
			Language->Get("FILE_DELETE"),  
		};
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"));
		KB.Populate(designselect);
		switch(KB.Open()) {
			default: break;
			
			case 0: { 
				Keyboard DKB(Language->Get("KEYBOARD_DESIGNDUMP"));
				DKB.Populate(designslots);
				int dSlot = DKB.Open();
				
				if(dSlot != -1) {
					std::string filename = "";
					Keyboard KB(Language->Get("DESIGN_DUMP_NAME"));
					if(KB.Open(filename) == -1)
						return;

					Wrap::Dump(PATH_DESIGN, filename, ".acnl", WrapLoc{ GetDesignSave(dSlot, 4), 0x26B }, WrapLoc{ (u32)-1, (u32)-1 });
				}
			} break;
			
			case 1: {
				Keyboard DKB(Language->Get("KEYBOARD_DESIGNDUMP"));
				DKB.Populate(designslots);
				int dSlot = DKB.Open();
				
				if(dSlot != -1) {
					Wrap::Restore(PATH_DESIGN, ".acnl", Language->Get("DESIGN_DUMP_RESTORE"), nullptr, true, WrapLoc{ GetDesignSave(dSlot, 4), 0x26B }, WrapLoc{ (u32)-1, (u32)-1 });  
					Player::ReloadDesign(GetRealSlot(dSlot, 4));
				}
			} break;
			
			case 2: 
				Wrap::Delete(PATH_DESIGN, ".acnl");
			break;
		}
	}
//Fill Emote List | player specific save code
	void emotelist(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> emoteopt = {
			Language->Get("VECTOR_EMOTIONLIST_FILL_LIST"),
			Language->Get("VECTOR_EMOTIONLIST_FILL_EMOTION"),
			Language->Get("VECTOR_EMOTIONLIST_CLEAR_LIST"),
		};
		
		static const u8 emotionIDs[40] = { 
			0x09, 0x0D, 0x06, 0x02, 0x12, 0x0E, 0x0B, 0x05, 0x01, 0x11, 0x0C, 
			0x08, 0x04, 0x13, 0x10, 0x0A, 0x07, 0x03, 0x14, 0x15, 0x16, 0x17, 
			0x1A, 0x24, 0x26, 0x1B, 0x19, 0x21, 0x2A, 0x29, 0x1D, 0x1C, 0x20, 
			0x27, 0x28, 0x18, 0x1E, 0x2B, 0x2C, 0x2E 
		};
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"));
		KB.Populate(emoteopt);
		
		switch(KB.Open()) {
			default: break;
			case 0: 
				Process::CopyMemory((void *)PlayerPTR::Pointer(0x89D0), emotionIDs, 0x28); 
			break; 
			case 1: {
				u8 emotion = 0; 
				Keyboard KB(Language->Get("EMOTION_LIST_TYPE_ID"));
				KB.IsHexadecimal(true);
				KB.CanAbort(true);
				
				if(KB.Open(emotion) < 0)
					return;
				
				std::memset((void *)PlayerPTR::Pointer(0x89D0), emotion, 0x28);
			} break;
			
			case 2: 
				std::memset((void *)PlayerPTR::Pointer(0x89D0), 0, 0x28);
			break;
		}
	}
//Fill Enzyklopedia List | player specific save code
	void enzyklopedia(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> enzyopt = {
			Language->Get("VECTOR_ENZY_FILL"),
			Language->Get("VECTOR_ENZY_CLEAR"),
		};		
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"));
		KB.Populate(enzyopt);
		switch(KB.Open()) {
			default: break;
			case 0: 
				std::memset((void *)PlayerPTR::Pointer(0x6C70), 0xFFFFFFFF, 0x38);
			break;
			case 1: 
				std::memset((void *)PlayerPTR::Pointer(0x6C70), 0, 0x38);
			break;
		} 
	}
//Change Dream Code | player specific save code
	void comodifier(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}

		Keyboard kb(Language->Get("DREAM_CODE_ENTER_ID"));
		kb.IsHexadecimal(true);
		kb.CanAbort(true);
		kb.DisplayTopScreen = true;
		
		u16 part1, part2, part3;
		if(kb.Open(part1, 0) >= 0) {
			if(kb.Open(part2, 0) >= 0) {
				if(kb.Open(part3, 0) >= 0) {
					Process::Write8(PlayerPTR::Pointer(0x56F9), (part1 >> 8) & 0xFF);
					Process::Write8(PlayerPTR::Pointer(0x56F4), part1 & 0xFF);
					
					Process::Write8(PlayerPTR::Pointer(0x56F3), (part2 >> 8) & 0xFF);
					Process::Write8(PlayerPTR::Pointer(0x56F2), part2 & 0xFF);
					
					Process::Write8(PlayerPTR::Pointer(0x56F1), (part3 >> 8) & 0xFF);
					Process::Write8(PlayerPTR::Pointer(0x56F0), part3 & 0xFF);

					if(*(u8 *)PlayerPTR::Pointer(0x56F8) == 0)
						Process::Write8(PlayerPTR::Pointer(0x56F8), 1); //activates dream code
				}
			}
		}	
	}
//Enable Unused Menu | player specific save code
	void debug1(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> cmnOpt =  { "" };
		
		bool IsON = *(u8 *)PlayerPTR::Pointer(0x572F) == 0x40;
 
		cmnOpt[0] = IsON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard KB(Language->Get("KEY_CHOOSE_OPTION"));
		KB.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		s8 op = KB.Open();
		if(op < 0)
			return;

		PlayerPTR::Write8(0x572F, IsON ? 0 : 0x40);
		debug1(entry);
	}

//Fill Song List | player specific save code
	void FillSongs(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> songopt = {
			Language->Get("VECTOR_ENZY_FILL"),
			Language->Get("VECTOR_ENZY_CLEAR"),
		};		
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(songopt);
		switch(optKb.Open()) {
			default: break;
			case 0: 
				std::memset((void *)PlayerPTR::Pointer(0x8F9C), 0xFFFFFFFF, 0x18);
			break;
			case 1:
				std::memset((void *)PlayerPTR::Pointer(0x8F9C), 0, 0x18);
			break;
		}
	}

//Fill Catalog | player specific save code	
	void FillCatalog(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Top)();
			return;
		}
		
		static const std::vector<std::string> songopt = {
			Language->Get("VECTOR_ENZY_FILL"),
			Language->Get("VECTOR_ENZY_CLEAR"),
		};	
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(songopt);
		switch(optKb.Open()) {
			default: break;
			case 0: 
				std::memset((void *)PlayerPTR::Pointer(0x6C90), 0xFF, 0x1A8);
			break;
			case 1: 
				std::memset((void *)PlayerPTR::Pointer(0x6C90), 0, 0x1A8);
			break;
		}
    }
}