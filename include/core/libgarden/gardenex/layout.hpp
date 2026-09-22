#pragma once

#include "core/infrastructure/Address.hpp"
#include <core/libgarden/garden/font/Mgr.hpp>
#include <core/libgarden/garden/ssys/ma/lyt/LayoutMgr.hpp>
#include <core/libgarden/garden/script/RenderMain.hpp>

#include <core/libgarden/nw/lyt/TextBox.hpp>
#include <core/libgarden/nw/font/TextWriterBase.hpp>

#include <core/libgarden/nn/gx/gx.h>

namespace gardenex
{
	inline uint32_t*& GetCmdBuffer()
	{
		return *reinterpret_cast<uint32_t**>(Address(0x975A9C).addr);
	}

	namespace detail
	{
		inline constinit nw::lyt::Material dummyMaterial {true};
		inline constinit nw::lyt::DrawInfo dummyDrawInfo {};
	}

	inline nw::lyt::TextBox CreateTextBox(Vector2 size, u16 maxCharCount, const char16* text, u16 length)
	{
		return {size, font::Mgr::Get()->GetFont(font::GARDEN_MSG_16), maxCharCount, text, length, &detail::dummyMaterial};
	}

	inline nw::lyt::DrawInfo& GetDummyDrawInfo()
	{
		detail::dummyDrawInfo.SetGraphicsResource(&ssys::ma::lyt::LayoutMgr::Get()->GetResource());
		return detail::dummyDrawInfo;
	}

	inline script::RenderMain& GetDummyTagProcessor()
	{
		// tagBufCount=4 needed so inline color tags (e.g. \x0e\0\3\2\3) are processed
		static script::RenderMain dummyTagProcessor {0, 0, 4};
		return dummyTagProcessor;
	}

	inline nn::math::MTX44& GetProjectionMatrix(bool bottomScreen, bool forTextWriter)
	{
		static constexpr float h = 240.0f;
		static constexpr float v01[] {-2.0f / h, 2.0f / h};
		static constexpr float v10[] {-2.0f / 320.0f, -2.0f / 400.0f};
		static constinit auto matrix = []
		{
			constexpr float n = 500.0f;
			constexpr float f = -1500.0f;
			nn::math::MTX44 m {};
			auto& v = m.data;
			v[0][1] = v01[0];
			v[1][0] = v10[0];
			v[0][3] = 1.0f;
			v[1][3] = 1.0f;
			v[2][2] = -2.0f / (f - n);
			v[2][3] = -(f + n) / (f - n);
			v[3][3] = 1.0f;
			return m;
		}();
		matrix.data[0][1] = forTextWriter ? v01[0] : v01[1];
		matrix.data[1][0] = bottomScreen ? v10[0] : v10[1];
		matrix.data[0][3] = forTextWriter ? 1.0f : 0.0f;
		matrix.data[1][3] = forTextWriter ? 1.0f : 0.0f;
		return matrix;
	};

	template<u32 Capacity>
	struct TextDisplayMem
	{
		TextDisplayMem()
		{
			nw::font::CharWriter::InitDispStringBuffer(m_Buffer, Capacity);
		}

		nw::font::DispStringBuffer* GetBuffer()
		{
			return reinterpret_cast<nw::font::DispStringBuffer*>(&m_Buffer[0]);
		}

		alignas(u32) u8 m_Buffer[nw::font::CharWriter::GetDispStringBufferSize(Capacity)];
	};

	struct PrintTextScope
	{
		PrintTextScope(nw::font::WideTextWriter& writer, float width, float height,
			font::FontID font = font::FontID::GARDEN_MSG_16) : writer {writer}
		{
			writer.SetFont(font::Mgr::Get()->GetFont(font));
			writer.SetFontSize(width, height);
			auto& tagProc = GetDummyTagProcessor();
			tagProc.SetDefaultColors(writer.GetTextColorTop(), writer.GetTextColorBottom());
			writer.SetTagProcessor(&tagProc);
			writer.StartPrint();
		}

		~PrintTextScope()
		{
			ssys::ma::lyt::LayoutMgr::Get()->GetDrawer().BuildTextCommand(&writer);
		}

		PrintTextScope(const PrintTextScope&) = delete;
		PrintTextScope(PrintTextScope&&) = delete;
		PrintTextScope& operator=(const PrintTextScope&) = delete;
		PrintTextScope& operator=(PrintTextScope&&) = delete;

		nw::font::WideTextWriter& writer;
	};

	inline constinit nn::math::MTX34 textViewMtx
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f
	};

	inline size_t DrawText(const nn::math::MTX34& viewMtx, nw::font::WideTextWriter& writer, bool bottomScreen = false)
	{
		auto& drawer = ssys::ma::lyt::LayoutMgr::Get()->GetDrawer();
		u32* cmdbuf = drawer.DrawBegin();
		u32* begin = cmdbuf;
		cmdbuf = drawer.SetProjectionMtx(cmdbuf, gardenex::GetProjectionMatrix(bottomScreen, true));
		cmdbuf = drawer.SetViewMtx(cmdbuf, viewMtx);
		cmdbuf = writer.UseCommandBuffer(cmdbuf, &drawer);
		GetCmdBuffer() = drawer.DrawEnd(cmdbuf);
		return GetCmdBuffer() - begin;
	}

	inline size_t DrawTextWithShadow(const nn::math::MTX34& viewMtx, nw::font::WideTextWriter& writer, bool bottomScreen = false)
	{
		auto& drawer = ssys::ma::lyt::LayoutMgr::Get()->GetDrawer();
		u32* cmdbuf = drawer.DrawBegin();
		u32* begin = cmdbuf;
		cmdbuf = drawer.SetProjectionMtx(cmdbuf, gardenex::GetProjectionMatrix(bottomScreen, true));
		u32* save = cmdbuf;
		cmdbuf = drawer.SetViewMtx(cmdbuf, viewMtx);
		cmdbuf = writer.UseCommandBuffer(cmdbuf, &drawer);
		u32 posX = save[6];
		u32 posY = save[10];
		u32 colorMap = save[36];
		*(float*)&save[6] += 1.0f;
		*(float*)&save[10] += 1.0f;
		save[36] = 0xff000000;
		size_t size = cmdbuf - save;
		memcpy(cmdbuf, save, size * sizeof(u32));
		cmdbuf[6] = posX;
		cmdbuf[10] = posY;
		cmdbuf[36] = colorMap;
		cmdbuf += size;
		GetCmdBuffer() = drawer.DrawEnd(cmdbuf);
		return GetCmdBuffer() - begin;
	}

	inline size_t DrawText(nn::math::VEC2 position, nw::font::WideTextWriter& writer, bool bottomScreen = false)
	{
		textViewMtx.data[0][3] = position.x;
		textViewMtx.data[1][3] = position.y;
		return DrawTextWithShadow(textViewMtx, writer, bottomScreen);
	}
}
