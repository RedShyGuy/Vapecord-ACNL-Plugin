#ifndef QUICKMENU_HPP
#define QUICKMENU_HPP

#include <CTRPluginFramework.hpp>
#include <Helpers/PluginMenuData.hpp>

namespace CTRPluginFramework {
    class QuickMenu {
        public:
            static void Init(void);
            static void AddEntry(EntryData entry);
            static void RemoveEntry(EntryData entry);
            static void ListAvailableCogEntrys(std::vector<EntryData> &data);
            static void UpdateFile(void);

            static std::vector<EntryData> obj_QuickMenu;
    };
}
#endif