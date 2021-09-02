#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"
#include <cstdarg>

namespace CTRPluginFramework {

	CurrRegion c_Region = INVALID;
//AutoRegion
	u32 Region::AutoRegion(u32 usa, u32 usawa, u32 eur, u32 eurwa, u32 jpn, u32 jpnwa, u32 kor, u32 korwa) {
		switch(c_Region) {	
			case CurrRegion::USA: return usa;  
			case CurrRegion::USAWA: return usawa; 
			case CurrRegion::EUR: return eur; 
			case CurrRegion::EURWA: return eurwa; 
			case CurrRegion::JPN: return jpn; 
			case CurrRegion::JPNWA: return jpnwa; 
			case CurrRegion::KOR: return kor; 
			case CurrRegion::KORWA: return korwa; 
			case CurrRegion::EURWL: return eurwa;
			case CurrRegion::INVALID: return 0;
			default: return 0;
		}
	}

	s8 Region::IsNewestVersion(std::string& versionSTR, const std::string& gameVersion) {
		versionSTR.clear();

		static const std::vector<u16> Pattern = { 0x0056, 0x0065, 0x0072, 0x002E, 0x0020 };
		u16* found = (u16 *)Utils::Search<u16>(0x00800000, 0x00100000, Pattern);
		if(found == nullptr)
			return -1;

		versionSTR = Utils::Format("%c.%c", (char)found[5], (char)found[7]);
		if(versionSTR != gameVersion)
			return -2;

		return 0;
	}
	
	bool Range(u32 value, u32 low, u32 high) {
		return(low <= value && value <= high); //return whether or not if the value is in a range
	}
	
	u32 Region::FollowPointer(u32 pointer, ...) {
		u32 offset;
		va_list pointers;
		va_start(pointers, pointer);
	//since the inital pointer will be a valid offset do a read32 call to it and store in offset	
		if(!Process::Read32(pointer, offset) || !Range(offset, 0x100000, 0x40000000))
			return -1;
	//go to next argument		
		pointer = va_arg(pointers, u32); 
	//the last arg needs to be -1 in order for the while loop to exit
		while(pointer != -1) {
			if(!Process::Read32(offset + pointer, offset) || !Range(offset, 0x100000, 0x40000000))
				return -1;
				
			pointer = va_arg(pointers, u32);
		}
		va_end(pointers);
		return offset;
	}
}