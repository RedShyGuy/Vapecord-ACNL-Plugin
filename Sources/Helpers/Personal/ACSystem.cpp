#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
    bool ACSystem::IsKeyDown(GameKey::GameKey key) {
		static const u32 ControllerInputCheck = Region::AutoRegion(0x304A14, 0x304C1C, 0x304AA4, 0x304AA4, 0x304A44, 0x304A44, 0x3049F4, 0x3049F4);
		static FUNCT func(ControllerInputCheck);
		return func.Call<bool>(key);
	}

	/*TouchCoord ACSystem::GetTouch() {
		static const u32 ControllerPointer = Region::AutoRegion(0x9762F4, 0, 0, 0, 0, 0, 0, 0);
		TouchCoord Coord = { 0, 0 };

		u32 Controll = Region::FollowPointer(ControllerPointer, 0xD8, 8, -1);
		if(Controll == -1)
			return Coord;

		Coord = *(TouchCoord *)(Controll + 0x24);

		return Coord;
	}

	u16 ACSystem::GetGameCoins() {
		static const u32 GetGCoins = Region::AutoRegion(0x6C9D58, 0, 0, 0, 0, 0, 0, 0);
		static FUNCT func(GetGCoins);
		return func.Call<u16>();
	}
	//something with anim stage
	//6E6078(0xA15B5C, 0x30198E40, 1);

	void FUN_006ca018(undefined4 param_1)
	bool FUN_00293264(void)
	int FUN_006a6660(uint param_1)
	void FUN_00133a20(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)


	bool ACSystem::SetGameCoins(int gamecoins) {
		
		u16 CurrentGC = ACSystem::GetGameCoins();
		FUN(CurrentGC);	

		static const u32 SubGC = Region::AutoRegion(0x34CAA0, 0, 0, 0, 0, 0, 0, 0);
		Process::Write32(SubGC, 0xE0800004);

		0xBFB420

		bool res = FUN(gamecoins);
//BFB420 (330BB950, x)
		Process::Write32(SubGC, 0xE0400004);
		return res; //0x953AE8
		static const u32 SetGCoins = Region::AutoRegion(0x6C9DA8, 0, 0, 0, 0, 0, 0, 0);
		static FUNCT func(SetGCoins);
		return func.Call<bool>(gamecoins);
	}*/
//32DF6790

/*
Light Switch cheats I made for fun 
*/

	void FUN_0056A2C8(u32 param_1, u32 param_2) {
		*(u32 *)(param_1 + 8) = param_2;
		*(u32 *)(param_1 + 0xC) = param_2;

		if(*(u32 *)(param_1 + 0x18) != 0) 
			*(u32 *)(*(u32 *)(param_1 + 0x18) + 0x10) = param_2;
	}

	u32 LightSwitch::GetData() {
		static const u32 data = Region::AutoRegion(0x951030, 0x950020, 0x95002C, 0x95002C, 0x94A02C, 0x94902C, 0x94902C, 0x94902C);
		return *(u32 *)data;
	}

	void LightSwitch::SetData(bool ON) {
		static const u32 data1 = Region::AutoRegion(0x569118, 0x568630, 0x568160, 0x568160, 0x567A50, 0x567A50, 0x567770, 0x567770);
		static const u32 data2 = Region::AutoRegion(0x5696F8, 0x568C10, 0x568740, 0x568740, 0x568030, 0x568030, 0x567D50, 0x567D50);
		static const u32 data3 = Region::AutoRegion(0x56946C, 0x568984, 0x5684B4, 0x5684B4, 0x567DA4, 0x567DA4, 0x567AC4, 0x567AC4);

		u32 switchData = GetData();
		if(switchData == 0)
			return;

		FUNCT func(data1);
		func.Call<void>(switchData + 0x38, switchData + 0x3F4, *(u32 *)(switchData + 0x488), 0);

		FUN_0056A2C8(switchData + 0x3F4, ON ? 0x3F800000 : 0);

		func = FUNCT(data2);
		func.Call<void>(switchData + 0x38);

		func = FUNCT(data3);
		func.Call<void>(switchData + 0x38, switchData + 0x3F4, *(u32 *)(switchData + 0x488), 0);
	}

	void LightSwitch::ON(u8 roomID) {
		static const u32 lightON = Region::AutoRegion(0x1E6844, 0x1E6288, 0x1E6864, 0x1E6864, 0x1E67A0, 0x1E67A0, 0x1E676C, 0x1E676C);
		static FUNCT func(lightON);
		func.Call<void>(0, roomID);
		SetData(true);

		if(GameHelper::IsInRoom(roomID))
			GameHelper::PlaySound(0x4F3 + IsBasement());
	}

	void LightSwitch::OFF(u8 roomID) {
		static const u32 lightOFF = Region::AutoRegion(0x1E7514, 0x1E6F58, 0x1E7534, 0x1E7534, 0x1E7470, 0x1E7470, 0x1E743C, 0x1E743C);
		static FUNCT func(lightOFF);
		func.Call<void>(0, roomID);
		SetData(false);

		if(GameHelper::IsInRoom(roomID))
			GameHelper::PlaySound(0x4F5);
	}

	bool LightSwitch::IsON(u8 roomID) {
		static const u32 lightIsON = Region::AutoRegion(0x1E7AC0, 0x1E7504, 0x1E7AE0, 0x1E7AE0, 0x1E7A1C, 0x1E7A1C, 0x1E79E8, 0x1E79E8);
		static FUNCT func(lightIsON);
		return func.Call<bool>(roomID);
	}

	bool LightSwitch::IsBasement(u8 roomID) {
		static const u32 lightBasement = Region::AutoRegion(0x1E8400, 0x1E7E44, 0x1E8420, 0x1E8420, 0x1E835C, 0x1E835C, 0x1E8328, 0x1E8328);
		static FUNCT func(lightBasement);
		return func.Call<bool>(roomID);
	}

/*
MEOW Coupon Cheats
*/

	static const u16 InitArr[102] = {
		0x0000, 0x0001, 0x0001, 0x0014, 0x0001, 0x2710, 0x0014, 0x0001, 0x0001, 
		0x0001, 0x0005, 0xC350, 0x0001, 0x0003, 0x0014, 0x0014, 0x000A, 0x0001, 
		0x0001, 0x0003, 0x0001, 0x0001, 0x0001, 0x0003, 0x0003, 0x000A, 0x0001, 
		0x0001, 0x0001, 0x0003, 0x0003, 0x0008, 0x0001, 0x0001, 0x1388, 0x0001, 
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0005, 
		0x0001, 0x0001, 0x0001, 0x1388, 0x0064, 0x0001, 0x0005, 0x0001, 0x0001, 
		0x0001, 0x0001, 0x0096, 0x0001, 0x01F4, 0x0001, 0x0005, 0x000A, 0x0001, 
		0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0003, 0x0001, 0x0001, 
		0x0001, 0x0001, 0x0005
	};

	u16 MEOW::GetInitiativesValue(int Initiative) {
		return InitArr[Initiative]; //0 is not valid
	}

	u8 MEOW::GetCurrentInitiatives(u32 InitiativeSaveOffset, int InitPos, int weekDay) {
		switch(InitPos) {
			default: return 0;
			case 0: return *(u8 *)(InitiativeSaveOffset + 0x11 + weekDay); //Daily
			case 1: return *(u8 *)(InitiativeSaveOffset + 0xA + weekDay); //Daily
			case 2: return *(u8 *)(InitiativeSaveOffset + 8); //Weekly
			case 3: return *(u8 *)(InitiativeSaveOffset + 9); //Weekly
		}
	}

	bool MEOW::InitiativeFinished(u32 InitiativeSaveOffset, int Initiative) {
		u16 iVar1 = GetInitiativesValue(Initiative);
		return iVar1 <= *(u16 *)(InitiativeSaveOffset + Initiative * 4 + 0x1C);
	}

	u32 MEOW::GetInitiativeOffset(u32 InitiativeSaveOffset, int Initiative) {
		return (InitiativeSaveOffset + Initiative * 4 + 0x1C);
	}

	void MEOW::ToggleInitiative(int pPos, int InitPos, int weekDay, bool finish) {
		u32 InitiativeSaveOffset = Player::GetSaveOffset(pPos) + 0x8AE0; //PlayerSaveOffset + 0x8AE0

		u8 currInit = GetCurrentInitiatives(InitiativeSaveOffset, InitPos, weekDay);

		if(!((currInit != 0) && (currInit < 0x66))) //0 is invalid Initiative | 0x65 is highest Initiative 
			return;

		if(InitiativeFinished(InitiativeSaveOffset, currInit) && !finish) {
			*(u16 *)GetInitiativeOffset(InitiativeSaveOffset, currInit) = 0; //Clear completion of Initiative
			return;
		}
		else if(!InitiativeFinished(InitiativeSaveOffset, currInit) && finish) {
			*(u16 *)GetInitiativeOffset(InitiativeSaveOffset, currInit) = GetInitiativesValue(currInit); //Complete Initiative
			return;
		}
	} 
}