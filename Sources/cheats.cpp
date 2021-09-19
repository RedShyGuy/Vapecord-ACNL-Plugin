#include "cheats.hpp"

namespace CTRPluginFramework {
	const u32 majorV = 1;
	const u32 minorV = 9;
	const u32 revisV = 5;
	const std::string fullV = Utils::Format("%d.%d.%d", majorV, minorV, revisV);

	bool DropPatternON = false;
	bool turbo = false;
	bool MapEditorActive = false;
	bool bypassing = false;
	bool animExecuting = false;
	bool fovbool = false;
	bool save = false;
	bool IfForceAllowed = true;
	
//Animation Related
	u32 emotion = 1;
	u32 soundID = 0x1000660;
	u32 itemslotid = 0;
	u16 snake = 1;
	u8 appearance[3] = { 0, 0, 0 };
	u8 anim1 = 6;
	u8 waitAnim = 0x56;
	u8 animToForce = 6;
	u32 animItem = 0x2001;
	
//Dropper Related
	u8 DropType = 0xA;
	u8 DropRoom = 0xA5;
	u32 ItemIDToReplace = 0x7FFE;
	u32 dropitem = 0x7FFE;
	u32 selectedX = 0; 
	u32 selectedY = 0; 
	u32 selectedItem = 0x10; 

	const std::string g_Empty = "-Empty-";

	int ItemFileLenght = 0;
	bool ItemFileExists = true;

	const Color pBlue(0, 196, 255);
	const Color pRed(255, 28, 28);
	const Color pGreen(44, 255, 28);
	const Color pYellow(255, 255, 14);
	const Color PressedButton(0x505050A0);
	const Color SelectedText(0xEF00FFFF);
	const Color White(255, 255, 255);
	const Color Crimson(220, 20, 60);
	const Color PaleGreen(152, 251, 152);
	const Color Coral(255, 127, 80);
	const Color BlueViolet(138, 43, 226);
	const Color MidnightBlue(25, 25, 112);
	const Color GoldenRod(218, 165, 32);
	const Color LightSeaGreen(32, 178, 170);
	const Color Salmon(250, 128, 114);
	const Color HotPink(255, 105, 180);
	const Color Chocolate(210, 105, 30);
	const Color DodgerBlue(30, 144, 255);
	const Color LimeGreen(50, 205, 50);
	const Color SpringGreen(0 ,250 ,154);
	const Color SpecialRed(225 ,0 ,52);

	const Color pColor[4] = {
		Color(pBlue), Color(pRed),		
		Color(pGreen), Color(pYellow),	
	};

	constexpr u32 ReValues[77] = { 
		0x1FF0000, 0x1FF01FF, 0xFFFF0000, 0x10000FF, 0x101, 
		0, 8, 6, 7, 4, 5, 2, 3,  
		1, 0, 7, 8, 5, 6, 3, 4, 
		1, 2, 0, 5, 7, 3, 8, 1, 
		6, 2, 4, 0, 3, 5, 1, 7, 
		2, 8, 4, 6, 0, 1, 3, 2, 
		5, 4, 7, 6, 8, 0, 2, 1, 
		4, 3, 6, 5, 8, 7, 0, 4, 
		2, 6, 1, 8, 3, 7, 5, 0, 
		6, 8, 4, 7, 2, 5, 1, 3,
	};
/*
Restores Drop Pattern if drop radius changer has been used to prevent any crashes
*/
	void RestoreDropPattern(void) {
		for(int i = 0; i < 77; ++i)
			Process::Write32(Code::DropPattern.addr + (i * 4), ReValues[i]);
	}

/*
Returns a randomized RGBA color
*/
	u32 Rainbow() {
		u8 ShiftR = Utils::Random(0, 255);
        u8 ShiftG = Utils::Random(0, 255);
        u8 ShiftB = Utils::Random(0, 255);	
		u8 ShiftA = Utils::Random(0, 255);
			
		return Set::ToRGBA_U32(ShiftR, ShiftG, ShiftB, ShiftA);
    }

	static bool RainBowON = false;

/*
Randomizes colors of Menu Folders
*/
	void RainbowEntrys(Time time) {
		static Clock timer(time);
		if(timer.HasTimePassed(Milliseconds(500)) && RainBowON) {   
			SaveCodesUpdate(Rainbow());
			MovementCodesUpdate(Rainbow());
			InventoryCodesUpdate(Rainbow());
			PlayerCodesUpdate(Rainbow());
			AnimationCodesUpdate(Rainbow());
			SeedingCodesUpdate(Rainbow());
			MoneyCodesUpdate(Rainbow());
			IslandCodesUpdate(Rainbow());
			NPCCodesUpdate(Rainbow());
			FunCodesUpdate(Rainbow());
			ExtraCodesUpdate(Rainbow());
			MiscCodesUpdate(Rainbow());
			timer.Restart();
		}
	}

	/*static const char* WeekDay[7] = { "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su" };

	struct TimeData {
		u32 Year;
		u8 Month; 
		u8 Day;
		u8 WeekDay;
		u8 Hour;
		u8 Minute;
		u8 Second;
	};

	struct TimeChar {
		std::string Date;
		std::string Time;
		std::string WeekDay;
	};

//AM = 0 | PM = 1
	s8 ConvertToAM_PM(u8& hour) {
		if(hour == 0 && hour != 12) {
			hour = 12; 
			return 0;
		}
		else if(hour == 12 && hour != 0) {
			hour = 12; 
			return 1;
		}
		else if(hour < 12 && hour != 0) {
			return 0;
		}
		else if(hour > 12 && hour != 0) {
			hour -= 12; 
			return 1;
		}

		return -1;
	}

	TimeChar GetGameTime(void) {
		static FUNCT func(0x2FB394);
		TimeData *data = func.Call<TimeData *>();

		std::string date = Utils::Format("%02d|%02d|%04d", data->Day, data->Month, data->Year);

		s8 res = ConvertToAM_PM(data->Hour);

		std::string time = Utils::Format("%02d:%02d:%02d %s", data->Hour, data->Minute, data->Second, res ? "PM" : "AM");

		std::string weekday = WeekDay[data->WeekDay-1];

		return TimeChar{ date, time, weekday };
	}*/

	static bool FCLoaded = false;

	void StoreFC(std::string &FC) {
		frdInit();

		FriendKey key;
		FRD_GetMyFriendKey(&key);

		u64 localFriendCode = 0;
		FRD_PrincipalIdToFriendCode(key.principalId, &localFriendCode);

		char buffer[13];
		sprintf(buffer, "%012lld", localFriendCode);
		std::string str = (std::string(buffer));
		FC = Utils::Format("FC: %s - %s - %s", str.substr(0, 4).c_str(), str.substr(4, 4).c_str(), str.substr(8, 4).c_str());

		frdExit();
	}

	void StoreBatteryPercentage(float &percentage) {
		u8 data[4];
		mcuHwcInit();
		MCUHWC_ReadRegister(0xA, data, 4);

		percentage = data[1] + data[2] / 256.0f;
        percentage = (u32)((percentage + 0.05f) * 10.0f) / 10.0f;

		mcuHwcExit();
	}

	void OnNewFrameCallback(Time ttime) {
		RainbowEntrys(ttime);

		static std::string FC = "";

		if(!FCLoaded) {
			StoreFC(FC);
			FCLoaded = true;
		}

		const FwkSettings &settings = FwkSettings::Get();
		const Screen& TopScreen = OSD::GetTopScreen();
		const Screen& BottomScreen = OSD::GetBottomScreen();
		time_t rawtime;
		struct tm * timeinfo;
		char timestamp[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(timestamp, 80, "%r | %F", timeinfo);

		float percentage = 0;
		StoreBatteryPercentage(percentage);

		int coordx = 30, coordy1 = 0, coordy2 = 218;

 		TopScreen.DrawRect(coordx, coordy1, 340, 20, settings.BackgroundMainColor, true);
		TopScreen.DrawRect(coordx + 2, coordy1 + 2, 340 - 4, 20 - 2, settings.BackgroundBorderColor, false);

		TopScreen.DrawRect(coordx, coordy2, 340, 22, settings.BackgroundMainColor, true);
		TopScreen.DrawRect(coordx + 2, coordy2 + 2, 340 - 4, 20 - 2, settings.BackgroundBorderColor, false);

		TopScreen.DrawSysfont(timestamp, coordx + 5, coordy1 + 3);
		TopScreen.DrawSysfont(Utils::Format("%d%%", (u32)percentage), coordx + 280, coordy1 + 3);

		TopScreen.DrawSysfont(FC, coordx + 5, coordy2 + 3);
		TopScreen.DrawSysfont("BETA 2", coordx + 280, coordy2 + 3);

		coordx = 20, coordy1 = 0, coordy2 = 218;

		BottomScreen.DrawRect(coordx, coordy1, 280, 20, settings.BackgroundMainColor, true);
		BottomScreen.DrawRect(coordx + 2, coordy1 + 2, 280 - 4, 20 - 2, settings.BackgroundBorderColor, false);
	
		BottomScreen.DrawRect(coordx, coordy2, 280, 22, settings.BackgroundMainColor, true);
		BottomScreen.DrawRect(coordx + 2, coordy2 + 2, 280 - 4, 20 - 2, settings.BackgroundBorderColor, false);
		
		BottomScreen.DrawSysfont("Discord: " + std::string(DISCORDINV), coordx + 5, coordy1 + 3);

		BottomScreen.DrawSysfont("Plugin made by Lukas#4444", coordx + 5, coordy2 + 3);
	}

/*
Toggles the random color entry
*/
	void rainbow(MenuEntry *entry) {
		if(entry->WasJustActivated())
			RainBowON = true;
		else if(!entry->IsActivated())
			RainBowON = false;
	}
}