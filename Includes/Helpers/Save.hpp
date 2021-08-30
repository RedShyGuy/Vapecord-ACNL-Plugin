#ifndef SAVE_HPP
#define SAVE_HPP

#include <CTRPluginFramework.hpp>
#include <type_traits>

namespace CTRPluginFramework {
	
	class Save {
		public:	
			template <typename T> 
			bool Write(u32 add, T val) {
				if(std::is_same<T, std::string>::value) {
					return Process::WriteString(s_SaveAddress + add, (const std::string&)val, StringFormat::Utf16);
				}
				else {
					if(!Process::CheckAddress(s_SaveAddress + add, MEMPERM_WRITE))
						return 0;

					*(T *)(s_SaveAddress + add) = val;

					return 1;
				}
				return 0;
			}

			template <typename T> 
			bool Read(u32 add, T& buff, int size = -1) {
				if(std::is_same<T, std::string>::value) 
					return Process::ReadString(s_SaveAddress + add, (std::string&)buff, size, StringFormat::Utf16);
				else {
					if(!Process::CheckAddress(s_SaveAddress + add, MEMPERM_WRITE))
						return 0;

					buff = *(T *)(s_SaveAddress + add);

					return 1;
				}
				return 0;
			}

			u32 Address(u32 add = 0);

			static Save *GetInstance();

			Save();

			static Save *s_Instance;
			static u32 s_SaveAddress;
	};
}
#endif