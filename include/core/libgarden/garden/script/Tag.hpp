#pragma once

#include <core/libgarden/nn/types.h>

#include <limits>
#include <concepts>

namespace script
{
	struct Tag
	{
		static constexpr char16 TAG_BEGIN = 0xe;

		enum class Group : s32
		{
			NONE = std::numeric_limits<s32>::max(),
			PREDEFINED = 0,
			GENERIC = 0xff,
		};
		
		enum class Predefined : s32
		{
			RUBY = 0,
			FONT = 1,
			SCALE = 2,
			SET_COLOR = 3,
		};

		enum class Generic : s32
		{
			STORE_CURSOR_Y = 0,
			SET_CURSOR_X = 1,
			SET_COLOR_RGBA = 6,
		};

		enum TextColor : char16
		{
			WHITE = 0,
			PINK,
			BLUE,
			GREEN,
			ORANGE,
			GREY,
			RED,
			DEFAULT,
		};

		using Str = const char16_t*;
		static Tag GetTag(const Str* text);

		const char16_t* data = nullptr;
		Group group = Group::NONE;
		s32 index = -1;
		size_t paramCount = 0;
		const char16_t* args = nullptr;
		size_t length = 0;
	};
	ASSERT_SIZE(Tag, 0x18);


	consteval auto MakeTag(Tag::Group group, auto index, std::convertible_to<char16> auto... args)
	{
		return std::array
		{
			Tag::TAG_BEGIN,
			(char16)group,
			(char16)index,
			(char16)(sizeof...(args) * sizeof(char16)),
			(char16)args...,
		};
	}

	consteval auto MakeTextColorTag(Tag::TextColor color)
	{
		return MakeTag(Tag::Group::PREDEFINED, Tag::Predefined::SET_COLOR, color);
	}

	consteval auto GetRestoreColorTag() { return MakeTextColorTag(Tag::TextColor::DEFAULT); }
	consteval auto GetGreenColorTag() { return MakeTextColorTag(Tag::TextColor::GREEN); }
	consteval auto GetRedColorTag() { return MakeTextColorTag(Tag::TextColor::RED); }
}
