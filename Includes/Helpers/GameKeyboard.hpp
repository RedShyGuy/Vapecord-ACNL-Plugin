#ifndef CHAT_HPP
#define CHAT_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace GameKeyboard {
		bool 				Write(const std::string& str);
		bool		 		CopySelected(std::string& str);
		bool 				DeleteSelected(void);
		bool 				IsOpen();
		bool				IsEmpty();
		bool 				Copy(std::string& str, int pos, int lenght);
		bool 				ConvertToItemID(std::string& str, u32 &ItemID);
		void 				SendMessage(const std::string& str);
	}
}
#endif