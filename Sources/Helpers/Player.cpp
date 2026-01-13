#include "Helpers/Player.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/Converters.hpp"
#include "Address/Address.hpp"
#include "Color.h"

namespace CTRPluginFramework {
/*
loads specific player
*/
	void Player::Load(int pIndex) {
		Animation::Idle();
	//This will load the players save data
		static Address pLoad(0x5C441C);
		pLoad.Call<void>(0, pIndex, 0);

		//BF77B0(0x330981F4)
		
	//This will port the player to his home
		static Address pReload(0x5B66FC);
		pReload.Call<void>(Game::GetRoomData(), pIndex, 1, 0);
	}

/*
reload design
*/
	void Player::ReloadDesign(int slot) {
		if(Game::BaseInvPointer() == 0) {
			return;
		}
		
		if(Inventory::GetCurrent() != 1) {
			return;
		}
		
		u32 InvData = *(u32 *)(Game::BaseInvPointer() + 0xC);
		u32 Data1 = InvData + (slot * 0x290);
		u32 Data2 = Data1 + 0x6BC;
		
		static Address ReloadOffset(0x320DE0);
		ReloadOffset.Call<void>(Data2);
	}
/*
writes design data
*/
	void Player::StealDesign(u8 slot) {
		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return;
		}

		player->Patterns[slot].CreatorData = player->PlayerInfo;
	}
/*
Update Tan
*/
	void Player::UpdateTan() {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			return;
		}

		ACNL_Player *player = Player::GetSaveData();
		if(!player) {
			return;
		}
		
		u32 i = PlayerClass::GetInstance()->Offset();
		u32 GetStoredData = i + 0x1B4; //0x33077570
	//This Stores the Tan Data Correctly
		
		static Address GetTanDataOffset(0x713798);
		u8 Tan = GetTanDataOffset.Call<u8>(&player->PlayerFeatures);
		
		Process::Write8(GetStoredData + 0x1C0, Tan);
		
	//This Updates the Tan
		static Address UpdateTanOffset(0x1D0B90);
		UpdateTanOffset.Call<void>(GetStoredData);
	}
/*
update appearance
*/
	void Player::UpdateStyle() {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			return;
		}

	//This Updates the Outfit	
		static Address update(0x68B2E4);
		update.Call<void>(PlayerClass::GetInstance()->Offset());
	}
/*
Write Outfit
*/
	void Player::WriteOutfit(u8 PlayerIndex, Item HeadWear, Item Glasses, Item Shirt, Item Pants, Item Socks, Item Shoes) {
		u32 x, y;
		if(!PlayerClass::GetInstance(PlayerIndex)->GetWorldCoords(&x, &y)) {
			return;
		}
		
		if(HeadWear.ID != 0xFFFF) {
			Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, HeadWear, 0, 0, 0, 0, x, y, 0);
		}
		if(Glasses.ID != 0xFFFF) {
			Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Glasses, 1, 0, 0, 0, x, y, 0);
		}
		if(Shirt.ID != 0xFFFF) {
			Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Shirt, 2, 0, 0, 0, x, y, 0);
		}
		if(Pants.ID != 0xFFFF) {
			Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Pants, 4, 0, 0, 0, x, y, 0);
		}
		if(Socks.ID != 0xFFFF) {
			Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Socks, 5, 0, 0, 0, x, y, 0);
		}
		if(Shoes.ID != 0xFFFF) {
			Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Shoes, 6, 0, 0, 0, x, y, 0);
		}
	}
//If player is loaded (uses to 8) 27B0E0(pP)
/*
get status of specific player
*/
	PlayerStatus Player::GetPlayerStatus(u8 pPlayer) {
		static Address pPOffset(0x2FF6CC);
		return pPOffset.Call<PlayerStatus>(pPlayer);
	}

/*
Get player save offset for any player
*/
	u32 Player::GetSpecificSave(u8 pPlayer) {
		static Address pPOffset(0x2FBA60);
		return pPOffset.Call<u32>(pPlayer);
	}
/*
Get Player Save Offset for loaded players
*/
	u32 Player::GetSaveOffset(u8 pIndex) {
		static Address pSOffset(0x2FEB2C);
		return pSOffset.Call<u32>(pIndex);
	}

	bool Player::SetUnlockableBitField(ACNL_Player *player, Item_Category category, bool state) {
		if(!player) {
			return false;
		}

		static Address GetItemIDFromCategory(0x5360A8);

		u32 categoryData = GetItemIDFromCategory.Call<u32>(category);
		Item startingItem = *(Item *)(categoryData);
		u32 categorySize = *(u32 *)(categoryData + 4);

		if(categorySize < 0x6EE && categorySize > 0) {
			for (u32 i = 0; i < categorySize; ++i) {
				Item nextItem = startingItem + i;

				const s32 bitFieldPos = nextItem.ID - 0x2000;
				if (bitFieldPos >= 0x172B) {
					continue;
				}

				const u32 wordIndex = bitFieldPos >> 5;
				if (wordIndex >= 0xBA) {
					continue;
				}

				const u32 bitMask = 1u << (bitFieldPos & 0x1F);
				if (state) {
					player->UnlockedItems[wordIndex] |= bitMask;
				} else {
					player->UnlockedItems[wordIndex] &= ~bitMask;
				}
			}
		}
		return true;
	}
/*
//Clear Mail in inv
	void Player::ClearMail(int slot) {
		if(Player::GetSaveOffset(4) == 0)
			return;
		
		if(GameHelper::BaseInvPointer() == 0) 
			return;
		
		u32 i = Inventory::GetCurrent();
		if(i != 0x41 && (i != 0 && !Inventory::Opened()))
			return;
			
		u32 MailDataOffset;
		u32 MailData;
		static const u32 ClearMailOffset(0x2FF34C, 0, 0, 0, 0, 0, 0, 0);
		u32 RemoveMailOffset;
		u32 MailPoint = *(u32 *)(GameHelper::BaseInvPointer() + 0xC);	
		
	//If Inventory is MailBox	
		if(i == 0x41) {
		//Gets Correct Mail Data
			MailDataOffset(0x21FFEC, 0, 0, 0, 0, 0, 0, 0);
			static FUNCT func1(MailDataOffset);
			MailData = func1.Call<u32>(MailPoint, slot);

		//Clears Mail
			static FUNCT func2(ClearMailOffset);
			func2.Call<void>(MailData);

		//If slot is in players mail inv	
			if(slot < 0xA) {
				RemoveMailOffset(0x2E3B18, 0, 0, 0, 0, 0, 0, 0);

				static FUNCT func3(RemoveMailOffset);
				func3.Call<void>(MailPoint + 0x2CBC, slot);
				return;
			}
			RemoveMailOffset(0x2740EC, 0, 0, 0, 0, 0, 0, 0);
			static FUNCT func4(RemoveMailOffset);
			func4.Call<void>(MailPoint + 0xC8, slot);
			return;
		}	
	
	//Gets Correct Mail Data
		MailDataOffset(0x320B34, 0, 0, 0, 0, 0, 0, 0);
		static FUNCT func5(MailDataOffset);
		MailData = func5.Call<u32>(*(u32 *)(MailPoint + 0xA328), slot);

	//Clears Mail Save Data
		static FUNCT func6(ClearMailOffset);
		func6.Call<void>(MailData);

	//Removes Mail Icon
		RemoveMailOffset(0x2E3B18, 0, 0, 0, 0, 0, 0, 0);

		static FUNCT func7(RemoveMailOffset);
		func7.Call<void>(*(u32 *)(MailPoint + 0xA328), slot);
	}
*/

/*
If player exitst
*/
	bool Player::Exists(u8 PlayerIndex) {
		static Address existsFunction(0x75F84C);
		u8 room = existsFunction.Call<u8>(*(u32 *)Address(0x954648).addr + 0x1C1, PlayerIndex);
		return room == 0xA5 ? 0 : 1;
	}
/*
get location
*/
	bool Player::IsIndoors() {
		u8 roomId = Game::GetRoom();
		if (roomId == 0 || //Town
			roomId == 1 || //Main Street
			roomId == 0x2D || //Bug-Off Ceremony
			roomId == 0x2F || //Fishing Tourney Ceremony
			roomId == 0x5E || //Title Screen
			roomId >= 0x68 && roomId <= 0x80 || //Island/Tours
			roomId >= 0x8E && roomId <= 0x91 || //Happy Home Showcase
			roomId >= 0x9D && roomId <= 0xA0) { //Desert Island Escape/Puzzle League & Campground
			return false;
		} 
		return true;
		//static Address getlocation(0x1E890C);
		//return getlocation.Call<bool>();
	}
/*
get room
*/
	u8 Player::GetRoom(u8 PlayerIndex) {
		static Address getroom(0x5C3DDC); 
		u32 var = getroom.Call<u32>(PlayerIndex);
		return var == 0 ? 0xFF : *(u8 *)var;
	}

	bool Player::IsInRoom(u8 room, u8 playerIndex) {
		return Player::GetRoom(playerIndex) == room;
	}

	ACNL_Player *Player::GetSaveData(u8 PlayerIndex) {
		u32 *addr = (u32 *)GetSpecificSave(PlayerIndex >= 4 ? (u8)Player::GetPlayerStatus(4) : PlayerIndex);
		return (ACNL_Player *)addr;
	}

	bool Player::SaveExists(ACNL_Player *player) {
		return (player && player->PlayerInfo.PlayerData.PID != 0 && player->PlayerInfo.TownData.TID != 0);
	}

	void Player::EditGender(u8 pIndex, u8 pGender) {
		ACNL_Player *player = Player::GetSaveData(pIndex);
		if(!player) {
			return;
		}

		u32 GardenPlus = Save::GetInstance()->AtOffset();

		PlayerID oldPlayerID = player->PlayerInfo.PlayerData;
		player->PlayerInfo.PlayerData.Gender = pGender;
		PlayerID newPlayerID = player->PlayerInfo.PlayerData;

		for(u32 addr = GardenPlus; addr < (GardenPlus + SAVE_GARDENPLUS); addr++) {
			if(*(PlayerID *)addr == oldPlayerID) {
				*(PlayerID *)addr = newPlayerID;
			}
		}
	}

	void Player::EditName(u8 pIndex, const std::string& name) {
		ACNL_Player *player = Player::GetSaveData(pIndex);
		if(!player) {
			return;
		}

		u32 GardenPlus = Save::GetInstance()->AtOffset();

		PlayerID oldPlayerID = player->PlayerInfo.PlayerData;
		Convert::STR_TO_U16(name, player->PlayerInfo.PlayerData.PlayerName);
		PlayerID newPlayerID = player->PlayerInfo.PlayerData;

		for(u32 addr = GardenPlus; addr < (GardenPlus + SAVE_GARDENPLUS); addr++) {
			if(*(PlayerID *)addr == oldPlayerID) {
				*(PlayerID *)addr = newPlayerID;
			}
		}
	}

	Color Player::GetColor(u8 pIndex) {
		static const Color pColor[4] = {
			Color(pBlue), Color(pRed),		
			Color(pGreen), Color(pYellow),	
		};

		if (pIndex >= 4) {
			pIndex = Game::GetActualPlayerIndex();
		}

		return pColor[pIndex];
	}
}
