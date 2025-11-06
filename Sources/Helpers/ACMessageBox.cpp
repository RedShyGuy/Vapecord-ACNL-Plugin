#include "Helpers/ACMessageBox.hpp"
#include "Address/Address.hpp"
#include "Helpers/PlayerClass.hpp"

namespace CTRPluginFramework {
    Time ACMSG::LifeSpan = Seconds(2);
	static Clock clock;

    void SetMSGData(u32 mData, const std::string& str) {
		static const Address point(0x90AACC);
		Process::Write32(mData, point.addr);
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
        static const Address point(0x951702);
        Process::Write8(point.addr, 1);
    }

    bool ACMSG::IsRunning(void) {
        static const Address point(0x951702);
        return *(u8 *)point.addr == 0;
    }

//calls ingame message box
	void ACMSG::Notify(const std::string& str) {
        Stop();

		static Address func1(0x5E3768);
		static Address func2(0x5E3920);
		static Address func3(0x5E3A64);	
		static Address func4(0x75BDAC);	
		static Address func5(0x81C104);
		static const Address point(0x8578A0);

		u32 mData = PlayerClass::GetInstance()->Offset(0x5750);

		func1.Call<void>(mData + 0x458, *(u32 *)(mData + 0x44C), 0);

		SetMSGData(mData + 0x51C, str);
		
		u32 uVar2 = mData + 0x458;
		func2.Call<void>(uVar2, mData + 0x51C);
		func3.Call<void>(uVar2);

		float fVar8 = func4.Call<float>(uVar2);

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

		func5.Call<void>(mData + 0x14, *(u32 *)point.addr, *(u32 *)(point.addr + 4));

		static PluginMenu *menu = PluginMenu::GetRunningInstance();
		*menu += StopMSGCallBack;
	}
}