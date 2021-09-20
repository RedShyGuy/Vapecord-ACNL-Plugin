#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <CTRPluginFramework.hpp>

extern bool IsIndoorsBool;

namespace CTRPluginFramework {
	enum class PlayerStatus : u8 {
		Town_00 = 0,
		Town_01 = 1,
		Town_02 = 2,
		Town_03 = 3,
		Isl_00 = 4,
		Isl_01 = 5,
		Isl_02 = 6,
		Isl_03 = 7,
		Empty = 8
	};

	namespace Player {
		u32 			GetBulletin(int slot);
		void 			Load(int pIndex);
		void			ReloadDesign(int slot);
		u32				GetDesign(int slot, int pIndex = 4);
		void			SetDesign(int slot, std::string DesignName, u16 PlayerID, std::string PlayerName, u8 PlayerGender, u16 PlayerTownID, std::string PlayerTownName, u32 DesignCheck1, u8 DesignCheck2, u8 DesignType);
		void			UpdateStyle();
		void			UpdateTan();
		u8				GetMailSlot();
		PlayerStatus 	GetPlayerStatus(u8 pPlayer);
		u32				GetSpecificSave(u8 pPlayer);
		u32				GetSaveOffset(u8 pIndex);
		void			WriteOutfit(u8 PlayerIndex, u16 HeadWear, u16 Glasses, u16 Shirt, u16 Pants, u16 Socks, u16 Shoes);
		void			WriteAppearance(u8 PlayerIndex, u8 HairStyle, u8 HairColor, u8 EyeStyle, u8 EyeColor, u8 TanLevel, u8 Gender);
		void 			ClearMail(int slot);
		void 			SetTool(u16 tool);
		u16 			GetTool();
		bool			Exists(u8 PlayerIndex);		
		float*		    GetCollisionSize(u8 PlayerIndex = 4);
		bool			IsIndoors();
		u8				GetRoom(u8 PlayerIndex);
	};

	namespace PlayerName {
		void 				UpdateReference(u8 pIndex, const std::string& pName, u8 pGender);
		void				Set(const std::string& name);
		void 				SetDesigns(const std::string& name);
		void				SetLetters(const std::string& name);
		void				SetVillagers(const std::string& name);
	}
}
#endif
