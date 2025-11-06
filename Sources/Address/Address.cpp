#include "Address/Address.hpp"
#include "Address/Addresses.hpp"

namespace CTRPluginFramework {
	std::string Address::regionName = "";
	Address::Region Address::regionId;

	std::string Address::LoadRegion(void) {
		switch(Process::GetTitleID()) {
			case TID_USA:
				regionName = "USA";
				regionId = Region::USA;
			break;
			case TID_USAWA:
				regionName = "USAWA";
				regionId = Region::USAWA;
			break;
			case TID_EUR:
				regionName = "EUR";
				regionId = Region::USAWA;
			break;
			case TID_EURWA:
			case TID_EURWL:
				regionName = "EURWA";
				regionId = Region::EURWA;
			break;
			case TID_JPN:
				regionName = "JPN";
				regionId = Region::JPN;
			break;
			case TID_JPNWA:
				regionName = "JPNWA";
				regionId = Region::JPNWA;
			break;
			case TID_KOR:
				regionName = "KOR";
				regionId = Region::KOR;
			break;
			case TID_KORWA:
				regionName = "KORWA";
				regionId = Region::KORWA;
			break;
			default:
				regionName = "";
			break;
		}
		return regionName;
	}

	Address::Address(u32 address) {
		if (regionId == Region::USA) {
			addr = address;
			return;
		}

		for (const auto& row : ADDRESSES) {
			if (row[0] == address) {
				addr = row[regionId];
				return;
			}
		}
	};
}