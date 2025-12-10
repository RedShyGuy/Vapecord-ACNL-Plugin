#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/Game.hpp"

namespace CTRPluginFramework {
	namespace GameKey {
		enum GameKey {
			N			= 0,
			A           = 0x1,       
			B           = 0x2,
			Select      = 0x800, 
			Start       = 0x800, 
			DPadRight   = 0x80000,
			DPadLeft    = 0x40000,
			DPadUp      = 0x10000,
			DPadDown    = 0x20000,
			R           = 0x4000, 
			L           = 0x2000,  
			X           = 0x8,
			Y           = 0x10,
			ZL          = 0x300000,		//CUSTOM
       		ZR          = 0x500000,		//CUSTOM
			Touchpad    = 0x8000,
			CStickRight = 0x8000000,	//CUSTOM
			CStickLeft  = 0x4000000,	//CUSTOM
			CStickUp    = 0x1000000,	//CUSTOM
			CStickDown  = 0x2000000,	//CUSTOM
			CPadRight   = 0x800000, 
			CPadLeft    = 0x400000, 
			CPadUp      = 0x100000,
			CPadDown    = 0x200000,
		};
	}
	
	namespace ACSystem {
        bool				IsKeyDown(GameKey::GameKey key);
		//TouchCoord		GetTouch();
		//u16 				GetGameCoins();
		//bool 				SetGameCoins(int gamecoins);
	}

	namespace LightSwitch {
		u32 	GetData();
		void 	SetData(bool ON);
		void 	ON(u8 roomID = Game::GetRoom());
		void 	OFF(u8 roomID = Game::GetRoom());
		bool 	IsON(u8 roomID = Game::GetRoom());
		bool 	IsBasement(u8 roomID = Game::GetRoom());
	}

	namespace MEOW {
		u16 	GetInitiativesValue(int Initiative);
		u8 		GetCurrentInitiatives(u32 InitiativeSaveOffset, int InitPos, int weekDay);
		bool 	InitiativeFinished(u32 InitiativeSaveOffset, int Initiative);
		u32 	GetInitiativeOffset(u32 InitiativeSaveOffset, int Initiative);
		void 	ToggleInitiative(int pPos, int InitPos, int weekDay, bool finish);
	}
}