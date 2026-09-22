#pragma once

#include <core/libgarden/nw/lyt/GraphicsResource.hpp>
#include <core/libgarden/nn/math/MTX34.hpp>
#include <core/libgarden/nn/math/VEC2.hpp>

namespace nw::lyt
{

	class DrawInfo
	{

	public:

		virtual constexpr ~DrawInfo() = default;

		void SetGraphicsResource(GraphicsResource* resource)
		{
			m_pGraphicsResource = resource;
		}

	private:
		nn::math::MTX44 m_ProjMtx {};
		nn::math::MTX34 m_ViewMtx = nn::math::MTX34::GetIdentity();
		nn::math::VEC2 m_Unk {1.0f, 1.0f};
		float m_Alpha {1.0f};
		GraphicsResource* m_pGraphicsResource {};
		class Layout* m_pLayout {};
		u8 m_Flags {};
	};
	ASSERT_SIZE(DrawInfo, 0x8c);
}
