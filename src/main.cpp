#include "core/Config.hpp"
#include "Files.h"
#include "core/infrastructure/PluginUtils.hpp"
#include "core/infrastructure/Address.hpp"
#include "core/ItemSequence.hpp"
#include "core/game_api/Game.hpp"
#include "features/cheats.hpp"
#include "core/Pretendo/Pretendo.hpp"
#include "core/Pretendo/PatternManager.hpp"
#include "core/Pretendo/PIALogger.hpp"

namespace CTRPluginFramework {
	static const std::string NOTE =
        std::string(
R"(Creator: Kwadukathole (Lukas)

Cheat Credits: Nico, Jay, Levi, Slattz, Gokiro, Kominost, Elominator and more

Code Credits: Foofoo_the_guy, PabloMK7, Jon & DaniElectra (Pretendo) and more

Translators: NeitherHateNorLike(Chinese Simplified & Traditional), みるえもん & みなと(Japanese), im a book(spanish), Fedecrash02(italian), Youssef, Arisa, & Lenoch(french), bkfirmen & Toby(german), Soopoolleaf(korean)

)") + Utils::Format("Discord: %s", DISCORDINV);

	static const std::string GameVersion = "1.5";
	static const std::string GameVersionUSAWA = "1.0"; //seems to be an exception

	extern int UI_Pos;
	bool OSD_SplashScreen(const Screen &Splash);
	void IndoorsSeedItemCheck(void);
	void InitMenu(PluginMenu *menu);

	enum GameVersionResult {
		CORRECT_VERSION,
		STRING_NOT_FOUND,
		WRONG_VERSION
	};

	GameVersionResult IsNewestVersion(std::string& versionSTR, const std::string& gameVersion) {
		versionSTR.clear();

		static const std::vector<u16> Pattern = { 0x0056, 0x0065, 0x0072, 0x002E, 0x0020 };
		u16* found = (u16 *)Utils::Search<u16>(0x00800000, 0x00200000, Pattern);
		if(found == nullptr) {
			return STRING_NOT_FOUND;
		}

		versionSTR = Utils::Format("%c.%c", (char)found[5], (char)found[7]);
		if(versionSTR != gameVersion) {
			return WRONG_VERSION;
		}

		return CORRECT_VERSION;
	}

	bool CheckGameVersion(void) {
		bool isUSAWA = Address::IsRegion(Address::Region::USAWA);
		std::string realGameVersion = isUSAWA ? GameVersionUSAWA : GameVersion;

		std::string currentVersion = "";
		GameVersionResult res = IsNewestVersion(currentVersion, realGameVersion);

		if(res == WRONG_VERSION) {
			Sleep(Seconds(5));
			static const std::string str = Utils::Format("Your game has the version %s\nThis plugin only supports the game version %s.\nDo you still want to try it out?", currentVersion.c_str(), realGameVersion.c_str());
            if((MessageBox(Color(0xDC143CFF) << "Warning, wrong game version!", str, DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
				return true;
			}

			return false;
		}

		else if(res == STRING_NOT_FOUND) {
			OSD::NotifySysFont("Game Version Not Found!");
			return false;
		}

		return true;
	}


	void SleepTime(void) {
		OSD::Run(OSD_SplashScreen);

		constexpr int tick_ms  = 150;
		constexpr int ui_cycle = 8;

		while(Game::IsRoomLoading()) {
			UI_Pos = (UI_Pos + 1) % ui_cycle;
			Sleep(Milliseconds(tick_ms));
		}

		OSD::Stop(OSD_SplashScreen);
	}

	void LaunchGameKeyboardAsCustomQwerty(Keyboard &keyboard);
	void PerformAutoSaveBackup(void);
	void ShowAntiScamScreen(void);

	int	main(void) {
		std::string region = Address::LoadRegion();

		PluginMenu *menu = new PluginMenu(Color::White << "ACNL Vapecord Plugin " << region, majorV, minorV, revisV, NOTE);
		menu->SynchronizeWithFrame(true);
		menu->ShowWelcomeMessage(false);

	//If title isn't ACNL
		if(region.empty()) {
			OSD::NotifySysFont("Plugin ready!");
			menu->Run();
			return 0;
		}

		if (!CheckGameVersion()) {
			OSD::NotifySysFont("Plugin ready!");
			menu->Run();
			return 0;
		}

	//This gets set here and not in the PatchProcess because it should only be set if the game is new leaf
		FwkSettings &settings = FwkSettings::Get();
		settings.SaveBackupCallback = PerformAutoSaveBackup;
		settings.SaveBackupMaxSlots = 3; //3 is currently the default

		EnableAllChecks();
		EnableAllPatches();

		if (!Config::EnsureConfigFile()) {
			OSD::NotifySysFont("Failed to create config file!", Color::Red);
			menu->Run();
			return 0;
		}

		Config::HandleConfigMigration();

		SleepTime();

		Config::SetupLanguage(false);

	//Show anti-scam warning on first launch
		ShowAntiScamScreen();

		ItemSequence::Init();

		menu->OnNewFrame = SendPlayerData;
		InitSaveReminder();

	//Patch Pretendo + RCE fix + PIA Logger
        PatternManager pm;
		initPiaLogger(pm);
        initPretendoPatches(pm);

		pm.Perform();

		enablePretendoPatches();

	//Load MenuFolders and Entrys (located in MenuCreate.cpp)
		InitMenu(menu);

	//Load Callbacks
		menu->OnOpening = SetSeederInfos;
		menu->Callback(IndoorsSeedItemCheck);
		menu->Callback(SaveReminderCallback);
		Process::exceptionCallback = CustomExceptionHandler;

	//Set custom keyboard
		Keyboard::SetCustomQwertyCallback(LaunchGameKeyboardAsCustomQwerty);

		OSD::NotifySysFont("ACNL Vapecord Plugin ready!");
	//Run Menu Loop
		menu->Run();
		return 0;
	}
}
