#ifndef QUICKMENU_HPP
#define QUICKMENU_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    class QuickMenu {
        public:
            static void Init(void);
            static void ListEntrys(std::vector<MenuEntry*> &quickmenu);
            static void AddEntry(MenuEntry* entry);


    };
}
#endif