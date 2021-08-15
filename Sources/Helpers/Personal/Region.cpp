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

	u32 Region::GetPatternAddress(const std::vector<u32>& pattern, u32 add, int pos) {
		u32* found = (u32 *)Utils::Search<u32>(0x00100000, Process::GetTextSize(), pattern);
		return (found[pos] + add);
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