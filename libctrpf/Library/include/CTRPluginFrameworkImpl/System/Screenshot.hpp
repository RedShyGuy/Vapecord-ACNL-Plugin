#ifndef CTRPLUGINFRAMEWORKIMPL_SYSTEM_SCREENSHOT_HPP
#define CTRPLUGINFRAMEWORKIMPL_SYSTEM_SCREENSHOT_HPP

#include "types.h"
#include "CTRPluginFramework/System/Clock.hpp"
#include "CTRPluginFramework/System/Task.hpp"
#include <3ds.h>

#include <string>


namespace CTRPluginFramework
{
    class Screenshot
    {
        using OnScreenshotCallback = bool (*)(void);
    public:

        struct ImageBuffer
        {
            u8  top[400 * 240 * 4];
            u32 topFormat;
            u32 topStride;
            u8  bottom[320 * 240 * 4];
            u32 bottomFormat;
            u32 bottomStride;
        };

        // Return true if the OSD must exit
        static void     Initialize(void);
        static bool     OSDCallback(u32 isBottom, void* addr, void* addrB, int stride, int format);
        static s32      TaskCallback(void *arg);
        static void     UpdateFileCount(void);

        static bool         IsEnabled;
        static u32          Hotkeys;
        static u32          Screens;
        static Time         Timer;
        static std::string  Path;
        static std::string  Prefix;
        static ImageBuffer *ImgBuffer;

        static OnScreenshotCallback ScreenshotCallback;

    private:

        static u32          _count;
        static u32          _mode;
        static u32          _filecount;
        static u32          _display;
        static Clock        _timer;
        static Clock        _hotkeyTimer;
        static Task         _task;

        static LightEvent   _readyEvent;
        static LightEvent   _resumeEvent;
    };
}

#endif
