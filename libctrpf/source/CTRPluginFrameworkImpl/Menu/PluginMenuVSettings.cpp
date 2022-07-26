#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuVSettings.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Menu/PluginMenu.hpp"
#include "CTRPluginFramework/System/Directory.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/System/Sleep.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"

#include <3ds.h>

#include <ctime>
#include <cstring>
#include <cstdio>

namespace CTRPluginFramework
{
    PluginMenuVSettings::CustomVFunction PluginMenuVSettings::setColor = nullptr;
    PluginMenuVSettings::CustomVFunction PluginMenuVSettings::setLanguage = nullptr;
    PluginMenuVSettings::CustomVFunction PluginMenuVSettings::setFwk = nullptr;
    PluginMenuVSettings::CustomVFunction PluginMenuVSettings::setVisibility = nullptr;
    PluginMenuVSettings::CustomVFunction PluginMenuVSettings::setReset = nullptr;

    PluginMenuVSettings::PluginMenuVSettings() :
        _mainMenu("Vapecord Settings"),
        _colorSettings("Set Color Mode"),
        _languageSettings("Set Language"),
        _FwkSettings("Set Plugin Colors"),
        _HideSettings("Hide/Unhide Cheats"),
        _resetSettings("Reset Settings"),
        _menu(&_mainMenu, nullptr),
        _exit(false)
    {
        InitMenu();
    }

    static void     V_SetColor(void)
    {
        if(PluginMenuVSettings::setColor != nullptr)
            PluginMenuVSettings::setColor(true);
    }

    static void     V_SetLanguage(void)
    {
        if(PluginMenuVSettings::setLanguage != nullptr)
            PluginMenuVSettings::setLanguage(true);
    }

    static void     V_SetFwk(void)
    {
        if(PluginMenuVSettings::setFwk != nullptr)
            PluginMenuVSettings::setFwk(true);
    }

    static void     V_HideCheats(void)
    {
        if(PluginMenuVSettings::setVisibility != nullptr)
            PluginMenuVSettings::setVisibility(true);
    }

    static void     V_Reset(void)
    {
        if(PluginMenuVSettings::setReset != nullptr)
            PluginMenuVSettings::setReset(true);
    }

    void    PluginMenuVSettings::InitMenu(void)
    {
        // Main menu
        _mainMenu.Append(new MenuEntryTools("Set Color Mode", V_SetColor, Icon::DrawColor));
        _mainMenu.Append(new MenuEntryTools("Set Language", V_SetLanguage, Icon::DrawLanguage));
        _mainMenu.Append(new MenuEntryTools("Set Plugin Colors", V_SetFwk, Icon::DrawPaint));
        _mainMenu.Append(new MenuEntryTools("Hide/Unhide Cheats", V_HideCheats, Icon::DrawHidden));
        _mainMenu.Append(new MenuEntryTools("Reset Settings", V_Reset, Icon::DrawReset));
    }

    bool    PluginMenuVSettings::operator()(EventList &eventList, Time &delta)
    {
        // Process Event
        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        // Update
        _Update(delta);

        static Task task([](void *arg) -> s32
        {
            static_cast<PluginMenuVSettings *>(arg)->_RenderTop();
            return 0;
        }, static_cast<void *>(this), Task::AppCores);

        // Render Top
        //_RenderTop();
        task.Start();

        // Render Bottom
        _RenderBottom();

        task.Wait();

        // Check buttons
        bool exit = _exit || Window::BottomWindow.MustClose();
        _exit = false;
        return (exit);
    }

    /*
    ** Process Event
    *****************/
    void    PluginMenuVSettings::_ProcessEvent(Event &event)
    {
        MenuItem    *item = nullptr;
        static int  selector = -1;

        int ret = _menu.ProcessEvent(event, &item);

        if (ret == MenuClose)
        {
            if (_menu.GetFolder() == &_mainMenu)
            {
                _exit = true;
                _menu.Open(&_mainMenu, selector);
            }
            else
                _menu.Open(&_mainMenu, selector);
        }
    }

    void PluginMenuVSettings::_RenderTopMenu(void)
    {

    }

    /*
    ** Render Top
    **************/

    void    PluginMenuVSettings::_RenderTop(void)
    {
        // Enable renderer
        Renderer::SetTarget(TOP);

        _menu.Draw();
    }

    /*
    ** Render Bottom
    *****************/
    void    PluginMenuVSettings::_RenderBottom(void)
    {
        const Color    &blank = Color::White;

        // Enable renderer
        Renderer::SetTarget(BOTTOM);

        // Window
        Window::BottomWindow.Draw();

        // Draw Framework version
        int posY = 30;
        Renderer::DrawString("Nothing to show here right now",  30, posY, blank);
    }

    /*
    ** Update
    ************/
    void    PluginMenuVSettings::_Update(Time delta)
    {
        /*
        ** Buttons
        *************/
        bool        isTouched = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
