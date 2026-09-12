#pragma once

#include <core/libgarden/nw/lyt/res/Animation.hpp>

namespace nw::lyt
{
	struct AnimResource
	{
		void Set(const void* resource);

		const void* resource {};
		const res::AnimationBlock* pai1 {};
		const res::Pat1* pat1 {};
		const void* pah1 {};
	};
}