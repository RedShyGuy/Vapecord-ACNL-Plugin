#ifndef CROEDITING_HPP
#define CROEDITING_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    namespace CRO {
        bool    GetMemAddress(const char* croFileName, u32& buffer);
        bool    WritePermToggle(u32 address, bool ON);
        bool    Write(const char* croFileName, u32 address, u32 value);
    }
}
#endif