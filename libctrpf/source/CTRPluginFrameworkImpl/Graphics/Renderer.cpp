#include "types.h"
#include "3ds.h"

#include "CTRPluginFramework/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFramework/System/Clock.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"
#include <cstdio>
#include <cmath>

#include "csvc.h"

#define FPS 0

namespace CTRPluginFramework
{
    using namespace CTRPluginFrameworkImpl::Services;

    thread_local RendererContext Renderer::_rendererContext;

    inline u32   GetFramebufferOffset(int posX, int posY, int bpp, int rowsize)
    {
        return ((rowsize - 1 - posY + posX * rowsize) * bpp);
    }

    void        Renderer::SetTarget(Target target)
    {
        RendererContext *ctx = GetContext();

        ctx->target = target;
        if (target == BOTTOM)
            ctx->screen = ScreenImpl::Bottom;
        else
            ctx->screen = ScreenImpl::Top;

        PrivColor::SetFormat(ctx->screen->GetFormat());
    }

    void        Renderer::EndFrame(bool copy)
    {
        static const IntRect    &background = Window::BottomWindow.GetRect(); // 20, 20, 280, 200);

#if FPS
        static Clock    fpsCounter;
#endif

        Preferences::ApplyBacklight();

        bool isTouchDown = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        // Draw Touch Cursor (on menu)
        if (isTouchDown && background.Contains(touchPos))
        {
            int posX = touchPos.x - 2;
            int posY = touchPos.y - 1;
            touchPos.x += 10;
            touchPos.y += 15;
            if (background.Contains(touchPos))
            {
                SetTarget(BOTTOM);
                DrawSysString("\uE058", posX, posY, 320, Color::White);
            }
        }

#if FPS
        SetTarget(BOTTOM);
        // Draw fps counter
        char buffer[20] = {0};

        sprintf(buffer, "FPS: %.02f",  1.f / fpsCounter.Restart().AsSeconds());
        int posY = 30;
        DrawString(buffer, 200, posY, Color::White, Color::Black);
#endif

        ScreenImpl::Top->SwapBuffer();
        ScreenImpl::Bottom->SwapBuffer();

        GSP::WaitBufferSwapped(3);
        if (copy)
        {
            ScreenImpl::Top->Copy();
            ScreenImpl::Bottom->Copy();
        }

    }

    void    Renderer::MenuSelector(int posX, int posY, int width, int height)
    {
        ScreenImpl  *screen = GetContext()->screen;

        //int x = posX;
        //int y = posY;
        int w = width;
        int h = height;


        u8 *left = screen->GetLeftFrameBuffer(posX, posY + 1);
        int bpp;
        int rowstride;
        GSPGPU_FramebufferFormat fmt;
        screen->GetFrameBufferInfos(rowstride, bpp, fmt);

        float fade = Preferences::Settings.CursorFadeValue;

        // Draw Rectangle
        while (--w >= 0)
        {
            h = height;
            //x = posX + w;
            u8 *dst = left + rowstride * w;
            while (--h >= 0)
            {
                Color &&c = PrivColor::FromFramebuffer(dst);

                c.Fade(fade);
                PrivColor::ToFramebuffer(dst, c);
                dst -= bpp;
            }
        }

        int tier = width / 3;
        int pitch = tier / 10;
        int j = 0;
        float fading = 0.0f;

        Color l(255, 255, 255);
        posY += height;
        u8 *dst = screen->GetLeftFrameBuffer(posX + (width - tier), posY);
        u8 *rtier = dst;
        Color black(60, 60, 60);
        // Right tier
        for (int i = tier; i > 0; --i)
        {
            l.Fade(fading);
            if (l <= black)
                break;
            PrivColor::ToFramebuffer(rtier, l);
            j++;
            if (j == pitch)
            {
                fading -= 0.01f;
                j = 0;
            }
            rtier += rowstride;

        }

        l = Color(255, 255, 255);
        // Middle tier
        for (int i = 0; i < tier; ++i)
        {
            PrivColor::ToFramebuffer(dst, l);
            dst -= rowstride;
        }
        fading = 0.0f;

        // Left tier
        for (int i = tier; i > 0; --i)
        {
            l.Fade(fading);
            if (l <= black)
                break;
            PrivColor::ToFramebuffer(dst, l);
            dst -= rowstride;
            j++;
            if (j == pitch)
            {
                fading -= 0.01f;
                j = 0;
            }
        }
    }

    void    ProcessingLogo::Reset(void)
    {
        step = 0;
        timer.Restart();
    }

    void    ProcessingLogo::Draw(int posX, int posY)
    {
        const char * waitLogo[] =
        {
            "\uE020", "\uE021", "\uE022", "\uE023", "\uE024", "\uE025", "\uE026", "\uE027"
        };

        Renderer::DrawSysString(waitLogo[step], posX, posY, 300, Color::SkyBlue);

        if (timer.HasTimePassed(Seconds(0.125f)))
        {
            ++step;
            timer.Restart();
            if (step >= 8)
                step = 0;
        }
    }
}
