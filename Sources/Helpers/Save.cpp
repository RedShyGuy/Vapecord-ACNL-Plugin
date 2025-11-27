#include "Helpers/Save.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
	u32 Save::s_SaveAddress = 0;
	Save *Save::s_Instance = nullptr;

	Save::Save() {
		if(s_SaveAddress == 0) {
			static CTRPluginFramework::Address d_garden(0x2FB344); 
			s_SaveAddress = d_garden.Call<u32>();
		}

		if(s_Instance != nullptr) {
			delete s_Instance;
		}

        s_Instance = this;
	}

	Save *Save::GetInstance() {
		if(s_Instance == nullptr) {
			s_Instance = new Save();
		}

        return s_Instance;
	}

	u32 Save::Address(u32 add) {
		return s_SaveAddress + add;
	}
}