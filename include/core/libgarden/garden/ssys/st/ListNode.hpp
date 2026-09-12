#pragma once

#include <core/libgarden/nn/types.h>

namespace ssys::st
{
	class ListNode
	{
	public:
		constexpr ListNode() = default;
		virtual ~ListNode();

	private:
		ListNode* m_pPrev { };
		ListNode* m_pNext { };
	};
	ASSERT_SIZE(ListNode, 0xc);
}