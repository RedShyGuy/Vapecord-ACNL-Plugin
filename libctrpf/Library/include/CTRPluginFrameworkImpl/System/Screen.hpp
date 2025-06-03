#ifndef CTRPLUGINFRAMEWORK_SCREENIMPL_HPP
#define CTRPLUGINFRAMEWORK_SCREENIMPL_HPP

#include <3ds.h>
#include "CTRPluginFrameworkImpl/Graphics/BMPImage.hpp"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"

namespace CTRPluginFramework
{
    using namespace CTRPluginFrameworkImpl::Services::GSP;

    enum
    {
        SCREENSHOT_TOP = 1,
        SCREENSHOT_BOTTOM = 2,
        SCREENSHOT_BOTH = SCREENSHOT_TOP | SCREENSHOT_BOTTOM
    };

    class Color;
    class ScreenImpl
    {
    public:

        enum LCDSetup
        {
            WidthHeight = 0x5C,     ///< Framebuffer width & height  Lower 16 bits: width, upper 16 bits: height
            FramebufferA1 = 0x68,   ///< Framebuffer A first address For top screen, this is the left eye 3D framebuffer.
            FramebufferA2 = 0x6C,   ///< Framebuffer A second address    For top screen, this is the left eye 3D framebuffer.
            Format = 0x70,          ///< Framebuffer format  Bit0-15: framebuffer format, bit16-31: unknown
            Select = 0x78,          ///< Framebuffer select  Bit0: which framebuffer to display, bit1-7: unknown
            Stride = 0x90,          ///< Framebuffer stride  Distance in bytes between the start of two framebuffer rows (must be a multiple of 8).
            FramebufferB1 = 0x94,   ///< Framebuffer B first address For top screen, this is the right eye 3D framebuffer. Unused for bottom screen.
            FramebufferB2 = 0x98    ///< Framebuffer B second address    For top screen, this is the right eye 3D framebuffer. Unused for bottom screen.
        };

        static  ScreenImpl *Top;
        static  ScreenImpl *Bottom;

        ScreenImpl(u32 lcdSetupInfo, u32 fillColorAddress, bool isTopScreen = false);

        bool                        IsTopScreen(void);
        bool                        Is3DEnabled(void);

        void                        Flash(Color &color);

        // TODO: remove ?
        static void                 Clean(void);
        // Switch displayed fb game <=> ctrpf
        static void                 SwitchFrameBuffers(bool game);
        static void                 ApplyFading(void);

        // Return non zero on failure
        static u32                  AcquireFromGsp(bool fade = true);
        // Return non zero on failure
        static u32                  CheckGspFrameBuffersInfo(void);

        void                        Fade(const float fade);
        void                        Flush(void);
        void                        Invalidate(void);

        // Clear Framebuffer + apply fade effect
        void                        Clear(bool applyFlagForCurrent);
        // Copy currently displayed image to second frame buffer
        void                        Copy(void);
        void                        Debug(void);

        // Fetch Game's FrameBufferInfo and swap to CTRPF FrameBufferInfo
        // Return non zero on failure
        u32                         Acquire(bool fade = true);
        // Return non zero on failure
        u32                         ImportFromGsp(void);

        // Rewrite Game's FrameBufferInfo
        void                        Release(void);

        // For OSD usage
        void                        Acquire(u32 left, u32 right, u32 stride, u32 format, bool backup = false);
        void                        SwapBuffer(void);
        // Arbitrary buffer swapping, do not reflect changes on screen
        void                        SwapBufferInternal(void);
        u32                         GetBacklight(void);
        void                        SetBacklight(u32 value);

        GSPGPU_FramebufferFormat    GetFormat(void) const;
        u16                         GetWidth(void) const;
        u32                         GetStride(void) const;
        u32                         GetRowSize(void) const;
        u32                         GetBytesPerPixel(void) const;
        u32                         GetFrameBufferSize(void) const;
        u32                         GetCurrentBufferID(void) const;

        void                        GetFrameBufferInfos(int &rowstride, int &bpp, GSPGPU_FramebufferFormat &format) const;

        u8                          *GetLeftFrameBuffer(bool current = false);
        u8                          *GetLeftFrameBuffer(int posX, int posY);
        u8                          *GetRightFrameBuffer(bool current = false);
        u8                          *GetRightFrameBuffer(int posX, int posY);
        void                        GetPosFromAddress(u32 address, int &posX, int &posY);

    private:
        friend class Renderer;
        friend void  CTRPluginFrameworkImpl::Services::GSP::Update(u32 threadId, Handle eventHandle, Handle sharedMemHandle);
        friend void  KeepThreadMain(void *);

        static void                 Initialize(void);
        // Displays ctrpf' screens without setting internal state as "screen captured"
        static void                 ApplyCtrpfScreens(void);

        u32                         _LCDSetup;  ///< Address of this screen LCD configuration
        u32                         _FillColor; ///< Address of this screen fill color register
        u32                         _backlightOffset;
        u32                         _currentBuffer;
        u32                         _leftFrameBuffers[2];
        u32                         _rightFrameBuffers[2];


        const u16                   _width;
        u32                         _stride;
        u32                         _rowSize;
        u32                         _bytesPerPixel;
        bool                        _isTopScreen;
        bool                        _isGspAcquired{false};
        GSPGPU_FramebufferFormat    _format;
        FrameBufferInfoShared       _frameBufferInfo{};
        FrameBufferInfoShared       _gameFrameBufferInfo{};
    };
}

#endif
