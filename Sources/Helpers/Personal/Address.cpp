#include "Helpers/Address.hpp"

namespace CTRPluginFramework {
	CurrRegion c_Region = INVALID;
//AutoRegion
	Address::Address(u32 usa, u32 usawa, u32 eur, u32 eurwa, u32 jpn, u32 jpnwa, u32 kor, u32 korwa) {
		switch(c_Region) {
			case CurrRegion::USA: addr = usa; break;
			case CurrRegion::USAWA: addr = usawa; break; 
			case CurrRegion::EUR: addr = eur; break;
			case CurrRegion::EURWA: addr = eurwa; break;
			case CurrRegion::JPN: addr = jpn; break;
			case CurrRegion::JPNWA: addr = jpnwa; break;
			case CurrRegion::KOR: addr = kor; break;
			case CurrRegion::KORWA: addr = korwa; break;
			case CurrRegion::EURWL: addr = eurwa; break;
			case CurrRegion::INVALID: addr = 0; break;
			default: addr = 0; break;
		}
	}

	Address::Address(u32 address) { 
		addr = address; 
	};

	Address::Address(void) { 
		addr = 0;
	};
}