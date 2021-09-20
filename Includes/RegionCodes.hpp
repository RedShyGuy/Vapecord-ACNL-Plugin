#ifndef REGIONCODES_HPP
#define REGIONCODES_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
	class Code {
	public:	
		static void     Load(void);
		
		static Address		IslPointer;
		static Address		GamePointer;
		static Address		chatpointer;
		static Address		max;
		static Address		MapBool;
		static Address		country;
		static Address		UnbreakableFlower;
		static Address		v_UnbreakableFlower;
		static Address		fov;
		static Address		nosave;
		static Address		CloseHoleItem;
		static Address		PickupItem;
		static Address		DropPattern;
		static Address		dropm2;
		static Address		dropm3;
		static Address		dropm4;
		static Address		dropm5;
		static Address		dropm6;
		static Address		dropm7;
		static Address		dropm8;
		static Address		dropm9;
		static Address		playselector;
		static Address		PlaceItemOffset;
		static Address		unlockcamera;
		static Address		CopyPasteFunc;
		static Address		LoadIcon;
		static Address		IndexChange;
		static Address		AnimConditionPatch;
		static Address		PlayerSave;
		static Address		LoadedPlayerSave;
		static Address		PlayerInstance;
		static Address		PlayerInstanceRefValue;
		static Address		ChatPoint;
		static Address		DisableChatRemoval;
		static Address		DisableChatRemovalVal;
		static Address		a_GetOnlinePlayerIndex;
		static Address		v_GetOnlinePlayerIndex;
		static Address		AnimFunction;
		static Address		SetupStackData;
		static Address		DataPointer;
	};
}

#endif