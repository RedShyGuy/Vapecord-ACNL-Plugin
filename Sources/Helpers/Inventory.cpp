#include "Helpers/Inventory.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/IDList.hpp"
#include "Address/Address.hpp"
#include "Files.h"

namespace CTRPluginFramework {
	u32 Inventory::GetCurrentItemData(int i) {
		if(Game::BaseInvPointer() == 0) {
			return -1;
		}
		
		if(!Opened()) {
			return -1;
		}
			
		if(GetCurrent() != 0) {
			return -1;
		}
			
		u32 Items = *(u32 *)(*(u32 *)(Game::BaseInvPointer() + 0xC) + 0xEC); //0x20 32DCEC10
		
		return (Items + (0xAC * i));
	}

//Get current inventory ID
	u8 Inventory::GetCurrent() {
		if(Game::BaseInvPointer() == 0) {
			return 0xFF;
		}
		
		return *(u8 *)(*(u32 *)(Game::BaseInvPointer() + 0xC) + 0x24);
	}

//get correct inv addition data
	u16 Inventory::GetAddData() {
		if(Game::BaseInvPointer() == 0) {
			return -1;
		}
		
		switch(GetCurrent()) {
			//Base Inventory
			case 0: 
				return 0x1EC0;
			
			//Island Box Inventory
			case 0x3E:
			case 0x3F:
				return 0x3CF4;
			
			//Item borrow Inventory
			case 0x40:
				return 0x27D4;
			
			//Closet Inventory
			case 0x3D:
			case 0x89:
				return 0x50;
			
			//hopefully all other inventory
			default:
				return 0x60;
		}
	}
//if inv is opened
	bool Inventory::Opened() {
		if(Game::BaseInvPointer() == 0) {
			return 0;
		}
		
		return *(u8 *)(*(u32 *)(Game::BaseInvPointer() + 0xC) + (0x8419 + GetAddData())) == 1;
	}
//Write Inventory Slot
	bool Inventory::WriteSlot(int slot, Item item, u8 lock) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return false;
		}
			
		if(!item.isValid(false)) {
			return false;
		}
		
	//Writes item and fixes lock if needed
		player->Inventory[slot] = item;
		player->InventoryItemLocks[slot] = lock;
		
		ReloadIcons();

		return true;
	}
//Read Inventory Slot	
	bool Inventory::ReadSlot(int slot, Item& item) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return false;
		}

		item = player->Inventory[slot];
		return true;
	}
//Get asked item
	bool Inventory::GetNextItem(Item itemID, u8 &slot, bool ignoreFlag) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return false;
		}
		
		slot = 0;
		while(true) {
			if (ignoreFlag) {
				if (itemID.ID == player->Inventory[slot].ID) {
					return true; // Item found
				}
			} else {
				if (itemID == player->Inventory[slot]) {
					return true; // Item found
				}
			}

			slot++; //goto next slot
			
			if (15 < slot) {//If item not found return
				return false;	
			}	
		}
	}
//Get asked closet item	
	bool Inventory::GetNextClosetItem(Item itemID, u8 &slot) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return false;
		}
		
		slot = 0;
		while(true) {
			if(itemID == player->Dressers[slot]) {//If item found return  
				return true;
			}
			
			slot++; //goto next slot
			
			if(179 < slot) {//If item not found return
				return -1;
			}		
		}
	}

	void Inventory::ReloadIcons() {
	//if inv is not loaded return
		if(Game::BaseInvPointer() == 0) {
			return;
		}
		
	//If inv is not opened return
		if(!Opened()) {
			return;
		}

		for(int i = 0; i < 16; ++i) {
			Address(0x26DC00).Call<void>(*(u32 *)(Game::BaseInvPointer() + 0xC) + GetAddData(), i);
		}
	}

//get current selected inventory slot
	bool Inventory::GetSelectedSlot(u8& slot) {
		if(!Opened()) {
			return false;
		}
		
		u32 offs = *(u32 *)(Game::BaseInvPointer() + 0xC);
		offs += 0xCC;
		slot = *(u8 *)offs;

		if(slot != -1 && slot < 0x10) {
			return true;
		}
		
		return false;
	}

//get hovered inv slot
	bool Inventory::GetHoveredSlot(u8& slot) {
		if(!Opened()) {
			return false;
		}
		
		u32 offs = *(u32 *)(Game::BaseInvPointer() + 0xC);
		offs += 0xD4;
		slot = *(u8 *)offs;

		if(slot != -1 && slot < 0x10) {
			return true;
		}
		
		return false;
	}
}