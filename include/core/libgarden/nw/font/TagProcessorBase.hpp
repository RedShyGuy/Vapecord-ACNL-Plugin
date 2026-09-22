#pragma once

#include <core/libgarden/nn/types.h>

namespace nw::font
{

	template<typename T>
	class TagProcessorBase
	{
	
	public:

		virtual ~TagProcessorBase() {
			delete this;
		}

	private:


	};
	ASSERT_SIZE(TagProcessorBase<char16>, 4);
	ASSERT_SIZE(TagProcessorBase<char>, 4);
}
