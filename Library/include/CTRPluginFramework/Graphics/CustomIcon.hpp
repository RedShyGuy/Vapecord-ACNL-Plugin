#ifndef CTRPLUGINFRAMEWORK_CUSTOMICON_HPP
#define CTRPLUGINFRAMEWORK_CUSTOMICON_HPP

#include "types.h"

namespace CTRPluginFramework
{
    class CustomIcon {

    public:
        struct Pixel
        {
            u8 a;
            u8 b;
            u8 g;
            u8 r;
        };

        Pixel*  pixArray;
        int     sizeX;
        int     sizeY;
        bool    isEnabled;

        CustomIcon(Pixel* pixArray, int sizeX, int sizeY, bool isEnabled);
        CustomIcon();
    };

}

#endif
