#pragma once

#include <core/libgarden/nn/types.h>

namespace nn::math
{
	struct MTX44
	{
		float data[4][4];
	};
	ASSERT_SIZE(MTX44, 0x40);
}
