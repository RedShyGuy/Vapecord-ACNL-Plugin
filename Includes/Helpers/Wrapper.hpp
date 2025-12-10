#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	using OnChangeCallback = void(*)(Keyboard &keyboard, KeyboardEvent &event);
	
	struct WrapLoc {
		u32 *Address;
		int Lenght;
	};

	enum ExHandler {
		SUCCESS,
		ERROR_OP, //error opening file
		ERROR_DRD, //dump/restore/delete error
		ERROR_LI, //error listing
		ERROR_UN, //Unexcpected error
	};

	u32 decodeARMBranch(const u32 src, const u32 val);

	extern Directory restoreDIR;

	namespace Wrap {
		ExHandler 	Dump(const std::string& path, std::string& filename, const std::string& filetype, WrapLoc *dump, ...);

		extern Directory restoreDIR;
		ExHandler	Restore(const std::string& path, const std::string& filetype, const std::string& KBMsg, OnChangeCallback cb, bool HasMSGBox, WrapLoc *rest, ...);

		ExHandler	Delete(const std::string& path, const std::string& filetype);
		//bool 		SetIcon(CustomIcon::Pixel* pixel, const std::string& path, CustomIcon &icon);
		//int 		ParseTXT(const std::string& filename, std::vector<std::string>& output);

		u32 		CalculateBranchInstruction(u32 PC, u32 target);

		u32			GetBranchTarget(u32 PC, u32 instruction);

		template<typename T>
		bool		KB(const std::string &msg, bool hex, const int length, T &output, T def, OnChangeCallback cb = nullptr) {
			Keyboard kb(msg);
			kb.IsHexadecimal(hex);
			kb.SetMaxLength(length);
			kb.OnKeyboardEvent(cb);
			return kb.Open((T &)output, (T)def) == 0;
		}
	}
	
	Process::ExceptionCallbackState CustomExceptionHandler(ERRF_ExceptionInfo* excep, CpuRegisters* regs);
}