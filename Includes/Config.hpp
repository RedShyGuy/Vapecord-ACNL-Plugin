#pragma once

#include <CTRPluginFramework.hpp>

#define majorV 3
#define minorV 1
#define revisV 0

#define APP_VERSION (majorV * 100 + minorV * 10 + revisV)

namespace CTRPluginFramework {
	bool WriteLanguage(const std::string& langCode);
	bool ReadLanguage(std::string &outLang);
	void CheckForLanguageFile(void);

	void DeleteLanguage(void);
	void SetupLanguage(bool SetInMenu);
	void cheatsVisibility(bool SetInMenu);
	void resetSettings(bool SetInMenu);
}