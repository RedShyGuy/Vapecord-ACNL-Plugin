#include "Helpers/Dropper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/ItemSequence.hpp"
#include "Helpers/Game.hpp"

#include "Helpers/IDList.hpp"
#include "Helpers/AnimData.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Player.hpp"
#include "Address/Address.hpp"
#include "RuntimeContext.hpp"

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

	bool MapEditorActive = false;
	bool DropPatternON = false;
	bool bypassing = false;
	u8 DropType = 0xA;
	Item ItemIDToReplace = { 0x7FFE, 0 };
	Item dropitem = { 0x7FFE, 0 };
	u32 selectedX = 0; 
	u32 selectedY = 0; 
	Item itemslotid = { 0x7FFE, 0 };
	u8 waitAnim = 0x56;

	const Item ReplaceEverything = Item{0xFFFF, 0xFFFF};

	constexpr u32 ReValues[77] = { 
		0x1FF0000, 0x1FF01FF, 0xFFFF0000, 0x10000FF, 0x101, 
		0, 8, 6, 7, 4, 5, 2, 3,  
		1, 0, 7, 8, 5, 6, 3, 4, 
		1, 2, 0, 5, 7, 3, 8, 1, 
		6, 2, 4, 0, 3, 5, 1, 7, 
		2, 8, 4, 6, 0, 1, 3, 2, 
		5, 4, 7, 6, 8, 0, 2, 1, 
		4, 3, 6, 5, 8, 7, 0, 4, 
		2, 6, 1, 8, 3, 7, 5, 0, 
		6, 8, 4, 7, 2, 5, 1, 3,
	};
/*
Restores Drop Pattern if drop radius changer has been used to prevent any crashes
*/
	void Dropper::RestorePattern(void) {
		for(int i = 0; i < 77; ++i) {
			Process::Write32(Address(0x85FE58).addr + (i * 4), ReValues[i]);
		}
	}

	int Dropper::Search_Replace(int ItemsPerRun, std::vector<Item> ItemToSearch, Item ItemToPlace, u8 AnimID, bool ItemSequenceUsage, const std::string& msg, bool DisplayMSG) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			return -1; //Player not loaded
		}

		if(RuntimeContext::getInstance()->isIndoors()) {
			return -2; //Only works outdoors
		}

		bool ItemSequenceWasON = false;

		if(!ItemSequenceUsage) {
			ItemSequenceWasON = true;
			ItemSequence::Switch(false);
		}

		if(!bypassing) {
			Dropper::DropItemLock(true);
		}

		u32 count = 0;
		u32 x = 0x10, y = 0x10;

		bool res = true;
		while(res) {
			while(res) {
				if(Game::GetItemAtWorldCoords(x, y)) {
					for(int i = 0; i < ItemToSearch.size(); i++) {
						if(Dropper::PlaceItemWrapper(1, ItemToSearch[i], &ItemToPlace, &ItemToPlace, x, y, 0, 0, 0, 0, 0, AnimID, 0xA5, ItemSequenceUsage)) {
							count++;
							if(count % ItemsPerRun == 0) {
								Sleep(Milliseconds(500));
							}
						}
					}
				}
				else {
					res = false;
				}

				y++;
			}
			res = true;
			
			y = 0x10;
			x++;
			if(!Game::GetItemAtWorldCoords(x, y)) {
				res = false;
			}
		}		
		
		if(DisplayMSG) {
			OSD::Notify(Utils::Format("%d %s", count, msg.c_str()));
		}

	//OFF
		if(!bypassing) {
			Dropper::DropItemLock(false);
		}	

		if(ItemSequenceWasON) {
			ItemSequence::Switch(true);
		}

		return 0;
	}

//Item Locks Switch
	void Dropper::DropItemLock(bool p_switch) {
		static const Address BypassItemLock1(0x5A11C8);
		static const Address BypassItemLock2(0x5A11CC);
		static const Address BypassItemLock3(0x5A13C8);
		static const Address BypassItemLock4(0x5A13CC);
		
		if(p_switch) {
			Process::Patch(BypassItemLock1.addr, 0xE3E00000);
			Process::Patch(BypassItemLock2.addr, 0xEA000012);
			Process::Patch(BypassItemLock3.addr, 0xE3A00000);
			Process::Patch(BypassItemLock4.addr, 0xE8BD83F0);
			return;
		}
		
		Process::Patch(BypassItemLock1.addr, 0xE1A05001);
		Process::Patch(BypassItemLock2.addr, 0x1A000001);
		Process::Patch(BypassItemLock3.addr, 0xE24DD01C);
		Process::Patch(BypassItemLock4.addr, 0xE1A07001);	
	}

//Place Item	
	u32 Dropper::PlaceItem(u8 ID, Item *ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4) {
		return Address(0x59FC7C).Call<u32>(ID, ItemToReplace, ItemToPlace, ItemToShow, worldx, worldy, u0, u1, u2, u3, u4);
	}
//Drop Check
	bool Dropper::DropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1) {
		static Address dropcheck(0x5990AC);
		return dropcheck.Call<bool>((u32)wX, (u32)wY, (u32)u0, disallowConcrete, u1);
	}
//Drop Item Wrapper	
	bool Dropper::PlaceItemWrapper(u8 ID, Item ItemToReplace, Item *ItemToPlace, Item *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID, bool itemsequenceallowed) {
		if(!PlayerClass::GetInstance()->IsLoaded()) 
			return 0;

		if(!ItemToPlace->isValid()) {
			OSD::Notify(Utils::Format("Invalid Item: %08X", *(u32 *)ItemToPlace));
			return 0;
		}

	//sets waitanim as autowaitanim
		u8 autoWaitAnim = waitAnim;
		
		if(ID == 0xB || ID == 0x13) {
			if(RuntimeContext::getInstance()->isIndoors()) {
				ID = 0xA;
				autoWaitAnim = 0x53;
			}
		}

	//If item sequence is enabled use next item if not use standard ItemToPlace
		Item *actualItemToPlace = (ItemSequence::Enabled() && itemsequenceallowed) ? ItemSequence::Next() : ItemToPlace;
		
	//If item sequence is enabled use actualIteToPlace as the item to show, if not use standard ItemToShow
		Item *actualItemToShow = (ItemSequence::Enabled() && itemsequenceallowed) ? actualItemToPlace : ItemToShow;

	//gets current player index -> player you selected
		u8 currentIndex = Game::GetOnlinePlayerIndex();
		
	//checks if the drop is forced on someone else
		bool forced = (currentIndex != Game::GetActualPlayerIndex()) && (Game::GetActualPlayerIndex() <= 3);
		
	//not too sure when it is not 0xFFFFFFFF but if it is return
		if(Game::GetLockedSpotIndex(worldx, worldy, roomID) != 0xFFFFFFFF) {
			return 0;
		}
		
	//checks if item at coords is not 0 -> if place is valid to drop
		Item *pItemAtCoords = Game::GetItemAtWorldCoords(worldx, worldy);
		if(!pItemAtCoords) {
			return 0;
		}

		Item empty = {0x7FFE, 0};

		Item *actualItemAtCoords = (pItemAtCoords->ID == 0x7FFE) ? &empty : pItemAtCoords;

	//writes the item to replace, if 0xFFFFFFFF is selected as item it will replace item at coords, if not use standard ItemToReplace
		Item *actualItemToReplace = ItemToReplace == ReplaceEverything ? actualItemAtCoords : &ItemToReplace;

	//if item to replace is not item at coords return -> means there wont be an item to replace
		if(*actualItemToReplace != *actualItemAtCoords) {
			return 0;
		}

		Item crashPreventItem = RuntimeContext::getInstance()->isIndoors() ? Item{0x2001, 0} : Item{0x7FFE, 0x8000};

		Item *ItemReplace = (actualItemToReplace->ID == 0x7FFE) ? &crashPreventItem : actualItemToReplace;
		Item *ItemPlace = (actualItemToPlace->ID == 0x7FFE) ? &crashPreventItem : actualItemToPlace;
		Item *ItemShow = (actualItemToShow->ID == 0x7FFE) ? &crashPreventItem : actualItemToShow;

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
			static const Address animpatch(0x682434);

		//display pattern, smash rock, bury
			if(waitAnim == 0x5D || waitAnim == 0x6B || waitAnim == 0x5A) {
				noWait = true;
			}
			
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
				else {
					autoWaitAnim = 0x3C;
				}
				
				noWaitPick:
			//write coords and item and ID to animation
				data.Pick_3C_3D(ID, *(Item *)actualItemToReplace, Coord{ worldx, worldy });
			}
			
			else {
			//write coords and item to animation
				data.AppendAnimData<u8>(animInstance, 0xE, worldx);
				data.AppendAnimData<u8>(animInstance, 0xF, worldy);
				data.AppendAnimData<u16>(animInstance, 0x10, actualItemToReplace->ID == 0x7FFE ? 0x2001 : actualItemToReplace->ID);
				
			//If forced an noWait is false
				if(forced && !noWait) {
					autoWaitAnim = 0x4C;
				}
				
			//if bury animation
				if((ID == 0x13 || autoWaitAnim == 0x4A) && !noWait && !forced) {
					autoWaitAnim = 0x4A;
					Process::Patch(animpatch.addr, 0xE3A00000);
					data.AppendAnimData<u16>(animInstance, 0x10, ID);
					Sleep(Milliseconds(5));
				}
			//if forced with noWait true
				else if(forced) {
					autoWaitAnim = 0x4F;
				}
			}
			
		//if selected index is the same as your player
			if(currentIndex == Game::GetActualPlayerIndex()) {
				data.ExecuteAnimation(autoWaitAnim);
			}
		//if not aka another player
			else {
				Animation::SendAnimPacket(Game::GetActualPlayerIndex(), animInstance, autoWaitAnim, roomID == 0xA5 ? Player::GetRoom(currentIndex) : roomID, currentIndex);	
			}
			
		//sleep if noWait is turned off
			if(!noWait) {
				Sleep(Milliseconds(40));
				Process::Patch(animpatch.addr, 0xE3700001);
			}
		}
		return true;
	}
}