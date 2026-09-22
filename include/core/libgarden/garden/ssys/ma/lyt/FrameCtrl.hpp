#pragma once

#include <core/libgarden/nw/lyt/AnimTransform.hpp>

namespace ssys::ma::lyt
{
	class FrameCtrl
	{
	public:
		virtual ~FrameCtrl();
		virtual void SetFrame(float frame) final
		{
			m_PrevFrame = frame;
			m_Frame = frame;
			m_pAnimTransform->SetFrame(frame);
		}

		void SetFirstFrame() { SetFrame(0.0f); }
		void SetFrameR(float r) { SetFrame(m_FrameCount - r); }
		void SetLastFrame() { SetFrame(m_FrameCount - 1.0f); }

		nw::lyt::AnimTransform* GetAnimTransform() { return m_pAnimTransform; }
		float GetFrame() const { return m_Frame; }
		float GetFrameCount() const { return m_FrameCount; }

		void SetAnimTransform(nw::lyt::AnimTransform* anim);

	protected:
		float m_FrameCount;
		float m_Frame;
		float m_PrevFrame;
		float m_FrameIncrease;
		u8 m_Loop;
		nw::lyt::AnimTransform* m_pAnimTransform;
	};
	ASSERT_SIZE(FrameCtrl, 0x1c);
}