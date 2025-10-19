#include "Helpers/Game.hpp"
#include "Config.hpp"
#include "Files.h"
#include "RegionCodes.hpp"
#include "Helpers/Inventory.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "NonHacker.hpp"

namespace CTRPluginFramework {
	static const std::string Note = "Creator: Lukas \n\n"
									"Code Credits: Nico, Jay, Levi, Slattz, Kominost, Elominator and more \n\n"
									"Translators: みるえもん & みなと(Japanese), im a book(spanish), Fedecrash02(italian), Youssef, Arisa, & Lenoch(french), bkfirmen & Toby(german), Soopoolleaf(korean) \n\n"
									"" << Utils::Format("Discord: %s", DISCORDINV);

	extern int UI_Pos;
	bool OSD_SplashScreen(const Screen &Splash);
	void IndoorsSeedItemCheck(void);
	void OnNewFrameCallback(Time ttime);
	void InitMenu(PluginMenu *menu);
	void GetPlayerInfoData(void);
	void RCO(void);
	void GetMessage_p1(void);
	void GetMessage_p2(void);
	void GetMessage_p3(void);
	void GetMessage_p4(void);

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

	bool CheckGameTitleID(void) {
		if(!GameHelper::IsACNL()) {
			Sleep(Seconds(5));
			MessageBox("Error 999", "Game Not Supported!\nCheats will not load\nPlugin Main Features are still usable!").SetClear(ClearScreen::Top)();
			return false;
		}
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

		CheckForLanguageFile();

	//If title isn't ACNL
		if(!CheckGameTitleID()) {
			menu->Run();
			return 0;
		}

		SleepTime();

	//Load Addresses
		Code::Load();
	//RCO only if game is supported
		RCO();
	//keeps internet connection when menu is opened
		InitKeepConnection();

		SetupLanguage(false);
	//Load MenuFolders and Entrys (located in MenuCreate.cpp)
		InitMenu(menu);

		ReserveItemData(ItemList);

	//Load Callbacks
		menu->Callback(IndoorsSeedItemCheck);

		menu->Callback(NonHacker_Player00);
		menu->Callback(NonHacker_Player01);
		menu->Callback(NonHacker_Player02);
		menu->Callback(NonHacker_Player03);

		menu->OnNewFrame = OnNewFrameCallback;

		Process::exceptionCallback = CustomExceptionHandler;

	//Run Menu Loop
		menu->Run();
		return 0;
	}
}
