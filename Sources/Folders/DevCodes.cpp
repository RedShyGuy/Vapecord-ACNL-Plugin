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

#include "Color.h"
#include "Files.h"

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

		Keyboard OKB(Language::getInstance()->get("KEY_CHOOSE_OPTION"));
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
			cmnOpt[0] = Color(pGreen) << Language::getInstance()->get("VECTOR_ENABLED");
		else 
			cmnOpt[0] = Color(pRed) << Language::getInstance()->get("VECTOR_DISABLED");

		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"));
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
			OSD::Notify(Utils::Format("Returned Value: %08X", result));
		}
    }

	void player_dumper(MenuEntry *entry) {
		ACNL_Player *player = Player::GetSaveData();

		if(!player) {
			MessageBox(Language::getInstance()->get("SAVE_PLAYER_NO"))();
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
		
		Keyboard optKb(Language::getInstance()->get("KEY_CHOOSE_OPTION"));
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
		
				Keyboard pKB(Language::getInstance()->get("KEY_SELECT_PLAYER"));
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
				playerFlag->Unknown5 = set; 
			break;
			case 5:
				playerFlag->Unknown6 = set; 
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
				playerFlag->MainStreetUnlocked = set; 
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
				playerFlag->Unknown31 = set; 
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
				playerFlag->Unknown37 = set; 
			break;
			case 37:
				playerFlag->Unknown38 = set; 
			break;
			case 38:
				playerFlag->Unknown39 = set; 
			break;
			case 39:
				playerFlag->Unknown40 = set; 
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
				playerFlag->Unknown88 = set; 
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
				playerFlag->Unknown92 = set; 
			break;
			case 92:
				playerFlag->Unknown93 = set; 
			break;
			case 93:
				playerFlag->Unknown94 = set; 
			break;
			case 94:
				playerFlag->Unknown95 = set; 
			break;
			case 95:
				playerFlag->Unknown96 = set; 
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
				playerFlag->Unknown102 = set; 
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
				playerFlag->Unknown109 = set; 
			break;
			case 109:
				playerFlag->Unknown110 = set; 
			break;
			case 110:
				playerFlag->Unknown111 = set; 
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
				playerFlag->Unknown121 = set; 
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
				playerFlag->Unknown139 = set; 
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
				playerFlag->Unknown147 = set; 
			break;
			case 147:
				playerFlag->Unknown148 = set; 
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
				playerFlag->Unknown153 = set; 
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
				playerFlag->Unknown173 = set; 
			break;
			case 173:
				playerFlag->Unknown174 = set; 
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
				playerFlag->Unknown209 = set; 
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
				playerFlag->Unknown315 = set; 
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
				playerFlag->Unknown319 = set; 
			break;
			case 319:
				playerFlag->Unknown320 = set; 
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
				playerFlag->Unknown356 = set; 
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
	
	u64 GetFriendCode(u8 pIndex) {
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
	}

	static void Cheat_EatEvents(Handle debug) {
        DebugEventInfo info;
        Result r;

        while(true) {
            if((r = svcGetProcessDebugEvent(&info, debug)) != 0) {
                if(r == (s32)(0xd8402009)) 
                    break;
            }
            svcContinueDebugEvent(debug, (DebugFlags)3);
        }
    }

	static u32 ReadWriteBuffer32 = 0;

	void set32(u32 addr, u32 value) {
		u32 pid = Process::GetProcessID();
		Handle processHandle;
		Result res = svcOpenProcess(&processHandle, pid);
		if (!R_SUCCEEDED(res))
			return;

    	Handle gHandle;
        res = svcDebugActiveProcess(&gHandle, Process::GetProcessID());
        if (!R_SUCCEEDED(res))
            return;

        Cheat_EatEvents(gHandle);

        *((u32*) (&ReadWriteBuffer32)) = value;
        svcWriteProcessMemory(gHandle, &ReadWriteBuffer32, addr, 4);

        svcCloseHandle(gHandle);
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

		if(Controller::IsKeysPressed(Key::ZL + Key::DPadDown)) {
			for(int i = 0; i < 4; ++i) {
				u64 fCode = GetFriendCode(i);
				if(fCode == 0) {
					OSD::Notify(Utils::Format("Player %d: Not Loaded!", i));
					continue;
				}

				char buffer[13];
				sprintf(buffer, "%012lld", fCode);
				std::string str = (std::string(buffer));
				GameKeyboard::SendMessage(Utils::Format("Player %d: %s - %s - %s", i, str.substr(0, 4).c_str(), str.substr(4, 4).c_str(), str.substr(8, 4).c_str()));
			}
		}

		if(Controller::IsKeysPressed(Key::ZR + Key::DPadDown)) {
			u32 buffer = 0;
			//CRO::GetMemAddress("Outdoor", buffer);
			set32(0xA00000, 0xE3A00000);
			//OSD::Notify(Utils::Format("%08X", (buffer + 0x1DB44)));
			//OSD::Notify(Utils::Format("UnknownPattern %08X", (u32)std::addressof(Town::GetSaveData()->UnknownPattern) - 0x31F26F80));
		}

		ACNL_Player *player = Player::GetSaveData();

		if(!player) 
			return;

		static u32 index = 0; //170

		if(Controller::IsKeysPressed(Key::L + Key::DPadRight)) {
			Keyboard KB("");
			KB.Open(index, index);
		}
		if(Controller::IsKeysPressed(Key::L + Key::DPadUp)) {
			SetPlayerFlag(&player->PlayerFlags, index, true);
		}
		if(Controller::IsKeysPressed(Key::L + Key::DPadDown)) {
			SetPlayerFlag(&player->PlayerFlags, index, false);
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

	//FUN_00584d88(int param_1,undefined4 param_2) Sets shampoodle sound?
	
	/*void SetMusic(u32 *data0x98B018, u32 soundID0x10000B2) {
		FUNCT(0x2FD0BC).Call<void>(0x2C, data[1], 4);

		u32 **var = FUNCT(0x586744).Call<u32 **>(data);
		
		var[8] = (u32 *)soundID;
		*(u8 *)(var + 9) = 1;
		*(u8 *)(var + 0x25) = 1;
		*(u8 *)(var + 0x26) = 3;
		var[0] = (u32 *)0x907BBC;
	}

	FUN_00584D88(0x98B018, uVar3, 0, 0x94F494);

	struct OnlineGameDat {
		u32 *unknown0;
		u32 unknown1;
		u32 unknown2;

		//0x1329D, Game Type

		//0x132B8, END
	};

	3238AE8 = nothing

	32238C68 = ModuleEventNpc.cro
	32238D94 = ModuleTour.cro

	32239378 = ModuleKotobuki.cro
	*/
	
//Item Island Code
	void islanditems(MenuEntry *entry) {
		if (Controller::IsKeysPressed(Key::ZL + Key::DPadRight)) {
			std::vector<ItemNamePack> results;

			std::string keyword = "cap";
			Item::searchAllByKeyword(keyword, results);
			
			OSD::Notify(Utils::Format("Found %d items with keyword: %s (1st Item: 0x%04X)", results.size(), keyword.c_str(), results.at(0)), Color::Cyan);
		}

		/*

		*/
		/*else if(Controller::IsKeysPressed(Key::R + Key::DPadRight)) {
			int res = FUNCTION(0x30F5FC).Call<int>(0xAF88F8, "Test %d", 25);
			char* buff = (char *)(0xAF88F8 + 0xC);
			OSD::Notify(std::string(buff));
		}

		if(Controller::IsKeysPressed(Key::R + Key::DPadUp)) {
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

			static FUNCT func(Address(0x64DB90););
			func.Call<bool>(player, 0xB9, globalData, 0);

			delete[] globalData;
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
		u32 address(0x64FC1C, 0, 0, 0, 0, 0, 0, 0);
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
		
		u32 LButton(0x5B4180, 0, 0, 0, 0, 0, 0, 0);
		u32 RButton(0x5B4194, 0, 0, 0, 0, 0, 0, 0);
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

			OSD::Notify(Utils::Format("Player %02X selected!", playerID));
		}

		if(Controller::IsKeysPressed(Key::L + Key::DPadLeft)) {
			random = !random;
			OSD::Notify("Random Mode: " << (random ? (Color::Green << "ON") : (Color::Red << "OFF")));
		}

		if(Controller::IsKeysDown(Key::R)) {
			if(random) {
				FishID.ID = Utils::Random(0x22E1, 0x234A);
				if(Game::GetItemCategorie(FishID) != Item_Categories::Fish) 
					return;
			}

			if(PlayerClass::GetInstance(playerID)->IsLoaded())
				throwfish.Call<void>(&FishID, PlayerClass::GetInstance(playerID)->GetCoordinates(), 1);
		}

		if(Controller::IsKeysDown(Key::R + Key::DPadUp)) 
			Wrap::KB<u16>("Set Fish ID:", true, 4, FishID.ID, FishID.ID);
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
}
