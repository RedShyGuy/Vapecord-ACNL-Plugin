#include "types.h"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/PrivColor.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "font6x10Linux.h"

namespace CTRPluginFramework
{
    void Renderer::DrawPixel(int posX, int posY, const Color& color)
    {
        ScreenImpl  *screen = GetContext()->screen;

        PrivColor::ToFramebuffer(screen->GetLeftFrameBuffer(posX, posY), color);
    }

    // Draw Character without background
    void     Renderer::DrawCharacter(int c, int posX, int posY, const Color &fg)
    {
        ScreenImpl  *screen = GetContext()->screen;

        u32 stride = screen->GetStride();
        //u32 bpp = screen->GetBytesPerPixel();
        int posXX = posX - 10;
        int posYY = posY;

        c *= 10;
        const u8 *charInFont = font + c;

        for (int yy = 0; yy < 10; ++yy)
        {
            u8  charPos = *charInFont++;
            u8  *fb = screen->GetLeftFrameBuffer(posX, posY++);

            for (int xx = 6; xx > 0; --xx)
            {
                if ((charPos >> xx) & 1)
                    PrivColor::ToFramebuffer(fb, fg);
                fb += stride;
            }
        }

        if (screen->Is3DEnabled())
        {
            charInFont -= 10;
            for (int yy = 0; yy < 10; ++yy)
            {
                u8  charPos = *charInFont++;
                u8  *fb = screen->GetRightFrameBuffer(posXX, posYY++);

                for (int xx = 6; xx > 0; --xx)
                {
                    if ((charPos >> xx) & 1)
                        PrivColor::ToFramebuffer(fb, fg);
                    fb += stride;
                }
            }
        }
    }

    // Draw Character with background
    void     Renderer::DrawCharacter(int c, int posX, int posY, const Color &fg, const Color &bg)
    {
        ScreenImpl  *screen = GetContext()->screen;

        u32 stride = screen->GetStride();
        //u32 bpp = screen->GetBytesPerPixel();
        int posXX = posX - 10;
        int posYY = posY;

        c *= 10;
        const u8 *charInFont = font + c;

        for (int yy = 0; yy < 10; ++yy)
        {
            u8  charPos = *charInFont++;
            u8  *fb = screen->GetLeftFrameBuffer(posX, posY++);

            for (int xx = 6; xx > 0; --xx)
            {
                PrivColor::ToFramebuffer(fb, (charPos >> xx) & 1 ? fg : bg);
                fb += stride;
            }
        }

        if (screen->Is3DEnabled())
        {
            charInFont -= 10;

            for (int yy = 0; yy < 10; ++yy)
            {
                u8  charPos = *charInFont++;
                u8  *fb = screen->GetRightFrameBuffer(posXX, posYY++);

                for (int xx = 6; xx > 0; --xx)
                {
                    PrivColor::ToFramebuffer(fb, (charPos >> xx) & 1 ? fg : bg);
                    fb += stride;
                }
            }
        }
    }

    int    Renderer::DrawString(const char *str, int posX, int &posY, Color fg)
    {
        Color bak = fg;

        while (*str)
        {
            u8 c = *str++;

            if (c == 0x18)
            {
                fg = bak;
                continue;
            }
            if (c == 0x1B)
            {
                fg.r = *str++;
                fg.g = *str++;
                fg.b = *str++;
                continue;
            }
            if (c == 0xFF) ///< Lazy fix for 0x18 arrow symbol
                c = 0x18;
            if (c == 0xFE) ///< Lazy fix for 0x1B arrow symbol
                c = 0x1B;
            DrawCharacter(c, posX, posY, fg);
            posX += 6;
        }
        posY += 10;
        return (posY);
    }

    int    Renderer::DrawString(const char *str, int posX, int &posY, Color fg, Color bg)
    {
        ScreenImpl  *screen = GetContext()->screen;

        u32 bpp = screen->GetBytesPerPixel();
        Color bak = fg;

        for (int i = 0; i < 2; i++)
        {
            u8 *fb = screen->GetLeftFrameBuffer(posX + i, posY);
            for (int y = 0; y < 10; y++)
            {
                PrivColor::ToFramebuffer(fb, bg);
                fb -= bpp;
            }
        }

        if (screen->Is3DEnabled())
        {
            for (int i = 0; i < 2; i++)
            {
                u8 *fb = screen->GetRightFrameBuffer(posX - 10 + i, posY);
                for (int y = 0; y < 10; y++)
                {
                    PrivColor::ToFramebuffer(fb, bg);
                    fb -= bpp;
                }
            }
        }

        posX += 2;
        while (*str)
        {
            u8 c = *str++;

            if (c == 0x18)
            {
                fg = bak;
                continue;
            }
            if (c == 0x1B)
            {
                fg.r = *str++;
                fg.g = *str++;
                fg.b = *str++;
                continue;
            }
            if (c == 0xFF) ///< Lazy fix for 0x18 arrow symbol
                c = 0x18;
            if (c == 0xFE) ///< Lazy fix for 0x1B arrow symbol
                c = 0x1B;
            DrawCharacter(c, posX, posY, fg, bg);
            posX += 6;
        }
        posY += 10;
        return (posY);
    }

    u32     Renderer::LinuxFontSize(const char* str)
    {
        u32 size = 0;

        for (; *str; str++)
        {
            char c = *str;
            if (c == 0x1B)
                str += 3;
            else if (c != 0x18)
                size += 6;
        }

        return (size);
    }
}
