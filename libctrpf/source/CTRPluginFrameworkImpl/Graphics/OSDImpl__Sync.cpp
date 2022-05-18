#include "CTRPluginFrameworkImpl/Graphics/OSDImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"
#include "CTRPluginFramework/System/Mutex.hpp"
#include "CTRPluginFramework/System/Lock.hpp"
#include "CTRPluginFramework/System/Thread.hpp"
#include "CTRPluginFramework/System/Sleep.hpp"

#include "csvc.h"
#include <3ds.h>
#include <cstring>

#define THREADVARS_MAGIC  0x21545624 // !TV$

namespace CTRPluginFramework
{
    using CTRPFLock = CTRPluginFramework::Lock;
    using namespace CTRPluginFrameworkImpl::Services;

    namespace
    {
        Mutex   FrameLockingMutex;
        bool    IsForced = false;
        bool    HasGameResumed = false;
        Task    CheckFrameResumedTask([](void *arg) -> s32
        {
            while (!HasGameResumed)
            {
                Sleep(Milliseconds(32)); ///< Wait for 2 frames

                if (!HasGameResumed)
                {
                    GSP::TriggerPSCEvents();
                }
            }

            return 0;
        });
    }

    void    OSDImpl::Lock(void)
    {
        RecursiveLock_Lock(&RecLock);
    }

    bool OSDImpl::TryLock(void)
    {
        return (RecursiveLock_TryLock(&RecLock));
    }

    void    OSDImpl::Unlock(void)
    {
        RecursiveLock_Unlock(&RecLock);
    }

    enum
    {
        CLEARED_STICKY = -2,
        CLEARED_ONESHOT = -1,
        SIGNALED_ONESHOT = 0,
        SIGNALED_STICKY = 1
    };

    static inline int LightEvent_TryReset(LightEvent& event)
    {
        __dmb();
        do
        {
            if (__ldrex(&event.state))
            {
                __clrex();
                return 0;
            }
        } while (__strex(&event.state, CLEARED_ONESHOT));
        __dmb();
        return 1;
    }

    // Return false when event was signaled, true on timeout
    // TODO: remove when my pr to ctrulib is accepted
    static bool    LightEvent__WaitTimeOut(LightEvent& event, const Time timeout)
    {
        s64     timeOutNs = timeout.AsMicroseconds() * 1000;
        Result  toRes = 0x09401BFE;
        Result  res = 0;

        for (; res != toRes; )
        {
            if (event.state == CLEARED_STICKY)
            {
                res = syncArbitrateAddressWithTimeout(&event.state, ARBITRATION_WAIT_IF_LESS_THAN_TIMEOUT, 0, timeOutNs);
                return res == toRes;
            }

            if (event.state != CLEARED_ONESHOT)
            {
                if (event.state == SIGNALED_STICKY)
                    return false;

                if (event.state == SIGNALED_ONESHOT && LightEvent_TryReset(event))
                    return false;
            }

            res = syncArbitrateAddressWithTimeout(&event.state, ARBITRATION_WAIT_IF_LESS_THAN_TIMEOUT, 0, timeOutNs);
        }

        return res == toRes;
    }

    int    OSDImpl::PauseFrame(void)
    {
        HasGameResumed = true;
        // Check if we need to pause frame
        if (WaitingForScreenshot || FramesToPlay || !ProcessImpl::IsPaused || !NeedToPauseFrame)
            return 0;

        // Check frame buffers validity before pausing
        if (ScreenImpl::CheckGspFrameBuffersInfo())
            return 0;

        {
            CTRPFLock   lock(FrameLockingMutex);

            if (IsForced || !NeedToPauseFrame)
                return 0;

            // Lock game threads except this one
            ProcessImpl::LockGameThreads();

            IsFramePaused = true;
        }

        // Wake up threads waiting for frame to be paused
        LightEvent_Signal(&OnFramePaused);

        // Wait until the frame is ready to continue
        LightEvent_Wait(&OnFrameResume);
        LightEvent_Clear(&OnFrameResume);

        IsFramePaused = false;
        return 1;
    }

    void    OSDImpl::WaitFramePaused(void)
    {
        if (IsFramePaused)
            return;

        bool    isAsync = ProcessImpl::Status & NoImage;

        if (!isAsync)
        {
            IsForced = false;
            NeedToPauseFrame = true;

            while ((IsForced = LightEvent__WaitTimeOut(OnFramePaused, Milliseconds(50)))
                   && ScreenImpl::CheckGspFrameBuffersInfo());
            LightEvent_Clear(&OnFramePaused);

            CTRPFLock   lock(FrameLockingMutex);

            // Lock game threads
            if (!IsFramePaused)
            {
                ProcessImpl::LockGameThreads();
                IsForced = true;
            }
        }

        IsFramePaused = true;

        // Wake up gsp event thread
        GSP::ResumeInterruptReceiver();
    }

    void    OSDImpl::ResumeFrame(const u32 nbFrames)
    {
        if (!IsFramePaused)
            return;

        HasGameResumed = false;

        bool    isAsync = ProcessImpl::Status & NoImage;

        GSP::PauseInterruptReceiver();

        if (isAsync)
            return;

        FramesToPlay = nbFrames;

        // Unlock game threads
        ProcessImpl::UnlockGameThreads();

        // Async job to ensure frame is resumed
        CheckFrameResumedTask.Start();

        // Wake up game's thread
        LightEvent_Signal(&OnFrameResume);

        if (nbFrames)
        {
            // Wait until all our frames are rendered and the process is paused again
            LightEvent_Wait(&OnFramePaused);
            LightEvent_Clear(&OnFramePaused);
        }
        else
        {
            IsForced = NeedToPauseFrame = false;
        }
    }
}
