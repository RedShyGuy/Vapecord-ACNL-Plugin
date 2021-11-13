#include "Helpers/QuickMenu.hpp"
#include "MenuPointers.hpp"

namespace CTRPluginFramework {
    std::vector<MenuEntry*> TestEntry;

    void QuickMenu::Init() {
        std::vector<MenuEntry *> Entrys = SAVEC->GetEntryList();

        TestEntry.push_back(Entrys[0]);
        TestEntry.push_back(Entrys[1]);
        TestEntry.push_back(Entrys[2]);
    }

    void QuickMenu::ListEntrys(std::vector<MenuEntry*> &quickmenu) {
        quickmenu = TestEntry;
    }

    void QuickMenu::AddEntry(MenuEntry* entry) {
        TestEntry.push_back(entry);
    }
}