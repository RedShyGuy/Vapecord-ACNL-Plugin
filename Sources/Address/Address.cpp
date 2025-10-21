#include "Address/AddressReader.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {
	std::string Address::regionName = "";

	std::string Address::LoadRegion(void) {
		switch(Process::GetTitleID()) {
			case TID_USA:
				regionName = "USA";
			break;
			case TID_USAWA:
				regionName = "USAWA";
			break;
			case TID_EUR:
				regionName = "EUR";
			break;
			case TID_EURWA:
			case TID_EURWL:
				regionName = "EURWA";
			break;
			case TID_JPN:
				regionName = "JPN";
			break;
			case TID_JPNWA:
				regionName = "JPNWA";
			break;
			case TID_KOR:
				regionName = "KOR";
			break;
			case TID_KORWA:
				regionName = "KORWA";
			break;
			default:
				regionName = "";
			break;
		}
		return regionName;
	}

	Address::Address(const std::string& key) { 
		addr = AddressReader::getInstance()->get(key); 
	};

	Address::Address(u32 address) {
		addr = address;
	}
}