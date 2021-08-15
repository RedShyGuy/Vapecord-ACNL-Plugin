#include <CTRPluginFramework.hpp>
#include "RegionCodes.hpp"
#include "cheats.hpp"

namespace CTRPluginFramework {
	void Code::Load(void) {
	//Country Offset to change country		
		country = Region::AutoRegion(0x350B18, 0x35052C, 0x34FC60, 0x34FC60, 0x34F8F8, 0x34F8F8, 0x0034F7BC, 0x34F7BC);
	//offset and OFF-Value for flowers		
		UnbreakableFlower = Region::AutoRegion(0x597F58, 0x597470, 0x596FA0, 0x596FA0, 0x596890, 0x596890, 0x596564, 0x596564);
		v_UnbreakableFlower = *(u32 *)UnbreakableFlower;
	//FOV offset		
		fov = Region::AutoRegion(0x47E48C, 0x47DE04, 0x47D4D4, 0x47D4D4, 0x47D16C, 0x47D16C, 0x47D02C, 0x47D02C);	
	//No Save Offsets
		nosave = Region::AutoRegion(0x1A0980, 0x1A03C8, 0x1A09A0, 0x1A09A0, 0x1A08E0, 0x1A08E0, 0x1A08E0, 0x1A08E0);
	//Pickup Offsets for correct item change
		CloseHoleItem = Region::AutoRegion(0x598DBC, 0x5982D4, 0x597E04, 0x597E04, 0x5976F4, 0x5976F4, 0x5973C8, 0x5973C8);
		PickupItem = Region::AutoRegion(0x59A268, 0x599780, 0x5992B0, 0x5992B0, 0x598BA0, 0x598BA0, 0x598874, 0x598874);
	//Offsets for Drop Mod
		DropPattern = Region::AutoRegion(0x85FE58, 0x85EE50, 0x85ECE4, 0x85ECE4, 0x859104, 0x858104, 0x8580B4, 0x8580B4);
		dropm2 = Region::AutoRegion(0x30B83C, 0x30B6DC, 0x30B820, 0x30B820, 0x30B844, 0x30B844, 0x30B7EC, 0x30B7EC);
		dropm3 = Region::AutoRegion(0x19CFFC, 0x19CA44, 0x19D01C, 0x19D01C, 0x19CF5C, 0x19CF5C, 0x19CF5C, 0x19CF5C);
		dropm4 = Region::AutoRegion(0x660430, 0x66FDC8, 0x66F8D8, 0x66F8D8, 0x66F398, 0x65EF28, 0x66EF40, 0x66EF40);
		dropm5 = Region::AutoRegion(0x59771C, 0x596C34, 0x596764, 0x596764, 0x596054, 0x596054, 0x595D28, 0x595D28);
		dropm6 = Region::AutoRegion(0x599348, 0x598860, 0x598390, 0x598390, 0x597C80, 0x597C80, 0x597954, 0x597954);
		dropm7 = Region::AutoRegion(0x59915C, 0x598674, 0x5981A4, 0x5981A4, 0x597A94, 0x597A94, 0x597768, 0x597768);
		dropm8 = Region::AutoRegion(0x599248, 0x598760, 0x598290, 0x598290, 0x597B80, 0x597B80, 0x597854, 0x597854);
		dropm9 = Region::AutoRegion(0x5991E8, 0x598700, 0x598230, 0x598230, 0x597B20, 0x597B20, 0x5977F4, 0x5977F4);
	//Player Select Offset
		playselector = Region::AutoRegion(0x75F010, 0x75DFF4, 0x75E018, 0x75DFF0, 0x75D7B0, 0x75D788, 0x75D358, 0x75D358);
	//Drop Item Function
		PlaceItemOffset = Region::AutoRegion(0x59FC7C, 0x59F194, 0x59ECC4, 0x59ECC4, 0x59E5B4, 0x59E5B4, 0x59E288, 0x59E288);
	//Unlocks camera
		unlockcamera = Region::AutoRegion(0x1A51C8, 0x1A4C10, 0x1A51E8, 0x1A51E8, 0x1A5128, 0x1A5128, 0x1A5128, 0x1A5128);
	//ingame write func
		CopyPasteFunc = Region::AutoRegion(0x2FC978, 0x2FC8E8, 0x2FC8B8, 0x2FC8B8, 0x2FC878, 0x2FC878, 0x2FC8E0, 0x2FC8E0);
	//Load item icon
		LoadIcon = Region::AutoRegion(0x26DC00, 0x26D644, 0x26DBFC, 0x26DBFC, 0x26DB1C, 0x26DB1C, 0x26DAE8, 0x26DAE8);
	//change index
		IndexChange = Region::AutoRegion(0x628B54, 0x62807C, 0x627B8C, 0x627B8C, 0x62764C, 0x62764C, 0x6271F4, 0x6271F4);
	//anim cond patch
		AnimConditionPatch = Region::AutoRegion(0x677504, 0x676A2C, 0x67653C, 0x67653C, 0x675FFC, 0x675FFC, 0x675BA4, 0x675BA4);
	//get player save data
		PlayerSave = Region::AutoRegion(0x2FBA60, 0x2FB96C, 0x2FBAE8, 0x2FBAE8, 0x2FB920, 0x2FB920, 0x2FB8F8, 0x2FB8F8);
	//get specific save data
		LoadedPlayerSave = Region::AutoRegion(0x2FEB2C, 0x2FE8A8, 0x2FEBB4, 0x2FEBB4, 0x2FEB60, 0x2FEB60, 0x2FEA98, 0x2FEA98);

		PlayerInstance = Region::AutoRegion(0x5C3EA0, 0x5C33D0, 0x5C2EE8, 0x5C2EE8, 0x5C27D8, 0x5C27D8, 0x5C24AC, 0x5C24AC);	

		PlayerInstanceRefValue = *(u32 *)(PlayerInstance + 0x10);

		DisableChatRemoval = Region::AutoRegion(0x1939F0, 0x193438, 0x193A10, 0x193A10, 0x193960, 0x193960, 0x193960, 0x193960);

		DisableChatRemovalVal = *(u32 *)(DisableChatRemoval);

		a_GetOnlinePlayerIndex = Region::AutoRegion(0x305EF0, 0x305F44, 0x305EF0, 0x305EF0, 0x305F6C, 0x305F6C, 0x305F3C, 0x305F3C);

		v_GetOnlinePlayerIndex = *(u32 *)(a_GetOnlinePlayerIndex + 8);

		AnimFunction = Region::AutoRegion(0x64DB90, 0x64D0B8, 0x64CBC8, 0x64CBC8, 0x64C688, 0x64C688, 0x64C230, 0x64C230);

	//Island Pointers
		IslPointer = Region::AutoRegion(0x953708, 0x9526F8, 0x952704, 0x952704, 0x94C704, 0x94B704, 0x94B704, 0x94B704);
		Isl2ndPointer = Region::AutoRegion(0x94FD58, 0x94ED48, 0x94ED58, 0x94ED58, 0x948D58, 0x947D58, 0x947D58, 0x947D58);
	//7FFE Pointer
		Pointer7FFE = Region::AutoRegion(0x95CFFC, 0x95BFDC, 0x95BFF4, 0x95BFF4, 0x955FF4, 0x954FF4, 0x954FF4, 0x954FF4); 
	//Game/Chat/Inv Pointers		
		GamePointer = Region::AutoRegion(0x954648, 0x953630, 0x953648, 0x953648, 0x94D648, 0x94C648, 0x94C648, 0x94C648);
		chatpointer = Region::AutoRegion(0x94FD84, 0x94ED74, 0x94ED84, 0x94ED84, 0x948D84, 0x947D84, 0x947D84, 0x947D84);
		InvMenu = Region::AutoRegion(0x98D500, 0x98C500, 0x98C500, 0x98C500, 0x986500, 0x985500, 0x985500, 0x985500);
	//Max Turbo Pointer	
		max = Region::AutoRegion(0x95D3FC, 0x95C3DC, 0x95C3F4, 0x95C3F4, 0x9563F4, 0x9553F4, 0x9553F4, 0x9553F4);
	//Map Boolean
		MapBool = Region::AutoRegion(0x950C30, 0x94FC20, 0x94FC30, 0x94FC30, 0x949C30, 0x948C30, 0x948C30, 0x948C30); 
	//Catalog To Pockets Offset		
		cooffset = Region::AutoRegion(0x950D1F, 0x94FD0F, 0x94FD1C, 0x94FD1F, 0x949D1C, 0x948D1F, 0x948D1F, 0x948D1F);
		cioffset = Region::AutoRegion(0x95033C, 0x94F32C, 0x94F33C, 0x94F33C, 0x94933C, 0x94833C, 0x94833C, 0x94833C);

		ChatPoint = Region::AutoRegion(0x95F11C, 0x95E0FC, 0x95E114, 0x95E114, 0x958114, 0x957114, 0x957114, 0x957114);
	}
	
	u32		Code::IslPointer = 0;
	u32		Code::Isl2ndPointer = 0;
	u32		Code::Pointer7FFE = 0;
	u32		Code::GamePointer = 0;
	u32		Code::chatpointer = 0;
	u32		Code::InvMenu = 0;
	u32		Code::max = 0;
	u32		Code::MapBool = 0;
	u32		Code::cooffset = 0;
	u32		Code::cioffset = 0;
	u32		Code::country = 0;
	u32		Code::UnbreakableFlower = 0;
	u32		Code::v_UnbreakableFlower = 0;
	u32		Code::fov = 0;
	u32		Code::nosave = 0;
	u32		Code::CloseHoleItem = 0;
	u32		Code::PickupItem = 0;
	u32		Code::DropPattern = 0;
	u32		Code::dropm2 = 0;
	u32		Code::dropm3 = 0;
	u32		Code::dropm4 = 0;
	u32		Code::dropm5 = 0;
	u32		Code::dropm6 = 0;
	u32		Code::dropm7 = 0;
	u32		Code::dropm8 = 0;
	u32		Code::dropm9 = 0;
	u32		Code::playselector = 0;
	u32		Code::PlaceItemOffset = 0;
	u32		Code::unlockcamera = 0;
	u32		Code::CopyPasteFunc = 0;
	u32 	Code::LoadIcon = 0;
	u32		Code::IndexChange = 0;
	u32		Code::AnimConditionPatch = 0;
	u32		Code::PlayerSave = 0;
	u32		Code::LoadedPlayerSave = 0;
	u32		Code::PlayerInstance = 0;
	u32		Code::PlayerInstanceRefValue = 0;
	u32 	Code::ChatPoint = 0;
	u32		Code::DisableChatRemoval = 0;
	u32		Code::DisableChatRemovalVal = 0;
	u32		Code::a_GetOnlinePlayerIndex = 0;
	u32		Code::v_GetOnlinePlayerIndex = 0;
	u32		Code::AnimFunction = 0;
}