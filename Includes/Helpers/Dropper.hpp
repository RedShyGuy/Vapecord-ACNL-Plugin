#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
	struct s_DropType {
		u8 dropID;
		u8 dropAnim;
	};

	struct TramplePkt {
		Item item;
		u8 roomID;
		u8 wX;
		u8 wY;
		u8 u0;
	};

	extern const s_DropType DropTypes[8];

	extern bool MapEditorActive;
	extern bool DropPatternON;
	extern bool bypassing;
	extern u8 DropType;
	extern Item ItemIDToReplace;
	extern Item dropitem;
	extern u32 selectedX; 
	extern u32 selectedY; 
	extern Item itemslotid;
	extern u8 waitAnim;

	extern const Item ReplaceEverything;

	namespace Dropper {
		void				RestorePattern(void);
		u32					PlaceItem(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy, bool isInside, bool u1, bool u2, bool u3, bool u4);
		void 				DropItemLock(bool p_switch);
		bool				DropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1);
		bool				PlaceItemWrapper(u8 ID, Item ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID, bool itemsequenceallowed = true);	
		int 				Search_Replace(int ItemsPerRun, std::vector<Item> ItemToSearch, Item ItemToPlace, u8 AnimID, bool ItemSequenceUsage, const std::string& msg = "", bool DisplayMSG = false);
	}
}