#include <CTRPluginFramework.hpp>
#include "LibCtrpfExtras/ScreenExtras.hpp"

namespace CTRPluginFramework {
    u32 ScreenExtras::DrawSysfont(const std::string &str, u32 posX, u32 posY, const Color &foreground, const Color &background) const {
        const u32 lineHeight = 16;

        screen.DrawRect(posX - spacingX, posY, SystemFontSize(str.c_str()) + (spacingX * 2), lineHeight, background);
        screen.DrawSysfont(str, posX, posY, foreground);

        return posY + lineHeight;
    }
}