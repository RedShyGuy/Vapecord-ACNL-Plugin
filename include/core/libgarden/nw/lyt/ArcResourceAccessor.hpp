#pragma once

#include <core/libgarden/nw/lyt/ResourceAccessor.hpp>

#include <core/libgarden/nw/font/Font.hpp>

#include <array>

namespace nw::lyt
{
	struct TextureInfo
	{
		std::array<u8, 0x10> data;
		u8 format;
		INSERT_PAD(3);
	};
	ASSERT_SIZE(TextureInfo, 0x14);

	class ArcResourceAccessor : ResourceAccessor
	{

	public:

		void* RegistFont(const char* name, const font::Font* font);
		TextureInfo GetTexture(const char* name);

		bool Attach(const void* data, const char* rootName);
	
	private:

		u8 m_Data[0x138];
	};
	ASSERT_SIZE(ArcResourceAccessor, 0x13c);
}
