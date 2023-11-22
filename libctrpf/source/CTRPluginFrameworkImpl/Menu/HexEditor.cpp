#include "CTRPluginFrameworkImpl/Menu/HexEditor.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"

#include <3ds.h>
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFrameworkImpl/Menu/Converter.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuActionReplay.hpp"

#include "CTRPluginFrameworkImpl/Disassembler/arm_disasm.h"

#include <cstdio>

#if __INTELLISENSE__
#define __builtin_clz(x) (x)
#define snprintf(str, ...) (str)
#endif

namespace CTRPluginFramework
{
    HexEditor *__g_hexEditor = nullptr;

    enum
    {
        JumpTo = 0,
        JumpRelative = 1,
        JumpToValue = 2
    };

    HexEditor::HexEditor(u32 target) :
        _submenu{ { "New cheat", "Jump to", "Jump relative", "Jump to value",
                    "Converter", "Move backward", "Move forward", "Save this address",
        "Browse history", "Clear history"}},//, "Play 1 Frame", "Play 2 frames", "Play 5 frames" }},
        _renderTask(_RenderTop, (void *)this, Task::AppCores)
    {
        // Init context
        _indexHistory = -1;

        // Construct keyboard
        _keyboard.SetLayout(Layout::HEXADECIMAL);
        _keyboard._showCursor = false;

        __g_hexEditor = this;

        // Init views
        _views[0] = new ByteView(_ctx);
        _views[1] = new IntegerView(_ctx);
        //_views[2] = new FloatView(_ctx);
        _views[2] = new AsmView(_ctx);
        _viewCurrent = _views[ByteV];

        // Init memory
        ProcessImpl::UpdateMemRegions();
        Goto(target, true);
    }

    HexEditor::~HexEditor(void)
    {
        for (u32 i = 0; i < 3; ++i)
            delete _views[i];
    }

    bool    HexEditor::operator()(EventList &eventList)
    {
        static bool     keysAreDisabled = false;

        if (!keysAreDisabled)
        {
            // Disable clear key
            _keyboard._keys->at(15).Enable(false);
            // Disable enter key
            _keyboard._keys->at(16).Enable(false);
            keysAreDisabled = true;
        }

        // Process events
        bool isSubMenuOpen = _submenu.IsOpen();

        for (size_t i = 0; i < eventList.size(); i++)
        {
            _submenu.ProcessEvent(eventList[i]);
            if (!isSubMenuOpen)
                _ProcessEvent(eventList[i]);
        }

        // If submenu is closed, check for a keyboard input
        if (!isSubMenuOpen)
        {
            int     out;
            if (!(_ctx._flags & InvalidSrc) && _keyboard(out))
                _viewCurrent->EditValueAtCursor(out);
        }
        // If submenu is opened, check for option
        else
        {
            int subchoice = _submenu();

            if (!subchoice--)         _CreateCheat();
            else if (!subchoice--)    _JumpTo(JumpTo);
            else if (!subchoice--)    _JumpTo(JumpRelative);
            else if (!subchoice--)    _JumpTo(JumpToValue);
            else if (!subchoice--)    ///< Converter
            {
                // Enable clear key
                _keyboard._keys->at(15).Enable(true);
                // Enable enter key
                _keyboard._keys->at(16).Enable(true);

                Converter *conv = Converter::Instance();

                if (conv)
                {
                    Item& item = _GetSelectedItem();
                    (*conv)(item.value32);
                }

                // Disable clear key
                _keyboard._keys->at(15).Enable(false);
                // Disable enter key
                _keyboard._keys->at(16).Enable(false);
            }
            else if (!subchoice--)  _MoveBackward();
            else if (!subchoice--)  _MoveForward();
            else if (!subchoice--)  _SaveThisAddress();
            else if (!subchoice--)  _BrowseHistory();
            else if (!subchoice--)  _ClearHistory();
            // Fix Play X frames!!!!!
            /*else if (!subchoice--)  { Process::Play(1);  _ctx._flags |= DirtySrc; return false; }
            else if (!subchoice--)  { Process::Play(2);  _ctx._flags |= DirtySrc; return false; }
            else if (!subchoice--)  { Process::Play(5);  _ctx._flags |= DirtySrc; return false; }*/
        }

        // Update components
        _Update();

        // Render TopScreen
        _renderTask.Start();
        //_RenderTop();

        // Render Bottom Screen
        _RenderBottom();

        _renderTask.Wait();

        if (Window::BottomWindow.MustClose())
        {
            // Enable clear key
            _keyboard._keys->at(15).Enable(true);
            // Enable enter key
            _keyboard._keys->at(16).Enable(true);
            keysAreDisabled = false;
            return (true);
        }

        return (false);
    }

    u32     HexEditor::GetCursorAddress(void)
    {
        return _ctx._cursorAddress;
    }

    void    HexEditor::_ProcessEvent(Event &event)
    {
        _viewCurrent->ProcessEvent(event);

        u32&    flags = _ctx._flags;

        if (event.type == Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case Key::A:
                {
                    if (flags & DirtyMemoryCache)
                        _ApplyChanges();
                    break;
                }

                case Key::B:
                {
                    if (flags & DirtyMemoryCache)
                        _DiscardChanges();
                    else
                        Window::BottomWindow.Close();
                    break;
                }
                case Key::L:
                {
                    if (!(flags & DirtyMemoryCache))
                        SetView(_viewId == ByteV ? AsmV : _viewId - 1);
                    break;
                }
                case Key::R:
                {
                    if (!(flags & DirtyMemoryCache))
                        SetView(_viewId == AsmV ? ByteV : _viewId + 1);
                    break;
                }
                default: break;
            }
        }
    }

    s32    HexEditor::_RenderTop(void *arg)
    {
        HexEditor *obj = __g_hexEditor;//(HexEditor *)arg;
        Context&   _ctx = obj->_ctx;

        Renderer::SetTarget(TOP);

        const Color     &maintextcolor = Preferences::Settings.MainTextColor;
        //const Color     &red = Color::Red;

        int   posY = 61;

        Window::TopWindow.Draw("HexEditor");

        // Render common elements

        // Selector address header
        Renderer::DrawRect(44, posY, 66, 20, Color::SkyBlue);
        posY += 5;
        Renderer::DrawString(obj->_ctx._cursorAddressStr, 54, posY, Color::Black);
        posY += 6;

        // Addresses body
        Renderer::DrawRect(44, posY, 66, 100, Color::SkyBlue);

        // Addresses
        for (size_t i = 0; i < _ctx._items.size(); i += obj->_viewCurrent->ItemsPerLine)
        {
            Renderer::DrawString(_ctx._items[i].addrCache, 54, posY, Color::Black);
        }


        // Render view
        obj->_viewCurrent->Draw();

        if (_ctx._flags & DirtyMemoryCache)
        {
            posY += 5;
            Renderer::DrawString("Apply changes: ", 44, posY, maintextcolor);
            posY -= 14;
            Renderer::DrawSysString("\uE000", 149, posY, 330, maintextcolor);

            posY +=2;
            Renderer::DrawString("Discard changes: ", 44, posY, maintextcolor);
            posY -= 14;
            Renderer::DrawSysString("\uE001", 149, posY, 330, maintextcolor);
        }
        else
        {
            posY += 5;
            Renderer::DrawString("Options: ", 44, posY, maintextcolor);
            posY -= 14;
            Renderer::DrawSysString("\uE002", 99, posY, 330, maintextcolor);
        }

        // Render submenu
        obj->_submenu.Draw();

        return 0;
    }

    void    HexEditor::_RenderBottom(void)
    {
        _keyboard._RenderBottom();

        Window::BottomWindow.DrawButton();
    }

    void    HexEditor::_Update(void)
    {
        bool        isTouched = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        Window::BottomWindow.Update(isTouched, touchPos);

        u32&    flags = _ctx._flags;

        // Check if src has changed
        if (flags & DirtySrc)
        {
            MemInfo     memregion = ProcessImpl::GetMemRegion(_ctx._address);

            // Check for invalid src
            if (memregion.base_addr == 0)
                flags |= InvalidSrc;
            else
                flags &= ~InvalidSrc;

            flags &= ~DirtySrc;
            flags |= DirtyViewCache;
        }

        if (flags & DirtyMemory && !(flags & InvalidSrc))
        {
            MemInfo     memregion = ProcessImpl::GetMemRegion(_ctx._address);
            u32         endRegion = memregion.base_addr + memregion.size;
            u32         address = _ctx._address;

            _ctx._items.resize(_viewCurrent->TotalItems);

            for (Item& item: _ctx._items)
            {
                // If address is outside the region, get next region
                if (address >= endRegion)
                {
                    memregion = ProcessImpl::GetNextRegion(memregion);
                    endRegion = memregion.base_addr + memregion.size;
                    address = memregion.base_addr;
                }

                item.address =  address;
                item.value32 = *(vu32 *)address;
                snprintf(item.addrCache, 9, "%08X", static_cast<unsigned int>(address));
                address += 4;
            }

            flags &= ~(DirtyMemory | DirtyMemoryCache);
            flags |= DirtyViewCache;
        }

        // Update view
        _viewCurrent->UpdateView();

        // Check if the the cursor cache needs to be refreshed
        if (flags & DirtyCursorCache)
        {
            flags &= ~DirtyCursorCache;
            snprintf(_ctx._cursorAddressStr, 9, "%08X", static_cast<unsigned int>(_ctx._cursorAddress));
        }
    }

    void    HexEditor::_CreateCheat(void)
    {
        if (!Preferences::Settings.AllowActionReplay)
            return;

        if (_ctx._flags & InvalidSrc)
        {
            MessageBox(Color::Orange << "Error", "Invalid address, abort")();
            return;
        }

        u32     address = _ctx._cursorAddress & ~3;
        PluginMenuActionReplay::NewARCode(0, address, *(u32 *)address);
    }

    void    HexEditor::_MoveBackward(void)
    {
        // If there's nothing in the history
        if (!_history.size())
            return;

        --_indexHistory;
        if (_indexHistory <= -1)
            _indexHistory = 0;

        // Jump to address
        Goto(_history[_indexHistory], true);
    }

    void    HexEditor::_MoveForward(void)
    {
        // If there's nothing in the history
        if (!_history.size())
            return;

        ++_indexHistory;
        if (static_cast<size_t>(_indexHistory) >= _history.size() - 1)
            _indexHistory = _history.size() - 1;

        // Jump to address
        Goto(_history[_indexHistory], true);
    }

    void    HexEditor::_SaveThisAddress(void)
    {
        u32     address = _ctx._cursorAddress;
        if (_history.size() && _history.back() == address)
            return;
        _history.push_back(address);
        _indexHistory = _history.size() - 1;
    }

    void    HexEditor::_BrowseHistory(void)
    {
        if (_history.empty())
        {
            MessageBox(Color::Orange << "Error", "History is empty")();
            return;
        }

        Keyboard    keyboard;
        std::vector<std::string> addresses;

        for (u32 v : _history)
            addresses.push_back(Utils::ToHex(v));

        keyboard.DisplayTopScreen = false;
        keyboard.Populate(addresses);

        int index = keyboard.Open();

        if (index != -1)
        {
            _indexHistory = index;
            Goto(_history[index], true);
        }
    }

    void    HexEditor::_ClearHistory(void)
    {
        _history.clear();
        _indexHistory = -1;
    }

    bool    HexEditor::Goto(u32 address, bool updateCursor)
    {
        // Check if the address is valid
        MemInfo     region = ProcessImpl::GetMemRegion(address);

        if (region.base_addr == 0)
            return false;

        // If we must move the cursor to target address
        if (updateCursor)
        {
            _ctx._cursorAddress = address;
            _ctx._flags |= DirtyCursor;
        }
        else
            _ctx._flags |= DirtyCursorAddress;

        // Make the address stride aligned
        u32 stride = __builtin_ctz(_viewCurrent->Stride);
        address = (address >> stride) << stride;

        _ctx._address = address;
        _ctx._flags |= DirtySrc | DirtyMemory;

        return true;
    }

    void    HexEditor::SetView(u32 view)
    {
        u32& flags = _ctx._flags;

        if (flags & DirtyMemoryCache)
            return;

        _viewId = view;
        _viewCurrent = _views[_viewId];

        // Check view boundaries
        u32     viewSizeInBytes = _viewCurrent->TotalItems << 2;
        while (_ctx._cursorAddress >= _ctx._address + viewSizeInBytes)
            _ctx._address += _viewCurrent->Stride;

        // Clear items cache
        _ctx._items.clear();
        // Set flags to refresh view
        flags |= UpdateView | DirtyCursor;
    }

    void    HexEditor::Refresh(void)
    {
        _ctx._flags |= DirtySrc;
    }

    void    HexEditor::_ApplyChanges(void)
    {
        if (!(_ctx._flags & DirtyMemoryCache) || _ctx._flags & InvalidSrc)
            return;

        for (Item &item : _ctx._items)
            *(vu32 *)item.address = item.value32;

        _ctx._flags &= ~DirtyMemoryCache;
        _ctx._flags |= DirtyViewCache;
    }

    void    HexEditor::_DiscardChanges(void)
    {
        for (Item& item : _ctx._items)
            item.value32 = *(vu32 *)item.address;

        _ctx._flags &= ~DirtyMemoryCache;
        _ctx._flags |= DirtyViewCache;
    }

    u32     HexEditor::_PromptForAddress(int mode)
    {
        static const char *msg[2] = {"Enter the address to jump to:", "Enter the offset to jump to:"};

        Keyboard    keyboard;

        const Color     &bgMain = Preferences::Settings.BackgroundMainColor;
        const Color     &bgSecondary = Preferences::Settings.BackgroundSecondaryColor;
        const Color     &skyblue = Color::SkyBlue;
        static IntRect  background(93, 95, 213, 50);

        Renderer::SetTarget(TOP);

        // Draw "window" background
        Renderer::DrawRect2(background, bgMain, bgSecondary);

        int posY = 115;
        Renderer::DrawString(msg[mode], 98, posY, skyblue);

        // We write to second framebuffer too because of keyboard below
        Renderer::EndFrame();

        Renderer::SetTarget(TOP);
        Renderer::DrawRect2(background, bgMain, bgSecondary);
        posY = 115;
        Renderer::DrawString(msg[mode], 98, posY, skyblue);

        keyboard.DisplayTopScreen = false;

        u32 address = mode == 0 ? _ctx._address : 0;

        // Enable clear key
        _keyboard._keys->at(15).Enable(true);
        // Enable enter key
        _keyboard._keys->at(16).Enable(true);
        // Enable cursor
        _keyboard._showCursor = true;

        // If user aborted, return 0
        if (keyboard.Open(address, address) == -1)
            address = 0;
        else if (mode == JumpRelative)
            address += _ctx._cursorAddress;

        // Disable clear key
        _keyboard._keys->at(15).Enable(false);
        // Disable enter key
        _keyboard._keys->at(16).Enable(false);
        // Disable cursor
        _keyboard._showCursor = false;

        return address;
    }

    void    HexEditor::_JumpTo(int mode)
    {
        u32 address;

        if (mode == JumpToValue)
        {
            address = _GetSelectedItem().value32;

            if (!(MessageBox("Confirm",
                Utils::Format("Do you want to jump to: 0x%08X ?", address), DialogType::DialogYesNo))())
                return;
        }
        else
        {
            address = _PromptForAddress(mode);
            if (!address)
                return;
        }

        if (Goto(address, true))
        {
            _history.push_back(address);
            _indexHistory = _history.size() - 1;
        }
    }

    void    HexEditor::_GotoPreviousRegion(void)
    {
        if (_ctx._flags & DirtyMemoryCache || _submenu.IsOpen())
            return;

        MemInfo     currentRegion = ProcessImpl::GetMemRegion(_ctx._items.front().address);
        MemInfo     previousRegion = ProcessImpl::GetPreviousRegion(currentRegion);

        Goto(previousRegion.base_addr, true);
    }

    void    HexEditor::_GotoNextRegion(void)
    {
        if (_ctx._flags & DirtyMemoryCache || _submenu.IsOpen())
            return;

        MemInfo     currentRegion = ProcessImpl::GetMemRegion(_ctx._items.front().address);
        MemInfo     nextRegion = ProcessImpl::GetNextRegion(currentRegion);

        Goto(nextRegion.base_addr, true);
    }


    HexEditor::Item&   HexEditor::_GetSelectedItem(void)
    {
        u32 item = _ctx._cursorY * _viewCurrent->ItemsPerLine;

        if (_viewCurrent->ItemsPerLine > 1)
            item += _ctx._cursorX >> 3;

        return _ctx._items[item];
    }

    /*
    ** Context
    */

    bool    HexEditor::Context::ScrollUp(u32 stride, bool updatecursor)
    {
        u32     target = _address - stride;
        // If we're already the upper possible, no scroll
        if (target < 0x00100000)
            return false;

        MemInfo     region = ProcessImpl::GetMemRegion(_address);

        // If we're at the top of the current region, fetch previous region
        if (target < region.base_addr)
        {
            region = ProcessImpl::GetPreviousRegion(region);
            target = region.base_addr + region.size - stride;
        }

        // Scroll
        __g_hexEditor->Goto(target, updatecursor);

        return true;
    }

    bool    HexEditor::Context::ScrollDown(u32 stride, bool updatecursor)
    {
        u32     target = _address + stride;
        u32     targetEnd = target + 10 * stride;

        MemInfo     region = ProcessImpl::GetMemRegion(_address);
        MemInfo     nextregion = ProcessImpl::GetNextRegion(region);

        // If we're already at the last region, don't scroll outside it
        if (region == nextregion && targetEnd >= region.base_addr + region.size)
            target = region.base_addr + region.size - 10 * stride;
        else if (target >= region.base_addr + region.size)
            target = nextregion.base_addr;

        // Scroll
        __g_hexEditor->Goto(target, updatecursor);

        return false;
    }

    /*
    ** View helpers
    */

    static inline s16   Min16(s16 left, s16 right)
    {
        return std::min(left, right);
    }

    static inline s16   Max16(s16 left, s16 right)
    {
        return std::max(left, right);
    }

    static inline s16   Wrap16(s16 limit, s16 wrap, s16 curval, s16 newval)
    {
        return curval == limit ? wrap : newval;
    }

    void     HexEditor::IView::ProcessEventsCommon(Context& _ctx, Event& event, s16 maxX)
    {
        // Key pressed
        if (event.type == Event::EventType::KeyPressed
            || (event.type == Event::KeyDown && _ctx._clock.HasTimePassed(_ctx._latency)))
        {
            _ctx._clock.Restart();

            u32&    flags = _ctx._flags;
            u32     key = event.key.code;
            s16 &   _cursorX = _ctx._cursorX;
            s16 &   _cursorY = _ctx._cursorY;
            bool    canScroll = !(_ctx._flags & DirtyMemoryCache);

            // Scroll up
            if (key == Key::DPadUp)
            {
                // If we're already at the first line
                if (_cursorY == 0)
                {
                    if (canScroll)
                        _ctx.ScrollUp(Stride, false);
                }
                // Otherwise, just move cursor
                else
                {
                    --_cursorY;
                    flags |= DirtyCursorAddress | DirtyCursorPos;
                }
            }

            // Scroll down
            if (key == Key::DPadDown)
            {
                // If we're already at the bottom line
                if (_cursorY == 9)
                {
                    if (canScroll)
                        _ctx.ScrollDown(Stride, false);
                }
                // Otherwise, just move cursor
                else
                {
                    ++_cursorY;
                    flags |= DirtyCursorAddress | DirtyCursorPos;
                }
            }
            // Scroll left
            if (key == Key::DPadLeft)
            {
                s16 cursor = _cursorX;

                _cursorX = Wrap16(0, maxX, _cursorX, _cursorX - 1);

                // Check if we jumped to previous line
                if (_cursorX > cursor)
                {
                    // If we have to scroll up
                    if (_cursorY == 0)
                    {
                        // If we can't scroll up, abort
                        if (!canScroll || !_ctx.ScrollUp(Stride, false))
                            _cursorX = cursor;
                        else
                            _cursorX = maxX;
                    }
                    else
                        --_cursorY;
                }

                flags |= DirtyCursorAddress | DirtyCursorPos;
            }
            // Scroll right
            if (key == Key::DPadRight)
            {
                s16 cursor = _cursorX;

                _cursorX = Wrap16(maxX, 0, _cursorX, _cursorX + 1);

                // Check if we jumped to next line
                if (_cursorX < cursor)
                {
                    // If we have to scroll down
                    if (_cursorY == 9)
                    {
                        // If we can't scroll down, abort
                        if (!canScroll || !_ctx.ScrollDown(Stride, false))
                            _cursorX = cursor;
                        else
                            _cursorX = 0;
                    }
                    else
                        ++_cursorY;
                }

                flags |= DirtyCursorAddress | DirtyCursorPos;
            }
        }
    }

    /*
    ** ByteView
    */

    HexEditor::ByteView::ByteView(Context& ctx) :
         _ctx{ctx}
    {
        ItemsPerLine = 2;
        DigitPerLine = 16;
        TotalItems = 20;
        Stride = 8;
    }

    void    HexEditor::ByteView::Draw(void)
    {
        int   posY = 61;

        // Draw value header
        Renderer::DrawRect(111, posY, 178, 20, Color::DeepSkyBlue);
        posY += 5;
        Renderer::DrawString("00 01 02 03   04 05 06 07", 125, posY, Color::Black);

        // Values body
        posY += 6;
        Renderer::DrawRect(111, posY, 178, 100, Color::White);

        posY -= 21;
        // Characters header
        Renderer::DrawRect(290, posY, 66, 20, Color::DeepSkyBlue);
        // Characters body
        posY += 21;
        Renderer::DrawRect(290, posY, 66, 100, Color::DeepSkyBlue);

        // Draw Cursor
        Renderer::DrawRect(_ctx._cursorPosX, _ctx._cursorPosY, 7, 10, Color::DeepSkyBlue);

        // Draw items
        int posYLeft = posY;
        int posYRight = posY;
        int posXLeft = 125;
        int posXRight = 209;

        int idx = 0;
        for (Item& item : _ctx._items)
        {
            int  posXX = idx & 1 ? posXRight : posXLeft;
            int& posYY = idx & 1 ? posYRight : posYLeft;

            // Values
            Renderer::DrawString(item.valueCache.c_str(), posXX, posYY, Color::Black);

            // Data
            posXX += idx & 1 ? 115 : 175;
            posYY -= 10;
            Renderer::DrawString(item.dataCache.c_str(), posXX, posYY, Color::Black);

            ++idx;
        }
    }

    void    HexEditor::ByteView::ProcessEvent(Event &event)
    {
        ProcessEventsCommon(_ctx, event, 15);
    }

    void    HexEditor::ByteView::UpdateView(void)
    {
        u32&    flags = _ctx._flags;

        // Check if cursor must be placed at a specified address
        if (flags & DirtyCursor)
        {
            flags &= ~DirtyCursor;
            flags |= (DirtyCursorPos | DirtyCursorCache);

            u32     start = _ctx._address;
            u32     target = _ctx._cursorAddress - start;

            u32     shiftY = __builtin_ctz(Stride);
            u32     shiftX = __builtin_clz(Stride);
            _ctx._cursorX = target << shiftX >> shiftX << 1;
            _ctx._cursorY = target >> shiftY;           /// _ctx._cursorY = target / _viewCurrent->Stride
        }

        // Check if cache needs to be refreshed
        if (flags & DirtyViewCache)
        {
            flags &= ~DirtyViewCache;

            for (Item& item: _ctx._items)
            {
                bool            red = false;
                u8            * b = &item.b[0];
                std::string&    value = item.valueCache;
                std::string&    data = item.dataCache;

                value.clear();
                data.clear();

                union
                {
                    u32     origin32;
                    u8      ob[4];
                };

                origin32 = *(vu32 *)item.address;

                for (u32 i = 0; i < 4; ++i)
                {
                    if (item.b[i] != ob[i] && !red)
                    {
                        value += Color::Red;
                        red = true;
                    }
                    else if (red)
                    {
                        value += Color::Black;
                        red = false;
                        flags |= DirtyMemoryCache;
                    }

                    value += Utils::Format("%02X ", b[i]);
                    data += b[i] >= 32 && b[i] <= 126 ? b[i] : '.';
                }
            }
        }

        // Check if cursor address needs to be refreshed
        if (flags & DirtyCursorAddress)
        {
            flags &= ~DirtyCursorAddress;
            flags |= DirtyCursorCache;

            _ctx._cursorAddress = _ctx._items[_ctx._cursorY << 1].address;
            _ctx._cursorAddress += _ctx._cursorX >> 1;
        }

        // Check if the position of the cursor has to be computed
        if (flags & DirtyCursorPos)
        {
            flags &= ~DirtyCursorPos;

            // posX
            {
                u32     startX = 124;
                u32     byteWidth = 3 * 6;
                u32     bytes = _ctx._cursorX >> 1;

                _ctx._cursorPosX = startX + bytes * byteWidth
                                + ((_ctx._cursorX & 1) ? 6 : 0)
                                + ((_ctx._cursorX >> 3) > 0 ? 12 : 0);
            }

            // posY
            {
                u32     startY = 82;
                u32     lineHeight = 10;
                u32     lines = _ctx._cursorY;

                _ctx._cursorPosY = startY + lines * lineHeight;
            }
        }
    }

    void    HexEditor::ByteView::EditValueAtCursor(u32 val)
    {
        // Edit value
        {
            u32     itemIdx = (_ctx._cursorY << 1) + (_ctx._cursorX > 7 ? 1 : 0);
            Item&   item = _ctx._items[itemIdx];
            u32     cursor = _ctx._cursorX > 7 ? _ctx._cursorX - 8 : _ctx._cursorX;

            cursor = (cursor & 1 ? cursor - 1 : cursor + 1) << 2;
            item.value32 &= ~(0xF << cursor);
            item.value32 |= val << cursor;
        }

        // Move cursor
        s16&    _cursorY = _ctx._cursorY;
        s16&    _cursorX = _ctx._cursorX;
        s16     cursor = _cursorX;

        // Advance cursor
        _cursorX = Wrap16(15, 0, _cursorX, _cursorX + 1);

        // Check if we jumped to next line
        if (_cursorX < cursor)
        {
            // If we have to scroll down
            if (_cursorY == 9)
            {
                // If we can't scroll down, abort
                if (_ctx._flags & DirtyMemoryCache || !_ctx.ScrollDown(Stride, false))
                    _cursorX = cursor;
            }
            else
                ++_cursorY;
        }

        _ctx._flags |= DirtyMemoryCache | DirtyViewCache | DirtyCursorAddress | DirtyCursorPos;
    }

    /*
    ** IntegerView
    */

    HexEditor::IntegerView::IntegerView(Context& ctx) :
        _ctx{ctx}
    {
        ItemsPerLine = 4;
        DigitPerLine = 32;
        TotalItems = 40;
        Stride = 16;
    }

    void    HexEditor::IntegerView::Draw(void)
    {
        int   posY = 61;

        // Draw value header
        Renderer::DrawRect(111, posY, 245, 20, Color::DeepSkyBlue);
        posY += 21;

        // Values body
        Renderer::DrawRect(111, posY, 245, 100, Color::White);

        // Draw Cursor
        Renderer::DrawRect(_ctx._cursorPosX, _ctx._cursorPosY, 7, 10, Color::DeepSkyBlue);

        // Draw items
        auto item = _ctx._items.begin();

        for (; item != _ctx._items.end();)
        {
            int posX = 121;

            for (int i = 0; i < 4; ++i, posX += 58)
            {
                int posYY = posY;

                // Values
                Renderer::DrawString(item++->valueCache.c_str(), posX, posYY, Color::Black);
            }
            posY += 10;
        }
    }

    void    HexEditor::IntegerView::ProcessEvent(Event &event)
    {
        ProcessEventsCommon(_ctx, event, 31);
    }

    void    HexEditor::IntegerView::UpdateView(void)
    {
        u32&    flags = _ctx._flags;

        // Check if cursor must be placed at a specified address
        if (flags & DirtyCursor)
        {
            flags &= ~DirtyCursor;
            flags |= (DirtyCursorPos | DirtyCursorCache);

            u32     start = _ctx._address;
            u32     target = _ctx._cursorAddress - start;
            u32     shift = 31 - __builtin_clz(Stride);
            u32     targetX = (target - (target >> shift << shift)) << 1; /// target % Stride * 2

            _ctx._cursorX = targetX >> 3 << 3; /// targetX / 8
            _ctx._cursorX += 6 - (targetX - _ctx._cursorX); /// 7 - targetX % 8
            _ctx._cursorY = target >> shift; /// _ctx._cursorY = target / Stride
        }

        // Check if cache needs to be refreshed
        if (flags & DirtyViewCache)
        {
            flags &= ~DirtyViewCache;

            for (Item& item: _ctx._items)
            {
                //bool            red = false;
                std::string&    value = item.valueCache;
                std::string&    data = item.dataCache;

                value.clear();
                data.clear();

                u32 origin = *(vu32 *)item.address;

                if (item.value32 != origin)
                {
                    value += Color::Red;
                    flags |= DirtyMemoryCache;
                }
                value += Utils::Format("%08X", item.value32);
            }
        }

        // Check if cursor address needs to be refreshed
        if (flags & DirtyCursorAddress)
        {
            flags &= ~DirtyCursorAddress;
            flags |= DirtyCursorCache;

            u32     cursorX = (_ctx._cursorX >> 3) << 3;
            _ctx._cursorAddress = cursorX >> 1;
            _ctx._cursorAddress += 3 - ((_ctx._cursorX - cursorX) >> 1);
            _ctx._cursorAddress += _ctx._items[_ctx._cursorY << 2].address;
        }

        // Check if the position of the cursor has to be computed
        if (flags & DirtyCursorPos)
        {
            flags &= ~DirtyCursorPos;

            // posX
            {
                u32     startX = 120;
                u32     byteWidth = 12;
                u32     bytes = _ctx._cursorX >> 1;
                u32     space = (_ctx._cursorX >> 3) * 10;

                _ctx._cursorPosX = startX + bytes * byteWidth + space
                                + ((_ctx._cursorX & 1) ? 6 : 0);
            }

            // posY
            {
                u32     startY = 82;
                u32     lineHeight = 10;
                u32     lines = _ctx._cursorY;

                _ctx._cursorPosY = startY + lines * lineHeight;
            }
        }
    }

    void    HexEditor::IntegerView::EditValueAtCursor(u32 val)
    {
        // Edit value
        {
            u32     xIdx = _ctx._cursorX >> 3;
            u32     itemIdx = (_ctx._cursorY << 2) + xIdx;
            Item&   item = _ctx._items[itemIdx];
            u32     cursor = (7 - (_ctx._cursorX - (xIdx << 3))) << 2;

            item.value32 &= ~(0xF << cursor);
            item.value32 |= val << cursor;
        }

        // Move cursor
        s16&    _cursorY = _ctx._cursorY;
        s16&    _cursorX = _ctx._cursorX;
        s16     cursor = _cursorX;

        // Advance cursor
        _cursorX = Wrap16(31, 0, _cursorX, _cursorX + 1);

        // Check if we jumped to next line
        if (_cursorX < cursor)
        {
            // If we have to scroll down
            if (_cursorY == 9)
            {
                // If we can't scroll down, abort
                if (_ctx._flags & DirtyMemoryCache || !_ctx.ScrollDown(Stride, false))
                    _cursorX = cursor;
            }
            else
                ++_cursorY;
        }

        _ctx._flags |= DirtyMemoryCache | DirtyViewCache | DirtyCursorAddress | DirtyCursorPos;
    }


    /*
    ** FloatView
    */

    HexEditor::FloatView::FloatView(Context& ctx) :
        _ctx{ctx}
    {
        ItemsPerLine = 2;
        DigitPerLine = 16;
        TotalItems = 20;
        Stride = 8;
    }

    void    HexEditor::FloatView::Draw(void)
    {
    }

    void    HexEditor::FloatView::ProcessEvent(Event &event)
    {
    }

    void    HexEditor::FloatView::UpdateView(void)
    {
    }

    void    HexEditor::FloatView::EditValueAtCursor(u32 val)
    {
    }

    /*
    ** AsmView
    */

    HexEditor::AsmView::AsmView(Context& ctx) :
        _ctx{ctx}
    {
        ItemsPerLine = 1;
        DigitPerLine = 8;
        TotalItems = 10;
        Stride = 4;
    }

    void    HexEditor::AsmView::Draw(void)
    {
        int   posY = 61;

        // Draw value header
        Renderer::DrawRect(111, posY, 58, 20, Color::DeepSkyBlue);
        posY += 21;

        Renderer::DrawRect(111, posY, 58, 100, Color::White);

        posY -= 21;
        // Characters header
        Renderer::DrawRect(170, posY, 186, 20, Color::DeepSkyBlue);
        // Characters body
        posY += 21;
        Renderer::DrawRect(170, posY, 186, 100, Color::DeepSkyBlue);

        // Draw Cursor
        Renderer::DrawRect(_ctx._cursorPosX, _ctx._cursorPosY, 7, 10, Color::DeepSkyBlue);

        // Draw items
        for (Item& item : _ctx._items)
        {
            int posYY = posY;
            // Values
            Renderer::DrawString(item.valueCache.c_str(), 116, posY, Color::Black);

            // Data
            Renderer::DrawString(item.dataCache.c_str(), 175, posYY, Color::Black);
        }
    }

    void    HexEditor::AsmView::ProcessEvent(Event &event)
    {
        ProcessEventsCommon(_ctx, event, 7);
    }

    void    HexEditor::AsmView::UpdateView(void)
    {
        u32&    flags = _ctx._flags;

        // Check if cursor must be placed at a specified address
        if (flags & DirtyCursor)
        {
            flags &= ~DirtyCursor;
            flags |= (DirtyCursorPos | DirtyCursorCache);

            u32     start = _ctx._address;
            u32     target = _ctx._cursorAddress - start;
            u32     targetX = (target % Stride) << 1;

            u32     shiftY = 31 - __builtin_clz(Stride);
            _ctx._cursorX = 6 - targetX;
            _ctx._cursorY = target >> shiftY;           /// _ctx._cursorY = target / _viewCurrent->Stride
        }

        // Check if cache needs to be refreshed
        if (flags & DirtyViewCache)
        {
            flags &= ~DirtyViewCache;

            for (Item& item: _ctx._items)
            {
                //bool            red = false;
                std::string&    value = item.valueCache;
                std::string&    data = item.dataCache;

                value.clear();
                data.clear();

                u32 origin = *(vu32 *)item.address;

                if (item.value32 != origin)
                {
                    value += Color::Red;
                    flags |= DirtyMemoryCache;
                }

                value += Utils::Format("%08X", item.value32);
                data = ARM_Disasm::Disassemble(item.address, item.value32);
            }
        }

        // Check if cursor address needs to be refreshed
        if (flags & DirtyCursorAddress)
        {
            flags &= ~DirtyCursorAddress;
            flags |= DirtyCursorCache;

            _ctx._cursorAddress = _ctx._items[_ctx._cursorY].address;
            _ctx._cursorAddress += 3 - (_ctx._cursorX >> 1);
        }

        // Check if the position of the cursor has to be computed
        if (flags & DirtyCursorPos)
        {
            flags &= ~DirtyCursorPos;

            // posX
            {
                u32     startX = 115;
                u32     byteWidth = 12;
                u32     bytes = _ctx._cursorX >> 1;

                _ctx._cursorPosX = startX + bytes * byteWidth
                                + ((_ctx._cursorX & 1) ? 6 : 0);
            }

            // posY
            {
                u32     startY = 82;
                u32     lineHeight = 10;
                u32     lines = _ctx._cursorY;

                _ctx._cursorPosY = startY + lines * lineHeight;
            }
        }
    }

    void    HexEditor::AsmView::EditValueAtCursor(u32 val)
    {
        // Edit value
        {
            u32     itemIdx = _ctx._cursorY;
            Item&   item = _ctx._items[itemIdx];
            u32     cursor = (7 - _ctx._cursorX) << 2;

            item.value32 &= ~(0xF << cursor);
            item.value32 |= val << cursor;
        }

        // Move cursor
        s16&    _cursorY = _ctx._cursorY;
        s16&    _cursorX = _ctx._cursorX;
        s16     cursor = _cursorX;

        // Advance cursor
        _cursorX = Wrap16(7, 0, _cursorX, _cursorX + 1);

        // Check if we jumped to next line
        if (_cursorX < cursor)
        {
            // If we have to scroll down
            if (_cursorY == 9)
            {
                // If we can't scroll down, abort
                if (_ctx._flags & DirtyMemoryCache || !_ctx.ScrollDown(Stride, false))
                    _cursorX = cursor;
            }
            else
                ++_cursorY;
        }

        _ctx._flags |= DirtyMemoryCache | DirtyViewCache | DirtyCursorAddress | DirtyCursorPos;
    }
}
