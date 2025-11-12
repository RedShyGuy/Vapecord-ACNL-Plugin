#ifndef ACMESSAGEBOX_HPP
#define ACMESSAGEBOX_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    using CheckFunc = bool(*)(u32*, u32*);

    struct __attribute__((packed)) Check_Data {
		CheckFunc method;
		u32 unk;
	};

    constexpr Check_Data CHECKS[] = {
        { Check1, 0 }, { Check2, 0 }, { Check3, 0 },
        { Check4, 0 }, { Check5, 0 }, { Check6, 0 },
        { Check7, 0 }, { Check8, 0 }, { Check9, 0 },
        { Check10, 0 }, { Check11, 0 }, { Check12, 0 },
        { Check13, 0 }, { Check14, 0 }, { Check15, 0 },
        { Check16, 0 },  { Check17, 0 }, { Check18, 0 },
        { Check19, 0 }
    };

    bool Check1(u32 *item, u32 *Stack);
	bool Check2(u32 *item, u32 *Stack);
	bool Check3(u32 *item, u32 *Stack);
	bool Check4(u32 *item, u32 *Stack);
	bool Check5(u32 *item, u32 *Stack);
	bool Check6(u32 *item, u32 *Stack);
	bool Check7(u32 *item, u32 *Stack);
	bool Check8(u32 *item, u32 *Stack);
	bool Check9(u32 *item, u32 *Stack);
	bool Check10(u32 *item, u32 *Stack);
	bool Check11(u32 *item, u32 *Stack);
	bool Check12(u32 *item, u32 *Stack);
	bool Check13(u32 *item, u32 *Stack);
	bool Check14(u32 *item, u32 *Stack);
	bool Check15(u32 *item, u32 *Stack);
	bool Check16(u32 *item, u32 *Stack);
	bool Check17(u32 *item, u32 *Stack);
	bool Check18(u32 *item, u32 *Stack);
	bool Check19(u32 *item, u32 *Stack);
}
#endif