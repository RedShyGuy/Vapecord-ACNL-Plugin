#pragma once

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

        template <typename T, class ...Args>
        T Call(const char* croFileName, u32 address, Args ...args) {
            u32 buffer = 0;
            if(CRO::GetMemAddress(croFileName, buffer)) {
                return ((T(*)(Args...))(buffer + address))(args...);
            }

            if constexpr (std::is_same_v<T, void>) {
                return; // no return for void
            } else {
                return T(); // default return value
            }
        };	
    }
}