#include <CTRPluginFramework.hpp>
#include "Address/Address.hpp"

namespace CTRPluginFramework {
    bool Check1(u32 *item, u32 *Stack) {
        static Address FUN_7694C0(0x7694C0);

		return FUN_7694C0.Call<bool>(item, Stack);
    }

	bool Check2(u32 *item, u32 *Stack) {
        static Address FUN_303BD4(0x303BD4);

		return FUN_303BD4.Call<bool>(item, Stack);
    }

	bool Check3(u32 *item, u32 *Stack) {
        static Address FUN_76937C(0x76937C);

		return FUN_76937C.Call<bool>(item, Stack);
    }

	bool Check4(u32 *item, u32 *Stack) {
        static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item, Stack);
		return data == 0x1c || data == 0x19;
    }

	bool Check5(u32 *item, u32 *Stack) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item, Stack);
		return data == 0x1b || data == 0x18;
	}

	bool Check6(u32 *item, u32 *Stack) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item, Stack);
		return data == 0x1d || data == 0x1a;
	}

	bool Check7(u32 *item, u32 *Stack) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item, Stack);
		return data == 0x21;
	}

	bool Check8(u32 *item, u32 *Stack) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item, Stack);
		return data == 0x20;
	}

	bool Check9(u32 *item, u32 *Stack) {
		static Address FUN_536854(0x536854);

		u16 data;

		u32 offset = FUN_536854.Call<u32>(item, Stack);
		if (offset != 0) {
			data = *(u16 *)(offset + 4) & 0xff;

			return data <= 0x2c && data != 0xc;
		}
		return false;
	}

	bool Check10(u32 *item, u32 *Stack) {
		static Address FUN_30B678(0x30B678);

		u16 data = FUN_30B678.Call<u16>(item, Stack);
		return data == 10;
	}

	bool Check11(u32 *item, u32 *Stack) {
		static Address FUN_30B678(0x30B678);

		u16 data = FUN_30B678.Call<u16>(item, Stack);
		return data == 0xc;
	}

	bool Check12(u32 *item, u32 *Stack) {
		static Address FUN_536854(0x536854);

		u16 data;

		u32 offset = FUN_536854.Call<u32>(item, Stack);
		if (offset != 0) {
			data = *(u16 *)(offset + 4) & 0xff;

			return data <= 0x2c && data != 0xe;
		}
		return false;
	}

	bool Check13(u32 *item, u32 *Stack) {
		static Address FUN_536854(0x536854);

		u16 data;

		u32 offset = FUN_536854.Call<u32>(item, Stack);
		if (offset != 0) {
			data = *(u16 *)(offset + 4) & 0xff;

			return data <= 0x2c && data == 3;
		}
		return false;
	}

	bool Check14(u32 *item, u32 *Stack) {
		static Address FUN_536854(0x536854);

		u8 data;

		u32 offset = FUN_536854.Call<u32>(item, Stack);
		if (offset != 0) {
			data = *(u8 *)(offset + 0x11);

			return data <= 0x55 && (data == 0x2d || data == 0x41);
		}
		return false;
	}

	bool Check15(u32 *item, u32 *Stack) {
		static Address FUN_2FCBC4(0x2FCBC4);

		u16 data = FUN_2FCBC4.Call<u16>(item, Stack);
		return data == 0xb;
	}

	bool Check16(u32 *item, u32 *Stack) {
		static Address FUN_2FCBC4(0x2FCBC4);

		u16 data = FUN_2FCBC4.Call<u16>(item, Stack);
		return data == 10;
	}

	bool Check17(u32 *item, u32 *Stack) {
		static Address FUN_3107F4(0x3107F4);

		return FUN_3107F4.Call<bool>(item, Stack);
	}

	bool Check18(u32 *item, u32 *Stack) {
		static Address FUN_3107F4(0x3107F4);

		return !FUN_3107F4.Call<bool>(item, Stack);
	}

	bool Check19(u32 *item, u32 *Stack) {
		static Address FUN_30B678(0x30B678);

		u16 data = FUN_30B678.Call<u16>(item, Stack);
		return data == 0xb;
	}
}
