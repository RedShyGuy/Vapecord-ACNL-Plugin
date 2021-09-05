#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
/*
Gets npc data for anim mods, coord mods, etc 0xB6F9B4
*/
  	u32 NPC::GetData(u16 ID, int count) {
		static const u32 point1 = Region::AutoRegion(0x95D3F4, 0x95C3D4, 0x95C3EC, 0x95C3EC, 0x9563EC, 0x9553EC, 0x9553EC, 0x9553EC);
		u32 point = *(u32 *)point1;
		if(point == 0)
			return 0;

		u32 res = 0;
		u16 val = 0;
		u32 data = *(u32 *)(point + 0xD00C);
		int cvar = 0;

		while(res = data, res != 0) {
			data = *(u32 *)(res + 4);
			if(*(u8 *)(*(u32 *)(res + 8) + 0xF) == 0) {
				val = *(u16 *)(*(u32 *)(res + 8) + 4);
				if((val < 0x400) || (0x1FF < (val & 0x3FF))) 
					val = val & 0x3FF;
				else 
					val = (val & 0x3FF) + 0x200;

				if((*(u8 *)(val + point + 0x1102C) & 2) == 0) {
					if(*(u16 *)(res + 0xC) == ID) {
						cvar++;
						if(count == cvar || count == -1)
							return *(u32 *)(res + 8);
					}
				}
			}
    	}

		return 0;
	}
  
 	u16 NPC::GetVID(u32 npcData) {
		static const u32 func1 = Region::AutoRegion(0x51D288, 0x51CBDC, 0x51C2D0, 0x51C2D0, 0x51BBEC, 0x51BBEC, 0x51B580, 0x51B580);

		return FUNCTION(func1).Call<u16>(npcData);
	}

//0x01 to 0x34
	u8 NPC::GetSPVID(u32 npcData) {
		u32 var = *(u32 *)(npcData + 0x660);
		var = *(u8 *)(var + 0x260);
		return var;
	}

	u8 NPC::IsPlayer(u32 npcData) {
		for(int i = 0; i < 8; ++i) {
    		if(*(u32 *)(npcData + 0x9AC) == Player::GetSpecificSave(i))
				return i;
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

		for(u16 i = 0x0196; i < 0x200; ++i) {
			data = GetData(i);
			if(data != 0) {
				SPVID = GetSPVID(data);
				vec.push_back(NPCdata{ IDList::GetSPNPCName(SPVID), data });
			}
		}
	}

	void NPC::GetLoadedNNPC(std::vector<NPCdata> &vec) {
		vec.clear();
		u32 data = 0;
		u16 VID = 0;
	//NNPC
		for(int i = 0; i < 0xA; ++i) {
			data = GetData(0x191 + !IsIndoorsBool, i);
			if(data != 0) {
				VID = GetVID(data);
				vec.push_back(NPCdata{ IDList::GetNNPCName(VID), data });
			}
		}

	//Special Case (Mainstreet NNPC)
		for(int i = 0; i < 0xA; ++i) {
			data = GetData(0x194, i);
			if(data != 0) {
				VID = GetVID(data);
				vec.push_back(NPCdata{ IDList::GetNNPCName(VID), data });
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
	}
}
