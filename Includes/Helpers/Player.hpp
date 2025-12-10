#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
	namespace Player {
		Color 			GetColor(u8 pIndex);

		void 			Load(int pIndex);
		void			ReloadDesign(int slot);
		void			StealDesign(u8 slot);
		void			UpdateStyle();
		void			UpdateTan();
		PlayerStatus 	GetPlayerStatus(u8 pPlayer);
		u32				GetSpecificSave(u8 pPlayer);
		u32				GetSaveOffset(u8 pIndex);
		void			WriteOutfit(u8 PlayerIndex, Item HeadWear, Item Glasses, Item Shirt, Item Pants, Item Socks, Item Shoes);
		void 			ClearMail(int slot);
		bool			Exists(u8 PlayerIndex);		
		float*		    GetCollisionSize(u8 PlayerIndex = 4);
		bool			IsIndoors();
		u8				GetRoom(u8 PlayerIndex);
		bool			IsInRoom(u8 room, u8 playerIndex = 4);

		ACNL_Player		*GetSaveData(u8 PlayerIndex = 4);
		bool 			SaveExists(ACNL_Player *player);

		bool 			SetUnlockableBitField(ACNL_Player *player, u8 ID, bool state);

		void 			EditGender(u8 pIndex, u8 pGender);
		void 			EditName(u8 pIndex, const std::string& name);
	};
}