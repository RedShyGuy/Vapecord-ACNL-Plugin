#ifndef CTRPLUGINFRAMEWORKIMPL_SYSTEM_HEAP_HPP
#define CTRPLUGINFRAMEWORKIMPL_SYSTEM_HEAP_HPP

namespace CTRPluginFramework
{
    namespace Heap
    {
        void    *Alloc(const size_t size);
        void    *MemAlign(const size_t size, size_t alignment);
        void    *Realloc(void *ptr, const size_t size);
        void    Free(void *ptr);
        size_t  SpaceFree(void);
    }
}

#endif
