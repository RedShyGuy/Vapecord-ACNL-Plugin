#include "types.h"

#include "CTRPluginFramework/Graphics/Render.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Font.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include <algorithm>

namespace CTRPluginFramework
{
    extern "C" CFNT_s* g_sharedFont;
    extern "C" int g_charPerSheet;

    inline u32   GetFramebufferOffset(int posX, int posY, int bpp, int rowsize)
    {
        return ((rowsize - 1 - posY + posX * rowsize) * bpp);
    }

    void Renderer::FontCalcGlyphPos(fontGlyphPos_s *out,  charWidthInfo_s **cwout, int glyphIndex, float scaleX, float scaleY)
    {
        FINF_s* finf = &g_sharedFont->finf;
        TGLP_s* tglp = finf->tglp;
        charWidthInfo_s *cwi = fontGetCharWidthInfo(nullptr, glyphIndex);
        *cwout = cwi;

        int sheetId = glyphIndex / g_charPerSheet;
        int glInSheet = glyphIndex % g_charPerSheet;
        out->sheetIndex = sheetId;
        out->xOffset = scaleX * cwi->left;
        out->xAdvance = scaleX * cwi->charWidth;
        out->width = scaleX * cwi->glyphWidth;

        int lineId = glInSheet / tglp->nRows;
        int rowId = glInSheet % tglp->nRows;

        float tp = (float)(rowId*(tglp->cellWidth+1)+1);
        float tx = (float)(tp / tglp->sheetWidth)   ;
        float ty = 1.0f - (float)((lineId+1)*(tglp->cellHeight+1)+1) / tglp->sheetHeight;
        float tw = (float)cwi->glyphWidth / tglp->sheetWidth;
        float th = (float)tglp->cellHeight / tglp->sheetHeight;
        out->texcoord.left = tx;
        out->texcoord.top = ty;
        out->texcoord.right = tx+tw;
        out->texcoord.bottom = ty + th;
    }

    float Renderer::GetTextSize(const char *text)
    {
        if (!text) return (0.0f);

        float   w = 0.f;
        u8      *c = (u8 *)text;

        // Skip UTF8 sig
        if (c[0] == 0xEF && c[1] == 0xBB && c[2] == 0xBF)
            c += 3;

        while (*c == '\n') c++;

        do
        {
            if (!*c) break;

            if (*c == '\r' || *c == '\n')
            {
                c++;
                continue;
            }

            if (*c == 0x18)
            {
                c++;
                continue;
            }

            if (*c == 0x1B)
            {
                c += 4;
                continue;
            }

            if (*c == 0x11)
            {
                c++;
                u16 control = *(u16*)c;
                c += 2;
                if (control & 0x8000)
                {
                    u16 skipToX = control & 0x1FF;
                    if (skipToX > w)
                        w = skipToX;
                }
                continue;
            }

            Glyph *glyph = Font::GetGlyph(c);
            if (glyph == nullptr) break;

            w += glyph->Width();
        } while (true);
        return (w);
    }


    int Renderer::GetLineCount(const char *text, float maxWidth)
    {
        if (!text) return (0.0f);

        int     lineCount = 1;
        float   w = 0.0f;
        u8      *c = (u8 *)text;

        // Skip UTF8 sig
        if (c[0] == 0xEF && c[1] == 0xBB && c[2] == 0xBF)
            c += 3;

        while (*c)
        {
            if (*c == '\r')
            {
                c++;
                continue;
            }

            if (*c == '\n')
            {
                lineCount++;
                w = 0.f;
                c++;
                continue;
            }

            if (*c == 0x18)
            {
                c++;
                continue;
            }

            if (*c == 0x1B)
            {
                c += 4;
                continue;
            }

            if (*c == 0x11)
            {
                c++;
                u16 control = *(u16*)c;
                c += 2;
                if (control & 0x8000)
                {
                    u16 skipToX = control & 0x1FF;
                    if (skipToX > w && skipToX < maxWidth)
                        w = skipToX;
                }
                continue;
            }

            Glyph *glyph = Font::GetGlyph(c);

            if (glyph == nullptr)
                break;

            float gSize = glyph->Width();

            if (w + gSize > maxWidth)
            {
                lineCount++;
                w = gSize;
            }
            else
                w += gSize;
        }

        return (lineCount);
    }

    void Renderer::GetTextInfos(const char *text, int &lineCount, float &lineMaxWidth, float maxWidth)
    {
        lineCount = 0;
        lineMaxWidth = 0.f;

        if (!text || !*text) return;

        lineCount = 1;
        float   w = 0.0f;
        u8      *c = (u8 *)text;

        // Skip UTF8 sig
        if (c[0] == 0xEF && c[1] == 0xBB && c[2] == 0xBF)
            c += 3;

        while (*c)
        {
            if (*c == '\n')
            {
                lineCount++;
                if (w > lineMaxWidth)
                    lineMaxWidth = w;
                w = 0.f;
                c++;
                continue;
            }

            if (*c == 0x18)
            {
                c++;
                continue;
            }

            if (*c == 0x1B)
            {
                c += 4;
                continue;
            }

            if (*c == 0x11)
            {
                c++;
                u16 control = *(u16*)c;
                c += 2;
                if (control & 0x8000)
                {
                    u16 skipToX = control & 0x1FF;
                    if (skipToX > w && skipToX < maxWidth)
                        w = skipToX;
                }
                continue;
            }

            Glyph *glyph = Font::GetGlyph(c);

            if (glyph == nullptr)
                break;

            float gSize = glyph->Width();

            if (w + gSize > maxWidth)
            {
                lineCount++;
                if (w > lineMaxWidth)
                    lineMaxWidth = w;
                w = gSize;
            }
            else
                w += gSize;
        }
        if (w > lineMaxWidth)
            lineMaxWidth = w;
    }

    void Renderer::DrawSysCheckBox(const char *str, int posX, int &posY, int xLimits, Color color, bool isChecked,  float offset)
    {
        Icon::DrawCheckBox(posX, posY, isChecked);
        posX += 20;
        DrawSysString(str, posX, posY, xLimits, color, offset);
        posY += 1;

    }

    void Renderer::DrawSysFolder(const char *str, int posX, int &posY, int xLimits, Color color, float offset)
    {
        Icon::DrawFolder(posX, posY);
        posX += 20;
        DrawSysString(str, posX, posY, xLimits, color, offset);
        posY += 1;
    }

    int Renderer::DrawGlyph(Glyph *glyph, int posX, int posY, Color color, u32 flags)
    {
        return DrawGlyph(GetContext()->screen, glyph, posX, posY, color, flags);
    }

    int Renderer::DrawGlyph(ScreenImpl *screen, Glyph *glyph, int posX, int posY, Color color, u32 flags)
    {
        if (!screen || !glyph)
            return posY;

        posX += glyph->xOffset;

        u32  stride = screen->_stride;
        u32  bpp = screen->_bytesPerPixel;
        u8   *data = glyph->glyph;
        u8   *left = static_cast<u8 *>(screen->GetLeftFrameBuffer(posX, posY));
        u8   *fb = left;
        u8   italicOffset = (flags & Render::FontDrawMode::ITALIC) ? 3 : 0;
        u32  lineCount = 0;

        for (int i = 0; i < 208; i++)
        {
            if (i != 0 && i % 13 == 0)
            {
                if ((lineCount % 4) == 0 && flags &Render::FontDrawMode::ITALIC)
                    italicOffset--;
                lineCount++;
                left -= bpp;
                fb = left;
            }

            u32 alpha = data[i];

            // Don't waste time on pixels which are only 5% visible
            for (int j = 0; (alpha > 12) && j < ((flags & Render::FontDrawMode::BOLD) ? 2 : 1); j++)
            {
                color.a = alpha;
                Color &&l = PrivColor::FromFramebuffer(fb + stride * j + italicOffset * stride);
                Color &&c = l.Blend(color, Color::BlendMode::Alpha);

                PrivColor::ToFramebuffer(fb + stride * j + italicOffset * stride, c);
            }
            fb += stride;
        }

        return (posX + glyph->xAdvance);
    }

    int Renderer::DrawGlyph(ScreenImpl *screen, Glyph *glyph, int posX, int posY, float &offset, Color color, u32 flags)
    {
        if (!screen || !glyph)
            return posY;

        posX += glyph->xOffset;

        u32  stride = screen->_stride;
        u32  bpp = screen->_bytesPerPixel;
        u8   *data = glyph->glyph;
        u8   *left = static_cast<u8 *>(screen->GetLeftFrameBuffer(posX, posY));
        u8   *fb = left;
        u8   italicOffset = (flags & Render::FontDrawMode::ITALIC) ? 3 : 0;
        u32  lineCount = 0;

        for (int i = static_cast<int>(offset); i < 208; i++)
        {
            if (i != 0 && i % 13 == 0)
            {
                if (offset)
                    i += offset;
                if ((lineCount % 4) == 0 && flags & Render::FontDrawMode::ITALIC)
                    italicOffset--;
                lineCount++;
                left -= bpp;
                fb = left;
            }

            u32 alpha = data[i];

            // Don't waste time on pixels which are only 5% visible
            for (int j = 0; (alpha > 12) && j < ((flags & Render::FontDrawMode::BOLD) ? 2 : 1); j++)
            {
                color.a = alpha;
                Color &&l = PrivColor::FromFramebuffer(fb + stride * j + italicOffset * stride);
                Color &&c = l.Blend(color, Color::BlendMode::Alpha);

                PrivColor::ToFramebuffer(fb + stride * j + italicOffset * stride, c);
            }
            fb += stride;
        }
        if (offset > 0.f)
        {
            posX -= offset;
            offset = 0;
        }

        return (posX + glyph->xAdvance);
    }

    int Renderer::DrawSysStringReturn(const unsigned char *stri, int posX, int& posY, int xLimits, Color color, int maxY, u32 flags)
    {
        // Check for a valid pointer
        if (!(stri && *stri))
            return (posX);
        int             lineCount = 1;
        u8              *str = const_cast<u8 *>(stri);
        int             x = posX;
        int             underLineStart = -1, strikeLineStart = -1;
        ScreenImpl      *screen = GetContext()->screen;
        void (*lineDrawer)(int posX, int posY, int width, const Color &color, int height) = Renderer::DrawLine;

        if (!screen)
            return posY;

        xLimits = std::min(xLimits, (GetContext()->target == TOP ? 400 : 320));

        // Skip UTF8 sig
        if (str[0] == 0xEF && str[1] == 0xBB && str[2] == 0xBF)
            str += 3;

        do
        {
            if (posY >= maxY)
                break;

            u8 c = *str;

            if (c == '\r')
                str++;
            if (c == '\n')
            {
                if (flags & Render::FontDrawMode::UNDERLINE) {
                    if (underLineStart != x)
                        lineDrawer(underLineStart, posY + 15, x - underLineStart, color, 1);
                    underLineStart = posX;
                }
                if (flags & Render::FontDrawMode::STRIKETHROUGH) {
                    if (strikeLineStart != x)
                        lineDrawer(strikeLineStart, posY + 9, x - strikeLineStart, color, 1);
                    strikeLineStart = posX;
                }
                x = posX;
                lineCount++;
                posY += 16;
                str++;
                continue;
            }

            if (c == 0x18)
            {
                color = Preferences::Settings.MainTextColor;
                str++;
                continue;
            }

            if (c == 0x1B)
            {
                str++;
                color.r = *str++;
                color.g = *str++;
                color.b = *str++;
                continue;
            }

            if (c == 0x11)
            {
                str++;
                u16 control = *(u16*)str;
                str += 2;
                if (control & 0x8000)
                {
                    u16 skipToX = control & 0x1FF;
                    if (skipToX > x && skipToX < xLimits) {
                        if (underLineStart == posX && x == posX) // Fix for aligned text after newline
                            underLineStart = skipToX;
                        if (strikeLineStart == posX && x == posX) // Fix for aligned text after newline
                            strikeLineStart = skipToX;
                        x = skipToX;
                    }
                }
                else if (control & 0x4000)
                {
                    u32 prevFlags = flags;
                    flags ^= (control & 0x7F);

                    if (flags & Render::LINEDOTTED)
                        lineDrawer = DrawStippledLine;
                    else
                        lineDrawer = DrawLine;

                    if ((prevFlags & Render::FontDrawMode::UNDERLINE) != (flags & Render::FontDrawMode::UNDERLINE)) {
                        bool startLine = flags & Render::FontDrawMode::UNDERLINE;
                        if (startLine)
                            underLineStart = x;
                        else
                        {
                            if (underLineStart != x)
                                lineDrawer(underLineStart, posY + 15, x - underLineStart, color, 1);
                            underLineStart = -1;
                        }

                    }
                    if ((prevFlags & Render::FontDrawMode::STRIKETHROUGH) != (flags & Render::FontDrawMode::STRIKETHROUGH)) {
                        bool startLine = flags & Render::FontDrawMode::STRIKETHROUGH;
                        if (startLine)
                            strikeLineStart = x;
                        else
                        {
                            if (strikeLineStart != x)
                                lineDrawer(strikeLineStart, posY + 9, x - strikeLineStart, color, 1);
                            strikeLineStart = -1;
                        }

                    }
                }
                continue;
            }

            Glyph *glyph = Font::GetGlyph(str);

            if (glyph == nullptr)
                break;

            if (x + glyph->Width() >= xLimits)
            {
                if (flags & Render::FontDrawMode::UNDERLINE) {
                    if (underLineStart != x)
                        lineDrawer(underLineStart, posY + 15, x - underLineStart, color, 1);
                    underLineStart = posX;
                }
                if (flags & Render::FontDrawMode::STRIKETHROUGH) {
                    if (strikeLineStart != x)
                        lineDrawer(strikeLineStart, posY + 9, x - strikeLineStart, color, 1);
                    strikeLineStart = posX;
                }
                x = posX;
                lineCount++;
                posY += 16;
            }

            x = DrawGlyph(screen, glyph, x, posY, color, flags);

        } while (*str);

        if (posY < maxY && flags & Render::FontDrawMode::UNDERLINE) {
            if (underLineStart != x)
                lineDrawer(underLineStart, posY + 15, x - underLineStart, color, 1);
        }
        if (posY < maxY && flags & Render::FontDrawMode::STRIKETHROUGH) {
            if (strikeLineStart != x)
                lineDrawer(strikeLineStart, posY + 9, x - strikeLineStart, color, 1);
        }

        posY += 16;
        return (x);
    }

    namespace RendererPriv
    {
        Color   g_customColor;
    }

    int Renderer::DrawSysString(const char *stri, int posX, int &posY, int xLimits, Color color, float offset, const char *end, u32 flags)
    {
        Glyph   *glyph;
        int      x = posX;
        //u8      *str = (u8 *)stri.c_str();
        u8 *str = (u8 *)stri;
        int             underLineStart = -1, strikeLineStart = -1;
        void            (*lineDrawer)(int posX, int posY, int width, const Color &color, int height) = DrawLine;
        ScreenImpl *screen = GetContext()->screen;

        if (!(str && *str) || !screen)
            return (x);

        xLimits = std::min(xLimits, (GetContext()->target == TOP ? 400 : 320));

        // Skip UTF8 sig
        if (str[0] == 0xEF && str[1] == 0xBB && str[2] == 0xBF)
            str += 3;

        do
        {
            u8 c = *str;

            if (x >= xLimits || str == (u8 *)end)
            {
                break;
            }

            if (c == '\n' || c == '\r')
            {
                str++;
                continue;
            }

            if (c == 0x18)
            {
                RendererPriv::g_customColor = color = Preferences::Settings.MainTextColor;
                str++;
                continue;
            }

            if (c == 0x1B)
            {
                str++;
                color.r = *str++;
                color.g = *str++;
                color.b = *str++;
                RendererPriv::g_customColor = color;
                continue;
            }

            if (c == 0x11)
            {
                str++;
                u16 control = *(u16*)str;
                str += 2;
                if (control & 0x8000)
                {
                    u16 skipToX = control & 0x1FF;
                    if (skipToX > x && skipToX < xLimits)
                        x = skipToX;
                }
                else if (control & 0x4000)
                {
                    u32 prevFlags = flags;
                    flags ^= (control & 0x7F);

                    if (flags & Render::LINEDOTTED)
                        lineDrawer = DrawStippledLine;
                    else
                        lineDrawer = DrawLine;

                    if ((prevFlags & Render::FontDrawMode::UNDERLINE) != (flags & Render::FontDrawMode::UNDERLINE)) {
                        bool startLine = flags & Render::FontDrawMode::UNDERLINE;
                        if (startLine)
                            underLineStart = x;
                        else
                        {
                            if (underLineStart != x)
                                lineDrawer(underLineStart, posY + 15, x - underLineStart, color, 1);
                            underLineStart = -1;
                        }

                    }
                    if ((prevFlags & Render::FontDrawMode::STRIKETHROUGH) != (flags & Render::FontDrawMode::STRIKETHROUGH)) {
                        bool startLine = flags & Render::FontDrawMode::STRIKETHROUGH;
                        if (startLine)
                            strikeLineStart = x;
                        else
                        {
                            if (strikeLineStart != x)
                                lineDrawer(strikeLineStart, posY + 9, x - strikeLineStart, color, 1);
                            strikeLineStart = -1;
                        }

                    }
                }
                continue;
            }

            if (posY >= 240)
                break;

            glyph = Font::GetGlyph(str);

            if (glyph == nullptr)
                break;

            float gWidth = glyph->Width();

            if (x + gWidth > xLimits)
            {
                break;
            }

            if (offset >= gWidth)
            {
                offset -= (gWidth);
                continue;
            }

            x = DrawGlyph(screen, glyph, x, posY, offset, color, flags);
        } while (*str);

        if (flags & Render::FontDrawMode::UNDERLINE) {
            if (underLineStart != x)
                lineDrawer(underLineStart, posY + 15, x - underLineStart, color, 1);
        }
        if (flags & Render::FontDrawMode::STRIKETHROUGH) {
            if (strikeLineStart != x)
                lineDrawer(strikeLineStart, posY + 9, x - strikeLineStart, color, 1);
        }

        posY += 16;
        return (x);
    }
}
