#include <CTRPluginFramework.hpp>
#include <vector>
#include "cheats.hpp"
#include "RegionCodes.hpp"

namespace CTRPluginFramework {
	static const std::string Note = "Creator: Lukas#4444 (RedShyGuy) \n\n"
									"Code Credits: Nico, Jay, Levi, Slattz, Kominost, Elominator and more \n\n"
									"Translators: みるえもん(Japanese), im a book(spanish), Fedecrash02(italian), Youssef & Arisa(french) \n\n"
									"" << Utils::Format("Discord: %s", DISCORDINV);


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

	extern bool logoExists;
	extern int UI_Pos;
	extern c_RGBA* logoArray;
	bool OSD_SplashScreen(const Screen &Splash);

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

	  //If title isn't ACNL
		if(!GameHelper::IsACNL()) {
			Sleep(Seconds(5));
			InitMenu(menu);
			MessageBox("Error 999", "Game Not Supported!\nCheats will not load\nPlugin Main Features are still usable!").SetClear(ClearScreen::Both)();
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
	
		CheckForCONFIG();

		ReserveItemData(ItemList);

		static const std::string MadeBy = FormatColor("Plugin made by %cLukas%c#%c4%c4%c4%c4\n%cJoin my Discord for updates and help on the plugin: ", Rainbow(), Rainbow(), Rainbow(), Rainbow(), Rainbow(), Rainbow(), Color::White) << Color::Red << DISCORDINV;								

		MessageBox("BETA VERSION", MadeBy).SetClear(ClearScreen::Top)();

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
