#ifndef DROPPER_HPP
#define DROPPER_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	struct s_DropType {
		u8 dropID;
		u8 dropAnim;
	};

	struct TramplePkt {
		u32 item;
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
	extern u32 ItemIDToReplace;
	extern u32 dropitem;
	extern u32 selectedX; 
	extern u32 selectedY; 
	extern u32 itemslotid;
	extern u8 waitAnim;

	namespace Dropper {
		void				RestorePattern(void);
		u32					PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool isInside, bool u1, bool u2, bool u3, bool u4);
		void 				DropItemLock(bool p_switch);
		bool				DropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1);
		bool				PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID, bool itemsequenceallowed = true);	
		int 				Search_Replace(int ItemsPerRun, std::vector<u32> ItemToSearch, u32 ItemToPlace, u8 AnimID, bool ItemSequenceUsage, const std::string& msg = "", bool DisplayMSG = false);
	}
}
#endif