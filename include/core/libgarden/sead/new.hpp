#pragma once

#include <core/libgarden/nn/types.h>

#include <new>

namespace sead
{
	class Heap;
}

void* operator new(size_t size);
void* operator new[](size_t size);

// void* operator new(size_t size, s32 alignment);
// void* operator new[](size_t size, s32 alignment);

void* operator new(size_t size, sead::Heap* heap, size_t alignment = sizeof(void*));
void* operator new[](size_t size, sead::Heap* heap, size_t alignment = sizeof(void*));

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

void operator delete(void* ptr, sead::Heap*, size_t);
void operator delete[](void* ptr, sead::Heap*, size_t);

namespace sead
{
	inline u8* AllocBuffer(size_t size, Heap* heap = nullptr, size_t alignment = sizeof(void*))
	{
		return new (heap, alignment) u8[size];
	}

	inline void FreeBuffer(const u8* ptr)
	{
		delete[] ptr;
	}
}
