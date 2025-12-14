#include "Address/Address.hpp"
#include "Address/Addresses.hpp"

namespace CTRPluginFramework {
	std::unordered_map<u32, u32> Address::origValList;
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
			SetAddressData(address);
			return;
		}

		for (const auto& row : ADDRESSES) {
			if (row[0] == address) {
				SetAddressData(row[regionId]);
				return;
			}
		}
	};

	void Address::SetAddressData(u32 address) {
		addr = address;

		//If address was already stored, use the stored original value
		if (origValList.find(addr) != origValList.end()) {
			origVal = origValList[addr];
			return;
		}
		//If not, read and store it
		origVal = *(u32 *)addr;
		origValList.emplace(addr, origVal);
	}

	Address::Address() {
		addr = 0;
		origVal = 0;
	}

	Address Address::decodeARMBranch(const u32 src, const u32 val) {
		s32 off = (val & 0xFFFFFF) << 2;
		off = (off << 6) >> 6; //sign extend

		Address address = Address();
		address.SetAddressData((u32)src + 8 + off);
		return address;
	}

	bool Address::IsRegion(Region region) {
		return regionId == region;
	}

	Address Address::MoveOffset(u32 offset) {
		Address copy = *this;
		copy.SetAddressData(copy.addr + offset);
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