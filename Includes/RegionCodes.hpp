#ifndef REGIONCODES_HPP
#define REGIONCODES_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	class Code {
	public:	
		static void     Load(void);
		
		static u32		IslPointer;
		static u32		Isl2ndPointer;
		static u32		Pointer7FFE;
		static u32		GamePointer;
		static u32		chatpointer;
		static u32		InvMenu;
		static u32		max;
		static u32		MapBool;
		static u32		cooffset;
		static u32		cioffset;
		static u32		country;
		static u32		UnbreakableFlower;
		static u32		v_UnbreakableFlower;
		static u32		fov;
		static u32		nosave;
		static u32		CloseHoleItem;
		static u32		PickupItem;
		static u32		DropPattern;
		static u32		dropm2;
		static u32		dropm3;
		static u32		dropm4;
		static u32		dropm5;
		static u32		dropm6;
		static u32		dropm7;
		static u32		dropm8;
		static u32		dropm9;
		static u32		playselector;
		static u32		PlaceItemOffset;
		static u32		unlockcamera;
		static u32		CopyPasteFunc;
		static u32		LoadIcon;
		static u32		IndexChange;
		static u32		AnimConditionPatch;
		static u32		PlayerSave;
		static u32		LoadedPlayerSave;
		static u32		PlayerInstance;
		static u32		PlayerInstanceRefValue;
		static u32		ChatPoint;
		static u32		DisableChatRemoval;
		static u32		DisableChatRemovalVal;
		static u32		a_GetOnlinePlayerIndex;
		static u32		v_GetOnlinePlayerIndex;
		static u32		AnimFunction;
	};
}

#endif