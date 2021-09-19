#include "cheats.hpp"

namespace CTRPluginFramework {
	const s_DropType DropTypes[8] = {
		{ 1, 0x3D }, //Pick
		{ 2, 0x40 }, //Pull
		{ 0xA, 0x56 }, //Drop
		{ 0xB, 0x5A }, //Bury (Outdoors Only)
		{ 0xC, 0x5C }, //Plant
		{ 0xD, 0x5D }, //Display Pattern
		{ 0x12, 0x6B }, //Smash Rock
		{ 0x13, 0x73 }, //Dig (Outdoors Only)
	};

	int Dropper::Search_Replace(int ItemsPerRun, std::vector<u32> ItemToSearch, u32 ItemToPlace, u8 AnimID, bool ItemSequenceUsage, const std::string& msg, bool DisplayMSG) {
		if(!PlayerClass::GetInstance()->IsLoaded()) 
			return -1; //Player not loaded

		if(IsIndoorsBool)
			return -2; //Only works outdoors

		bool ItemSequenceWasON = false;

		if(!ItemSequenceUsage) {
			ItemSequenceWasON = true;
			ItemSequence::Switch(false);
		}

		if(!bypassing) 
			GameHelper::DropItemLock(true);

		u32 count = 0;
		u32 x = 0x10, y = 0x10;

		bool res = true;
		while(res) {
			while(res) {
				if((u32)GameHelper::GetItemAtWorldCoords(x, y) != 0) {
					for(int i = 0; i < ItemToSearch.size(); i++) {
						if(Dropper::PlaceItemWrapper(1, ItemToSearch[i], &ItemToPlace, &ItemToPlace, x, y, 0, 0, 0, 0, 0, AnimID, 0xA5, ItemSequenceUsage)) {
							count++;
							if(count % ItemsPerRun == 0) 
								Sleep(Milliseconds(500));
						}
					}
				}
				else 
					res = false;

				y++;
			}
			res = true;
			
			y = 0x10;
			x++;
			if((u32)GameHelper::GetItemAtWorldCoords(x, y) == 0) 
				res = false;
		}		
		
		if(DisplayMSG)
			OSD::Notify(Utils::Format("%d %s", count, msg.c_str()));

	//OFF
		if(!bypassing) 
			GameHelper::DropItemLock(false);	

		if(ItemSequenceWasON)
			ItemSequence::Switch(true);

		return 0;
	}
//Place Item	
	u32 Dropper::PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4) {
		return Code::PlaceItemOffset.Call<u32>(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow, worldx, worldy, u0, u1, u2, u3, u4);
	}
//Drop Check	
	bool Dropper::DropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1) {
		static Address dropcheck(0x5990AC, 0x5985C4, 0x5980F4, 0x5980F4, 0x5979E4, 0x5979E4, 0x5976B8, 0x5976B8);
		return dropcheck.Call<bool>((u32)wX, (u32)wY, (u32)u0, disallowConcrete, u1);
	}
//Drop Item Wrapper	
	bool Dropper::PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID, bool itemsequenceallowed) {
		if(!PlayerClass::GetInstance()->IsLoaded()) 
			return 0;

		if(!IDList::ItemValid((*ItemToPlace & 0xFFFFFFFF))) {
			OSD::Notify(Utils::Format("Invalid Item: %08X", *ItemToPlace));
			return 0;
		}

	//sets waitanim as autowaitanim
		u8 autoWaitAnim = waitAnim;
		
		if(ID == 0xB || ID == 0x13) {
			if(IsIndoorsBool) {
				ID = 0xA;
				autoWaitAnim = 0x53;
			}
		}

	//If item sequence is enabled use next item if not use standard ItemToPlace
		u32 *actualItemToPlace = (ItemSequence::Enabled() && itemsequenceallowed) ? ItemSequence::Next() : ItemToPlace;
		
	//If item sequence is enabled use actualIteToPlace as the item to show, if not use standard ItemToShow
		u32 *actualItemToShow = (ItemSequence::Enabled() && itemsequenceallowed) ? actualItemToPlace : ItemToShow;

	//gets current player index -> player you selected
		u8 currentIndex = GameHelper::GetOnlinePlayerIndex();
		
	//checks if the drop is forced on someone else
		bool forced = (currentIndex != GameHelper::GetActualPlayerIndex()) && (GameHelper::GetActualPlayerIndex() <= 3);
		
	//not too sure when it is not 0xFFFFFFFF but if it is return
		if(GameHelper::GetLockedSpotIndex(worldx, worldy, roomID) != 0xFFFFFFFF) 
			return 0;
		
	//checks if item at coords is not 0 -> if place is valid to drop
		u32 *pItemAtCoords = GameHelper::GetItemAtWorldCoords(worldx, worldy);
		if(pItemAtCoords == nullptr) 
			return 0;

		u32 empty = 0x7FFE;

		u32 *actualItemAtCoords = (*pItemAtCoords & 0x7FFE) == 0x7FFE ? &empty : pItemAtCoords;

	//writes the item to replace, if 0xFFFFFFFF is selected as item it will replace item at coords, if not use standard ItemToReplace
		u32 *actualItemToReplace = ItemToReplace == 0xFFFFFFFF ? actualItemAtCoords : &ItemToReplace;

	//if item to replace is not item at coords return -> means there wont be an item to replace
		if(*actualItemToReplace != *actualItemAtCoords) 
			return 0;

		u32 crashPreventItem = IsIndoorsBool ? 0x2001 : 0x80007FFE;

		u32 *ItemReplace = (*actualItemToReplace & 0x7FFE) == 0x7FFE ? &crashPreventItem : actualItemToReplace;
		u32 *ItemPlace = (*actualItemToPlace & 0x7FFE) == 0x7FFE ? &crashPreventItem : actualItemToPlace;
		u32 *ItemShow = (*actualItemToShow & 0x7FFE) == 0x7FFE ? &crashPreventItem : actualItemToShow;

	//place item function
		Dropper::PlaceItem(ID, ItemReplace, ItemPlace, ItemShow, worldx, worldy, u0, u1, 0, u3, u4);

		if(!(ID >= 0xA && ID <= 0xD)) {
			bool noWait = false;
			
		//gets player data
			u32 player = PlayerClass::GetInstance()->Offset();
			
		//gets animation data
			u32 animInstance = Animation::GetAnimationInstance(player, 0, 0, 0);

			AnimData data;
			data.Init(animInstance, player, currentIndex);

			Process::Write32(player + 0x844, 0);
			Process::Write8(player + 0x8CC, ID);

		//animation patch
			static const Address animpatch(0x682434, 0x68195C, 0x68146C, 0x68146C, 0x680F2C, 0x680F2C, 0x680AD4, 0x680AD4);

		//display pattern, smash rock, bury
			if(waitAnim == 0x5D || waitAnim == 0x6B || waitAnim == 0x5A)
				noWait = true;
			
		//if pick or pluck
			if(waitAnim == 0x3D || waitAnim == 0x40) {
				noWait = true;
				goto noWaitPick;
			}
			
		//if pick or pluck
			if(ID >= 1 && ID <= 3 && !noWait) {
				if(forced) {
					switch(ID) {
						case 1:
						case 2:
							autoWaitAnim = 0x3D;
						break;
					}
				}
				else 
					autoWaitAnim = 0x3C;
				
				noWaitPick:
			//write coords and item and ID to animation
				data.Pick_3C_3D(ID, *(u32 *)actualItemToReplace, Coord{ worldx, worldy });
			}
			
			else {
			//write coords and item to animation
				data.AppendAnimData<u8>(animInstance, 0xE, worldx);
				data.AppendAnimData<u8>(animInstance, 0xF, worldy);
				data.AppendAnimData<u16>(animInstance, 0x10, *(u16 *)actualItemToReplace == 0x7FFE ? 0x2001 : *(u16 *)actualItemToReplace);
				
			//If forced an noWait is false
				if(forced && !noWait) 
					autoWaitAnim = 0x4C;
				
			//if bury animation
				if((ID == 0x13 || autoWaitAnim == 0x4A) && !noWait && !forced) {
					autoWaitAnim = 0x4A;
					Process::Patch(animpatch.addr, 0xE3A00000);
					data.AppendAnimData<u16>(animInstance, 0x10, ID);
					Sleep(Milliseconds(5));
				}
			//if forced with noWait true
				else if(forced) 
					autoWaitAnim = 0x4F;
			}
			
		//if selected index is the same as your player
			if(currentIndex == GameHelper::GetActualPlayerIndex()) 
				data.ExecuteAnimation(autoWaitAnim);
		//if not aka another player
			else 
				Animation::SendAnimPacket(GameHelper::GetActualPlayerIndex(), animInstance, autoWaitAnim, roomID == 0xA5 ? Player::GetRoom(currentIndex) : roomID, currentIndex);	
			
		//sleep if noWait is turned off
			if(!noWait) {
				Sleep(Milliseconds(40));
				Process::Patch(animpatch.addr, 0xE3700001);
			}
		}
		return true;
	}
}