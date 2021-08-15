#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
    Time ACMSG::LifeSpan = Seconds(2);
	static Clock clock;

    void SetMSGData(u32 mData, const std::string& str) {
	//TODO: port addresses
		static const u32 point = Region::AutoRegion(0x90AACC, 0, 0, 0, 0, 0, 0, 0);
		Process::Write32(mData, point);
		Process::Write32(mData + 4, mData + 0x18);
		Process::Write32(mData + 8, 0x78);
		Process::Write32(mData + 0xC, 4);
		Process::Write32(mData + 0x10, 0xFFFFFFFF);
		Process::Write32(mData + 0x14, 0xFFFFFFFF);
		Process::WriteString(mData + 0x18, str, StringFormat::Utf16);
	}

	void StopMSGCallBack(void) {
		static bool first = true;
		if(first) {
			clock.Restart();
			first = false;
		}

		if(clock.HasTimePassed(ACMSG::LifeSpan) || !ACMSG::IsRunning()) { 
			ACMSG::Stop();

			first = true;

			static PluginMenu *menu = PluginMenu::GetRunningInstance();
			*menu -= StopMSGCallBack;
  		}
	}

    void ACMSG::Stop(void) {
		clock.Restart();
	//TODO: port addresses
        static const u32 point = Region::AutoRegion(0x951702, 0, 0, 0, 0, 0, 0, 0);
        Process::Write8(point, 1);
    }

    bool ACMSG::IsRunning(void) {
	//TODO: port addresses
        static const u32 point = Region::AutoRegion(0x951702, 0, 0, 0, 0, 0, 0, 0);
        return *(u8 *)point == 0;
    }

//calls ingame message box
	void ACMSG::Notify(const std::string& str) {
        Stop();
	//TODO: port addresses
		static const u32 func1 = Region::AutoRegion(0x5E3768, 0, 0, 0, 0, 0, 0, 0);
		static const u32 func2 = Region::AutoRegion(0x5E3920, 0, 0, 0, 0, 0, 0, 0);
		static const u32 func3 = Region::AutoRegion(0x5E3A64, 0, 0, 0, 0, 0, 0, 0);	
		static const u32 func4 = Region::AutoRegion(0x75BDAC, 0, 0, 0, 0, 0, 0, 0);	
		static const u32 func5 = Region::AutoRegion(0x81C104, 0, 0, 0, 0, 0, 0, 0);
		static const u32 point = Region::AutoRegion(0x8578A0, 0, 0, 0, 0, 0, 0, 0);

		u32 mData = PlayerClass::GetInstance()->Offset(0x5750);

		FUNCT(func1).Call<void>(mData + 0x458, *(u32 *)(mData + 0x44C), 0);

		SetMSGData(mData + 0x51C, str);
		
		u32 uVar2 = mData + 0x458;
		FUNCT(func2).Call<void>(uVar2, mData + 0x51C);
		FUNCT(func3).Call<void>(uVar2);

		float fVar8 = FUNCT(func4).Call<float>(uVar2);

		uVar2 = *(u32 *)(mData + 0x44C);
		fVar8 = fVar8 + 0.5;
		u32 uVar5 = *(u32 *)(*(u32 *)(mData + 0x44C) + 0x4C);
		*(float *)(uVar2 + 0x48) = fVar8;
		*(u32 *)(uVar2 + 0x4C) = uVar5;

		if(*(u32 *)(mData + 0x450) != 0) {
			uVar2 = *(u32 *)(mData + 0x450);
			uVar5 = *(u32 *)(*(u32 *)(mData + 0x450) + 0x4C);
			*(float *)(uVar2 + 0x48) = fVar8;
			*(u32 *)(uVar2 + 0x4C) = uVar5;
		}

		fVar8 = fVar8 * 0.5 + 200.0;
		*(float *)(mData + 0x628) = fVar8;
		uVar2 = *(u32 *)(mData + 0x448);
		uVar5 = *(u32 *)(*(u32 *)(mData + 0x448) + 0x2C);
		u32 uVar7 = *(u32 *)(*(u32 *)(mData + 0x448) + 0x30);
		*(float *)(uVar2 + 0x28) = fVar8;
		*(u32 *)(uVar2 + 0x2C) = uVar5;
		*(u32 *)(uVar2 + 0x30) = uVar7;
		*(u8 *)(uVar2 + 0xB7) = *(u8 *)(uVar2 + 0xB7) & 0xCF;

		FUNCT(func5).Call<void>(mData + 0x14, *(u32 *)point, *(u32 *)(point + 4));

		static PluginMenu *menu = PluginMenu::GetRunningInstance();
		*menu += StopMSGCallBack;
	}
}