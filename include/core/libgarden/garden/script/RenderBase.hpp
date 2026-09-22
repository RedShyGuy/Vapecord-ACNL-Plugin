#pragma once

#include "Tag.hpp"

#include <core/libgarden/nn/math/VEC2.hpp>

#include <core/libgarden/nw/font/TagProcessorBase.hpp>
#include <core/libgarden/nw/ut/Color.hpp>

#include <array>

namespace script
{
	using Color8 = nw::ut::Color8;

	class RenderBase : public nw::font::TagProcessorBase<char16>
	{
	public:
		void ClearBuf2()
		{
			if (m_pBuf2 != nullptr)
				*m_pBuf2 = 0;
		}

		void SetDefaultColors(Color8 top, Color8 bottom) { m_TextColors = {top, bottom}; }
		const auto& GetDefaultColors() const { return m_TextColors; }

	private:
		std::array<Color8, 2> m_TextColors;
		nn::math::VEC2 m_FontSize;
		char16* m_pText;
		size_t m_Len;
		s32 m_Count;
		void* m_pBuf;
		u8* m_pBuf2;
		char16* m_pEnd;
	};
	ASSERT_SIZE(RenderBase, 0x2c);
}