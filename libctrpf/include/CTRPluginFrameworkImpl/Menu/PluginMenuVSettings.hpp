#ifndef CTRPLUGINFRAMEWORKIMPL_PLUGINMENUVSETTINGS_HPP
#define CTRPLUGINFRAMEWORKIMPL_PLUGINMENUVSETTINGS_HPP

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"

#include <vector>
#include <string>
#include "MenuEntryTools.hpp"

namespace CTRPluginFramework
{
    class PluginMenuVSettings
    {
        using EventList = std::vector<Event>;
    public:
        PluginMenuVSettings();

        void UpdateSettings();
        void InitMenu();

        ~PluginMenuVSettings(){}

        // Return true if the Close Button is pressed, else false
        bool    operator()(EventList &eventList, Time &delta);
    private:
        void    _ProcessEvent(Event &event);
        void    _RenderTop(void);
        void    _RenderTopMenu(void);
        void    _RenderBottom(void);
        void    _Update(Time delta);

        MenuFolderImpl  _mainMenu;
        MenuEntryImpl   _colorSettings;
        MenuEntryImpl   _languageSettings;
        MenuEntryImpl   _FwkSettings;
        MenuEntryImpl   _HideSettings;
        MenuEntryImpl   _resetSettings;

        Menu            _menu;
        bool            _exit;
    };
}

#endif