#pragma once

#include <core/libgarden/nw/font/TextWriterResource.hpp>
#include <core/libgarden/nn/math/MTX44.hpp>

namespace nw::lyt
{
	class GraphicsResource
	{

	public:

		auto& GetTextWriterResource() { return m_TextWriterResource; }

		void ResetGlState() {
			using FN_ResetGlState = void (*)(GraphicsResource* self);
			static const FN_ResetGlState resetGlState = reinterpret_cast<FN_ResetGlState>(0x004b5a1c);
			resetGlState(this);
		}
		void ResetGlProgramState() {
			using FN_ResetGlProgramState = void (*)(GraphicsResource* self);
			static const FN_ResetGlProgramState resetGlProgramState = reinterpret_cast<FN_ResetGlProgramState>(0x004b5b84);
			resetGlProgramState(this);
		}

		void SetProjectionMtx(const nn::math::MTX44 &mtx) {
			using FN_SetProjectionMtx = void (*)(GraphicsResource* self, const nn::math::MTX44& mtx);
			static const FN_SetProjectionMtx setProjectionMtx = reinterpret_cast<FN_SetProjectionMtx>(0x004b5b10);
			setProjectionMtx(this, mtx);
		}

		static void FinalizeGraphics() {
			using FN_FinalizeGraphics = void (*)();
			static const FN_FinalizeGraphics finalizeGraphics = reinterpret_cast<FN_FinalizeGraphics>(0x004bee70);
			finalizeGraphics();
		}

	private:
		u8 data[8];
		font::TextWriterResource m_TextWriterResource;
		u8 data2[0x1ec];
	};
	ASSERT_SIZE(GraphicsResource, 0x2e4);
}