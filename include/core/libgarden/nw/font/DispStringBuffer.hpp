#pragma once

#include <core/libgarden/nn/types.h>

namespace nw::font
{

	struct DispStringBuffer
	{
		static constexpr u32 CalcCommandBufferCapacity(u32 charCount)
		{
			return (charCount * 0x4e + 0x39) & 0xfffffffc;
		}

		DispStringBuffer();
	};

}
