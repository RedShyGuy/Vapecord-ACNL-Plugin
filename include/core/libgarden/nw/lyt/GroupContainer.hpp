#pragma once

#include <core/libgarden/nw/lyt/Group.hpp>

namespace nw::lyt
{
	class GroupContainer
	{
	public:
		void AppendGroup(Group* group);
		Group* FindGroupByName(const char* name);

	private:
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Winvalid-offsetof"
		nw::ut::LinkList<Group, offsetof(Group, Group::m_Node)> m_Groups;
		#pragma GCC diagnostic pop
	};
	ASSERT_SIZE(GroupContainer, 0xc);
}