#pragma once

#include <core/libgarden/nn/math/MTX34.hpp>
#include <core/libgarden/GLES2/gl2.h>

namespace nw::font
{

	class TextWriterResource
	{

	public:

		void UseProgram()
		{
			glUseProgram(m_ProgramId);
		}

		void SetViewMtx(const nn::math::MTX34& mtx) {
			using FN_SetViewMtx = void (*)(TextWriterResource* self, const nn::math::MTX34& mtx);
			static const FN_SetViewMtx setViewMtx = reinterpret_cast<FN_SetViewMtx>(Address(0x7ADEC8).addr);
			setViewMtx(this, mtx);
		}

		static void FinalizeGX();

	private:
		u32 m_ProgramId;
		u8 data[0xec];
	};
	ASSERT_SIZE(TextWriterResource, 0xf0);
}