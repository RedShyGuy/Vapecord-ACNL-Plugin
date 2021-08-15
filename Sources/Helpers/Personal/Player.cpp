#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
/*
loads specific player
*/
	void Player::Load(int pIndex) {
		Animation::Idle();
	//This will load the players save data
		static const u32 pLoad = Region::AutoRegion(0x5C441C, 0x5C394C, 0x5C3464, 0x5C3464, 0x5C2D54, 0x5C2D54, 0x5C2A28, 0x5C2A28);

		static FUNCT func1(pLoad);
		func1.Call<void>(0, pIndex, 0);
		
	//This will port the player to his home
		static const u32 pReload = Region::AutoRegion(0x5B66FC, 0x5B5C14, 0x5B5744, 0x5B5744, 0x5B5034, 0x5B5034, 0x5B4D08, 0x5B4D08);
		static FUNCT func2(pReload);
		func2.Call<void>(GameHelper::GetRoomData(), pIndex, 1, 0);
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
		
		static const u32 ReloadOffset = Region::AutoRegion(0x320DE0, 0x3207F4, 0x320134, 0x320134, 0x31FDE8, 0x31FDE8, 0x31FCA0, 0x31FCA0);
		static FUNCT func(ReloadOffset);
		func.Call<void>(Data2);
	}

/*
writes design data
*/
	void Player::SetDesign(int slot, std::string DesignName, u16 PlayerID, std::string PlayerName, u8 PlayerGender, u16 PlayerTownID, std::string PlayerTownName, u32 DesignCheck1, u8 DesignCheck2, u8 DesignType) {
		if(Player::GetSaveOffset(4) == 0)
			return;
		
		u32 design = Player::GetDesign(slot);
		
		if(DesignName != "") 
			Process::WriteString(design, DesignName, 0x28, StringFormat::Utf16); //Sets Design Name
		
		if(PlayerID != 0xFFFF)
			Process::Write16(design + 0x2A, PlayerID); //Sets Player ID
		
		if(PlayerName != "") 
			Process::WriteString(design + 0x2C, PlayerName, 0x10, StringFormat::Utf16); //Sets Player Name

		if(PlayerGender != 0xFF)
			Process::Write8(design + 0x3E, PlayerGender); //Sets Player Gender
		if(PlayerTownID != 0xFFFF)
			Process::Write16(design + 0x40, PlayerTownID); //Sets Player Town ID
		
		if(PlayerTownName != "") 
			Process::WriteString(design + 0x42, PlayerTownName, 0x10, StringFormat::Utf16); //Sets Player Town Name
		
		if(DesignCheck1 != 0xFFFFFFFF)
			Process::Write32(design + 0x54, DesignCheck1); //Sets Design Check1
		if(DesignCheck2 != 0xFF)
			Process::Write8(design + 0x68, DesignCheck2); //Sets Design Check2
		if(DesignType != 0xFF)
			Process::Write8(design + 0x69, DesignType); //Sets Design Type
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
		
		u32 i = PlayerClass::GetInstance()->Offset();
		u32 GetStoredData = i + 0x1B4; //0x33077570
	//This Stores the Tan Data Correctly
		
		static const u32 GetTanDataOffset = Region::AutoRegion(0x713798, 0x712C48, 0x7127A0, 0x712778, 0x711F4C, 0x711F24, 0x711AF4, 0x711ACC);

		static FUNCT func1(GetTanDataOffset);
		u8 Tan = func1.Call<u8>(Player::GetSaveOffset(4) + 4);
		
		Process::Write8(GetStoredData + 0x1C0, Tan);
		
	//This Updates the Tan
		static const u32 UpdateTanOffset = Region::AutoRegion(0x1D0B90, 0x1D05D4, 0x1D0BB0, 0x1D0BB0, 0x1D0AEC, 0x1D0AEC, 0x1D0AEC, 0x1D0AEC);

		static FUNCT func2(UpdateTanOffset);
		func2.Call<void>(GetStoredData);
	}
/*
update appearance
*/
	void Player::UpdateStyle() {
		if(!PlayerClass::GetInstance()->IsLoaded())
			return;

	//This Updates the Outfit	
		static const u32 update = Region::AutoRegion(0x68B2E4, 0x68A80C, 0x68A31C, 0x68A31C, 0x689DDC, 0x689DDC, 0x689984, 0x689984);

		static FUNCT func(update);
		func.Call<void>(PlayerClass::GetInstance()->Offset());
	}
/*
Write Outfit
*/
	void Player::WriteOutfit(u8 PlayerIndex, u16 HeadWear, u16 Glasses, u16 Shirt, u16 Pants, u16 Socks, u16 Shoes) {
		u32 x, y;
		if(!PlayerClass::GetInstance(PlayerIndex)->GetWorldCoords(&x, &y))
			return;
		
		if(HeadWear != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, HeadWear, 0, 0, 0, 0, x, y, 0);
		if(Glasses != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Glasses, 1, 0, 0, 0, x, y, 0);
		if(Shirt != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Shirt, 2, 0, 0, 0, x, y, 0);
		if(Pants != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Pants, 4, 0, 0, 0, x, y, 0);
		if(Socks != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Socks, 5, 0, 0, 0, x, y, 0);
		if(Shoes != 0xFFFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0x36, Shoes, 6, 0, 0, 0, x, y, 0);
	}
/*
Write Appearance
*/
	void Player::WriteAppearance(u8 PlayerIndex, u8 HairStyle, u8 HairColor, u8 EyeStyle, u8 EyeColor, u8 TanLevel, u8 Gender) {
		u32 x, y;
		if(!PlayerClass::GetInstance(PlayerIndex)->GetWorldCoords(&x, &y))
			return;
		
		u8 HairStyleID = HairStyle != -1 ? HairStyle : *(u8 *)PlayerPTR::Pointer(4);
		u8 HairColorID = HairColor != -1 ? HairColor : *(u8 *)PlayerPTR::Pointer(5);
		u8 EyeColorID = EyeColor != -1 ? EyeColor : *(u8 *)PlayerPTR::Pointer(7);
		
		u8 Appearance[3] = {HairStyleID, HairColorID, EyeColorID};
		
		if(HairStyle != 0xFF | EyeColor != 0xFF | HairColor != 0xFF) Animation::ExecuteAnimationWrapper(PlayerIndex, 0xB9, 0, 0, 0, 0, 0, x, y, 0, Appearance);
		if(EyeStyle != 0xFF) PlayerPTR::Write8(6, EyeStyle);
		if(TanLevel != 0xFF) PlayerPTR::Write8(8, TanLevel);		
		if(Gender != 0xFF) PlayerPTR::Write8(0x55BA, Gender);	
	}
//If player is loaded (uses to 8) 27B0E0(pP)
/*
get status of specific player
*/
	PlayerStatus Player::GetPlayerStatus(u8 pPlayer) {
		static const u32 pPOffset = Region::AutoRegion(0x2FF6CC, 0x2FF8B0, 0x2FF754, 0x2FF754, 0x2FF92C, 0x2FF92C, 0x2FF978, 0x2FF978);

		static FUNCT func(pPOffset);
		return func.Call<PlayerStatus>(pPlayer);
	}

/*
Get player save offset for any player
*/
	u32 Player::GetSpecificSave(u8 pPlayer) {
		static const u32 pPOffset = Region::AutoRegion(0x2FBA60, 0x2FB96C, 0x2FBAE8, 0x2FBAE8, 0x2FB920, 0x2FB920, 0x2FB8F8, 0x2FB8F8);

		static FUNCT func(pPOffset);
		return func.Call<u32>(pPlayer);
	}
/*
Get Player Save Offset for loaded players
*/
	u32 Player::GetSaveOffset(u8 pIndex) {
		static const u32 pSOffset = Region::AutoRegion(0x2FEB2C, 0x2FE8A8, 0x2FEBB4, 0x2FEBB4, 0x2FEB60, 0x2FEB60, 0x2FEA98, 0x2FEA98);

		static FUNCT func(pSOffset);
		return func.Call<u32>(pIndex);
	}

//get current selected mail slot
	int Player::GetMailSlot() {	
		if(GameHelper::BaseInvPointer() == 0) 
			return 0xFF;
		
		u8 slot = *(u8 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0xCC);
		if(slot != 0xFF && slot < 0x1E && slot > 0x13) 
			return (int)slot - 19;
		
		return 0xFF;
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
		static const u32 ClearMailOffset = Region::AutoRegion(0x2FF34C, 0, 0, 0, 0, 0, 0, 0);
		u32 RemoveMailOffset;
		u32 MailPoint = *(u32 *)(GameHelper::BaseInvPointer() + 0xC);	
		
	//If Inventory is MailBox	
		if(i == 0x41) {
		//Gets Correct Mail Data
			MailDataOffset = Region::AutoRegion(0x21FFEC, 0, 0, 0, 0, 0, 0, 0);
			static FUNCT func1(MailDataOffset);
			MailData = func1.Call<u32>(MailPoint, slot);

		//Clears Mail
			static FUNCT func2(ClearMailOffset);
			func2.Call<void>(MailData);

		//If slot is in players mail inv	
			if(slot < 0xA) {
				RemoveMailOffset = Region::AutoRegion(0x2E3B18, 0, 0, 0, 0, 0, 0, 0);

				static FUNCT func3(RemoveMailOffset);
				func3.Call<void>(MailPoint + 0x2CBC, slot);
				return;
			}
			RemoveMailOffset = Region::AutoRegion(0x2740EC, 0, 0, 0, 0, 0, 0, 0);
			static FUNCT func4(RemoveMailOffset);
			func4.Call<void>(MailPoint + 0xC8, slot);
			return;
		}	
	
	//Gets Correct Mail Data
		MailDataOffset = Region::AutoRegion(0x320B34, 0, 0, 0, 0, 0, 0, 0);
		static FUNCT func5(MailDataOffset);
		MailData = func5.Call<u32>(*(u32 *)(MailPoint + 0xA328), slot);

	//Clears Mail Save Data
		static FUNCT func6(ClearMailOffset);
		func6.Call<void>(MailData);

	//Removes Mail Icon
		RemoveMailOffset = Region::AutoRegion(0x2E3B18, 0, 0, 0, 0, 0, 0, 0);

		static FUNCT func7(RemoveMailOffset);
		func7.Call<void>(*(u32 *)(MailPoint + 0xA328), slot);
	}
*/

/*
Set Tool
*/
	void Player::SetTool(u16 tool) {
		if(Player::GetSaveOffset(4) == 0) 
			return;
			
		Process::Write16(PlayerPTR::Pointer(0x26), tool);
	}
/*
Get Tool
*/
	u16 Player::GetTool() {
		return Player::GetSaveOffset(4) != 0 ? *(u16 *)PlayerPTR::Pointer(0x26) : 0x7FFE;
	}
/*
If player exitst
*/
	bool Player::Exists(u8 PlayerIndex) {
		static const u32 existsFunction = Region::AutoRegion(0x75F84C, 0x75E830, 0x75E854, 0x75E82C, 0x75DFEC, 0x75DFC4, 0x75DB94, 0x75DB6C);
		static FUNCT func(existsFunction);
		u8 room = func.Call<u8>(*(u32 *)Code::GamePointer + 0x1C1, PlayerIndex);
		return room == 0xA5 ? 0 : 1;
	}
/*
get location
*/
	bool Player::IsIndoors() {
		static const u32 getlocation = Region::AutoRegion(0x1E890C, 0x1E8350, 0x1E892C, 0x1E892C, 0x1E8868, 0x1E8868, 0x1E8834, 0x1E8834);

		static FUNCT func(getlocation);
		return func.Call<bool>();
	}
/*
get room
*/
	u8 Player::GetRoom(u8 PlayerIndex) {
		static const u32 getroom = Region::AutoRegion(0x5C3DDC, 0x5C330C, 0x5C2E24, 0x5C2E24, 0x5C2714, 0x5C2714, 0x5C23E8, 0x5C23E8); 

		static FUNCT func(getroom);
		u32 animObjPtr = func.Call<u32>(PlayerIndex);
		return animObjPtr == 0 ? 0xFF : *(u8 *)animObjPtr;
	}


	void PlayerName::UpdateReference(u8 pIndex, const std::string& pName, s8 pGender) {
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

			if(pName != "") 
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
