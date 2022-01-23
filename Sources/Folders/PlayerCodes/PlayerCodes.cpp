#include "cheats.hpp"
#include "Helpers/Game.hpp"
#include "Helpers/PlayerClass.hpp"
#include "Helpers/Inventory.hpp"
#include "TextFileParser.hpp"
#include "Helpers/Player.hpp"
#include "Helpers/Address.hpp"
#include "Helpers/Animation.hpp"
#include "Helpers/AnimData.hpp"
#include "Helpers/Dropper.hpp"
#include "Color.h"
#include "Files.h"

u32 BGR_HairVal = 0;
u32 BGR_EyeVal = 0;
extern "C" void BGRHook(void);
extern "C" void SetPlayerIconCoordinates(void);

namespace CTRPluginFramework {
	static std::vector<std::string> strings1 = { "", "", "", "", "", "" };
		
	static std::vector<std::string> strings2 = { "", "", "", "" };
	
	void GetPlayerInfoData(void) {	
		u8 pIndex = GameHelper::GetOnlinePlayerIndex();
		if(!PlayerClass::GetInstance(pIndex)->IsLoaded())
			return;
	
	//gets coordinates
		float *pCoords = PlayerClass::GetInstance(pIndex)->GetCoordinates();
		
	//Gets world coords	
		if(!MapEditorActive)
			PlayerClass::GetInstance(pIndex)->GetWorldCoords(&selectedX, &selectedY);
		
	//gets item standing on
		u32 pItem = (u32)GameHelper::GetItemAtWorldCoords(selectedX, selectedY);	
		
		strings1[0] = ("Coordinates: " << std::to_string(pCoords[0]).erase(4) << "|" << std::to_string(pCoords[2]).erase(4));
		strings1[1] = (Utils::Format("World Coordinates: %02X|%02X", (u8)(selectedX & 0xFF), (u8)(selectedY & 0xFF)));
		strings1[2] = ("Velocity: " << std::to_string(*PlayerClass::GetInstance(pIndex)->GetVelocity()).erase(4));
		strings1[3] = (Utils::Format("Animation: %02X / %03X", *PlayerClass::GetInstance(pIndex)->GetAnimation(), *PlayerClass::GetInstance(pIndex)->GetSnake()));
		strings1[4] = ("Standing on: " << (pItem != 0 ? Utils::Format("%08X", *(u32 *)pItem) : "N/A") << (GameHelper::GetLockedSpotIndex(selectedX, selectedY) != 0xFFFFFFFF ? "(Locked)" : ""));
		strings1[5] = (Utils::Format("Room: %02X", GameHelper::RoomCheck()));

	//gets inv item
		u8 slot = 0;
		if(Inventory::GetSelectedSlot(slot)) 
			Inventory::ReadSlot(slot, itemslotid);
		else 
			itemslotid = 0xFFFFFFFF;
		
		strings2[0] = (Utils::Format("Pickup: %08X", PickupSeederItemID));
		strings2[1] = (Utils::Format("Drop: %08X", dropitem));
		strings2[2] = ("Replace: " << (ItemIDToReplace == 0xFFFFFFFF ? "everything" : Utils::Format("%08X", ItemIDToReplace)));
		strings2[3] = (itemslotid != 0xFFFFFFFF) ? Utils::Format("Item ID: %04X", itemslotid) : "No Slot Selected";
	}
//debug OSD
	bool debugOSD(const Screen &screen) {
		u8 pIndex = GameHelper::GetOnlinePlayerIndex();
		if(!PlayerClass::GetInstance(pIndex)->IsLoaded()) 
			return 0;
			
		if(!screen.IsTop)
			return 0;
		
		static constexpr u8 YPositions1[7] = { 10, 20, 30, 40, 50, 60 };
		static constexpr u8 YPositions2[4] = { 0, 10, 20, 30 };
		
		int i = 0, j = 0;
		
	//gets player
		screen.Draw(Utils::Format("Player %02X", pIndex), 0, 0, pColor[pIndex]);
		
		while(i < 6) {
			for(auto GetString = strings1.begin(); GetString != strings1.end(); ++GetString) {
				screen.Draw(*GetString, 0, YPositions1[i]);
				i++;
			}	
		}

	//If own player is loaded
		if(PlayerClass::GetInstance()->IsLoaded()) {	
			while(j < 4) {
				for(auto GetString = strings2.begin(); GetString != strings2.end(); ++GetString) {
					screen.Draw(*GetString, 285, YPositions2[j]);
					j++;
				}	
			}
		}
		return 1;
	}

	static bool PlayerInfoON = false;
//enable debug OSD
	void debug(MenuEntry *entry) {
		std::vector<std::string> cmnOpt =  { "" };

		cmnOpt[0] = PlayerInfoON ? (Color(pGreen) << Language->Get("VECTOR_ENABLED")) : (Color(pRed) << Language->Get("VECTOR_DISABLED"));
		
		Keyboard optKb(Language->Get("KEY_CHOOSE_OPTION"), cmnOpt);
		
		Sleep(Milliseconds(100));
		s8 op = optKb.Open();	
		if(op < 0)
			return;

		PluginMenu *menu = PluginMenu::GetRunningInstance();
		
		if(!PlayerInfoON) {
			*menu += GetPlayerInfoData;
			OSD::Run(debugOSD); 
			PlayerInfoON = true;
		}
		else {
			*menu -= GetPlayerInfoData;
			OSD::Stop(debugOSD); 
			PlayerInfoON = false;
		}
		debug(entry);
	}

//player loader | for now disabled
/*
	void pLoaderEntry(MenuEntry *entry) {
		if(!PlayerClass::GetInstance()->IsLoaded()) {
			MessageBox(Language->Get("SAVE_PLAYER_NO")).SetClear(ClearScreen::Both)();
			return;
		}

		if(GameHelper::GetOnlinePlayerCount() != 0) {
			MessageBox("Only works offline!").SetClear(ClearScreen::Both)();
			return;
		}
		
		std::vector<std::string> pV = {
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
			Color::Silver << "-Empty-",
		};
		
		for(int i = 0; i <= 3; ++i) {
			u32 pO = Player::GetSpecificSave(i);
			if(*(u16 *)(pO + 0x55A6) != 0) {
				std::string pS = "";
				Process::ReadString((pO + 0x55A8), pS, 0x10, StringFormat::Utf16);
				pV[i] = pColor[i] << pS;
			}
		}
		
		Keyboard pKB(Language->Get("KEY_SELECT_PLAYER"), pV);
		
		Sleep(Milliseconds(100));
		s8 pChoice = pKB.Open();
		if((pChoice >= 0) && (pV[pChoice] != Color::Silver << "-Empty-"))
			Player::Load(pChoice);
	}
*/

//Neck Position
	void neckentry(MenuEntry *entry) {
		float speed = 5.0;
		float neckpos = 0;
		if(entry->Hotkeys[0].IsDown()) {
			neckpos = Process::ReadFloat(PlayerClass::GetInstance()->Offset(0x210), neckpos);
			Process::WriteFloat(PlayerClass::GetInstance()->Offset(0x210), (neckpos + -2.8633e+08 + speed));
		}
	}

	void RGB_To_BGR(u32 &x) {
		u8 byteshift[3] = { 0 };

	//shifts into bytes
		byteshift[0] = ((x & 0x00FF0000) >> 16);
		byteshift[1] = ((x & 0x0000FF00) >> 8);
		byteshift[2] = ((x & 0x000000FF));

	//reshifts back into integer
		x = (byteshift[2] << 16) + (byteshift[1] << 8) + byteshift[0];
	}

	u32 RainbowBGR() { 
		u8 ShiftB = Utils::Random(0, 255);
		u8 ShiftG = Utils::Random(0, 255);
		u8 ShiftR = Utils::Random(0, 255); 	
			
		return Set::ToRGB(ShiftB, ShiftG, ShiftR);
    }

	static u32 rval1 = 0, rval2 = 0;

	struct App_Colors {
		u32 Hair_BGR;
		u32 Eye_BGR;
	};

	void SaveColor(MenuEntry *entry) {
		std::vector<std::string> opt = {
			Language->Get("CUSTOM_SET_HAIR"),
			Language->Get("CUSTOM_SET_EYE"),
			Language->Get("CUSTOM_SAVE"),
		};

		Keyboard KB("", opt);

		switch(KB.Open()) {
			default: break;
			case 0: {
				Keyboard kb(Language->Get("CUSTOM_ENTER_HAIR"));
				kb.IsHexadecimal(true);

				Sleep(Milliseconds(100));
				s8 res = kb.Open(rval1);
				if(res < 0)
					return;

				RGB_To_BGR(rval1);
			} break;

			case 1: {
				Keyboard kb(Language->Get("CUSTOM_ENTER_EYE"));
				kb.IsHexadecimal(true);

				Sleep(Milliseconds(100));
				s8 res = kb.Open(rval2);
				if(res < 0)
					return;

				RGB_To_BGR(rval2);
			} break;

			case 2: {
				if(!File::Exists(Utils::Format(PATH_COLOR, regionName.c_str()))) 
					File::Create(Utils::Format(PATH_COLOR, regionName.c_str()));

				App_Colors NewColor = App_Colors{ rval1, rval2 };

				File f_color(Utils::Format(PATH_COLOR, regionName.c_str()), File::WRITE);
				f_color.Write(&NewColor, 8);	
				f_color.Flush();
                f_color.Close();

				Sleep(Milliseconds(100));
				MessageBox(Language->Get("CUSTOM_FILE_SAVED")).SetClear(ClearScreen::Top)();
			} break;
		}
	}

	void App_ColorMod(MenuEntry *entry) {
		static Hook hook;
		if(entry->WasJustActivated()) {
			if(File::Exists(Utils::Format(PATH_COLOR, regionName.c_str()))) {
				App_Colors OldColor;

				File f_color(Utils::Format(PATH_COLOR, regionName.c_str()), File::READ);
				f_color.Read(&OldColor, 8);
				f_color.Flush();
                f_color.Close();

				rval1 = OldColor.Hair_BGR;
				rval2 = OldColor.Eye_BGR;
				OSD::Notify("Loaded colors from file!", Color::Orange);
			}

			static const Address address(0x4A33C8, 0x4A2D40, 0x4A2410, 0x4A2410, 0x4A20A8, 0x4A20A8, 0x4A1F68, 0x4A1F68);
			hook.Initialize(address.addr, (u32)BGRHook);
		  	hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();
		}

		BGR_HairVal = rval1;
		BGR_EyeVal = rval2;

		if(entry->Hotkeys[0].IsDown()) {
			Sleep(Milliseconds(100));
			rval1 = RainbowBGR();
			rval2 = RainbowBGR();
		}

		if(!entry->IsActivated()) 
            hook.Disable();
	}
	
//Wear Helmet And Accessory /*Credits to Levi*/
	void hatz(MenuEntry *entry) { 
		static const Address hatwear(0x68C630, 0x68BB58, 0x68B668, 0x68B668, 0x68B128, 0x68B128, 0x68ACD0, 0x68ACD0);
		if(entry->WasJustActivated()) 
			Process::Patch(hatwear.addr, 0xE1A00000); 
		else if(!entry->IsActivated()) 
			Process::Patch(hatwear.addr, 0x3AFFFFD6); 
	}

//Faint	
	void Faint(MenuEntry *entry) { 
        if(Controller::IsKeysPressed(entry->Hotkeys[0].GetKeys())) {
			u32 x, y;
			if(PlayerClass::GetInstance(GameHelper::GetOnlinePlayerIndex())->GetWorldCoords(&x, &y))
				Animation::ExecuteAnimationWrapper(GameHelper::GetOnlinePlayerIndex(), 0x9D, 0, 0, 0, 0, 0, x, y, 0, 0);
		}
    }

	c_RGBA* playerIcon[4] = { nullptr, nullptr, nullptr, nullptr };

	void LoadPlayerIcons(void) {
		for(int i = 0; i < 4; ++i) {
			if(PlayerClass::GetInstance(i)->IsLoaded() && playerIcon[i] == nullptr) {
				std::string filestr = Utils::Format(PATH_PICON, i);
				File file(filestr, File::READ);
				if(!file.IsOpen())
					return;

				playerIcon[i] = new c_RGBA[file.GetSize() / sizeof(c_RGBA)];
				file.Read(playerIcon[i], file.GetSize());
				file.Close();
			}
			else if(!PlayerClass::GetInstance(i)->IsLoaded() && playerIcon[i] != nullptr) {
				delete[] playerIcon[i];
				playerIcon[i] = nullptr;
			}
		}
	}

//OSD For Show Players On The Map	
	bool players(const Screen &screen) { 
		if(screen.IsTop || !GameHelper::MapBoolCheck())
			return 0;

		u32 XPos, YPos;

		for(int i = 0; i < 4; ++i) {
			if(PlayerClass::GetInstance(i)->IsLoaded()) {
				PlayerClass::GetInstance(i)->PlayerClass::CalculateMapCoordinates(XPos, YPos);

				if(playerIcon[i] != nullptr) {	
					int Pixels = 0, XResult = 9 + XPos, YResult = 15 + YPos;

					for(int X = XPos; X < XResult; ++X) {
						for(int Y = YPos; Y < YResult; ++Y) {
							Color cPix = Color(playerIcon[i][Pixels].R, playerIcon[i][Pixels].G, playerIcon[i][Pixels].B, playerIcon[i][Pixels].A);
							if(cPix != Color(0, 0, 0, 0) && playerIcon[i][Pixels].A == 0xFF)
								screen.DrawPixel(X, Y, cPix);

							Pixels++;
						}
					}
				}
				else 
					screen.DrawRect(XPos - 1, YPos + 1, 6, 6, pColor[i]);
			}
		}

		return 1;
	}
	
//Show Players On The Map
	void map(MenuEntry *entry) {
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		static const Address writePatch(0x2215B0, 0x220FF4, 0x2215D0, 0x2215D0, 0x2214F0, 0x2214F0, 0x2214BC, 0x2214BC);
		static Hook hook;

		if(entry->WasJustActivated()) {
			hook.Initialize(writePatch.addr, (u32)SetPlayerIconCoordinates);
			hook.SetFlags(USE_LR_TO_RETURN);
			hook.Enable();

			u32 pInstance = PlayerClass::GetInstance()->Offset();
			if(pInstance != 0 && GameHelper::MapBoolCheck()) {
				u32 aInstance = Animation::GetAnimationInstance(pInstance, 0, 0, 0);

				AnimData data;
				data.Init(aInstance, pInstance, 4);
				data.Congrats_2A();
				data.ExecuteAnimation(0x2A);
			}

			*menu += LoadPlayerIcons;
			OSD::Run(players); 
		}

		else if(!entry->IsActivated()) {
			hook.Disable();

			*menu -= LoadPlayerIcons;
			for(int i = 0; i < 4; ++i) {
				delete[] playerIcon[i];
				playerIcon[i] = nullptr;
			}

			OSD::Stop(players); 
		}
	}

	void NeverBedHead(MenuEntry *entry) {
		static const Address antiBedHead(0x20C798, 0x20C1DC, 0x20C7B8, 0x20C7B8, 0x20C6D8, 0x20C6D8, 0x20C6A4, 0x20C6A4);
		if(entry->WasJustActivated()) 
			Process::Patch(antiBedHead.addr, 0xE1A00000);
		else if(!entry->IsActivated()) 
			Process::Patch(antiBedHead.addr, 0xE5C41004);    
	}
}