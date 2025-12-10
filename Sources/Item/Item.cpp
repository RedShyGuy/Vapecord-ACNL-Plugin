#include <CTRPluginFramework.hpp>
#include "Item/Item.hpp"
#include "Address/Address.hpp"
#include "RuntimeContext.hpp"

#define RANGE(X, START, END)	((X & 0xFFFF) >= START && (X & 0xFFFF) <= END)
#define IS(X, ADDR)				((X & 0xFFFF) == ADDR)

namespace CTRPluginFramework {
	bool Item::isValid(bool IsDropped) {
		if(!isFlagValid(IsDropped)) {
			return false;
		}

        u16 tempID = ID;
		if((tempID & 0xFFFF) >> 12 == 4 || (tempID & 0xFFFF) >> 12 == 5) {//if present
			tempID = tempID - 0x2000; //convert present to standard Item ID to know if it is valid
		}

	//If player is outdoors or doesnt drop item
		if(!RuntimeContext::getInstance()->isIndoors() || !IsDropped) {
			if(IS(tempID, 0x7FFE) || RANGE(tempID, 0, 0xFD)) {
				return true;
			}
		}

		if(RANGE(tempID, 0x2001, 0x2060) || RANGE(tempID, 0x2089, 0x2185) || 
            RANGE(tempID, 0x21E4, 0x22DF) || RANGE(tempID, 0x22E1, 0x30CB) || 
            	RANGE(tempID, 0x30D2, 0x3108) || RANGE(tempID, 0x3130, 0x33B4) || 
            		RANGE(tempID, 0x33BC, 0x34CD) || RANGE(tempID, 0x3726, 0x372A)) {
			return true;
		}
		
		return false;
	}

    bool Item::isFlagValid(bool IsDropped) {
	//If player is indoors and drops item
		if(RuntimeContext::getInstance()->isIndoors() && IsDropped) {
			if(Flags >= 0x8000) {
				return false;
			}
		}
		return true;
	}

    std::string Item::GetName() {
		if (Item::hasNoName()) {
			for (const ItemNamePack& items : IllegalItemNameList) {
				if (items.ID == ID) {
					return std::string(items.name);
				}
			}
		}

		static Address SetUpStack(0x3081E8);
		static Address SetUpItem(0x769DBC);
		static Address SetUp(0x312610);

		u32 Stack[44];
		u32 add = SetUpStack.Call<u32>(Stack, Stack + 0x18, 0x21);

		u16 itemID = SetUpItem.Call<u16>(&ID);
		SetUp.Call<void>(*(u32 *)Address(0x95EEDC).addr, add, (char *)"STR_Item_name", itemID);

		std::string itemName = "";
		Process::ReadString(Stack[1] + 0xC, itemName, 0x30, StringFormat::Utf16);

		if (itemName.empty()) {
			return "???";
		}
		else {
			itemName += 2; //Skips formatting chars
			itemName.pop_back(); //Removes formatting char
			return itemName;
		}
	}

    bool Item::hasNoName() {
		if (RANGE(ID, 0, 0x00FD) || RANGE(ID, 0x2493, 0x2494) ||
            RANGE(ID, 0x30A3, 0x30A7) || RANGE(ID, 0x33A7, 0x33B4) || 
            	RANGE(ID, 0x340C, 0x3419) || RANGE(ID, 0x3727, 0x3729) || 
            		IS(ID, 0x2037) || IS(ID, 0x324D) || IS(ID, 0x3263) || IS(ID, 0x341E)) {
            return true;
        }
		
		return false;
	}

	bool Item::isTool() {
		return (IS(ID, 0x2001) || IS(ID, 0x3729) || RANGE(ID, 0x334C, 0x33A2));
	}

    bool Item::searchByKeyword(const std::string& keyword, ItemNamePack& foundItem) {
		ItemNamePack partialMatch;
		bool hasPartial = false;

        for (u16 id = 0; id < 0x372B; ++id) {
            Item item(id);
			const std::string& name = item.GetName();

			size_t pos = name.find(keyword);

			if (pos != std::string::npos) {
				if (!hasPartial) {
					partialMatch = {name, id};
					hasPartial = true;
				}
			}

			// exact match
			if (name == keyword) {
				foundItem = {name, id};
				return true;
			}
		}

		if (hasPartial) {
			foundItem = partialMatch;
			return true;
		}

        return false;
    }

    bool Item::searchAllByKeyword(const std::string& keyword, std::vector<ItemNamePack>& foundItems) {
        for (u16 id = 0; id <= 0x372B; ++id) {
            Item item(id);
            if (item.GetName().find(keyword) != std::string::npos) {
                foundItems.push_back(ItemNamePack{item.GetName(), id});
            }
        }
        return !foundItems.empty();
    }
}