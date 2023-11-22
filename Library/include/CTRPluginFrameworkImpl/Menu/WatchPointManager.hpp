#pragma once

#include "types.h"
#include <3ds.h>
#include "CTRPluginFramework/System/Mutex.hpp"
#include <vector>

namespace CTRPluginFramework
{
    struct WatchPointHit
    {
        u32     _wfar;
        u32     _count;
        u32     _flags;

        enum
        {
            Skip = 1
        };

        WatchPointHit(u32 address)
        {
            _wfar = address;
            _count = _flags = 0;
        }
    };

    struct WatchPoint
    {
        enum
        {
            Disabled = 0,
            Read,
            Write,
            ReadWrite
        };

        u32     _address{0};
        u32     _size{4};
        u32     _kind{Disabled};
        u32     _totalHit{0};

        std::vector<WatchPointHit>  _hits{};

        WatchPointHit&  GetHitData(u32 wfar);

        void    RenderTop(void);
        void    RenderBottom(void);
    };

    class   WatchPointManager
    {
    public:
        void            Initialize(void);

        static void     SetWatchPoint(u32 address, u32 size, u32 kind);
        static void     DisableWatchPoint(u32 address);

        static void     Open(void);


        static void     HandleException(ERRF_ExceptionInfo* excep, CpuRegisters* regs);
    private:
        WatchPointManager(void) = default;

        Mutex           _mutex{};
        bool            _isInitialized{false};
        u32             _contextId{0};
        u32             _total{0};
        WatchPoint      _watchPoints[2]{};

        static WatchPointManager    _singleton;
    };
}
