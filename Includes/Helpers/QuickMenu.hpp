#ifndef QUICKMENU_HPP
#define QUICKMENU_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    struct quickMenuData {
        std::vector<MenuEntry*> entry;
        std::vector<s8> ID;
    };  

    class QuickMenu {
        public:
            static void Init(void);
            static void ListEntrys(quickMenuData &quickmenu);
            static void AddEntry(MenuEntry* entry, s8 ID);
            static void ListAvailableCogEntrys(std::vector<MenuEntry*> &cogEntrys, std::vector<s8> &ID);

            static quickMenuData g_quickMenu;
            static std::vector<s8> q_quickMenuFile;
            static quickMenuData data;
    };
}
#endif