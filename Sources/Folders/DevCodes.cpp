#include "cheats.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/Game.hpp"
#include "Address/Address.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/ACMessageBox.hpp"
#include "Helpers/CROEditing.hpp"
#include "Helpers/ACSystem.hpp"
#include "Helpers/Converters.hpp"
#include "Helpers/GameKeyboard.hpp"
#include "Helpers/Town.hpp"
#include "Helpers/NPC.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Inventory.hpp"
#include "House/House.hpp"

#include "Color.h"
#include "Files.h"

extern "C" void PATCH_CustomButtons(void);

namespace CTRPluginFramework {
//Integer For Custom Dumper
	static WrapLoc cdump;
	static std::string filetype = "dat";
//Custom Dumper	
	void customdump(MenuEntry *entry) {	
		const std::vector<std::string> customdopt = { "Set Custom Dump", "Dump/Restore" };
		
		const std::vector<std::string> customdopt1 = { "Start Offset", "Dump length", "File Type" };
		
		const std::vector<std::string> customdopt2 = { "Dump", "Restore", "Delete" };

		Keyboard CKB("text");

		Keyboard OKB(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION));
		OKB.Populate(customdopt);
		switch(OKB.Open()) {
			default: break;
			case 0: {
				CKB.GetMessage() = ("Set your custom dump offset\nHow to: \n1.Type in the start offset to dump from\n2.Type in the length of the dump\n3. Type in the file type the dump should have\n- Great to test things.\n- If you don't know what you are doing don't\n- restore weird dumps!.");
				CKB.Populate(customdopt1);
				
				switch(CKB.Open()) {
					default: break;
					case 0:
						Wrap::KB<u32>("Offset to start the dump.", true, 8, *(u32 *)cdump.Address, *(u32 *)cdump.Address);
					break;
					
					case 1:
						Wrap::KB<u32>("The length of the dump.", true, 8, *(u32 *)&cdump.Lenght, *(u32 *)&cdump.Lenght);
					break;
					
					case 2: {
						Keyboard KB("The file type of the dump.");
						KB.SetMaxLength(8);
						KB.Open(filetype);
					} break;	
				}
			} break;
			
			case 1: {
				CKB.GetMessage() = ("Now you can dump the file/or restore it\n(the restore function also needs the same start offset/dump length and file type as the dump!).");
				CKB.Populate(customdopt2);
				switch(CKB.Open()) {
					default: break;
					case 0: {
						std::string filename = "";
						CKB.GetMessage() = ("Custom Dumper\n\nName the Dump you'd like to create.");
						if(CKB.Open(filename) == -1)
							return;

						Wrap::Dump(Utils::Format(PATH_CUSTOM, Address::regionName.c_str()), filename, "." + filetype, &cdump, nullptr);
					} break;	
					case 1: 
						Wrap::Restore(Utils::Format(PATH_CUSTOM, Address::regionName.c_str()), "." + filetype, "Select which dump to restore.", nullptr, true, &cdump, nullptr); 
					break;
					case 2:
						Wrap::Delete(Utils::Format(PATH_CUSTOM, Address::regionName.c_str()), "." + filetype);
					break;			
				}
			} break;	
		}
	}

    static bool Turbo_Call = false;

    void FunctionsCallerSettings(MenuEntry *entry) {
		std::vector<std::string> cmnOpt =  { "" };

		if(Turbo_Call) 
			cmnOpt[0] = Color(pGreen) << Language::getInstance()->get(TextID::VECTOR_ENABLED);
		else 
			cmnOpt[0] = Color(pRed) << Language::getInstance()->get(TextID::VECTOR_DISABLED);

		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION));
		optKb.Populate(cmnOpt);
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

        if(Controller::IsKeysPressed(Key::R | Key::DPadRight)) {
            if(Wrap::KB<u32>("Enter address of function:", true, 8, funcaddress, funcaddress, nullptr)) {
				Keyboard KB("Enter ID:");
				KB.SetMaxLength(8);
				KB.IsHexadecimal(true);
				for(int i = 0; i < 12; i++) {
					KB.GetMessage() = "Enter value for param " << std::to_string(i) << ":" << 
					"\n" << Color(pBlue) << "0x11111111 for player instance" << 
					"\n" << Color(pRed) << "0x22222222 for animation instance" << 
					"\n" << Color(pGreen) << "0x33333333 for map offset";
					
					if(KB.Open(p[i]) == -1) {
						size = i--;
						OSDExtras::Notify(Utils::Format("Set Function: %08X with %02d parameters!", funcaddress, size));
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
							p[i] = Game::GetCurrentMap();
						break;
					}
				}	
			}
        }

		if(Turbo_Call ? Controller::IsKeysDown(Key::R | Key::DPadDown) : Controller::IsKeysPressed(Key::R | Key::DPadDown)) {
			if(!Process::CheckAddress(funcaddress))
				return;	

			Sleep(Milliseconds(100));
			Address func(funcaddress);
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
			OSDExtras::Notify(Utils::Format("Returned Value: %08X", result));
		}
    }

	void player_dumper(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get(TextID::SAVE_PLAYER_NO))();
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

		WrapLoc locPlayer;
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION));
		optKb.Populate(select);
		switch(optKb.Open()) {
			default: break;
			case 0: {
				for(int i = 0; i <= 3; ++i) {
					player = Player::GetSaveData(i);
					if(player) {
						if(Player::SaveExists(player)) {
							std::string str = "";
							Convert::U16_TO_STR(player->PlayerInfo.PlayerData.PlayerName, str);
							pV[i] = Player::GetColor(i) << str;
						}
					}
				}
		
				Keyboard pKB(Language::getInstance()->get(TextID::KEY_SELECT_PLAYER));
				pKB.Populate(pV);
				int pChoice = pKB.Open();
				
				if((pChoice != -1) && (pV[pChoice] != Color::Silver << "-Empty-")) {
					std::string filename = "";
					Keyboard KB("Player Dumper\nSet File Name:");
					if(KB.Open(filename) == -1)
						return;

					player = Player::GetSaveData(pChoice);
					locPlayer = { (u32 *)player, sizeof(player) };
					Wrap::Dump(Utils::Format(PATH_PLAYER, Address::regionName.c_str()), filename, ".player", &locPlayer, nullptr);
				}
			} break;
			
			case 1:
				player = Player::GetSaveData();
				locPlayer = { (u32 *)player, sizeof(player) };
				Wrap::Restore(Utils::Format(PATH_PLAYER, Address::regionName.c_str()), ".player", "Player Restorer\nSelect File:", nullptr, true, &locPlayer, nullptr); 
			break;

			case 2:
				Wrap::Delete(Utils::Format(PATH_PLAYER, Address::regionName.c_str()), ".player");
			break;
		}
	}

	void SetPlayerFlag(Player_Flags *playerFlag, int index, bool set) {
		switch(index) {
			default: break;
			case 0:
				playerFlag->Unknown1 = set; 
			break;
			case 1:
				playerFlag->Unknown2 = set; 
			break;
			case 2:
				playerFlag->Unknown3 = set; 
			break;
			case 3:
				playerFlag->Unknown4 = set; 
			break;
			case 4:
				playerFlag->PlayerSetNameAndTownName = set; 
			break;
			case 5:
				playerFlag->ReddIntroduced = set; 
			break;
			case 6:
				playerFlag->Unknown7 = set; 
			break;
			case 7:
				playerFlag->Unknown8 = set; 
			break;
			case 8:
				playerFlag->Unknown9 = set; 
			break;
			case 9:
				playerFlag->FinishedFirstDay = set; 
			break;
			case 10:
				playerFlag->Unknown11 = set; 
			break;
			case 11:
				playerFlag->Unknown12 = set; 
			break;
			case 12:
				playerFlag->Unknown13 = set; 
			break;
			case 13:
				playerFlag->Unknown14 = set; 
			break;
			case 14:
				playerFlag->Unknown15 = set; 
			break;
			case 15:
				playerFlag->Unknown16 = set; 
			break;
			case 16:
				playerFlag->Unknown17 = set; 
			break;
			case 17:
				playerFlag->Unknown18 = set; 
			break;
			case 18:
				playerFlag->Unknown19 = set; 
			break;
			case 19:
				playerFlag->Unknown20 = set; 
			break;
			case 20:
				playerFlag->Unknown21 = set; 
			break;
			case 21:
				playerFlag->Unknown22 = set; 
			break;
			case 22:
				playerFlag->Unknown23 = set; 
			break;
			case 23:
				playerFlag->Unknown24 = set; 
			break;
			case 24:
				playerFlag->Unknown25 = set; 
			break;
			case 25:
				playerFlag->Unknown26 = set; 
			break;
			case 26:
				playerFlag->Unknown27 = set; 
			break;
			case 27:
				playerFlag->TomNookIntroduced = set; 
			break; 
			case 28:
				playerFlag->BlathersIntroduced = set; 
			break;
			case 29:
				playerFlag->Unknown30 = set; 
			break;
			case 30:
				playerFlag->CelesteIntroduced = set; 
			break;
			case 31:
				playerFlag->Unknown32 = set; 
			break;
			case 32:
				playerFlag->Unknown33 = set; 
			break;
			case 33:
				playerFlag->Unknown34 = set; 
			break;
			case 34:
				playerFlag->Unknown35 = set; 
			break;
			case 35:
				playerFlag->Unknown36 = set; 
			break;
			case 36:
				playerFlag->TomNookDownFirstPayment = set; 
			break;
			case 37:
				playerFlag->ChooseHouseLocation = set; 
			break;
			case 38:
				playerFlag->MainStreetUnlocked = set; 
			break;
			case 39:
				playerFlag->HouseLocationChosen = set; 
			break;
			case 40:
				playerFlag->KnowIsabelleName = set; 
			break;
			case 41:
				playerFlag->KnowKappn = set; 
			break;
			case 42:
				playerFlag->Unknown43 = set; 
			break;
			case 43:
				playerFlag->Unknown44 = set; 
			break;
			case 44:
				playerFlag->Unknown45 = set; 
			break;
			case 45:
				playerFlag->Unknown46 = set; 
			break;
			case 46:
				playerFlag->Unknown47 = set; 
			break;
			case 47:
				playerFlag->Unknown48 = set; 
			break;
			case 48:
				playerFlag->Unknown49 = set; 
			break;
			case 49:
				playerFlag->Unknown50 = set; 
			break;
			case 50:
				playerFlag->Unknown51 = set; 
			break;
			case 51:
				playerFlag->Unknown52 = set; 
			break;
			case 52:
				playerFlag->Unknown53 = set; 
			break;
			case 53:
				playerFlag->Unknown54 = set; 
			break;
			case 54:
				playerFlag->Unknown55 = set; 
			break;
			case 55:
				playerFlag->Unknown56 = set; 
			break;
			case 56:
				playerFlag->Unknown57 = set; 
			break;
			case 57:
				playerFlag->Unknown58 = set; 
			break;
			case 58:
				playerFlag->Unknown59 = set; 
			break;
			case 59:
				playerFlag->Unknown60 = set; 
			break;
			case 60:
				playerFlag->Unknown61 = set; 
			break;
			case 61:
				playerFlag->Unknown62 = set; 
			break;
			case 62:
				playerFlag->Unknown63 = set; 
			break;
			case 63:
				playerFlag->Unknown64 = set; 
			break;
			case 64:
				playerFlag->Unknown65 = set; 
			break;
			case 65:
				playerFlag->Unknown66 = set; 
			break;
			case 66:
				playerFlag->Unknown67 = set; 
			break;
			case 67:
				playerFlag->Unknown68 = set; 
			break;
			case 68:
				playerFlag->Unknown69 = set; 
			break;
			case 69:
				playerFlag->Unknown70 = set; 
			break;
			case 70:
				playerFlag->Unknown71 = set; 
			break;
			case 71:
				playerFlag->Unknown72 = set; 
			break;
			case 72:
				playerFlag->Unknown73 = set; 
			break;
			case 73:
				playerFlag->Unknown74 = set; 
			break;
			case 74:
				playerFlag->Unknown75 = set; 
			break;
			case 75:
				playerFlag->Unknown76 = set; 
			break;
			case 76:
				playerFlag->Unknown77 = set; 
			break;
			case 77:
				playerFlag->Unknown78 = set; 
			break;
			case 78:
				playerFlag->Unknown79 = set; 
			break;
			case 79:
				playerFlag->Unknown80 = set; 
			break;
			case 80:
				playerFlag->Unknown81 = set; 
			break;
			case 81:
				playerFlag->ResetPending = set; 
			break;
			case 82:
				playerFlag->Unknown83 = set; 
			break;
			case 83:
				playerFlag->Unknown84 = set; 
			break;
			case 84:
				playerFlag->Unknown85 = set; 
			break;
			case 85:
				playerFlag->Unknown86 = set; 
			break;
			case 86:
				playerFlag->Unknown87 = set; 
			break;
			case 87:
				playerFlag->PeteIntroduction = set; 
			break;
			case 88:
				playerFlag->Unknown89 = set; 
			break;
			case 89:
				playerFlag->Unknown90 = set; 
			break;
			case 90:
				playerFlag->Unknown91 = set; 
			break;
			case 91:
				playerFlag->ExteriorRenovationsUnlocked = set; 
			break;
			case 92:
				playerFlag->HouseLoanRepayed = set; 
			break;
			case 93:
				playerFlag->Unknown94 = set; //Unset(?) when Tom Nook speaks with you after your house was built
			break;
			case 94:
				playerFlag->Unknown95 = set; //Unset(?) when Tom Nook speaks with you after your house was built
			break;
			case 95:
				playerFlag->HouseUpgradeFinished = set; 
			break;
			case 96:
				playerFlag->Unknown97 = set; 
			break;
			case 97:
				playerFlag->Unknown98 = set; 
			break;
			case 98:
				playerFlag->Unknown99 = set; 
			break;
			case 99:
				playerFlag->Unknown100 = set; 
			break;
			case 100:
				playerFlag->Unknown101 = set; 
			break;
			case 101:
				playerFlag->MuseumExhibitExplained = set; 
			break;
			case 102:
				playerFlag->Unknown103 = set; 
			break;
			case 103:
				playerFlag->Unknown104 = set; 
			break;
			case 104:
				playerFlag->PermitApprovalArrived = set; 
			break;
			case 105:
				playerFlag->MayorJobIntroduction = set; 
			break;
			case 106:
				playerFlag->PermitApproval = set; 
			break;
			case 107:
				playerFlag->PermitIntroduction = set; 
			break;
			case 108:
				playerFlag->CitizenSatisfactionExplained = set; 
			break;
			case 109:
				playerFlag->TownFlagExplained = set; 
			break;
			case 110:
				playerFlag->TownTuneExplained = set; 
			break;
			case 111:
				playerFlag->PWPExplained = set; 
			break;
			case 112:
				playerFlag->OrdinanceExplained = set; 
			break;
			case 113:
				playerFlag->Unknown114 = set; 
			break;
			case 114:
				playerFlag->Unknown115 = set; 
			break;
			case 115:
				playerFlag->PermitFinished = set; 
			break;
			case 116:
				playerFlag->Unknown117 = set; 
			break;
			case 117:
				playerFlag->Unknown118 = set; 
			break;
			case 118:
				playerFlag->HasBeeSting = set; 
			break;
			case 119:
				playerFlag->Unknown120 = set; 
			break;
			case 120:
				playerFlag->EditDesignIntro = set; 
			break;
			case 121:
				playerFlag->Unknown122 = set; 
			break;
			case 122:
				playerFlag->Unknown123 = set; 
			break;
			case 123:
				playerFlag->CanTravel = set; 
			break; //i.e. can use train
			case 124:
				playerFlag->HasTPCPicture = set; 
			break;
			case 125:
				playerFlag->Unknown126 = set; 
			break;
			case 126:
				playerFlag->Unknown127 = set; 
			break;
			case 127:
				playerFlag->Unknown128 = set; 
			break;
			case 128:
				playerFlag->Unknown129 = set; 
			break;
			case 129:
				playerFlag->Unknown130 = set; 
			break;
			case 130:
				playerFlag->Unknown131 = set; 
			break;
			case 131:
				playerFlag->Unknown132 = set; 
			break;
			case 132:
				playerFlag->BefriendSable1 = set; 
			break;
			case 133:
				playerFlag->Unknown134 = set; 
			break;
			case 134:
				playerFlag->BefriendSable2 = set; 
			break;
			case 135:
				playerFlag->Unknown136 = set; 
			break;
			case 136:
				playerFlag->BefriendSable3 = set; 
			break;
			case 137:
				playerFlag->Unknown138 = set; 
			break;
			case 138:
				playerFlag->Unknown139 = set; //Set when Tom Nook speaks with you after your house was built
			break;
			case 139:
				playerFlag->Unknown140 = set; 
			break;
			case 140:
				playerFlag->Unknown141 = set; 
			break;
			case 141:
				playerFlag->Unknown142 = set; 
			break;
			case 142:
				playerFlag->Unknown143 = set; 
			break;
			case 143:
				playerFlag->Unknown144 = set; 
			break;
			case 144:
				playerFlag->Unknown145 = set; 
			break;
			case 145:
				playerFlag->Unknown146 = set; 
			break;
			case 146:
				playerFlag->TalkToLyleForTheFirstTime = set; 
			break;
			case 147:
				playerFlag->Unknown148 = set; //Set when Tom Nook speaks with you after your house was built
			break;
			case 148:
				playerFlag->Unknown149 = set; 
			break;
			case 149:
				playerFlag->Unknown150 = set; 
			break;
			case 150:
				playerFlag->Unknown151 = set; 
			break;
			case 151:
				playerFlag->UnlockedKappn = set; 
			break;
			case 152:
				playerFlag->LyleIntroduction = set; 
			break;
			case 153:
				playerFlag->Unknown154 = set; 
			break;
			case 154:
				playerFlag->RecievedHHSIntro = set; 
			break;
			case 155:
				playerFlag->Unknown156 = set; 
			break;
			case 156:
				playerFlag->Unknown157 = set; 
			break;
			case 157:
				playerFlag->Unknown158 = set; 
			break;
			case 158:
				playerFlag->Unknown159 = set; 
			break;
			case 159:
				playerFlag->Unknown160 = set; 
			break;
			case 160:
				playerFlag->Unknown161 = set; 
			break;
			case 161:
				playerFlag->Unknown162 = set; 
			break;
			case 162:
				playerFlag->FinishedShrunkSignatures = set; 
			break;
			case 163:
				playerFlag->Unknown164 = set; 
			break;
			case 164:
				playerFlag->Unknown165 = set; 
			break;
			case 165:
				playerFlag->Unknown166 = set; 
			break;
			case 166:
				playerFlag->Unknown167 = set; 
			break;
			case 167:
				playerFlag->Unknown168 = set; 
			break;
			case 168:
				playerFlag->Unknown169 = set; 
			break;
			case 169:
				playerFlag->Unknown170 = set; 
			break;
			case 170:
				playerFlag->Unknown171 = set; 
			break;
			case 171:
				playerFlag->Unknown172 = set; 
			break;
			case 172:
				playerFlag->IsabelleWallpaperGiftForGettingHouse = set; 
			break;
			case 173:
				playerFlag->KickIntroduced = set; 
			break;
			case 174:
				playerFlag->Unknown175 = set; 
			break;
			case 175:
				playerFlag->Unknown176 = set; 
			break;
			case 176:
				playerFlag->Unknown177 = set; 
			break;
			case 177:
				playerFlag->Unknown178 = set; 
			break;
			case 178:
				playerFlag->Unknown179 = set; 
			break;
			case 179:
				playerFlag->Unknown180 = set; 
			break;
			case 180:
				playerFlag->Unknown181 = set; 
			break;
			case 181:
				playerFlag->Unknown182 = set; 
			break;
			case 182:
				playerFlag->Unknown183 = set; 
			break;
			case 183:
				playerFlag->Unknown184 = set; 
			break;
			case 184:
				playerFlag->Unknown185 = set; 
			break;
			case 185:
				playerFlag->Unknown186 = set; 
			break;
			case 186:
				playerFlag->Unknown187 = set; 
			break;
			case 187:
				playerFlag->Unknown188 = set; 
			break;
			case 188:
				playerFlag->Unknown189 = set; 
			break;
			case 189:
				playerFlag->Unknown190 = set; 
			break;
			case 190:
				playerFlag->Unknown191 = set; 
			break;
			case 191:
				playerFlag->Unknown192 = set; 
			break;
			case 192:
				playerFlag->Unknown193 = set; 
			break;
			case 193:
				playerFlag->Unknown194 = set; 
			break;
			case 194:
				playerFlag->Unknown195 = set; 
			break;
			case 195:
				playerFlag->Unknown196 = set; 
			break;
			case 196:
				playerFlag->Unknown197 = set; 
			break;
			case 197:
				playerFlag->Unknown198 = set; 
			break;
			case 198:
				playerFlag->Unknown199 = set; 
			break;
			case 199:
				playerFlag->Unknown200 = set; 
			break;
			case 200:
				playerFlag->Unknown201 = set; 
			break;
			case 201:
				playerFlag->Unknown202 = set; 
			break;
			case 202:
				playerFlag->Unknown203 = set; 
			break;
			case 203:
				playerFlag->Unknown204 = set; 
			break;
			case 204:
				playerFlag->Unknown205 = set; 
			break;
			case 205:
				playerFlag->Unknown206 = set; 
			break;
			case 206:
				playerFlag->Unknown207 = set; 
			break;
			case 207:
				playerFlag->KnowsPermitRequirements = set; 
			break;
			case 208:
				playerFlag->TomNookToldYouFirstPayment = set; 
			break;
			case 209:
				playerFlag->Permit_Points1 = set; 
			break;
			case 210:
				playerFlag->Permit_Points2 = set; 
			break;
			case 211:
				playerFlag->Permit_Points3 = set; 
			break;
			case 212:
				playerFlag->Permit_Points4 = set; 
			break;
			case 213:
				playerFlag->Permit_Points5 = set; 
			break;
			case 214:
				playerFlag->Permit_Points6 = set; 
			break;
			case 215:
				playerFlag->Permit_Points7 = set; 
			break;
			case 216:
				playerFlag->Permit_Points8 = set; 
			break;
			case 217:
				playerFlag->Permit_Points9 = set; 
			break;
			case 218:
				playerFlag->Permit_Points10 = set; 
			break;
			case 219:
				playerFlag->Unknown220 = set; 
			break;
			case 220:
				playerFlag->Unknown221 = set; 
			break;
			case 221:
				playerFlag->Unknown222 = set; 
			break;
			case 222:
				playerFlag->Unknown223 = set; 
			break;
			case 223:
				playerFlag->Unknown224 = set; 
			break;
			case 224:
				playerFlag->Unknown225 = set; 
			break;
			case 225:
				playerFlag->Unknown226 = set; 
			break;
			case 226:
				playerFlag->Unknown227 = set; 
			break;
			case 227:
				playerFlag->Unknown228 = set; 
			break;
			case 228:
				playerFlag->Unknown229 = set; 
			break;
			case 229:
				playerFlag->Unknown230 = set; 
			break;
			case 230:
				playerFlag->Unknown231 = set; 
			break;
			case 231:
				playerFlag->Unknown232 = set; 
			break;
			case 232:
				playerFlag->Unknown233 = set; 
			break;
			case 233:
				playerFlag->Unknown234 = set; 
			break;
			case 234:
				playerFlag->Unknown235 = set; 
			break;
			case 235:
				playerFlag->Unknown236 = set; 
			break;
			case 236:
				playerFlag->Unknown237 = set; 
			break;
			case 237:
				playerFlag->Unknown238 = set; 
			break;
			case 238:
				playerFlag->Unknown239 = set; 
			break;
			case 239:
				playerFlag->Unknown240 = set; 
			break;
			case 240:
				playerFlag->Unknown241 = set; 
			break;
			case 241:
				playerFlag->Unknown242 = set; 
			break;
			case 242:
				playerFlag->Unknown243 = set; 
			break;
			case 243:
				playerFlag->Unknown244 = set; 
			break;
			case 244:
				playerFlag->Unknown245 = set; 
			break;
			case 245:
				playerFlag->Unknown246 = set; 
			break;
			case 246:
				playerFlag->Unknown247 = set; 
			break;
			case 247:
				playerFlag->Unknown248 = set; 
			break;
			case 248:
				playerFlag->Unknown249 = set; 
			break;
			case 249:
				playerFlag->Unknown250 = set; 
			break;
			case 250:
				playerFlag->HasClubTortimerMembership = set; 
			break;
			case 251:
				playerFlag->ClubTortimerFirstAsked = set; 
			break;
			case 252:
				playerFlag->ClubTortimerRulesExplained = set; 
			break;
			case 253:
				playerFlag->Unknown254 = set; 
			break;
			case 254:
				playerFlag->Unknown255 = set; 
			break;
			case 255:
				playerFlag->Unknown256 = set; 
			break;
			case 256:
				playerFlag->Unknown257 = set; 
			break;
			case 257:
				playerFlag->Unknown258 = set; 
			break;
			case 258:
				playerFlag->Unknown259 = set; 
			break;
			case 259:
				playerFlag->Unknown260 = set; 
			break;
			case 260:
				playerFlag->Unknown261 = set; 
			break;
			case 261:
				playerFlag->Unknown262 = set; 
			break;
			case 262:
				playerFlag->Unknown263 = set; 
			break;
			case 263:
				playerFlag->Unknown264 = set; 
			break;
			case 264:
				playerFlag->Unknown265 = set; 
			break;
			case 265:
				playerFlag->Unknown266 = set; 
			break;
			case 266:
				playerFlag->Unknown267 = set; 
			break;
			case 267:
				playerFlag->Unknown268 = set; 
			break;
			case 268:
				playerFlag->Unknown269 = set; 
			break;
			case 269:
				playerFlag->Unknown270 = set; 
			break;
			case 270:
				playerFlag->Unknown271 = set; 
			break;
			case 271:
				playerFlag->Unknown272 = set; 
			break;
			case 272:
				playerFlag->Unknown273 = set; 
			break;
			case 273:
				playerFlag->Unknown274 = set; 
			break;
			case 274:
				playerFlag->Unknown275 = set; 
			break;
			case 275:
				playerFlag->Unknown276 = set; 
			break;
			case 276:
				playerFlag->Unknown277 = set; 
			break;
			case 277:
				playerFlag->Unknown278 = set; 
			break;
			case 278:
				playerFlag->Unknown279 = set; 
			break;
			case 279:
				playerFlag->Unknown280 = set; 
			break;
			case 280:
				playerFlag->Unknown281 = set; 
			break;
			case 281:
				playerFlag->Unknown282 = set; 
			break;
			case 282:
				playerFlag->Unknown283 = set; 
			break;
			case 283:
				playerFlag->Unknown284 = set; 
			break;
			case 284:
				playerFlag->Unknown285 = set; 
			break;
			case 285:
				playerFlag->Unknown286 = set; 
			break;
			case 286:
				playerFlag->Unknown287 = set; 
			break;
			case 287:
				playerFlag->Unknown288 = set; 
			break;
			case 288:
				playerFlag->Unknown289 = set; 
			break;
			case 289:
				playerFlag->Unknown290 = set; 
			break;
			case 290:
				playerFlag->Unknown291 = set; 
			break;
			case 291:
				playerFlag->Unknown292 = set; 
			break;
			case 292:
				playerFlag->Unknown293 = set; 
			break;
			case 293:
				playerFlag->Unknown294 = set; 
			break;
			case 294:
				playerFlag->Unknown295 = set; 
			break;
			case 295:
				playerFlag->Unknown296 = set; 
			break;
			case 296:
				playerFlag->Unknown297 = set; 
			break;
			case 297:
				playerFlag->Unknown298 = set; 
			break;
			case 298:
				playerFlag->Unknown299 = set; 
			break;
			case 299:
				playerFlag->Unknown300 = set; 
			break;
			case 300:
				playerFlag->Unknown301 = set; 
			break;
			case 301:
				playerFlag->Unknown302 = set; 
			break;
			case 302:
				playerFlag->Unknown303 = set; 
			break;
			case 303:			
				playerFlag->Unknown304 = set; 
			break;
			case 304:
				playerFlag->Unknown305 = set; 
			break;
			case 305:
				playerFlag->Unknown306 = set; 
			break;
			case 306:
				playerFlag->Unknown307 = set; 
			break;
			case 307:
				playerFlag->Unknown308 = set; 
			break;
			case 308:
				playerFlag->Unknown309 = set; 
			break;
			case 309:
				playerFlag->Unknown310 = set; 
			break;
			case 310:
				playerFlag->Unknown311 = set; 
			break;
			case 311:
				playerFlag->Unknown312 = set; 
			break;
			case 312:
				playerFlag->Unknown313 = set; 
			break;
			case 313:
				playerFlag->Unknown314 = set; 
			break;
			case 314:
				playerFlag->SecretStorageExplained = set; 
			break;
			case 315:
				playerFlag->Unknown316 = set; 
			break;
			case 316:
				playerFlag->Unknown317 = set; 
			break;
			case 317:
				playerFlag->Unknown318 = set; 
			break;
			case 318:
				playerFlag->BuildingSecretStorage = set; 
			break;
			case 319:
				playerFlag->UnlockedSecretStorage = set; 
			break;
			case 320:
				playerFlag->Unknown321 = set; 
			break;
			case 321:
				playerFlag->Unknown322 = set; 
			break;
			case 322:
				playerFlag->Unknown323 = set; 
			break;
			case 323:
				playerFlag->Unknown324 = set; 
			break;
			case 324:
				playerFlag->Unknown325 = set; 
			break;
			case 325:
				playerFlag->Unknown326 = set; 
			break;
			case 326:
				playerFlag->Unknown327 = set; 
			break;
			case 327:
				playerFlag->Unknown328 = set; 
			break;
			case 328:
				playerFlag->Unknown329 = set; 
			break;
			case 329:
				playerFlag->Unknown330 = set; 
			break;
			case 330:
				playerFlag->Unknown331 = set; 
			break;
			case 331:
				playerFlag->Unknown332 = set; 
			break;
			case 332:
				playerFlag->Unknown333 = set; 
			break;
			case 333:
				playerFlag->Unknown334 = set; 
			break;
			case 334:
				playerFlag->Unknown335 = set; 
			break;
			case 335:
				playerFlag->Unknown336 = set; 
			break;
			case 336:
				playerFlag->Unknown337 = set; 
			break;
			case 337:
				playerFlag->Unknown338 = set; 
			break;
			case 338:
				playerFlag->Unknown339 = set; 
			break;
			case 339:
				playerFlag->Unknown340 = set; 
			break;
			case 340:
				playerFlag->Unknown341 = set; 
			break;
			case 341:
				playerFlag->Unknown342 = set; 
			break;
			case 342:
				playerFlag->Unknown343 = set; 
			break;
			case 343:
				playerFlag->Unknown344 = set; 
			break;
			case 344:
				playerFlag->Unknown345 = set; 
			break;
			case 345:
				playerFlag->Unknown346 = set; 
			break;
			case 346:
				playerFlag->Unknown347 = set; 
			break;
			case 347:
				playerFlag->Unknown348 = set; 
			break;
			case 348:
				playerFlag->Unknown349 = set; 
			break;
			case 349:
				playerFlag->Unknown350 = set; 
			break;
			case 350:
				playerFlag->Unknown351 = set; 
			break;
			case 351:
				playerFlag->Unknown352 = set; 
			break;
			case 352:
				playerFlag->Unknown353 = set; 
			break;
			case 353:
				playerFlag->Unknown354 = set; 
			break;
			case 354:
				playerFlag->Unknown355 = set; 
			break;
			case 355:
				playerFlag->UnlockedHouseEditor = set; 
			break;
			case 356:
				playerFlag->Unknown357 = set; 
			break;
			case 357:
				playerFlag->Unknown358 = set; 
			break;
			case 358:
				playerFlag->Unknown359 = set; 
			break;
			case 359:
				playerFlag->RecievedCATIntro = set; 
			break;
			case 360:
				playerFlag->Unknown361 = set; 
			break;
			case 361:
				playerFlag->Unknown362 = set; 
			break;
			case 362:
				playerFlag->Unknown363 = set; 
			break;
			case 363:
				playerFlag->Unknown364 = set; 
			break;
			case 364:
				playerFlag->Unknown365 = set; 
			break;
			case 365:
				playerFlag->Unknown366 = set; 
			break;
			case 366:
				playerFlag->Unknown367 = set; 
			break;
			case 367:
				playerFlag->Unknown368 = set; 
			break;
			case 368:
				playerFlag->Unknown369 = set; 
			break;
			case 369:
				playerFlag->Unknown370 = set; 
			break;
			case 370:
				playerFlag->Unknown371 = set; 
			break;
			case 371:
				playerFlag->Unknown372 = set; 
			break;
			case 372:
				playerFlag->Unknown373 = set; 
			break;
			case 373:
				playerFlag->Unknown374 = set; 
			break;
			case 374:
				playerFlag->Unknown375 = set; 
			break;
			case 375:
				playerFlag->Unknown376 = set; 
			break;
			case 376:
				playerFlag->Unknown377 = set; 
			break;
			case 377:
				playerFlag->Unknown378 = set; 
			break;
			case 378:
				playerFlag->Unknown379 = set; 
			break;
			case 379:
				playerFlag->Unknown380 = set; 
			break;
			case 380:
				playerFlag->Unknown381 = set; 
			break;
			case 381:
				playerFlag->Unknown382 = set; 
			break;
			case 382:
				playerFlag->CanUseCensusMenu = set; 
			break;
			case 383:
				playerFlag->Unknown384 = set; 
			break;
			case 384:
				playerFlag->Unknown385 = set; 
			break;
			case 385:
				playerFlag->Unknown386 = set; 
			break;
			case 386:
				playerFlag->Unknown387 = set; 
			break;
			case 387:
				playerFlag->Unknown388 = set; 
			break;
			case 388:
				playerFlag->Unknown389 = set; 
			break;
			case 389:
				playerFlag->Unknown390 = set; 
			break;
			case 390:
				playerFlag->Unknown391 = set; 
			break;
			case 391:
				playerFlag->Unknown392 = set; 
			break;
			case 392:
				playerFlag->Unknown393 = set; 
			break;
			case 393:
				playerFlag->Unknown394 = set; 
			break;
			case 394:
				playerFlag->Unknown395 = set; 
			break;
			case 395:
				playerFlag->Unknown396 = set; 
			break;
			case 396:
				playerFlag->Unknown397 = set; 
			break;
			case 397:
				playerFlag->Unknown398 = set; 
			break;
			case 398:
				playerFlag->Unknown399 = set; 
			break;
			case 399:
				playerFlag->Unknown400 = set; 
			break;
			case 400:
				playerFlag->Unknown401 = set; 
			break;
			case 401:
				playerFlag->Unknown402 = set; 
			break;
			case 402:
				playerFlag->Unknown403 = set; 
			break;
			case 403:
				playerFlag->Unknown404 = set; 
			break;
			case 404:
				playerFlag->Unknown405 = set; 
			break;
			case 405:
				playerFlag->Unknown406 = set; 
			break;
			case 406:
				playerFlag->Unknown407 = set; 
			break;
			case 407:
				playerFlag->Unknown408 = set; 
			break;
			case 408:
				playerFlag->Unknown409 = set; 
			break;
			case 409:
				playerFlag->Unknown410 = set; 
			break;
			case 410:
				playerFlag->Unknown411 = set; 
			break;
			case 411:
				playerFlag->Unknown412 = set; 
			break;
			case 412:
				playerFlag->Unknown413 = set; 
			break;
			case 413:
				playerFlag->Unknown414 = set; 
			break;
			case 414:
				playerFlag->Unknown415 = set; 
			break;
			case 415:
				playerFlag->Unknown416 = set; 
			break;
		}
	}

	struct PlayerFlagsSTRUCT {
		u8 data[0x34];
	};

	static u8 lastFlags[0x34] = {0};

	bool GetFlag(PlayerFlagsSTRUCT* flags, int idx) {
		return (flags->data[idx >> 3] >> (idx & 7)) & 1;
	}

	void CheckPlayerFlags(PlayerFlagsSTRUCT* flags) {
		static bool initialized = false;

		if (!initialized) {
			memcpy(lastFlags, flags->data, 0x34);
			initialized = true;
			return;
		}

		for (int byte = 0; byte < 0x34; byte++) {
			u8 diff = lastFlags[byte] ^ flags->data[byte];
			if (!diff) continue;

			for (int bit = 0; bit < 8; bit++) {
				if (diff & (1 << bit)) {
					int flagIndex = byte * 8 + bit;
					OSDExtras::Notify(Utils::Format("Player Flag %d changed to %d", flagIndex+1, GetFlag(flags, flagIndex)));
				}
			}
		}

		memcpy(lastFlags, flags->data, 0x34);
	}

	struct TownFlagsSTRUCT {
		u8 data[17];
	};

	static u8 lastTownFlags[17] = {0};

	bool GetTownFlag(TownFlagsSTRUCT* flags, int idx) {
		return (flags->data[idx >> 3] >> (idx & 7)) & 1;
	}

	void CheckTownFlags(TownFlagsSTRUCT* flags) {
		static bool initialized = false;

		if (!initialized) {
			memcpy(lastTownFlags, flags->data, 17);
			initialized = true;
			return;
		}

		for (int byte = 0; byte < 17; byte++) {
			u8 diff = lastTownFlags[byte] ^ flags->data[byte];
			if (!diff) continue;

			for (int bit = 0; bit < 8; bit++) {
				if (diff & (1 << bit)) {
					int flagIndex = byte * 8 + bit;
					OSDExtras::Notify(Utils::Format("Town Flag %d changed to %d", flagIndex+1, GetTownFlag(flags, flagIndex)));
				}
			}
		}

		memcpy(lastTownFlags, flags->data, 17);
	}

	static PlayerHouseExterior lastExterior;

	void CheckHouseSaveData(ACNL_TownData *town) {
		static bool initialized = false;

		if (!initialized) {
			lastExterior = town->PlayerHouse[0].exterior2;
			initialized = true;
			return;
		}

		if (town->PlayerHouse[0].exterior2.HouseSize != lastExterior.HouseSize)
			OSDExtras::Notify(Utils::Format("House Size changed to %02X", town->PlayerHouse[0].exterior2.HouseSize));
		if (town->PlayerHouse[0].exterior2.HouseStyle != lastExterior.HouseStyle)
			OSDExtras::Notify(Utils::Format("House Style changed to %02X", town->PlayerHouse[0].exterior2.HouseStyle));
		if (town->PlayerHouse[0].exterior2.HouseDoorShape != lastExterior.HouseDoorShape)
			OSDExtras::Notify(Utils::Format("House Door Shape changed to %02X", town->PlayerHouse[0].exterior2.HouseDoorShape));
		if (town->PlayerHouse[0].exterior2.HouseBrick != lastExterior.HouseBrick)
			OSDExtras::Notify(Utils::Format("House Brick changed to %02X", town->PlayerHouse[0].exterior2.HouseBrick));
		if (town->PlayerHouse[0].exterior2.HouseRoof != lastExterior.HouseRoof)
			OSDExtras::Notify(Utils::Format("House Roof changed to %02X", town->PlayerHouse[0].exterior2.HouseRoof));
		if (town->PlayerHouse[0].exterior2.HouseDoor != lastExterior.HouseDoor)
			OSDExtras::Notify(Utils::Format("House Door changed to %02X", town->PlayerHouse[0].exterior2.HouseDoor));
		if (town->PlayerHouse[0].exterior2.HouseFence != lastExterior.HouseFence)
			OSDExtras::Notify(Utils::Format("House Fence changed to %02X", town->PlayerHouse[0].exterior2.HouseFence));
		if (town->PlayerHouse[0].exterior2.HousePavement != lastExterior.HousePavement)
			OSDExtras::Notify(Utils::Format("House Pavement changed to %02X", town->PlayerHouse[0].exterior2.HousePavement));
		if (town->PlayerHouse[0].exterior2.HouseMailBox != lastExterior.HouseMailBox)
			OSDExtras::Notify(Utils::Format("House MailBox changed to %02X", town->PlayerHouse[0].exterior2.HouseMailBox));

		lastExterior = town->PlayerHouse[0].exterior2;
	}

	const std::vector<std::string> roomNames = {
		"Middle Room", "SecondRoom", "BasementRoom", "RightRoom", "LeftRoom", "BackRoom"
	};

	static RoomFlags lastRoomflags[6];

	void CheckRoomFlags(int roomIndex, ACNL_Room *room) {
		static bool initialized[6] = {
			false, false, false, false, false, false
		};

		if (!initialized[roomIndex]) {
			lastRoomflags[roomIndex] = room->flags;
			initialized[roomIndex] = true;
			return;
		}

		if (room->flags.BrightLight != lastRoomflags[roomIndex].BrightLight)
			OSDExtras::Notify(Utils::Format("Bright Light changed to %02X for %s", room->flags.BrightLight, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown2 != lastRoomflags[roomIndex].Unknown2)
			OSDExtras::Notify(Utils::Format("Unknown2 changed to %02X for %s", room->flags.Unknown2, roomNames.at(roomIndex).c_str()));
		if (room->flags.RegularLight != lastRoomflags[roomIndex].RegularLight)
			OSDExtras::Notify(Utils::Format("Regular Light changed to %02X for %s", room->flags.RegularLight, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown4 != lastRoomflags[roomIndex].Unknown4)
			OSDExtras::Notify(Utils::Format("Unknown4 changed to %02X for %s", room->flags.Unknown4, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown5 != lastRoomflags[roomIndex].Unknown5)
			OSDExtras::Notify(Utils::Format("Unknown5 changed to %02X for %s", room->flags.Unknown5, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown6 != lastRoomflags[roomIndex].Unknown6)
			OSDExtras::Notify(Utils::Format("Unknown6 changed to %02X for %s", room->flags.Unknown6, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown7 != lastRoomflags[roomIndex].Unknown7)
			OSDExtras::Notify(Utils::Format("Unknown7 changed to %02X for %s", room->flags.Unknown7, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown8 != lastRoomflags[roomIndex].Unknown8)
			OSDExtras::Notify(Utils::Format("Unknown8 changed to %02X for %s", room->flags.Unknown8, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown9 != lastRoomflags[roomIndex].Unknown9)
			OSDExtras::Notify(Utils::Format("Unknown9 changed to %02X for %s", room->flags.Unknown9, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown10 != lastRoomflags[roomIndex].Unknown10)
			OSDExtras::Notify(Utils::Format("Unknown10 changed to %02X for %s", room->flags.Unknown10, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown11 != lastRoomflags[roomIndex].Unknown11)
			OSDExtras::Notify(Utils::Format("Unknown11 changed to %02X for %s", room->flags.Unknown11, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown12 != lastRoomflags[roomIndex].Unknown12)
			OSDExtras::Notify(Utils::Format("Unknown12 changed to %02X for %s", room->flags.Unknown12, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown13 != lastRoomflags[roomIndex].Unknown13)
			OSDExtras::Notify(Utils::Format("Unknown13 changed to %02X for %s", room->flags.Unknown13, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown14 != lastRoomflags[roomIndex].Unknown14)
			OSDExtras::Notify(Utils::Format("Unknown14 changed to %02X for %s", room->flags.Unknown14, roomNames.at(roomIndex).c_str()));
		if (room->flags.LowLight != lastRoomflags[roomIndex].LowLight)
			OSDExtras::Notify(Utils::Format("Low Light changed to %02X for %s", room->flags.LowLight, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown16 != lastRoomflags[roomIndex].Unknown16)
			OSDExtras::Notify(Utils::Format("Unknown16 changed to %02X for %s", room->flags.Unknown16, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown17 != lastRoomflags[roomIndex].Unknown17)
			OSDExtras::Notify(Utils::Format("Unknown17 changed to %02X for %s", room->flags.Unknown17, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown18 != lastRoomflags[roomIndex].Unknown18)
			OSDExtras::Notify(Utils::Format("Unknown18 changed to %02X for %s", room->flags.Unknown18, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown19 != lastRoomflags[roomIndex].Unknown19)
			OSDExtras::Notify(Utils::Format("Unknown19 changed to %02X for %s", room->flags.Unknown19, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown20 != lastRoomflags[roomIndex].Unknown20)
			OSDExtras::Notify(Utils::Format("Unknown20 changed to %02X for %s", room->flags.Unknown20, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown21 != lastRoomflags[roomIndex].Unknown21)
			OSDExtras::Notify(Utils::Format("Unknown21 changed to %02X for %s", room->flags.Unknown21, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown22 != lastRoomflags[roomIndex].Unknown22)
			OSDExtras::Notify(Utils::Format("Unknown22 changed to %02X for %s", room->flags.Unknown22, roomNames.at(roomIndex).c_str()));
		if (room->flags.LightSwitchState != lastRoomflags[roomIndex].LightSwitchState)
			OSDExtras::Notify(Utils::Format("Light Switch State changed to %02X for %s", room->flags.LightSwitchState, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown24 != lastRoomflags[roomIndex].Unknown24)
			OSDExtras::Notify(Utils::Format("Unknown24 changed to %02X for %s", room->flags.Unknown24, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown25 != lastRoomflags[roomIndex].Unknown25)
			OSDExtras::Notify(Utils::Format("Unknown25 changed to %02X for %s", room->flags.Unknown25, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown26 != lastRoomflags[roomIndex].Unknown26)
			OSDExtras::Notify(Utils::Format("Unknown26 changed to %02X for %s", room->flags.Unknown26, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown27 != lastRoomflags[roomIndex].Unknown27)
			OSDExtras::Notify(Utils::Format("Unknown27 changed to %02X for %s", room->flags.Unknown27, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown28 != lastRoomflags[roomIndex].Unknown28)
			OSDExtras::Notify(Utils::Format("Unknown28 changed to %02X for %s", room->flags.Unknown28, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown29 != lastRoomflags[roomIndex].Unknown29)
			OSDExtras::Notify(Utils::Format("Unknown29 changed to %02X for %s", room->flags.Unknown29, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown30 != lastRoomflags[roomIndex].Unknown30)
			OSDExtras::Notify(Utils::Format("Unknown30 changed to %02X for %s", room->flags.Unknown30, roomNames.at(roomIndex).c_str()));
		if (room->flags.RoomSize != lastRoomflags[roomIndex].RoomSize)
			OSDExtras::Notify(Utils::Format("Room Size changed to %02X for %s", room->flags.RoomSize, roomNames.at(roomIndex).c_str()));
		if (room->flags.IsRoomUpgrading != lastRoomflags[roomIndex].IsRoomUpgrading)
			OSDExtras::Notify(Utils::Format("IsRoomUpgrading changed to %02X for %s", room->flags.IsRoomUpgrading, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown33 != lastRoomflags[roomIndex].Unknown33)
			OSDExtras::Notify(Utils::Format("Unknown33 changed to %02X for %s", room->flags.Unknown33, roomNames.at(roomIndex).c_str()));
		if (room->flags.Unknown34 != lastRoomflags[roomIndex].Unknown34)
			OSDExtras::Notify(Utils::Format("Unknown34 changed to %02X for %s", room->flags.Unknown34, roomNames.at(roomIndex).c_str()));

		lastRoomflags[roomIndex] = room->flags;
	}

	static ACNL_Room lastRoom[6];

	void CheckMiniRoomStruct(int roomIndex, ACNL_Room *room) {
		static bool initialized[6] = {
			false, false, false, false, false, false
		};

		if (!initialized[roomIndex]) {
			lastRoom[roomIndex] = *room;
			initialized[roomIndex] = true;
			return;
		}

		if (room->UnkItem4 != lastRoom[roomIndex].UnkItem4)
			OSDExtras::Notify(Utils::Format("UnkItem4 changed to %04X for %s", room->UnkItem4.ID, roomNames.at(roomIndex).c_str()));

		lastRoom[roomIndex] = *room;
	}

	void displaySaveFlagDifference(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();
		ACNL_TownData *town = Town::GetSaveData();

		if (!town) 
			return;

		TownFlagsSTRUCT tf;
		u8* tarr = tf.data;
		u32 toffset = Save::GetInstance()->AtOffset(0x621CF);
		u8* tsrc = (u8*)toffset;

		for (size_t i = 0; i < 17; i++) {
			tarr[i] = tsrc[i]; 
		}

		CheckTownFlags(&tf);

		CheckHouseSaveData(town);

		ACNL_PlayerHouse *house = &town->PlayerHouse[0];
		if (house) {
			ACNL_Room* rooms[6] = {
				&house->MiddleRoom,
				&house->SecondRoom,
				&house->BasementRoom,
				&house->RightRoom,
				&house->LeftRoom,
				&house->BackRoom
			};

			for (u32 i = 0; i < 6; i++) {
				CheckRoomFlags(i, rooms[i]);
				CheckMiniRoomStruct(i, rooms[i]);
			}
		}

		if(!player) 
			return;

		PlayerFlagsSTRUCT pf;
		u8* arr = pf.data;
		u32 offset = Player::GetSaveOffset(4) + 0x5700;
		u8* src = (u8*)offset;

		for (size_t i = 0; i < 0x34; i++) {
			arr[i] = src[i]; 
		}

		CheckPlayerFlags(&pf);

		static u32 index = 0;

		if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			Keyboard KB("");
			KB.Open(index, index);
		}
		if(Controller::IsKeysPressed(Key::L + Key::DPadUp)) {
			SetPlayerFlag(&player->PlayerFlags, index-1, true);
		}
		if(Controller::IsKeysPressed(Key::L + Key::DPadDown)) {
			SetPlayerFlag(&player->PlayerFlags, index-1, false);
		}
	}

	/*u64 GetFriendCode(u8 pIndex) {
		u32 gPoint = *(u32 *)Address(0x954648).addr;
		if(gPoint == 0)
			return 0;

		frdInit(false);

		u32 pID = *(u32 *)((gPoint + 0xC0) + (0x18 * pIndex));

		if((int)pID == -3)
			return 0;
		
		u64 fCode = 0;
		FRD_PrincipalIdToFriendCode(pID, &fCode);

		frdExit();

		return fCode;
	}*/

//Hook
	bool randomfall(void) {
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

	/*
	//u32 Currentmap(0x9B5A24, 0x9B4A24, 0x9B4A24, 0x9B4A24, 0x9AEA04, 0x9ADA04, 0x9ADA24, 0x9ADA24);
		void ItemPlacer(u32* ItemID) {
			static FUNCT func(0x581E3C);
			func.Call<void>(GameHelper::GetCurrentMap(), ItemID, PlayerClass::GetInstance()->GetCoordinates(), 0);
		}
	*/

	/*
	Function which sets coordinates and all to storage for player
	68dee8(u32 pinstance)
	*/
	
	void unlockCroRegion(MenuEntry *entry) {
		static std::string str = "";
		static u32 buffer = 0;
 
		if(Controller::IsKeysPressed(Key::R + Key::DPadUp)) {
			if(Wrap::KB<std::string>("Enter Cro Name:", false, 15, str, str)) {
				if(CRO::GetMemAddress(str.c_str(), buffer)) {
					if(CRO::WritePermToggle(buffer, true)) {
						OSDExtras::Notify(Utils::Format("Unlocked %s | Address: %08X", str.c_str(), buffer));
					}
				}
			}
		}
		else if(Controller::IsKeysPressed(Key::R + Key::DPadDown)) {
			if(CRO::GetMemAddress(str.c_str(), buffer)) {
				if(CRO::WritePermToggle(buffer, false)) {
					OSDExtras::Notify(Utils::Format("Locked %s | Address: %08X", str.c_str(), buffer));
				}
			}
		}
	}

	void lightswitch(MenuEntry *entry) {
		static const Address TargetAddress(0x190EA8);
		static const Address Patch(0x1E7AD8);

		if(entry->WasJustActivated()) {
		//this disables the "non switchable light" flag to be written
			u32 val = 0;
			Process::Read32(TargetAddress.addr, val);
			Process::Write32(TargetAddress.addr, val + 0x146); //patch BL 0x1E7554 to BL 0x1E7A6C
		//this disables the reading of the "non switchable light" flag
			Process::Write32(Patch.addr, 0xE3A00000);
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
			Process::Read32(TargetAddress.addr, val);
			Process::Write32(TargetAddress.addr, val - 0x146);
			Process::Write32(Patch.addr, 0x05960028);
		}
	}

	void FishThrower(MenuEntry *entry) {				
		static u8 playerID = 0;
		static bool random = false;
		Item FishID(0x22E1, 0);

		static Address throwfish(0x5C2DAC);

		if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			if(playerID == 3) playerID = 0;		
			else playerID++;

			OSDExtras::Notify(Utils::Format("Player %02X selected!", playerID));
		}

		if(Controller::IsKeysPressed(Key::L + Key::DPadLeft)) {
			random = !random;
			OSDExtras::Notify("Random Mode: " << (random ? (Color::Green << "ON") : (Color::Red << "OFF")));
		}

		if(Controller::IsKeysDown(Key::R)) {
			if(random) {
				FishID.ID = Utils::Random(0x22E1, 0x234A);
				if(Game::GetItemCategory(FishID) != Item_Category::Fish) 
					return;
			}

			if(PlayerClass::GetInstance(playerID)->IsLoaded())
				throwfish.Call<void>(&FishID, PlayerClass::GetInstance(playerID)->GetCoordinates(), 1);
		}

		if(Controller::IsKeysDown(Key::R + Key::DPadUp)) 
			Wrap::KB<u16>("Set Fish ID:", true, 4, FishID.ID, FishID.ID);
	}
}
