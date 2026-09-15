#pragma once

#include <CTRPluginFramework.hpp>
#include <type_traits>

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
			bool Patch(u32 newValue);
			bool Unpatch(void);
			bool IsPatched(void);

			template <typename T>
			bool Write(T newValue) {
				if constexpr (std::is_same_v<T, u32>) {
					return Patch(newValue);
				}
				else if constexpr (std::is_same_v<T, u16>) {
					return Process::Write16(addr, newValue);
				}
				else if constexpr (std::is_same_v<T, u8>) {
					return Process::Write8(addr, newValue);
				}
				else if constexpr (std::is_same_v<T, float>) {
					return Process::WriteFloat(addr, newValue);
				}
				else if constexpr (std::is_same_v<T, u64>) {
					return Process::Write64(addr, newValue);
				}
				else return false;
			};

			template <typename T, class ...Args>
			T Call(Args ...args) {
				return((T(*)(Args...))(addr))(args...);
			};
		private:
		 	void SetAddressData(u32 address);
			static std::unordered_map<u32, u32> origValList;
	};
}

using CTRPluginFramework::Address;