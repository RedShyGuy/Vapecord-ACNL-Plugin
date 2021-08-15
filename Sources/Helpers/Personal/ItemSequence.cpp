#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
	std::vector<u32> ItemID;
	std::vector<std::string> KBItems;
	u8 Index;
	bool ISActive = false;
	
	void ItemSequence::Init() {
		ItemID.clear();
		ItemID.push_back(dropitem);
	}
	
	void ItemSequence::OpenIS() {
		u32 AddedItem;
		
		Keyboard AddKB(Language->Get("ITEM_SEQUENCER_ENTER_ID"));
		AddKB.CanAbort(true);
		AddKB.IsHexadecimal(true);
		AddKB.SetMaxLength(8);
		AddKB.DisplayTopScreen = true;
		
	//Clears and adds all items to vector
		KBItems.clear();
		KBItems.push_back("Standard Drop Item");
	//if item vector is not empty add existing items to keyboard
		if(ItemID.size() != 0) {
			for(u8 i = 1; i < ItemID.size(); i++) 
				KBItems.push_back(Utils::Format("%08X", ItemID.at(i)));
		}
		
		KBItems.push_back("Add...");
		
		Keyboard OptKB(Language->Get("ITEM_SEQUENCER_CHOOSE"));
		OptKB.CanAbort(true);
		OptKB.Populate(KBItems);
		int Select;
		Select = OptKB.Open();
		
		switch(Select) {
			case -1:
				return;
				
			case 0:
				Sleep(Milliseconds(200));
				AddKB.Open(dropitem, dropitem);
			break;
			
			default:
				Sleep(Milliseconds(200));
			//If new item gets added
				if(Select + 1 == KBItems.size()) {
				//Adds new item
					if(AddKB.Open(AddedItem) == 0) 
						ItemID.push_back(AddedItem);
				}
				else {
				//if already existing item is selected 
					if(AddKB.Open(ItemID.at(Select), ItemID.at(Select)) == -1) {
					//If Item is not selected remove position
						ItemID.erase(ItemID.begin() + Select);
					}
				}
			break;
		}
		Sleep(Milliseconds(200));
		OpenIS();
	}
//if item sequence is enabled
	bool ItemSequence::Enabled() {
		return ItemID.size() != 0 && ISActive;
	}
//itemsequence on/off	
	void ItemSequence::Switch(bool enable) {
		ISActive = enable;
	}
//moves to next item
	u32 *ItemSequence::Next() {
		if(ItemID.size() - 1 > Index) 
			Index++;
		else 
			Index = 0;
		
		if(Index == 0) 
			return &dropitem;
		
		return &ItemID.at(Index);
	}
//Look at the next item	
	u32 ItemSequence::PeekNext() {
		if(ItemID.size() - 1 > Index) 
			Index++;
		else 
			Index = 0;
		
		if(Index == 0) 
			return dropitem;
		
		return ItemID.at(Index);
	}
}