#pragma once

#include "core/libgarden/nw/lyt/ArcResourceAccessor.hpp"

namespace ssys::ma::lyt
{

	class ArcResourceAccessorVRAM final : public nw::lyt::ArcResourceAccessor
	{

	};
	ASSERT_SIZE(ArcResourceAccessorVRAM, 0x13c);
}