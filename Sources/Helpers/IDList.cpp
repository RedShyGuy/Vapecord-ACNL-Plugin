#include "Helpers/IDList.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/NPC.hpp"
#include "Address/Address.hpp"
#include "Language.hpp"
#include "Helpers/ItemSearcher/ItemCheckMethods.hpp"

#define RANGE(X, START, END)	((X & 0xFFFF) >= START && (X & 0xFFFF) <= END)
#define IS(X, ADDR)				((X & 0xFFFF) == ADDR)

namespace CTRPluginFramework {
	Range KeyRange::sRange;

	void ValidKeyboardCheck(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u16 ID = StringToHex<u16>(input, 0xFFFF);
		if(!IDList::ValidID((ID & 0xFFFF), KeyRange::sRange.start, KeyRange::sRange.end)) {
			keyboard.SetError(Color::Red << Language::getInstance()->get("INVALID_ID"));
			return;
		}
	}

	void ItemChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		Item ID = (Item)StringToHex<u32>(input, 0xFFFF);
		if(!IDList::ItemValid(ID)) {
			keyboard.SetError(Color::Red << "Invalid ID!");
			return;
		}
	}

	void TextItemChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();
		Item ID = (Item)StringToHex<u32>(input, 0xFFFF);

		if(!IDList::ItemValid(ID, false)) {
			keyboard.GetMessage() = "";
			keyboard.SetError(Color::Red << Language::getInstance()->get("INVALID_ID"));
			return;
		}

		keyboard.GetMessage() = IDList::GetItemName(ID);
	}

	bool IDList::IsHalfAcre(u8 acreID) {
		if(RANGE(acreID, 0x9E, 0xA3) || IS(acreID, 0xA8))
			return true;

		return false;
	}

	bool IDList::RoomValid(u8 roomID) {
		//if villager not exists room warp not work
		//if no exhibtion house those crash: 0x92 - 0x97

		if(!IDList::ValidID(roomID, 0, 0xA4))
			return false;

		if(IDList::ValidID(roomID, 0x27, 0x29) || IDList::ValidID(roomID, 0x5D, 0x60) || IDList::ValidID(roomID, 0x98, 0x9C) || IDList::ValidID(roomID, 0x69, 0x8D))
			return false;
		
		switch(roomID) {
			case 0x62: 
			case 0x65: 
			case 0x66: 
				return false;
		}
		
		return true;
	}
//If menu is valid
	bool IDList::MenuValid(u8 MenuID) {
		if(RANGE(MenuID, 5, 7) || RANGE(MenuID, 0x2E, 0x43) || RANGE(MenuID, 0x47, 0x48) || RANGE(MenuID, 0x5F, 0x60) || MenuID == 0x65 || 
			RANGE(MenuID, 0x6A, 0x6C) || RANGE(MenuID, 0x70, 0x71) || RANGE(MenuID, 0x79, 0x7A) || MenuID == 0x87 || MenuID == 0x89)
			return 1;

		return 0;
	}
//if building is valid
	bool IDList::BuildingValid(u8 buildingID) {
		if(buildingID > 0x4B && buildingID < 0x69 || buildingID > 0x6A && buildingID < 0x81 || buildingID > 0x8F && buildingID < 0xFC) 
			return true;
		
		return false;
	}
//if emotion is valid
	bool IDList::EmotionValid(u8 emotionID) {
		return (emotionID > 0 && emotionID < 0x41);
	}
//If snake is valid
	bool IDList::SnakeValid(u16 snakeID) {
		if(snakeID > 0 && snakeID < 0x276) {
			switch(snakeID) {
				default: return true;
				case 0xF7: 
				case 0xFA:
				case 0xFB:
				case 0xFC:
				case 0xFD:
				case 0x124:
				case 0x125:
					return false;
			}
		}
		
		return false;
	}
//If animation is valid
	bool IDList::AnimationValid(u8 animID, u8 playerIndex) {
		if(animID > 0 && animID < 0xEB) {
			if(IsIndoorsBool) {
				switch(animID) {
				//those would be fixed but they crash others indoors :/
					case 0x5F:
					case 0x7E:
					case 0x87:
				//those teleport and i am not sure how to prevent it
					case 0x33:
					case 0xD1:
					case 0xD2:
					case 0xD3:
					case 0xD7:
					case 0xD8: 
						return false;
				}
			}
			
			if(!Player::IsInRoom(0, playerIndex)) {
			//stuff for kappns boat
				switch(animID) {
					case 0xBB:
					case 0xBC:
					case 0xBE:
					case 0xBF: 
						return false;
				}
			}
			return true;
		}
		
		return false;
	}
//If music is valid
	bool IDList::MusicValid(u16 musicID) {
		if(musicID < 0xFFF) {
			if(musicID > 0xDB && musicID < 0x10B) 
				return false;
			
			return true;
		}
		
		return false;
	}

	bool FlagValid(u16 flagID, bool IsDropped) {
	//If player is indoors and drops item
		if(IsIndoorsBool && IsDropped) {
			if(flagID >= 0x8000)
				return false;
		}
		return true;
	}
//If item is valid
	bool IDList::ItemValid(Item itemID, bool IsDropped) {
		u16 item = itemID.ID;
		u16 flag = itemID.Flags;

		if(!FlagValid(flag, IsDropped))
			return false;

		if((item & 0xFFFF) >> 12 == 4 || (item & 0xFFFF) >> 12 == 5) //if present
			item = item - 0x2000; //convert present to standard Item ID to know if it is valid

	//If player is outdoors or doesnt drop item
		if(!IsIndoorsBool || !IsDropped) {
			if(IS(item, 0x7FFE) || RANGE(item, 0, 0xFD)) 
				return true;
		}

		if(RANGE(item, 0x2001, 0x2060) || RANGE(item, 0x2089, 0x2185) || RANGE(item, 0x21E4, 0x22DF) || RANGE(item, 0x22E1, 0x30CB) || RANGE(item, 0x30D2, 0x3108) || RANGE(item, 0x3130, 0x33B4) || RANGE(item, 0x33BC, 0x34CD) || RANGE(item, 0x3726, 0x372A))
			return true;
		
		return false;
	}

//If tool is valid
	bool IDList::ToolsValid(Item toolsID) {
		return (toolsID.ID == 0x2001 || (toolsID.ID == 0x3729) || (toolsID.ID > 0x334B && toolsID.ID < 0x33A3));
	}

//Get Building Name
	std::string IDList::GetBuildingName(u8 ID) {
		if(ID >= 8 && ID <= 0x11) {
			ACNL_VillagerData *villager = NPC::GetSaveData();
			if(!villager)
				return Utils::Format("NPC %d", ID - 8);

			u16 VID = villager->Villager[ID - 8].Mini1.VillagerID;
			if(VID == 0xFFFF)
				return "NPC -Empty-";

			return "NPC " + NPC::GetNName(VID);
		}

		for(const ID_U8Data& buildings : Buildings) {
			if(buildings.ID == ID) {
				return std::string(buildings.Name);
			}
		}

		return Language::getInstance()->get("INVALID");
	}
//get country name
	std::string IDList::SetCountryName(u8 country) {
		for(const ID_U8Data& countrys : Countrys) {
			if(countrys.ID == country) {
				return std::string(countrys.Name);
			}
		}

		return Language::getInstance()->get("INVALID");
	}

	std::string IDList::GetItemName(Item item) {
		if (IDList::hasItemNoName(item)) {
			for (const ID_U16Data& Items : Items) {
				if (Items.ID == item.ID) {
					return std::string(Items.Name);
				}
			}
		}

		static Address SetUpStack(0x3081E8);
		static Address SetUpItem(0x769DBC);
		static Address SetUp(0x312610);

		u32 Stack[44];
		u32 add = SetUpStack.Call<u32>(Stack, Stack + 0x18, 0x21);

		u16 itemID = SetUpItem.Call<u16>(&item);

		SetUp.Call<void>(*(u32 *)Address(0x95EEDC).addr, add, (char *)"STR_Item_name", itemID);

		std::string ItemName = "";
		Process::ReadString(Stack[1] + 0xC, ItemName, 0x30, StringFormat::Utf16);

		if (ItemName.empty()) {
			return "???";
		}
		else {
			ItemName += 2; //Skips formatting chars
			ItemName.pop_back(); //Removes formatting char
			return ItemName;
		}
	} 

	bool IDList::hasItemNoName(Item item) {
		if (item.ID >= 0 && item.ID <= 0x00FD)
			return true;

		if (item.ID == 0x2037)
			return true;

		if (item.ID >= 0x2493 && item.ID <= 0x2494)
			return true;

		if (item.ID >= 0x30A3 && item.ID <= 0x30A7)
			return true;

		if (item.ID == 0x324D)
			return true;

		if (item.ID == 0x3263)
			return true;

		if (item.ID >= 0x33A7 && item.ID <= 0x33B4)
			return true;

		if (item.ID >= 0x340C && item.ID <= 0x3419)
			return true;

		if (item.ID == 0x341E)
			return true;

		if (item.ID >= 0x3727 && item.ID <= 0x3729)
			return true;

		return false;
	}

	

	u32 FUN_BE57C4(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
		static Address FUN_00001144(0x2FCC14); //Returns shortened item (?), or u16
		static Address FUN_0000114c(0x2FEA78); //Returns u32(?)
		static Address FUN_00001154(0x2F776C); //Returns void, probably thunk

		u32 uVar1;
		u32 uStack_10;
		
		uStack_10 = param_4;
		uVar1 = FUN_00001144.Call<u16>(&uStack_10, 0x2000);
		uVar1 = FUN_0000114c.Call<u16>(param_1, uVar1, 0);
		FUN_00001154.Call<void>(&uStack_10);
		return uVar1;
	}

	static constexpr int MAX_CATEGORIES = 15;

	Item_Categories Categories_To_Search[15] = {
        Wallpaper, Carpets, Furniture, 
        Shirts, Trousers, Dresses, 
		Hats, Accesories, Shoes, 
        Socks, Umbrellas, MailPapers, 
        Songs, Gyroids, AnalyzedFossils
    };

	u16 GetItemByCategory(u32 param_1, Item_Categories itemCategory, int index) {
		static Address FUN_0000132C(0x771278);
		return FUN_0000132C.Call<u16>(param_1, (u8)itemCategory, index);
	}

	struct __attribute__((packed)) SearchData {
		u8 unk_1[0x38]; //0
		u32 unk_2; //0x38
		u32 unk_3; //0x3C
		u32 unk_4; //0x40
		u32 unk_5; //0x44
		u32 unk_6; //0x48
		u32 unk_7; //0x4C
		u32 unk_8; //0x50
		u32 unk_9; //0x54
		u32 unk_A; //0x58 (When Catalog opens 0x9B, After Search 0x2C)
		u32 increment_data_1[0x42]; //0x5C (Default -1, increment list, after search stays increment list (0 -> 0x42))
		u32 increment_data_2[0x6A9]; //0x168 (Default -1, increment list, after search goes back to -1 (0x43 -> 0x6EC))
		int Item_Category_Index; //0x1C10
		int Found_Items; //0x1C14
		u32 unk_B; //0x1C18
		u32 unk_C; //0x1C1C
		u32 unk_D; //0x1C20
	};

	bool AAASearchItemByKeywordFUNC(u32 param) {
		SearchData *data = (SearchData *)param;

		static Address FUN_007712C0(0x7712C0); //Returns u32
		static Address FUN_006F02F4(0x6F02F4); //Returns bool
		static Address FUN_002FCC14(0x2FCC14); //Returns shortened item (?), or u16
		static Address FUN_0076A66C(0x76A66C); //Returns u32
		static Address FUN_002F776C(0x2F776C); //Returns void, probably thunk
		static Address FUN_002FEA78(0x2FEA78); //Returns u32(?)
		static Address FUN_0075C5B8(0x75C5B8); //Returns u32(?)
		static Address FUN_005F56E4(0x5F56E4); //Returns u32, probably thunk
		static Address FUN_00308368(0x308368); //Returns void, probably thunk
		static Address FUN_0030B6BC(0x30B6BC); //Returns u16, checks item & 0x7FFF
		static Address FUN_002FD0BC(0x2FD0BC); //Returns u32 (?)
		static Address FUN_00301B3D(0x301B3D); //Returns u32

		u32 _LAB_00003d58 = *(u32 *)(0xC0CE88);
		u32 _LAB_00003d64 = *(u32 *)(0x953CC4); //Always 0 ?
		u32 _LAB_00003d70 = *(u32 *)(0xC086B8);
		u32 _LAB_00003d74 = *(u32 *)(0xC086BC);

		//u32 _LAB_00003d6c = *(u32 *)(0x849D0C); //Seems to be always 0x32
		constexpr int MAX_VALUE = 50; //For what i dont know, seems to stop the search

		u32 local_b8[3];
		u32 *piVar6;
		u32 iVar11 = 0;

		u16 uVar2;
		u32 uVar3;
		u32 uVar4;
		u32 iVar5;
		u32 uVar9;
		
		bool bVar13;

		u32 Stack1_34[21];
		u8 Stack2_AC[80];
		u32 saveOffset = *(u32 *)(0xAAFF18 + 0x234); //Player Save Offset

		do {
		//Set to next category, unsure yet why it is a while loop
			while (uVar3 = FUN_007712C0.Call<u32>(data->unk_A), uVar3 <= data->Found_Items) {
				data->Item_Category_Index++;

			//If all categories got searched -> exit
				if (MAX_CATEGORIES <= data->Item_Category_Index) {
					data->Found_Items = 0;
					data->unk_B = _LAB_00003d70;
					data->unk_C = _LAB_00003d74;
					return 0;
				}

				//saveOffset + 0x6C20 is player save stuff (don't know what exactly)
				FUN_006F02F4.Call<bool>(data->unk_A, saveOffset + 0x6C20, (u8)Categories_To_Search[data->Item_Category_Index]);

				data->Found_Items = 0;

				iVar11 += 5;
				if (MAX_VALUE <= iVar11) {
					return 0;
				}
			}
			
			uVar4 = GetItemByCategory(data->unk_A, Categories_To_Search[data->Item_Category_Index], data->Found_Items);
			FUN_002FCC14.Call<u16>(Stack1_34, uVar4); //goes through every item by category
			
			iVar5 = FUN_0076A66C.Call<u32>(Stack1_34);
			if (iVar5 != 0) {
				FUN_002FEA78.Call<u32>(local_b8, Stack1_34, 0);

				iVar5 = FUN_0075C5B8.Call<u32>(local_b8, data->unk_2, 1);
				if (iVar5 == 0) {
					uVar3 = data->unk_D;
					if (uVar3 == 0) {
				LAB_00003b7c:
						if (uVar3 != 0) {
							goto LAB_00003b84;
						}
					}
					else {
						uVar3 = 1;
						uVar9 = 0;

						Address *pcVar8 = nullptr;

						do {
							if ((data->unk_D & uVar3) != 0) {
								Check_Data checkData = CHECKS[uVar9];
								bVar13 = (checkData.unk & 1U) == 0;
								//piVar6 = (u32 *)(param_1 + (piVar7.unk >> 1));

								if (bVar13) {

									//pcVar8 = new Address(piVar7.method);
								} else {
									//pcVar8 = new Address(*piVar6 + piVar7.method);
								}

								if (pcVar8 != nullptr) {
									iVar5 = pcVar8->Call<u32>(piVar6, Stack1_34);
								}
								
								pcVar8 = nullptr;
								if (iVar5 != 0) {
									uVar3 = 1;
									goto LAB_00003b7c;
								}
							}

							uVar9++;
							uVar3 <<= 1;
						} while (uVar9 < 0x13);
					}
				}
				else {
				LAB_00003b84:
					uVar2 = FUN_0030B6BC.Call<u16>(Stack1_34);

					if (data->unk_5 == 0) {
						data->unk_9++;

						uVar4 = FUN_002FD0BC.Call<u32>(0xBC4, _LAB_00003d64);
						iVar5 = FUN_00301B3D.Call<u32>(uVar4, (u32)FUN_BE57C4, 0x5C, 0x20);

						*(u32 *)(iVar5 + 0xBC0) = 0;
						data->unk_6 = iVar5;
						data->unk_5 = iVar5;
					}

					if (0x1F < data->unk_7) {

						data->unk_7 = 0;
						data->unk_9++;

						uVar4 = FUN_002FD0BC.Call<u32>(0xBC4, _LAB_00003d64);
						iVar5 = FUN_00301B3D.Call<u32>(uVar4, (u32)FUN_BE57C4, 0x5C, 0x20);

						*(u32 *)(iVar5 + 0xBC0) = 0;
						*(u32 *)(data->unk_6 + 0xBC0) = iVar5;
						data->unk_6 = iVar5;
					}

					*(u16 *)(data->unk_6 + data->unk_7 * 2 + 0xB80) = uVar2;
					data->unk_8++;
					data->unk_7++;
				}

				local_b8[0] = _LAB_00003d58;
				iVar5 = FUN_005F56E4.Call<u32>(Stack2_AC);
				FUN_00308368.Call<void>(iVar5 + -0xC);
			}

			iVar11++;
			data->Found_Items++;

			if (MAX_VALUE <= iVar11) {
				FUN_002F776C.Call<void>(Stack1_34);
				return 0;
			}
			FUN_002F776C.Call<void>(Stack1_34);
		} while(true);
	}

	bool SearchItemByKeywordFUNC(u32 param_1/*0x307A6B70*/) {
		static Address FUN_007712C0(0x7712C0); //Returns u32
		static Address FUN_006F02F4(0x6F02F4); //Returns bool
		static Address FUN_002FCC14(0x2FCC14); //Returns shortened item (?), or u16
		static Address FUN_0076A66C(0x76A66C); //Returns u32
		static Address FUN_002F776C(0x2F776C); //Returns void, probably thunk
		static Address FUN_002FEA78(0x2FEA78); //Returns u32(?)
		static Address FUN_0075C5B8(0x75C5B8); //Returns u32(?)
		static Address FUN_005F56E4(0x5F56E4); //Returns u32, probably thunk
		static Address FUN_00308368(0x308368); //Returns void, probably thunk
		static Address FUN_0030B6BC(0x30B6BC); //Returns u16, checks item & 0x7FFF
		static Address FUN_002FD0BC(0x2FD0BC); //Returns u32 (?)
		static Address FUN_00301B3D(0x301B3D); //Returns u32

		u32 _LAB_00003d58 = *(u32 *)(0xC0CE88);
		u32 _LAB_00003d64 = *(u32 *)(0x953CC4); //Always 0 ?
		//u32 _LAB_00003d6c = *(u32 *)(0x849D0C); //Seems to be always 0x32
		u32 _LAB_00003d70 = *(u32 *)(0xC086B8);
		u32 _LAB_00003d74 = *(u32 *)(0xC086BC);

		constexpr int MAX_VALUE = 50; //For what i dont know, seems to stop the search

		u32 local_b8[3];
		u32 *piVar6;
		u32 iVar11 = 0;

		u16 uVar2;
		u32 uVar3;
		u32 uVar4;
		u32 iVar5;
		u32 uVar9;
		
		bool bVar13;

		u32 Stack1_34[21];
		u8 Stack2_AC[80];
		u32 saveOffset = *(u32 *)(0xAAFF18 + 0x234); //Player Save Offset

		int* ItemCategoryIndex = (int *)(param_1 + 0x1C10); //Maybe could just be a simple int without memory offset
		int* FoundItems = (int *)(param_1 + 0x1C14); //0 is no item, 1 is 1 item found, etc (Only correct after search)
		u32* CleanUp1 = (u32 *)(param_1 + 0x1C18); //Not sure for what
		u32* CleanUp2 = (u32 *)(param_1 + 0x1C1C); //Not sure for what

		do {
		//Set to next category, unsure yet why it is a while loop
			while (uVar3 = FUN_007712C0.Call<u32>(param_1 + 0x58), uVar3 <= *FoundItems) {
				(*ItemCategoryIndex)++;

			//If all categories got searched -> exit
				if (MAX_CATEGORIES <= *ItemCategoryIndex) {
					(*FoundItems) = 0;
					(*CleanUp1) = _LAB_00003d70;
					(*CleanUp2) = _LAB_00003d74;
					return 0;
				}

				//saveOffset + 0x6C20 is player save stuff (don't know what exactly)
				FUN_006F02F4.Call<bool>(param_1 + 0x58, saveOffset + 0x6C20, (u8)Categories_To_Search[*ItemCategoryIndex]);

				(*FoundItems) = 0;

				iVar11 += 5;
				if (MAX_VALUE <= iVar11) {
					return 0;
				}
			}
			
			uVar4 = GetItemByCategory(param_1 + 0x58, Categories_To_Search[*ItemCategoryIndex], *FoundItems);
			FUN_002FCC14.Call<u16>(Stack1_34, uVar4); //goes through every item by category
			
			iVar5 = FUN_0076A66C.Call<u32>(Stack1_34);
			if (iVar5 != 0) {
				FUN_002FEA78.Call<u32>(local_b8, Stack1_34, 0);

				iVar5 = FUN_0075C5B8.Call<u32>(local_b8, param_1 + 0x38, 1);
				if (iVar5 == 0) {
					uVar3 = *(u32 *)(param_1 + 0x1C20);
					if (uVar3 == 0) {
				LAB_00003b7c:
						if (uVar3 != 0) {
							goto LAB_00003b84;
						}
					}
					else {
						uVar3 = 1;
						uVar9 = 0;

						do {
							if ((*(u32 *)(param_1 + 0x1C20) & uVar3) != 0) {
								Check_Data checkData = CHECKS[uVar9];
								bVar13 = (checkData.unk & 1U) == 0;
								piVar6 = (u32 *)(param_1 + (checkData.unk >> 1));

								if (bVar13) {
									iVar5 = checkData.method(piVar6, Stack1_34);
								} else {
									// müssen wir uns erst piVar6 = (u32 *)(param_1 + (checkData.unk >> 1)); anschauen tatsächlich

									//pcVar8 = new Address(*piVar6 + methodData.method);
									//iVar5 = pcVar8->Call<u32>(piVar6, Stack1_34);
								}
								
								if (iVar5 != 0) {
									uVar3 = 1;
									goto LAB_00003b7c;
								}
							}

							uVar9++;
							uVar3 <<= 1;
						} while (uVar9 < 0x13);
					}
				}
				else {
				LAB_00003b84:
					uVar2 = FUN_0030B6BC.Call<u16>(Stack1_34);

					if (*(u32 *)(param_1 + 0x44) == 0) {
						*(u32 *)(param_1 + 0x54) = *(u32 *)(param_1 + 0x54) + 1;

						uVar4 = FUN_002FD0BC.Call<u32>(0xBC4, _LAB_00003d64);
						iVar5 = FUN_00301B3D.Call<u32>(uVar4, (u32)FUN_BE57C4, 0x5C, 0x20);

						*(u32 *)(iVar5 + 0xBC0) = 0;
						*(u32 *)(param_1 + 0x48) = iVar5;
						*(u32 *)(param_1 + 0x44) = iVar5;
					}

					if (0x1F < *(u32 *)(param_1 + 0x4C)) {

						*(u32 *)(param_1 + 0x4C) = 0;
						*(u32 *)(param_1 + 0x54) = *(u32 *)(param_1 + 0x54) + 1;

						uVar4 = FUN_002FD0BC.Call<u32>(0xBC4, _LAB_00003d64);
						iVar5 = FUN_00301B3D.Call<u32>(uVar4, (u32)FUN_BE57C4, 0x5C, 0x20);

						*(u32 *)(iVar5 + 0xBC0) = 0;
						*(u32 *)(*(u32 *)(param_1 + 0x48) + 0xBC0) = iVar5;
						*(u32 *)(param_1 + 0x48) = iVar5;
					}

					*(u16 *)(*(u32 *)(param_1 + 0x48) + *(u32 *)(param_1 + 0x4C) * 2 + 0xB80) = uVar2;
					*(u32 *)(param_1 + 0x50) = *(u32 *)(param_1 + 0x50) + 1;
					*(u32 *)(param_1 + 0x4c) = *(u32 *)(param_1 + 0x4c) + 1;
				}

				local_b8[0] = _LAB_00003d58;
				iVar5 = FUN_005F56E4.Call<u32>(Stack2_AC);
				FUN_00308368.Call<void>(iVar5 + -0xC);
			}

			iVar11++;
			(*FoundItems)++;

			if (MAX_VALUE <= iVar11) {
				FUN_002F776C.Call<void>(Stack1_34);
				return 0;
			}
			FUN_002F776C.Call<void>(Stack1_34);
		} while(true);
	}

	bool SearchItemByKeyword(std::string& Keyword) {
		u32 baseInvPointer = *(u32 *)(Game::BaseInvPointer() + 0xC);
		u32 searchStringData = *(u32 *)(baseInvPointer + 0x3BAC);

		//String of item with formatting chars at the beginning / +0xC starts actual string
		//u32 actualSearchString = *(u32 *)(searchStringData);
		SearchItemByKeywordFUNC(searchStringData); //Called by 0x21C1C0

		Process::CopyMemory((u32 *)0xA00000, (u32 *)0x330BDA90, 0x500);
		return false;
	}

	//Item* searchByName(const std::string& name) const;
    //std::vector<ItemReader::Entry> searchAllByName(const std::string& name) const;

	/*
	Item* ItemReader::searchByName(const std::string& name) const {
        std::vector<ItemReader::Entry> items = searchAllByName(name);
        if (items.empty()) {
            return nullptr;
        }

        // look for exact match among the results
        int pos = 0;
        for(Entry item : items) {
            if(item.name == name) {
                return new Item(item.item);
            }
        }

        // get first found item if no exact match
        return new Item(items[0].item);
    }

    std::vector<ItemReader::Entry> ItemReader::searchAllByName(const std::string& name) const {
        std::vector<Entry> items;

        std::string nameCopy = name;
        UtilsExtras::ConvertToLowcase(nameCopy);

		for(Entry entry : entries) {
            std::string entryName = entry.name;
			if(entryName.find(nameCopy) != std::string::npos) {
                items.push_back(entry);
			}
		}

		return items;
    }*/
	//struct MsgBox {
	//	/*0x98D56C*/s8 answerCount = 0; //seems to tell how many answers (0 = 1, 1 = 2)
	//	/*0x98D56D*/s8 openingSound = 0; //seems to choose msg box opening sound
	//	/*0x98D56E*/s16 padding = 0; //never changes
	//	/*0x98D570*/s32 msg_stringName = -1;
	//	/*0x98D574*/s32 decline_stringName = -1;
	//	/*0x98D578*/s32 unknown3 = -1;
	//	/*0x98D57C*/s32 accept_stringName = -1;
	//	/*0x98D580*/s32 accept_soundID = -1;
	//	/*0x98D584*/s32 decline_soundID = -1;
	//	/*0x98D588*/s32 unknown6 = -1; //Either -1, -2, -3
	//	/*0x98D58C*/s32 unknown7 = 0; //Only Function 0x537D84 results get written to it
	//	/*0x98D590*/char* string_fileName = nullptr;
	//	/*0x98D594*/char* playerName = nullptr; //Seems to always be the player name
	//	/*0x98D598*/s32 unknown8 = 0; //never changes(?)
	//	/*0x98D59C*/s32 unknown9 = 0; //never changes(?)
	//	/*0x98D5A0*/s32* funcPointer = nullptr; //seems to only have Function 0x21ACE8
	//	/*0x98D5A4*/s8 unknown10 = 1; //Only gets 1 written to
	//	/*0x98D5A5*/s8 unknown11 = 1; //Only gets 1 or 0 written to
	//	/*0x98D5A6*/s8 unknown12 = 0; //Only gets 1 or 0 written to
	//	/*0x98D5A7*/s8 unknown13 = 0; //Only gets 1 written to
	//};

	//FUNCT(0x6D32D0).Call<void>(); //Clear MSG Box Struct (0x098D56C)

	//0x251B70(0x32DC48B8)

	/*

	void func(void) {
		MsgBox msgbox;
		msgbox.answerCount = 1;

		msgbox.accept_stringName = 0x180;
		msgbox.decline_stringName = 0x181;
		msgbox.msg_stringName = 0x177;

		msgbox.string_fileName = (char *)"SYS_2D_UI";

		msgbox.unknown10 = 1;
		msgbox.unknown11 = 1;
		msgbox.unknown12 = 0;

		msgbox.accept_soundID = 0x1000429;
	}

	*/

//Get Room Name
	std::string IDList::GetRoomName(u8 ID) {
		static Address RoomName(0x5B4BE4); 

		if(ID <= 0xA5) 
			return Color::Green << (std::string)(RoomName.Call<char *>(ID));
		
		return Color::Red << Language::getInstance()->get("INVALID");
	}
//If Invalid ID
	bool IDList::ValidID(u16 ID, u16 StardID, u16 EndID) {
		return (ID >= StardID && ID <= EndID);
	}	
}