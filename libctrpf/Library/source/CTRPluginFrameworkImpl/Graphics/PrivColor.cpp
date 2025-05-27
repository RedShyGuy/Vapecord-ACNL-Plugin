#include "types.h"

#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "CTRPluginFrameworkImpl/Graphics/PrivColor.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    thread_local FCPointer   PrivColor::FromFramebuffer = _ReadBGR8;
    thread_local F8Pointer   PrivColor::ToFramebuffer = _WriteBGR8;
    thread_local bool        PrivColor::_useClamp = false;
    thread_local IntRect     PrivColor::_clampArea;
    thread_local GSPGPU_FramebufferFormat PrivColor::_format = GSP_BGR8_OES;

    void    PrivColor::UseClamp(bool willUse)
    {
        _useClamp = willUse;
        SetFormat(_format);
    }

    void    PrivColor::UseClamp(bool willUse, const IntRect& rect)
    {
        _useClamp = willUse;
        _clampArea = rect;
        SetFormat(_format);
    }

    GSPGPU_FramebufferFormat PrivColor::GetFormat()
    {
        return (_format);
    }

    /*
    ** Private
    *****************/
    void    PrivColor::SetFormat(GSPGPU_FramebufferFormat format)
    {
        _format = format;
        switch (format)
        {
            case GSP_RGBA8_OES:
                FromFramebuffer = _ReadRGBA8;
                ToFramebuffer = _useClamp ? _WriteRGBA8Clamp : _WriteRGBA8;
                break;
            case GSP_BGR8_OES:
                FromFramebuffer = _ReadBGR8;
                ToFramebuffer = _useClamp ? _WriteBGR8Clamp : _WriteBGR8;
                break;
            case GSP_RGB565_OES:
                FromFramebuffer = _ReadRGB565;
                ToFramebuffer = _useClamp ? _WriteRGB565Clamp : _WriteRGB565;
                break;
            case GSP_RGB5_A1_OES:
                FromFramebuffer = _ReadRGB5A1;
                ToFramebuffer = _useClamp ? _WriteRGB5A1Clamp : _WriteRGB5A1;
                break;
            case GSP_RGBA4_OES:
                FromFramebuffer = _ReadRGBA4;
                ToFramebuffer = _useClamp ? _WriteRGBA4Clamp : _WriteRGBA4;
                break;
        }
    }

    Color   PrivColor::_ReadRGBA8(u8 *src)
    {
        Color color;

        color.a = *src++;
        color.b = *src++;
        color.g = *src++;
        color.r = *src;
        return (color);
    }

    Color   PrivColor::_ReadBGR8(u8 *src)
    {
        Color color;

        color.a = 255;
        color.b = *src++;
        color.g = *src++;
        color.r = *src;
        return (color);
    }

    Color   PrivColor::_ReadRGB565(u8 *src)
    {
        union
        {
            u16     u;
            u8      b[2];
        }           half;
        Color       color;


        //half.b[0] = *src++;
        //half.b[1] = *src;
        half.u = *reinterpret_cast<u16 *>(src);

        color.a = 255;
        color.r = (half.u >> 8) & 0xF8;
        color.g = (half.u >> 3) & 0xFC;
        color.b = (half.u << 3) & 0xF8;
        return (color);
    }

    Color   PrivColor::_ReadRGB5A1(u8 *src)
    {
        union
        {
            u16     u;
            u8      b[2];
        }           half;
        Color       color;

        half.b[0] = *src++;
        half.b[1] = *src;

        color.a = 255;
        color.r = (half.u >> 8) & 0xF8;
        color.g = (half.u >> 3) & 0xF8;
        color.b = (half.u << 2) & 0xF8;
        return (color);
    }

    Color   PrivColor::_ReadRGBA4(u8 *src)
    {
        union
        {
            u16     u;
            u8      b[2];
        }           half;
        Color       color;

        half.b[0] = *src++;
        half.b[1] = *src;

        color.a = 255;
        color.r = (half.u >> 8) & 0xF0;
        color.g = (half.u >> 4) & 0xF0;
        color.b = half.u & 0xF0;
        return (color);
    }

    u8      *PrivColor::_WriteRGBA8(u8 *dst, const Color &color)
    {
        *dst++ = color.a;
        *dst++ = color.b;
        *dst++ = color.g;
        *dst++ = color.r;
        return (dst);
    }

    u8      *PrivColor::_WriteBGR8(u8 *dst, const Color &color)
    {
        *dst++ = color.b;
        *dst++ = color.g;
        *dst++ = color.r;
        return (dst);
    }

    u8      *PrivColor::_WriteRGB565(u8 *dst, const Color &color)
    {
        union
        {
            u16     u;
            char    b[2];
        }           half;

        half.u  = (color.r & 0xF8) << 8;
        half.u |= (color.g & 0xFC) << 3;
        half.u |= (color.b & 0xF8) >> 3;

        /**(dst++) = half.b[0];
        *(dst++) = half.b[1];*/
        *reinterpret_cast<u16 *>(dst) = half.u;
        return dst + 2;
    }

    u8      *PrivColor::_WriteRGB5A1(u8 *dst, const Color &color)
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

        *(dst++) = half.b[0];
        *(dst++) = half.b[1];
        return (dst);
    }

    u8      *PrivColor::_WriteRGBA4(u8 *dst, const Color &color)
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

        *(dst++) = half.b[0];
        *(dst++) = half.b[1];
        return (dst);
    }

    /*
    ** Clamp
    ************/
    u8      *PrivColor::_WriteRGBA8Clamp(u8 *dst, const Color &color)
    {
        int posX;
        int posY;

        Renderer::GetContext()->screen->GetPosFromAddress((u32)dst, posX, posY);

        if (!_clampArea.Contains(posX, posY))
            return (dst + 4);

        *dst++ = color.a;
        *dst++ = color.b;
        *dst++ = color.g;
        *dst++ = color.r;
        return (dst);
    }

    u8      *PrivColor::_WriteBGR8Clamp(u8 *dst, const Color &color)
    {
        int posX;
        int posY;

        Renderer::GetContext()->screen->GetPosFromAddress((u32)dst, posX, posY);

        if (!_clampArea.Contains(posX, posY))
            return (dst + 3);

        *dst++ = color.b;
        *dst++ = color.g;
        *dst++ = color.r;
        return (dst);
    }

    u8      *PrivColor::_WriteRGB565Clamp(u8 *dst, const Color &color)
    {
        int posX;
        int posY;

        Renderer::GetContext()->screen->GetPosFromAddress((u32)dst, posX, posY);

        if (!_clampArea.Contains(posX, posY))
            return (dst + 2);

        union
        {
            u16     u;
            char    b[2];
        }           half;

        half.u  = (color.r & 0xF8) << 8;
        half.u |= (color.g & 0xFC) << 3;
        half.u |= (color.b & 0xF8) >> 3;

        *(dst++) = half.b[0];
        *(dst++) = half.b[1];
        return (dst);
    }

    u8      *PrivColor::_WriteRGB5A1Clamp(u8 *dst, const Color &color)
    {
        int posX;
        int posY;

        Renderer::GetContext()->screen->GetPosFromAddress((u32)dst, posX, posY);

        if (!_clampArea.Contains(posX, posY))
            return (dst + 2);

        union
        {
            u16     u;
            char    b[2];
        }           half;

        half.u  = (color.r & 0xF8) << 8;
        half.u |= (color.g & 0xF8) << 3;
        half.u |= (color.b & 0xF8) >> 2;
        half.u |= 1;

        *(dst++) = half.b[0];
        *(dst++) = half.b[1];
        return (dst);
    }

    u8      *PrivColor::_WriteRGBA4Clamp(u8 *dst, const Color &color)
    {
        int posX;
        int posY;

        Renderer::GetContext()->screen->GetPosFromAddress((u32)dst, posX, posY);

        if (!_clampArea.Contains(posX, posY))
            return (dst + 2);

        union
        {
            u16     u;
            char    b[2];
        }           half;

        half.u  = (color.r & 0xF0) << 8;
        half.u |= (color.g & 0xF0) << 4;
        half.u |= (color.b & 0xF0);
        half.u |= 0x0F;

        *(dst++) = half.b[0];
        *(dst++) = half.b[1];
        return (dst);
    }
}
