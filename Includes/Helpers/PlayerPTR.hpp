#ifndef PLAYERPTR_HPP
#define PLAYERPTR_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace PlayerPTR {
		u32		Pointer(u32 set);
		bool	Write32(u32 lenght, u32 value);	
		bool	Write16(u32 lenght, u16 value);	
		bool	Write8(u32 lenght, u8 value);	
	}
}
#endif