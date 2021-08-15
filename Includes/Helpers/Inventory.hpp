#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	class Inventory {
	public:
		static u32 			GetCurrentItemData(int i = 0);
		static u32 			GetNextItem(u16 itemID, u8 &slot);
		static u32 			GetNextClosetItem(u32 itemID, u8 &slot);
		static s8  			GetSelectedSlot();
		static s8 			GetHoveredSlot();
		static u16			GetAddData();
		static bool			Opened();
		static u8			GetCurrent();
		static void			WriteLock(int slot, u8 lock = 0);
		static u8			GetLock(int slot);
		static void 		WriteSlot(int slot, u32 item, u8 lock = 0);
		static u32 			ReadSlot(int slot, u8 inv = 0);
		static void			ReloadIcons(void);
	};
}
#endif
