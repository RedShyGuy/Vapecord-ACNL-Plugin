#ifndef CTRPLUGINFRAMEWORK_SYSTEM_THREAD_HPP
#define CTRPLUGINFRAMEWORK_SYSTEM_THREAD_HPP

#include "types.h"
#include "3ds.h"

namespace CTRPluginFramework
{
    class ThreadEx
    {
    public:
        enum
        {
            IDLE,
            RUNNING,
            FINISHED
        };

        ThreadEx(ThreadFunc entrypoint,u32 stackSize, u32 priority, int affinity);
        ~ThreadEx(void);

        Result  Start(void *arg);
        Result  Join(bool releaseResources);
        Handle  GetHandle(void);
        u32     GetStatus(void);

        u8          priority{0x3F};
        s8          affinity{-1};

        static void     Yield(void);

    private:
        u8          _state{IDLE};
        Thread_tag *_thread{nullptr};
    };
}

#endif
