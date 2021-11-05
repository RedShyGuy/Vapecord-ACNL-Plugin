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
									"Translators: みるえもん(Japanese), im a book(spanish), Fedecrash02(italian), Youssef & Arisa(french), bkfirmen(german) \n\n"
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
	static const std::string GameVersionUSAWA = "1.1"; //seems to be an exception, I am guessing the Devs of ACNL messed something up there

	bool CheckGameVersion(void) {
		static const Address isUSAWA(0, 1, 0, 0, 0, 0, 0, 0);
		std::string realGameVersion = (bool)isUSAWA.addr ? GameVersionUSAWA : GameVersion;
		
		u8 u_byte = f_GameVer::NoneVer;
		ReadConfig(CONFIG::GameVer, u_byte);
		if(u_byte == f_GameVer::Accepted)
			return true;

		std::string currentVersion = "";
		u8 res = IsNewestVersion(currentVersion, realGameVersion);

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
	
	static bool WasSuspended = false;
	void SuspendCallBack(u32 param) {
	/*If Game suspenses*/
		if(!WasSuspended) {
			
			delete ItemList; //delete ItemList to clear a lot of memory to prevent memory issues if game is suspended
			ItemList = nullptr;

			WasSuspended = true;
			goto reset;
		}

		OSD::Notify("Initializing Memory", Color::Purple);
	/*If Game unsuspenses*/
		ItemList = new Item();
		ReserveItemData(ItemList); //redo ItemList when game is unsuspended
		
		WasSuspended = false;

	reset:
		const HookContext &curr = HookContext::GetCurrent();
		static Address func(decodeARMBranch(curr.targetAddress, curr.overwrittenInstr));
		func.Call<void>(param);
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
		CheckGameVersion();

		SleepTime();

	//Load Addresses
		Code::Load();
	//RCO only if game is supported
		RCO();
	//Load MenuFolders and Entrys (located in MenuCreate.cpp)
		InitMenu(menu);

		ReserveItemData(ItemList);

		static const Address suspendAddress(0x124F60, 0x1249D0, 0x124F84, 0x124F84, 0x124F4C, 0x124F4C, 0x124F4C, 0x124F4C);
		static Hook suspendHook;
		suspendHook.Initialize(suspendAddress.addr, (u32)SuspendCallBack);
		suspendHook.SetFlags(USE_LR_TO_RETURN);
		suspendHook.Enable();

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
