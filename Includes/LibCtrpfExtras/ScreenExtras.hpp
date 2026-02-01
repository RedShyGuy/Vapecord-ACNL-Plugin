#pragma once

#include <CTRPluginFramework.hpp>
#include "TextID.hpp"
#include "Language.hpp"

namespace CTRPluginFramework {
    class ScreenExtras {
    public:
        ScreenExtras(const Screen& screen) : screen(screen) {}

        /**
        * \brief Draw a string using system font (support utf8 strings with special chars & unicode)
        * \param str The string to draw
        * \param posX The position on screen to draw the string to
        * \param posY The position on screen to draw the string to
        * \param foreground The color of the characters
        * \param background The color of the background
        * \return posY + 16 (line feed)
        */
        u32     DrawSysfont(const std::string &str, u32 posX, u32 posY, const Color &foreground = Color::White, const Color &background = Color::Black) const;
    
        static u32 SystemFontSize(const char* str) {
            u32 size = OSD::GetTextWidth(true, str);
            size += 1; //To give a little padding
            return (size);
        }
    private:
        const Screen& screen;

        static const int lineHeight = 16;
        static const int spacingY = 3;
        static const int spacingX = 2;
    };
}