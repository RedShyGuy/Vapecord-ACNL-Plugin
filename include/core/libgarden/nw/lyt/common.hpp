#pragma once

#include <core/libgarden/nn/types.h>

namespace nw::lyt
{

	void* Alloc(size_t size, size_t alignment = 4);
	void* DeviceAlloc(size_t size, size_t alignment = 4);

}