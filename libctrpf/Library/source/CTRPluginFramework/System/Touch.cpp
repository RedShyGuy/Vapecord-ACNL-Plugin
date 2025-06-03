#include "types.h"
#include "3ds.h"

#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFramework/System/Touch.hpp"

namespace CTRPluginFramework
{
    bool        Touch::IsDown(void)
    {
        return (Controller::GetKeysDown() & Key::Touchpad);
    }

    UIntVector  Touch::GetPosition(void)
    {
        touchPosition   tp;

        hidTouchRead(&tp);
        return (UIntVector(tp.px, tp.py));
    }
}
