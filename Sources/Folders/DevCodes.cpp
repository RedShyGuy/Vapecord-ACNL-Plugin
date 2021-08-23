#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
//Integer For Custom Dumper
	static WrapLoc cdump;
	static std::string filetype = "dat";
//Custom Dumper	
	void customdump(MenuEntry *entry) {	
		std::vector<std::string> customdopt = { "Set Custom Dump", "Dump/Restore" };
		
		std::vector<std::string> customdopt1 = { "Start Offset", "Dump length", "File Type" };
		
		std::vector<std::string> customdopt2 = { "Dump", "Restore", "Delete" };

		Keyboard CKB("text");

		Keyboard OKB(Language->Get("KEY_CHOOSE_OPTION"));
		OKB.Populate(customdopt);
		switch(OKB.Open()) {
			default: break;
			case 0: {
				Sleep(Milliseconds(100));
				CKB.GetMessage() = ("Set your custom dump offset\nHow to: \n1.Type in the start offset to dump from\n2.Type in the length of the dump\n3. Type in the file type the dump should have\n- Great to test things.\n- If you don't know what you are doing don't\n- restore weird dumps!.");
				CKB.Populate(customdopt1);
				
				switch(CKB.Open()) {
					default: break;
					case 0:
						Wrap::KB<u32>("Offset to start the dump.", true, 8, cdump.Address, cdump.Address);
					break;
					
					case 1:
						Wrap::KB<u32>("The length of the dump.", true, 8, cdump.Lenght, cdump.Lenght);
					break;
					
					case 2: {
						Keyboard KB("The file type of the dump.");
						KB.SetMaxLength(8);
						KB.Open(filetype);
					} break;	
				}
			} break;
			
			case 1: {
				Sleep(Milliseconds(100));
				CKB.GetMessage() = ("Now you can dump the file/or restore it\n(the restore function also needs the same start offset/dump length and file type as the dump!).");
				CKB.Populate(customdopt2);
				switch(CKB.Open()) {
					default: break;
					case 0: {
						std::string filename = "";
						CKB.GetMessage() = ("Custom Dumper\n\nName the Dump you'd like to create.");
						if(CKB.Open(filename) == -1)
							return;

						Wrap::Dump(PATH_CUSTOM, filename, "." + filetype, cdump, WrapLoc{ (u32)-1, (u32)-1 });
					} break;	
					case 1: 
						Wrap::Restore(PATH_CUSTOM, "." + filetype, "Select which dump to restore.", nullptr, true, cdump, WrapLoc{ (u32)-1, (u32)-1 }); 
					break;
					case 2:
						Wrap::Delete(PATH_CUSTOM, "." + filetype);
					break;			
				}
			} break;	
		}
	}

    static bool Turbo_Call = false;

    void FunctionsCallerSettings(MenuEntry *entry) {
		std::vector<std::string> cmnOpt =  { "" };

		if(Turbo_Call) 
			cmnOpt[0] = Color(pGreen) << Language->Get("VECTOR_ENABLED");
		else 
			cmnOpt[0] = Color(pRed) << Language->Get("VECTOR_DISABLED");

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cmnOpt);
		Sleep(Milliseconds(100));
		int op = optKb.Open();
		if(op == -1)
			return;
		
		Turbo_Call = !Turbo_Call;
		FunctionsCallerSettings(entry);
    }

    void FunctionsCaller(MenuEntry *entry) {
		static u32 funcaddress;
		static u32 p[11];
		u32 result;
		static int size;

        if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
            if(Wrap::KB<u32>("Enter address of function:", true, 8, funcaddress, funcaddress, nullptr)) {
				Keyboard KB("Enter ID:");
				KB.SetMaxLength(8);
				KB.IsHexadecimal(true);
				for(int i = 0; i < 12; i++) {
					Sleep(Milliseconds(100));
					KB.GetMessage() = "Enter value for param " << std::to_string(i) << ":" << 
					"\n" << Color(pBlue) << "0x11111111 for player instance" << 
					"\n" << Color(pRed) << "0x22222222 for animation instance" << 
					"\n" << Color(pGreen) << "0x33333333 for map offset";
					
					if(KB.Open(p[i]) == -1) {
						size = i--;
						OSD::Notify(Utils::Format("Set Function: %08X with %02d parameters!", funcaddress, size));
						return;
					}

					switch(p[i]) {
						case 0x11111111: 
							p[i] = PlayerClass::GetInstance()->Offset();
						break;

						case 0x22222222:
							p[i] = Animation::GetAnimationInstance(PlayerClass::GetInstance()->Offset(), 0, 0, 0);
						break;

						case 0x33333333:
							p[i] = GameHelper::GetCurrentMap();
						break;
					}
				}	
			}
        }

		if(Turbo_Call ? Controller::IsKeysDown(entry->Hotkeys[1].GetKeys()) : Controller::IsKeysPressed(entry->Hotkeys[1].GetKeys())) {
			if(!Process::CheckAddress(funcaddress))
				return;

			Sleep(Milliseconds(100));
			static FUNCT func(funcaddress);
			switch(size) {
				case 0: result = func.Call<u32>(); break;
				case 1: result = func.Call<u32>(p[0]); break;
				case 2: result = func.Call<u32>(p[0], p[1]); break;
				case 3: result = func.Call<u32>(p[0], p[1], p[2]); break;
				case 4: result = func.Call<u32>(p[0], p[1], p[2], p[3]); break;
				case 5: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4]); break;
				case 6: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4], p[5]); break;
				case 7: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4], p[5], p[6]); break;
				case 8: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]); break;
				case 9: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]); break;
				case 10: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]); break;
				case 11: result = func.Call<u32>(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10]); break;
			}
			OSD::Notify(Utils::Format("Returned Value: %08X", result));
		}
    }

	void player_dumper(MenuEntry *entry) {
		if(Player::GetSaveOffset(4) == 0) {
			MessageBox(Language->Get("SAVE_PLAYER_NO"))();
			return;
		}
		
		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};
		
		const std::vector<std::string> select = {
			"Player Dump",
			"Player Restore",
			"Delete File"
		};
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(select);
		switch(optKb.Open()) {
			default: break;
			case 0: {
				for(int i = 0; i <= 3; i++) {
					u32 pO = Player::GetSaveOffset(i);
					if(pO != 0) {
						std::string pS = "";
						Process::ReadString((pO + 0x55A8), pS, 0x10, StringFormat::Utf16);
						pV[i] = pColor[i] << pS;
					}
				}
		
				Keyboard pKB(Language->Get("KEY_SELECT_PLAYER"));
				pKB.Populate(pV);
				int pChoice = pKB.Open();
				
				if((pChoice != -1) && (pV[pChoice] != Color::Silver << "-Empty-")) {
					std::string filename = "";
					Keyboard KB("Player Dumper\nSet File Name:");
					if(KB.Open(filename) == -1)
						return;

					Wrap::Dump(PATH_PLAYER, filename, ".player", WrapLoc{ Player::GetSaveOffset(pChoice), 0xA480 }, WrapLoc{ (u32)-1, (u32)-1 });
				}
			} break;
			
			case 1:
				Wrap::Restore(PATH_PLAYER, ".player", "Player Restorer\nSelect File:", nullptr, true, WrapLoc{ Player::GetSaveOffset(4), 0xA480 }, WrapLoc{ (u32)-1, (u32)-1 }); 
			break;

			case 2:
				Wrap::Delete(PATH_PLAYER, ".player");
			break;
		}
	}
	
//Message Box Debug	
	void msgboxtest(MenuEntry *entry) {
		static std::string text;
		if(Controller::IsKeysPressed(Key::R + Key::DPadLeft)) 
			ACMSG::Notify(text);
		if(Controller::IsKeysPressed(Key::R + Key::DPadRight)) {
			Keyboard kb("Enter Text");
			kb.Open(text);
		}
	}
/*
//Hook
	bool randomfall(void) {
		if(Player::GetTool() == 0x3357) 
			return 1;
		
		if(Controller::IsKeyDown(Key::A)) 
			return 1;
		
		return 0;
	}
//Enable Unused Fall Down
	void falldownfishing(MenuEntry *entry) {
		static Hook hook;
		if(entry->WasJustActivated()) {
			u32 addressToHookAt = 0x656CCC;
			hook.Initialize(addressToHookAt, (u32)randomfall);
		  	hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		}

		if(!entry->IsActivated()) 
            hook.Disable();
	}
*/

/*
	u32 FUN_001C4940(void) {
  		return *(u32 *)(0x951378 + 0x18);
	}

	u8 FUN_0082E21C(u32 param_1, uint param_2) {
		u8 bVar1;
		
		if(param_2 < 7) {
			bVar1 = *(u8 *)(param_1 + 0x3CA4 + (param_2 >> 3)) >> (param_2 & 7) & 1;
		}

		else {
			bVar1 = 0;
		}
		return bVar1;
	}

	u32 FUN_007633f4(int param_1) {	
		u32 iVar1 = FUN_001C4940();

		u32 piVar2 = *(u32 *)(iVar1 + 0xECD0);

		u8 val = *(u8 *)(param_1 + 0x34C);
		u8 res = FUN_0082E21C(piVar2, val);
		

		if((*(u32 *)(param_1 + 0x348) == piVar2) && (val < *(u8 *)(iVar1 + 0xECD4)) && res != 0) { 
			piVar2 = piVar2 + val * 0x22A + 3;
		}
		else {
			piVar2 = 0;
		}

		iVar1 = *(u32 *)(piVar2 + 0x19A);
		if(iVar1 != 0) {
			iVar1 = 1;
		}
		return iVar1;
	}
*/

/*
//Analyze fossils
	void Analyzer(MenuEntry *entry) {
		u32 item = Inventory::ReadSlot(Inventory::GetSelectedSlot());
		
		if(item == 0x202A) 
			Inventory::WriteSlot(Inventory::GetSelectedSlot(), Utils::Random(0x3130, 0x3172));
	}
*/

/*
//u32 Currentmap = Region::AutoRegion(0x9B5A24, 0x9B4A24, 0x9B4A24, 0x9B4A24, 0x9AEA04, 0x9ADA04, 0x9ADA24, 0x9ADA24);
	void ItemPlacer(u32* ItemID) {
		static FUNCT func(0x581E3C);
		func.Call<void>(GameHelper::GetCurrentMap(), ItemID, PlayerClass::GetInstance()->GetCoordinates(), 0);
	}
*/
	

	/*
	Function which sets coordinates and all to storage for player
	68dee8(u32 pinstance)
	*/

	/*
	assigns data to save menu (options)
	void FUN_005e7fd8(int param_1)

	Save and continue function
	void FUN_006a2f88(undefined4 param_1)
	Save and quit function
	void FUN_006a2b6c(undefined4 param_1)

	0x6A3620(0x3307D284); //executes func for save menu
	*/

/*
	u32 soundArray[11] = {
		0x0100038E, 0x01000392, 0x0100038C, 0x010003E8, 
		0x010003E9, 0x0100038F, 0x0100038D, 0x010003C5, 
		0x01000393, 0x01000395, 0x010004BD
	};

//0x33084094
	void FUN_005E7FD8(u32 param_1) {
		u32 local_5c[3];
		u32 local_40;
		u32 local_3c[3];

		u32 local_48 = param_1 + 0x518;
		u32 local_4c = param_1 + 0x578;

		u32 val = 0;
		u32 funcArray = 0;

		s8 i = 0;
		while(i < 6) {
			u32 var1 = (local_48 + i * 0x10); 
			u32 var2 = (local_4c + i * 0x10);

			u32 iVar7 = *(u32 *)(param_1 + i * 4 + 0x1274);

			u32 iVar3 = *(u32 *)(iVar7 + 0xEC);
			if(iVar3 != 0) {
				Process::Write32(iVar7 + 4, *(u32 *)(iVar3 + 0xD8));
				Process::Write32(iVar7 + 8, *(u32 *)(iVar3 + 0xDC));
			}

			iVar3 = *(u32 *)(iVar7 + 0xF0);
			if(iVar3 != 0) {
				Process::Write32(iVar7 + 0x48, *(u32 *)(iVar3 + 0xD8));
				Process::Write32(iVar7 + 0x4C, *(u32 *)(iVar3 + 0xDC));
			}

			bool bVar12 = i < 0;
			bool bVar14 = i == false;
			bool bVar13 = bVar12;
			if(!bVar12) {
				iVar3 = *(u32 *)(param_1 + 8);
				bVar13 = iVar3 - i < 0;
				bVar14 = iVar3 == i;
			}
			
			if((bVar14 || bVar13 != (!bVar12 && (iVar3 <= i))) || (local_40 = *(u32 *)(param_1 + 0xC) + i * 0xAC, local_40 == 0)) {
				val = *(u32 *)var1;
				if(*(u32 *)val != 0) {
					funcArray = *(u32 *)val;

					FUNCT(*(u32 *)(funcArray + 0x78)).Call<void>(val, 0x8CAF0E, 0); //0x4BBFAC
				}

				val = *(u32 *)var2;
				if(*(u32 *)val != 0) {
					funcArray = *(u32 *)val;

					FUNCT(*(u32 *)(funcArray + 0x78)).Call<void>(val, 0x8CAF0E, 0); //0x4BBFAC
				}

				val = *(u32 *)(var1 + 4);
				if(*(u32 *)val != 0) {
					funcArray = *(u32 *)val;

					FUNCT(*(u32 *)(funcArray + 0x78)).Call<void>(val, 0x8CAF0E, 0); //0x4BBFAC
				}

				val = *(u32 *)(var2 + 4);
				if(*(u32 *)val != 0) {
					funcArray = *(u32 *)val;

					FUNCT(*(u32 *)(funcArray + 0x78)).Call<void>(val, 0x8CAF0E, 0); //0x4BBFAC
				}
			}

			else {
				**(u8 **)(iVar7 + 0x24) = 0;
				**(u8 **)(iVar7 + 0x68) = 0;

				Process::Write32(iVar7 + 0xE0, *(u32 *)(iVar7 + 0xEC));
				Process::Write32(iVar7 + 0xE4, *(u32 *)(iVar7 + 0xF0));

				local_3c[0] = FUNCT(*(u32 *)(local_40 + 8) + 0xC).Call<u32>();

				FUNCT(0x602480).Call<void>(iVar7 + 0x78, local_3c, 0x95EF34);
				FUNCT(0x60231c).Call<void>(iVar7 + 0x78, 0x95EF34);

				Process::Write32(iVar7 + 0xE0, 0);
				Process::Write32(iVar7 + 0xE4, 0);

				local_5c[0] = *(u32 *)(iVar7 + 0x14);
				u32 local_50 = *(u32 *)(*(u32 *)(iVar7 + 0x20) + (*(u32 *)(iVar7 + 0x1C) + -1) * 4);
				iVar3 = FUNCT(0x5EAB78).Call<u32>(*(u32 *)(iVar7 + 0xEC), local_5c, &local_50);

				val = *(u32 *)var1;
				if(*(u32 *)val != 0) {
					Process::Write32(val + 0x48, iVar3);
					Process::Write32(val + 0x4C, val + 0x4C);
				}

				val = *(u32 *)var2;
				if(*(u32 *)val != 0) {
					Process::Write32(val + 0x48, iVar3);
					Process::Write32(val + 0x4C, val + 0x4C);
				}

				val = *(u32 *)(var1 + 8);
				if(*(u32 *)val != 0) {
					Process::Write32(val + 0x48, iVar3);
					Process::Write32(val + 0x4C, val + 0x4C);
				}

				FUNCT(0x5D77F8).Call<void>(iVar7);

				u32 soundID = 0;

				if(*(u8 *)(local_40 + 0xA4) < 0xB)
					soundID = soundArray[*(u8 *)(local_40 + 0xA4)];
				else
					soundID = 0x100038E;

				val = *(u32 *)(var1 + 0xC);
				Process::Write32(val, soundID);
			}
			
			i++;
		}
	}


	void CustomSaveScreen(MenuEntry *entry) {
		if(entry->WasJustActivated()) {
			static Hook hook;
			hook.Initialize(0x5E7FD8, (u32)FUN_005E7FD8);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.SetReturnAddress(0x5E821C);
			hook.Enable();
		}
*/

//Item Island Code
	void islanditems(MenuEntry *entry) {

		/*if(Controller::IsKeysPressed(Key::R + Key::DPadUp)) {
			AnimationData *globalData = new AnimationData(); //<u8, u8, u8, u8, u8, u16, u8>

			u32 player = PlayerClass::GetInstance()->Offset();

			globalData->roomID = 0;
        	globalData->animID = 0xB9;
			globalData->xCoordSH = Calculate16BitFloat(*(float *)(player + 0x14));
			globalData->zCoordSH = Calculate16BitFloat(*(float *)(player + 0x1C));
			globalData->rotationSH = PlayerClass::GetInstance()->GetRotation();

			*(u8 *)globalData->data = 0x10;
			*(u8 *)(globalData->data + 1) = 0;

			*(u8 *)(globalData->data + 2) = 3;
			*(u8 *)(globalData->data + 3) = 3;
			*(u8 *)(globalData->data + 4) = 3;

			*(u16 *)(globalData->data + 5) = 0x7FFE;

			*(u8 *)(globalData->data + 7) = 0;

			static FUNCT func(Code::AnimFunction);
			func.Call<bool>(player, 0xB9, globalData, 0);

			delete globalData;
		}*/
	}
	
	void PlayerLoader(MenuEntry *entry) {
		/*if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			u32 principalID = 0;
			u64 *friendcode = nullptr;
			Keyboard KB("Enter Principal ID");
			KB.IsHexadecimal(true);
			if(KB.Open(principalID, principalID) == 0) {
				FRD_PrincipalIdToFriendCode(principalID, friendcode);
				OSD::Notify(Utils::Format("FriendCode: %ld", *friendcode));
			}
		}*/
	}

	/*bool ScreenshotmapperL(void) {
		return Controller::IsKeyDown(Key::ZL);
	}

	bool ScreenshotmapperR(void) {
		return Controller::IsKeyDown(Key::ZR);
	}

	float fu0 = 0.0, fu1 = 0.0;

	void ChangeSpeed(u32 u0, u32 u1, u32 speedAddress) {
		static FUNCT func(0x56BBA8);
		func.Call<void>(fu0, fu1, speedAddress);
	}

	u32 itemID = 0;

	void keymap(MenuEntry *entry) {	
		static Hook hook;
		u32 address = Region::AutoRegion(0x64FC1C, 0, 0, 0, 0, 0, 0, 0);
		hook.Initialize(address, (u32)ChangeSpeed);
		hook.SetFlags(USE_LR_TO_RETURN);
		//hook.Enable();

		if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			OSD::Notify(IDList::GetItemName(itemID));
		}

		if(Controller::IsKeysPressed(Key::L + Key::DPadDown)) {
			Keyboard KB("");
			KB.Open(itemID, itemID);
		}
		
		u32 LButton = Region::AutoRegion(0x5B4180, 0, 0, 0, 0, 0, 0, 0);
		u32 RButton = Region::AutoRegion(0x5B4194, 0, 0, 0, 0, 0, 0, 0);
		L1.Initialize(LButton, (u32)ScreenshotmapperL);
		R1.Initialize(RButton, (u32)ScreenshotmapperR);
		L1.SetFlags(USE_LR_TO_RETURN);
		R1.SetFlags(USE_LR_TO_RETURN);
		
		L1.Enable();
		R1.Enable();
	}	

	void valuedisplayer(MenuEntry *entry) {
		if(entry->WasJustActivated())
			Process::Write32(0x294E00, 0xE1A00000); //Disables menu from dissapearing

		//34CAA0 E0800004
		//34CAA0 E0400004
	}*/

	void lightswitch(MenuEntry *entry) {
		static const u32 TargetAddress = Region::AutoRegion(0x190EA8, 0x1908F0, 0x190EC8, 0x190EC8, 0x190E18, 0x190E18, 0x190E18, 0x190E18);
		static const u32 Patch = Region::AutoRegion(0x1E7AD8, 0x1E751C, 0x1E7AF8, 0x1E7AF8, 0x1E7A34, 0x1E7A34, 0x1E7A00, 0x1E7A00);

		if(entry->WasJustActivated()) {
		//this disables the "non switchable light" flag to be written
			u32 val = 0;
			Process::Read32(TargetAddress, val);
			Process::Write32(TargetAddress, val + 0x146); //patch BL 0x1E7554 to BL 0x1E7A6C
		//this disables the reading of the "non switchable light" flag
			Process::Write32(Patch, 0xE3A00000);
		}

		static u8 roomID = 0;

		if(Controller::IsKeysPressed(Key::L + Key::DPadUp)) {
			if(LightSwitch::IsON())
				LightSwitch::OFF();
			else
				LightSwitch::ON();
		}

		if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			Wrap::KB<u8>("", true, 2, roomID, roomID);
		}

		if(Controller::IsKeysPressed(Key::L + Key::DPadLeft)) {
			if(LightSwitch::IsON(roomID))
				LightSwitch::OFF(roomID);
			else
				LightSwitch::ON(roomID);
		}

		if(!entry->IsActivated()) {
			u32 val = 0;
			Process::Read32(TargetAddress, val);
			Process::Write32(TargetAddress, val - 0x146);
			Process::Write32(Patch, 0x05960028);
		}
	}

	void FishThrower(MenuEntry *entry) {				
		static u8 playerID = 0;
		static bool random = false;
		u16 FishID = 0x22E1;

		//static FUNCT func(0x5C3DDC);
		static FUNCT throwfish(0x5C2DAC);

		if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			if(playerID == 3) playerID = 0;		
			else playerID++;

			OSD::Notify(Utils::Format("Player %02X selected!", playerID));
		}

		if(Controller::IsKeysPressed(Key::L + Key::DPadLeft)) {
			random = !random;
			OSD::Notify("Random Mode: " << (random ? (Color::Green << "ON") : (Color::Red << "OFF")));
		}

		if(Controller::IsKeysDown(Key::R)) {
			if(random) {
				FishID = Utils::Random(0x22E1, 0x234A);
				while(FishID == 0x232C | FishID == 0x232B | FishID == 0x232A  | FishID == 0x2329)
					FishID = Utils::Random(0x22E1, 0x234A);
			}

			throwfish.Call<void>(&FishID, PlayerClass::GetInstance(playerID)->GetCoordinates(), 1);
		}

		if(Controller::IsKeysDown(Key::R + Key::DPadUp)) 
			Wrap::KB<u16>("Set Fish ID:", true, 4, FishID, FishID);
	}

//player_dumper 33077C8A
	/*void RestoreAll(MenuEntry *entry) {
		const std::vector<std::string> select = {
			Color(0x0077FFFF) << "Restore Exhibition",
			Color(0xFFDE00FF) << "Restore Friend",
			Color(0xFF00EFFF) << "Restore Design",
			Color(0xFF2C2CFF) << "Restore Mail"
		};

		Keyboard optKb(parser->getEntry("KEY_CHOOSE_OPTION"));
		optKb.Populate(select);
		switch(optKb.Open()) {
			case 0: {
				if(GameHelper::GetExhibition() == 0) {
					MessageBox(Color(0x0077FFFF) << "Load Happy Home Showcase for it to work!").SetClear(ClearScreen::Top)();
					return;
				}
				
				Wrap::Restore(PATH_PLAYER, ".dat", "Restore Exhibition:", nullptr, WrapLoc{ GameHelper::GetExhibition(), 0x17BE10 }, WrapLoc{ (u32)-1, (u32)-1 }); 
			} break;
			case 1: {
				if(GameHelper::GetFriend() == 0) {
					MessageBox(Color(0xFFDE00FF) << "Load player save for it to work!").SetClear(ClearScreen::Top)();
					return;
				}
				
				Wrap::Restore(PATH_PLAYER, ".dat", "Restore Friend:", nullptr, WrapLoc{ GameHelper::GetFriend(), 0x29608 }, WrapLoc{ (u32)-1, (u32)-1 }); 
			} break;
			case 2: {
				if(GameHelper::GetDesign() == 0) {
					MessageBox(Color(0xFF00EFFF) << "Load design Save for it to work!").SetClear(ClearScreen::Top)();
					return;
				}
				
				Wrap::Restore(PATH_PLAYER, ".dat", "Restore Design:", nullptr, WrapLoc{ GameHelper::GetDesign(), 0x25F90 }, WrapLoc{ (u32)-1, (u32)-1 }); 
			} break;
			case 3: {
				if(GameHelper::GetMail() == 0) {
					MessageBox(Color(0xFF2C2CFF) << "Load mail save for it to work!").SetClear(ClearScreen::Top)();
					return;
				}
				
				Wrap::Restore(PATH_PLAYER, ".dat", "Restore Mail:", nullptr, WrapLoc{ GameHelper::GetMail(), 0x1C208 }, WrapLoc{ (u32)-1, (u32)-1 }); 
			} break;
		}
	}*/

	/*void claim(void) {
		u32 DesignData = *(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x464; //0x32DC4E1C
		
		const u32 GetSlot = 0x724214;
		Process::Write32((u32)&FUN, GetSlot);
		u8 DesignSlot = FUN(DesignData);
		
		u32 Design = Player::GetSaveOffset(4) + 0x548C;		
		u8 correctslot = *(u8 *)(Design + (0x1 * DesignSlot));
		
		std::string PlayerName = "", TownName = "";
		Process::ReadString(PlayerPTR::Pointer(0x55A8), PlayerName, 0x10, StringFormat::Utf16);
		Process::ReadString(PlayerPTR::Pointer(0x55BE), TownName, 0x10, StringFormat::Utf16);
		
		Player::SetDesign(correctslot, "", *(u16 *)PlayerPTR::Pointer(0x55A6), PlayerName, *(u8 *)PlayerPTR::Pointer(0x55BA), *(u16 *)PlayerPTR::Pointer(0x55BC), TownName, *(u32 *)PlayerPTR::Pointer(0x55D0), 0xFF, 0xFF);
	
		FUN_008188a0(iParm1 + 0xa4,PTR_FUN_00852d64,DAT_00852d68);
	}

	void claimdesignbutton(MenuEntry *entry) {
		static Hook hook;
		
		const u32 AlwaysWear = 0x258E58;
		const u32 AlwaysUmbrella = 0x2585B8;
		
		if(entry->WasJustActivated()) {	
			Process::Write32(AlwaysWear, 0xE1A00000);
			Process::Write32(AlwaysUmbrella, 0xE1A00000);
		
			u32 HookOff = 0x25A0A4;
			hook.Initialize(HookOff, (u32)claim);
			hook.SetFlags(0);
			hook.Enable();
		}
		
		if(GameHelper::BaseInvPointer() != 0) {
			if(!Inventory::Opened())
				return;
			
			u32 Designs = *(u32 *)(*(u32 *)(GameHelper::BaseInvPointer() + 0xC) + 0x22C0);
			
			for(int i = 0; i < 6; i++) {
				u32 CurrDesign = Designs + (0xAC * i);
			//If ID is Wear
				if(*(u8 *)(CurrDesign + 0xA8) == 0)
					Process::WriteString((CurrDesign + 0x20), "Claim Design", 0x28, StringFormat::Utf16);
			}
		}
		
		if(!entry->IsActivated()) {	
			Process::Write32(AlwaysWear, 0x1A000010);
			Process::Write32(AlwaysUmbrella, 0x0A000009);
		    hook.Disable();
		}
	}

	void CustomButtonString(int button, u8 buttonID) {		
		for(int i = 0; i < 6; i++) {
			u32 CurrItem = Inventory::GetCurrentItemData(i);
			if(CurrItem == 0xFFFFFFFF)
				return;
				
		//If ID is Take Medicine
			if(*(u8 *)(CurrItem + 0xA8) == buttonID) 
				Process::WriteString((CurrItem + 0x20), ItemText[button], 0x28, StringFormat::Utf16);
		}
	}
	
	void CButton(void) {
		CustomButtonString(Button[0], 0x18); //Take Medicine
		CustomButtonString(Button[1], 0x0F); //Release
		CustomButtonString(Button[2], 0x05); //Remove Wet Suit
	}
	
	
	
	81E96C
	*/	

	static const std::vector<std::string> citemsettings = {
		"Duplicate", "Wrap it!", "Put in Storage", "Pay Debt", "Disable",
	};

	u32 SetNameCall(u32 param_1, u32 param_2, u32 param_3, u8 param_4) {
		//register u32 SYSID asm("r5");
		u32 SYSID = 0;

		u32 res = FUNCT(0x312610).Call<u32>(param_1, param_2, param_3, param_4);

		if(*(u8 *)(SYSID + 0xC) == 0x2A) {
			Process::WriteString(*(u32 *)(param_2 + 4), citemsettings[0], 0x20, StringFormat::Utf16);
		}

		return res;
	}

	using FuncType = void(*)(u32);

	static const FuncType ButtonArr[4] = {
		CustomButton::DuplicateItem, CustomButton::WrapItem, CustomButton::PutItemToStorage, CustomButton::PayDebt
	};
	
//put item into storage
	void SettingsButton(MenuEntry *entry) {
		static const std::vector<std::string> cbuttons = { 
			"Item Button 1", "Item Button 2", "Outfit Button 1",
		};
		
		static const std::vector<std::string> coutfitsettings = { 
			"Random Outfit", "Disable", 
		};

		static const u32 FuncPointer = Region::AutoRegion(0x950A00, 0, 0, 0, 0, 0, 0, 0);

		static const u32 SYSNameFunc = Region::AutoRegion(0x5D5860, 0, 0, 0, 0, 0, 0, 0);
		static Hook NameHook;

		static const u32 Undo1 = *(u32 *)(FuncPointer + 0xBC);
		static const u32 Undo2 = *(u32 *)(FuncPointer + 0x74);
		static const u32 Undo3 = *(u32 *)(FuncPointer + 0x24);

		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"));
		optKb.Populate(cbuttons);
		s8 op = optKb.Open();
		if(op < 0)
			return;

	//1st Custom button | replaces medicine button
		if(op == 0) {
			optKb.Populate(citemsettings);
			op = optKb.Open();
			if(op < 0)
				return;

			static const u32 AlwaysMedicine = Region::AutoRegion(0x19BC04, 0, 0, 0, 0, 0, 0, 0);

			if(op == 4) {
				Process::Patch(AlwaysMedicine, 0x1A000020); //Unpatch

				Process::Write32(FuncPointer + 0xBC, Undo1);

				NameHook.Disable();
				return;
			}

			NameHook.Initialize(SYSNameFunc, (u32)SetNameCall);
			NameHook.SetFlags(USE_LR_TO_RETURN);
			NameHook.Enable();

			Process::Patch(AlwaysMedicine, 0xE1A00000); //Always Take Medicine

			Process::Write32(FuncPointer + 0xBC, (u32)ButtonArr[op]);
		}
	//2nd Custom button | replaces release button
		else if(op == 1) {
			optKb.Populate(citemsettings);
			op = optKb.Open();
			if(op < 0)
				return;
			
			static const u32 AlwaysRelease = Region::AutoRegion(0x19BD60, 0, 0, 0, 0, 0, 0, 0);
			static const u32 NeverToss = Region::AutoRegion(0x19BD80, 0, 0, 0, 0, 0, 0, 0);

			if(op == 4) {
				Process::Patch(AlwaysRelease, 0x0A000013);
				Process::Patch(NeverToss, 0xE3A02037);

				Process::Write32(FuncPointer + 0x74, Undo2);
				return;
			}
				
			Process::Patch(AlwaysRelease, 0xE1A00000); //Enables Always Release
			Process::Patch(NeverToss, 0xE3A0200C); //Changes Toss if Cicada Shell to release

			Process::Write32(FuncPointer + 0x74, (u32)ButtonArr[op]);
		}
		
	//3rd Custom button | replaces remove wet suit button
		else if(op == 2) {
			optKb.Populate(coutfitsettings);
			op = optKb.Open();
			if(op < 0)
				return;
			
			static const u32 WetSuitButton = Region::AutoRegion(0x19DBA4, 0, 0, 0, 0, 0, 0, 0);
			static const u32 SocksButton = Region::AutoRegion(0x19DC78, 0, 0, 0, 0, 0, 0, 0);

			if(op == 1) {
				Process::Patch(WetSuitButton, 0x1A000009);
				Process::Patch(SocksButton, 0x1A000009);

				Process::Write32(FuncPointer + 0x24, Undo3);
				return;
			}
			
			Process::Patch(WetSuitButton, 0xE1A00000); //Always Remove Wet Suit
			Process::Patch(SocksButton, 0xEA000009); //Disable Remove Socks

			Process::Write32(FuncPointer + 0x24, (u32)CustomButton::RandomOutfit);
		}
	}
}
