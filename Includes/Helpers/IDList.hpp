#ifndef IDLIST_HPP
#define IDLIST_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	struct IDS1 {
		const char* Name; //name of ID
		u8 ID; //ID
	};

	struct IDS2 {
		const char* Name; //name of ID
		u16 ID; //ID
	};

	struct IDS3 {
		const char* Name; //name of ID
		u32 ID; //ID
	};

	struct Furniture {
		std::string Name;
		u16 Items[16];
	};

	struct Range {
		u32 start;
		u32 end;
	};

	class KeyRange {
		public:
			static void Set(Range range) {
				sRange = range;
			}
			static Range Get(void) {
				return sRange;
			}

			static Range sRange;
	};

	void ValidKeyboardCheck(Keyboard& keyboard, KeyboardEvent& event);
	void ItemChange(Keyboard& keyboard, KeyboardEvent& event);

	namespace IDList {
		bool						IsHalfAcre(u8 acreID);
		bool 						RoomValid(u8 roomID);
		bool 						MenuValid(u8 MenuID);
		bool						BuildingValid(u8 buildingID);
		bool						ToolsValid(u16 toolsID);
		bool 						ItemValid(u32 itemID, bool IsDropped = true);
		bool 						MusicValid(u16 musicID);
		bool 						AnimationValid(u8 animID);
		bool 						SnakeValid(u16 snakeID);
		bool 						EmotionValid(u8 emotionID);
		std::string 				GetSeedName(u16 itemID);
		std::string  				SetCountryName(u8 country);
		std::string 				GetBuildingName(u8 ID);
		std::string 				GetRoomName(u8 ID);
		std::string					GetItemName(u16 ItemID);
		std::string 				GetNNPCName(u16 VID);
		std::string 				GetSPNPCName(u8 SPVID);
		bool		 				ValidID(u16 ID, u16 StardID, u16 EndID);
		bool 						PopulateNPCAmiibo(SpecieID specieID, std::vector<std::string> &vec, std::vector<PACKED_AmiiboInfo> &info, bool HoldenFillyAllowed = false, bool NonCaravanAllowed = true);
	}
}
#endif

