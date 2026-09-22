#pragma once

#include <core/libgarden/garden/script/Tag.hpp>

#include <string_view>

namespace gardenex::script
{
	using namespace std::literals;

	namespace tag
	{
		inline constexpr std::u16string_view white = u"\x0e\0\3\2\0"sv;
		inline constexpr std::u16string_view pink = u"\x0e\0\3\2\1"sv;
		inline constexpr std::u16string_view blue = u"\x0e\0\3\2\2"sv;
		inline constexpr std::u16string_view green = u"\x0e\0\3\2\3"sv;
		inline constexpr std::u16string_view orange = u"\x0e\0\3\2\4"sv;
		inline constexpr std::u16string_view grey = u"\x0e\0\3\2\5"sv;
		inline constexpr std::u16string_view red = u"\x0e\0\3\2\6"sv;
		inline constexpr std::u16string_view restoreColor = u"\x0e\0\3\2\7"sv;
	}
};
