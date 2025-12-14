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
			origVal = *(u32 *)addr;
			return;
		}

		for (const auto& row : ADDRESSES) {
			if (row[0] == address) {
				addr = row[regionId];
				origVal = *(u32 *)addr;
				return;
			}
		}
	};

	Address::Address() {
		addr = 0;
		origVal = 0;
	}

	Address Address::decodeARMBranch(const u32 src, const u32 val) {
		s32 off = (val & 0xFFFFFF) << 2;
		off = (off << 6) >> 6; //sign extend

		Address address = Address();
		address.addr = (u32)src + 8 + off;
		address.origVal = *(u32 *)address.addr;
		
		return address;
	}

	bool Address::IsRegion(Region region) {
		return regionId == region;
	}

	Address Address::MoveOffset(u32 offset) {
		Address copy = *this;

		copy.addr += offset;
		copy.origVal = *(u32 *)copy.addr;
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

	bool Address::IsPatched(void) {
		return (*(u32*)addr != origVal);
	}
}
