#ifndef QUICKMENU_HPP
#define QUICKMENU_HPP

#include <CTRPluginFramework.hpp>
#include <Helpers/PluginMenuData.hpp>

namespace CTRPluginFramework {
    class QuickMenu {
        public:
            static void Init(void);
            static void AddEntry(MenuEntry *entry);
            static void RemoveEntry(MenuEntry *entry);
            static void ListAvailableCogEntrys(std::vector<MenuEntry *> &data);
            static void UpdateFile(void);

            static std::vector<MenuEntry *> obj_QuickMenu;
    };
}
#endif