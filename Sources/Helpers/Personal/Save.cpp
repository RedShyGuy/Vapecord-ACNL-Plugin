#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

namespace CTRPluginFramework {
	u32 Save::s_SaveAddress = 0;
	Save *Save::s_Instance = nullptr;

	Save::Save() {
		if(s_SaveAddress == 0) {
			static const u32 d_garden = Region::AutoRegion(0x2FB344, 0x2FB328, 0x2FB340, 0x2FB340, 0x2FB354, 0x2FB354, 0x2FB360, 0x2FB360); 
			static FUNCT func(d_garden);	
			s_SaveAddress = func.Call<u32>();
		}

		if(s_Instance != nullptr)
            delete s_Instance;

        s_Instance = this;
	}

	Save *Save::GetInstance() {
		if(s_Instance == nullptr)
            s_Instance = new Save();

        return s_Instance;
	}

	u32 Save::Address(u32 add) {
		return s_SaveAddress + add;
	}
}