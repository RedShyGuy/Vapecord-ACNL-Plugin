#pragma once

#include "ResAccInterface.hpp"
#include "ArcResourceAccessorVRAM.hpp"

namespace ssys::ma::lyt
{
	class ArcResAcc : public ResAccInterface
	{
	public:
		virtual ~ArcResAcc();

		bool SetData(const void* arcData, const char* rootName);

	protected:
		ArcResourceAccessorVRAM* GetResourceAccessorVRAM() { return &m_ArcResourceAccessor; }

	private:
		ArcResourceAccessorVRAM m_ArcResourceAccessor;
	};
	ASSERT_SIZE(ArcResAcc, 0x148);
}