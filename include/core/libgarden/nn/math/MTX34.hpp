#pragma once

#include <core/libgarden/nn/types.h>

namespace nn::math
{
	struct MTX34
	{
		float data[3][4];

		static consteval auto GetIdentity()
		{
			return MTX34
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f
			};
		}
	};
	ASSERT_SIZE(MTX34, 0x30);
}
