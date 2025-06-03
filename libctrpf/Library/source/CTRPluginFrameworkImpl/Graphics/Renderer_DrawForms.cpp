#include "types.h"

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFramework/Graphics.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "3ds.h"
#include <algorithm>
#include <queue>

namespace CTRPluginFramework
{
    inline u32   GetFramebufferOffset(int posX, int posY, int bpp, int rowsize)
    {
        return ((rowsize - 1 - posY + posX * rowsize) * bpp);
    }

    static void        DrawLineV(u32 fmt, const Color& color, u8 *dst, u32 height)
    {
        switch (fmt)
        {
        case GSP_RGBA8_OES:
        {
            //u32 *dst32 = (u32 *)dst;
            while (height--)
            {
                *dst++ = color.a;
                *dst++ = color.b;
                *dst++ = color.g;
                *dst++ = color.r;

                //*dst32++ = color.raw;
            }
            break;
        }
        case GSP_BGR8_OES:
        {
            while (height--)
            {
                *dst++ = color.b;
                *dst++ = color.g;
                *dst++ = color.r;
            }
            break;
        }
        case GSP_RGB565_OES:
        {
            union
            {
                u16     u;
                char    b[2];
            }           half;

            half.u  = (color.r & 0xF8) << 8;
            half.u |= (color.g & 0xFC) << 3;
            half.u |= (color.b & 0xF8) >> 3;

            while (height--)
            {
                *(dst++) = half.b[0];
                *(dst++) = half.b[1];
            }
            break;
        }
        case GSP_RGB5_A1_OES:
        {
            union
            {
                u16     u;
                char    b[2];
            }           half;

            half.u  = (color.r & 0xF8) << 8;
            half.u |= (color.g & 0xF8) << 3;
            half.u |= (color.b & 0xF8) >> 2;
            half.u |= 1;

            while (height--)
            {
                *(dst++) = half.b[0];
                *(dst++) = half.b[1];
            }

            break;
        }
        case GSP_RGBA4_OES:
        {
            union
            {
                u16     u;
                char    b[2];
            }           half;

            half.u  = (color.r & 0xF0) << 8;
            half.u |= (color.g & 0xF0) << 4;
            half.u |= (color.b & 0xF0);
            half.u |= 0x0F;

            while (height--)
            {
                *(dst++) = half.b[0];
                *(dst++) = half.b[1];
            }
            break;
        }
        default:
            break;
        }
    }

    static void        DrawLineV2(u32 fmt, const Color& color1, const Color& color2, u8 *dst, u32 height)
    {
        union UColor
        {
            u16     u;
            char    b[2];
        };

        switch (fmt)
        {
        case GSP_RGBA8_OES:
        {
            u32 *dst32 = (u32 *)dst;
            while (height--)
            {
                const Color& color = height & 1 ? color1 : color2;
                *dst32++ = color.raw;
            }
            break;
        }
        case GSP_BGR8_OES:
        {
            while (height--)
            {
                const Color& color = height & 1 ? color1 : color2;
                *dst++ = color.b;
                *dst++ = color.g;
                *dst++ = color.r;
            }
            break;
        }
        case GSP_RGB565_OES:
        {
            UColor c1;
            UColor c2;

            c1.u  = (color1.r & 0xF8) << 8;
            c1.u |= (color1.g & 0xFC) << 3;
            c1.u |= (color1.b & 0xF8) >> 3;

            c2.u  = (color2.r & 0xF8) << 8;
            c2.u |= (color2.g & 0xFC) << 3;
            c2.u |= (color2.b & 0xF8) >> 3;

            while (height--)
            {
                const UColor& c = height & 1 ? c1 : c2;
                *(dst++) = c.b[0];
                *(dst++) = c.b[1];
            }
            break;
        }
        case GSP_RGB5_A1_OES:
        {
            UColor c1;
            UColor c2;

            c1.u  = (color1.r & 0xF8) << 8;
            c1.u |= (color1.g & 0xF8) << 3;
            c1.u |= (color1.b & 0xF8) >> 2;
            c1.u |= 1;

            c2.u  = (color2.r & 0xF8) << 8;
            c2.u |= (color2.g & 0xF8) << 3;
            c2.u |= (color2.b & 0xF8) >> 2;
            c2.u |= 1;

            while (height--)
            {
                const UColor& c = height & 1 ? c1 : c2;
                *(dst++) = c.b[0];
                *(dst++) = c.b[1];
            }

            break;
        }
        case GSP_RGBA4_OES:
        {
            UColor c1;
            UColor c2;

            c1.u  = (color1.r & 0xF0) << 8;
            c1.u |= (color1.g & 0xF0) << 4;
            c1.u |= (color1.b & 0xF0);
            c1.u |= 0x0F;

            c2.u  = (color2.r & 0xF0) << 8;
            c2.u |= (color2.g & 0xF0) << 4;
            c2.u |= (color2.b & 0xF0);
            c2.u |= 0x0F;

            while (height--)
            {
                const UColor& c = height & 1 ? c1 : c2;
                *(dst++) = c.b[0];
                *(dst++) = c.b[1];
            }
            break;
        }
        default:
            break;
        }
    }

    void        Renderer::DrawLine(int posX, int posY, int width, const Color &color, int height)
    {
        if (posY < 0)
        {
            if (posY + height < 0)
                return;
            height += posY;
            posY = 0;
        }

        while (posY + height > 240)
        {
            if (height <= 0)
                return;
            --height;
        }

        // Clamp check
        if (PrivColor::_useClamp)
        {
            const IntRect& clampArea = PrivColor::_clampArea;

            // Top
            if (posY < clampArea.leftTop.y)
            {
                int diff = clampArea.leftTop.y - posY;

                posY += diff;
                height -= diff;

                if (height <= 0)
                    return;
            }

            // Bottom
            int borderY = clampArea.leftTop.y + clampArea.size.y;
            if (posY + height > borderY)
            {
                height = borderY - posY;
            }

            // Left
            if (posX < clampArea.leftTop.x)
            {
                int diff = clampArea.leftTop.x - posX;

                posX += diff;
                width -= diff;

                if (diff <= 0)
                    return;
            }

            // Right
            int borderX = clampArea.leftTop.x + clampArea.size.x;
            if (posX + width > borderX)
            {
                width = borderX - posX;
            }
        }

        ScreenImpl  *screen = GetContext()->screen;
        u8          *dst = screen->GetLeftFrameBuffer(posX, posY + height - 1);
        u32         stride = screen->GetStride();
        u32         fmt = screen->GetFormat();
        //u32         size = screen->GetBytesPerPixel() * height;

        while (width-- > 0)
        {
            DrawLineV(fmt, color, dst, height);
            dst += stride;
        }
    }

    void        Renderer::DrawLine(const IntVector &start, const IntVector &end, const Color &color)
    {
        int posX = start.x;
        int posY = start.y;
        int width = end.x - posX; // 50 - 10 = 40
        int height = 1 + end.y - posY;// 1

        while (posY + height > 240)
        {
            if (height <= 0)
                return;
            --height;
        }

        // Clamp check
        if (PrivColor::_useClamp)
        {
            const IntRect& clampArea = PrivColor::_clampArea;

            // Top
            if (posY < clampArea.leftTop.y)
            {
                int diff = clampArea.leftTop.y - posY;

                posY += diff;
                height -= diff;

                if (height <= 0)
                    return;
            }

            // Bottom
            int borderY = clampArea.leftTop.y + clampArea.size.y;
            if (posY + height > borderY)
            {
                height = borderY - posY;
            }

            // Left
            if (posX < clampArea.leftTop.x)
            {
                int diff = clampArea.leftTop.x - posX;

                posX += diff;
                width -= diff;

                if (diff <= 0)
                    return;
            }

            // Right
            int borderX = clampArea.leftTop.x + clampArea.size.x;
            if (posX + width > borderX)
            {
                width = borderX - posX;
            }
        }

        ScreenImpl  *screen = GetContext()->screen;
        u8          *dst = screen->GetLeftFrameBuffer(posX, posY + height - 1);
        u32         stride = screen->GetStride();
        u32         fmt = screen->GetFormat();

        while (width-- > 0)
        {
            DrawLineV(fmt, color, dst, height);
            dst += stride;
        }
    }

    void        Renderer::DrawStippledLine(int posX, int posY, int width, const Color &color, int height)
    {
        if (posY < 0)
        {
            if (posY + height < 0)
                return;
            height += posY;
            posY = 0;
        }
        while (posY + height > 240)
        {
            if (height <= 0)
                return;
            --height;
        }

        ScreenImpl  *screen = GetContext()->screen;
        u8          *dst = screen->GetLeftFrameBuffer(posX, posY + height - 1);
        u32         stride = screen->GetStride();
        int         pitch = 5;

        while (width-- > 0)
        {
            u8 *dd = dst;
            if (pitch > 0)
            {
                for (int y = 0; y < height; ++y)
                    dd = PrivColor::ToFramebuffer(dd, color);
            }
            --pitch;

            if (pitch <= -5)
                pitch = 5;

            dst += stride;
        }
    }
#if GEOMETRY
    void        Renderer::Arc(int posX, int posY, int r, Color color)
    {
        int d;
        int y;
        int x = 0;
        y = r;
        d = 1 - r;
        _DrawPixel(x,y, color);
        while ( y > x )
        {
            if ( d < 0 )
                d += 2 * x + 3 ;
            else
            {
                d += 2 * (x - y) + 5 ;
                y--;
            }
            x++ ;
            _DrawPixel(x + posX, posY + (r - y), color) ;
        }
    }

    void Renderer::Ellipse(int posX, int posY, long a, long b, Color color)
    {
        int x;
        int y;
        double d1;
        double d2;
        x = 0;
        y = b;
        d1 = b * b - a * a * b + a * a / 4;
        _DrawPixel(posX, posY, color);
        while ( a * a * (y - .5) > b * b * (x + 1))
        {
            if ( d1 < 0 )
            {
                d1 += b * b * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
                x++;
                y--;
            }
            _DrawPixel(posX + x, posY + (b - y), color);
        }
        d2 = b * b * (x + .5) * (x + .5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
        while ( y > 0 )
        {
            if ( d2 < 0 )
            {
                d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
                y-- ;
                x++ ;
            }
            else
            {
                d2 += a * a * (-2 * y + 3);
                y--;
            }
            _DrawPixel(posX + x, posY + (b - y), color);
        }
    }

    void Renderer::EllipseIncomplete(int posX, int posY, float a, float b, int max, int aff, Color color)
    {
        int     x;
        int     y;
        float   d1;
        int     d2;

        x = 0;
        y = b;
        d1 = b * b - a * a * b + a * a / 4;
        _DrawPixel(posX + x, posY, color);
        _DrawPixel(posX + x, posY + (b - y), color);
        _DrawPixel(posX + x, posY - (b - y), color);
        _DrawPixel(posX - x, posY - (b - y), color);
        _DrawPixel(posX - x, posY + (b - y), color);
        int cpt = 0;

        while ((a * a * (y - .5) > b * b * (x + 1)) && (cpt < max))
        {
            cpt++;
            if (d1 < 0)
            {
                d1 += b * b * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
                x++;
                y--;
            }
            _DrawPixel(posX + x, posY + (b - y), color);
            _DrawPixel(posX + x, posY - (b - y), color);
            _DrawPixel(posX - x, posY - (b - y), color);
            _DrawPixel(posX - x, posY + (b - y), color);
        }
        d2 =(float)(b * b * (x + .5) * (x + .5) + a * a * (y - 1) * (y - 1) - a * a * b * b);
        while ((y > 0 ) && (cpt < max ))
        {
            cpt++;
            if (d2 < 0)
            {
                d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
                y--;
                x++;
            }
            else
            {
                d2 += a * a * (-2 * y + 3);
                y--;
            }
            _DrawPixel(posX + x, posY + (b - y), color);
            _DrawPixel(posX + x, posY - (b - y), color);
            _DrawPixel(posX - x, posY - (b - y), color);
            _DrawPixel(posX - x, posY + (b - y), color);        }
    }

    void Renderer::RoundedRectangle(const IntRect &rect, float radius, int max, Color color, bool mustFill, Color fillColor)
    {
        int     x;
        int     y;
        float   d1;
        int     d2;

        using Point = IntVector;

        std::queue<Point> points;

        int     width = rect.size.x;
        int     height = rect.size.y;

        int     posX = rect.leftTop.x;
        int     posY = rect.leftTop.y;

        // From negative rectangle to positive
        if (width < 0)
        {
            width = -width;
            posX -= width;
        }
        if (height < 0)
        {
            height = -height;
            posY -= height;
        }
        int cpt = 0;

        x = 0;
        y = radius;
        d1 = radius * radius - radius * radius * radius + radius * radius / 4;

        float d1Bak = d1;

        while ((radius * radius * (radius - .5) > radius * radius * (x + 1)) && (cpt < max))
        {
            cpt++;
            if (d1 < 0)
            {
                d1 += radius * radius * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += radius * radius * (2 * x + 3) + radius * radius * (-2 * y + 2);
                x++;
                y--;
            }
        }
        while ((y > 0 ) && (cpt < max ))
        {
            cpt++;
            if (d2 < 0)
            {
                d2 += radius * radius * (2 * x + 2) + radius * radius * (-2 * y + 3);
                y--;
                x++;
            }
            else
            {
                d2 += radius * radius * (-2 * y + 3);
                y--;
            }
        }
        d1 = d1Bak;
        int rHeight = radius - y;
        int rWidth = x;

        y = radius;
        x = 0;

        width -= rWidth;
        {
            int posXX;
            int posYY;


            // Left Top Corner
            posXX = posX - x + rWidth;
            posYY = posY + (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX + 1, posYY));

            // Right Top Corner
            posXX = posX + x + width;
            posYY = posY + (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX - 1, posYY));

            // Left Bottom Corner
            posXX = posX - x + rWidth;
            posYY = posY - (radius - y) + height;
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX + 1, posYY));

            // Right Bottom Corner
            posXX = posX + x + width;
            posYY = posY + height - (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX - 1, posYY));

        }
        cpt = 0;

        while ((radius * radius * (radius - .5) > radius * radius * (x + 1)) && (cpt < max))
        {
            cpt++;
            if (d1 < 0)
            {
                d1 += radius * radius * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += radius * radius * (2 * x + 3) + radius * radius * (-2 * y + 2);
                x++;
                y--;
            }
            int posXX;
            int posYY;


            // Left Top Corner
            posXX = posX - x + rWidth;
            posYY = posY + (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX + 1, posYY));

            // Right Top Corner
            posXX = posX + x + width;
            posYY = posY + (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX - 1, posYY));

            // Left Bottom Corner
            posXX = posX - x + rWidth;
            posYY = posY - (radius - y) + height;
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX + 1, posYY));

            // Right Bottom Corner
            posXX = posX + x + width;
            posYY = posY + height - (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX - 1, posYY));
        }
        d2 =(float)(radius * radius * (x + .5) * (x + .5) + radius * radius * (y - 1) * (y - 1) - radius * radius * radius * radius);
        while ((y > 0 ) && (cpt < max ))
        {
            cpt++;
            if (d2 < 0)
            {
                d2 += radius * radius * (2 * x + 2) + radius * radius * (-2 * y + 3);
                y--;
                x++;
            }
            else
            {
                d2 += radius * radius * (-2 * y + 3);
                y--;
            }
            int posXX;
            int posYY;

            // Left Top Corner
            posXX = posX - x + rWidth;
            posYY = posY + (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX + 1, posYY));

            // Right Top Corner
            posXX = posX + x + width;
            posYY = posY + (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX - 1, posYY));

            // Left Bottom Corner
            posXX = posX - x + rWidth;
            posYY = posY - (radius - y) + height;
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX + 1, posYY));

            // Right Bottom Corner
            posXX = posX + x + width;
            posYY = posY + height - (radius - y);
            _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX - 1, posYY));
        }

        // Top line
        DrawLine(posX + rWidth, posY - 1, width - rWidth, color);
        // Bottom line
        DrawLine(posX + rWidth, posY + height - 1, width - rWidth, color);
        // Left line
        DrawLine(posX - x + rWidth, posY + rHeight - 1, 1, color, height - (rHeight* 2));
        // Right line
        DrawLine(posX + x + width, posY + rHeight, 1, color, height - (rHeight * 2));

        if (mustFill)
        {
            while (!points.empty())
            {
                Point left = points.front(); points.pop();
                Point right = points.front(); points.pop();

                DrawLine(left, right, fillColor);
            }

            int posXX = posX - x + rWidth + 1;
            int posYY = posY + rHeight - 1;
            int wwidth = x + width - 1;
            int hheight = height - (rHeight* 2);
            DrawLine(posXX, posYY, wwidth, fillColor, hheight);


            /*IntVector start = rect._leftTopCorner;
            IntRect area = rect;
            area._leftTopCorner.x -= 1;
            area._size.x += 2;

            start.x += rWidth + 1;
            start.y++;

            FormFiller(start, area, true, fillColor, color);    */
        }
    }
#endif
    void        Renderer::ComputeRoundedRectangle(std::vector<IntLine> &out, const IntRect &rect, float radius, int max)
    {
        int     x;
        int     y;
        float   d1;
        int     d2 = 0;

        using Point = IntVector;

        std::queue<Point> points;

        int     width = rect.size.x;
        int     height = rect.size.y;

        int     posX = rect.leftTop.x;
        int     posY = rect.leftTop.y;

        int     posYBak;


        if (width < 0)
        {
            width = -width;
            posX -= width;
        }

        if (height < 0)
        {
            height = -height;
            posY -= height;
        }

        posYBak = posY;

        // Correct posY
        //posY += (_rowSize[_target] - 240);

        int cpt = 0;

        x = 0;
        y = radius;
        d1 = radius * radius - radius * radius * radius + radius * radius / 4;

        float d1Bak = d1;

        while ((radius * radius * (radius - .5) > radius * radius * (x + 1)) && (cpt < max))
        {
            cpt++;
            if (d1 < 0)
            {
                d1 += radius * radius * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += radius * radius * (2 * x + 3) + radius * radius * (-2 * y + 2);
                x++;
                y--;
            }
        }
        while ((y > 0 ) && (cpt < max ))
        {
            cpt++;
            if (d2 < 0)
            {
                d2 += radius * radius * (2 * x + 2) + radius * radius * (-2 * y + 3);
                y--;
                x++;
            }
            else
            {
                d2 += radius * radius * (-2 * y + 3);
                y--;
            }
        }
        d1 = d1Bak;
        int rHeight = radius - y;
        int rWidth = x;

        y = radius;
        x = 0;

        width -= rWidth;
        {
            int posXX;
            int posYY;


            // Left Top Corner
            posXX = posX - x + rWidth;
            posYY = posY + (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Right Top Corner
            posXX = posX + x + width;
            posYY = posY + (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Left Bottom Corner
            posXX = posX - x + rWidth;
            posYY = posY - (radius - y) + height;
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Right Bottom Corner
            posXX = posX + x + width;
            posYY = posY + height - (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

        }
        cpt = 0;

        while ((radius * radius * (radius - .5) > radius * radius * (x + 1)) && (cpt < max))
        {
            cpt++;
            if (d1 < 0)
            {
                d1 += radius * radius * (2 * x + 3);
                x++;
            }
            else
            {
                d1 += radius * radius * (2 * x + 3) + radius * radius * (-2 * y + 2);
                x++;
                y--;
            }
            int posXX;
            int posYY;


            // Left Top Corner
            posXX = posX - x + rWidth;
            posYY = posY + (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Right Top Corner
            posXX = posX + x + width;
            posYY = posY + (radius - y);
            // _DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Left Bottom Corner
            posXX = posX - x + rWidth;
            posYY = posY - (radius - y) + height;
            ///_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Right Bottom Corner
            posXX = posX + x + width;
            posYY = posY + height - (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));
        }
        d2 =(float)(radius * radius * (x + .5) * (x + .5) + radius * radius * (y - 1) * (y - 1) - radius * radius * radius * radius);
        while ((y > 0 ) && (cpt < max ))
        {
            cpt++;
            if (d2 < 0)
            {
                d2 += radius * radius * (2 * x + 2) + radius * radius * (-2 * y + 3);
                y--;
                x++;
            }
            else
            {
                d2 += radius * radius * (-2 * y + 3);
                y--;
            }
            int posXX;
            int posYY;

            // Left Top Corner
            posXX = posX - x + rWidth;
            posYY = posY + (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Right Top Corner
            posXX = posX + x + width;
            posYY = posY + (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Left Bottom Corner
            posXX = posX - x + rWidth;
            posYY = posY - (radius - y) + height;
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));

            // Right Bottom Corner
            posXX = posX + x + width;
            posYY = posY + height - (radius - y);
            //_DrawPixel(posXX, posYY, color);
            points.push(Point(posXX, posYY));
        }

        // Top line
        //DrawLine(posX + rWidth, posYBak - 1, width - rWidth, color);
        // Bottom line
        //DrawLine(posX + rWidth, posYBak + height - 1, width - rWidth, color);
        // Left line
        //DrawLine(posX - x + rWidth, posYBak + rHeight - 1, 1, color, height - (rHeight* 2));
        // Right line
        //DrawLine(posX + x + width, posYBak + rHeight, 1, color, height - (rHeight * 2));

        while (!points.empty())
        {
                Point left = points.front(); points.pop();
                Point right = points.front(); points.pop();
                out.push_back(IntLine(left, right));
                //DrawLine(left, right, fillColor);
        }

        // Top Line
        out.push_back(IntLine(posX + rWidth - 1, posYBak, width - rWidth + 3, 1));
        // Bottom Line
        out.push_back(IntLine(posX + rWidth - 1, posYBak + height, width - rWidth + 3, 1));
        // Left Line
        out.push_back(IntLine(posX - x + rWidth, posYBak + rHeight, 1, height - (rHeight* 2)));
        // Right Line
        out.push_back(IntLine(posX + x + width, posYBak + rHeight, 1, height - (rHeight * 2)));

        int posXX = posX - x + rWidth + 1;
        int posYY = posYBak + rHeight;
        int wwidth = x + width - 1;
        int hheight = height - (rHeight* 2);

        out.push_back(IntLine(posXX, posYY, wwidth, hheight));
            //DrawLine(posXX, posYY, wwidth, fillColor, hheight);

    }

    void        Renderer::DrawRoundedRectangle(const IntRect &rect, const Color &border, const Color &fill)
    {
        static std::vector<IntLine>     _lines;

        if (_lines.empty())
            ComputeRoundedRectangle(_lines, IntRect(0, 0, rect.size.x, rect.size.y), 7.f, 50);

        u32     bMax = _lines.size() - 5;
        u32     i;
        u32     posX = rect.leftTop.x;
        u32     posY = rect.leftTop.y;

        for (i = 0; i < bMax; ++i)
        {
            IntLine &line = _lines[i];

            // Draw border
            Renderer::DrawPixel(posX + line.start.x, posY + line.start.y, border);
            Renderer::DrawPixel(posX + line.end.x, posY + line.end.y, border);

            // Fill line
            Renderer::DrawLine(posX + 1 + line.start.x, posY + line.start.y, line.end.x - line.start.x - 1, fill);
        }

        for (; i < _lines.size() - 1; ++i)
        {
            IntLine &line = _lines[i];

            Renderer::DrawLine(posX + line.start.x, posY + line.start.y, line.end.x, border, line.end.y);
        }

        IntLine &line = _lines[i];
        Renderer::DrawLine(posX + line.start.x, posY + line.start.y, line.end.x, fill, line.end.y);
    }
    void        Renderer::DrawRect(int posX, int posY, int width, int height, const Color &color, bool fill, int thickness)
    {
        if (fill)
        {
            DrawLine(posX, posY, width, color, height);
        }
        else
        {
            // Top line
            DrawLine(posX, posY, width, color, thickness);
            // Bottom line
            DrawLine(posX, posY + height - thickness, width, color, thickness);
            // Left line
            DrawLine(posX, posY, thickness, color, height);
            // Right line
            DrawLine(posX + width - thickness, posY, thickness, color, height);
        }
    }

    void        Renderer::DrawRect(const IntRect &rect, const Color &color, bool fill)
    {
        if (fill)
        {
            DrawLine(rect.leftTop.x, rect.leftTop.y, rect.size.x, color, rect.size.y);
        }
        else
        {
            int posX = rect.leftTop.x;
            int posY = rect.leftTop.y;
            int width = rect.size.x;
            int height = rect.size.y;
            // Top line
            DrawLine(posX, posY, width, color, 1);
            // Bottom line
            DrawLine(posX, posY + height - 1, width, color, 1);
            // Left line
            DrawLine(posX, posY, 1, color, height);
            // Right line
            DrawLine(posX + width - 1, posY, 1, color, height);
        }
    }

    void        Renderer::DrawRect2(const IntRect &rect, const Color &color1, const Color &color2)
    {
        int height = rect.size.y;

        int posX = rect.leftTop.x;
        int posY = rect.leftTop.y;
        int width = rect.size.x;

        ScreenImpl  *screen = GetContext()->screen;
        u8          *dst = screen->GetLeftFrameBuffer(posX, posY + height - 1);
        u32         stride = screen->GetStride();
        u32         fmt = screen->GetFormat();

        while (width-- > 0)
        {
            DrawLineV2(fmt, color1, color2, dst, height);
            dst += stride;
        }
    }
}
