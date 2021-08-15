#ifndef ITEMSEQUENCE_HPP
#define ITEMSEQUENCE_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
	namespace ItemSequence {
		void		Init();
		u32			*Next();
		u32			PeekNext();
		void		Switch(bool enable);
		bool		Enabled();
		void		OpenIS();
	}
}
#endif