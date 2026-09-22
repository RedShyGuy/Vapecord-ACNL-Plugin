#pragma once

#include <core/libgarden/nw/lyt/ArcResourceAccessor.hpp>

#include <core/libgarden/nn/math/VEC2.hpp>

namespace nw::lyt
{
	class TexMap
	{
	public:
		enum WrapMode : u8
		{
			CLAMP,
			REPEAT,
			MIRROR,
		};

		enum Filter : u8
		{
			NEAR,
			LINEAR,
		};

		TexMap();
		TexMap(const TextureInfo& info);

		void Set(const TextureInfo& info)
		{
			m_Data = info.data;
			// flags = ((info.format & 0xf) << 8) | (flags & 0xfffff0ff);
			m_Format = info.format & 0xf;
			ResetU32Info();
		}

		void SetWrapAndFilter(WrapMode wrapS, WrapMode wrapT, Filter minFilter, Filter magFilter)
		{
			m_WrapS = wrapS;
			m_WrapT = wrapT;
			m_MinFilter = minFilter;
			m_MagFilter = magFilter;
			ResetU32Info();
		}

	public:
		void ResetU32Info();

		std::array<u8, 0x10> m_Data;
		WrapMode m_WrapS : 2;
		WrapMode m_WrapT : 2;
		Filter m_MinFilter : 2;
		u8 unknown : 1;
		Filter m_MagFilter : 1;
		u8 m_Format : 4;
		u32 flags2;
		u32 flags3;
		u32 flags4;
	};
	ASSERT_SIZE(TexMap, 0x20);

	struct TexMatrix
	{
		nn::math::VEC2 translation;
		float rotation;
		nn::math::VEC2 scale;
	};
	ASSERT_SIZE(TexMatrix, 0x14);

	struct TexCoordGen
	{
		u8 type;
		u8 source;
		u8 padding[2];
	};
	ASSERT_SIZE(TexCoordGen, 4);
}
