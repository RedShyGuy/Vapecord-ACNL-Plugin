#include "Config.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Save.hpp"
#include "cheats.hpp"
#include "Plugin/Plugin_Color.hpp"
#include "Files.h"

namespace CTRPluginFramework {
	void pluginSettingsEntry(MenuEntry *entry) {
		static const std::vector<std::string> settingsOptions = {
			Language::getInstance()->get(TextID::CHANGE_LANGUAGE),
			Language::getInstance()->get(TextID::CHANGE_MENU_COLORS)
		};

		Keyboard keyboard(Language::getInstance()->get(TextID::PLUGIN_SETTINGS), settingsOptions);
		int choice = keyboard.Open();
		if (choice < 0) {
			return;
		}

		switch (choice) {
			case 0:
				SetupLanguage(true);
				break;
			case 1:
				EditColors();
				break;
			default:
				break;
		}
	}
}