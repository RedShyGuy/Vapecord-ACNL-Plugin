#pragma once

#include <core/libgarden/nw/font/CharWriter.hpp>
#include <core/libgarden/nw/font/TagProcessorBase.hpp>

namespace nw::font
{
	template<typename T>
	class TextWriterBase : public CharWriter
	{
	public:
		enum Alignment : u32
		{
			LEFT = 0,
			CENTER = 1,
			RIGHT = 2,
			TOP = 0,
			BOTTOM = 2,
		};

		TextWriterBase() {
			using FN_Ctor = void (*)(TextWriterBase* self);
			static const FN_Ctor ctor = reinterpret_cast<FN_Ctor>(0x007e9ce8);
			ctor(this);
		}

		float CalcStringWidth(const T* str, s32 len) const {
			//TODO: UNKNOWN
			using FN_CalcStringWidth = float (*)(const TextWriterBase* self, const T* str, s32 len);
			static const FN_CalcStringWidth calcStringWidth = reinterpret_cast<FN_CalcStringWidth>(0);
			return calcStringWidth(this, str, len);
		}

		float Print(const T* str, s32 len) const {
			using FN_Print = float (*)(const TextWriterBase* self, const T* str, s32 len);
			static const FN_Print print = reinterpret_cast<FN_Print>(0x007e97a8);
			return print(this, str, len);
		}

		void SetCharSpace(float value) { m_CharSpace = value; }
		void SetLineSpace(float value) { m_LineSpace = value; }
		void SetWidthLimit(float value) { m_WidthLimit = value; }
		void SetTagProcessor(TagProcessorBase<char16>* value) { m_pTagProcessor = value; }

		void SetTextAlignment(Alignment originX, Alignment originY, Alignment text)
		{
			m_TextAlignment = text;
			m_OriginX = originX;
			m_OriginY = originY;
		}

	private:
		float m_WidthLimit;
		float m_CharSpace;
		float m_LineSpace;
		s32 m_TabWidth;
		Alignment m_TextAlignment : 4;
		Alignment m_OriginX : 4;
		Alignment m_OriginY : 4;
		TagProcessorBase<char16>* m_pTagProcessor;
	};

	using TextWriter = TextWriterBase<char>;
	using WideTextWriter = TextWriterBase<char16_t>;

	ASSERT_SIZE(TextWriter, 0x64);
	ASSERT_SIZE(WideTextWriter, 0x64);
}