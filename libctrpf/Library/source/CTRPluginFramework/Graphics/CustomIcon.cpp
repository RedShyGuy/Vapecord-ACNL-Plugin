#include "types.h"
#include "CTRPluginFramework/Graphics/CustomIcon.hpp"

namespace CTRPluginFramework
{
    CustomIcon::CustomIcon(Pixel* pixArray, int sizeX, int sizeY, bool isEnabled)
    {
        this->pixArray = pixArray;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->isEnabled = isEnabled;
    }

    CustomIcon::CustomIcon()
    {
        pixArray = nullptr;
        sizeX = 0;
        sizeY = 0;
        isEnabled = true;
    }
}
