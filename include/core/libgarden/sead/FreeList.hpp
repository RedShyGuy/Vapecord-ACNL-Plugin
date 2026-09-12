#pragma once

#include <core/libgarden/nn/types.h>
#include <new>

namespace sead
{
class FreeList
{
public:
    void setWork(void* work, s32 elem_size, s32 num);
    void reset();

    void* alloc();
    void free(void* ptr);

    void* getFree() const { return mFree; }
    void* work() const { return mWork; }

    static const size_t cPtrSize = sizeof(void*);

private:
    struct Node
    {
        Node* nextFree;
    };

    Node* mFree = nullptr;
    void* mWork = nullptr;
};

inline void FreeList::reset()
{
    mFree = nullptr;
    mWork = nullptr;
}

inline void* FreeList::alloc()
{
    if (!mFree)
        return nullptr;

    void* ptr = mFree;
    mFree = mFree->nextFree;
    return ptr;
}

inline void FreeList::free(void* ptr)
{
    mFree = new (ptr) Node{mFree};
}
}  // namespace sead
