#pragma once

#include "FrameCtrl.hpp"
#include "ResAccInterface.hpp"

namespace ssys::ma::lyt
{
	class Animation : public FrameCtrl
	{
	public:
		Animation();

		virtual ~Animation() override;

		Animation(const Animation&) = delete;
		Animation& operator=(const Animation&) = delete;

		bool Initialize(const char* name, const ResAccInterface* resAcc);
		bool IsDone() const;
		void IncreaseFrame();
		bool IsBound() const { return m_IsBound == 1; }
		bool IsUnbound() const { return m_IsBound == 0; }

		bool Finish()
		{
			if (IsDone()) return true;
			IncreaseFrame();
			return false;
		}

	protected:
		struct EntryInfo
		{
			u32 unk1;
			u32 unk2;
			u32 unk3;
			u16 unk4;
			u8 unk5;
		};
		ASSERT_SIZE(EntryInfo, 0x10);

		u8 m_IsBound;
		u16 m_EntryCount;
		EntryInfo* m_pEntries;
		bool m_IsDescendingBind;
	};
	ASSERT_SIZE(Animation, 0x28);
}