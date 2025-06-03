#ifndef POINTERSEARCHER_HPP
#define POINTERSEARCHER_HPP

#include "types.h"
#include <vector>

namespace CTRPluginFramework
{
    struct Pointer
    {
        u32     base;
        std::vector<u32> offsets;
    };

    class PointerSearcher
    {
    public:
        PointerSearcher(const u32 address, const u32 maxOffset, const u32 maxLayer);
        ~PointerSearcher(void) = default;

        void    Start(void);

    private:
        const u32   _address;
        const u32   _maxOffset;
        const u32   _maxLayer;

        std::vector<Pointer>    _results;
    };
}

#endif
