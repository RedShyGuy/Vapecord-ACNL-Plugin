#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUEXECUTELOOP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUEXECUTELOOP

#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"

#include <vector>
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include <3ds.h>

namespace CTRPluginFramework
{
    class MenuEntryImpl;
    class MenuEntryActionReplay;
    class PluginMenuExecuteLoop
    {
    public:
        PluginMenuExecuteLoop(void);
        ~PluginMenuExecuteLoop(void) = default;

        static void WriteEnabledCheatsToFile(Preferences::Header &header, File &file);

        static void     Add(MenuEntryImpl *entry);
        static void     Remove(MenuEntryImpl *entry);
        static void     ExecuteBuiltin(void);

        static void     AddAR(MenuEntryActionReplay *entry);
        static void     RemoveAR(MenuEntryActionReplay *entry);
        static void     ExecuteAR(void);

        bool    operator()(void);

    private:
        static PluginMenuExecuteLoop          * _firstInstance;

        Mutex   _mutex;

        std::vector<MenuEntryImpl *>            _builtinEnabledList{};
        std::vector<MenuEntryActionReplay *>    _arEnabledList{};
    };
}

#endif
