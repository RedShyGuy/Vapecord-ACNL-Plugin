#include "types.h"
#include <3ds.h>
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Scheduler.hpp"
#include "CTRPluginFrameworkImpl/Graphics/OSDImpl.hpp"

extern "C" {
    bool _g_isCitra = false;
}

namespace CTRPluginFramework
{
    bool    SystemImpl::IsNew3DS = false;
    bool    SystemImpl::IsCitra = false;
    u32     SystemImpl::CFWVersion = 0;
    u32     SystemImpl::RosalinaHotkey = 0;
    u32     SystemImpl::AptStatus = 0;
    u8      SystemImpl::Language = CFG_LANGUAGE_EN;
    //vapecord infobar
    u64     SystemImpl::FriendCode = 0;

   static  LightEvent      g_sleepEvent;

    enum
    {
	    FLAG_ACTIVE       = BIT(0),
	    FLAG_ALLOWSLEEP   = BIT(1),
	    FLAG_ORDERTOCLOSE = BIT(2),
	    FLAG_SHUTDOWN     = BIT(3),
	    FLAG_POWERBUTTON  = BIT(4),
	    FLAG_WKUPBYCANCEL = BIT(5),
	    FLAG_WANTSTOSLEEP = BIT(6),
	    FLAG_SLEEPING     = BIT(7),
	    FLAG_EXITED       = BIT(31),
    };

	//vapecord infobar
    void GetLocalFriendCode(u64 *friendcode)
    {
        frdInit(false);

        FriendKey key;
		FRD_GetMyFriendKey(&key);

		FRD_PrincipalIdToFriendCode(key.principalId, friendcode);

        frdExit();
    }

    void    SystemImpl::Initialize(void)
    {
        s64 out;

        LightEvent_Init(&g_sleepEvent, RESET_STICKY);

        IsNew3DS = R_SUCCEEDED(svcGetSystemInfo(&out, 0x10001, 1));

        if (R_SUCCEEDED(svcGetSystemInfo(&out, 0x10000, 0)))
        {
            CFWVersion = static_cast<u32>(out);
            if (R_SUCCEEDED(svcGetSystemInfo(&out, 0x10000, 0x101)))
                RosalinaHotkey = static_cast<u32>(out);
        }
        else
            CFWVersion = 0; ///< Unknown

        // Get System's language
        CFGU_GetSystemLanguage(&Language);
        
        //vapecord infobar
        GetLocalFriendCode(&FriendCode);
    }

    void     SystemImpl::CheckCitra(void)
    {
        s64 output = 0;
        svcGetSystemInfo(&output, 0x20000, 0);
        IsCitra = output;
        _g_isCitra = output;
    }

    bool    SystemImpl::WantsToSleep(void)
    {
        return AptStatus & FLAG_WANTSTOSLEEP;
    }

    bool     SystemImpl::IsSleeping(void)
    {
        return AptStatus & (FLAG_SLEEPING | FLAG_WANTSTOSLEEP);
    }

    void    SystemImpl::ReadyToSleep(void)
    {
        if (AptStatus & FLAG_WANTSTOSLEEP)
        {
            AptStatus &= ~FLAG_WANTSTOSLEEP;
            LightEvent_Clear(&g_sleepEvent);
            AptStatus |= FLAG_SLEEPING;
        }
    }

    void    SystemImpl::WakeUpFromSleep(void)
    {
        AptStatus &= ~FLAG_WANTSTOSLEEP;
        AptStatus &= ~FLAG_SLEEPING;
        LightEvent_Signal(&g_sleepEvent);
    }

    bool    SystemImpl::Status(void)
    {
        if (AptStatus & FLAG_SHUTDOWN)
        {
            return true;
        }

        if (AptStatus & FLAG_SLEEPING)
            LightEvent_Wait(&g_sleepEvent);

        return false;
    }
}
