#ifndef CROEDITING_HPP
#define CROEDITING_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    namespace CRO {
        bool    GetMemAddress(const char* croFileName, u32& buffer);
        bool    WritePermToggle(u32 address, bool ON);
        
        template<typename T>
        bool Write(const char* croFileName, u32 address, T value) {
            u32 buffer = 0;
            if(!CRO::GetMemAddress(croFileName, buffer))
                return false;

            if(*(T *)(buffer + address) == value)
                return false;

            if(WritePermToggle(buffer, true)) {
                *(T *)(buffer + address) = value;
                WritePermToggle(buffer, false);
                return true;
            }
            return false;
        }
    }
}
#endif