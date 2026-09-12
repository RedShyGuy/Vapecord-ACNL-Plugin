#pragma once

#include <core/libgarden/nn/types.h>

namespace nw::ut
{
	struct BinaryFileHeader
	{
		u32 magic;
		u16 byteOrderMark;
		u16 headerSize;
		u32 version;
		u32 fileSize;
		u32 blockCount;
	};
	ASSERT_SIZE(BinaryFileHeader, 0x14);
}
