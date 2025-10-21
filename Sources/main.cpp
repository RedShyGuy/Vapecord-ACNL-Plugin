#include "Helpers/Game.hpp"
#include "Config.hpp"
#include "Files.h"
#include "Helpers/Inventory.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "NonHacker.hpp"
#include "Address/Address.hpp"
#include "Address/AddressReader.hpp"

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
		std::string region = Address::LoadRegion();

		PluginMenu *menu = new PluginMenu(Color::White << "ACNL Vapecord Plugin " << region, majorV, minorV, revisV, Note);
		menu->SynchronizeWithFrame(true);

		CheckForLanguageFile();

	//If title isn't ACNL
		if(region.empty()) {
			menu->Run();
			return 0;
		}

		SleepTime();

	//Load Addresses
		if (!AddressReader::getInstance()->loadFromBinary(PATH_ADDRESSES_BIN, region)) {
			MessageBox(Utils::Format("Error 600\nThe addresses.bin is missing the selected region (%s)\nGet more info and help on the Discord Server: %s", region.c_str(), DISCORDINV)).SetClear(ClearScreen::Top)();
			Process::ReturnToHomeMenu();
			return 0;
		}

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
