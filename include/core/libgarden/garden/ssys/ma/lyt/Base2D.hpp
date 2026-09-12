#pragma once

#include "../../st/ListNode.hpp"

namespace ssys::ma::lyt
{

	class Base2D : st::ListNode
	{

	public:

		virtual void DrawCached();
		virtual void Animate();
		virtual void BuildImpl();
		virtual void DrawSave();
		virtual void UpdateProj();

		void SetLayer(u8 layer) { m_Layer = layer; }

	private:

		u8 m_Layer;
		INSERT_PAD(3);
	};
	ASSERT_SIZE(Base2D, 0x10);
}