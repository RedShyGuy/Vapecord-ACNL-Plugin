#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
	u32 Inventory::GetCurrentItemData(int i) {
		if(GameHelper::BaseInvPointer() == 0)
			return 0xFFFFFFFF;
		
		if(!Opened())
			return 0xFFFFFFFF;
			
		if(GetCurrent() != 0)
			return 0xFFFFFFFF;
			
		u32 Items = *(u32 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xEC); //0x20 32DCEC10
		
		return (Items + (0xAC * i));
	}
//Get inv lock
	u8 Inventory::GetLock(int slot) {
		if(Player::GetSaveOffset(4) == 0)
			return 0xFF;
		
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
	s8 Inventory::GetSelectedSlot() {
		if(GameHelper::BaseInvPointer() == 0) 
			return -1;
		
		s8 slot = *(s8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xCC);
		if(slot != -1 && slot < 0x10) 
			return slot;
		
		return -1;
	}
//get hovered inv slot
	s8 Inventory::GetHoveredSlot() {
		if(GameHelper::BaseInvPointer() == 0) 
			return -1;
		
		s8 slot = *(s8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xD4);
		if(slot != -1 && slot < 0x10) 
			return slot;
		
		return -1;
	}
//get correct inv addition data
	u16 Inventory::GetAddData() {
		if(GameHelper::BaseInvPointer() == 0) 
			return 0xFFFF;
		
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
	void Inventory::WriteSlot(int slot, u32 item, u8 lock) {
		if(Player::GetSaveOffset(4) == 0)
			return;
			
		if(!IDList::ItemValid(item, false)) 
			return;
		
	//Writes item and fixes lock if needed
		PlayerPTR::Write32(0x6BD0 + (0x4 * slot), item);
		WriteLock(slot, lock);
		
		ReloadIcons();
	}
//Read Inventory Slot	
	u32 Inventory::ReadSlot(int slot, u8 inv) {
		if(Player::GetSaveOffset(4) == 0) 
			return 0xFFFFFFFF;
			
		switch(inv) {
			case 0: //If Standard Inventory
				return *(u32 *)PlayerPTR::Pointer(0x6BD0 + (0x4 * slot));
				
			case 0x3D: //If Closet
				return *(u32 *)PlayerPTR::Pointer(0x92F0 + (0x4 * slot));
				
			case 0x89: //If Secret Closet
				return *(u32 *)PlayerPTR::Pointer(0x7A6D8 + (0x4 * slot));
		}	
		return 0xFFFFFFFF;
	}
//Get asked item
	u32 Inventory::GetNextItem(u16 itemID, u8 &slot) {
		if(Player::GetSaveOffset(4) == 0) 
			return 0xFFFFFFFF;
		
		slot = 0;
		while(true) {
			u32 ItemOffset = PlayerPTR::Pointer(0x6BD0 + (0x4 * slot));
			if(itemID == *(u32 *)ItemOffset) //If item found return offset 
				return ItemOffset;
			
			slot++; //goto next slot
			
			if(15 < slot) //If item not found return
				return 0xFFFFFFFF;			
		}
	}
//Get asked closet item	
	u32 Inventory::GetNextClosetItem(u32 itemID, u8 &slot) {
		if(Player::GetSaveOffset(4) == 0) 
			return 0xFFFFFFFF;
		
		slot = 0;
		while(true) {
			u32 ItemOffset = PlayerPTR::Pointer(0x92F0 + (0x4 * slot));
			if(itemID == *(u32 *)ItemOffset) //If item found return offset 
				return ItemOffset;
			
			slot++; //goto next slot
			
			if(179 < slot) //If item not found return
				return 0xFFFFFFFF;			
		}
	}

	void Inventory::ReloadIcons() {
	//if inv is not loaded return
		if(GameHelper::BaseInvPointer() == 0) 
			return;
		
	//If inv is not opened return
		if(!Opened())
			return;
		
		static FUNCT func(Code::LoadIcon);

		for(int i = 0; i < 16; ++i)
			func.Call<void>(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + GetAddData(), i);
	}
}