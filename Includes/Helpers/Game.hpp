#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"
#include "Helpers/PlayerClass.hpp"

namespace CTRPluginFramework {
	struct c_RGBA {
		u8 R;
		u8 G;
		u8 B;
		u8 A;
	};

	namespace Building {
		ACNL_BuildingData 	*GetSaveData();
	}

	namespace Game {
		ACNL_Date 			GetCurrentDate(void);
		void				PlaySound(u16 soundID);
	//save files
		//u32				GetMail();
		//u32 				GetDesign();
		//u32				GetFriend();
		//u32 				GetExhibition();

		Item_Categories 	GetItemCategorie(Item itemID);
	//value display
		bool 				SetValueDisplay(u8 type);
		void 				ResetValueDisplay(void);
	//Building Stuff
		void				MoveBuilding();
		void				RemoveBuilding();
		void 				PlaceBuilding(u8 buildingID);
		void				PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID);
	//Money Stuff
		void 				SetBadges(u8 badge, u8 type, bool WithStats);
		int					DecryptValue(u64 *position);
		void				EncryptValue(u64 *position, int moneyamount);
	//Flower Converter
		void				ToIndoorFlowers(Item& input);
		void				ToOutdoorFlowers(Item& input);
	//Inventory Stuff
		u32 				MailPointer();
		u32					BaseInvPointer();
	//Locked Spot Stuff
		void				ClearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4 = 4);
		u32					CreateLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt = 1);
		u32					GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID = 0xA5);
	//Other Stuff	
		bool 				IsOutdoorItem(Item item);
		void				ReloadRoom(float *coords = PlayerClass::GetInstance()->GetCoordinates());
		void				Catalog(bool directcall = false);
		u32					RoomFunction(u8 room, bool u0, bool u1, bool u2);
		u8					GetOnlinePlayerIndex();
		u8					GetActualPlayerIndex();
		u8					GetOnlinePlayerCount();
		Item				*GetItemAtWorldCoords(u32 x, u32 y);
		u32					GetCurrentMap(void);	
		bool				RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything, bool counting = true);
		void				Particles(u32 particleID, float *floats);
		void				TrampleAt(u8 wX, u8 wY);

		bool				IsGameInRoom(u8 room);
		u8					GetRoom();
		u8					NextRoomCheck();
		bool				MapBoolCheck();
		bool				IsRoomLoading();
		bool				SetItem(Item *item);
		void				OpenMenu(u8 menuID, bool NoMenCall = false);
		void				ChangeGameType(u8 GameType);
		u8					GetGameType();
		bool				GameSaving();
		u32 				GetRoomData();
		bool				WaterFlower(u8 wX, u8 wY);
		void				SetCurrentTime(bool forward, int Minutes = 0, int Hours = 0, int Days = 0, int Months = 0, int Years = 0);
	}

	namespace Camera {
		u32			GetInstance();
		float* 		GetCoordinates();
		void		AddToX(float val);
		void		AddToY(float val);
		void		AddToZ(float val);
		void		AddToYRotation(u16 val);
	}

	template <typename A, typename V>
	bool IfInArray(A array, V var) {
		return(std::find(std::begin(array), std::end(array), var) != std::end(array));
	}

	template <typename Data>
	Data StringToHex(const std::string& str, Data returntype) {
		u32 val = 0;
		const u8* hex = (const u8*)str.c_str();
		
		while(*hex) {
			u8 byte = (u8)*hex++;

			if(byte >= '0' && byte <= '9') 
				byte = byte - '0';
			else if(byte >= 'a' && byte <= 'f') 
				byte = byte - 'a' + 10;
			else if(byte >= 'A' && byte <= 'F') 
				byte = byte - 'A' + 10;
			else if(byte == ' ') //skips space
				continue;
			else 
				return returntype; ///< Incorrect char

			val = (val << 4) | (byte & 0xF);
		}
		
		return val;
	}
}