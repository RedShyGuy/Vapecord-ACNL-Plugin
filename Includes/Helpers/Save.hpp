#pragma once

#include <CTRPluginFramework.hpp>
#include <type_traits>

#define SAVE_GARDENPLUS 0x89B00  //0x95CF94
#define SAVE_MAIL 		0x1C208  //0x983050
#define SAVE_EXHIBITION 0x17BE10 //0x9B4AC0
#define SAVE_FRIEND 	0x29608  //0x983038
#define SAVE_AMIIBO 	0x128 	 //0x95CF88
#define SAVE_DESIGN 	0x25F90  //0x983088
#define SAVE_PLAYER		0xC8B8	 //0x983010

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