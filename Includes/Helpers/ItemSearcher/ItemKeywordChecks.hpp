#ifndef ITEMKEYWORDCHECKS_HPP
#define ITEMKEYWORDCHECKS_HPP

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    using KeywordCheckFunc = bool(*)(u32*);

    struct __attribute__((packed)) Keyword_Check_Data {
		KeywordCheckFunc method;
		u32 unk;
	};

	/*
	Refurbish ready
	*/
	bool IsRefurbishable(u32 *item);

	/*
	Wall hang ready
	*/
	bool IsWallHangReady(u32 *item);

	/*
	GracieGrace
	*/
	bool IsGracieGracie(u32 *item);

	/*
	Short sleeves
	*/
	bool IsShortSleeves(u32 *item);

	/*
	Long sleeves
	*/
	bool IsLongSleeves(u32 *item);

	/*
	Sleeveless
	No sleeves
	*/
	bool IsSleeveless(u32 *item);

	/*
	Short pants
	Half pants
	Shorts
	*/
	bool IsShortPants(u32 *item);

	/*
	Long pants
	*/
	bool IsLongPants(u32 *item);

	/*
	Seat
	Chair
	Stool
	*/
	bool IsChair(u32 *item);

	/*
	Watch
	Clock
	*/
	bool IsClock(u32 *item);

	/*
	Lamp
	Light
	Lantern	
	*/
	bool IsLamp(u32 *item);

	/*
	Drawers
	Chest
	Closet
	Cupboard
	*/
	bool IsDrawers(u32 *item);

	/*
	Plant
	*/
	bool IsPlant(u32 *item);

	/*
	Instrument
	*/
	bool IsInstrument(u32 *item);

	/*
	Shoe
	Shoes
	*/
	bool IsShoes(u32 *item);

	/*
	Socks
	*/
	bool IsSocks(u32 *item);

	/*
	Not for sale
	*/
	bool IsNotForSale(u32 *item);

	/*
	For sale
	*/
	bool IsForSale(u32 *item);

	/*
	Audio
	*/
	bool IsAudio(u32 *item);

	inline constexpr Keyword_Check_Data CHECKS[19] = {
        { IsRefurbishable, 0 }, { IsWallHangReady, 0 }, { IsGracieGracie, 0 },
        { IsShortSleeves, 0 }, { IsLongSleeves, 0 }, { IsSleeveless, 0 },
        { IsShortPants, 0 }, { IsLongPants, 0 }, { IsChair, 0 },
        { IsClock, 0 }, { IsLamp, 0 }, { IsDrawers, 0 },
        { IsPlant, 0 }, { IsInstrument, 0 }, { IsShoes, 0 },
        { IsSocks, 0 },  { IsNotForSale, 0 }, { IsForSale, 0 },
        { IsAudio, 0 }
    };
}
#endif