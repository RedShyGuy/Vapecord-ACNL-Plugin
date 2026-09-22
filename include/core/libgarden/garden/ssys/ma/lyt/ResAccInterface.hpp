#pragma once

#include <core/libgarden/nw/lyt/ResourceAccessor.hpp>

namespace ssys::ma::lyt
{
	class ResAccInterface
	{
	public:
		virtual ~ResAccInterface();

		nw::lyt::ResourceAccessor* GetResourceAccessor()
		{
			return m_pResourceAccessor;
		}

	protected:
		// void SetData(const void* data)
		// {
		// 	ASSERT_OFFSET(ResAccInterface, m_pData, 8);

		// 	m_pData = data;
		// }

	private:
		nw::lyt::ResourceAccessor* m_pResourceAccessor;
		const void* m_pData;
	};
	ASSERT_SIZE(ResAccInterface, 0xc);
}