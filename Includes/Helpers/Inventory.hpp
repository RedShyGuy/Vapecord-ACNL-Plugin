#pragma once

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

#define IsItemSlot(X)	(X >= 0x00 && X <= 0x0F)
#define IsMailSlot(X)	(X >= 0x14 && X <= 0x1D)

namespace CTRPluginFramework {
	using slotReader = void(*)(u32, u8);

	/*
	Not finished, lots of unknown ones or not properly names ones
	does need a lot of work, for now enough
	*/
	enum class MenuType : u8 {
		BaseInventory = 0,
		BaseDesigns,
		Encyclopedia,
		TPC,
		BestFriends_1,
		ChatKeyboard,
		Emotions,
		PeopleHereList,
		SaveWhereItemLeila, //Unused
		GiveWhatItem_1,
		SellWhatItemReese,
		ChooseASongItem,
		GiveWhatItemChip,
		GiveWhatItemNat,
		GiveWhatItemJack,
		DonateWhatItemBlathers,
		AssessWhatItemBlathers,
		GiveWhatItem_2,
		GiveWhatItemNooklings,
		ShowWhatItemCyrus,
		DisplayWhatItemReese,
		GiveWhatItem_3,
		GiveWhatItemCyrus_1,
		GiveWhatItemCyrus_2,
		GiveWhatItemCyrus_3,
		GiveWhatItemCyrus_4,
		GiveWhatItemFranklin_1,
		GiveWhatItemFranklin_2,
		GiveWhatItemShrunk_1,
		GiveWhatItemShrunk_2,
		GiveWhatItemRedd,
		SwapWithWhichGyroidItem,
		GiveWhatItemLeila,
		GiveWhatItem_4,
		GiveWhatItem_5,
		GiveWhatItem_6,
		GiveWhatItem_7,
		GiveWhatItem_8,
		GiveWhatPresentItem,
		GiveWhatItem_9,
		GiveWhatItem_10,
		GiveWhatItemSnowmam,
		ThrowWhatAwayItem,
		WrapWhatItem,
		SellWhatItemNooklings,
		GiveWhatItemIsabelle,
		ChangeDateAndTime,
		ChooseATimeToMeet,
		SelectTheDateMail,
		WhenIsYourBirthday,
		HowManyDoYouWantTurnips,
		SellItForWhatPriceReese,
		UNKNOWN_Bells,
		EnterPIN, //maybe unused? don't know
		EnterDreamAddressLuna,
		DepositBells,
		WithdrawBells,
		PayLoanBells,
		DonateBells,
		ConvertCouponsToBells,
		UNKNOWN_BellsOrCoupons,
		LockerItem,
		IslandBinItemTown,
		IslandBinItemIsland,
		BorrowToolsLloid,
		MailBox, //Might crash when forced opened
		SendMail,
		GiveWhatMailForFutureSelf,
		SaveMail, //crashes
		UNKNOWN_Crash_1, //crashes | maybe mail related
		UNKNOWN_Crash_2,
		BulletinBoard_1, //Crashes
		BulletinBoard_2,
		SwapItemInventory_1, //item dropping
		SwapBuryItemInventory,
		SwapItemInventory_2, //item dropping
		SwapItemInventory_3, //item dropping
		UNKNOWN_Crash_3, //probably item dropping of some sort
		ChooseADesignMable,
		ChooseADesignTownFlag,
		ChooseADesignCyrus,
		ChooseADesignIsabelle,
		ChooseADesignForExhibitMuseum, 
		ChooseADesignSable,
		TakeWhichDesignWendell,
		ChooseADesign_2, //pencil icon
		SaveWhereDesign,
		ChooseADesign_3, //pencil icon
		SaveWhereDesignMable_1,
		SaveWhereDesignMable_2,
		SaveWhereDesignSable,
		SaveWhereDesignReese,
		SaveWhereDesignWendell_1,
		UNKNOWN_Crash_4, //might also be save where design
		SaveWhereDesignWendell_2,
		GiveWhatTimeCapsuleMail,
		MiiSelector,
		UNKNOWN_Crash_5,
		UNKNOWN_Crash_6,
		UNKNOWN_Crash_7,
		UNKNOWN_Crash_8,
		TownTreeTownHistory,
		GiveWhatPresentToyDay,
		ViewPresentsToyDay,
		BingoCardStampSnowman,
		BingoCardViewSnowman,
		DonatedFishListMuseum,
		DonatedInsectListMuseum,
		ScavengerHuntList,
		List, //some sort of list, like the scavenger hunt list
		SignForALiveVenueList,
		UNKNOWN_1, //freezes player, restart needed
		ChangeTPCMessage,
		IsThisPictureOKTPCPictureTaken,
		UNKNOWN_Crash_9,
		BestFriendsShowTPC,
		BestFriends_2,
		BestFriendsWriteMessagePrivate,
		UNKNOWN_2,
		AvailableTownsToVisitList, //crashes when open outside
		AvailableTownsToVisitListWithUpdateButton, //might have been planned to not auto update it
		EditTownTune_1,
		EditTownTune_2,
		EditDesign,
		Exhibit1FMuseum,
		SelectTourListIsland,
		UNKNOWN_Crash_10,
		UNKNOWN_Crash_11,
		UNKNOWN_Crash_12,
		UNKNOWN_Crash_13,
		UNKNOWN_Crash_14,
		ScanAmiibo,
		MoveFurniture,
		UNKNOWN_Crash_15,
		RVVisitorInfo,
		ReceiveCouponsTownHall,
		UNKNOWN_Crash_16,
		SecretStorageLocker,
		BubbleMessage, //dynamic, can be return home from island, launch nintendo 3ds image share, etc.
		UNKNOWN_3, //just resets the current menu?
		UNKNOWN_4, //freezes everything, restart needed
		//... maybe more
	};

	namespace Inventory {
		u32 		GetCurrentItemData(int i = 0);
		bool 		GetNextItem(Item itemID, u8 &slot, bool ignoreFlag = false);
		bool 		GetNextClosetItem(Item itemID, u8 &slot);
		bool 		GetSelectedSlot(u8& slot);
		bool 		GetHoveredSlot(u8& slot);
		u16			GetAddData();
		bool		Opened();
		u8			GetCurrent();

		bool 		WriteSlot(int slot, Item item, u8 lock = 0);
		bool 		ReadSlot(int slot, Item& item);
		void		ReloadIcons(void);
	}
}