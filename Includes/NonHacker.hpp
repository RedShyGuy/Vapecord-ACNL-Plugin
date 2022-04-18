#ifndef NONHACKER_HPP
#define NONHACKER_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
	class NonHacker {
		public:
			NonHacker(u8 playerID);
			~NonHacker(void);

			u32 GetPlayerMessageData(void);
			std::string GetPlayerMessage(void);
			void ClearPlayerMessage(void);
			std::string GetPlayerName(void);
			u8 GetPlayerIndex(void);
			bool IsPlayerMessageOnScreen(void);

			void Animation(void);
			void Emotion(void);
			void Snake(void);
			void Music(void);
			void Item(void);

			bool IsCommand(const std::string& command, bool secondary = false);

			s8 pID = -1;
			std::string pMessage = "";
			std::string pName = "";

			u8 animID = 6;
			u8 emotionID = 1;
			u16 snakeID = 1;
			u16 musicID = 0x660;
			CTRPluginFramework::Item itemID = { 0x7FFE, 0 };

			static bool Accessible[5];
	};
	
	void NonHacker_Player00(void);
	void NonHacker_Player01(void);
	void NonHacker_Player02(void);
	void NonHacker_Player03(void);
}
#endif