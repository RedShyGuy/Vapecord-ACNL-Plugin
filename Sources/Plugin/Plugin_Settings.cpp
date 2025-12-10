#include "Config.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Save.hpp"
#include "cheats.hpp"
#include "Files.h"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
	void DeleteLanguage(void) {
		File::Remove(Utils::Format(CONFIGNAME, Address::regionName.c_str()));
	}

	bool WriteLanguage(const std::string& langCode) {
		File file;
		if (File::Open(file, Utils::Format(CONFIGNAME, Address::regionName.c_str()), File::CREATE | File::WRITE) != File::SUCCESS) {
			return false;
		}

		u8 len = static_cast<u8>(langCode.size());
		if (file.Write(&len, sizeof(len)) != File::SUCCESS) {
			file.Close();
			return false;
		}

		if (file.Write(langCode.data(), len) != File::SUCCESS) {
			file.Close();
			return false;
		}

		file.Close();
		return true;
	}

	bool ReadLanguage(std::string &outLang) {
		File file;
		if (File::Open(file, Utils::Format(CONFIGNAME, Address::regionName.c_str()), File::READ) != File::SUCCESS) {
			return false;
		}

		u8 len = 0;
		if (file.Read(&len, sizeof(len)) != File::SUCCESS) {
			file.Close();
			return false;
		}

		if (len == 0) {
			file.Close();
			return false;
		}

		outLang.resize(len);
		if (file.Read(&outLang[0], len) != File::SUCCESS) {
			file.Close();
			return false;
		}

		file.Close();
		return true;
	}

	void CheckForLanguageFile(void) {
		if(!Directory::IsExists(V_STANDARD)) {
			Directory::Create(V_STANDARD);
		}

		if(!Directory::IsExists(V_DATA)) {
			Directory::Create(V_DATA);
		}

		if(!Directory::IsExists(Utils::Format(V_DIRECTORY, Address::regionName.c_str()))) {
			Directory::Create(Utils::Format(V_DIRECTORY, Address::regionName.c_str()));
		}

		if(!File::Exists(Utils::Format(CONFIGNAME, Address::regionName.c_str()))) {
			File::Create(Utils::Format(CONFIGNAME, Address::regionName.c_str()));
		}
	} 
}