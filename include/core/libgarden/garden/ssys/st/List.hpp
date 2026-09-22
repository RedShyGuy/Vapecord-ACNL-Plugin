#pragma once

#include "ListNode.hpp"

namespace ssys::st
{
	class List
	{
	public:
		virtual constexpr ~List() { }

	private:
		ListNode* m_pFirst;
		ListNode* m_pLast;
	};
	ASSERT_SIZE(List, 0xc);
}