#ifndef CTRPLUGINFRAMEWORKIMPL_SUBMENU_HPP
#define CTRPLUGINFRAMEWORKIMPL_SUBMENU_HPP

#include "types.h"
#include "CTRPluginFramework/System/Clock.hpp"
#include <string>
#include <vector>

namespace CTRPluginFramework
{
    class Event;
    class SubMenu
    {
        using StringVector = std::vector<std::string>;
    public:
        SubMenu(const StringVector &options);
        ~SubMenu(void);

        // return -1 if nothing was selected, 0 or higher for the selected option
        int     operator()(void);
        void    Draw(void);
        void    ProcessEvent(const Event &event);
        bool    IsOpen(void);
        void    ChangeOptions(const StringVector &options);
        u32     OptionsCount(void);

    private:
        StringVector    _options;
        u32             _selector;
        bool            _isOpen;
        bool            _action;
        Clock           _fadeClock;
    };
}

#endif
