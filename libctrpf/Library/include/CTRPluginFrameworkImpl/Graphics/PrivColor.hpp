#ifndef COLOR_HPP
#define COLOR_HPP

#include <3ds.h>
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFramework/System/Rect.hpp"

namespace CTRPluginFramework
{

    using FCPointer = Color (*)(u8 *);
    using F8Pointer = u8*   (*)(u8 *, const Color&);

    class PrivColor
    {
    public:
        static thread_local FCPointer FromFramebuffer;
        static thread_local F8Pointer ToFramebuffer;

        static thread_local bool     _useClamp;
        static thread_local IntRect  _clampArea;

        static void     UseClamp(bool willUse);
        static void     UseClamp(bool willUse, const IntRect& rect);

        static GSPGPU_FramebufferFormat GetFormat(void);
        static void     SetFormat(GSPGPU_FramebufferFormat format);

    private:
        friend class ScreenImpl;

        static Color    _ReadRGBA8(u8 *src);
        static Color    _ReadBGR8(u8 *src);
        static Color    _ReadRGB565(u8 *src);
        static Color    _ReadRGB5A1(u8 *src);
        static Color    _ReadRGBA4(u8 *src);

        static u8       *_WriteRGBA8(u8 *dst, const Color &color);
        static u8       *_WriteBGR8(u8 *dst, const Color &color);
        static u8       *_WriteRGB565(u8 *dst, const Color &color);
        static u8       *_WriteRGB5A1(u8 *dst, const Color &color);
        static u8       *_WriteRGBA4(u8 *dst, const Color &color);

        static u8       *_WriteRGBA8Clamp(u8 *dst, const Color &color);
        static u8       *_WriteBGR8Clamp(u8 *dst, const Color &color);
        static u8       *_WriteRGB565Clamp(u8 *dst, const Color &color);
        static u8       *_WriteRGB5A1Clamp(u8 *dst, const Color &color);
        static u8       *_WriteRGBA4Clamp(u8 *dst, const Color &color);


        static thread_local GSPGPU_FramebufferFormat _format;
    };
}

#endif
