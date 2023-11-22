#include "types.h"
#include <3ds.h>
#include "csvc.h"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/System/Thread.hpp"
#include "CTRPluginFramework/Utils.hpp"

#include <vector>

using namespace CTRPluginFramework;

namespace CTRPluginFrameworkImpl {
namespace Services
{
    namespace GSP
    {
        static void   InterruptReceiver(void *arg);

        static bool                     RunInterruptReceiver{true};
        static bool                     CatchInterrupt{false};
        static u32                      ThreadId{0};
        static vu8 *                    SharedMemoryBlock{nullptr};
        static vu8 *                    EventData{nullptr};
        static vu8 *                    CmdReqQueue{nullptr};
        static volatile Handle          GSPEvent;
        static Handle                   WakeEvent;
        static LightEvent               VBlank0Event;
        static LightEvent               VBlank1Event;
        static LightSemaphore           Semaphore;
        static Hook                     GSPRegisterInterruptReceiverHook;
        static ThreadEx                 InterruptReceiverThread{InterruptReceiver, 0x1000, 0x1A, -1};
        static FrameBufferInfoShared *  SharedFrameBuffers[2]{nullptr};

        s32    BufferFlags{0};
        u32    InterruptReceiverThreadPriority;

        extern "C" void __gsp__Update(u32 threadId, Handle eventHandle, Handle sharedMemHandle)
        {
            Update(threadId, eventHandle, sharedMemHandle);
        }

        static void NAKED GSPGPU__RegisterInterruptHook(void)
        {
#ifndef _MSC_VER
            __asm__ __volatile__
            (
                "ldr    r2, [r4, #0xC] @event handle   \n"
                "str    r2, [sp, #-4]!                 \n"
                "svc    0x32                           \n"
                "ands   r1, r0, #0x80000000            \n"
                "ldr    r1, [sp], #4                   \n"
                "bxmi	lr                             \n"
                "stmfd	sp!, {r0, lr}                  \n"
                "ldr    r0, [r4, #8] @ thread id       \n"
                "ldr    r2, [r4, #0x10] @ shared mem handle \n"
                "bl     __gsp__Update                  \n"
                "ldmfd	sp!, {r0, pc}                  \n"
            );
#endif
        }
        Result  Initialize(void)
        {
            const std::vector<u32> gspgpuRegisterInterruptPattern =
            {
                0xE92D4070,
                0xE1A06003,
                0xE59D5010,
                0xEE1D3F70,
                0xE2834080,
                0xE59F3034,
                0xE3A0C000,
                0xE584100C,
                0xE9841004,
                0xE5843000,
                0xE5900000,
                0xEF000032,
                0xE2101102,
                0x4A000004,
                0xE5940008,
                0xE5850000,
                0xE5940010,
                0xE5860000,
                0xE5940004,
                0xE8BD8070,
                0x00130042,
            };

            const std::vector<u32> gspgpuRegisterInterruptPattern2 =
            {
                0xE92D4070,
                0xE1A05003,
                0xE59D6010,
                0xEE1D4F70,
                0xE59FC034,
                0xE3A03000,
                0xE5A4C080,
                0xE584100C,
                0xE1C420F4,
               /* 0xE5900000,
                0xEF000032,
                0xE2101102,
                0x4A000004,
                0xE5940008,
                0xE5860000,
                0xE5940010,
                0xE5850000,
                0xE5940004,
                0xE8BD8070,
                0x00130042*/
            };

            u32 found = 0;
            u32 addr = Utils::Search(0x00100000, Process::GetTextSize(), gspgpuRegisterInterruptPattern);
            Hook& hook = GSPRegisterInterruptReceiverHook;

            if (addr)
            {
                hook.Initialize(addr + 0x2C, (u32)GSPGPU__RegisterInterruptHook).SetFlags(USE_LR_TO_RETURN);
                hook.Enable();
            }
            else
            {
                for (addr = 0x00100000; !found && addr <= Process::GetTextSize() - gspgpuRegisterInterruptPattern2.size(); )
                {
                    addr = Utils::Search(0x00100000, Process::GetTextSize(), gspgpuRegisterInterruptPattern2);
                    if (!addr)
                        return -1;

                    u32 *a = (u32 *)addr;
                    for (u32 i = 0; i < 20; ++i)
                    {
                        if (a[i] == 0x00130042)
                        {
                            found = 1;
                            break;
                        }
                    }
                }
                if (!found)
                    return -1;

                hook.Initialize(addr + 0x28, (u32)GSPGPU__RegisterInterruptHook).SetFlags(USE_LR_TO_RETURN);
                hook.Enable();
            }

            svcCreateEvent(&WakeEvent, RESET_ONESHOT);
            LightEvent_Init(&VBlank0Event, RESET_STICKY);
            LightEvent_Init(&VBlank1Event, RESET_STICKY);
            LightSemaphore_Init(&Semaphore, 1, 1);

            return 0;
        }

        void    Update(u32 threadId, Handle eventHandle, Handle sharedMemHandle)
        {
            ThreadId = threadId;
            GSPEvent = eventHandle;

            if (SharedMemoryBlock == nullptr)
            {
                SharedMemoryBlock = static_cast<vu8 *>(mappableAlloc(0x1000));
                svcMapMemoryBlock(sharedMemHandle, reinterpret_cast<u32>(SharedMemoryBlock), static_cast<MemPerm>(0x3), static_cast<MemPerm>(0x10000000));
            }

            u8  *base = const_cast<u8 *>(SharedMemoryBlock) + 0x200 + threadId * 0x80;
            SharedFrameBuffers[0] = reinterpret_cast<FrameBufferInfoShared *>(base);
            SharedFrameBuffers[1] = reinterpret_cast<FrameBufferInfoShared *>(base + 0x40);
            EventData = SharedMemoryBlock + threadId * 0x40;
            CmdReqQueue = SharedMemoryBlock + 0x800 + threadId * 0x200;

            if (InterruptReceiverThread.GetStatus() == ThreadEx::IDLE)
            {
                RunInterruptReceiver = true;
                CatchInterrupt = false;
                InterruptReceiverThread.priority = InterruptReceiverThreadPriority;
                InterruptReceiverThread.Start(nullptr);
            }

#define FORCE_SCREEN_RESET
#ifdef FORCE_SCREEN_RESET
            ScreenImpl::ApplyCtrpfScreens();
#endif
        }

        void    FrameBufferInfo::FillFrameBufferFrom(FrameBufferInfo &src)
        {
            // Only support this.format == RGB_565
            if ((format & 0xF) != GSP_RGB565_OES)
                return;

            union
            {
                u16     u;
                char    b[2];
            }           half;

            u8  *pix = reinterpret_cast<u8 *>(src.framebuf0_vaddr);
            u8  *dst = reinterpret_cast<u8 *>(framebuf0_vaddr);
            u32  width = (src.format & 0x60) > 0 ? 400 : 320;

            svcInvalidateProcessDataCache(CUR_PROCESS_HANDLE, (u32)src.framebuf0_vaddr, width * src.framebuf_widthbytesize);

            switch (src.format & 7)
            {
            case GSP_RGBA8_OES:
            {
                u32 padding = src.framebuf_widthbytesize - 4 * 240;

                for (; width > 0; --width)
                {
                    for (u32 height = 240; height > 0; --height)
                    {
                        half.u  = (*pix++ & 0xF8) << 8;
                        half.u |= (*pix++ & 0xFC) << 3;
                        half.u |= (*pix++ & 0xF8) >> 3;

                        *dst++ = half.b[0];
                        *dst++ = half.b[1];
                        ++pix;
                    }
                    pix += padding;
                }

                break;
            }
            case GSP_BGR8_OES:
            {
                u32 padding = src.framebuf_widthbytesize - 3 * 240;

                for (; width > 0; --width)
                {
                    for (u32 height = 240; height > 0; --height)
                    {
                        half.u = (*pix++ & 0xF8) >> 3;
                        half.u |= (*pix++ & 0xFC) << 3;
                        half.u |= (*pix++ & 0xF8) << 8;

                        *dst++ = half.b[0];
                        *dst++ = half.b[1];
                    }
                    pix += padding;
                }

                break;
            }
            case GSP_RGB565_OES:
            {
                u32 size = 240 * 2;

                for (; width > 0; --width)
                {
                    std::copy(pix, pix + size, dst);
                    pix += src.framebuf_widthbytesize;
                    dst += size;
                }
                break;
            }
            case GSP_RGB5_A1_OES:
            {
                u32 padding = src.framebuf_widthbytesize - 2 * 240;

                union
                {
                    u16     u{0};
                    u8      b[2];
                }           col;

                for (; width > 0; --width)
                {
                    for (u32 height = 240; height > 0; --height)
                    {
                        col.b[0] = *pix++;
                        col.b[1] = *pix++;

                        half.u  = ((col.u >> 8) & 0xF8) << 8;
                        half.u |= ((col.u >> 3) & 0xF8) << 3;
                        half.u |= ((col.u << 2) & 0xF8) >> 3;

                        *dst++ = half.b[0];
                        *dst++ = half.b[1];
                    }
                    pix += padding;
                }

                break;
            }
            case GSP_RGBA4_OES:
            {
                u32 padding = src.framebuf_widthbytesize - 2 * 240;

                union
                {
                    u16     u{0};
                    u8      b[2];
                }           col;

                for (; width > 0; --width)
                {
                    for (u32 height = 240; height > 0; --height)
                    {
                        col.b[0] = *pix++;
                        col.b[1] = *pix++;

                        half.u  = ((col.u >> 8) & 0xF0) << 8;
                        half.u |= ((col.u >> 4) & 0xF0) << 3;
                        half.u |= (col.u & 0xF0) >> 3;

                        *dst++ = half.b[0];
                        *dst++ = half.b[1];
                    }
                    pix += padding;
                }

                break;
            }
            default:
                break;
            }
        }

        void    FrameBufferInfoShared::FillFrameBuffersFrom(FrameBufferInfoShared &src)
        {
            const u32 displayed = src.header.screen;

            fbInfo[0].FillFrameBufferFrom(src.fbInfo[displayed]);
        }

        static void  ClearInterrupts(void)
        {
            bool    strexFailed;

            do
            {
                // Do a load on all header fields as an atomic unit
                __ldrex((s32 *)EventData);

                strexFailed = __strex((s32 *)EventData, 0);

            } while (__builtin_expect(strexFailed, 0));
        }

        struct QueueBody
        {
            u32 control;
            s32 droppedPdc0Count;
            s32 droppedPdc1Count;
            u8 data[52];
        } PACKED;

        static QueueBody g_backup;

        static void SaveQueue(void)
        {
            QueueBody *queue = (QueueBody *)EventData;

            __ldrex((s32 *)EventData);
            g_backup.control = queue->control;
            g_backup.droppedPdc0Count = queue->droppedPdc0Count;
            g_backup.droppedPdc1Count = queue->droppedPdc1Count;

            for (int i = 0; i < 52; ++i)
            {
                g_backup.data[i] = queue->data[i];
            }

            __clrex();

            // OSD::Notify(Utils::Format("SQ Count: %08X", queue->control));
        }

        /*static void RestoreQueue(void)
        {
            QueueBody *queue = (QueueBody *)EventData;

            __ldrex((s32 *)EventData);
            queue->control = g_backup.control;
            queue->droppedPdc0Count = g_backup.droppedPdc0Count;
            queue->droppedPdc1Count = g_backup.droppedPdc1Count;

            for (int i = 0; i < 52; ++i)
            {
                queue->data[i] = g_backup.data[i];
            }

            __clrex();

            // OSD::Notify(Utils::Format("RQ Count: %08X", queue->control));
        }*/

        static int  PopInterrupt(void)
        {
            int     curEvt;
            bool    strexFailed;

            do
            {
                union
                {
                    u32     as_u32;
                    struct
                    {
                        u8  cur;
                        u8  count;
                        u8  status;
                        u8  control;
                    };
                } header;

                // Do a load on all header fields as an atomic unit
                header.as_u32 = __ldrex((s32 *)EventData);

                if (__builtin_expect(header.count == 0, 0))
                {
                    __clrex();
                    return -1;
                }

                curEvt = EventData[0xC + header.cur];

                header.cur += 1;
                if (header.cur >= 0x34)
                    header.cur -= 0x34;
                header.count -= 1;
                // header.status = 0;

                strexFailed = __strex((s32 *)EventData, header.as_u32);

            } while (__builtin_expect(strexFailed, 0));

            return curEvt;
        }

        static void  EnqueueEvent(s8 event, bool signal = true)
        {
            //int     curEvt;
            bool    strexFailed;

            do
            {
                union
                {
                    u32     as_u32;
                    struct
                    {
                        u8  cur;
                        u8  count;
                        u8  status;
                        u8  control;
                    };
                } header;

                // Do a load on all header fields as an atomic unit
                header.as_u32 = __ldrex((s32 *)EventData);

                s32 lastIndex = (header.cur + header.count) % 0x34;

                EventData[0xC + lastIndex] = event;

                __dsb();

                header.count++;

                strexFailed = __strex((s32 *)EventData, header.as_u32);

            } while (__builtin_expect(strexFailed, 0));

            if (signal)
                svcSignalEvent(GSPEvent);
        }

        static void InterruptReceiver(void *arg UNUSED)
        {
            while (RunInterruptReceiver)
            {
                while (!CatchInterrupt)
                {
                    if (!RunInterruptReceiver)
                        break;

                    LightSemaphore_Release(&Semaphore, 1);

                    svcWaitSynchronization(WakeEvent, U64_MAX);

                    LightSemaphore_Acquire(&Semaphore, 1);
                    SaveQueue();
                }

                ClearInterrupts();
                while (CatchInterrupt)
                {
                    svcClearEvent(GSPEvent);
                    static_cast<void>(!svcWaitSynchronization(GSPEvent, U64_MAX));

                    while (true)
                    {
                        int curEvt = PopInterrupt();

                        if (curEvt == -1)
                            break;

                        // Top screen event
                        if (curEvt == GSPGPU_EVENT_VBlank0)
                        {
                            LightEvent_Signal(&VBlank0Event);
                        }

                        // Bottom screen event
                        if (curEvt == GSPGPU_EVENT_VBlank1)
                        {
                            LightEvent_Signal(&VBlank1Event);
                        }
                    }
                }
            }

            svcExitThread();
        }

        void    TriggerPSCEvents(void)
        {
            EnqueueEvent(GSPGPU_EVENT_PSC0, false);
            EnqueueEvent(GSPGPU_EVENT_PSC1, true);
        }

        void    PauseInterruptReceiver(void)
        {
            svcClearEvent(WakeEvent);
            CatchInterrupt = false;
            svcSignalEvent(GSPEvent);
            LightSemaphore_Acquire(&Semaphore, 1);
        }

        void    ResumeInterruptReceiver(void)
        {
            CatchInterrupt = true;
            LightSemaphore_Release(&Semaphore, 1);
            svcSignalEvent(WakeEvent);
        }

        void    WaitForVBlank(void)
        {
            ClearInterrupts();
            LightEvent_Clear(&VBlank0Event);
            LightEvent_Wait(&VBlank0Event);
        }

        void    WaitForVBlank1(void)
        {
            ClearInterrupts();
            LightEvent_Clear(&VBlank1Event);
            LightEvent_Wait(&VBlank1Event);
        }

        void    WaitForVBlankBoth(void)
        {
            ClearInterrupts();
            LightEvent_Clear(&VBlank0Event);
            LightEvent_Clear(&VBlank1Event);
            LightEvent_Wait(&VBlank0Event);
            LightEvent_Wait(&VBlank1Event);
        }

        void    SwapBuffer(int screen)
        {
            s32 *addr = &SharedFrameBuffers[screen]->header.header;
            s32 val;
            do
            {
                val = __ldrex(addr);
                val ^= 1;       // Toggle displayed screen
                val |= 0x100;   // Update flag
            } while (__strex(addr, val));
        }

        void    WaitBufferSwapped(int screen)
        {
            if (screen == 1)
            {
                WaitForVBlank1();
                goto __clearBottom;
            }
            if (screen == 0)
            {
                WaitForVBlank();
            }
            else
            {
                WaitForVBlankBoth();
            }

            if (BufferFlags & FB_TOP_NEED_CLEAR)
            {
                BufferFlags &= ~FB_TOP_NEED_CLEAR;
                ScreenImpl::Top->Clear(false);
            }

            if (screen == 0)
                return;

        __clearBottom:
            if (BufferFlags & FB_BOTTOM_NEED_CLEAR)
            {
                BufferFlags &= ~FB_BOTTOM_NEED_CLEAR;
                ScreenImpl::Bottom->Clear(false);
            }
        }

        u32    ImportFrameBufferInfo(FrameBufferInfoShared& dest, int screen)
        {
            u8 *src = reinterpret_cast<u8 *>(SharedFrameBuffers[screen]);
            u8 *dst = reinterpret_cast<u8 *>(&dest);

            if (!src) return -1;

            std::copy(src, src + sizeof(FrameBufferInfoShared), dst);
            return 0;
        }

        static u32 *plgVAtoGameVa(u32 *va)
        {
            if (SystemImpl::IsCitra)
                return va;
            else
                return reinterpret_cast<u32 *>(svcConvertVAToPA(va, false) - 0xC000000);
        }

        void    SetFrameBufferInfo(FrameBufferInfoShared& src, int screen, bool convert)
        {
            u8 *s = reinterpret_cast<u8 *>(&src.fbInfo);
            u8 *dst = reinterpret_cast<u8 *>(&SharedFrameBuffers[screen]->fbInfo);

            std::copy(s, s + sizeof(FrameBufferInfo) * 2, dst);

            // VA to PA to expected VA
            if (convert)
            {
                FrameBufferInfoShared *fbs = SharedFrameBuffers[screen];

                fbs->fbInfo[0].framebuf0_vaddr = plgVAtoGameVa(src.fbInfo[0].framebuf0_vaddr);
                if (src.fbInfo[0].framebuf1_vaddr)
                    fbs->fbInfo[0].framebuf1_vaddr = plgVAtoGameVa(src.fbInfo[0].framebuf1_vaddr);

                fbs->fbInfo[1].framebuf0_vaddr = plgVAtoGameVa(src.fbInfo[1].framebuf0_vaddr);
                if (src.fbInfo[1].framebuf1_vaddr)
                    fbs->fbInfo[1].framebuf1_vaddr = plgVAtoGameVa(src.fbInfo[0].framebuf1_vaddr);
            }

            __dsb();

            src.header.update = 1;
            s32 *addr = &SharedFrameBuffers[screen]->header.header;
            do
            {
                __ldrex(addr);
            } while (__strex(addr, src.header.header));
        }
    } ///< GSP
}}
