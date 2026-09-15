#pragma once

#include "core/infrastructure/Address.hpp"
#include "core/libgarden/nw/font/Font.hpp"
#include "core/libgarden/sead/String.hpp"

namespace font
{

	enum FontID
	{
		GARDEN_MSG_16,
		GARDEN_RUBY,
		GARDEN_NO,
		GARDEN_NO_F16,
		UNK
	};

	struct FontInfo
	{
		const char* name;
		const nw::font::Font* font;
	};

	class Mgr
	{

	public:

		static Mgr* Get() {
			return *reinterpret_cast<Mgr**>(Address(0x9539CC).addr);
		}

		static FontInfo GetFontInfo(FontID font = FontID::GARDEN_MSG_16)
		{
			auto* const mgr = Get();
			return { mgr->GetFontName(font)->GetString(), mgr->GetFont(font) };
		}

		const sead::FixedSafeString<32>* GetFontName(FontID font) const {
			using FN_GetFontName = const sead::FixedSafeString<32>* (*)(const Mgr* self, FontID font);
			static const FN_GetFontName getFontName = reinterpret_cast<FN_GetFontName>(Address(0x748d88).addr);
			return getFontName(this, font);
		}

		const nw::font::Font* GetFont(FontID font) const {
			using FN_GetFont = const nw::font::Font* (*)(const Mgr* self, FontID font);
			static const FN_GetFont getFont = reinterpret_cast<FN_GetFont>(Address(0x52ED74).addr);
			return getFont(this, font);
		}


	private:

		static Mgr* s_pInstance;
	};

}
