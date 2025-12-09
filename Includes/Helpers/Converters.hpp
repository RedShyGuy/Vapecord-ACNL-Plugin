#pragma once

#include <CTRPluginFramework.hpp>
#include <array>

namespace CTRPluginFramework {
    namespace Convert {

        template <class T, size_t N>
        int U16_TO_STR(const T(&input)[N], std::string& output) {
            const int BUFF_SIZE = 7;
            if(MB_CUR_MAX >= BUFF_SIZE) 
                return -1; //"BUFF_SIZE too small"

            std::string result;
            bool shifts = std::wctomb(nullptr, 0); //reset the conversion state
            for(const wchar_t wc : input) {
                std::array<char, BUFF_SIZE> buffer;
                const int ret = std::wctomb(buffer.data(), wc);
                if(ret < 0) 
                    return -2; //"inconvertible wide characters in the current locale"

                buffer[ret] = '\0'; //make 'buffer' contain a C-style string
                result = result + std::string(buffer.data());
            }
            output = result;
            return 0; //success
        }

        template <class T, size_t N>
        int STR_TO_U16(const std::string& input, T(&array)[N]) {
            u16 buffer[sizeof(array) / sizeof(u16)] = { 0 };
            utf8_to_utf16(buffer, reinterpret_cast<const u8*>(input.data()), input.size());
            std::copy(std::begin(buffer), std::end(buffer), std::begin(array));
            return 0; //Success
        }
    }
}