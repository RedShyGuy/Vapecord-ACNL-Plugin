#pragma once

#include <CTRPluginFramework.hpp>
#include "IDList.hpp"
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {

  struct NPCdata {
      std::string name;
      u32 data;
  };
  
  namespace NPC {
      u32 GetData(u16 ID, int count = -1);
      u8 GetSPVID(u32 npcData);
      u16 GetVID(u32 npcData);
      u8 IsPlayer(u32 npcData);
      u32 GetPlayerSave(u32 npcData);

      ACNL_VillagerData *GetSaveData();

      std::string GetNName(u16 VID);
      std::string GetSPName(u8 SPVID);
      std::string GetRace(u8 ID);
      bool PopulateAmiibo(SpecieID specieID, std::vector<std::string> &vec, std::vector<PACKED_AmiiboInfo> &info, bool HoldenFillyAllowed = false, bool NonCaravanAllowed = true);
      void PopulateRace(std::vector<std::string> &vec);

      void GetLoadedSPNPC(std::vector<NPCdata> &vec);
      void GetLoadedNNPC(std::vector<NPCdata> &vec);
      void GetLoadedPNPC(std::vector<NPCdata> &vec);
  }
}