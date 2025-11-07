#include "Helpers/Game.hpp"
#include "Config.hpp"
#include "Files.h"
#include "RegionCodes.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/QuickMenu.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "NonHacker.hpp"
#include "Helpers/Pretendo.hpp"
#include "PatternManager.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace CTRPluginFramework {
	static const std::string Note = "Creator: Lukas#4444 (RedShyGuy) \n\n"
									"Code Credits: Nico, Jay, Levi, Slattz, Kominost, Elominator, Jon & DaniElectra (Pretendo), and more\n\n"
									"Translators: みるえもん & みなと(Japanese), im a book(spanish), Fedecrash02(italian), Youssef, Arisa, & Lenoch(french), bkfirmen & Toby(german), Soopoolleaf(korean) \n\n"
									"" << Utils::Format("Discord: %s", DISCORDINV);

	extern int UI_Pos;
	bool OSD_SplashScreen(const Screen &Splash);
	void IndoorsSeedItemCheck(void);
	void devcallback(void);
	void OnNewFrameCallback(Time ttime);
	void InitMenu(PluginMenu *menu);
	void GetPlayerInfoData(void);
	void RCO(void);
	void GetMessage_p1(void);
	void GetMessage_p2(void);
	void GetMessage_p3(void);
	void GetMessage_p4(void);

/*
Will be called at the start of the plugin to load the language, colors and the dev check
*/
	void UpdateInstance(void) {
		PluginMenu *menu = PluginMenu::GetRunningInstance();
		if(menu == nullptr) //if menu didn't load yet
			return;

		CustomFWK(false);
		SetupLanguage(false);
		SetupColors(false);
		IsDevModeUsable();
		QuickMenu::Init();
		PluginMenuData::Init();

		PluginMenu::V_SetColor(SetupColors);
		PluginMenu::V_SetLanguage(SetupLanguage);
		PluginMenu::V_SetFwk(CustomFWK);
		PluginMenu::V_SetVisibility(cheatsVisibility);
		PluginMenu::V_SetReset(resetSettings);
		*menu -= UpdateInstance;
	}

/*
Gets region name to append to plugin title | also sets current region
*/
	std::string GetRegionName(void) {
		switch(Process::GetTitleID()) {
			case TID_USA: 
				c_Region = CurrRegion::USA; 
				regionName = "USA";
			break;
			case TID_USAWA: 
				c_Region = CurrRegion::USAWA;
				regionName = "USAWA";
			break;
			case TID_EUR: 
				c_Region = CurrRegion::EUR;
				regionName = "EUR";
			break;
			case TID_EURWA: 
				c_Region = CurrRegion::EURWA;
				regionName = "EURWA";
			break;
			case TID_JPN: 
				c_Region = CurrRegion::JPN;
				regionName = "JPN";
			break;
			case TID_JPNWA: 
				c_Region = CurrRegion::JPNWA;
				regionName = "JPNWA";
			break;
			case TID_KOR: 
				c_Region = CurrRegion::KOR;
				regionName = "KOR";
			break;
			case TID_KORWA: 
				c_Region = CurrRegion::KORWA;
				regionName = "KORWA";
			break;
			case TID_EURWL: 
				c_Region = CurrRegion::EURWL;
				regionName = "EURWL";
			break;
			default: 
				c_Region = CurrRegion::INVALID;
				regionName = "";
			break;
		}
		return regionName;
	}

/*
Checks game version
-1 = string not found
-2 = wrong version
0 = correct version
*/
	s8 IsNewestVersion(std::string& versionSTR, const std::string& gameVersion) {
		versionSTR.clear();

		static const std::vector<u16> Pattern = { 0x0056, 0x0065, 0x0072, 0x002E, 0x0020 };
		u16* found = (u16 *)Utils::Search<u16>(0x00800000, 0x00200000, Pattern);
		if(found == nullptr)
			return -1;

		versionSTR = Utils::Format("%c.%c", (char)found[5], (char)found[7]);
		if(versionSTR != gameVersion)
			return -2;

		return 0;
	}

	static const std::string GameVersion = "1.5";
	static const std::string GameVersionUSAWA = "1.1"; //seems to be an exception, I am guessing the Devs of ACNL messed something up there

	bool CheckGameVersion(void) {
		static const Address isUSAWA(0, 1, 0, 0, 0, 0, 0, 0);
		std::string realGameVersion = (bool)isUSAWA.addr ? GameVersionUSAWA : GameVersion;
		
		u8 u_byte = f_GameVer::NoneVer;
		ReadConfig(CONFIG::GameVer, u_byte);
		if(u_byte == f_GameVer::Accepted)
			return true;

		std::string currentVersion = "";
		s8 res = IsNewestVersion(currentVersion, realGameVersion);

		if(res == -2) {
			Sleep(Seconds(5));
			static const std::string str = Utils::Format("Your game has the version %s\nThis plugin only supports the game version %s. Make sure you have the correct game version before you use this plugin!\nIgnore this warning?", currentVersion.c_str(), realGameVersion.c_str());
            if(!(MessageBox(Color(0xDC143CFF) << "Warning, wrong game version!", str, DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
				WriteConfig(CONFIG::GameVer, f_GameVer::Declined);
				return false;
			}
			else {
				WriteConfig(CONFIG::GameVer, f_GameVer::Accepted);
				return true;
			}
		}

		else if(res == -1) {
			OSD::Notify("Game Version Not Found!");
			WriteConfig(CONFIG::GameVer, f_GameVer::Declined);
			return false;
		}

		WriteConfig(CONFIG::GameVer, f_GameVer::Accepted);
		return true;
	}

	bool CheckGameTitleID(void) {
		u8 u_byte = f_GameID::NoneID;
		ReadConfig(CONFIG::GameID, u_byte);
		if(u_byte == f_GameID::CorrectID)
			return true;
		else if(u_byte == f_GameID::WrongID)
			return false;

		if(!GameHelper::IsACNL()) {
			Sleep(Seconds(5));
			MessageBox("Error 999", "Game Not Supported!\nCheats will not load\nPlugin Main Features are still usable!").SetClear(ClearScreen::Top)();
			WriteConfig(CONFIG::GameID, f_GameID::WrongID);
			return false;
		}

		WriteConfig(CONFIG::GameID, f_GameID::CorrectID);
		return true;
	}

/*
Will set a counter at the start of the plugin as long as the title screen didn't load to
prevent any issues with freezing of the plugin
*/
	void SleepTime(void) {
		OSD::Run(OSD_SplashScreen);

		while(GameHelper::LoadRoomBool()) {
			if(UI_Pos >= 7) UI_Pos = 0;
			else UI_Pos++;

			Sleep(Milliseconds(150));
		}

		OSD::Stop(OSD_SplashScreen);
	}

	void InitKeepConnection(void);

	int	main(void) {
		PluginMenu *menu = new PluginMenu(Color::White << "ACNL Vapecord Plugin " << GetRegionName(), majorV, minorV, revisV, Note);
		menu->SynchronizeWithFrame(true);

		CheckForCONFIG();

	//If title isn't ACNL
		if(!CheckGameTitleID()) {
			menu->Run();
			return 0;
		}

	//Check if the game has the correct version
		CheckGameVersion();

		SleepTime();

	//Load Addresses
		Code::Load();
	//RCO only if game is supported
		RCO();
	//keeps internet connection when menu is opened
		InitKeepConnection();
	//Load MenuFolders and Entrys (located in MenuCreate.cpp)
		InitMenu(menu);

		ReserveItemData(ItemList);

		menu->OnFirstOpening = StartingMsg;

	//Load Callbacks
		menu->Callback(devcallback);
		menu->Callback(IndoorsSeedItemCheck);

		menu->Callback(NonHacker_Player00);
		menu->Callback(NonHacker_Player01);
		menu->Callback(NonHacker_Player02);
		menu->Callback(NonHacker_Player03);

		menu->Callback(UpdateInstance);

		menu->OnNewFrame = OnNewFrameCallback;

		Process::exceptionCallback = CustomExceptionHandler;

    // Patch Pretendo + RCE fix
        PatternManager pm;
        initPretendoPatches(pm);

        pm.Perform();

        enablePretendoPatches();

	//Run Menu Loop
		menu->Run();
		return 0;
	}
}
