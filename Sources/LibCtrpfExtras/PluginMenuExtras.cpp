#include "LibCtrpfExtras/PluginMenuExtras.hpp"
#include "LibCtrpfExtras/MenuFolderExtras.hpp"
#include "LibCtrpfExtras/MenuEntryExtras.hpp"
#include "LibCtrpfExtras/HotkeyExtras.hpp"

namespace CTRPluginFramework {
    /*
    Every entry in vapecord uses menufolderextras and menuentryextras.
    So hard casting won't cause issues.
    */
    void UpdateFolderAndEntries(MenuFolder *folder) {
        auto extraFolder = static_cast<MenuFolderExtras*>(folder);

        for (MenuEntry *entry : folder->GetEntryList()) {
            auto extraEntry = static_cast<MenuEntryExtras*>(entry);
            extraEntry->SetColor(MenuFolderExtras::GetFolderColor(extraFolder->GetFolderType()));
            extraEntry->Update();

            for (int i = 0; i < entry->Hotkeys.Count(); i++) {
                Hotkey &hotkey = entry->Hotkeys[i];
                TextID nameKey = extraEntry->GetHotkeyNameKeys()[i];

                hotkey.operator=(Language::getInstance()->get(nameKey));
            }

            entry->RefreshNote();
        }

        extraFolder->Update();
    }

    void PluginMenuExtras::Update() {
        PluginMenu *menu = PluginMenu::GetRunningInstance();
		if (menu) {
			for (MenuFolder *folder : menu->GetFolderList()) {
                if (!folder->GetFolderList().empty()) {
                    for (MenuFolder *subFolder : folder->GetFolderList()) {
                        UpdateFolderAndEntries(subFolder);
                    }
                }

				UpdateFolderAndEntries(folder);
            }

            for (MenuEntry *entry : menu->GetEntryList()) {
                auto extraEntry = static_cast<MenuEntryExtras*>(entry);
                extraEntry->SetColor(Color::White);
                extraEntry->Update();

                for (int i = 0; i < entry->Hotkeys.Count(); i++) {
                    Hotkey &hotkey = entry->Hotkeys[i];
                    TextID nameKey = extraEntry->GetHotkeyNameKeys()[i];

                    hotkey.operator=(Language::getInstance()->get(nameKey));
                }

                entry->RefreshNote();
            }
		}
    }
}