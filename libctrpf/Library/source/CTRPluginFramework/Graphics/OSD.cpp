#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFrameworkImpl/Graphics/OSDImpl.hpp"
#include "CTRPluginFrameworkImpl/Graphics/PrivColor.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFramework/System/Process.hpp"

#include <algorithm>
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "CTRPluginFrameworkImpl/System/Services/Gsp.hpp"

namespace CTRPluginFramework
{
    using namespace CTRPluginFrameworkImpl::Services;

    u8      *Screen::GetFramebuffer(u32 posX, u32 posY, bool useRightFb) const
    {
        if (useRightFb && (!IsTop || !Is3DEnabled))
            return (nullptr);

        u8  *fb = useRightFb ? (u8 *)RightFramebuffer : (u8 *)LeftFramebuffer;

        // FB Row size in bytes / sizeof(u16) for RGB565
        u32 rowsize = Stride / BytesPerPixel;

        // Ensure that the position is within pos(0, 0) - pos(SCREEN_WIDTH, 240)
        posX = std::min(posX, (IsTop ? (u32)400 : (u32)320));
        posY = std::min(posY, (u32)240);

        // Correct posY to include format gap if any
        posY += rowsize - 240;

        // posY is reversed, hence the rowsize - posY part
        u32 offset = (rowsize - 1 - posY + posX * rowsize) * BytesPerPixel;

        return (fb + offset);
    }

    u32     Screen::Draw(const std::string &str, u32 posX, u32 posY, const Color& foreground, const Color& background) const
    {
        Renderer::SetTarget(IsTop ? TOP : BOTTOM);
        int newPosY = posY;
        Renderer::DrawString(str.c_str(), posX, newPosY, foreground, background);
        return (newPosY);
    }

    u32 Screen::DrawSysfont(const std::string &str, u32 posX, u32 posY, const Color &foreground) const
    {
        Renderer::SetTarget(IsTop ? TOP : BOTTOM);
        int newPosY = posY;
        Renderer::DrawSysString(str.c_str(), posX, newPosY, 400, foreground);
        return (posY);
    }

    void    Screen::DrawRect(u32 posX, u32 posY, u32 width, u32 height, const Color& color, bool filled) const
    {
        Renderer::SetTarget(IsTop ? TOP : BOTTOM);
        Renderer::DrawRect(posX, posY, width, height, color, filled);
    }

    void    Screen::DrawPixel(u32 posX, u32 posY, const Color& color) const
    {
        Renderer::SetTarget(IsTop ? TOP : BOTTOM);
        Renderer::DrawPixel(posX, posY, color);
    }

    void    Screen::ReadPixel(u32 posX, u32 posY, Color& pixel, bool fromRightFb) const
    {
        u8 *fb = GetFramebuffer(posX, posY, fromRightFb);

        pixel = PrivColor::FromFramebuffer(fb);
    }

    void    Screen::Fade(const float fadeAmount)
    {
        ScreenImpl* scrImpl = IsTop ? ScreenImpl::Top : ScreenImpl::Bottom;
        scrImpl->Fade(fadeAmount);
    }

    int     OSD::Notify(const std::string &str, const Color &fg, const Color &bg)
    {
        OSDImpl::Lock();

        if (OSDImpl::Notifications.size() >= 50)
        {
            OSDImpl::Unlock();
            return (-1);
        }

        OSDImpl::Notifications.push_back(new OSDImpl::OSDMessage(str, fg, bg));
        OSDImpl::Unlock();

        return (0);
    }

    void    OSD::Run(OSDCallback cb)
    {
        OSDImpl::Lock();

        // If the callback is going to be added, make sure it's not in the trash bin
        if (OSDImpl::CallbacksTrashBin.size())
        {
            auto it = std::find(OSDImpl::CallbacksTrashBin.begin(), OSDImpl::CallbacksTrashBin.end(), cb);
            if (it != OSDImpl::CallbacksTrashBin.end()) OSDImpl::CallbacksTrashBin.erase(it);
        }

        if (std::find(OSDImpl::Callbacks.begin(), OSDImpl::Callbacks.end(), cb) == OSDImpl::Callbacks.end())
            OSDImpl::Callbacks.push_back(cb);

        OSDImpl::Unlock();
    }

    void    OSD::Stop(OSDCallback cb)
    {
        OSDImpl::Lock();

        if (std::find(OSDImpl::CallbacksTrashBin.begin(), OSDImpl::CallbacksTrashBin.end(), cb) == OSDImpl::CallbacksTrashBin.end())
            OSDImpl::CallbacksTrashBin.push_back(cb);

        OSDImpl::Unlock();
    }

    float   OSD::GetTextWidth(bool sysfont, const std::string &text)
    {
        if (sysfont)
            return Renderer::GetTextSize(text.c_str());
        return Renderer::LinuxFontSize(text.c_str());
    }

    const Screen & OSD::GetTopScreen(void)
    {
        return OSDImpl::TopScreen;
    }

    const Screen & OSD::GetBottomScreen(void)
    {
        return OSDImpl::BottomScreen;
    }

    void    OSD::SwapBuffers(void)
    {
        ScreenImpl::Bottom->SwapBuffer();
        ScreenImpl::Top->SwapBuffer();

        GSP::WaitBufferSwapped(3);
        OSDImpl::UpdateScreens();
    }

    void    OSD::Lock(void)
    {
        OSDImpl::Lock();
    }

    bool    OSD::TryLock(void)
    {
        return (OSDImpl::TryLock());
    }

    void OSD::Unlock(void)
    {
        OSDImpl::Unlock();
    }
}
