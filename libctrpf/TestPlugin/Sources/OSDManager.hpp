#ifndef OSD_MANAGER_HPP
#define OSD_MANAGER_HPP

#include <3ds.h>
#include "CTRPluginFramework.hpp"

#include <map>
#include <string>
#include <tuple>

namespace CTRPluginFramework
{
    #define OSDManager (*_OSDManager::GetInstance())
#define DEBUG 0
#if DEBUG
#define TRACE  { OSDManager["trace"].SetScreen(true).SetPos(10,50) = std::string(__FUNCTION__) << ":" << __LINE__; Sleep(Seconds(0.04f)); }
#define XTRACE(str, ...) { OSDManager["trace"].SetScreen(true).SetPos(0,30) = std::string(__FUNCTION__) << ":" << __LINE__ << Utils::Format(str, __VA_ARGS__); Sleep(Seconds(0.04f)); }
#else
#define TRACE
#define XTRACE
#endif

    using OSDMITuple = std::tuple<bool, std::string, u32, u32, bool>;
    struct OSDMI
    {
        OSDMI &operator=(const std::string &str);
        OSDMI &operator=(const OSDMITuple &tuple);
        OSDMI &SetPos(u32 posX, u32 posY);
        OSDMI &SetScreen(bool topScreen);
        OSDMI &Enable(void);
        OSDMI &Disable(void);
    private:
        friend class  _OSDManager;
        explicit OSDMI(OSDMITuple &tuple);

        OSDMITuple  &data;
    };

    class _OSDManager
    {
    public:
        ~_OSDManager(void);

        static _OSDManager  *GetInstance(void);

        OSDMI   operator[](const u32 key);
        void    Remove(const u32 key);
        void    Lock(void);
        void    Unlock(void);
        u32     Size(void);
    private:

        _OSDManager(void);

        static bool     OSDCallback(const Screen &screen);
        static s32      RenderTaskFunc(void *arg);
        static _OSDManager *_singleton;

        Mutex   _mutex;
        Task    _renderTask1;
        Task    _renderTask2;
        std::map<u32, std::tuple<bool, std::string, u32, u32, bool>> _items{};
    };
}

#endif
