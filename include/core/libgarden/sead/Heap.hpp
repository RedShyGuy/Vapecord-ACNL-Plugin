#pragma once

#include <core/libgarden/nn/types.h>

namespace sead
{
	class Heap
	{
	public:
		virtual ~Heap();
		virtual void SetupTypeInfo();
		virtual u32 GetTypeInfo();
		virtual void Destroy();
		virtual void Adjust();
		virtual void* Allocate(size_t size, size_t alignment = 4);
		virtual void Free(void*);
		virtual void ResizeFront();
		virtual void ResizeBack();
		virtual void* TryRealloc(void*);
		virtual void FreeAll();
		virtual void* GetStartAddress();
		virtual void* GetEndAddress();
		virtual size_t GetSize();
		virtual size_t GetFreeSize();
		virtual size_t GetMaxAllocatableSize();
		virtual bool IsInclude();
		virtual bool IsFreeable();
		virtual bool IsResizable();
		virtual bool IsAdjustable();


	private:
		u8 m_Data[0x70];
	};
	ASSERT_SIZE(Heap, 0x74);
}
