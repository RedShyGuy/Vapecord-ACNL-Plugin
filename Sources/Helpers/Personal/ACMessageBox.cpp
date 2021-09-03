#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
    Time ACMSG::LifeSpan = Seconds(2);
	static Clock clock;

    void SetMSGData(u32 mData, const std::string& str) {
		static const u32 point = Region::AutoRegion(0x90AACC, 0x9099A0, 0x90994C, 0x90994C, 0x903DBC, 0x902DBC, 0x902D28, 0x902D28);
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
        static const u32 point = Region::AutoRegion(0x951702, 0x9506F2, 0x9506FE, 0x9506FE, 0x94A6FE, 0x9496FE, 0x9496FE, 0x9496FE);
        Process::Write8(point, 1);
    }

    bool ACMSG::IsRunning(void) {
        static const u32 point = Region::AutoRegion(0x951702, 0x9506F2, 0x9506FE, 0x9506FE, 0x94A6FE, 0x9496FE, 0x9496FE, 0x9496FE);
        return *(u8 *)point == 0;
    }

//calls ingame message box
	void ACMSG::Notify(const std::string& str) {
        Stop();

		static const u32 func1 = Region::AutoRegion(0x5E3768, 0x5E2C98, 0x5E27A0, 0x5E27A0, 0x5E2020, 0x5E2020, 0x5E1CA8, 0x5E1CA8);
		static const u32 func2 = Region::AutoRegion(0x5E3920, 0x5E2E50, 0x5E2958, 0x5E2958, 0x5E21D8, 0x5E21D8, 0x5E1E60, 0x5E1E60);
		static const u32 func3 = Region::AutoRegion(0x5E3A64, 0x5E2F94, 0x5E2A9C, 0x5E2A9C, 0x5E231C, 0x5E231C, 0x5E1FA4, 0x5E1FA4);	
		static const u32 func4 = Region::AutoRegion(0x75BDAC, 0x75AD90, 0x75ADB4, 0x75AD8C, 0x75A54C, 0x75A524, 0x75A0F4, 0x75A0CC);	
		static const u32 func5 = Region::AutoRegion(0x81C104, 0x81B004, 0x81B10C, 0x81B0E4, 0x81A8AC, 0x81A884, 0x81A44C, 0x81A424);
		static const u32 point = Region::AutoRegion(0x8578A0, 0x856898, 0x85672C, 0x85672C, 0x853938, 0x852938, 0x852904, 0x852904);

		u32 mData = PlayerClass::GetInstance()->Offset(0x5750);

		FUNCTION(func1).Call<void>(mData + 0x458, *(u32 *)(mData + 0x44C), 0);

		SetMSGData(mData + 0x51C, str);
		
		u32 uVar2 = mData + 0x458;
		FUNCTION(func2).Call<void>(uVar2, mData + 0x51C);
		FUNCTION(func3).Call<void>(uVar2);

		float fVar8 = FUNCTION(func4).Call<float>(uVar2);

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

		FUNCTION(func5).Call<void>(mData + 0x14, *(u32 *)point, *(u32 *)(point + 4));

		static PluginMenu *menu = PluginMenu::GetRunningInstance();
		*menu += StopMSGCallBack;
	}
}