#pragma once

#include <cstddef>

namespace nw::os
{
	class IAllocator
	{
	public:
		virtual ~IAllocator() = 0;
		virtual void* Allocate(size_t size, size_t alignment) = 0;
		virtual void Free(void* p) = 0;
	};
}
