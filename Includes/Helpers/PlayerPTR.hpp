#ifndef PLAYERPTR_HPP
#define PLAYERPTR_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace PlayerPTR {
		u32		Pointer(u32 set);
		bool	Write32(u32 lenght, u32 value);	
		bool	Write16(u32 lenght, u16 value);	
		bool	Write8(u32 lenght, u8 value);	
		bool 	WriteString(u32 lenght, const std::string &input, StringFormat outFmt = StringFormat::Utf16);
	}

	/*class PlayerSave {
		public:	
			template <typename T> 
			bool Write(u32 add, T val) {
				if(std::is_same<T, std::string>::value) 
					return Process::WriteString(m_PlayerOffset + add, (const std::string&)val, StringFormat::Utf16);
				else {
					if(!Process::CheckAddress(m_PlayerOffset + add, MEMPERM_WRITE))
						return 0;

					*(T *)(m_PlayerOffset + add) = val;

					return 1;
				}
				return 0;
			}

			template <typename T> 
			bool Read(u32 add, T& buff, int size = -1) {
				if(std::is_same<T, std::string>::value) 
					return Process::ReadString(m_PlayerOffset + add, (std::string&)buff, size, StringFormat::Utf16);
				else {
					if(!Process::CheckAddress(m_PlayerOffset + add, MEMPERM_WRITE))
						return 0;

					buff = *(T *)(m_PlayerOffset + add);

					return 1;
				}
				return 0;
			}

			//Player Pointer Data Stuff
			static PlayerSave* 	GetInstance(int pIndex = 4);	
			u32 				Offset(u32 add = 0);
			bool 				IsLoaded();
			
		private:
			PlayerSave(void);
			~PlayerSave(void);

			static PlayerSave* m_Instance;

			static u32 m_PlayerOffset;
			static int m_PlayerIndex;
			
			bool m_PlayerLoaded;
	};*/
}
#endif