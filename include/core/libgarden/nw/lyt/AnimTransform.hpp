#pragma once

#include <core/libgarden/nw/lyt/res/AnimationBlock.hpp>
#include <core/libgarden/nw/ut/LinkList.hpp>

namespace nw::lyt
{
	class AnimTransform
	{
	public:
		virtual ~AnimTransform();

		void SetFrame(float frame) { m_Frame = frame; }

		const res::AnimationBlock* GetResource() const
		{
			return m_pResource;
		}

	private:
		friend class Layout;

		ut::LinkListNode m_Node;
		const res::AnimationBlock* m_pResource;
		float m_Frame;
	};
	ASSERT_SIZE(AnimTransform, 0x14);
}
