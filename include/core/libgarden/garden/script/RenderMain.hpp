#pragma once

#include "RenderBase.hpp"

namespace script
{

	class RenderMain : public RenderBase
	{
	public:

		RenderMain(size_t textBufCapacity = 0, size_t storeBufCapacity = 0, size_t tagBufCount = 0) {
			using FN_Ctor = void (*)(RenderMain* self, size_t textBufCapacity, size_t storeBufCapacity, size_t tagBufCount);
			static const FN_Ctor ctor = reinterpret_cast<FN_Ctor>(0x005d7338);
			ctor(this, textBufCapacity, storeBufCapacity, tagBufCount);
		}
		virtual ~RenderMain() override {
			using FN_Destructor = void (*)(RenderMain* self);
			static const FN_Destructor destructor = reinterpret_cast<FN_Destructor>(0x005d7434);
			destructor(this);
		}

	private:
		s32 m_Offset;
		void * m_pUnk0;
		size_t m_Size;
		s32 m_Unk1;
		nn::math::VEC2 m_Unk2;
	};
	ASSERT_SIZE(RenderMain, 0x44);
}
