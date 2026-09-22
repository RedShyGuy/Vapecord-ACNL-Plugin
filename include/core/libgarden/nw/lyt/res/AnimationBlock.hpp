#pragma once

#include <core/libgarden/nw/ut/BinaryBlockHeader.hpp>

#include <array>
#include <bit>

namespace nw::lyt::res
{
	struct AnimationBlock
	{
		ut::BinaryBlockHeader header;
		u16 frameCount;
		u8 loop;
		u16 textureCount;
		u16 entryCount;
		u32 entryTableOffset;
	};
	ASSERT_SIZE(AnimationBlock, 0x14);

	enum class AnimationTarget : u8
	{
		PANE,
		MATERIAL,
	};

	struct ALIGN(4) AnimationBlockEntry
	{
		std::array<char, 0x14> name;
		u8 tagCount;
		AnimationTarget target;
	};
	ASSERT_SIZE(AnimationBlockEntry, 0x18);

	enum class AnimationTagType : u32
	{
		PANE_SRT = std::bit_cast<u32>(std::array<char, 4> {'C', 'L', 'P', 'A'}),
		TEXTURE_SRT = std::bit_cast<u32>(std::array<char, 4> {'C', 'L', 'T', 'S'}),
		VISIBILITY = std::bit_cast<u32>(std::array<char, 4> {'C', 'L', 'V', 'I'}),
		VERTEX_COLOR = std::bit_cast<u32>(std::array<char, 4> {'C', 'L', 'V', 'C'}),
		MATERIAL_COLOR = std::bit_cast<u32>(std::array<char, 4> {'C', 'L', 'M', 'C'}),
		TEXTURE = std::bit_cast<u32>(std::array<char, 4> {'C', 'L', 'T', 'P'}),
		COUNT = 6,
	};

	struct AnimationTag
	{
		AnimationTagType tag;
		u8 entryCount;
	};
	ASSERT_SIZE(AnimationTag, 0x8);

	enum class AnimationComponent : u8
	{
		TRANSLATE_X = 0,
		TRANSLATE_Y = 1,
		TRANSLATE_Z = 2,
		ROTATE_X = 3,
		ROTATE_Y = 4,
		ROTATE_Z = 5,
		SCALE_X = 6,
		SCALE_Y = 7,
		SIZE_W = 8,
		SIZE_H = 9,

		TRANSLATE_S = 0,
		TRANSLATE_T = 1,
		ROTATE = 2,
		SCALE_S = 3,
		SCALE_T = 4,

		VISIBILITY = 0,

		LT_R = 0,
		LT_G = 1,
		LT_B = 2,
		LT_A = 3,
		RT_R = 4,
		RT_G = 5,
		RT_B = 6,
		RT_A = 7,
		LB_R = 8,
		LB_G = 9,
		LB_B = 0xa,
		LB_A = 0xb,
		RB_R = 0xc,
		RB_G = 0xd,
		RB_B = 0xe,
		RB_A = 0xf,
		PANE_ALPHA = 0x10,

		MATERIAL_COLOR_0_R = 0,
		MATERIAL_COLOR_0_G = 1,
		MATERIAL_COLOR_0_B = 2,
		MATERIAL_COLOR_0_A = 3,
		MATERIAL_COLOR_1_R = 4,
		MATERIAL_COLOR_1_G = 5,
		MATERIAL_COLOR_1_B = 6,
		MATERIAL_COLOR_1_A = 7,
		MATERIAL_COLOR_2_R = 8,
		MATERIAL_COLOR_2_G = 9,
		MATERIAL_COLOR_2_B = 0xa,
		MATERIAL_COLOR_2_A = 0xb,
		MATERIAL_COLOR_3_R = 0xc,
		MATERIAL_COLOR_3_G = 0xd,
		MATERIAL_COLOR_3_B = 0xe,
		MATERIAL_COLOR_3_A = 0xf,
		MATERIAL_COLOR_4_R = 0x10,
		MATERIAL_COLOR_4_G = 0x11,
		MATERIAL_COLOR_4_B = 0x12,
		MATERIAL_COLOR_4_A = 0x13,
		MATERIAL_COLOR_5_R = 0x14,
		MATERIAL_COLOR_5_G = 0x15,
		MATERIAL_COLOR_5_B = 0x16,
		MATERIAL_COLOR_5_A = 0x17,
		MATERIAL_COLOR_6_R = 0x18,
		MATERIAL_COLOR_6_G = 0x19,
		MATERIAL_COLOR_6_B = 0x1a,
		MATERIAL_COLOR_6_A = 0x1b,
		MATERIAL_COLOR_7_R = 0x1c,
		MATERIAL_COLOR_7_G = 0x1d,
		MATERIAL_COLOR_7_B = 0x1e,
		MATERIAL_COLOR_7_A = 0x1f,

		TEXTURE = 0,
	};

	struct AnimationTagEntry
	{
		u8 index;
		AnimationComponent animationTarget;
		u8 curveType;
		u16 keyFrameCount;
		u32 keyFramesOffset;
	};
	ASSERT_SIZE(AnimationTagEntry, 0xc);

	struct StepKeyFrame
	{
		float frame;
		s16 value;
	};
	ASSERT_SIZE(StepKeyFrame, 0x8);

	struct HermiteKeyFrame
	{
		float frame;
		float value;
		float slope;
	};
	ASSERT_SIZE(HermiteKeyFrame, 0xc);
}
