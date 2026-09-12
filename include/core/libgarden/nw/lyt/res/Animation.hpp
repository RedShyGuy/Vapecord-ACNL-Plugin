#pragma once

#include <core/libgarden/nw/lyt/res/AnimationBlock.hpp>
#include <core/libgarden/nw/ut/BinaryFileHeader.hpp>

namespace nw::lyt::res
{
	struct Animation
	{
		ut::BinaryFileHeader header;
	};
	ASSERT_SIZE(Animation, 0x14);

	struct Pat1
	{
		ut::BinaryBlockHeader header;
		u16 order;
		u16 groupCount;
		u32 nameOffset;
		u32 groupNamesOffset;
		s16 startFrame;
		s16 endFrame;
		u8 childBinding;
		u8 padding[3];
	};
	ASSERT_SIZE(Pat1, 0x1c);
}
