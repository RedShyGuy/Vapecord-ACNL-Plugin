#include "Config.hpp"
#include "Files.h"
#include "Helpers/Wrapper.hpp"
#include "Address/Address.hpp"
#include "Helpers/ItemReader.hpp"
#include "Helpers/ItemSequence.hpp"
#include "Helpers/Game.hpp"
#include "cheats.hpp"

namespace CTRPluginFramework {
	static const std::string NOTE =
        std::string(
			R"(Creator: Kwadukathole (Lukas)

			Code Credits: Nico, Jay, Levi, Slattz, Kominost, Elominator and more

			Translators: みるえもん & みなと(Japanese), im a book(spanish), Fedecrash02(italian), Youssef, Arisa, & Lenoch(french), bkfirmen & Toby(german), Soopoolleaf(korean)
		)") + Utils::Format("Discord: %s", DISCORDINV);

	extern int UI_Pos;
	bool OSD_SplashScreen(const Screen &Splash);
	void IndoorsSeedItemCheck(void);
	void InitMenu(PluginMenu *menu);

	void SleepTime(void) {
		OSD::Run(OSD_SplashScreen);

		constexpr int tick_ms  = 150;
		constexpr int ui_cycle = 8;

		while(!Game::IsGameInRoom(0x63)) {
			UI_Pos = (UI_Pos + 1) % ui_cycle;
			Sleep(Milliseconds(tick_ms));
		}

		OSD::Stop(OSD_SplashScreen);
	}

	void InitKeepConnection(void);

	int	main(void) {
		std::string region = Address::LoadRegion();

		PluginMenu *menu = new PluginMenu(Color::White << "ACNL Vapecord Plugin " << region, majorV, minorV, revisV, NOTE);
		menu->SynchronizeWithFrame(true);

		CheckForLanguageFile();

	//If title isn't ACNL
		if(region.empty()) {
			menu->Run();
			return 0;
		}

		SleepTime();

		ItemSequence::Init();
	//keeps internet connection when menu is opened
		InitKeepConnection();

		SetupLanguage(false);
	//Load MenuFolders and Entrys (located in MenuCreate.cpp)
		InitMenu(menu);

		if (!ItemReader::getInstance()->loadFromBinary(PATH_ITEM_BIN)) {
			MessageBox(Utils::Format("Error 660\nThe item.bin is missing\nGet more info and help on the Discord Server: %s", DISCORDINV)).SetClear(ClearScreen::Top)();
			Process::ReturnToHomeMenu();
			return 0;
		}

	//Load Callbacks
		menu->Callback(IndoorsSeedItemCheck);
		menu->OnNewFrame = SendPlayerData;
		Process::exceptionCallback = CustomExceptionHandler;

	//Run Menu Loop
		menu->Run();
		return 0;
	}
}
