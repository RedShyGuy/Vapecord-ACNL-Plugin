#include "Helpers/Player.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/PlayerClass.hpp"
#include "RegionCodes.hpp"

bool IsIndoorsBool = false;

namespace CTRPluginFramework {
/*
loads specific player
*/
	void Player::Load(int pIndex) {
		Animation::Idle();
	//This will load the players save data
		static Address pLoad(0x5C441C, 0x5C394C, 0x5C3464, 0x5C3464, 0x5C2D54, 0x5C2D54, 0x5C2A28, 0x5C2A28);
		pLoad.Call<void>(0, pIndex, 0);

		//BF77B0(0x330981F4)
		
	//This will port the player to his home
		static Address pReload(0x5B66FC, 0x5B5C14, 0x5B5744, 0x5B5744, 0x5B5034, 0x5B5034, 0x5B4D08, 0x5B4D08);
		pReload.Call<void>(GameHelper::GetRoomData(), pIndex, 1, 0);
	}

/*
reload design
*/
	void Player::ReloadDesign(int slot) {
		if(GameHelper::BaseInvPointer() == 0) 
			return;
		
		if(Inventory::GetCurrent() != 1)
			return;
		
		u32 InvData = *(u32 *)(GameHelper::BaseInvPointer() + 0xC);
		u32 Data1 = InvData + (slot * 0x290);
		u32 Data2 = Data1 + 0x6BC;
		
		static Address ReloadOffset(0x320DE0, 0x3207F4, 0x320134, 0x320134, 0x31FDE8, 0x31FDE8, 0x31FCA0, 0x31FCA0);
		ReloadOffset.Call<void>(Data2);
	}

/*
writes design data
*/
	void Player::StealDesign(u8 slot) {
		ACNL_Player *player = Player::GetData();
		if(!player)
			return;

		player->Patterns[slot].CreatorData = player->PlayerInfo;
	}
/*
Gets correct design data
*/
	u32 Player::GetDesign(int slot, int pIndex) {
		return (Player::GetSaveOffset(pIndex) + 0x2C) + (0x870 * slot);
	}
/*
Get correct bulletin board message (14 Max)
*/
	u32 Player::GetBulletin(int slot) {
		return Save::GetInstance()->Address(0x6E2B8 + (0x1AC * slot));
	}
/*
Update Tan
*/
	void Player::UpdateTan() {
		if(!PlayerClass::GetInstance()->IsLoaded())
			return;

		ACNL_Player *player = Player::GetData();
		if(!player)
			return;
		
		u32 i = PlayerClass::GetInstance()->Offset();
		u32 GetStoredData = i + 0x1B4; //0x33077570
	//This Stores the Tan Data Correctly
		
		static Address GetTanDataOffset(0x713798, 0x712C48, 0x7127A0, 0x712778, 0x711F4C, 0x711F24, 0x711AF4, 0x711ACC);
		u8 Tan = GetTanDataOffset.Call<u8>(player->Tan);
		
		Process::Write8(GetStoredData + 0x1C0, Tan);
		
	//This Updates the Tan
		static Address UpdateTanOffset(0x1D0B90, 0x1D05D4, 0x1D0BB0, 0x1D0BB0, 0x1D0AEC, 0x1D0AEC, 0x1D0AEC, 0x1D0AEC);
		UpdateTanOffset.Call<void>(GetStoredData);
	}
/*
update appearance
*/
	void Player::UpdateStyle() {
		if(!PlayerClass::GetInstance()->IsLoaded())
			return;

	//This Updates the Outfit	
		static Address update(0x68B2E4, 0x68A80C, 0x68A31C, 0x68A31C, 0x689DDC, 0x689DDC, 0x689984, 0x689984);
		update.Call<void>(PlayerClass::GetInstance()->Offset());
	}
/*
Write Outfit
*/
	void Player::WriteOutfit(u8 PlayerIndex, Item HeadWear, Item Glasses, Item Shirt, Item Pants, Item Socks, Item Shoes) {
		u32 x, y;
		if(!PlayerClass::GetInstance(PlayerIndex)->GetWorldCoords(&x, &y))
			return;
		
		if(HeadWear.ID != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, HeadWear, 0, 0, 0, 0, x, y, 0);
		if(Glasses.ID != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Glasses, 1, 0, 0, 0, x, y, 0);
		if(Shirt.ID != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Shirt, 2, 0, 0, 0, x, y, 0);
		if(Pants.ID != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Pants, 4, 0, 0, 0, x, y, 0);
		if(Socks.ID != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Socks, 5, 0, 0, 0, x, y, 0);
		if(Shoes.ID != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Shoes, 6, 0, 0, 0, x, y, 0);
	}
//If player is loaded (uses to 8) 27B0E0(pP)
/*
get status of specific player
*/
	PlayerStatus Player::GetPlayerStatus(u8 pPlayer) {
		static Address pPOffset(0x2FF6CC, 0x2FF8B0, 0x2FF754, 0x2FF754, 0x2FF92C, 0x2FF92C, 0x2FF978, 0x2FF978);
		return pPOffset.Call<PlayerStatus>(pPlayer);
	}

/*
Get player save offset for any player
*/
	u32 Player::GetSpecificSave(u8 pPlayer) {
		static Address pPOffset(0x2FBA60, 0x2FB96C, 0x2FBAE8, 0x2FBAE8, 0x2FB920, 0x2FB920, 0x2FB8F8, 0x2FB8F8);
		return pPOffset.Call<u32>(pPlayer);
	}
/*
Get Player Save Offset for loaded players
*/
	u32 Player::GetSaveOffset(u8 pIndex) {
		static Address pSOffset(0x2FEB2C, 0x2FE8A8, 0x2FEBB4, 0x2FEBB4, 0x2FEB60, 0x2FEB60, 0x2FEA98, 0x2FEA98);
		return pSOffset.Call<u32>(pIndex);
	}

	bool Player::SetUnlockableBitField(ACNL_Player *player, u8 ID, bool state) {
		if(!player)
			return false;

		static Address SetUp1(0x5360A8, 0, 0, 0, 0, 0, 0, 0);
		static Address SetUp2(0x6BA680, 0, 0, 0, 0, 0, 0, 0);
		static Address SetStack(0x2FCC14, 0, 0, 0, 0, 0, 0, 0);
		static Address ReadStack(0x769DBC, 0, 0, 0, 0, 0, 0, 0);

		u32 val = SetUp1.Call<u32>(ID);
		u32 uVar5 = *(u32 *)(val + 4);
		u32 data;

		if(uVar5 < 0x6EE) {
			u32 uVar6 = 0;
			if(uVar5 != 0) {
				do {
					u32 uVar3 = SetUp2.Call<u32>(ID, uVar6);
					SetStack.Call<void>(&data, uVar3);
					s32 uVar4 = ReadStack.Call<u32>(&data);

					if(uVar4 > -1) {
						if((uVar4 >> 5) < 0xBA) {
							if(state)
								player->UnlockedItems[(uVar4 >> 5)] |= (1 << (uVar4 & 0x1F));
							else
								player->UnlockedItems[(uVar4 >> 5)] &= ~(1 << (uVar4 & 0x1F));
						}
					}		

					uVar6++;
				} while(uVar6 < uVar5);	
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
		static Address existsFunction(0x75F84C, 0x75E830, 0x75E854, 0x75E82C, 0x75DFEC, 0x75DFC4, 0x75DB94, 0x75DB6C);
		u8 room = existsFunction.Call<u8>(*(u32 *)Code::GamePointer.addr + 0x1C1, PlayerIndex);
		return room == 0xA5 ? 0 : 1;
	}
/*
get location
*/
	bool Player::IsIndoors() {
		static Address getlocation(0x1E890C, 0x1E8350, 0x1E892C, 0x1E892C, 0x1E8868, 0x1E8868, 0x1E8834, 0x1E8834);
		return getlocation.Call<bool>();
	}
/*
get room
*/
	u8 Player::GetRoom(u8 PlayerIndex) {
		static Address getroom(0x5C3DDC, 0x5C330C, 0x5C2E24, 0x5C2E24, 0x5C2714, 0x5C2714, 0x5C23E8, 0x5C23E8); 
		u32 var = getroom.Call<u32>(PlayerIndex);
		return var == 0 ? 0xFF : *(u8 *)var;
	}

	ACNL_Player *Player::GetData(u8 PlayerIndex) {
		u32 *addr = (u32 *)GetSpecificSave(PlayerIndex >= 4 ? GameHelper::GetOnlinePlayerIndex() : PlayerIndex);
		return (ACNL_Player *)addr;
	}

	bool Player::SaveExists(ACNL_Player *player) {
		return (player && player->PlayerInfo.PID != 0 && player->PlayerInfo.TownData.TID != 0);
	}

	void PlayerName::UpdateReference(u8 pIndex, const std::string& pName, u8 pGender) {
		u32 pAddress = Player::GetSaveOffset(pIndex >= 4 ? GameHelper::GetOnlinePlayerIndex() : pIndex);

	//search all player ID references
		u16 pData[11], pCheck[11];
		Process::CopyMemory((void *)pData, (void *)(pAddress + 0x55A6), 22);

		for(u32 addr = Save::GetInstance()->Address(); addr < Save::GetInstance()->Address(SAVE_GARDENPLUS); addr += 2) {
			Process::CopyMemory((void *)pCheck, (void *)addr, 22);
			if(!std::equal(std::begin(pData), std::end(pData), std::begin(pCheck)))
				continue;

			if(pGender != -1)
				Process::Write8(addr + 20, pGender);

			if(!pName.empty()) 
				Process::WriteString(addr + 2, pName, 16, StringFormat::Utf16);

			addr += 22;
		}
	}
	
/*
Set Player Name
*/
	void PlayerName::Set(const std::string& name) {
		PlayerName::UpdateReference(4, name, -1);
	}
}
