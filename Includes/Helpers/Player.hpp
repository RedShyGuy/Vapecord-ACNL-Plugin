#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

extern bool IsIndoorsBool;

namespace CTRPluginFramework {
	namespace Player {
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

		ACNL_Player		*GetSaveData(u8 PlayerIndex = 4);
		bool 			SaveExists(ACNL_Player *player);

		bool 			SetUnlockableBitField(ACNL_Player *player, u8 ID, bool state);

		void 			EditGender(u8 pIndex, u8 pGender);
		void 			EditName(u8 pIndex, const std::string& name);
	};
}
#endif
