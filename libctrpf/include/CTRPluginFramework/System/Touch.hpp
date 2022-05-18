#ifndef CTRPLUGINFRAMEWORK_TOUCH_HPP
#define CTRPLUGINFRAMEWORK_TOUCH_HPP

#include "Vector.hpp"

namespace CTRPluginFramework
{
    class Touch
    {
    public:
        static bool         IsDown(void);
        static UIntVector   GetPosition(void);
    };
}

#endif