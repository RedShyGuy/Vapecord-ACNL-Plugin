#pragma once

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
			enum Region {
				USA = 0,
				USAWA,
				EUR,
				EURWA,
				JPN,
				JPNWA,
				KOR,
				KORWA,
			};

			Address(void);

			Address(u32 address);

			static Address decodeARMBranch(const u32 src, const u32 val);

			static bool IsRegion(Region region);

            static std::string LoadRegion(void);

			u32 addr;
			u32 origVal;

            static std::string regionName;
			static Region regionId;

			Address MoveOffset(u32 offset);
			bool WriteFloat(float newValue);
			bool Patch(u32 newValue);
			bool Unpatch(void);

			template <typename T, class ...Args>
			T Call(Args ...args) {
				return((T(*)(Args...))(addr))(args...);
			};	
	};
}