#include "types.h"
#include "3ds.h"
#include "CTRPluginFramework/System/Time.hpp"

namespace CTRPluginFramework
{
    void    Sleep(Time sleepTime)
    {
        if (sleepTime > Time::Zero)
            svcSleepThread(sleepTime.AsMicroseconds() * 1000);
    }
}