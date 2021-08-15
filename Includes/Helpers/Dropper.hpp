#ifndef DROPPER_HPP
#define DROPPER_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	struct s_DropType {
		u8 dropID;
		u8 dropAnim;
	};

	extern const s_DropType DropTypes[8];

	namespace Dropper {
		u32					PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool isInside, bool u1, bool u2, bool u3, bool u4);
		bool				DropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1);
		bool				PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID, bool itemsequenceallowed = true);	
		int 				Search_Replace(int ItemsPerRun, std::vector<u32> ItemToSearch, u32 ItemToPlace, u8 AnimID, bool ItemSequenceUsage, const std::string& msg = "", bool DisplayMSG = false);
	}
}
#endif