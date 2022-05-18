#ifndef CTRPLUGINFRAMEWORKIMPL_SYSTEMIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_SYSTEMIMPL_HPP

namespace CTRPluginFramework
{
    class SystemImpl
    {
    public:
        static void     Initialize(void);
        static bool     WantsToSleep(void);
        static bool     IsSleeping(void);
        static void     ReadyToSleep(void);
        static void     WakeUpFromSleep(void);
        // Thread must exit if this function returns true
        static bool     Status(void);

        static bool     IsNew3DS;
        static u32      CFWVersion;
        static u32      RosalinaHotkey;
        static u32      AptStatus;
        static u8       Language;
    };
}

#endif
