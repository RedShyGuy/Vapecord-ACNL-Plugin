#pragma once
// Custom allocator for stl container
// Based from http://www.josuttis.com/libbook/memory/myalloc.hpp.html

#include <limits>
#include "CTRPluginFrameworkImpl/System/Heap.hpp"

template <class T>
class IAllocator
{
public:
    // type definitions
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;

    // rebind allocator to type U
    template <class U>
    struct rebind
    {
        typedef IAllocator<U> other;
    };

    // return address of values
    pointer address(reference value) const
    {
        return &value;
    }
    const_pointer address(const_reference value) const
    {
        return &value;
    }

    /* constructors and destructor
    * - nothing to do because the allocator has no state
    */
    IAllocator()
    {
    }
    IAllocator(const IAllocator&)
    {
    }
    template <class U>
    IAllocator(const IAllocator<U>&)
    {
    }
    ~IAllocator()
    {
    }

    // return maximum number of elements that can be allocated
    size_type max_size() const
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    // allocate but don't initialize num elements of type T
    pointer allocate(size_type num, const void* = 0)
    {
        pointer ret = static_cast<pointer>(CTRPluginFramework::Heap::Alloc(num * sizeof(T)));
        // print message and allocate memory with global new
        return ret;
    }

    // initialize elements of allocated storage p with value value
    void construct(pointer p, const T& value)
    {
        // initialize memory with placement new
        if (p)
            new(static_cast<void *>(p))T(value);
    }

    // destroy elements of initialized storage p
    void destroy(pointer p)
    {
        // destroy objects by calling their destructor
        p->~T();
    }

    // deallocate storage p of deleted elements
    void deallocate(pointer p, size_type num)
    {
        CTRPluginFramework::Heap::Free(p);
    }
};

// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator== (const IAllocator<T1>&, const IAllocator<T2>&)
{
    return true;
}

template <class T1, class T2>
bool operator!= (const IAllocator<T1>&, const IAllocator<T2>&)
{
    return false;
}
