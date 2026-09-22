#pragma once

#include <core/libgarden/nw/ut/LinkList.hpp>


namespace nw::lyt
{

	class PaneBase
	{
	public:
		constexpr PaneBase() = default;
		virtual ~PaneBase();

		constexpr PaneBase(const PaneBase&) = default;
		constexpr PaneBase(PaneBase&&) = default;
		PaneBase& operator=(const PaneBase&);
		PaneBase& operator=(PaneBase&&);

		ut::LinkListNode m_Node;
	};
	ASSERT_SIZE(PaneBase, 0xc);
}
