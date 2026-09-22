#pragma once

#include <core/libgarden/nn/types.h>

namespace nw::ut
{
	struct LinkListNode
	{
		LinkListNode* next {};
		LinkListNode* prev {};
	};

	namespace internal
	{
		class LinkListImpl
		{
		public:
			struct iterator
			{
				LinkListNode* node;
			};

			size_t size() const { return m_Count; }

			LinkListNode* erase(LinkListNode*);
			LinkListNode* insert(iterator, LinkListNode*);

			iterator begin() { return {m_Node.next}; }
			iterator end() { return {&m_Node}; }

		private:
			size_t m_Count {};
			LinkListNode m_Node {&m_Node, &m_Node};
		};
	}

	template<typename T, long Offset>
	class LinkList : public internal::LinkListImpl
	{
	public:
		void push_back(T* value) { internal::LinkListImpl::insert(end(), ToNode(value)); }
		void erase(T* value) { internal::LinkListImpl::erase(ToNode(value)); }

	private:
		static LinkListNode* ToNode(T* obj)
		{
			return (LinkListNode*)((u8*)obj + Offset);
		}
	};

}
