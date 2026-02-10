#include <CTRPluginFramework.hpp>
#include "LibCtrpfExtras/ScreenExtras.hpp"
#include <math.h>

namespace CTRPluginFramework {
    inline Color _ReadRGB565(u8 *src) {
        union {
            u16 u;
            u8 b[2];
        } half;

        Color color;

        half.u = *reinterpret_cast<u16 *>(src);

        color.a = 255;
        color.r = (half.u >> 8) & 0xF8;
        color.g = (half.u >> 3) & 0xFC;
        color.b = (half.u << 3) & 0xF8;
        return (color);
    }

    inline u8 *_WriteRGB565(u8 *dst, const Color &color) {
        union {
            u16 u;
            char b[2];
        } half;

        half.u  = (color.r & 0xF8) << 8;
        half.u |= (color.g & 0xFC) << 3;
        half.u |= (color.b & 0xF8) >> 3;

        *reinterpret_cast<u16 *>(dst) = half.u;
        return dst + 2;
    }

    void ScreenExtras::BlendRect(u32 startX, u32 startY, u32 width, u32 height, const Color &fg) const {
        if (!width || !height) {
            return;
        }

        const u32 maxWidth  = screen.IsTop ? 400 : 320;
        const u32 maxHeight = 240;

        if (startX >= maxWidth || startY >= maxHeight) {
            return;
        }

        width  = std::min(width,  maxWidth  - startX);
        height = std::min(height, maxHeight - startY);

        constexpr u32 bpp = 2;
        const u32 stride = screen.Stride;

        const u32 a  = fg.a;
        const u32 ia = 255 - a;

        const u32 fr = fg.r;
        const u32 fg_g = fg.g;
        const u32 fb = fg.b;

        u8* base = (u8*)screen.LeftFramebuffer;

        const u32 yBase = (maxHeight - 1 - startY) * bpp;

        for (u32 x = 0; x < width; ++x) {
            u8* p = base + (startX + x) * stride + yBase;

            u32 y = height;

            while (y--) {
                u16 px = *(u16*)p;

                u32 br = (px >> 8) & 0xF8;
                u32 bg = (px >> 3) & 0xFC;
                u32 bb = (px << 3) & 0xF8;

                u32 r = (fr * a + br * ia) >> 8;
                u32 g = (fg_g * a + bg * ia) >> 8;
                u32 b = (fb * a + bb * ia) >> 8;

                *(u16*)p =
                    ((r & 0xF8) << 8) |
                    ((g & 0xFC) << 3) |
                    ((b & 0xF8) >> 3);

                p -= bpp;
            }
        }
    }

    u32 ScreenExtras::DrawSysfontWithBackground(const std::string &str, u32 posX, u32 posY, const Color &foreground, const Color &background) const {
        const u32 lineHeight = 16;
        if (screen.Is3DEnabled) {
            screen.DrawRect(posX, posY, SystemFontSize(str.c_str()), lineHeight, background);
        } else {
            BlendRect(posX, posY, SystemFontSize(str.c_str()), lineHeight, background);
        }
        
        screen.DrawSysfont(str, posX, posY, foreground);
        return posY + lineHeight;
    }
}