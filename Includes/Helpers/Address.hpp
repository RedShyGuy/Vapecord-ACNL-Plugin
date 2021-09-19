#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <CTRPluginFramework.hpp>

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