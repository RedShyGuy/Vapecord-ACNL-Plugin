#include <CTRPluginFramework.hpp>
#include "TextFileParser.hpp"
#include "cheats.hpp"

#define RANGE(X, START, END)	((X & 0xFFFF) >= START && (X & 0xFFFF) <= END)
#define IS(X, ADDR)				((X & 0xFFFF) == ADDR)

namespace CTRPluginFramework {
	Range KeyRange::sRange;

	void ValidKeyboardCheck(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u16 ID = StringToHex<u16>(input, 0xFFFF);
		if(!IDList::ValidID((ID & 0xFFFF), KeyRange::sRange.start, KeyRange::sRange.end)) {
			keyboard.SetError(Color::Red << Language->Get("INVALID_ID"));
			return;
		}
	}

	void ItemChange(Keyboard& keyboard, KeyboardEvent& event) {
		std::string& input = keyboard.GetInput();	
		u32 ID = StringToHex<u32>(input, 0xFFFF);
		if(!IDList::ItemValid((ID & 0xFFFFFFFF))) {
			keyboard.SetError(Color::Red << "Invalid ID!");
			return;
		}
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
	bool IDList::AnimationValid(u8 animID) {
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
			
			if(!GameHelper::IsInRoom(0)) {
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
	bool IDList::ItemValid(u32 itemID, bool IsDropped) {
		u16 item = (itemID & 0xFFFF);
		u16 flag = ((itemID >> 16) & 0xFFFF);

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
	bool IDList::ToolsValid(u16 toolsID) {
		return (toolsID == 0x2001 || (toolsID == 0x3729) || (toolsID > 0x334B && toolsID < 0x33A3));
	}

//Get Building Name
	std::string IDList::GetBuildingName(u8 ID) {
		if(ID >= 8 && ID <= 0x11) {
			u32 VillagerBase = Save::GetInstance()->Address(0x292A4); //VID

			u8 Index = ID - 8;
			u32 Address = VillagerBase + (0x2518 * Index);
			u16 VID = *(u16 *)(Address + 0x2C);

			return "NPC " + GetNNPCName(VID);
		}

		for(const IDS1& buildings : Buildings) {
			if(buildings.ID == ID) {
				return std::string(buildings.Name);
			}
		}

		return Language->Get("INVALID");
	}
//get country name
	std::string IDList::SetCountryName(u8 country) {
		for(const IDS1& countrys : Countrys) {
			if(countrys.ID == country) {
				return std::string(countrys.Name);
			}
		}

		return Language->Get("INVALID");
	}
	
	std::string IDList::GetSeedName(u16 itemID) {
		for(int i = 0; i < ItemFileLenght; i++) {
			if(ItemList->ID[i] == itemID) {
				return ItemList->Name[i];
			}
		}
		return Language->Get("INVALID");
	}

	/*std::string IDList::GetItemName(u16 ItemID) {
		static const u32 ItemFunc = Region::AutoRegion(0x2FEA78, 0, 0, 0, 0, 0, 0, 0); 

		u32 Stack[44];
		
		static FUNCT func(ItemFunc);
		func.Call<void>(Stack, &ItemID, 0);

		std::string ItemName = "";
		Process::ReadString(Stack[1], ItemName, 0x20, StringFormat::Utf16);

		return ItemName == "" ? "???" : ItemName;
	}*/

	std::string IDList::GetNNPCName(u16 VID) {
		static const u32 SetUp = Region::AutoRegion(0x308210, 0x3083E4, 0x308298, 0x308298, 0x30822C, 0x30822C, 0x308240, 0x308240);
		static const u32 NNPCModelData = Region::AutoRegion(0xA84AF0, 0xA83AF0, 0xA83AF0, 0xA83AF0, 0xA7DAF0, 0xA7CAF0, 0xA7CAF0, 0xA7CAF0);

		u32 Stack[44];
		u32 add = FUNCT(Code::SetupStackData).Call<u32>(Stack);

		u32 npcModel = *(u32 *)(NNPCModelData) + VID * 0x22;
		FUNCT(SetUp).Call<void>(*(u32 *)Code::DataPointer, add, (char *)"STR_NNpc_name", npcModel + 10);

		std::string NNPCName = "";
		Process::ReadString(Stack[1], NNPCName, 0x20, StringFormat::Utf16);
		return NNPCName == "" ? "???" : NNPCName;
	}

	std::string IDList::GetSPNPCName(u8 SPVID) {
		static const u32 SetUp = Region::AutoRegion(0x75D108, 0x75C0EC, 0x75C110, 0x75C0E8, 0x75B8A8, 0x75B880, 0x75B450, 0x75B428);

		u32 Stack[44];
		u32 add = FUNCT(Code::SetupStackData).Call<u32>(Stack);

		FUNCT(SetUp).Call<void>(*(u32 *)Code::DataPointer, add, (char *)"STR_SPNpc_name", SPVID);

		std::string SPNPCName = "";
		Process::ReadString(Stack[1], SPNPCName, 0x20, StringFormat::Utf16);
		return SPNPCName == "" ? "???" : SPNPCName;
	}

	bool IDList::PopulateNPCAmiibo(SpecieID specieID, std::vector<std::string> &vec, std::vector<PACKED_AmiiboInfo> &info, bool HoldenFillyAllowed, bool NonCaravanAllowed) {
		if(specieID == SpecieID::Special) {
			vec.clear();
			info.clear();

			for(const SPAmiiboInfo& amiibo : amiiboSPVillagers) {
				if(!NonCaravanAllowed && amiibo.VID == 0xFFFF) //Non Caravan get skipped
					continue;

				std::string Name = "";
				if(amiibo.SPVID == 0xFE) Name = "Villager";
				else if(amiibo.SPVID == 0xFF) Name = "Timmy&Tommy";
				else Name = GetSPNPCName(amiibo.SPVID);

				info.push_back(PACKED_AmiiboInfo{Name, amiibo.ID0, amiibo.ID1, amiibo.VID});
				vec.push_back(Name);
			}

			return true;
		}

		else if(specieID >= (SpecieID)0) {
			vec.clear();
			info.clear();

			for(const AmiiboInfo& amiibo : amiiboVillagers) {
				if(!HoldenFillyAllowed && amiibo.ID0 == 0) //Holden and Filly get skipped
					continue;

				if(amiibo.Species == specieID) {
					std::string Name = GetNNPCName(amiibo.VID);
					info.push_back(PACKED_AmiiboInfo{Name, amiibo.ID0, amiibo.ID1, amiibo.VID});
					vec.push_back(Name);
				}
			}
			return true;
		}
		return false;
	}

//Get Room Name
	std::string IDList::GetRoomName(u8 ID) {
		static const u32 RoomName = Region::AutoRegion(0x5B4BE4, 0x5B40FC, 0x5B3C2C, 0x5B3C2C, 0x5B351C, 0x5B351C, 0x5B31F0, 0x5B31F0); 
		static FUNCT func(RoomName);

		if(ID <= 0xA5) 
			return Color::Green << (std::string)(func.Call<char *>(ID));
		
		return Color::Red << Language->Get("INVALID");
	}
//If Invalid ID
	bool IDList::ValidID(u16 ID, u16 StardID, u16 EndID) {
		return (ID >= StardID && ID <= EndID);
	}	
}