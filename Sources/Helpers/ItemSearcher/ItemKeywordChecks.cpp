#include <CTRPluginFramework.hpp>
#include "Address/Address.hpp"
#include "Helpers/ItemSearcher/ItemKeywordChecks.hpp"

namespace CTRPluginFramework {
    bool IsRefurbishable(u32 *item) {
        static Address FUN_7694C0(0x7694C0);

		return FUN_7694C0.Call<bool>(item);
    }

	bool IsWallHangReady(u32 *item) {
        static Address FUN_303BD4(0x303BD4);

		return FUN_303BD4.Call<bool>(item);
    }

	bool IsGracieGracie(u32 *item) {
        static Address FUN_76937C(0x76937C);

		return FUN_76937C.Call<bool>(item);
    }

	bool IsShortSleeves(u32 *item) {
        static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item);
		return data == 0x1c || data == 0x19;
    }

	bool IsLongSleeves(u32 *item) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item);
		return data == 0x1b || data == 0x18;
	}

	bool IsSleeveless(u32 *item) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item);
		return data == 0x1d || data == 0x1a;
	}

	bool IsShortPants(u32 *item) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item);
		return data == 0x21;
	}

	bool IsLongPants(u32 *item) {
		static Address FUN_311840(0x311840);

		u16 data = FUN_311840.Call<u16>(item);
		return data == 0x20;
	}

	bool IsChair(u32 *item) {
		static Address FUN_536854(0x536854);

		u16 data;

		u32 offset = FUN_536854.Call<u32>(item);
		if (offset != 0) {
			data = *(u16 *)(offset + 4) & 0xff;

			return data <= 0x2c && data != 0xc;
		}
		return false;
	}

	bool IsClock(u32 *item) {
		static Address FUN_30B678(0x30B678);

		u16 data = FUN_30B678.Call<u16>(item);
		return data == 10;
	}

	bool IsLamp(u32 *item) {
		static Address FUN_30B678(0x30B678);

		u16 data = FUN_30B678.Call<u16>(item);
		return data == 0xc;
	}

	bool IsDrawers(u32 *item) {
		static Address FUN_536854(0x536854);

		u16 data;

		u32 offset = FUN_536854.Call<u32>(item);
		if (offset != 0) {
			data = *(u16 *)(offset + 4) & 0xff;

			return data <= 0x2c && data != 0xe;
		}
		return false;
	}

	bool IsPlant(u32 *item) {
		static Address FUN_536854(0x536854);

		u16 data;

		u32 offset = FUN_536854.Call<u32>(item);
		if (offset != 0) {
			data = *(u16 *)(offset + 4) & 0xff;

			return data <= 0x2c && data == 3;
		}
		return false;
	}

	bool IsInstrument(u32 *item) {
		static Address FUN_536854(0x536854);

		u8 data;

		u32 offset = FUN_536854.Call<u32>(item);
		if (offset != 0) {
			data = *(u8 *)(offset + 0x11);

			return data <= 0x55 && (data == 0x2d || data == 0x41);
		}
		return false;
	}

	bool IsShoes(u32 *item) {
		static Address FUN_2FCBC4(0x2FCBC4);

		u16 data = FUN_2FCBC4.Call<u16>(item);
		return data == 0xb;
	}

	bool IsSocks(u32 *item) {
		static Address FUN_2FCBC4(0x2FCBC4);

		u16 data = FUN_2FCBC4.Call<u16>(item);
		return data == 10;
	}

	bool IsNotForSale(u32 *item) {
		static Address FUN_3107F4(0x3107F4);

		return FUN_3107F4.Call<bool>(item);
	}

	bool IsForSale(u32 *item) {
		static Address FUN_3107F4(0x3107F4);

		return !FUN_3107F4.Call<bool>(item);
	}

	bool IsAudio(u32 *item) {
		static Address FUN_30B678(0x30B678);

		u16 data = FUN_30B678.Call<u16>(item);
		return data == 0xb;
	}
}
