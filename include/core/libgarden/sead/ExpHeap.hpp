#pragma once

#include "core/libgarden/sead/Heap.hpp"
#include "core/libgarden/sead/String.hpp"

namespace sead
{
	class ExpHeap final : public Heap
	{
	public:
		static ExpHeap* CreateRoot(void* addr, size_t size, const SafeString* name, bool unk = false);

		virtual void* Allocate(size_t size, size_t alignment) override;
		virtual void Free(void*) override;

	private:
	};
}
