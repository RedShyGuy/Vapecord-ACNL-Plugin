#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <CTRPluginFramework.hpp>

#define TID_USA 		0x0004000000086300
#define TID_USAWA 		0x0004000000198E00
#define TID_EUR 		0x0004000000086400
#define TID_EURWA 		0x0004000000198F00
#define TID_JPN 		0x0004000000086200
#define TID_JPNWA 		0x0004000000198D00
#define TID_KOR 		0x0004000000086500
#define TID_KORWA 		0x0004000000199000
#define TID_EURWL 		0x00040000004C5700

namespace CTRPluginFramework {
	enum CurrRegion {
		INVALID = 0,
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

	extern std::string regionName;

	class Address {
		public:
			Address(u32 usa, u32 usawa, u32 eur, u32 eurwa, u32 jpn, u32 jpnwa, u32 kor, u32 korwa);
			
			Address(u32 address);

			Address(void);

			u32 addr;

			template <typename T, class ...Args>
			T Call(Args ...args) {
				return((T(*)(Args...))(addr))(args...);
			};	
	};
}
#endif