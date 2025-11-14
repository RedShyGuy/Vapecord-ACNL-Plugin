#ifndef ITEMSEARCHER_HPP
#define ITEMSEARCHER_HPP

#include <CTRPluginFramework.hpp>
#include "Helpers/GameStructs.hpp"

namespace CTRPluginFramework {
    inline constexpr const Item_Categories CATEGORIES_TO_SEARCH[15] = {
        Wallpaper, Carpets, Furniture, 
        Shirts, Trousers, Dresses, 
		Hats, Accesories, Shoes, 
        Socks, Umbrellas, MailPapers, 
        Songs, Gyroids, AnalyzedFossils
    };

	struct __attribute__((packed)) ItemCategoryOffset {
		u16 begin;
		u16 end;
		u32 size;
	};

	struct __attribute__((packed)) ItemCategoryData {
		Item_Categories category;
		u32 index[0x6ED];
	};

    inline constexpr int INDEX_LENGTH = sizeof(ItemCategoryData::index) / sizeof(ItemCategoryData::index[0]);

    /*
	struct __attribute__((packed)) SearchData {
		u32 FuncPointer; //0 //0xC0CD54
		u8 unk_1[0x34]; //4
		u32 unk_2; //0x38
		u32 unk_3; //0x3C
		u32 unk_4; //0x40
		u32 unk_5; //0x44
		u32 unk_6; //0x48
		u32 unk_7; //0x4C
		u32 unk_8; //0x50
		u32 unk_9; //0x54
		u32 itemCategory; //0x58 (When Catalog opens 0x9B, After Search 0x2C)
		u32 increment_data[0x6ED]; //0x5C
		int Item_Category_Index; //0x1C10
		int Found_Items; //0x1C14
		u32 unk_B; //0x1C18
		u32 unk_C; //0x1C1C
		u32 unk_D; //0x1C20
	};
    */

    bool SearchItemByKeyword(std::string& Keyword);
    bool SearchItemByKeywordFUNC(u32 param_1/*0x307A6B70*/);
}
#endif