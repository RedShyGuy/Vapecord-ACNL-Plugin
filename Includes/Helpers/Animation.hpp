#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/Game.hpp"

namespace CTRPluginFramework {
	extern bool IfForceAllowed;

	namespace Animation {
		void 		Idle(u8 pID = Game::GetOnlinePlayerIndex());
		void		SendAnimPacket(u8 senderIndex, u32 animObj, u8 animID, u8 roomID, u8 targetPlayerIndex);
		bool		ExecuteAnimationWrapper(u8 playerIndex, u8 animID, Item animItem, u8 emotion, u16 snake, u16 sound, bool u0, u8 wX, u8 wY, bool directSend = false, u8 appearance[] = nullptr);
		u32			GetAnimationInstance(u32 playerInstance, u8 u0, u8 u1, u32 u2);
	}
}