#include "Helpers/PlayerPTR.hpp"
#include "Helpers/Player.hpp"

namespace CTRPluginFramework {
//get player pointer
	u32 PlayerPTR::Pointer(u32 set) {	
		return Player::GetSaveOffset(4) != 0 ? Player::GetSaveOffset(4) + set : 0;
	}
//Write 32	
	bool PlayerPTR::Write32(u32 lenght, u32 value) {
		return Process::Write32(PlayerPTR::Pointer(lenght), value);
	}
//Write 16
	bool PlayerPTR::Write16(u32 lenght, u16 value) {
		return Process::Write16(PlayerPTR::Pointer(lenght), value);
	}
//Write 8	
	bool PlayerPTR::Write8(u32 lenght, u8 value) {
		return Process::Write8(PlayerPTR::Pointer(lenght), value);
	}
//Write String
	bool PlayerPTR::WriteString(u32 lenght, const std::string &input, StringFormat outFmt) {
		return Process::WriteString(PlayerPTR::Pointer(lenght), input, outFmt);
	}
}
