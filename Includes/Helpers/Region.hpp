#ifndef REGION_HPP
#define REGION_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	enum CurrRegion {
		INVALID,
        USA,
		USAWA,
        EUR,
		EURWA,
        JPN,
		JPNWA,
		KOR,
		KORWA,
		EURWL //special case
    };
	
	extern CurrRegion c_Region;

	namespace Region {
		u32		AutoRegion(u32 usa, u32 usawa, u32 eur, u32 eurwa, u32 jpn, u32 jpnwa, u32 kor, u32 korwa);
		s8 		IsNewestVersion(std::string& versionSTR, const std::string& gameVersion);

		u32		FollowPointer(u32 pointer, ...);
	}
}
#endif