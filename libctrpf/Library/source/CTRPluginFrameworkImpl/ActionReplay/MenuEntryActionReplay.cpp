#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/MenuEntryActionReplay.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARHandler.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"

namespace CTRPluginFramework
{
    static void    ActionReplay_ExecuteCode(MenuEntryImpl *entry)
    {
        if (!entry || !entry->IsActivated()) return;

        MenuEntryActionReplay *ar = reinterpret_cast<MenuEntryActionReplay *>(entry);
        ARCodeContext &ctx = ar->context;

        if (ctx.hasError || ctx.codes.empty())
        {
            entry->Disable();
            return;
        }

        ARHandler::Execute(ctx);
    }

    MenuEntryActionReplay::MenuEntryActionReplay(const std::string &name, const std::string &note) :
        MenuEntryImpl{ name, note }
    {
        _type = ActionReplay;
        context.Clear();
        GameFunc = (FuncPointer)ActionReplay_ExecuteCode;
    }

    MenuEntryActionReplay::~MenuEntryActionReplay()
    {
        // Remove from execution's list, will also disable all hooks
        PluginMenuExecuteLoop::RemoveAR(this);
    }

    MenuEntryActionReplay*    MenuEntryActionReplay::Update(void)
    {
        context.Update();
        if (!name.empty())
        {
            if (context.hasError)
            {
                if (name[0] == 0x1B)
                    name = name.substr(4);
                name = Color::Red << name;
            }
        }
        return this;
    }
}
