#include "core/Config.hpp"
#include "core/HUD.hpp"
#include "Files.h"
#include "core/infrastructure/Address.hpp"

namespace CTRPluginFramework {
	namespace {
		constexpr int CONFIG_FORMAT_VERSION = 2;

		static std::string _GetConfigPath() {
			return Utils::Format(PATH_PLUGIN_CONFIG, Address::regionName.c_str());
		}

		static bool _TryParseInt(const std::string &value, int &out) {
			if (value.empty()) {
				return false;
			}

			int sign = 1;
			size_t index = 0;
			if (value[0] == '-') {
				sign = -1;
				index = 1;
			}

			if (index >= value.size()) {
				return false;
			}

			int result = 0;
			for (; index < value.size(); ++index) {
				const char c = value[index];
				if (c < '0' || c > '9') {
					return false;
				}
				result = (result * 10) + (c - '0');
			}

			out = result * sign;
			return true;
		}

		static std::string _Trim(const std::string &input) {
			size_t start = 0;
			while (start < input.size() && (input[start] == ' ' || input[start] == '\t' || input[start] == '\r' || input[start] == '\n')) {
				++start;
			}

			size_t end = input.size();
			while (end > start && (input[end - 1] == ' ' || input[end - 1] == '\t' || input[end - 1] == '\r' || input[end - 1] == '\n')) {
				--end;
			}

			return input.substr(start, end - start);
		}

		static bool _ReadAllText(const std::string &path, std::string &outText) {
			File file;
			if (File::Open(file, path, File::READ) != File::SUCCESS) {
				return false;
			}

			const size_t fileSize = static_cast<size_t>(file.GetSize());
			outText.clear();
			if (fileSize == 0) {
				file.Close();
				return true;
			}

			outText.resize(fileSize);
			if (file.Read(&outText[0], fileSize) != File::SUCCESS) {
				file.Close();
				return false;
			}

			file.Close();
			return true;
		}

		static bool _WriteAllText(const std::string &path, const std::string &text) {
			if (File::Exists(path)) {
				File::Remove(path);
			}

			File file;
			if (File::Open(file, path, File::CREATE | File::WRITE) != File::SUCCESS) {
				return false;
			}

			if (!text.empty() && file.Write(text.data(), text.size()) != File::SUCCESS) {
				file.Close();
				return false;
			}

			file.Close();
			return true;
		}

		static bool _EnsureConfigDirectories() {
			if (!Directory::IsExists(V_STANDARD)) {
				Directory::Create(V_STANDARD);
			}

			if (!Directory::IsExists(V_DATA)) {
				Directory::Create(V_DATA);
			}

			const std::string regionDir = Utils::Format(V_DIRECTORY, Address::regionName.c_str());
			if (!Directory::IsExists(regionDir)) {
				Directory::Create(regionDir);
			}

			return true;
		}

		struct ParsedConfig {
			Config::PluginConfig config;
			bool hasFormat = false;
			int formatVersion = 0;
			bool hasLanguage = false;
			bool hasPluginVersion = false;
			bool hasSaveReminder = false;
		};

		static ParsedConfig _ParseConfig(const std::string &content) {
			ParsedConfig parsed;
			parsed.config.languageCode.clear();
			parsed.config.pluginVersion = Config::GetPluginVersionString();

			size_t lineStart = 0;
			while (lineStart <= content.size()) {
				size_t lineEnd = content.find('\n', lineStart);
				if (lineEnd == std::string::npos) {
					lineEnd = content.size();
				}

				std::string line = _Trim(content.substr(lineStart, lineEnd - lineStart));
				if (!line.empty() && line[0] != '#' && line[0] != ';') {
					size_t split = line.find('=');
					if (split != std::string::npos) {
						std::string key = _Trim(line.substr(0, split));
						std::string value = _Trim(line.substr(split + 1));

						if (key == "language") {
							parsed.config.languageCode = value;
							parsed.hasLanguage = true;
						}
						else if (key == "plugin_version") {
							parsed.config.pluginVersion = value;
							parsed.hasPluginVersion = true;
						}
						else if (key == "config_format") {
							parsed.hasFormat = _TryParseInt(value, parsed.formatVersion);
						}
						else if (key == "save_reminder_enabled") {
							parsed.config.saveReminderEnabled = (value == "1" || value == "true");
							parsed.hasSaveReminder = true;
						}
						else if (key == "save_reminder_interval") {
							_TryParseInt(value, parsed.config.saveReminderInterval);
							parsed.hasSaveReminder = true;
						}
						else if (key == "scam_warning_shown") {
							parsed.config.scamWarningShown = (value == "1" || value == "true");
						}
					}
				}

				if (lineEnd == content.size()) {
					break;
				}
				lineStart = lineEnd + 1;
			}

			return parsed;
		}

		static std::string _BuildConfigText(const Config::PluginConfig &config) {
			return "# Vapecord ACNL Plugin config\n"
				"config_format=" + std::to_string(CONFIG_FORMAT_VERSION) + "\n"
				"plugin_version=" + config.pluginVersion + "\n"
				"language=" + config.languageCode + "\n"
				"save_reminder_enabled=" + (config.saveReminderEnabled ? "1" : "0") + "\n"
				"save_reminder_interval=" + std::to_string(config.saveReminderInterval) + "\n"
				"scam_warning_shown=" + (config.scamWarningShown ? "1" : "0") + "\n";
		}
	}

	std::string Config::GetPluginVersionString() {
		return Utils::Format("%d.%d.%d", majorV, minorV, revisV);
	}

	bool Config::ReadConfig(PluginConfig &outConfig) {
		if (!EnsureConfigFile()) {
			return false;
		}

		std::string content;
		if (!_ReadAllText(_GetConfigPath(), content)) {
			return false;
		}

		outConfig = _ParseConfig(content).config;

		return true;
	}

	bool Config::WriteConfig(const PluginConfig &config) {
		if (!_EnsureConfigDirectories()) {
			return false;
		}

		PluginConfig normalized = config;
		if (normalized.pluginVersion.empty()) {
			normalized.pluginVersion = GetPluginVersionString();
		}

		return _WriteAllText(_GetConfigPath(), _BuildConfigText(normalized));
	}

	bool Config::EnsureConfigFile() {
		if (!_EnsureConfigDirectories()) {
			return false;
		}

		const std::string configPath = _GetConfigPath();
		if (!File::Exists(configPath)) {
			PluginConfig defaultConfig;
			defaultConfig.languageCode = "";
			defaultConfig.pluginVersion = GetPluginVersionString();
			return _WriteAllText(configPath, _BuildConfigText(defaultConfig));
		}

		return true;
	}

	bool Config::IsConfigOutdated() {
		if (!EnsureConfigFile()) {
			return true;
		}

		std::string content;
		if (!_ReadAllText(_GetConfigPath(), content)) {
			return true;
		}

		const ParsedConfig parsed = _ParseConfig(content);
		if (!parsed.hasFormat || parsed.formatVersion < CONFIG_FORMAT_VERSION) {
			return true;
		}

		if (!parsed.hasPluginVersion || parsed.config.pluginVersion != GetPluginVersionString()) {
			return true;
		}

		if (!parsed.hasLanguage) {
			return true;
		}

		return false;
	}

	bool Config::UpdateConfig() {
		if (!EnsureConfigFile()) {
			return false;
		}

		std::string content;
		if (!_ReadAllText(_GetConfigPath(), content)) {
			return false;
		}

		const ParsedConfig parsed = _ParseConfig(content);

		PluginConfig merged;
		merged.languageCode = parsed.hasLanguage ? parsed.config.languageCode : "";
		merged.pluginVersion = GetPluginVersionString();
		merged.scamWarningShown = false;
		merged.saveReminderEnabled = parsed.hasSaveReminder ? parsed.config.saveReminderEnabled : true;
		merged.saveReminderInterval = parsed.hasSaveReminder ? parsed.config.saveReminderInterval : 25;

		return WriteConfig(merged);
	}

	bool Config::SetLanguage(const std::string &langCode) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}

		config.languageCode = langCode;
		config.pluginVersion = GetPluginVersionString();
		return WriteConfig(config);
	}

	bool Config::GetLanguage(std::string &outLang) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}

		outLang = config.languageCode;
		return !outLang.empty();
	}

	void Config::DeleteLanguage(void) {
		File::Remove(_GetConfigPath());
	}

	bool Config::GetSaveReminderEnabled(bool &outEnabled) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}
		outEnabled = config.saveReminderEnabled;
		return true;
	}

	bool Config::SetSaveReminderEnabled(bool enabled) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}
		config.saveReminderEnabled = enabled;
		return WriteConfig(config);
	}

	bool Config::GetSaveReminderInterval(int &outInterval) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}
		outInterval = config.saveReminderInterval;
		return true;
	}

	bool Config::SetSaveReminderInterval(int interval) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}
		config.saveReminderInterval = interval;
		return WriteConfig(config);
	}

	bool Config::GetScamWarningShown(bool &outShown) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}
		outShown = config.scamWarningShown;
		return true;
	}

	bool Config::SetScamWarningShown(bool shown) {
		PluginConfig config;
		if (!ReadConfig(config)) {
			return false;
		}
		config.scamWarningShown = shown;
		return WriteConfig(config);
	}

    void Config::HandleConfigMigration() {
		if (IsConfigOutdated()) {
			UpdateConfig();
            HUD::Notify("Plugin configuration was migrated", Color::Purple);
		}

		const std::string region = Address::regionName;
		const std::string legacyFiles[] = {
			Utils::Format("E:/Vapecord/%s/plugin.bin", region.c_str()),
			Utils::Format("E:/Vapecord/%s/set_language.bin", region.c_str()),
			Utils::Format("E:/Vapecord/%s/quickmenu.bin", region.c_str()),
			Utils::Format("E:/Vapecord/%s/hidden.bin", region.c_str())
		};

		for (const std::string &path : legacyFiles) {
			if (File::Exists(path)) {
				File::Remove(path);
			}
		}
    }
}
