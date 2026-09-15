#pragma once

#include "core/infrastructure/Address.hpp"
#include "Layout.hpp"
#include "../../st/List.hpp"

#include <core/libgarden/nw/lyt/GraphicsResource.hpp>
#include <core/libgarden/nw/lyt/Drawer.hpp>

namespace ssys::ma::lyt
{
	class LayoutList : public st::List
	{

	};
	ASSERT_SIZE(LayoutList, 0xc);

	class LayoutMgr
	{
	public:
		static constexpr u32 topScreenWidth = 400;
		static constexpr u32 bottomScreenWidth = 320;
		static constexpr u32 topScreenHeight = 240;
		static constexpr u32 bottomScreenHeight = 240;

		//static LayoutMgr* Get() { return s_pInstance; }
		static LayoutMgr* Get() {
			return *reinterpret_cast<LayoutMgr**>(Address(0x976C40).addr);
		}

		auto& GetResource() { return m_Resource; }
		auto& GetDrawer() { return m_Drawer; }

		void DrawBegin(u32 width, u32 height) {
			using FN_DrawBegin = void (*)(LayoutMgr* self, u32 width, u32 height);
			auto drawBegin = reinterpret_cast<FN_DrawBegin>(Address(0x56A9A4).addr);
        	drawBegin(this, width, height);
		}

		void DrawBegin(bool isBottomScreen) {
			DrawBegin(isBottomScreen ? bottomScreenWidth : topScreenWidth, topScreenHeight);
		}

		void Register(Base2D* b2d, bool bottomScreen) {
			using FN_Register = void (*)(LayoutMgr* self, Base2D* b2d, bool bottomScreen);
			auto _register = reinterpret_cast<FN_Register>(Address(0x56A954).addr);
        	_register(this, b2d, bottomScreen);
		}

	private:
		virtual ~LayoutMgr() {
			using FN_Destructor = void (*)(LayoutMgr* self);
			auto destructor = reinterpret_cast<FN_Destructor>(Address(0x56AC34).addr);
			destructor(this);
		}

		static LayoutMgr* s_pInstance;

		LayoutList m_DrawListA;
		LayoutList m_DrawListB;
		nw::lyt::GraphicsResource m_Resource;
		u8 data[0x64];
		nw::lyt::Drawer m_Drawer;
	};
	ASSERT_SIZE(LayoutMgr, 0xa08);
}
