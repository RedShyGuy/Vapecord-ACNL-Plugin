#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <CTRPluginFramework.hpp>

#define IsItemSlot(X)	(X >= 0x00 && X <= 0x0F)
#define IsMailSlot(X)	(X >= 0x14 && X <= 0x1D)

namespace CTRPluginFramework {
	struct Item {
		std::vector<std::string> Name;
		std::vector<u16> ID;
	};
	
	extern Item* ItemList;
	extern int ItemFileLenght;
	extern bool ItemFileExists;

	using slotReader = void(*)(u32, u8);

	namespace Inventory {
		u32 		GetCurrentItemData(int i = 0);
		u32 		GetNextItem(u16 itemID, u8 &slot);
		u32 		GetNextClosetItem(u32 itemID, u8 &slot);
		bool 		GetSelectedSlot(u8& slot);
		bool 		GetHoveredSlot(u8& slot);
		u16			GetAddData();
		bool		Opened();
		u8			GetCurrent();
		void		WriteLock(int slot, u8 lock = 0);
		u8			GetLock(int slot);

		bool 		WriteSlot(int slot, u32 item, u8 lock = 0);
		bool 		ReadSlot(int slot, u32& item, u8 inv = 0);
		void		ReloadIcons(void);
	}

	void ReserveItemData(Item* out);
	int ItemSearch(const std::string& match, Item& out);
	std::string ItemIDSearch(u16 ItemID);
}
#endif
