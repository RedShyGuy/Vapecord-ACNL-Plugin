#include "cheats.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Game.hpp"
#include "Address/Address.hpp"
#include "Helpers/GameStructs.hpp"
#include "Helpers/Converters.hpp"
#include "Helpers/Town.hpp"

#include "Color.h"
#include "Files.h"

namespace CTRPluginFramework {
//Name Changer | Player specific save code
	void NameChanger(MenuEntry* entry) {
		if(!Player::GetSaveData()) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		Keyboard keyboard(Language::getInstance()->get(TextID::NAME_CHANGER_ENTER_NAME));
		std::string input = "";
		keyboard.SetMaxLength(8);

		if(keyboard.Open(input) < 0) {
			return;
		}

		Player::EditName(4, input);
	}

//Player Appearance Changer	
	void playermod(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Both)();
			return;
		}

		static const u8 ValidID[5][2] = {
			{ 0x00, 0x21 }, { 0x00, 0x0F }, 
			{ 0x00, 0x0B }, { 0x00, 0x05 }
		};

		static const u16 ValidID2[6][2] = {
			{ 0x280B, 0x28F3 },
			{ 0x28F5, 0x295B },
			{ 0x2493, 0x26F5 },
			{ 0x26F8, 0x2776 },
			{ 0x2777, 0x279E },
			{ 0x279F, 0x27E5 }
		};
		
		const std::vector<std::string> playeropt = {
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_HAIR_STYLE),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_HAIR_COLOR),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_EYE_STYLE),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_EYE_COLOR),
			
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_GENDER),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_TAN),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_OUTFIT)
		};
		
		const std::vector<std::string> genderopt = {
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_GENDER_MALE), 
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_GENDER_FEMALE),
		};
		
		const std::vector<std::string> tanopt = {
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_TAN_DARK),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_TAN_TAN),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_TAN_FAIR),
			Language::getInstance()->get(TextID::VECTOR_PLAYER_MOD_TAN_CUSTOM),
		};
		
		const std::vector<std::string> outfitplayeropt = {
			Language::getInstance()->get(TextID::VECTOR_OUTFIT_HEADGEAR), 
			Language::getInstance()->get(TextID::VECTOR_OUTFIT_GLASSES), 
			Language::getInstance()->get(TextID::VECTOR_OUTFIT_SHIRT), 
			Language::getInstance()->get(TextID::VECTOR_OUTFIT_PANTS), 
			Language::getInstance()->get(TextID::VECTOR_OUTFIT_SOCKS), 
			Language::getInstance()->get(TextID::VECTOR_OUTFIT_SHOES)
		};

		u8 ID = 0;
		u16 item = 0;

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), playeropt);
		
		int choice = optKb.Open();
		if(choice < 0) {
			return;
		}
			
	//Standard Face Appearance Change
		if(choice < 4) {
			KeyRange::Set({ ValidID[choice][0], ValidID[choice][1] });
			if(Wrap::KB<u8>(Language::getInstance()->get(TextID::ENTER_ID) << Utils::Format("%02X -> %02X", ValidID[choice][0], ValidID[choice][1]), true, 2, ID, ID, ValidKeyboardCheck)) {
				switch(choice) {
					case 0: player->PlayerAppearance.PlayerFeatures.HairStyle = ID; goto update;
					case 1: player->PlayerAppearance.PlayerFeatures.HairColor = ID; goto update;
					case 2: player->PlayerAppearance.PlayerFeatures.Face = ID; goto update;
					case 3: player->PlayerAppearance.PlayerFeatures.EyeColor = ID; goto update;
				}
			}
		}
	//Gender Change
		if(choice == 4) {
			optKb.Populate(genderopt);

			int gender = optKb.Open();
			if(gender < 0) {
				return;
			}

			Player::EditGender(4, gender);
		}
				
		if(choice == 5) {
			optKb.Populate(tanopt);

			switch(optKb.Open()) {
				default: break;	
				case 0: player->PlayerAppearance.PlayerFeatures.Tan = 0xF; goto tanupdate;
				case 1: player->PlayerAppearance.PlayerFeatures.Tan = 0xA; goto tanupdate;
				case 2: player->PlayerAppearance.PlayerFeatures.Tan = 0; goto tanupdate;
				case 3: {
					u8 val = 0;
					if(Wrap::KB<u8>(Language::getInstance()->get(TextID::PLAYER_APPEARANCE_TAN_LEVEL) << "0x00 -> 0x0F", false, 2, val, 0)) {
						player->PlayerAppearance.PlayerFeatures.Tan = val;
					}
				} goto tanupdate;
			}
		}
			
		if(choice == 6) {
			optKb.Populate(outfitplayeropt);

			int res = optKb.Open();
			if(res < 0) {
				return;
			}

			KeyRange::Set({ ValidID2[res][0], ValidID2[res][1] });
			if(Wrap::KB<u16>(Language::getInstance()->get(TextID::ENTER_ID) << Utils::Format("%04X -> %04X", ValidID2[res][0], ValidID2[res][1]), true, 4, item, item, ValidKeyboardCheck)) {
				switch(res) {
					case 0: player->PlayerAppearance.PlayerOutfit.Hat.ID = item; break;
					case 1: player->PlayerAppearance.PlayerOutfit.Accessory.ID = item; break;
					case 2: player->PlayerAppearance.PlayerOutfit.TopWear.ID = item; break;
					case 3: player->PlayerAppearance.PlayerOutfit.BottomWear.ID = item; break;
					case 4: player->PlayerAppearance.PlayerOutfit.Socks.ID = item; break;
					case 5: player->PlayerAppearance.PlayerOutfit.Shoes.ID = item; break;
				}

				Player::WriteOutfit(Game::GetOnlinePlayerIndex(), 
					player->PlayerAppearance.PlayerOutfit.Hat, 
					player->PlayerAppearance.PlayerOutfit.Accessory, 
					player->PlayerAppearance.PlayerOutfit.TopWear, 
					player->PlayerAppearance.PlayerOutfit.BottomWear, 
					player->PlayerAppearance.PlayerOutfit.Socks, 
					player->PlayerAppearance.PlayerOutfit.Shoes);
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
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Both)();
			return;
		}
	
		const std::vector<std::string> randomopt = {
			Language::getInstance()->get(TextID::VECTOR_RANDOM_OUTFIT),
			Language::getInstance()->get(TextID::VECTOR_RANDOM_PLAYER)
		};	

		Keyboard randkb(Language::getInstance()->get(TextID::KEY_RANDOMIZE_PLAYER), randomopt);
		switch(randkb.Open()) {
			default: break;			
			case 0: 
				Player::WriteOutfit(Game::GetOnlinePlayerIndex(), (Item)Utils::Random(0x280B, 0x28F3), 
																		(Item)Utils::Random(0x28F5, 0x295B), 
																		(Item)Utils::Random(0x2493, 0x26F5), 
																		(Item)Utils::Random(0x26F8, 0x2776), 
																		(Item)Utils::Random(0x2777, 0x279E), 
																		(Item)Utils::Random(0x279F, 0x27E5));
			break;
			case 1: {
				player->PlayerAppearance.PlayerFeatures.HairStyle = Utils::Random(0, 0x21);
				player->PlayerAppearance.PlayerFeatures.HairColor = Utils::Random(0, 0xF);
				player->PlayerAppearance.PlayerFeatures.Face = Utils::Random(0, 4);
				player->PlayerAppearance.PlayerFeatures.EyeColor = Utils::Random(0, 4);
				player->PlayerAppearance.PlayerFeatures.Tan = Utils::Random(0, 0xF);
				
				player->PlayerAppearance.PlayerOutfit.Hat.ID = Utils::Random(0x280B, 0x28F3);
				player->PlayerAppearance.PlayerOutfit.Accessory.ID = Utils::Random(0x28F5, 0x295B);
				player->PlayerAppearance.PlayerOutfit.TopWear.ID = Utils::Random(0x2493, 0x26F5);
				player->PlayerAppearance.PlayerOutfit.BottomWear.ID = Utils::Random(0x26F8, 0x2776);
				player->PlayerAppearance.PlayerOutfit.Socks.ID = Utils::Random(0x2777, 0x279E);
				player->PlayerAppearance.PlayerOutfit.Shoes.ID = Utils::Random(0x279F, 0x27E5);

			//Reloads player style
				Player::UpdateStyle();
			} break;	
		}
	}

//Player Backup/Restore
	void playerbackup(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Both)();
			return;
		}

		WrapLoc locPlayer = { (u32 *)player, sizeof(ACNL_Player) };
		
		const std::vector<std::string> backopt = {
			Language::getInstance()->get(TextID::VECTOR_RANDOM_BACKUP),
			Language::getInstance()->get(TextID::VECTOR_RANDOM_RESTORE),
			Language::getInstance()->get(TextID::FILE_DELETE),  
		};

		Keyboard backkb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), backopt);
		switch(backkb.Open()) {
			default: break;		
			case 0: {
				std::string filename = "";
				Keyboard KB(Language::getInstance()->get(TextID::RANDOM_PLAYER_DUMP));

				if(KB.Open(filename) == -1) {
					return;
				}

				Wrap::Dump(Utils::Format(PATH_PLAYER, Address::regionName.c_str()), filename, ".player", &locPlayer, nullptr);
			} break;
			case 1: {
				Wrap::Restore(Utils::Format(PATH_PLAYER, Address::regionName.c_str()), ".player", Language::getInstance()->get(TextID::RANDOM_PLAYER_RESTORE), nullptr, true, &locPlayer, nullptr); 
				Player::UpdateTan();
				Player::UpdateStyle();
			} break;	
			case 2: 
				Wrap::Delete(Utils::Format(PATH_PLAYER, Address::regionName.c_str()), ".player");
			break;
		}
	}

//TPC Message Changer | Player specific save code
	void tpcmessage(MenuEntry* entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::string input = "";

		Keyboard KB(Language::getInstance()->get(TextID::TPC_MESSAGE_ENTER_NAME));
		KB.SetMaxLength(26);

		if(KB.Open(input) >= 0) {
			Convert::STR_TO_U16(input, player->TPCText);
		}
	}

//TPC Image Dumper | non player specific save code
	void tpc(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		const std::vector<std::string> g_player = {
			Language::getInstance()->get(TextID::VECTOR_PLAYER_1), 
			Language::getInstance()->get(TextID::VECTOR_PLAYER_2), 
			Language::getInstance()->get(TextID::VECTOR_PLAYER_3), 
			Language::getInstance()->get(TextID::VECTOR_PLAYER_4), 
		};
		
		const std::vector<std::string> tpcselectopt = {
			Language::getInstance()->get(TextID::VECTOR_TPCDUMP_DUMP),
			Language::getInstance()->get(TextID::VECTOR_TPCDUMP_RESTORE),
			Language::getInstance()->get(TextID::FILE_DELETE),  
		};

		WrapLoc locTPC;
			
		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), tpcselectopt);

		switch(KB.Open()) {
			default: break;
			
			case 0: {
				Keyboard PKB(Language::getInstance()->get(TextID::KEY_SELECT_PLAYER), g_player);

				int index = PKB.Open();
				if(index < 0) {
					return;
				}

				player = Player::GetSaveData(index);
				if(player) {
					std::string filename = "";
					Keyboard KB(Language::getInstance()->get(TextID::TPC_DUMPER_NAME));

					if(KB.Open(filename) < 0) {
						return;
					}

					locTPC = { (u32 *)player->TPCPic, sizeof(player->TPCPic) };
					Wrap::Dump(Utils::Format(PATH_TPC, Address::regionName.c_str()), filename, ".jpg", &locTPC, nullptr);
				}
			} break;
			
			case 1: 
				player = Player::GetSaveData();
				locTPC = { (u32 *)player->TPCPic, sizeof(player->TPCPic) };
				Wrap::Restore(Utils::Format(PATH_TPC, Address::regionName.c_str()), ".jpg", Language::getInstance()->get(TextID::TPC_DUMPER_RESTORE), nullptr, true, &locTPC, nullptr);
			break;
			
			case 2: 
				Wrap::Delete(Utils::Format(PATH_TPC, Address::regionName.c_str()), ".jpg");
			break;
		}
	}

//dump designs | player specific save code
	void DesignDumper(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> designslots;
		
		for(int i = 1; i <= 10; ++i) {
			designslots.push_back(Utils::Format(Language::getInstance()->get(TextID::VECTOR_DESIGN).c_str(), i));
		}
		
		const std::vector<std::string> designselect = {
			Language::getInstance()->get(TextID::VECTOR_DESIGNDUMP_DUMP), 
			Language::getInstance()->get(TextID::VECTOR_DESIGNDUMP_RESTORE), 
			Language::getInstance()->get(TextID::FILE_DELETE),  
		};

		WrapLoc locPattern;
		int dSlot = 0;
		
		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), designselect);
		
		switch(KB.Open()) {
			default: break;
			
			case 0: { 
				Keyboard DKB(Language::getInstance()->get(TextID::KEYBOARD_DESIGNDUMP), designslots);
				
				dSlot = DKB.Open();
				if(dSlot < 0) {
					return;
				}

				std::string filename = "";
				Keyboard KB(Language::getInstance()->get(TextID::DESIGN_DUMP_NAME));

				if(KB.Open(filename) < 0) {
					return;
				}

				locPattern = { (u32 *)&player->Patterns[player->PatternOrder[dSlot]], sizeof(ACNL_Pattern) };
				Wrap::Dump(Utils::Format(PATH_DESIGN, Address::regionName.c_str()), filename, ".acnl", &locPattern, nullptr);
			} break;
			
			case 1: {
				Keyboard DKB(Language::getInstance()->get(TextID::KEYBOARD_DESIGNDUMP), designslots);
				
				dSlot = DKB.Open();
				if(dSlot < 0) {
					return;
				}

				locPattern = { (u32 *)&player->Patterns[player->PatternOrder[dSlot]], sizeof(ACNL_Pattern) };
				Wrap::Restore(Utils::Format(PATH_DESIGN, Address::regionName.c_str()), ".acnl", Language::getInstance()->get(TextID::DESIGN_DUMP_RESTORE), nullptr, true, &locPattern, nullptr);
				Player::ReloadDesign(player->PatternOrder[dSlot]);
			} break;
			
			case 2: 
				Wrap::Delete(Utils::Format(PATH_DESIGN, Address::regionName.c_str()), ".acnl");
			break;
		}
	}
//Fill Emote List | player specific save code
	void emotelist(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		const std::vector<std::string> emoteopt = {
			Language::getInstance()->get(TextID::VECTOR_EMOTIONLIST_FILL_LIST),
			Language::getInstance()->get(TextID::VECTOR_EMOTIONLIST_FILL_EMOTION),
			Language::getInstance()->get(TextID::VECTOR_EMOTIONLIST_CLEAR_LIST),
		};

		static Address emoticons(0x8902A4);
		Emoticons *gameEmotes = new Emoticons();
		gameEmotes = (Emoticons *)emoticons.addr;
		if(!gameEmotes) {
			return;
		}
		
		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), emoteopt);
	
		switch(KB.Open()) {
			default: break;
			case 0: 
				player->Emotes = *gameEmotes;
			break; 
			case 1: {
				u8 emotion = 0; 
				Keyboard KB(Language::getInstance()->get(TextID::EMOTION_LIST_TYPE_ID));
				KB.IsHexadecimal(true);
				
				if(KB.Open(emotion) < 0) {
					return;
				}
				
				std::memset((void *)player->Emotes.emoticons, emotion, 0x28);
			} break;
			
			case 2: 
				std::memset((void *)player->Emotes.emoticons, 0, 0x28);
			break;
		}
	}
//Fill Enzyklopedia List | player specific save code
	void enzyklopedia(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		const std::vector<std::string> enzyopt = {
			Language::getInstance()->get(TextID::VECTOR_ENZY_FILL),
			Language::getInstance()->get(TextID::VECTOR_ENZY_CLEAR),
		};

		static const Item_Category EncyclopediaID[3] = { 
			Item_Category::Bugs, Item_Category::Fish, 
			Item_Category::SeaCreatures
		};
		
		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), enzyopt);
		
		switch(KB.Open()) {
			default: break;
			case 0:
				for(int i = 0; i < 3; ++i) {
					Player::SetUnlockableBitField(player, EncyclopediaID[i], true);
				}

				for(int i = 0; i < 72; ++i) {
					player->EncyclopediaSizes.Insects[i] = Utils::Random(1, 0x3FFF);
					player->EncyclopediaSizes.Fish[i] = Utils::Random(1, 0x3FFF);

					if(i < 30) {
						player->EncyclopediaSizes.SeaCreatures[i] = Utils::Random(1, 0x3FFF);
					}
				}
			break;
			case 1: 
				for(int i = 0; i < 3; ++i) {
					Player::SetUnlockableBitField(player, EncyclopediaID[i], false);
				}

				for(int i = 0; i < 72; ++i) {
					player->EncyclopediaSizes.Insects[i] = 0;
					player->EncyclopediaSizes.Fish[i] = 0;

					if(i < 30) {
						player->EncyclopediaSizes.SeaCreatures[i] = 0;
					}
				}
			break;
		} 
	}
//Change Dream Code | player specific save code
	void comodifier(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}

		Keyboard kb(Language::getInstance()->get(TextID::DREAM_CODE_ENTER_ID));
		kb.IsHexadecimal(true);
		kb.DisplayTopScreen = true;
		
		u16 part1, part2, part3;

		if(kb.Open(part1, 0) >= 0) {
			if(kb.Open(part2, 0) >= 0) {
				if(kb.Open(part3, 0) >= 0) {
					player->DreamCode.DCPart1 = (part2 << 16) + part3;
					player->DreamCode.DCPart2 = (part1 & 0xFF);
					player->DreamCode.DCPart3 = (part1 >> 8);

					player->DreamCode.HasDreamAddress = true;
				}
			}
		}	
	}
//Enable Unused Menu | player specific save code
	void debug1(MenuEntry *entry) {
		static Address currentSelectedTPCMenu(0x951014);

		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		std::vector<std::string> cmnOpt =  { "" };
		
		bool IsON = player->PlayerFlags.CanUseCensusMenu == 1;

		cmnOpt[0] = IsON ? (Color(pGreen) << Language::getInstance()->get(TextID::VECTOR_ENABLED)) : (Color(pRed) << Language::getInstance()->get(TextID::VECTOR_DISABLED));
		
		Keyboard KB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), cmnOpt);

		int op = KB.Open();
		if(op < 0) {
			return;
		}

		if (IsON) {
			player->PlayerFlags.CanUseCensusMenu = false;
			if (*(u8 *)(currentSelectedTPCMenu.addr) == 1) { //Census Menu is selected
				*(u8 *)(currentSelectedTPCMenu.addr) = 2; //Set to TPC Menu
			}
		} else {
			player->PlayerFlags.CanUseCensusMenu = true;
		}

		debug1(entry);
	}

//Fill Song List | player specific save code
	void FillSongs(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		const std::vector<std::string> songopt = {
			Language::getInstance()->get(TextID::VECTOR_ENZY_FILL),
			Language::getInstance()->get(TextID::VECTOR_ENZY_CLEAR),
		};

		static const std::pair<u16, u16> Pairs = { 0x212B, 0x2186 };
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), songopt);
		
		static Address getField(0x2FF76C);

		switch(optKb.Open()) {
			default: break;
			case 0: { 
				for(u16 i = Pairs.first; i < Pairs.second; ++i) {
					int field = getField.Call<int>(&i);
					player->AddedSongs[(field >> 5)] |= (1 << (field & 0x1F));
				}
			} break;
			case 1:
				for(u16 i = Pairs.first; i < Pairs.second; ++i) {
					int field = getField.Call<int>(&i);
					player->AddedSongs[(field >> 5)] &= ~(1 << (field & 0x1F));
				}
			break;
		}
	}

//Fill Catalog | player specific save code	
	void FillCatalog(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		const std::vector<std::string> songopt = {
			Language::getInstance()->get(TextID::VECTOR_ENZY_FILL),
			Language::getInstance()->get(TextID::VECTOR_ENZY_CLEAR),
		};	

		static const Item_Category CatalogID[15] = {
			Item_Category::Wallpaper, Item_Category::Carpets,
			Item_Category::Furniture, Item_Category::Shirts,
			Item_Category::Dresses, Item_Category::Trousers,
			Item_Category::Socks, Item_Category::Shoes,
			Item_Category::Hats, Item_Category::Accesories,
			Item_Category::Umbrellas, Item_Category::MailPapers,
			Item_Category::Songs, Item_Category::Gyroids,
			Item_Category::AnalyzedFossils
		};
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), songopt);
		
		switch(optKb.Open()) {
			default: break;
			case 0: 
				for(int i = 0; i < 15; ++i) {
					Player::SetUnlockableBitField(player, CatalogID[i], true);
				}
			break;
			case 1: 
				for(int i = 0; i < 15; ++i) {
					Player::SetUnlockableBitField(player, CatalogID[i], false);
				}
			break;
		}
    }

//Unlock QR Machine | half player specific save code	
	void unlockqrmachine(MenuEntry *entry) {	
		ACNL_Player *player = Player::GetSaveData();
		ACNL_TownData *town = Town::GetSaveData();

		if(!player || !town) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO)).SetClear(ClearScreen::Top)();
			return;
		}
		
		const std::vector<std::string> cmnOpt =  {
			Language::getInstance()->get(TextID::VECTOR_ENABLE),
			Language::getInstance()->get(TextID::VECTOR_DISABLE)
		};

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), cmnOpt);

		int op = optKb.Open();
		if(op < 0) {
			return;
		}

		bool enable = op == 0;

		town->TownFlags.QRMachineUnlocked = enable;

		player->PlayerFlags.BefriendSable1 = enable;
		player->PlayerFlags.BefriendSable2 = enable;
		player->PlayerFlags.BefriendSable3 = enable;
	}
}