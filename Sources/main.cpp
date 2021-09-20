#include "Helpers/Game.hpp"
#include "Config.hpp"
#include "Files.h"
#include "RegionCodes.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Wrapper.hpp"
#include "NonHacker.hpp"

namespace CTRPluginFramework {
	static const std::string Note = "Creator: Lukas#4444 (RedShyGuy) \n\n"
									"Code Credits: Nico, Jay, Levi, Slattz, Kominost, Elominator and more \n\n"
									"Translators: みるえもん(Japanese), im a book(spanish), Fedecrash02(italian), Youssef & Arisa(french) \n\n"
									"" << Utils::Format("Discord: %s", DISCORDINV);

	extern bool logoExists;
	extern int UI_Pos;
	extern c_RGBA* logoArray;
	bool OSD_SplashScreen(const Screen &Splash);
	void IndoorsSeedItemCheck(void);
	void devcallback(void);
	void OnNewFrameCallback(Time ttime);
	void InitMenu(PluginMenu *menu);
	void GetPlayerInfoData(void);
	void RCO(void);

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
		*menu -= UpdateInstance;
	}

/*
Gets region name to append to plugin title | also sets current region
*/
	std::string GetRegionName(void) {
		switch(Process::GetTitleID()) {
			case TID_USA: 
				c_Region = CurrRegion::USA; 
			return "USA";
			case TID_USAWA: 
				c_Region = CurrRegion::USAWA;
			return "USAWA";
			case TID_EUR: 
				c_Region = CurrRegion::EUR;
			return "EUR";
			case TID_EURWA: 
				c_Region = CurrRegion::EURWA;
			return "EURWA";
			case TID_JPN: 
				c_Region = CurrRegion::JPN;
			return "JPN";
			case TID_JPNWA: 
				c_Region = CurrRegion::JPNWA;
			return "JPNWA";
			case TID_KOR: 
				c_Region = CurrRegion::KOR;
			return "KOR";
			case TID_KORWA: 
				c_Region = CurrRegion::KORWA;
			return "KORWA";
			case TID_EURWL: 
				c_Region = CurrRegion::EURWL;
			return "EURWL";
			default: 
				c_Region = CurrRegion::INVALID;
			return "";
		}
	}

/*
Checks game version
-1 = string not found
-2 = wrong version
0 = correct version
*/
	u8 IsNewestVersion(std::string& versionSTR, const std::string& gameVersion) {
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

	bool CheckGameVersion(void) {
		u8 u_byte = f_GameVer::NoneVer;
		ReadConfig(CONFIG::GameVer, u_byte);
		if(u_byte == f_GameVer::Accepted)
			return true;

		std::string currentVersion = "";
		u8 res = IsNewestVersion(currentVersion, GameVersion);

		if(res == -2) {
			if(u_byte == f_GameVer::Declined)
				return false;

			Sleep(Seconds(5));
			static const std::string str = Utils::Format("Your game has the version %s\nThis plugin only supports the game version %s. Make sure you have the correct game version before you use this plugin!\nIgnore this warning?", currentVersion.c_str(), GameVersion.c_str());
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
		File file(PATH_LOGO, File::READ);
		if(!file.IsOpen()) {
			OSD::Notify("logo.bin missing", Color::Red);
			logoExists = false;
		}
		else {
			logoArray = new c_RGBA[file.GetSize() / sizeof(c_RGBA)];
			file.Read(logoArray, file.GetSize());
			file.Close();
		}

		OSD::Run(OSD_SplashScreen);

		while(GameHelper::LoadRoomBool()) {
			if(UI_Pos >= 7) UI_Pos = 0;
			else UI_Pos++;

			Sleep(Milliseconds(150));
		}

		OSD::Stop(OSD_SplashScreen);
		delete[] logoArray;
	}

	int	main(void) {
		PluginMenu *menu = new PluginMenu("ACNL Vapecord Plugin " << GetRegionName(), majorV, minorV, revisV, Note);
		menu->SynchronizeWithFrame(true);

		CheckForCONFIG();

	//If title isn't ACNL
		if(!CheckGameTitleID()) {
			menu->Run();
			return 0;
		}

	//Check if the game has the correct version
		if(!CheckGameVersion()) {
			menu->Run();
			return 0;
		}

		SleepTime();

	//Load Addresses
		Code::Load();
	//RCO only if game is supported
		RCO();
	//Load MenuFolders and Entrys (located in MenuCreate.cpp)
		InitMenu(menu);

		ReserveItemData(ItemList);

		menu->OnFirstOpening = StartingMsg;

	//Load Callbacks
		menu->Callback(devcallback);
		menu->Callback(IndoorsSeedItemCheck);
		menu->Callback(NonHackerCallBack);
		menu->Callback(UpdateInstance);

		menu->OnNewFrame = OnNewFrameCallback;

		Process::exceptionCallback = CustomExceptionHandler;

	//Run Menu Loop
		menu->Run();
		return 0;
	}
}
