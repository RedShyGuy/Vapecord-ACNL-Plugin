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
				regionId = Region::EUR;
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
			Process::Read32(addr, origVal);
			return;
		}

		for (const auto& row : ADDRESSES) {
			if (row[0] == address) {
				addr = row[regionId];
				Process::Read32(addr, origVal);
				return;
			}
		}
	};

	bool Address::IsRegion(Region region) {
		return regionId == region;
	}

	Address Address::MoveOffset(u32 offset) {
		Address copy = *this;

		copy.addr += offset;
		Process::Read32(copy.addr, copy.origVal);
		return copy;
	}

	bool Address::Patch(u32 newValue) {
		return Process::Patch(addr, newValue);
	};

	bool Address::WriteFloat(float newValue) {
		return Process::WriteFloat(addr, newValue);
	}

	bool Address::Unpatch(void) {
		return Process::Patch(addr, origVal);
	}
}