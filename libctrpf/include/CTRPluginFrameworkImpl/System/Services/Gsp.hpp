#pragma once

#include <3ds.h>

namespace CTRPluginFrameworkImpl {
namespace Services
{
    namespace GSP
    {
        enum
        {
            TOP_SCREEN = 0,
            BOTTOM_SCREEN = 1
        };

        enum
        {
            FB_TOP_READY = BIT(0),
            FB_BOTTOM_READY = BIT(1),

            FB_BOTH_READY = (FB_TOP_READY | FB_BOTTOM_READY),

            FB_TOP_NEED_CLEAR = BIT(2),
            FB_BOTTOM_NEED_CLEAR = BIT(3),

            FB_BOTH_NEED_CLEAR = (FB_TOP_NEED_CLEAR | FB_BOTTOM_NEED_CLEAR)
        };

        union  FrameBufferInfoHeader
        {
            s32     header;
            struct
            {
                u8  screen;
                u8  update;
            };
        };

        struct FrameBufferInfo
        {
            u32 active_framebuf;        ///< Active framebuffer. (0 = first, 1 = second)
            u32 *framebuf0_vaddr;       ///< Framebuffer virtual address, for the main screen this is the 3D left framebuffer.
            u32 *framebuf1_vaddr;       ///< For the main screen: 3D right framebuffer address.
            u32 framebuf_widthbytesize; ///< Value for 0x1EF00X90, controls framebuffer width.
            u32 format;                 ///< Framebuffer format, this u16 is written to the low u16 for LCD register 0x1EF00X70.
            u32 framebuf_dispselect;    ///< Value for 0x1EF00X78, controls which framebuffer is displayed.
            u32 unk;                    ///< Unknown.

            void    FillFrameBufferFrom(FrameBufferInfo& src);
        };

        struct FrameBufferInfoShared
        {
            FrameBufferInfoHeader     header;
            FrameBufferInfo           fbInfo[2];

            void    FillFrameBuffersFrom(FrameBufferInfoShared& src);
        };

        extern u32  InterruptReceiverThreadPriority;
        extern s32  BufferFlags;

        Result  Initialize(void);
        void    Update(u32 threadId, Handle eventHandle, Handle sharedMemHandle);
        void    PauseInterruptReceiver(void);
        void    ResumeInterruptReceiver(void);
        void    TriggerPSCEvents(void);
        void    WaitForVBlank(void);
        void    WaitForVBlank1(void);
        void    SwapBuffer(int screen);
        // 0: Top, 1: Bottom, 3: Both
        void    WaitBufferSwapped(int screen);

        u32     ImportFrameBufferInfo(FrameBufferInfoShared& dest, int screen);
        void    SetFrameBufferInfo(FrameBufferInfoShared& src, int screen, bool convert);

        static inline s32      __ldrex__(s32 *addr)
        {
            s32 v;
            do
                v = __ldrex(addr);
            while (__strex(addr, v));

            return v;
        }

        static inline void     __strex__(s32 *addr, s32 val)
        {
            do
                __ldrex(addr);
            while (__strex(addr, val));
        }
    }
}}
