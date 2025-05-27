#ifndef CTRPLUGINFRAMEWORKIMPL_MENUENTRYACTIONREPLAY_HPP
#define CTRPLUGINFRAMEWORKIMPL_MENUENTRYACTIONREPLAY_HPP

#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCode.hpp"

namespace CTRPluginFramework
{
    class MenuEntryActionReplay : public MenuEntryImpl
    {
        friend class Menu;
    public:
        MenuEntryActionReplay(const std::string &name, const std::string &note = "");
        ~MenuEntryActionReplay();

        MenuEntryActionReplay*    Update(void);

        ARCodeContext   context;
    };
}

#endif
