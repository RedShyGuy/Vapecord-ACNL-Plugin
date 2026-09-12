#pragma once

#include <core/libgarden/nn/math/MTX34.hpp>
#include <core/libgarden/nn/math/MTX44.hpp>
#include <core/libgarden/nw/font/CharWriter.hpp>

namespace nw::font
{

	class RectDrawer
	{

	public:

		virtual ~RectDrawer();
		virtual void Finalize();
		virtual u32* DrawBegin(u32 options = 0);
		virtual u32* DrawBegin(u32* cmdbuf, u32 options = 0);
		virtual u32* DrawEnd(u32 options = 0);
		virtual u32* DrawEnd(u32* cmdbuf, u32 options = 0);

		u32* SetProjectionMtx(u32* cmdbuf, const nn::math::MTX44& proj) {
			using FN_SetProjectionMtx = u32* (*)(RectDrawer* self, u32* cmdbuf, const nn::math::MTX44& proj);
			static const FN_SetProjectionMtx setProjectionMtx = reinterpret_cast<FN_SetProjectionMtx>(0x004d7fb0);
			return setProjectionMtx(this, cmdbuf, proj);
		}

		u32* SetViewMtx(u32* cmdbuf, const nn::math::MTX34& view)
		{
			m_ViewMtxComponentCount = 3;
			*reinterpret_cast<nn::math::MTX34*>(&m_ViewMtxCmdBuffer[3]) = view;
			return AddUniformMtx(cmdbuf);
		}

		u32* AddUniformMtx(u32* cmdbuf) {
			using FN_AddUniformMtx = u32* (*)(RectDrawer* self, u32* cmdbuf);
			static const FN_AddUniformMtx addUniformMtx = reinterpret_cast<FN_AddUniformMtx>(0x004d6fd0);
			return addUniformMtx(this, cmdbuf);
		}

		void BuildTextCommand(CharWriter* writer) {
			using FN_BuildTextCommand = void (*)(RectDrawer* self, CharWriter* writer);
			static const FN_BuildTextCommand buildTextCommand = reinterpret_cast<FN_BuildTextCommand>(0x004d7890);
			buildTextCommand(this, writer);
		}

	private:

		u8 m_Unk0[0x10];
		u8 m_Unk1[0x10];
		u8 m_ComponentCount1;
		u8 m_ViewMtxComponentCount;
		u8 m_ComponentCount3;
		u8 m_ComponentCount4;
		u32 m_ProjectionMtxCmdBuffer[24];
		u32 m_Unk3[108];
		u32 m_ViewMtxCmdBuffer[132];
		u32 m_Unk4[132];
		u32 m_Unk5[15];
	};
	ASSERT_SIZE(RectDrawer, 0x694);
}
