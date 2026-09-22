#pragma once

#include "Animation.hpp"
#include "ArcResAccReader.hpp"
#include "Base2D.hpp"
#include "DummyLayout.hpp"

#include <core/libgarden/nw/lyt/DrawInfo.hpp>
#include <core/libgarden/nw/lyt/Picture.hpp>
#include <core/libgarden/nw/lyt/TextBox.hpp>
#include <core/libgarden/nw/lyt/Bounding.hpp>
#include <core/libgarden/nw/lyt/Group.hpp>

namespace ssys::ma::lyt
{
	using Pane = nw::lyt::Pane;
	using TextBox = nw::lyt::TextBox;
	using Picture = nw::lyt::Picture;
	using Group = nw::lyt::Group;

	class Layout : public Base2D
	{
	public:
		Layout();
		virtual ~Layout() override;

		void SetArcResAccReader(ArcResAccReader* reader) { m_pArcReader = reader; }
		DummyLayout& GetLayout() { return m_Layout; }
		Pane* GetRootPane() { return m_Layout.GetRootPane(); }

		void Animate();
		bool Build(const char* name, const ArcResAccReader* arcReader = nullptr, u32 cmdCacheSize = 0x100);
		Pane* FindPane(const char* name);
		TextBox* FindTextBox(const char* name);
		Picture* FindPicture(const char* name);
		Group* FindGroup(const char* name);

		void BindAnimation(Animation* anim, const Group* group, bool recursive = false);
		void UnbindAnimation(Animation* anim, const Group* group, bool recursive = false);
		void UnbindAllAnimation();

	private:
		DummyLayout m_Layout;
		nw::lyt::DrawInfo m_DrawInfo;
		nn::math::MTX34 m_ViewMatrix;
		ArcResAccReader* m_pArcReader;
		u8 field58_0xf0;
		u8 field59_0xf1;
		u8 field60_0xf2;
		u8 field61_0xf3;
		u8 field62_0xf4;
		u8 field63_0xf5;
		u8 field64_0xf6;
		u8 field65_0xf7;
		u8 field66_0xf8;
		u8 field67_0xf9;
		u8 field68_0xfa;
		u8 field69_0xfb;
		u8 field70_0xfc;
		u8 field71_0xfd;
		u8 field72_0xfe;
		u8 field73_0xff;
		u32 m_Cmdlist;
		u32 m_CmdCache[6];
		u8 field84_0x11c;
		bool m_AnimateDone;
		u8 field86_0x11e;
		bool field87_0x11f;
		u8 field88_0x120;
		u8 field89_0x121;
		u8 field90_0x122;
		u8 field91_0x123;
		float m_Parallax;
		float field93_0x128;
		u8 field94_0x12c;
		u8 field95_0x12d;
		u8 field96_0x12e;
		u8 field97_0x12f;
		u8 field98_0x130;
		u8 field99_0x131;
		u8 field100_0x132;
		u8 field101_0x133;
		u32 field102_0x134;
		u8 field103_0x138;
		u8 field104_0x139;
		u8 field105_0x13a;
		u8 field106_0x13b;
		float field107_0x13c;
		u8 field108_0x140;
		u8 field109_0x141;
		u8 field110_0x142;
		u8 field111_0x143;
		u8 field112_0x144;
		u8 field113_0x145;
		u8 field114_0x146;
		u8 field115_0x147;
		u8 field116_0x148;
		u8 field117_0x149;
		u8 field118_0x14a;
		u8 field119_0x14b;
	};
	ASSERT_SIZE(Layout, 0x14c);
}