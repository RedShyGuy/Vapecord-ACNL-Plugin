#pragma once

#include <core/libgarden/nw/ut/LinkList.hpp>

namespace nw::lyt
{
	struct AnimationLink
	{
		ut::LinkListNode node;
		class AnimTransform* anim;
		u16 index;
		bool unknown;
	};
	ASSERT_SIZE(AnimationLink, 0x10);
}
