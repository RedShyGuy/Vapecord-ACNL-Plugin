#include "Helpers/ACSystem.hpp"
#include "Address/Address.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Player.hpp"

namespace CTRPluginFramework {
    bool ACSystem::IsKeyDown(GameKey::GameKey key) {
		static Address ControllerInputCheck(0x304A14);
		return ControllerInputCheck.Call<bool>(key);
	}

	/*TouchCoord ACSystem::GetTouch() {
		static const u32 ControllerPointer(0x9762F4, 0, 0, 0, 0, 0, 0, 0);
		TouchCoord Coord = { 0, 0 };

		u32 Controll = Region::FollowPointer(ControllerPointer, 0xD8, 8, -1);
		if(Controll == -1)
			return Coord;

		Coord = *(TouchCoord *)(Controll + 0x24);

		return Coord;
	}

	u16 ACSystem::GetGameCoins() {
		static const u32 GetGCoins(0x6C9D58, 0, 0, 0, 0, 0, 0, 0);
		static FUNCTION func(GetGCoins);
		return func.Call<u16>();
	}
	//something with anim stage
	//6E6078(0xA15B5C, 0x30198E40, 1);

	void FUN_006ca018(undefined4 param_1)
	bool FUN_00293264(void)
	int FUN_006a6660(uint param_1)
	void FUN_00133a20(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

	0BFB420


	bool ACSystem::SetGameCoins(int gamecoins) {
		
		u16 CurrentGC = ACSystem::GetGameCoins();
		FUN(CurrentGC);	

		static const u32 SubGC(0x34CAA0, 0, 0, 0, 0, 0, 0, 0);
		Process::Write32(SubGC, 0xE0800004);

		0xBFB420

		bool res = FUN(gamecoins);
//BFB420 (330BB950, x)
		Process::Write32(SubGC, 0xE0400004);
		return res; //0x953AE8
		static const u32 SetGCoins(0x6C9DA8, 0, 0, 0, 0, 0, 0, 0);
		static FUNCTION func(SetGCoins);
		return func.Call<bool>(gamecoins);
	}*/
//32DF6790

/*
Light Switch cheats I made for fun 
*/

	void FUN_0056A2C8(u32 param_1, u32 param_2) {
		*(u32 *)(param_1 + 8) = param_2;
		*(u32 *)(param_1 + 0xC) = param_2;

		if(*(u32 *)(param_1 + 0x18) != 0) {
			*(u32 *)(*(u32 *)(param_1 + 0x18) + 0x10) = param_2;
		}
	}

	u32 LightSwitch::GetData() {
		static const Address data(0x951030);
		return *(u32 *)data.addr;
	}

	void LightSwitch::SetData(bool ON) {
		static Address data1(0x569118);
		static Address data2(0x5696F8);
		static Address data3(0x56946C);

		u32 switchData = GetData();
		if(switchData == 0) {
			return;
		}

		data1.Call<void>(switchData + 0x38, switchData + 0x3F4, *(u32 *)(switchData + 0x488), 0);

		FUN_0056A2C8(switchData + 0x3F4, ON ? 0x3F800000 : 0);

		data2.Call<void>(switchData + 0x38);

		data3.Call<void>(switchData + 0x38, switchData + 0x3F4, *(u32 *)(switchData + 0x488), 0);
	}

	void LightSwitch::ON(u8 roomID) {
		static Address lightON(0x1E6844);
		lightON.Call<void>(0, roomID);
		SetData(true);

		if(Game::IsGameInRoom(roomID)) {
			Game::PlaySound(0x4F3 + IsBasement());
		}
	}

	void LightSwitch::OFF(u8 roomID) {
		static Address lightOFF(0x1E7514);
		lightOFF.Call<void>(0, roomID);
		SetData(false);

		if(Game::IsGameInRoom(roomID)) {
			Game::PlaySound(0x4F5);
		}
	}

	bool LightSwitch::IsON(u8 roomID) {
		static Address lightIsON(0x1E7AC0);
		return lightIsON.Call<bool>(roomID);
	}

	bool LightSwitch::IsBasement(u8 roomID) {
		static Address lightBasement(0x1E8400);
		return lightBasement.Call<bool>(roomID);
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

		if(!((currInit != 0) && (currInit < 0x66))) {//0 is invalid Initiative | 0x65 is highest Initiative 
			return;
		}

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