#include "Helpers/AnimData.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Address/Address.hpp"


namespace CTRPluginFramework {
    u16 Calculate16BitFloat(float coord) {
		u16 res = static_cast<u16>(coord * 4.0); 
		return res;
	}
    
    void AnimData::Init(u32 animInst, u32 playerInst, u8 pIndex) {
        AnimData::animInst = animInst;
        AnimData::playerInst = playerInst;
        AnimData::pIndex = pIndex;
    }

    void AnimData::AppendCoordData(u32 animInst, u32 pos, float *Coord) {
        static Address coordoffset(0x5D4C88);
		coordoffset.Call<void>(animInst + pos, Coord);
    }
    
//Animation Function
	bool AnimData::ExecuteAnimation(u8 animID) {
		return Address(0x64DB90).Call<bool>(playerInst, animID, animInst, 0);
	}

    void AnimData::FaceCutoutStandee_C1(u8 option) {
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
        AppendAnimData<u8>(animInst, 0x12, 0);
        AppendAnimData<u8>(animInst, 0x13, option);
    }

    void AnimData::Pick_3C_3D(u8 dropID, Item itemID, Coord worldCoords) {
        AppendAnimData<Item>(animInst, 0xE, itemID);
        AppendAnimData<u8>(animInst, 0x12, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0x13, worldCoords.wY);
        AppendAnimData<u8>(animInst, 0x15, dropID);
        AppendCoordData(animInst, 0x17, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::Null_D9_75() {
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::Faint_9D() {
        AppendCoordData(animInst, 0xF, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::DoorOpen_44() {
        AppendCoordData(animInst, 0x10, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

//684480(pInstance, hairstyleID, haircolorID, eyecolorID, miihat, u0 = 3)
//Research about this one a bit
    void AnimData::Appearance_B9(Item animItem, u8 appearance[]) {
        AppendAnimData<u8>(animInst, 0xC, 0x10);
        AppendAnimData<u8>(animInst, 0xD, 0);
    //Write Appearance to Animation
        AppendAnimData<u8>(animInst, 0xE, appearance[0]);
        AppendAnimData<u8>(animInst, 0xF, appearance[1]);
        AppendAnimData<u8>(animInst, 0x10, appearance[2]);
    //Writes Item to animation //used for mii head
        AppendAnimData<u16>(animInst, 0x11, animItem.ID);
    }

    void AnimData::OpenMailBox_8B_DE() {
        AppendAnimData<u32>(animInst, 0x12, 0x528000 >> 8);
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::CloseMailBox_8C_DF() {
        AppendAnimData<u32>(animInst, 0x12, 0x8000 >> 8);
        AppendAnimData<u8>(animInst, 0x13, 1);
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::BuryItem_53_52(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);

        AppendAnimData<u32>(animInst, 0x10, playerInst);

        AppendAnimData<u8>(animInst, 0x11, 1);
        AppendAnimData<u8>(animInst, 0x16, 1);
    }

    void AnimData::Bury_4F(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);

        AppendAnimData<u16>(animInst, 0x10, 0x7FFE);
        AppendAnimData<u8>(animInst, 0x14, 0);
        AppendAnimData<u8>(animInst, 0x15, 0);
        AppendAnimData<u8>(animInst, 0x16, 0);
        AppendCoordData(animInst, 0x17, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::BuryOut_50_51(Item animItem, Coord worldCoords) {
        AppendAnimData<u8>(animInst, 5, 0);
        AppendAnimData<u8>(animInst, 0x16, 1);

        AppendAnimData<Item>(animInst, 0x10, animItem);

        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);

        AppendAnimData<u8>(animInst, 0x15, 0);

        AppendCoordData(animInst, 0x17, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::BeeSting_A7(bool gets_stung) {
        AppendAnimData<u8>(animInst, 0xE, gets_stung);
    }

    void AnimData::Emotion_AF_88(u8 emotion) {
        AppendAnimData<u8>(animInst, 0xE, emotion & 0xFF);
        AppendAnimData<u8>(animInst, 0xF, 1);
        //AppendAnimData<u8>(animInst, 0x15, (u8)4);
    }

    void AnimData::Null_14(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0x14, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0x15, worldCoords.wY);

        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::SitDown_79() {
        AppendCoordData(animInst, 0xF, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::SitDown_7A() {
        AppendCoordData(animInst, 0x10, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::StandUp_AC() {
        AppendAnimData<u32>(animInst, 0x17, *(u32 *)(playerInst + 0x8D3) << 8);
        AppendAnimData<u8>(animInst, 0x18, 2);

        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
        AppendCoordData(animInst, 0x13, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::ThrowFishRod_B0() {
        AppendCoordData(animInst, 0x14, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::Null_E1_62() {
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::ShovelHit_4C(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);
        AppendAnimData<u8>(animInst, 0x10, 1);
			
	//Rock Break
        AppendAnimData<u8>(animInst, 0x11, 1); //rock hit
        AppendAnimData<u8>(animInst, 0x1A, 1); //rock break

        AppendCoordData(animInst, 0x12, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    }

    void AnimData::ToyHammerHit_9C(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, 2);
        AppendAnimData<u32>(animInst, 0xF, *(u32 *)(playerInst + 0x5A8));

        AppendCoordData(animInst, 0x12, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    
        u16 var1 = *(u16 *)(playerInst + 0x2E);
        AppendAnimData<u8>(animInst, 0x17, (char)((int)var1 >> 8));
    }

    void AnimData::Congrats_2A() {
        AppendAnimData<u32>(animInst, 0xE, 0xFD041C);
        AppendAnimData<u32>(animInst, 0x12, 0x208800);
        AppendAnimData<u32>(animInst, 0x16, 0);
        AppendAnimData<u32>(animInst, 0x1A, 0x40000FE);
        AppendAnimData<u32>(animInst, 0x1E, 0x7F00000C);
        AppendAnimData<u32>(animInst, 0x22, 0xB32300);

        Process::Write32(playerInst + 0x146C, 0);
    }

    void AnimData::SwitchLight_69(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);

        AppendCoordData(animInst, 0x10, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    }

    void AnimData::Snake_C4(u16 snake, u16 sound) {
        AppendAnimData<u16>(animInst, 0xE, snake & 0xFFF);
        AppendAnimData<u16>(animInst, 0x10, sound & 0xFFFF);

        AppendAnimData<u8>(animInst, 0x13, 1);
        AppendAnimData<u16>(animInst, 0x15, 0xFFFE);
        AppendAnimData<u8>(animInst, 0x17, 4);
        AppendAnimData<u8>(animInst, 0x18, *(u8 *)(playerInst + 0x2F));
    }

    void AnimData::MoonJump_C4() {
        AppendAnimData<u16>(animInst, 0xA, 0xFFFF);
        AppendAnimData<u16>(animInst, 0xC, 0x7F);
        AppendAnimData<u16>(animInst, 0xE, 0xC);
        AppendAnimData<u16>(animInst, 0x10, 0x38C);

        AppendAnimData<u8>(animInst, 0x13, 0);
        AppendAnimData<u16>(animInst, 0x15, 0xFFFE);
        AppendAnimData<u8>(animInst, 0x17, 4);
        AppendAnimData<u8>(animInst, 0x18, *(u8 *)(playerInst + 0x2F));
    }

    void AnimData::Null_C5(u16 snake) {
        AppendAnimData<u16>(animInst, 0xE, snake & 0xFFF);
        AppendAnimData<u16>(animInst, 0x10, snake & 0xFFFF);

        AppendAnimData<u8>(animInst, 0x12, *(u8 *)(playerInst + 0x2F));
        AppendAnimData<u8>(animInst, 0x13, 1);
        AppendAnimData<u8>(animInst, 0x17, 5);
    }

    void AnimData::KnockDoor_45(u16 sound) {
        AppendAnimData<u16>(animInst, 0xE, sound & 0xFFF);
        AppendAnimData<u8>(animInst, 0x11, 1);

        AppendCoordData(animInst, 0x12, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::UnequipTool_3B(u8 emotion) {
        AppendAnimData<u8>(animInst, 0xF, emotion & 0xFF);
    }

    void AnimData::HoldTool_3C(Item animItem, u8 emotion, Coord worldCoords) {
        AppendAnimData<Item>(animInst, 0xE, animItem);

        AppendAnimData<u8>(animInst, 0x12, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0x13, worldCoords.wY);

        AppendAnimData<u8>(animInst, 0x15, emotion & 0xFF);
    }

    void AnimData::ShovelHitsNothing_4A(u8 emotion, Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);

        AppendAnimData<u8>(animInst, 0x10, emotion & 0xFF);
    }

    void AnimData::ChangeOutfit_36(Item animItem, u8 emotion) {
        AppendAnimData<Item>(animInst, 0xE, animItem);
        AppendAnimData<u8>(animInst, 0x12, emotion & 0xFF); //Not emotion, position of outfit
        AppendAnimData<u32>(animInst, 0x1C, 0x01480000);
    }  

    void AnimData::SitDown_7B_7C_7D_7E_7F(Coord worldCoords) {
        AppendCoordData(animInst, 0xF, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    
        AppendAnimData<u8>(animInst, 0x13, *(u8 *)(playerInst + 0x2F));
    }
//bruh what a long name
    void AnimData::Null_1C_25_1D_E2_E5_A8_A9_AA_AB_AD_AE_80_81_82_83_84_85_86_87_2B_2C_2D_2E_2F_30_31_32(u8 emotion, Coord worldCoords) {
        AppendCoordData(animInst, 0x2, PlayerClass::GetInstance(pIndex)->GetCoordinates());
        
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    
        AppendAnimData<u8>(animInst, 0x12, *(u8 *)(playerInst + 0x2F));
        AppendAnimData<u8>(animInst, 0x13, emotion & 0xFF);
    }

    void AnimData::FishFall_B3_B4_B5(Item animItem, Coord worldCoords) {
        AppendCoordData(animInst, 0xE, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    
        AppendAnimData<Item>(animInst, 0x12, animItem);
    }

    void AnimData::Null_95_33(Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0x15, 1);
        
        AppendCoordData(animInst, 0xF, PlayerClass::GetInstance(pIndex)->GetCoordinates(worldCoords.wX, worldCoords.wY));
    }

    void AnimData::PickUp_3E(Item animItem, u16 snake) {
        AppendAnimData<u16>(animInst, 0xE, animItem.ID);
        AppendAnimData<u16>(animInst, 0x10, animItem.Flags);
        AppendAnimData<u16>(animInst, 0x12, snake & 0xFFF);
        AppendAnimData<u32>(animInst, 0x14, 0x20101);
    }

    void AnimData::EquipItem_38(Item animItem) {
        AppendAnimData<u16>(animInst, 0xE, animItem.ID);
        AppendAnimData<u16>(animInst, 0x10, animItem.Flags);
        AppendAnimData<u8>(animInst, 0x12, 1);
    }

    void AnimData::PickPluck_41_42(Item animItem) {
        AppendAnimData<u16>(animInst, 0xE, animItem.ID);
        AppendAnimData<u16>(animInst, 0x10, animItem.Flags);
        AppendAnimData<u8>(animInst, 0x14, 3);

        AppendCoordData(animInst, 0x15, PlayerClass::GetInstance(pIndex)->GetCoordinates());
    }

    void AnimData::Null_B7_71_72_24_3F_3D_92_93_94(Item animItem) {
        AppendAnimData<u16>(animInst, 0xE, animItem.ID);
        AppendAnimData<u16>(animInst, 0x10, animItem.Flags);
    }

    void AnimData::Null_Default(Item animItem, Coord worldCoords) {
        AppendAnimData<u8>(animInst, 0xE, worldCoords.wX);
        AppendAnimData<u8>(animInst, 0xF, worldCoords.wY);

        AppendAnimData<u16>(animInst, 0x10, animItem.ID);
    }
}
