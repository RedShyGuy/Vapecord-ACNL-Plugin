#pragma once

#include <CTRPluginFramework.hpp>

#include <string>

#define majorV 3
#define minorV 4
#define revisV 0

#define APP_VERSION (majorV * 100 + minorV * 10 + revisV)

namespace CTRPluginFramework {
	namespace Config {
		struct PluginConfig {
			std::string languageCode;
			std::string pluginVersion;
			bool saveReminderEnabled = true;
			int saveReminderInterval = 25;
			bool scamWarningShown = false;
		};

		std::string GetPluginVersionString();

		bool ReadConfig(PluginConfig &outConfig);
		bool WriteConfig(const PluginConfig &config);
		bool EnsureConfigFile();
		bool IsConfigOutdated();
		bool UpdateConfig();

		void HandleConfigMigration();

		bool SetLanguage(const std::string &langCode);
		bool GetLanguage(std::string &outLang);

		void DeleteLanguage(void);
		void SetupLanguage(bool SetInMenu);

		bool GetSaveReminderEnabled(bool &outEnabled);
		bool SetSaveReminderEnabled(bool enabled);
		bool GetSaveReminderInterval(int &outInterval);
		bool SetSaveReminderInterval(int interval);

		bool GetScamWarningShown(bool &outShown);
		bool SetScamWarningShown(bool shown);
	};
}