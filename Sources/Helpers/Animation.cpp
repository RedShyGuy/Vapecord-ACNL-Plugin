#include "Helpers/Animation.hpp"
#include "Helpers/AnimData.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Player.hpp"
#include "Address/Address.hpp"


namespace CTRPluginFramework {
	bool IfForceAllowed = true;
//Get Animation Instance
	u32 Animation::GetAnimationInstance(u32 playerInstance, u8 someVal1, u8 someVal2, u32 encVal) {
		static Address getaniminst(0x6576F8);
		return getaniminst.Call<u32>(playerInstance, someVal1, someVal2, encVal);
	} 

//Animation Wrapper	
	bool Animation::ExecuteAnimationWrapper(u8 pIndex, u8 animID, Item animItem, u8 emotion, u16 snake, u16 sound, bool u0, u8 wX, u8 wY, bool directSend, u8 appearance[]) {	
	//Gets actual PlayerIndex
		u8 AIndex = Game::GetActualPlayerIndex();
		
	//If PlayerIndex is not the same as yours
		bool forced = (pIndex != AIndex) && (AIndex <= 3);
	//This gets used if the player you want to force is not the player you selected
		bool needOverwrite = (Game::GetOnlinePlayerIndex() != pIndex);
		
	//Gets Player Instance
		u32 playerInstance = PlayerClass::GetInstance(pIndex)->Offset();
		
	//If selected player is not loaded return false
		if(!PlayerClass::GetInstance(pIndex)->IsLoaded()) {
			return 0;
		}
		
	//Gets Animation Instance to append anim data
		u32 animInstance = Animation::GetAnimationInstance(playerInstance, 0, 0, 0);
		
	//If animation instance returns 0, probably means animation can't be executed
		if(animInstance == 0) {
			return 0;
		}

		AnimData data;
		data.Init(animInstance, playerInstance, pIndex);
		
	//Gets used to write the coordinates of the animation
		float coords[3];
		static Address coordoffset(0x5D4C88);
		coordoffset.Call<void>(animInstance + 2, PlayerClass::GetInstance(pIndex)->GetCoordinates(wX, wY)); 
		
	//Knock Door patch
		static const Address knock(0x655400);
		
	//sets animation data for each animation correctly	
		switch(animID) {
		//item anims	
			case 0xB7:
			case 0x71:
			case 0x72:
			case 0x24:
			case 0x3F:
			case 0x3D:
			case 0x92:
			case 0x93:
			case 0x94:
				data.Null_B7_71_72_24_3F_3D_92_93_94(animItem);
			break;
		//clover pick up | item to inv
			case 0x41:
			case 0x42:
				data.PickPluck_41_42(animItem);
			break;
		//equip item
			case 0x38:
				data.EquipItem_38(animItem);
			break;
		//pick up item
			case 0x3E:
				data.PickUp_3E(animItem, snake);
			break;
		//get santa bag | changes to sting removal as santa bag is annoying
			case 0x39: {
				animID = 0xA7;
				data.BeeSting_A7();
			} break;
		//emotion anims
			case 0xAF:
			case 0x88:
				data.Emotion_AF_88(emotion);
			break;
		//snake anim 1
			case 0xC4:
				data.Snake_C4(snake, sound);
			break;
		//snake & sound anim
			case 0xC5:
				data.Null_C5(snake);
			break;
		//knock door
			case 0x45: {
				Process::Patch(knock.addr, 0xE8BD8FF0); //patch so knocking doesn't happen   
				data.KnockDoor_45(sound);
			} break;
		//put item out
			case 0x3B:
				data.UnequipTool_3B(emotion);
			break;
		//Hold item
			case 0x3C:
				data.HoldTool_3C(animItem, emotion, Coord{ wX, wY });
			break;
		//shovel hits nothing
			case 0x4A:
				data.ShovelHitsNothing_4A(emotion, Coord{ wX, wY });
			break;
		//change outfit
			case 0x36:
				data.ChangeOutfit_36(animItem, emotion);
			break;
		//sitting
			case 0x7B:
			case 0x7C:
			case 0x7D:
			case 0x7F:
				data.SitDown_7B_7C_7D_7E_7F(Coord{ wX, wY });
			break;
		//Face Cutout Standee standing
			case 0xC1:
				data.FaceCutoutStandee_C1(emotion);
			break;
		//a lot of random ones
			case 0x1C:
			case 0x25:
			case 0x1D: 
			case 0xE2:
			case 0xE5:
			case 0xA8:
			case 0xA9:
			case 0xAA:
			case 0xAB:
			case 0xAD:
			case 0xAE:
			case 0x80:
			case 0x81:
			case 0x82:
			case 0x83:
			case 0x84:
			case 0x85:
			case 0x86:
			case 0x87:
			case 0x2B:
			case 0x2C:
			case 0x2D:
			case 0x2E:
			case 0x2F:
			case 0x30:
			case 0x31:
			case 0x32:
				data.Null_1C_25_1D_E2_E5_A8_A9_AA_AB_AD_AE_80_81_82_83_84_85_86_87_2B_2C_2D_2E_2F_30_31_32(emotion, Coord{ wX, wY });
			break;
		//unused fall down for fishing
			case 0xB3:
			case 0xB4:
			case 0xB5:
				data.FishFall_B3_B4_B5(animItem, Coord{ wX, wY });
			break;
		//appearance change
			case 0xB9:
				data.Appearance_B9(animItem, appearance);
			break;
		//faint
			case 0x9D:
				data.Faint_9D();
			break;
		//door open
			case 0x44:
				data.DoorOpen_44();
			break;
		//random ones
			case 0xD9:
			case 0x75:
				data.Null_D9_75();
			break;
		//Open mailbox
			case 0x8B:
			case 0xDE:
				data.OpenMailBox_8B_DE();
			break;
		//close mailbox
			case 0x8C:
			case 0xDF:
				data.CloseMailBox_8C_DF();
			break;
		//bury item in
			case 0x53:
			case 0x52:
				data.BuryItem_53_52(Coord{ wX, wY });
			break;
		//Bury
			case 0x4F:
				data.Bury_4F(Coord{ wX, wY });
			break;
		//Bury item out
			case 0x50:
			case 0x51:
				data.BuryOut_50_51(animItem, Coord{ wX, wY });
			break;
		//random one
			case 0x14:
				data.Null_14(Coord{ wX, wY });
			break;
		//sit down
			case 0x79:
				data.SitDown_79();
			break;
		//sit down v2
			case 0x7A:
				data.SitDown_7A();
			break;
		//stand up
			case 0xAC:
				data.StandUp_AC();
			break;
		//throw fish rod
			case 0xB0:
				data.ThrowFishRod_B0();
			break;
		//random ones
			case 0xE1:
			case 0x62:
				data.Null_E1_62();
			break;
		//player blinking
			case 0x95:
			case 0x33:
				data.Null_95_33(Coord{ wX, wY });
			break;
		//switch on light
			case 0x69:
				data.SwitchLight_69(Coord{ wX, wY });
			break;
		//rock hit
			case 0x4C:
				data.ShovelHit_4C(Coord{ wX, wY });
			break;
		//congrats
			case 0x2A: //0xAAEAD8 is pointer for it
				data.Congrats_2A();
			break;
		//toy hammer
			case 0x9C: 
				data.ToyHammerHit_9C(Coord{ wX, wY });
			break;
		//every other animation
			default:
				data.Null_Default(animItem, Coord{ wX, wY });
			break;
		}

		if(!directSend) {
		//If animation is forced and needs overwrite patch code
			static const Address OverWriteInstance(0x1ABADC);	
			if(forced && needOverwrite) {
				Process::Patch(Address(0x5C3EA0).addr + 0x10, 0xE3A00000 + pIndex); //Patches code so player instance returns Index Address of selected player
				Process::Patch(OverWriteInstance.addr, 0xE3A00000 + pIndex); //Patches code so instead of own player selected player gets used
			}
		//If animation is forced on someone else patch code
			static const Address PlayerDataCondition(0x2FEB64);
			static const Address IndexCondition(0x677530);
			static const Address InstanceCondition(0x677537);	
			static const Address Condition1(0x677454);
			static const Address Condition2(0x6774F0);

			if(forced) {
				Process::Patch(PlayerDataCondition.addr, 0xE1A00000); //removes condition
				
				Process::Patch(IndexCondition.addr, 0xE1A01006); //removes condition
				Process::Write8(InstanceCondition.addr, 0xEB); //removes condition
				Process::Patch(Condition1.addr, 0xE1A00000); //removes condition
				Process::Patch(Condition2.addr, 0xE1A00000); //removes condition
				Process::Patch(Address(0x677504).addr, 0xE1A00000); //removes condition
				Process::Patch(Address(0x628B54).addr, 0xE3A01000 + pIndex); //in sendPkt Function GetOnlinePlayerIndex inline
				Sleep(Milliseconds(5));
			}
		//Executes Animation
			data.ExecuteAnimation(animID);
			
		//If animation is forced on someone else undo patches
			if(forced) {
				Sleep(Milliseconds(5));
				
				Process::Patch(PlayerDataCondition.addr, 0x1A000002); //Undo Patch

				Process::Patch(IndexCondition.addr, 0x01A01006); //Undo Patch
				Process::Write8(InstanceCondition.addr, 0x0B); //Undo Patch
				Process::Patch(Condition1.addr, 0x0A000038); //Undo Patch	
				Process::Patch(Condition2.addr, 0x0A00001C); //Undo Patch
				Process::Patch(Address(0x677504).addr, 0x1A000017); //Undo Patch
				Process::Patch(Address(0x628B54).addr, 0xE5D11268); //Undo Patch
		
				u32 res = Wrap::CalculateBranchInstruction(Address(0x5C3EA0).addr + 0x10, Address(0x305EF0).addr);
				Process::Patch(Address(0x5C3EA0).addr + 0x10, 0x2B000000 + res); //Undo Patch
				Process::Patch(OverWriteInstance.addr, 0xE3A00004); //Undo Patch
			}
		}
	//If it's a direct send
		else {
			if(forced) {
				Animation::SendAnimPacket(AIndex, animInstance, animID, Player::GetRoom(pIndex), pIndex); //Uses Animation Pack if forced
			}
			else {
				data.ExecuteAnimation(animID); //Executes Animation on yourself
			}
		}
		
		//After it's over undo the patch with the Knock Animation
		Process::Patch(knock.addr, 0xE24DD00C);    
		
		Sleep(Milliseconds(25));
		return 1;
	}
//Send Animation Packet
	void Animation::SendAnimPacket(u8 senderIndex, u32 animObj, u8 animID, u8 roomID, u8 targetPlayerIndex) {
 		static const Address PatchIndex(0x5C3CAC);

		Process::Write8(animObj, roomID);
		Process::Write8(animObj + 1, animID);

		Process::Patch(Address(0x628B54).addr, 0xE3A01000 + targetPlayerIndex); //in sendPkt Function GetOnlinePlayerIndex inline
		Process::Write8(PatchIndex.addr, targetPlayerIndex);
		Sleep(Milliseconds(5));

		static Address SendPacketFunc(0x5C3C7C);
		SendPacketFunc.Call<void>(targetPlayerIndex, animObj); 

		Sleep(Milliseconds(5));
		Process::Patch(Address(0x628B54).addr, 0xE5D11268);
		Process::Write8(PatchIndex.addr, 4);
	}
//Idle Animation
	void Animation::Idle(u8 pID) {
		u32 x, y;
		if(!PlayerClass::GetInstance(pID)->GetWorldCoords(&x, &y)) {
			return;
		}
		
		Animation::ExecuteAnimationWrapper(pID, 6, {0, 0}, 0, 0, 0, 0, x, y, 0);
	}
}
