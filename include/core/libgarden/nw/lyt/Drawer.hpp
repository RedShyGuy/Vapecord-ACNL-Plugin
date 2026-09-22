#pragma once

#include <core/libgarden/nw/font/RectDrawer.hpp>

namespace nw::lyt
{
	class Drawer : public font::RectDrawer
	{

	public:

	private:
		u32 data[4];
	};
	ASSERT_SIZE(Drawer, 0x6a4);
}

