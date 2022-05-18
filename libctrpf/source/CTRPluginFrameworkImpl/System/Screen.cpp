#include "types.h"
#include <3ds.h>
#include "csvc.h"

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    using namespace CTRPluginFrameworkImpl::Services;

    #define REG(x)          *(vu32 *)(x)
    #define REG32(x)        *(vu32 *)((x) | (1u << 31))
    #define LCDREG(offset)  REG(this->_LCDSetup + offset)

    struct ScreensFramebuffers
    {
        u8  topFramebuffer0[400 * 240 * 2];
        u8  topFramebuffer1[400 * 240 * 2];
        u8  bottomFramebuffer0[320 * 240 * 2];
        u8  bottomFramebuffer1[320 * 240 * 2];
    } PACKED;

    // Reserve the place for the Screen objects
    static u8  _topBuf[sizeof(ScreenImpl)] ALIGN(4);
    static u8  _botBuf[sizeof(ScreenImpl)] ALIGN(4);

    ScreenImpl  *ScreenImpl::Top = nullptr;
    ScreenImpl  *ScreenImpl::Bottom = nullptr;

    u32 GetBPP(GSPGPU_FramebufferFormat format)
    {
        switch(format)
        {
            case GSP_RGBA8_OES:
                return 4;
            case GSP_BGR8_OES:
                return 3;
            case GSP_RGB565_OES:
            case GSP_RGB5_A1_OES:
            case GSP_RGBA4_OES:
                return 2;
        }
        return 3;
    }

    ScreenImpl::ScreenImpl(const u32 lcdSetupInfo, const u32 fillColorAddress, const bool isTopScreen) :
        _LCDSetup(lcdSetupInfo),
        _FillColor(fillColorAddress),
        _backlightOffset{isTopScreen ? 0x40u : 0x840u},
        _currentBuffer(0),
        _width(isTopScreen ? 400 : 320),
        _stride(0), _rowSize(0), _bytesPerPixel(0),
        _isTopScreen(isTopScreen), _format()
    {
    }

    void    ScreenImpl::Initialize(void)
    {
        auto *screensFbs = reinterpret_cast<ScreensFramebuffers *>(FwkSettings::Header->heapVA);

        Top = new (_topBuf) ScreenImpl(0x10400400 | (1u << 31), (0x10202000 | (1u << 31)) + 0x204, true);
        Bottom = new (_botBuf) ScreenImpl(0x10400500 | (1u << 31), (0x10202000 | (1u << 31)) + 0xA04);

        // Top screen
        GSP::FrameBufferInfo* fb = &Top->_frameBufferInfo.fbInfo[0];

        fb->active_framebuf = 0;
        fb->framebuf0_vaddr = reinterpret_cast<u32 *>(screensFbs->topFramebuffer0);
        fb->framebuf1_vaddr = fb->framebuf0_vaddr;
        fb->framebuf_widthbytesize = 240 * 2; // Enforce RGB565
        fb->format = 0x42; // GSP_RGB565_OES;
        fb->framebuf_dispselect = 0;

        fb = &Top->_frameBufferInfo.fbInfo[1];

        fb->active_framebuf = 1;
        fb->framebuf0_vaddr = reinterpret_cast<u32 *>(screensFbs->topFramebuffer1);
        fb->framebuf1_vaddr = fb->framebuf0_vaddr;
        fb->framebuf_widthbytesize = 240 * 2; // Enforce RGB565
        fb->format = 0x42; // GSP_RGB565_OES;
        fb->framebuf_dispselect = 1;

        // Bottom screen
        fb = &Bottom->_frameBufferInfo.fbInfo[0];

        fb->active_framebuf = 0;
        fb->framebuf0_vaddr = reinterpret_cast<u32 *>(screensFbs->bottomFramebuffer0);
        fb->framebuf1_vaddr = nullptr;
        fb->framebuf_widthbytesize = 240 * 2; // Enforce RGB565
        fb->format = GSP_RGB565_OES;
        fb->framebuf_dispselect = 0;

        fb = &Bottom->_frameBufferInfo.fbInfo[1];

        fb->active_framebuf = 1;
        fb->framebuf0_vaddr = reinterpret_cast<u32 *>(screensFbs->bottomFramebuffer1);
        fb->framebuf1_vaddr = nullptr;
        fb->framebuf_widthbytesize = 240 * 2; // Enforce RGB565
        fb->format = GSP_RGB565_OES;
        fb->framebuf_dispselect = 1;
    }

    void    ScreenImpl::ApplyCtrpfScreens(void)
    {
            Top->_frameBufferInfo.header.screen = Top->_currentBuffer;
            Bottom->_frameBufferInfo.header.screen = Bottom->_currentBuffer;
            GSP::SetFrameBufferInfo(Top->_frameBufferInfo, 0, true);
            GSP::SetFrameBufferInfo(Bottom->_frameBufferInfo, 1, true);
    }

    void    ScreenImpl::Fade(const float fade)
    {
        const u32   size = GetFrameBufferSize() / _bytesPerPixel;
        u8         *frameBuf = GetLeftFrameBuffer();

        PrivColor::SetFormat(_format);

        for (int i = size; i > 0; --i)
        {
            frameBuf = PrivColor::ToFramebuffer(frameBuf, PrivColor::FromFramebuffer(frameBuf).Fade(fade));
        }
    }

    u32     ScreenImpl::Acquire(bool fade)
    {
        // Fetch game frame buffers & check validity
        if (ImportFromGsp())
            return -1;

        // Copy images and convert to RGB565 to ctrpf's frame buffer (0)
        _frameBufferInfo.FillFrameBuffersFrom(_gameFrameBufferInfo);

        _format = GSP_RGB565_OES;
        _stride = 240*2;
        _bytesPerPixel = 2;
        _rowSize = 240;

        auto &fbInfo = _frameBufferInfo.fbInfo;

        _leftFrameBuffers[0] = reinterpret_cast<u32>(fbInfo[0].framebuf0_vaddr);
        _leftFrameBuffers[1] = reinterpret_cast<u32>(fbInfo[1].framebuf0_vaddr);
        _rightFrameBuffers[0] = reinterpret_cast<u32>(fbInfo[0].framebuf1_vaddr);
        _rightFrameBuffers[1] = reinterpret_cast<u32>(fbInfo[1].framebuf1_vaddr);

        // Set fb1 as current
        _currentBuffer = _frameBufferInfo.header.screen = 1;

        // Apply fade to fb0
        if (fade)
            Fade(0.3f);

        // Copy to fb1
        _currentBuffer = 0;
        Copy();

        // Flush fb0
        _currentBuffer = 1;
        Flush();

        // Apply current frame buffers
        GSP::SetFrameBufferInfo(_frameBufferInfo, !_isTopScreen, true);

        _isGspAcquired = true;

        return 0;
    }

    u32     ScreenImpl::ImportFromGsp(void)
    {
        // GSP screens are plugin screens, so don't fetch
        if (_isGspAcquired)
            return 0;

        // Fetch game frame buffers
        if (GSP::ImportFrameBufferInfo(_gameFrameBufferInfo, !_isTopScreen))
            return -1;

        // Check frame buffers validity
        const u32 displayed = _gameFrameBufferInfo.header.screen;
        if (!Process::CheckAddress(reinterpret_cast<u32>(_gameFrameBufferInfo.fbInfo[displayed].framebuf0_vaddr)))
            return -1;

        return 0;
    }

    void    ScreenImpl::Release(void)
    {
        GSP::SetFrameBufferInfo(_gameFrameBufferInfo, !_isTopScreen, false);
        _isGspAcquired = false;
    }

    void    ScreenImpl::Acquire(u32 left, u32 right, u32 stride, u32 format, bool backup)
    {
        _currentBuffer = 1;

        _format = static_cast<GSPGPU_FramebufferFormat>(format & 7);
        _stride = stride;
        _bytesPerPixel = GetBPP(_format);
        _rowSize = _stride / _bytesPerPixel;
        _leftFrameBuffers[0] = left;
        _rightFrameBuffers[0] = right;

        _gameFrameBufferInfo.header.screen = 0;
        GSP::FrameBufferInfo& fbInfo = _gameFrameBufferInfo.fbInfo[0];

        fbInfo.active_framebuf = 0;
        fbInfo.framebuf0_vaddr = reinterpret_cast<u32 *>(left);
        fbInfo.framebuf1_vaddr = reinterpret_cast<u32 *>(right);
        fbInfo.framebuf_widthbytesize = stride;
        fbInfo.format = format;
        fbInfo.framebuf_dispselect = 0;
    }

    void    ScreenImpl::Flush(void)
    {
        u32 size = GetFrameBufferSize();

        // Flush currentBuffer
        svcFlushProcessDataCache(CUR_PROCESS_HANDLE, (u32)GetLeftFrameBuffer(), size);
    }

    void    ScreenImpl::Invalidate(void)
    {
        u32 size = GetFrameBufferSize();

        // Invalidate currentBuffer
        svcInvalidateProcessDataCache(CUR_PROCESS_HANDLE, (u32)GetLeftFrameBuffer(), size);
    }

    void    ScreenImpl::Clear(bool applyFlagForCurrent)
    {
        const u32 displayed = _gameFrameBufferInfo.header.screen;

        _frameBufferInfo.fbInfo[!_currentBuffer].FillFrameBufferFrom(_gameFrameBufferInfo.fbInfo[displayed]);

        Fade(0.3f);

        if (!applyFlagForCurrent)
            return;

        s32 val = GSP::__ldrex__(&GSP::BufferFlags);

        val |= (_isTopScreen ? GSP::FB_TOP_NEED_CLEAR : GSP::FB_BOTTOM_NEED_CLEAR);
        GSP::__strex__(&GSP::BufferFlags, val);
    }

    void    ScreenImpl::Copy(void)
    {
        u32 size = GetFrameBufferSize();
        u8 *dst = GetLeftFrameBuffer();
        u8 *src = GetLeftFrameBuffer(true);

        std::copy(src, src + size, dst);
    }

    void    ScreenImpl::Debug(void)
    {
        /*
        int posY = 10;
        if (_isTopScreen)
        {
            Renderer::SetTarget(TOP);
            Renderer::DrawString(Utils::Format("FB0: %08X", _leftFramebuffers[0]).c_str(), 10, posY, Color::Blank, Color::Black);
            Renderer::DrawString(Utils::Format("FB1: %08X", _leftFramebuffers[1]).c_str(), 10, posY, Color::Blank, Color::Black);
            Renderer::DrawString(Utils::Format("Sel: %d", _originalBuffer).c_str(), 10, posY, Color::Blank, Color::Black);
        }
        else
        {
            Renderer::SetTarget(BOTTOM);
            Renderer::DrawString(Utils::Format("FB0: %08X", _leftFramebuffers[0]).c_str(), 10, posY, Color::Blank, Color::Black);
            Renderer::DrawString(Utils::Format("FB1: %08X", _leftFramebuffers[1]).c_str(), 10, posY, Color::Blank, Color::Black);
            Renderer::DrawString(Utils::Format("Sel: %d", _originalBuffer).c_str(), 10, posY, Color::Blank, Color::Black);
        }
        */
    }

    bool    ScreenImpl::IsTopScreen(void)
    {
        return _isTopScreen;
    }

    bool    ScreenImpl::Is3DEnabled(void)
    {
        if (!_isTopScreen)
            return false;

        u32 left = _leftFrameBuffers[!_currentBuffer];
        u32 right = _rightFrameBuffers[!_currentBuffer];

        return right && right != left && *(float *)(0x1FF81080) > 0.f;
    }

    void    ScreenImpl::Flash(Color &color)
    {
        u32     fillColor = (color.ToU32() & 0xFFFFFF) | 0x01000000;

        for (int i = 0; i < 0x64; i++)
        {
            REG(_FillColor) = fillColor;
            svcSleepThread(5000000); // 0.005 second
        }
        REG(_FillColor) = 0;
    }

    void    ScreenImpl::Clean(void)
    {
        if (!System::IsNew3DS())
            return;

        Top->Clear(true);
        Bottom->Clear(true);
    }

    void    ScreenImpl::SwitchFrameBuffers(bool game)
    {
        if (game)
        {
            Top->Release();
            Bottom->Release();

            GSP::WaitBufferSwapped(3);
        }
        else
        {
            Top->_frameBufferInfo.header.screen = Top->_currentBuffer;
            Bottom->_frameBufferInfo.header.screen = Bottom->_currentBuffer;
            GSP::SetFrameBufferInfo(Top->_frameBufferInfo, 0, true);
            GSP::SetFrameBufferInfo(Bottom->_frameBufferInfo, 1, true);
            GSP::WaitBufferSwapped(3);
            Top->_isGspAcquired = true;
            Bottom->_isGspAcquired = true;
        }
    }

    void    ScreenImpl::ApplyFading(void)
    {
        Top->Fade(0.5f);
        Bottom->Fade(0.5f);

        Top->SwapBuffer();
        Bottom->SwapBuffer();

        GSP::WaitBufferSwapped(3);

        Top->Copy();
        Bottom->Copy();
    }

#define GPU_PSC0_CNT                REG32(0x1040001C)
#define GPU_PSC1_CNT                REG32(0x1040002C)
#define GPU_TRANSFER_CNT            REG32(0x10400C18)
#define GPU_CMDLIST_CNT             REG32(0x104018F0)

    u32     ScreenImpl::AcquireFromGsp(bool fade)
    {
        // Wait for gpu to finish all stuff
        while ((GPU_PSC0_CNT | GPU_PSC1_CNT | GPU_TRANSFER_CNT | GPU_CMDLIST_CNT) & 1);
        u32 err = Top->Acquire(fade) | Bottom->Acquire(fade);

        //if (!err)
        //    GSP::WaitBufferSwapped(3);

        return err;
    }

    u32     ScreenImpl::CheckGspFrameBuffersInfo()
    {
        return Top->ImportFromGsp() | Bottom->ImportFromGsp();
    }

    /*
    ** Swap buffers
    *****************/

    void    ScreenImpl::SwapBuffer(void)
    {
        svcFlushDataCacheRange(GetLeftFrameBuffer(), GetFrameBufferSize());

        GSP::SwapBuffer(!_isTopScreen);

        _currentBuffer = !_currentBuffer;
    }

    void    ScreenImpl::SwapBufferInternal(void)
    {
        _currentBuffer = !_currentBuffer;
    }

    u32     ScreenImpl::GetBacklight(void)
    {
        return REG32(0x10202200 + _backlightOffset);
    }

    void    ScreenImpl::SetBacklight(u32 value)
    {
        REG32(0x10202200 + _backlightOffset) = value;
    }

    GSPGPU_FramebufferFormat   ScreenImpl::GetFormat(void) const
    {
        return _format;
    }

    u16     ScreenImpl::GetWidth(void) const
    {
        return _width;
    }

    u32     ScreenImpl::GetStride(void) const
    {
        return _stride;
    }

    u32     ScreenImpl::GetRowSize(void) const
    {
        return _rowSize;
    }

    u32     ScreenImpl::GetBytesPerPixel(void) const
    {
        return _bytesPerPixel;
    }

    u32     ScreenImpl::GetFrameBufferSize(void) const
    {
        return _stride * _width;
    }

    u32     ScreenImpl::GetCurrentBufferID(void) const
    {
        return _currentBuffer;
    }

    void    ScreenImpl::GetFrameBufferInfos(int &rowStride, int &bpp, GSPGPU_FramebufferFormat &format) const
    {
        rowStride = _stride;
        bpp = _bytesPerPixel;
        format = _format;
    }

    /*
    ** Framebuffers Getters
    ******************************/

    /*
    ** Left
    *************/
    u8      *ScreenImpl::GetLeftFrameBuffer(bool current)
    {
        return reinterpret_cast<u8 *>(_leftFrameBuffers[current ? _currentBuffer : !_currentBuffer]);
    }

    u8      *ScreenImpl::GetLeftFrameBuffer(int posX, int posY)
    {

        posX = std::max(posX, 0);
        posX = std::min(posX, (_isTopScreen ? 400 : 320));
        posY = std::max(posY, 0);
        posY = std::min(posY, 240);

        // Correct posY
        posY += _rowSize - 240;

        u32 offset = (_rowSize - 1 - posY + posX * _rowSize) * _bytesPerPixel;

        return reinterpret_cast<u8 *>(_leftFrameBuffers[!_currentBuffer]) + offset;
    }

    /*
    ** Right
    *************/

    u8      *ScreenImpl::GetRightFrameBuffer(bool current)
    {
        if (!_isTopScreen)
            return (nullptr);

        return reinterpret_cast<u8 *>(_rightFrameBuffers[current ? _currentBuffer : !_currentBuffer]);
    }

    u8      *ScreenImpl::GetRightFrameBuffer(int posX, int posY)
    {
        if (!_isTopScreen)
            return (nullptr);

        posX = std::max(posX, 0);
        posX = std::min(posX, (_isTopScreen ? 400 : 320));
        posY = std::max(posY, 0);
        posY = std::min(posY, 240);

        // Correct posY
        posY += _rowSize - 240;
        u32 offset = (_rowSize - 1 - posY + posX * _rowSize) * _bytesPerPixel;

        return reinterpret_cast<u8 *>(_rightFrameBuffers[!_currentBuffer]) + offset;
    }

    void    ScreenImpl::GetPosFromAddress(u32 addr, int &posX, int &posY)
    {
        addr -= _leftFrameBuffers[!_currentBuffer];

        posX = addr / (_rowSize * _bytesPerPixel);
        posY = _rowSize - 1 - ((addr / _bytesPerPixel) - (_rowSize * posX));
        posY -= _rowSize - 240;
    }
}
