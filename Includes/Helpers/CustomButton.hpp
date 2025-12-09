#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    namespace CustomButton {
        void DuplicateItem(u32 InvData);
        void WrapItem(u32 InvData);
        void RemoveItem(u32 InvData);
        void PutItemToStorage(u32 InvData);
        void PayDebt(u32 InvData);

        void RandomOutfit(u32 InvData);
    }
}