#include "LibCtrpfExtras/ColorExtras.hpp"

namespace CTRPluginFramework {
    std::string ColorExtras::Format(const char *fmt, ...) 
    {
		std::string res;

		va_list list;
		va_start(list, fmt);
		while(*fmt != '\0') {
			if(*fmt != '%') {
				res += *fmt;
				fmt++;
				continue;
			}

			fmt++;

			if(*fmt == '\0') {
				break;
			}

			if(*fmt == 'c') {
				void* p = va_arg(list, void *);

				u32 RawColor = (u32)p;

				char strColor[5] = { 
					0x1B, 
					std::max((u8)1, (u8)(RawColor & 0x000000FF)), 
					std::max((u8)1, (u8)((RawColor & 0x0000FF00) >> 8)),
					std::max((u8)1, (u8)((RawColor & 0x00FF0000) >> 16)), 
				};
				res += strColor[0];
				res += strColor[1];
				res += strColor[2];
				res += strColor[3];
       		}      

			fmt++;
		}

		return res;
	}

    std::string ColorExtras::RemoveColor(const std::string &str) 
    {
		std::vector<char> r_char(str.begin(), str.end());
		if((u8)r_char[0] == 0x1B) { //if colorcode
			return str.substr(4);
		}
		return str;
	}
}