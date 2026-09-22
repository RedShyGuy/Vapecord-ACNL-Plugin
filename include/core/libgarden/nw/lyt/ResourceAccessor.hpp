#pragma once

#include <core/libgarden/nn/types.h>

namespace nw::lyt
{

	class ResourceAccessor
	{

	public:
		virtual ~ResourceAccessor();

	private:
	};
	ASSERT_SIZE(ResourceAccessor, 4);
}
