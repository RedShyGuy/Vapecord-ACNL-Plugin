#include <CTRPluginFramework.hpp>
#include "Helpers/ItemSearcher/ItemSearcher.hpp"
#include "Helpers/IDList.hpp"
#include "Helpers/Player.hpp"
#include "Address/Address.hpp"
#include "Helpers/ItemSearcher/ItemKeywordChecks.hpp"

namespace CTRPluginFramework {
	/*
	Item gets shortened to work further with it
	itemID - 0x2000

	if invalid, return -1
	*/

    ItemCategoryOffset GetItemCategoryOffset(Item_Categories itemCategory) {
		static Address GetItemCategoryLengthAddress(0x5360A8);
		u32 offset = GetItemCategoryLengthAddress.Call<u32>((u8)itemCategory);
		return *(ItemCategoryOffset *)offset;
	}

	u32 GetShortenedItem(Item item) {
		u32 result = -1;

		if ((item & 0xFFFF7FFF) - 0x2000 < 0x172B) {
			result = (item & 0xFFFF7FFF) - 0x2000;
		}

		return result;
	}

    /* Part decomp of 0x6ba680 */
	u16 GetNextItemByCategory(ItemCategoryOffset offset, u32 index) {
		u16 item = 0x7FFE;
		
		if (index < offset.end) {
			item = offset.begin + index & 0xFFFF;
		}

		return item;
	}

	void ClearItemCategorySizeFromStack(ItemCategoryData *itemCategoryData) {
        for (int i = 0; i < INDEX_LENGTH; ++i) {
            itemCategoryData->index[i] = -1;
        }

        itemCategoryData->category = Item_Categories::Invalid;
    }

	bool WriteItemCategorySizeToStack(ItemCategoryData *itemCategoryData, ACNL_Player *player, Item_Categories itemCategory) {
		ClearItemCategorySizeFromStack(itemCategoryData);

        ItemCategoryOffset offset = GetItemCategoryOffset(itemCategory);

		int length = offset.size;
		if (length < INDEX_LENGTH+1) {
			int validIndex = 0;
			if (length != 0) {
                for (int index = 0; index < length; ++index) {
					u16 item = GetNextItemByCategory(offset, index);
					u32 shortenedItem = GetShortenedItem(Item(item));

					bool itemUnlocked = false;
					if ((int)shortenedItem >= 0 && ((shortenedItem >> 5) < 0xBA)) {
						itemUnlocked = (player->UnlockedItems[shortenedItem >> 5] >> (shortenedItem & 0x1F)) & 1;
					}

					if (itemUnlocked) {
						itemCategoryData->index[validIndex++] = index;
					}
				}
			}

            itemCategoryData->category = itemCategory;
			return true;
		}

		ClearItemCategorySizeFromStack(itemCategoryData);
		return false;
	}

	u32 FUN_BE57C4(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
		static Address FUN_00001144(0x2FCC14); //Returns shortened item (?), or u16
		static Address FUN_0000114c(0x2FEA78); //Returns u32(?)
		static Address FUN_00001154(0x2F776C); //Returns void, probably thunk

		u32 uVar1;
		u32 uStack_10;
		
		uStack_10 = param_4;
		uVar1 = FUN_00001144.Call<u16>(&uStack_10, 0x2000);
		uVar1 = FUN_0000114c.Call<u16>(param_1, uVar1, 0);
		FUN_00001154.Call<void>(&uStack_10);
		return uVar1;
	}

    /* decomp of 0x771278 */
	u16 GetItemByCategory(ItemCategoryData *itemCategoryData, Item_Categories itemCategory, int index) {
        if (itemCategoryData->category == itemCategory) {
            ItemCategoryOffset offset = GetItemCategoryOffset(itemCategory);

            if (index <= offset.size) {
                return GetNextItemByCategory(offset, itemCategoryData->index[index]);
            }
        }
        return 0x7FFE;
	}

    /* Decomp of 0x7712c0 */
    int GetItemsToSearchCount(ItemCategoryData *itemCategoryData) {
        if (itemCategoryData->category == Item_Categories::Invalid) {
            return 0;
        }
           
        int count = 0;
        ItemCategoryOffset offset = GetItemCategoryOffset(itemCategoryData->category);

        while (itemCategoryData->index[count] != -1 && itemCategoryData->index[count] < offset.size) {
            count++;
        }

        return count;
    }

	bool SearchItemByKeywordFUNC(u32 param_1/*0x307A6B70*/) {
		//OSD::Notify("Searching...");

		static Address FUN_007712C0(0x7712C0); //Returns u32
		static Address FUN_006F02F4(0x6F02F4); //Returns bool
		static Address FUN_002FCC14(0x2FCC14); //Returns shortened item (?), or u16
		static Address FUN_0076A66C(0x76A66C); //Returns u32
		static Address FUN_002F776C(0x2F776C); //Returns void, probably thunk
		static Address FUN_002FEA78(0x2FEA78); //Returns u32(?)
		static Address FUN_0075C5B8(0x75C5B8); //Returns u32(?)
		static Address FUN_005F56E4(0x5F56E4); //Returns u32, probably thunk
		static Address FUN_00308368(0x308368); //Returns void, probably thunk
		static Address FUN_002FD0BC(0x2FD0BC); //Returns u32 (?)
		static Address FUN_00301B3D(0x301B3D); //Returns u32

		/*
		u32 FUN_00003a34(int param_1) {
			u32 method = *(u32 *)(param_1 + 0x1C18);

			if ((*(u32 *)(param_1 + 0x1C1C) & 1) != 0) {
				method = method + *(u32 *)(param_1 + *(u32 *)(param_1 + 0x1C1C) >> 1);
			}

			Address address(method);
			return address.Call<u32>();
		}
		*/

		/*
		1. 0xbe66c4 (0x36c4)
		2. X-Times 0xbe6a68 (Search Function)
		3. 0xbe6d7c
		*/

		u32 _LAB_00003d58 = 0xC070D0; //Offset 240D0
		u32 _LAB_00003d64 = *(u32 *)(0x953CC4); //Always 0 ?
		u32 _LAB_00003d70 = 0xBE6880; //Offset 3880

		constexpr int MAX_VALUE = 50; //For what i dont know, seems to stop the search  *(u32 *)(0x849D0C);
		constexpr int MAX_CATEGORIES = sizeof(CATEGORIES_TO_SEARCH) / sizeof(CATEGORIES_TO_SEARCH[0]);
		constexpr int MAX_CHECK_METHODS = sizeof(CHECKS) / sizeof(CHECKS[0]);

		u32 local_b8[3];
		u32 *piVar6;
		u32 iVar11 = 0;

		u32 uVar4;

		u32 Item_Stack[21];
		u8 Stack2_AC[80];

		int* ItemCategoryIndex = (int *)(param_1 + 0x1C10);
		int* index = (int *)(param_1 + 0x1C14); //0 is no item, 1 is 1 item found, etc (Only correct after search)
		u32* CleanUp1 = (u32 *)(param_1 + 0x1C18); //Not sure for what
		u32* CleanUp2 = (u32 *)(param_1 + 0x1C1C); //Not sure for what

		do {
			int i;
		//Set to next category, unsure yet why it is a while loop
			while (i = GetItemsToSearchCount((ItemCategoryData *)(param_1 + 0x58)), i <= *index) {
				++(*ItemCategoryIndex);

			//If all categories got searched -> exit
				if (MAX_CATEGORIES <= *ItemCategoryIndex) {
					(*index) = 0;
					(*CleanUp1) = _LAB_00003d70;
					(*CleanUp2) = 0;
					return 0;
				}

				WriteItemCategorySizeToStack((ItemCategoryData *)(param_1 + 0x58), Player::GetSaveData(), CATEGORIES_TO_SEARCH[*ItemCategoryIndex]);

				(*index) = 0;

				iVar11 += 5;
				if (MAX_VALUE <= iVar11) {
					return 0;
				}
			}
			
			uVar4 = GetItemByCategory((ItemCategoryData *)(param_1 + 0x58), CATEGORIES_TO_SEARCH[*ItemCategoryIndex], *index);
			FUN_002FCC14.Call<u16>(Item_Stack, uVar4); //goes through every item by category
			
			u32 res = FUN_0076A66C.Call<u32>(Item_Stack);
			if (res != 0) {
				FUN_002FEA78.Call<u32>(local_b8, Item_Stack, 0); //actually gets the item from the name i believe, stores it in 

				res = FUN_0075C5B8.Call<u32>(local_b8, param_1 + 0x38, 1);
            //If keyword search (or its the check below it)
				if (res == 0) {
					if (*(u32 *)(param_1 + 0x1C20) != 0) {
						int bitShifter = 1;
						int checkIndex = 0;

						do {
							if ((*(u32 *)(param_1 + 0x1C20) & bitShifter) != 0) {
								Keyword_Check_Data checkData = CHECKS[checkIndex];
								//piVar6 = (u32 *)(param_1 + (checkData.unk >> 1));

								if ((checkData.unk & 1U) == 0) {
									res = checkData.method(Item_Stack);
								}
								//else {
								//	res = Address(*piVar6 + (u32)checkData.method).Call<u32>(Item_Stack, piVar6);
								//}

								if (res != 0) {
									goto LAB_00003b84;
								}
							}

							++checkIndex;
							bitShifter <<= 1;
						} while (checkIndex < MAX_CHECK_METHODS);
					}
				}
            //If default item search
				else {
				LAB_00003b84:
					u16 uVar2 = *Item_Stack & 0x7FFF;

					if (*(u32 *)(param_1 + 0x44) == 0) {
						++*(u32 *)(param_1 + 0x54);

						uVar4 = FUN_002FD0BC.Call<u32>(0xBC4, _LAB_00003d64);

						u32 offset = FUN_00301B3D.Call<u32>(uVar4, (u32)FUN_BE57C4, 0x5C, 0x20);
						*(u32 *)(offset + 0xBC0) = 0;
						*(u32 *)(param_1 + 0x48) = offset;
						*(u32 *)(param_1 + 0x44) = offset;
					}

					if (0x1F < *(u32 *)(param_1 + 0x4C)) {
						*(u32 *)(param_1 + 0x4C) = 0;
						++*(u32 *)(param_1 + 0x54);

						uVar4 = FUN_002FD0BC.Call<u32>(0xBC4, _LAB_00003d64);
						u32 offset = FUN_00301B3D.Call<u32>(uVar4, (u32)FUN_BE57C4, 0x5C, 0x20);

						*(u32 *)(offset + 0xBC0) = 0;
						*(u32 *)(*(u32 *)(param_1 + 0x48) + 0xBC0) = offset;
						*(u32 *)(param_1 + 0x48) = offset;
					}

					*(u16 *)(*(u32 *)(param_1 + 0x48) + *(u32 *)(param_1 + 0x4C) * 2 + 0xB80) = uVar2;
					++*(u32 *)(param_1 + 0x50);
					++*(u32 *)(param_1 + 0x4c);
				}

				local_b8[0] = _LAB_00003d58;
				res = FUN_005F56E4.Call<u32>(Stack2_AC);
				FUN_00308368.Call<void>(res + -0xC);
			}

			++iVar11;
			++(*index);

			if (MAX_VALUE <= iVar11) {
				FUN_002F776C.Call<void>(Item_Stack);
				return 0;
			}
			FUN_002F776C.Call<void>(Item_Stack);
		} while(true);
	}

	bool SearchItemByKeyword(std::string& Keyword) {
		//for (auto& itemCategory : CATEGORIES_TO_SEARCH) {

		//}

		return false;
		/*u32 baseInvPointer = *(u32 *)(Game::BaseInvPointer() + 0xC);
		u32 searchStringData = *(u32 *)(baseInvPointer + 0x3BAC);

		//String of item with formatting chars at the beginning / +0xC starts actual string
		//u32 actualSearchString = *(u32 *)(searchStringData);
		SearchItemByKeywordFUNC(searchStringData); //Called by 0x21C1C0

		for (int i = 0; i < test.size(); i++) {
			Process::Write16(0x00A00000 + (i * 2), test.at(i)); //Writes found items to 0xA00000
		}

		//Process::CopyMemory((u32 *)0xA00000, (u32 *)0x330BDA90, 0x500);
		return false;*/
	}
}