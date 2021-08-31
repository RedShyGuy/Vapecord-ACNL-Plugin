#ifndef NPC_HPP
#define NPC_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {

  struct NPCdata {
      std::string name;
      u32 data;
  };
  
  namespace NPC {
    u32 GetData(u16 ID, int count = -1);
    u8 GetSPVID(u32 npcData);
    u16 GetVID(u32 npcData);
    s8 IsPlayer(u32 npcData);
    u32 GetPlayerSave(u32 npcData);
    
    void GetLoadedSPNPC(std::vector<NPCdata> &vec);
    void GetLoadedNNPC(std::vector<NPCdata> &vec);
    void GetLoadedPNPC(std::vector<NPCdata> &vec);
  }
}
#endif
