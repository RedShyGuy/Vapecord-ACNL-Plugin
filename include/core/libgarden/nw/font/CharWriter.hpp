#pragma once

#include <core/libgarden/nw/font/TextWriterResource.hpp>
#include <core/libgarden/nw/font/Font.hpp>
#include <core/libgarden/nw/ut/Color.hpp>
#include <core/libgarden/nw/font/DispStringBuffer.hpp>

#include <core/libgarden/nn/math/VEC2.hpp>
#include <core/libgarden/nn/math/VEC3.hpp>

namespace nw::font
{

	class CharWriter
	{
	public:

		enum class TextGradient : u8
		{
			NONE,
			HORIZONTAL,
			VERTICAL,
		};

		CharWriter() = default;

		CharWriter(const CharWriter&) = delete;

		void SetFont(const Font* font) { m_pFont = font; }
		void SetDispStringBuffer(DispStringBuffer* buf) { m_pDispBuffer = buf; }
 		void SetFontSize(float width, float height) {
			using FN_SetFontSize = void (*)(CharWriter* self, float width, float height);
			static const FN_SetFontSize setFontSize = reinterpret_cast<FN_SetFontSize>(Address(0x4D64D0).addr);
			setFontSize(this, width, height);
		}

		void SetTextColors(ut::Color8 top, ut::Color8 bottom)
		{
			m_Gradient = top == bottom ? TextGradient::NONE : TextGradient::VERTICAL;
			m_TextColors[0] = top;
			m_TextColors[1] = bottom;
			UpdateVertexColors();
		}

		ut::Color8 GetTextColorTop() const { return m_TextColors[0]; }
		ut::Color8 GetTextColorBottom() const { return m_TextColors[1]; }

		float GetCursorX() const { return m_Cursor.x; }
		float GetCursorY() const { return m_Cursor.y; }

		void SetCursorX(float value) { m_Cursor.x = value; }
		void SetCursorY(float value) { m_Cursor.y = value; }

		void StartPrint() {
			using FN_StartPrint = void (*)(CharWriter* self);
			static const FN_StartPrint startPrint = reinterpret_cast<FN_StartPrint>(Address(0x4D64B4).addr);
			startPrint(this);
		}
		u32* UseCommandBuffer(u32* cmdbuf, class RectDrawer* drawer) {
			using FN_UseCommandBuffer = u32* (*)(CharWriter* self, u32* cmdbuf, void* drawer);
			static const FN_UseCommandBuffer useCmdBuf = reinterpret_cast<FN_UseCommandBuffer>(Address(0x4D652C).addr);
			return useCmdBuf(this, cmdbuf, drawer);
		}

		static constexpr u32 GetDispStringBufferSize(u32 charCount)
		{
			u32 cap = DispStringBuffer::CalcCommandBufferCapacity(charCount);
			return ((((((charCount + 7) >> 3) + 3) & 0xfffffffc) + charCount * 0x2c + 0x37) & 0xfffffff0) + cap * 4;
		}

		static DispStringBuffer* InitDispStringBuffer(void *mem, u32 charCount) {
			using FN_InitDispStringBuffer = void* (*)(void* mem, u32 charCount);
			static const FN_InitDispStringBuffer initDispBuf = reinterpret_cast<FN_InitDispStringBuffer>(Address(0x4D6790).addr);
			return reinterpret_cast<DispStringBuffer*>(initDispBuf(mem, charCount));
		}

	private:
		void UpdateVertexColors() {
			using FN_UpdateVertexColors = void (*)(CharWriter* self);
			static const FN_UpdateVertexColors updateVertexColors = reinterpret_cast<FN_UpdateVertexColors>(Address(0x4D6738).addr);
			updateVertexColors(this);
		}

		ut::Color8 m_Color0;
		ut::Color8 m_Color1;
		ut::Color8 m_VertexColors[4];
		ut::Color8 m_TextColors[2];
		TextGradient m_Gradient;
		nn::math::VEC2 m_Scale;
		nn::math::VEC3 m_Cursor;
		f32 m_FixedWidth;
		const Font* m_pFont;
		TextWriterResource* m_pResource;
		DispStringBuffer* m_pDispBuffer;
		bool m_UseFixedWidth;
		u8 m_Alpha;
	};
	ASSERT_SIZE(CharWriter, 0x4c);
}