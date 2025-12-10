#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
	struct ID_U8Data {
		const char* Name; //name of ID
		u8 ID; //ID
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

	enum class SpecieID : u8 {
		Cat = 0, Elephant, Sheep, Bear,
		Dog, Squirrel, Rabbit, Duck,
		Hippo, Wolf, Mouse, Pig,
		Chicken, Bull, Cow, Bird,
		Frog, Alligator, Goat, Tiger,
		Anteater, Koala, Horse, Octopus,
		Lion, Bearcub, Rhinocero, Gorilla,
		Ostrich, Kangaroo, Eagle, Penguin,
		Monkey, Hamster, Deer, Special
	};

	struct AmiiboInfo {
		u32 ID0; //Amiibo ID0 for spoofing
		u32 ID1; //Amiibo ID1 for spoofing
		u16 VID; //VID for Caravan and other stuff
		SpecieID Species; //ID of NPC's species to differentiate NPCs
	};

	struct SPAmiiboInfo {
		u32 ID0; //Amiibo ID0 for spoofing
		u32 ID1; //Amiibo ID1 for spoofing
		u16 VID; //VID for Caravan and other stuff
		u8  SPVID;
	};

	struct PACKED_AmiiboInfo {
		std::string Name;
		u32 ID0; //Amiibo ID0 for spoofing
		u32 ID1; //Amiibo ID1 for spoofing
		u16 VID; //VID for Caravan and other stuff
	};

	void ValidKeyboardCheck(Keyboard& keyboard, KeyboardEvent& event);
	void ItemChange(Keyboard& keyboard, KeyboardEvent& event);
	void TextItemChange(Keyboard& keyboard, KeyboardEvent& event);
	extern const ID_U8Data Buildings[205];
	extern const SPAmiiboInfo amiiboSPVillagers[55];
    extern const AmiiboInfo amiiboVillagers[399];
	extern const ID_U8Data Music[256];

	namespace IDList {
		bool						IsHalfAcre(u8 acreID);
		bool 						RoomValid(u8 roomID);
		bool 						MenuValid(u8 MenuID);
		bool						BuildingValid(u8 buildingID);
		bool 						MusicValid(u16 musicID);
		bool 						AnimationValid(u8 animID, u8 playerIndex = 4);
		bool 						SnakeValid(u16 snakeID);
		bool 						EmotionValid(u8 emotionID);
		std::string 				GetBuildingName(u8 ID);
		std::string					GetMusicName(u16 musicID);
		std::string 				GetRoomName(u8 ID);
		bool		 				ValidID(u16 ID, u16 StardID, u16 EndID);
	}
}