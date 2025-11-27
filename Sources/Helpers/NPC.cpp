#include "Helpers/NPC.hpp"
#include "Address/Address.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/IDList.hpp"
#include "RuntimeContext.hpp"

namespace CTRPluginFramework {
/*
Gets npc data for anim mods, coord mods, etc 0xB6F9B4
*/
  	u32 NPC::GetData(u16 ID, int count) {
		u32 point = *(u32 *)Address(0x95D3F4).addr;
		if(point == 0) {
			return 0;
		}

		u32 res = 0;
		u16 val = 0;
		u32 data = *(u32 *)(point + 0xD00C);
		int cvar = 0;

		while(res = data, res != 0) {
			data = *(u32 *)(res + 4);
			if(*(u8 *)(*(u32 *)(res + 8) + 0xF) == 0) {
				val = *(u16 *)(*(u32 *)(res + 8) + 4);
				if((val < 0x400) || (0x1FF < (val & 0x3FF))) {
					val = val & 0x3FF;
				}
				else {
					val = (val & 0x3FF) + 0x200;
				}

				if((*(u8 *)(val + point + 0x1102C) & 2) == 0) {
					if(*(u16 *)(res + 0xC) == ID) {
						cvar++;
						if(count == cvar || count == -1) {
							return *(u32 *)(res + 8);
						}
					}
				}
			}
    	}

		return 0;
	}

	ACNL_VillagerData *NPC::GetSaveData() {
		u32 *addr = (u32 *)(Address(0x2FB344).Call<u32>() + 0x292A0);
		return (ACNL_VillagerData *)addr;
	}

	std::string NPC::GetNName(u16 VID) {
		static Address SetUp(0x308210);
		static const Address NNPCModelData(0xA84AF0);

		u32 Stack[44];
		u32 add = Address(0x81F9D0).Call<u32>(Stack);

		u32 npcModel = *(u32 *)(NNPCModelData.addr) + VID * 0x22;
		SetUp.Call<void>(*(u32 *)Address(0x95EEDC).addr, add, (char *)"STR_NNpc_name", npcModel + 10);

		std::string NNPCName = "";
		Process::ReadString(Stack[1], NNPCName, 0x20, StringFormat::Utf16);
		return NNPCName.empty() ? "???" : NNPCName;
	}

	std::string NPC::GetSPName(u8 SPVID) {
		static Address SetUp(0x75D108);

		u32 Stack[44];
		u32 add = Address(0x81F9D0).Call<u32>(Stack);

		SetUp.Call<void>(*(u32 *)Address(0x95EEDC).addr, add, (char *)"STR_SPNpc_name", SPVID);

		std::string SPNPCName = "";
		Process::ReadString(Stack[1], SPNPCName, 0x20, StringFormat::Utf16);
		return SPNPCName.empty() ? "???" : SPNPCName;
	}

	std::string NPC::GetRace(u8 ID) {
		static Address SetUpStack(0x3081E8);
		static Address SetUp(0x312610);

	//No clue why, but the USA and EUR version have some formatting string parts at the beginning of the NPC race string which we need to skip
		static const Address Fix(0xC);

		u32 Stack[44];
		u32 add = SetUpStack.Call<u32>(Stack, Stack + 0x18, 0x10);

		/*
		//Way to get race ID by VID
		u32 npcModel = *(u32 *)(NNPCModelData) + VID * 0x22;
		*(u8 *)(npcModel + 2);
		*/

		SetUp.Call<void>(*(u32 *)Address(0x95EEDC).addr, add, (char *)"STR_Race", ID);

		std::string NPCRace = "";

		Process::ReadString(Stack[1] + Fix.addr, NPCRace, 0x20, StringFormat::Utf16);

		return NPCRace.empty() ? "???" : NPCRace;
	}

	bool NPC::PopulateAmiibo(SpecieID specieID, std::vector<std::string> &vec, std::vector<PACKED_AmiiboInfo> &info, bool HoldenFillyAllowed, bool NonCaravanAllowed) {
		if(specieID == SpecieID::Special) {
			vec.clear();
			info.clear();

			for(const SPAmiiboInfo& amiibo : amiiboSPVillagers) {
				if(!NonCaravanAllowed && amiibo.VID == 0xFFFF) {//Non Caravan get skipped
					continue;
				}

				std::string Name = "";
				if(amiibo.SPVID == 0xFE) {
					Name = "Villager";
				}
				else if(amiibo.SPVID == 0xFF) {
					Name = "Timmy&Tommy";
				}
				else {
					Name = GetSPName(amiibo.SPVID);
				}

				info.push_back(PACKED_AmiiboInfo{Name, amiibo.ID0, amiibo.ID1, amiibo.VID});
				vec.push_back(Name);
			}

			return true;
		}

		else if(specieID >= (SpecieID)0) {
			vec.clear();
			info.clear();

			for(const AmiiboInfo& amiibo : amiiboVillagers) {
				if(!HoldenFillyAllowed && amiibo.ID0 == 0) {//Holden and Filly get skipped
					continue;
				}

				if(amiibo.Species == specieID) {
					std::string Name = GetNName(amiibo.VID);
					info.push_back(PACKED_AmiiboInfo{Name, amiibo.ID0, amiibo.ID1, amiibo.VID});
					vec.push_back(Name);
				}
			}
			return true;
		}
		return false;
	}

	void NPC::PopulateRace(std::vector<std::string> &vec) {
		vec.clear();
	//0 - 34
		for(int i = 0; i < 35; ++i) {
			vec.push_back(GetRace(i));
		}
		vec.push_back("special characters");
	}

 	u16 NPC::GetVID(u32 npcData) {
		static Address func1(0x51D288);

		return func1.Call<u16>(npcData);
	}

//0x01 to 0x34
	u8 NPC::GetSPVID(u32 npcData) {
		u32 var = *(u32 *)(npcData + 0x660);
		var = *(u8 *)(var + 0x260);
		return var;
	}

	u8 NPC::IsPlayer(u32 npcData) {
		for(int i = 0; i < 8; ++i) {
    		if(*(u32 *)(npcData + 0x9AC) == Player::GetSpecificSave(i)) {
				return i;
			}
		}
		return -1;
	}

	u32 NPC::GetPlayerSave(u32 npcData) {
		return *(u32 *)(npcData + 0x9AC);
	}
  
  	void NPC::GetLoadedSPNPC(std::vector<NPCdata> &vec) {
		vec.clear();
		u32 data = 0;
		u8 SPVID = 0;
	//SPNPC
		for(u16 i = 0x0196; i < 0x1FC; ++i) {
			data = GetData(i);
			if(data != 0) {
				SPVID = GetSPVID(data);
				vec.push_back(NPCdata{ GetSPName(SPVID), data });
			}
		}

	//Special Case (Boat SPNPC (Kappn))	
		data = GetData(0x18C);
		if(data != 0) {
			vec.push_back(NPCdata{ GetSPName(0x15), data }); //0x15 is Kappn's SPVID (it isn't in the RAM when he is loaded there)
		}

	//Special Case (Tour Tortimer (specifically at the tour results))	
		data = GetData(0x18B);
		if(data != 0) {
			vec.push_back(NPCdata{ GetSPName(0x41), data }); //0x41 is Tortimers's SPVID (it isn't in the RAM when he is loaded there)
		}
	}

	void NPC::GetLoadedNNPC(std::vector<NPCdata> &vec) {
		vec.clear();
		u32 data = 0;
		u16 VID = 0;
	//NNPC
		for(int i = 0; i < 0xA; ++i) {
			data = GetData(0x191 + !RuntimeContext::getInstance()->isIndoors(), i);
			if(data != 0) {
				VID = GetVID(data);
				vec.push_back(NPCdata{ GetNName(VID), data });
			}
		}

	//Special Case (Mainstreet NNPC)
		for(int i = 0; i < 0xA; ++i) {
			data = GetData(0x194, i);
			if(data != 0) {
				VID = GetVID(data);
				vec.push_back(NPCdata{ GetNName(VID), data });
			}
		}

	//Special Case (Tour NNPC (specifically the Hide and Seek Tour))
		for(int i = 0; i < 5; ++i) {
			data = GetData(0x1FC, i);
			if(data != 0) {
				VID = GetVID(data);
				vec.push_back(NPCdata{ GetNName(VID), data });
			}
		}
	}

	void NPC::GetLoadedPNPC(std::vector<NPCdata> &vec) {
		vec.clear();
		std::string str = "";
		u32 data = 0;
		u32 save = 0;
	//PNPC
		for(int i = 0; i < 8; ++i) {
			data = GetData(0x193, i);
			if(data != 0) {
				save = GetPlayerSave(data);
				str = "";
				Process::ReadString(save + 0x55A8, str, 16, StringFormat::Utf16); 
				vec.push_back(NPCdata{ str, data });
			}
		}

	//Special Case (Tour PNPC (specifically at the tour results))
		for(int i = 0; i < 5; ++i) {
			data = GetData(0x189, i);
			if(data != 0) {
				save = GetPlayerSave(data);
				str = "";
				Process::ReadString(save + 0x55A8, str, 16, StringFormat::Utf16); 
				vec.push_back(NPCdata{ str, data });
			}
		}

	//Special Case (Boat PNPC (When going to the island/leaving island))
		for(int i = 0; i < 5; ++i) {
			data = GetData(0x18D, i);
			if(data != 0) {
				save = GetPlayerSave(data);
				str = "";
				Process::ReadString(save + 0x55A8, str, 16, StringFormat::Utf16); 
				vec.push_back(NPCdata{ str, data });
			}
		}
	}
}
