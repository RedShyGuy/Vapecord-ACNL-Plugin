#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

#define IsItemSlot(X)	(X >= 0x00 && X <= 0x0F)
#define IsMailSlot(X)	(X >= 0x14 && X <= 0x1D)

namespace CTRPluginFramework {
	using slotReader = void(*)(u32, u8);

	namespace Inventory {
		u32 		GetCurrentItemData(int i = 0);
		bool 		GetNextItem(Item itemID, u8 &slot, bool ignoreFlag = false);
		bool 		GetNextClosetItem(Item itemID, u8 &slot);
		bool 		GetSelectedSlot(u8& slot);
		bool 		GetHoveredSlot(u8& slot);
		u16			GetAddData();
		bool		Opened();
		u8			GetCurrent();

		bool 		WriteSlot(int slot, Item item, u8 lock = 0);
		bool 		ReadSlot(int slot, Item& item);
		void		ReloadIcons(void);
	}
}
#endif
