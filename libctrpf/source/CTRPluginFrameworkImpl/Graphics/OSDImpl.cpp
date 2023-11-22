#include "CTRPluginFrameworkImpl/Graphics/OSDImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "font6x10Linux.h"
#include "csvc.h"

#include <vector>
#include <cstring>
#include "CTRPluginFramework/System/Sleep.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"

#define THREADVARS_MAGIC  0x21545624 // !TV$
// Thanks to Luma3DS custom mapping, we have a direct access to those
#define REG32(x)                    *(vu32 *)(x | (1u << 31))
#define GPU_PSC0_CNT                REG32(0x1040001C)
#define GPU_PSC1_CNT                REG32(0x1040002C)
#define GPU_TRANSFER_CNT            REG32(0x10400C18)
#define GPU_CMDLIST_CNT             REG32(0x104018F0)

namespace CTRPluginFramework
{
    bool        OSDImpl::DrawSaveIcon = false;
    bool        OSDImpl::MessColors = false;
    u32         OSDImpl::WaitingForScreenshot = 0;
    u32         OSDImpl::FramesToPlay = 0;
    Hook        OSDImpl::OSDHook;
    Screen      OSDImpl::TopScreen;
    Screen      OSDImpl::BottomScreen;
    RecursiveLock OSDImpl::RecLock;
    FloatingButton OSDImpl::FloatingBtn(IntRect(0, 0, 40, 40), Icon::DrawRocket);
    std::vector<OSDImpl::OSDMessage*> OSDImpl::Notifications;
    std::vector<OSDCallback> OSDImpl::Callbacks;
    std::vector<OSDCallback> OSDImpl::CallbacksTrashBin;
    void*       OSDImpl::previousFBAddr[2][2][2] = {0};

    bool        OSDImpl::IsFramePaused = false;
    bool        OSDImpl::NeedToPauseFrame = false;
    bool        OSDImpl::WritesToPrevFB = false;
    LightEvent  OSDImpl::OnNewFrameEvent;
    LightEvent  OSDImpl::OnFramePaused;
    LightEvent  OSDImpl::OnFrameResume;
    Task        OSDImpl::DrawNotifTask1(OSDImpl::DrawNotif_TaskFunc, nullptr, Task::AppCores);
    Task        OSDImpl::DrawNotifTask2(OSDImpl::DrawNotif_TaskFunc, nullptr, Task::AppCores);

    static const Time  g_second = Seconds(1.f);

    struct FpsCounter
    {
        u32     nbFrames{0};
        Clock   clock;
        Time    last;

        char text[16];

        Time    Update(void)
        {
            Time elapsedTime = clock.GetElapsedTime();
            Time delta = elapsedTime - last;

            last = elapsedTime;
            ++nbFrames;
            if (elapsedTime > g_second)
            {
                sprintf(text, "FPS: %ld", nbFrames);
                nbFrames = 0;
                clock.Restart();
            }

            return delta;
        }

        void    Display(void)
        {
            int posY = 10;
            Renderer::DrawString(text, 10, posY, Color::White, Color::Black);
        }
    };

    static FpsCounter g_fpsCounter[2];

    void    InstallOSD(void);

    void    OSDImpl::_Initialize(void)
    {
        RecursiveLock_Init(&RecLock);

        // Init frame event
        LightEvent_Init(&OnNewFrameEvent, RESET_STICKY);
        LightEvent_Init(&OnFramePaused, RESET_STICKY);
        LightEvent_Init(&OnFrameResume, RESET_STICKY);
        IsFramePaused = false;

        const u64 titlesNeedWritePrevFB[] =
        {
            0x0004000000101200ULL, // Puyo Puyo Tetris
            0x0004000000197500ULL, // Puyo Puyo Chronicle
            0x0004000000056600ULL, // Puyo Puyo!! 20th Anniversary
        };
        u64 titleID = Process::GetTitleID();

        for (u32 i = 0; i < sizeof(titlesNeedWritePrevFB) / sizeof(u64); i++)
        {
            if (titleID == titlesNeedWritePrevFB[i])
            {
                WritesToPrevFB = true;
                break;
            }
        }

        InstallOSD();
    }

#define XEND    390

    OSDImpl::OSDMessage::OSDMessage(const std::string& str, const Color& fg, const Color& bg)
    {
        text = str;
        width = Renderer::LinuxFontSize(text.c_str());
        drawn = false;
        foreground = fg;
        background = bg;
        time = Clock();
    }

    void    OSDImpl::Update(void)
    {
        if (Preferences::IsEnabled(Preferences::UseFloatingBtn))
        {
            FloatingBtn.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));
            if (FloatingBtn())
                PluginMenuImpl::ForceOpen();
        }

        if (TryLock())
            return;

        if (Notifications.size() && Notifications.front()->drawn)
            Notifications.erase(std::remove_if(Notifications.begin(), Notifications.end(),
                [](OSDMessage *message)
                {
                    bool remove = message->drawn && message->time.HasTimePassed(Seconds(5.f));
                    if (remove)
                        delete message;
                    return remove;
                }),
                Notifications.end());

        Unlock();
    }

    bool    OSDImpl::Draw(void)
    {
        if (Notifications.empty())
            return false;

        int posX;
        int posY = std::min((u32)15, (u32)Notifications.size());
        posY = 230 - (15 * posY);
        int count = 0;

        for (OSDMessage *message : Notifications)
        {
            posX = XEND - message->width;
            Renderer::DrawString(message->text.c_str(), posX, posY, message->foreground, message->background);
            posY += 5;
            if (!message->drawn)
                message->time.Restart();

            message->drawn = true;

            count++;
            if (count >= 15)
                break;
        }

        return (true);
    }

    s32     OSDImpl::DrawNotif_TaskFunc(void *arg)
    {
        Renderer::SetTarget(TOP);

        DrawNotifArgs * args = reinterpret_cast<DrawNotifArgs *>(arg);
        OSDIter         messIter = args->begin;
        OSDIter         endIter = args->end;
        int             posY = args->posY;

        for (; messIter != endIter; ++messIter)
        {
            OSDMessage *message = *messIter;

            int posX = XEND - message->width;
            Renderer::DrawString(message->text.c_str(), posX, posY, message->foreground, message->background);
            posY += 5;
            if (!message->drawn)
                message->time.Restart();

            message->drawn = true;
        }

        return 0;
    }

    u32 GetBPP(GSPFormat format);

    u32     OSDImpl::MainCallback(u32 isBottom, int nextBank, void *leftFb, void *rightFb, int stride, int format, int swap)
    {
        // Only call our OSD callback if left frame buffer is valid
        if (leftFb)
        {
            CallbackCommon(isBottom, leftFb, rightFb, stride, format, swap);
        }

        if (ProcessImpl::Status == Running)
            return HookContext::GetCurrent().OriginalFunction<u32>(isBottom, nextBank, leftFb, rightFb, stride, format, swap);

        return 0;
    }

    // TODO: more research on this pattern ?
    u32     OSDImpl::MainCallback2(u32 r0, u32 *params, u32 isBottom, u32 arg)
    {
        // It appears that sometimes that ptr is nullptr ???
        if (params)
        {
            // u32 isBottom = params[0];
            void    *leftFb = (void *)params[1];
            // void *addrB = params[2]; possible, not confirmed
            u32 stride = params[3];
            u32 format = params[4] & 0xF;

            if (leftFb)
                CallbackCommon(isBottom, leftFb, nullptr, stride, format, 0);
        }

        if (ProcessImpl::Status == Running)
            return HookContext::GetCurrent().OriginalFunction<u32>(r0, params, isBottom, arg);

        return 0;
    }

    Result      OSDImpl::OnTopScreenFrame(void)
    {
        if (FramesToPlay)
            --FramesToPlay;

        // If frame has to be paused
        PauseFrame();

        if (DrawSaveIcon)
            Icon::DrawSave(10, 10);

        // Signal a new frame to all threads waiting for it
       // Controller::Update();code
        OSDImpl::Update();
        LightEvent_Pulse(&OnNewFrameEvent);
        // ThreadEx::Yield(); ///< Give control to plugin thread

        return 0;
    }

    Result      OSDImpl::OnBottomScreenFrame(void)
    {
        // Floating button
        if (Preferences::IsEnabled(Preferences::UseFloatingBtn))
            FloatingBtn.Draw();

        // Touch cursor and / or position
        if (Touch::IsDown())
        {
            IntVector touchPos(Touch::GetPosition());

            if (Preferences::IsEnabled(Preferences::DrawTouchCursor))
            {
                int posX = touchPos.x - 2;
                int posY = touchPos.y - 1;
                Icon::DrawHandCursor(posX, posY);
            }

            if (Preferences::IsEnabled(Preferences::DrawTouchPosition))
            {
                std::string &&str = Utils::Format("Touch.x: %d  Touch.y: %d", touchPos.x, touchPos.y);
                int posY = 20;
                Renderer::DrawString(str.c_str(), 10, posY, Color::White, Color::Black);
            }
        }

        return 0;
    }

    static u32 GetBuffer(u32 addr)
    {
        FwkSettings &settings = FwkSettings::Get();
        u32 ret;

        if (settings.CachedDrawMode || SystemImpl::IsCitra)
        {
            ret = addr;
        }
        else
        {
            u32 newAddr = PA_FROM_VA(addr);
            ret = (newAddr & (1 << 31)) ? newAddr : 0;
        }

        if (addr < 0x01000000 || addr > 0x40000000) return 0;
        else return ret;
    }

    void     OSDImpl::CallbackCommon(u32 isBottom, void* addr, void* addrB, int stride, int format, int swap)
    {
        if (SystemImpl::Status())
            return;
        // TODO: fully remove this, rosalina implements it now
        // Preferences::ApplyBacklight();

        bool drawFps = (Preferences::IsEnabled(Preferences::ShowBottomFps) && isBottom) || (Preferences::IsEnabled(Preferences::ShowTopFps) && !isBottom);
        if (drawFps) g_fpsCounter[isBottom].Update();

        // Screen shot first
        if (Screenshot::OSDCallback(isBottom, addr, addrB, stride, format))
            return;

        /*if (!drawFps && !DrawSaveIcon && !MessColors
            && Callbacks.empty() && Notifications.empty())
            return; */

        // Convert to actual addresses and check validity
        if (WritesToPrevFB)
        {
            // TO DO: This is a very hacky fix! Research why this actually happens
            // From my research, looks like puyo puyo games either re-render
            // or do some post-processing to the FB after CTRPF writes to them,
            // which causes the changes to be overwritten.
            // As a hacky solution, we actually draw to the FB that's currently
            // being displayed. This causes some artifacts such as flickering
            // but is better than nothing. /shrug
            previousFBAddr[isBottom][swap ? 1 : 0][0] = addr;
            previousFBAddr[isBottom][swap ? 1 : 0][1] = addrB;

            addr = previousFBAddr[isBottom][swap ? 0 : 1][0];
            addrB = previousFBAddr[isBottom][swap ? 0 : 1][1];
        }
        addr = (void*)GetBuffer((u32)addr);
        if (!isBottom)
            addrB = (void*)GetBuffer((u32)addrB);

        if (!addr)
            return;

        // TODO: remove
        // if (MessColors)
        //    MessColor((u32)addr, stride, format);

        if (!isBottom)
        {
            ScreenImpl::Top->Acquire((u32)addr, (u32)addrB, stride, format & 0b111);
            Renderer::SetTarget(TOP);

            Result res = OnTopScreenFrame();

            // No osd when game is paused
            if (ProcessImpl::IsPaused || res)
                return;
        }
        else
        {
            ScreenImpl::Bottom->Acquire((u32)addr, (u32)addrB, stride, format & 0b111);
            Renderer::SetTarget(BOTTOM);

            OnBottomScreenFrame();
        }

        DrawNotifArgs   args[2]; ///< Careful with the scope of that var

        Lock();

        FwkSettings &settings = FwkSettings::Get();
        if (settings.CachedDrawMode) {
            svcInvalidateProcessDataCache(CUR_PROCESS_HANDLE, reinterpret_cast<u32>(addr), isBottom ? stride * 320 : stride * 400);
            if (!isBottom && addrB && addrB != addr)
                svcInvalidateProcessDataCache(CUR_PROCESS_HANDLE, reinterpret_cast<u32>(addrB), stride * 400);
        }

        if (Notifications.size())
        {
            int nbOfMessage = std::min((u32)15, (u32)Notifications.size());
            int posY = 230 - 15 * nbOfMessage;

            args[0].begin = Notifications.begin();
            args[0].end = std::next(Notifications.begin(), nbOfMessage / 2);
            args[0].posY = posY;
            args[1].begin = args[0].end;
            args[1].end = std::next(Notifications.begin(), nbOfMessage);
            args[1].posY = posY + 15 * (nbOfMessage / 2);

            DrawNotifTask1.Start((void *)&args[0]);
            DrawNotifTask2.Start((void *)&args[1]);
        }

        // Remove callbacks in the trash bin
        if (CallbacksTrashBin.size())
        {
            Callbacks.erase(std::remove_if(Callbacks.begin(), Callbacks.end(),
                [](OSDCallback cb)
                {
                    auto&   trashbin = CallbacksTrashBin;
                    auto    foundIter = std::find(trashbin.begin(), trashbin.end(), cb);

                    return foundIter != trashbin.end();
                }),
                Callbacks.end());

            CallbacksTrashBin.clear();
        }

        // We need to ensure that notifications are handled before running callbacks
        DrawNotifTask1.Wait();
        DrawNotifTask2.Wait();

        // Call OSD Callbacks
        if (Callbacks.size())
        {
            Screen screen;

            screen.IsTop = !isBottom;
            screen.Is3DEnabled = isBottom ? false : ScreenImpl::Top->Is3DEnabled();
            screen.LeftFramebuffer = (u32)addr;
            screen.RightFramebuffer = (u32)addrB;
            screen.Stride = stride;
            screen.BytesPerPixel = GetBPP((GSPFormat)format);
            screen.Format = (GSPFormat)format;

            for (size_t i = 0; i < Callbacks.size(); i++)
                if (Callbacks[i]) Callbacks[i](screen);
        }

        Unlock();

        // Draw fps
        if (drawFps)
            g_fpsCounter[isBottom].Display();

        if (settings.CachedDrawMode) {
            if (!SystemImpl::IsCitra) svcFlushProcessDataCache(CUR_PROCESS_HANDLE, reinterpret_cast<u32>(addr), isBottom ? stride * 320 : stride * 400);
            if (!isBottom && addrB && addrB != addr)
                if (!SystemImpl::IsCitra) svcFlushProcessDataCache(CUR_PROCESS_HANDLE, reinterpret_cast<u32>(addrB), stride * 400);
        }
    }

    void    OSDImpl::UpdateScreens(void)
    {
        ScreenImpl *screen = ScreenImpl::Top;

        // Top screen
        TopScreen.IsTop = true;
        TopScreen.Is3DEnabled = screen->Is3DEnabled();
        TopScreen.LeftFramebuffer = (u32)screen->GetLeftFrameBuffer(false);
        TopScreen.RightFramebuffer = (u32)screen->GetRightFrameBuffer(false);
        TopScreen.Stride = (u32)screen->GetStride();
        TopScreen.BytesPerPixel = screen->GetBytesPerPixel();
        TopScreen.Format = screen->GetFormat();

        screen = ScreenImpl::Bottom;

        // Bottom screen
        BottomScreen.IsTop = false;
        BottomScreen.Is3DEnabled = screen->Is3DEnabled();
        BottomScreen.LeftFramebuffer = (u32)screen->GetLeftFrameBuffer(false);
        BottomScreen.RightFramebuffer = (u32)screen->GetRightFrameBuffer(false);
        BottomScreen.Stride = (u32)screen->GetStride();
        BottomScreen.BytesPerPixel = screen->GetBytesPerPixel();
        BottomScreen.Format = screen->GetFormat();
    }

    static const u32    g_OSDPattern[] =
    {
        0xE1833000, // ORR R3, R3, R0
        0xE2044CFF, // AND R4, R4, #0xFF00
        0xE3C33CFF, // BIC R3, R3, #0xFF00
        0xE1833004, // ORR R3, R3, R4
        0xE1824F93, // STREX R4, R3, [R2] // 0x14

        0xE8830E60, // STMIA R3, {R5, R6, R9 - R11}
        0xEE078F9A, // MCR p15, 0, R8, c7, c10, 4 // Data Synchronization Barrier
        0xE3A03001, // MOV R3, #1
        0xE7902104, // LDR R2, [R0, R4, LSL#2] // 0x10

        0xEE076F9A, // MCR p15, 0, R6, c7, c10, 4 // Data Synchronization Barrier
        0xE3A02001, // MOV R2, #1
        0xE7901104, // LDR R1, [R0, R4, LSL#2]
        0xE1911F9F, // LDREX R1, [R1]
        0xE3C110FF, // BIC R1, R1, #0xFF // 0x14

        0xE3A00000, // MOV R0, #0
        0xEE070F9A, // MCR P15, 0, R0, c7, c10, 4
        0xE3A00001, // MOV R0, #1
        0xE7951104, // LDR R1, [R5, R4, LSL#2] // 0x10

        0xE3A00000, // MOV R0, #0
        0xEE070F9A, // MCR P15, 0, R0, c7, c10, 4
        0xE2850001, // ADD R0, R5, #1
        0xEA000004, // B #16 // 0x10
    };

    static u8       *memsearch(u8 *startPos, const void *pattern, u32 size, u32 patternSize)
    {
        const u8 *patternc = (const u8 *)pattern;
        u32 table[256];

        //Preprocessing
        for (u32 i = 0; i < 256; i++)
            table[i] = patternSize;
        for (u32 i = 0; i < patternSize - 1; i++)
            table[patternc[i]] = patternSize - i - 1;

        //Searching
        u32 j = 0;
        while (j <= size - patternSize)
        {
            u8 c = startPos[j + patternSize - 1];
            if (patternc[patternSize - 1] == c && memcmp(pattern, startPos + j, patternSize - 1) == 0)
                return startPos + j;
            j += table[c];
        }

        return nullptr;
    }

    static u32     SearchStmfd(u32 start, u32 size, u32 stmfd)
    {
        if (!start || !size || !stmfd)
            return (0);

        u32     result = 0;
        u32     *end = (u32 *)(start - size);

        for (u32 *addr = (u32 *)start; addr > end; addr--)
        {
            if (*addr == stmfd)
            {
                result = (u32)addr;
                break;
            }
        }
        return result;
    }

    static u32     SearchOSD(u32 pattern)
    {
        u8     *address = nullptr;

        if (pattern == 0)
        {
            address = memsearch((u8 *)0x100000, g_OSDPattern, Process::GetTextSize(), 0x14);

            if (address == nullptr)
            {
                address = memsearch((u8 *)0x100000, &g_OSDPattern[5], Process::GetTextSize(), 0x10);
            }
        }
        else if (pattern == 3)
        {
            address = memsearch((u8 *)0x100000, &g_OSDPattern[9], Process::GetTextSize(), 0x14);
        }
        else if (pattern == 4)
        {
            address = memsearch((u8 *)0x100000, &g_OSDPattern[14], Process::GetTextSize(), 0x10);
        }
        else if (pattern == 5)
        {
            address = memsearch((u8 *)0x100000, &g_OSDPattern[18], Process::GetTextSize(), 0x10);
        }

        return (u32)address;
    }

    // TODO: clean
    void    InstallOSD(void)
    {
        const u32   stmfd1 = 0xE92D5FF0; // STMFD SP!, {R4-R12, LR}
        const u32   stmfd2 = 0xE92D47F0; // STMFD SP!, {R4-R10, LR}
        const u32   stmfd3 = 0xE92D4070; // STMFD SP!, {R4-R6, LR}

        u32         found = SearchOSD(0);
        u32         found2 = 0;

        found = SearchStmfd(found, 0x400, stmfd1);
        if (!found)
        {
            found = SearchOSD(3);
            found = SearchStmfd(found, 0x400, stmfd2);
            if (!found)
            {
                found2 = SearchOSD(4);
                found2 = SearchStmfd(found2, 0x400, stmfd3);
                if (!found2)
                {
                    //MessageBox("OSD couldn't be installed: #1 !")();
                    return;
                }
                OSDImpl::OSDHook.InitializeForMitm(found2, u32(OSDImpl::MainCallback2)).Enable();
                return;
            }
        }

        OSDImpl::OSDHook.InitializeForMitm(found, u32(OSDImpl::MainCallback)).Enable();
    }

    /*static void    MessColor(u32 startAddr, u32 stride, u32 format)
    {
        //u32 endBuffer = startAddr + (stride * 400);
        u32 bpp = GetBPP((GSPGPU_FramebufferFormat)format);

        PrivColor::SetFormat((GSPGPU_FramebufferFormat)format);

        if (bpp == 4)
        {

            for (int x = 0; x < 400; ++x)
            {
                u32 *fb = (u32 *)(startAddr + stride * x);
                u32 fbend = (u32)fb + 240 * 4;

                while (fb < (u32 *)fbend)
                {
                    u32 c = *fb;

                    //color.a = 255;
                    u8 b = (c >> 16); //Swap R
                                      //u8 g = (c >> 8);
                    u8 r = c; //Swap b

                              //color.Fade(0.1f);

                    c &= 0xFF00FF00;
                    c |= r << 16;
                    c |= b;

                    *fb++ = c;
                }
            }
        }

        else if (bpp == 3)
        {
            for (int x = 0; x < 400; ++x)
            {
                u32 *fb = (u32 *)(startAddr + stride * x);
                u32 fbend = (u32)fb + 240 * 3;

                while (fb < (u32 *)fbend)
                {
                    u32 c = *fb;

                    u8 b = (c >> 16); //Swap R
                    u8 g = (c >> 8);
                    u8 r = c; //Swap b

                    c &= 0xFF000000;
                    c |= r << 16;
                    c |= g << 8;
                    c |= b;

                    *fb = c;
                    fb = (u32 *)((u32)fb + 3);
                }
            }
        }
        else if (bpp == 2)
        {
            for (int x = 0; x < 400; ++x)
            {
                u16 *fb = (u16 *)(startAddr + stride * x);
                u16 *fbEnd = fb + 240;

                while (fb < fbEnd)
                {
                    u16 c = *fb;

                    u8 b = (c >> 8) & 0xF8; //Swap R
                    u8 g = (c >> 3) & 0xFC;
                    u8 r = (c << 3) & 0xF8; //Swap b

                    c = (r & 0xF8) << 8;
                    c |= (g & 0xFC) << 3;
                    c |= (b & 0xF8) >> 3;

                    *fb++ = c;
                }
            }
        }
    }*/
}
