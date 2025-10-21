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
	class Address {
		public:
            Address(const std::string& key);

			Address(u32 address);

            static std::string LoadRegion(void);

			u32 addr;

            static std::string regionName;

			template <typename T, class ...Args>
			T Call(Args ...args) {
				return((T(*)(Args...))(addr))(args...);
			};	
	};
}
#endif