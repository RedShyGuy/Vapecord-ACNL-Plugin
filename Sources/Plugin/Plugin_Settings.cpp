#include "Config.hpp"
#include "Helpers/Wrapper.hpp"
#include "Helpers/Save.hpp"
#include "Helpers/PluginMenuData.hpp"
#include "Helpers/KeySequence.hpp"
#include "Helpers/Address.hpp"
#include "cheats.hpp"
#include "Files.h"

namespace CTRPluginFramework {
	void WriteLanguage(f_Language language) {
		File file(Utils::Format(CONFIGNAME, regionName.c_str()), File::WRITE);
		file.Seek(0, File::SeekPos::SET);
		file.Write(&language, 1);
	}

	void ReadLanguage(f_Language &language) {
		File file(Utils::Format(CONFIGNAME, regionName.c_str()), File::READ);
		file.Seek(0, File::SeekPos::SET);
		file.Read(&language, 1);
	}

	void CheckForLanguageFile(void) {
		if(!Directory::IsExists(V_STANDARD)) Directory::Create(V_STANDARD);
		if(!Directory::IsExists(V_DATA)) Directory::Create(V_DATA);

		if(!Directory::IsExists(Utils::Format(V_DIRECTORY, regionName.c_str()))) 
			Directory::Create(Utils::Format(V_DIRECTORY, regionName.c_str()));

		if(!File::Exists(Utils::Format(CONFIGNAME, regionName.c_str()))) {
			File::Create(Utils::Format(CONFIGNAME, regionName.c_str()));
		}
	} 
}