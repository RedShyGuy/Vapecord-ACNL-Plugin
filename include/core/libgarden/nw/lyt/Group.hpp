#pragma once

#include <core/libgarden/nw/lyt/Pane.hpp>

#include <array>
#include <cstring>

namespace nw::lyt
{
	void* Allocate(size_t);

	struct PaneNode
	{
		void* operator new(size_t size) { return Allocate(size); }

		ut::LinkListNode node;
		Pane* pane;
	};
	ASSERT_SIZE(PaneNode, 0xc);

	class Group
	{
	public:
		Group(const void* resource, Pane* root);
		constexpr Group() = default;
		constexpr Group(const char* name)
		{
			strncpy(m_Name.data(), name, m_Name.size());
			m_Name.back() = 0;
		}

		virtual ~Group();

		void AppendPane(Pane* pane)
		{
			m_Panes.push_back(new PaneNode {{}, pane});
		}

	private:
		friend class GroupContainer;

		ut::LinkListNode m_Node;
		ut::LinkList<PaneNode, offsetof(PaneNode, PaneNode::node)> m_Panes;
		std::array<char, 17> m_Name {};
		bool m_UserOwned = false; // if true, won't be destroyed when group container is destroyed
	};
	ASSERT_SIZE(Group, 0x2c);
}
