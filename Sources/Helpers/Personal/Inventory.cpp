#include "cheats.hpp"

namespace CTRPluginFramework {
	u32 Inventory::GetCurrentItemData(int i) {
		if(GameHelper::BaseInvPointer() == 0)
			return -1;
		
		if(!Opened())
			return -1;
			
		if(GetCurrent() != 0)
			return -1;
			
		u32 Items = *(u32 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xEC); //0x20 32DCEC10
		
		return (Items + (0xAC * i));
	}
//Get inv lock
	u8 Inventory::GetLock(int slot) {
		if(Player::GetSaveOffset(4) == 0)
			return -1;
		
		return *(u8 *)(PlayerPTR::Pointer(0x6C10) + slot);
	}
//Write Inv Lock
	void Inventory::WriteLock(int slot, u8 lock) {
		if(Player::GetSaveOffset(4) == 0)
			return;
		
		PlayerPTR::Write8(0x6C10 + slot, lock);
	}
//Get current inventory ID
	u8 Inventory::GetCurrent() {
		if(GameHelper::BaseInvPointer() == 0) 
			return 0xFF;
		
		return *(u8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x24);
	}
//get current selected inventory slot
	u8 Inventory::GetSelectedSlot() {
		if(GameHelper::BaseInvPointer() == 0) 
			return -1;
		
		u8 slot = *(u8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xCC);
		if(slot != -1 && slot < 0x10) 
			return slot;
		
		return -1;
	}
//get hovered inv slot
	u8 Inventory::GetHoveredSlot() {
		if(GameHelper::BaseInvPointer() == 0) 
			return -1;
		
		u8 slot = *(u8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xD4);
		if(slot != -1 && slot < 0x10) 
			return slot;
		
		return -1;
	}
//get correct inv addition data
	u16 Inventory::GetAddData() {
		if(GameHelper::BaseInvPointer() == 0) 
			return -1;
		
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
		if(GameHelper::BaseInvPointer() == 0) 
			return 0;
		
		return *(u8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + (0x8419 + GetAddData())) == 1;
	}
//Write Inventory Slot
	bool Inventory::WriteSlot(int slot, u32 item, u8 lock) {
		if(Player::GetSaveOffset(4) == 0)
			return false;
			
		if(!IDList::ItemValid(item, false)) 
			return false;
		
	//Writes item and fixes lock if needed
		PlayerPTR::Write32(0x6BD0 + (0x4 * slot), item);
		WriteLock(slot, lock);
		
		ReloadIcons();

		return true;
	}
//Read Inventory Slot	
	bool Inventory::ReadSlot(int slot, u32& item, u8 inv) {
		if(Player::GetSaveOffset(4) == 0) 
			return false;
			
		switch(inv) {
			case 0: { //If Standard Inventory
				item = *(u32 *)PlayerPTR::Pointer(0x6BD0 + (0x4 * slot));
			} return true;
				
			case 0x3D: { //If Closet
				item = *(u32 *)PlayerPTR::Pointer(0x92F0 + (0x4 * slot));
			} return true;
				
			case 0x89: { //If Secret Closet
				item = *(u32 *)PlayerPTR::Pointer(0x7A6D8 + (0x4 * slot));
			} return true;
		}	
		return false;
	}
//Get asked item
	u32 Inventory::GetNextItem(u16 itemID, u8 &slot) {
		if(Player::GetSaveOffset(4) == 0) 
			return -1;
		
		slot = 0;
		while(true) {
			u32 ItemOffset = PlayerPTR::Pointer(0x6BD0 + (0x4 * slot));
			if(itemID == *(u32 *)ItemOffset) //If item found return offset 
				return ItemOffset;
			
			slot++; //goto next slot
			
			if(15 < slot) //If item not found return
				return -1;			
		}
	}
//Get asked closet item	
	u32 Inventory::GetNextClosetItem(u32 itemID, u8 &slot) {
		if(Player::GetSaveOffset(4) == 0) 
			return -1;
		
		slot = 0;
		while(true) {
			u32 ItemOffset = PlayerPTR::Pointer(0x92F0 + (4 * slot));
			if(itemID == *(u32 *)ItemOffset) //If item found return offset 
				return ItemOffset;
			
			slot++; //goto next slot
			
			if(179 < slot) //If item not found return
				return -1;			
		}
	}

	void Inventory::ReloadIcons() {
	//if inv is not loaded return
		if(GameHelper::BaseInvPointer() == 0) 
			return;
		
	//If inv is not opened return
		if(!Opened())
			return;

		for(int i = 0; i < 16; ++i)
			Code::LoadIcon.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + GetAddData(), i);
	}
}