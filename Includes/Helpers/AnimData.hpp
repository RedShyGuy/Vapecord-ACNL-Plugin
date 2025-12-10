#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
	struct Coord {
		u8 wX;
		u8 wY;
	};

	u16 Calculate16BitFloat(float coord);

	/*void Calculate_unknown0(AnimationData data, u8 u1, bool u2) {
		u8 *pbVar1;
		u8 bVar2;
		
		pbVar1 = (u8 *)(data.unknown0 + (u1 >> 3));
		bVar2 = (u8)(1 << (u1 & 7));

		if(!u2) {
			if(u1 < 0xB) {
				bVar2 = *pbVar1 & ~bVar2;
				*pbVar1 = bVar2;
			}
		}
		else {
			if(u1 < 0xB) {
				bVar2 = *pbVar1 | bVar2;
				*pbVar1 = bVar2;
			}
		}
	}

	void Calculate_unknown1(AnimationData data) {
		if((data.unknown0 << 0x1A) < 0) || (iVar6 = FUN_006E5794(sVar2), iVar6 != 0)) || (sVar2 == 0x11)) {
			//*(u8 *)(param_1 + 0x173) = *(u8 *)(param_1 + 0x173) | 2;
			bVar3 = data.unknown1 | 4;
		}
		else 
			bVar3 = data.unknown1 & 0xFB;

		data.unknown1 = bVar3;
	}*/

	struct AnimationData {
		u8 roomID;
        u8 animID;
        u16 xCoordSH; //16Bit Float
        u16 zCoordSH; //16Bit Float
        u16 rotationSH; //16Bit Float (Rotation is already a 16Bit Float)
        u8 animCount = 0; //How many animations have been executed
        u8 unknown0 = 1; //standard anim func makes it always 1
        u8 unknown1 = 0; //standard anim func makes it always 0
        u8 unknown2 = 8; //standard anim func makes it always 8
        u16 unknown3 = 0; //seems to be always 0 written to it

		u8 data[0x20];
	};

	class AnimData {
		public:
			void    	Faint_9D();
			void		DoorOpen_44();
			void		Null_D9_75();
			void		Pick_3C_3D(u8 dropID, Item itemID, Coord worldCoords);

			void		FaceCutoutStandee_C1(u8 option);
			void    	Appearance_B9(Item animItem, u8 appearance[] = nullptr);
			void		OpenMailBox_8B_DE();
			void		CloseMailBox_8C_DF();
			void		BuryItem_53_52(Coord worldCoords);
			void		Bury_4F(Coord worldCoords);
			void		BuryOut_50_51(Item animItem, Coord worldCoords);
			void		BeeSting_A7(bool gets_stung = 1);
			void		Emotion_AF_88(u8 emotion);
			void		Null_14(Coord worldCoords);
			void		SitDown_79();
			void		SitDown_7A();
			void		StandUp_AC();
			void		ThrowFishRod_B0();
			void		Null_E1_62();
			void		ShovelHit(Coord worldCoords);
			void		ShovelHit_4C(Coord worldCoords);
			void		ToyHammerHit_9C(Coord worldCoords);
			void		Congrats_2A();
			void		SwitchLight_69(Coord worldCoords);
			void		Snake_C4(u16 snake, u16 sound);
			void		MoonJump_C4();
			void		Null_C5(u16 snake);
			void		KnockDoor_45(u16 sound);
			void		UnequipTool_3B(u8 emotion);
			void		HoldTool_3C(Item animItem, u8 emotion, Coord worldCoords);
			void		ShovelHitsNothing_4A(u8 emotion, Coord worldCoords);
			void		ChangeOutfit_36(Item animItem, u8 emotion);
			void		SitDown_7B_7C_7D_7E_7F(Coord worldCoords);
			void		Null_1C_25_1D_E2_E5_A8_A9_AA_AB_AD_AE_80_81_82_83_84_85_86_87_2B_2C_2D_2E_2F_30_31_32(u8 emotion, Coord worldCoords);
			void		FishFall_B3_B4_B5(Item animItem, Coord worldCoords);
			void		Null_95_33(Coord worldCoords);
			void		PickUp_3E(Item animItem, u16 snake);
			void		EquipItem_38(Item animItem);
			void		PickPluck_41_42(Item animItem);
			void		Null_B7_71_72_24_3F_3D_92_93_94(Item animItem);
			void		Null_Default(Item animItem, Coord worldCoords);

			void		Init(u32 animInst, u32 playerInst, u8 pIndex);
			bool		ExecuteAnimation(u8 animID);
			void		AppendCoordData(u32 animInst, u32 pos, float *Coord);

			template <typename Data>
			void AppendAnimData(u32 animInst, u32 pos, Data data) {
				*(Data *)(animInst + pos) = data;
			}

		private:
			u32 animInst;
			u32 playerInst;
			u8 pIndex;
	};
}