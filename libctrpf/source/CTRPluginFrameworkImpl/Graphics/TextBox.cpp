#include "types.h"
#include <3ds.h>
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Graphics/TextBox.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFramework/System/Clock.hpp"
#include <cmath>

namespace CTRPluginFramework
{
    TextBox::TextBox() :
        _text{ nullptr }, _isOpen{ false }, _displayScrollbar{ false }, _fastscroll{ false }, _currentLine{ 0 },
        _maxLines{ 0 }, _scrollbarSize{ 0 }, _scrollCursorSize{ 0 }, _maxScrollCursorPosY{ 0 }, _scrollPadding{ 0 },
        _scrollPosition{ 0 }
    {
        _drawBox = true;
    }

    /*
    ** Constructor
    ***************/
    TextBox::TextBox(const std::string &title, const std::string &text, const IntRect &box) :
    _title(title), _text(&text), _box(box), _border(IntRect(box.leftTop.x + 2, box.leftTop.y + 2, box.size.x - 4, box.size.y - 4))
    {
        _currentLine = 0;
        _isOpen = false;
        _maxLines = ((box.size.y - 10) / 16) - 1;
        _GetTextInfos();
        if (_newline.size() <= _maxLines)
        {
            _maxLines = _newline.size();
            _displayScrollbar = false;
        }
        else
        {
            int height = _box.size.y;

            height -= 10;

            int lines = _newline.size() + 1;
            int lsize = 16 * lines;

            float padding = (float)height / (float)lsize;
            int cursorSize =  padding * height;
            _scrollbarSize = height;

            if (cursorSize < 5)
                cursorSize = 5;

            _scrollCursorSize = cursorSize;
            _maxScrollCursorPosY = _box.leftTop.y + 5 + _scrollbarSize - cursorSize;
            _scrollPadding = padding * 16.f;
            _scrollPosition = 0.f;
            _displayScrollbar = true;
        }
        _fastscroll = _drawBox = true;
    }

    /*
    ** Open
    ************/
    void    TextBox::Open(void) const
    {
        _isOpen = true;
    }

    /*
    ** Close
    ***********/
    void    TextBox::Close(void) const
    {
        _isOpen = false;
    }

    /*
    ** IsOpen
    ***********/
    bool    TextBox::IsOpen(void) const
    {
        return (_isOpen);
    }

    /*
    ** Process Event
    *****************/
    bool    TextBox::ProcessEvent(Event &event)
    {
        if (!_isOpen)
            return (false);

        if (event.type == Event::KeyDown)
        {
            switch (event.key.code)
            {
                case Key::DPadUp:
                case Key::CPadUp:
                {
                    if (_inputClock.HasTimePassed(Milliseconds(100)))
                    {
                        if (_currentLine > 0)
                        {
                            _currentLine--;
                            _scrollPosition -= _scrollPadding;; //_scrollPadding;
                        }
                        //else
                         //   _currentLine = _newline.size() - _maxLines;
                        _inputClock.Restart();
                    }
                    break;
                }
                case Key::DPadDown:
                case Key::CPadDown:
                {
                    if (_inputClock.HasTimePassed(Milliseconds(100)))
                    {
                        if (_currentLine < _newline.size() - _maxLines)
                        {
                            _currentLine++;
                            _scrollPosition += _scrollPadding;
                        }
                        //else
                        //    _currentLine = 0;
                        _inputClock.Restart();
                    }
                    break;
                }
                case Key::DPadLeft:
                {
                    if (_fastscroll)
                    {
                        _scrollPosition = 0.f;
                        _currentLine = 0;
                    }
                    break;
                }
                case Key::DPadRight:
                {
                    if (_fastscroll)
                    {
                        _scrollPosition = (_newline.size() - _maxLines) * _scrollPadding;
                        _currentLine = std::max((int)(_newline.size() - _maxLines), 0);
                    }
                    break;
                }
                case Key::CStickUp:
                {
                    if (_inputClock.HasTimePassed(Milliseconds(100)))
                    {
                        if (_currentLine > 1)
                        {
                            _currentLine -= 2;
                            _scrollPosition -= _scrollPadding * 2;
                        }
                        //else
                         //   _currentLine = _newline.size() - _maxLines;
                        _inputClock.Restart();
                    }
                    break;
                }
                case Key::CStickDown:
                {
                    if (_inputClock.HasTimePassed(Milliseconds(100)))
                    {
                        if (_currentLine < _newline.size() - _maxLines)
                        {
                            _currentLine += 2;
                            _scrollPosition += _scrollPadding * 2;
                        }
                        //else
                        //    _currentLine = 0;
                        _inputClock.Restart();
                    }
                    break;
                }
                case Key::B:
                {
                    Close();
                    return (false);
                    break;
                }
                default: break;
            }
        }
        return (true);
    }

    void    TextBox::Update(const std::string& title, std::string& text)
    {
        _currentLine = 0;
        _title = title;
        _text = &text;
        _maxLines = ((_box.size.y - 10) / 16);
        if (!title.empty())
            _maxLines -= 1;
        _GetTextInfos();
        if (_newline.size() - 1 <= _maxLines)
        {
            _maxLines = _newline.size();
            _displayScrollbar = false;
        }
        else
        {
            int height = _box.size.y;

            height -= 10;

            int lines = _newline.size() - 1;
            int lsize = 16 * lines;

            float padding = (float)height / (float)lsize;
            int cursorSize = padding * height;
            _scrollbarSize = height;

            if (cursorSize < 5)
                cursorSize = 5;

            _scrollCursorSize = cursorSize;
            _scrollPadding = padding * 16.f;
            _scrollPosition = 0.f;
            _maxScrollCursorPosY = _box.leftTop.y + 5 + _scrollbarSize - cursorSize;
            _displayScrollbar = true;
        }
    }

    /*
    ** Draw
    ***********/
    namespace RendererPriv
    {
        extern Color g_customColor;
    }

    void    TextBox::Draw(void)
    {
        if (_currentLine >= _newline.size())
            _currentLine = 0;

        int  max = std::min((int)(_currentLine + _maxLines), (int)(_newline.size() - 1));

        FwkSettings     &settings = Preferences::Settings;
        //const Color     &titleColor = settings.WindowTitleColor;
        const Color     &textColor = settings.MainTextColor;
        const Color     &blank = Color::White;


        if (_drawBox)
        {
            // Draw Background
            if (Preferences::topBackgroundImage != nullptr
                && (Preferences::topBackgroundImage->GetDimensions() <= _box.size))
                Preferences::topBackgroundImage->Draw(_box, -0.3f);
            else
            {
                Renderer::DrawRect2(_box, settings.BackgroundMainColor, settings.BackgroundSecondaryColor);
                Renderer::DrawRect(_border, settings.BackgroundBorderColor, false);
            }
        }

        //Window::TopWindow.Draw(_title);

        int posX = _box.leftTop.x + 5;
        int posY = _box.leftTop.y + 5;
        int xLimit = posX + _box.size.x - 10;

        // Draw Title
        if (!_title.empty())
        {
            int width;
            width = Renderer::DrawSysString((const char *)_title.c_str(), posX, posY, xLimit, settings.WindowTitleColor);
            int length = width - posX + 30;
            if (posX + length > xLimit)
                length = xLimit - posX;
            Renderer::DrawLine(posX, posY, length, blank);
            posY += 7;
        }
        else
            posY += 5;

        // Draw Text
        RendererPriv::g_customColor = textColor;
        for (int i = _currentLine; i < max; i++)
        {
            Renderer::DrawSysString((char *)_newline[i], posX, posY, xLimit, RendererPriv::g_customColor, 0, (char *)_newline[i + 1]);
        }

        if (!_displayScrollbar)
            return;

        // Draw scroll bar
        const Color  &dimGrey = Color::DimGrey;
        const Color  &silver = Color::Silver;

        // Background
        posX = _box.leftTop.x + _box.size.x - 8;
        posY = _box.leftTop.y + 5;

        Renderer::DrawLine(posX, posY + 1, 1, silver, _scrollbarSize - 2);
        Renderer::DrawLine(posX + 1, posY, 1, silver, _scrollbarSize);
        Renderer::DrawLine(posX + 2, posY + 1, 1, silver, _scrollbarSize - 2);

        posY += (int)(_scrollPosition);// * _scrollbarSize);

        posY = std::min((u32)posY, _maxScrollCursorPosY);

        Renderer::DrawLine(posX, posY + 1, 1, dimGrey, _scrollCursorSize - 2);
        Renderer::DrawLine(posX + 1, posY, 1, dimGrey, _scrollCursorSize);
        Renderer::DrawLine(posX + 2, posY + 1, 1, dimGrey, _scrollCursorSize - 2);
    }

    void    TextBox::DrawConst(void) const
    {
        TextBox *nonconst = const_cast<TextBox *>(this);
        nonconst->Draw();
    }

    void    TextBox::Update(const bool isTouchDown, const IntVector &pos)
    {
    }

    /*
    ** Text infos
    ***************/

    u8   *TextBox::_GetWordWidth(u8 *str, float &width)
    {
        width = 0;

        if (!str || !(*str))
            return (nullptr);

        while (*str != '\n')
        {
            if (*str == 0x18)
            {
                str++;
                continue;
            }
            if (*str == 0x1B)
            {
                str += 4;
                continue;
            }

            bool isSpace = *str == ' ' ? true : false;
            Glyph *glyph = Font::GetGlyph(str);

            if (glyph == nullptr)
            {
                return (nullptr);
            }


            width += glyph->Width();

            if (isSpace)
                break;
        }
        return (str);
    }

    u8   *CutWordWidth(u8 *str, float &width, float maxWidth)
    {
        width = 0;

        if (!str || !(*str))
            return (nullptr);

        while (*str != '\n')
        {
            if (*str == 0x18)
            {
                str++;
                continue;
            }
            if (*str == 0x1B)
            {
                str += 4;
                continue;
            }

            bool isSpace = *str == ' ' ? true : false;

            u8      *strBak = str;
            Glyph   *glyph = Font::GetGlyph(str);

            if (glyph == nullptr)
            {
                return (nullptr);
            }

            width += glyph->Width();

            if (isSpace)
                break;

            if (width > maxWidth)
            {
                str = strBak;
                break;
            }
        }
        return (str);
    }

    /*
    ** Get text infos
    ******************/
    void     TextBox::_GetTextInfos(void)
    {
        _newline.clear();

        u8      *str = const_cast<u8 *>(reinterpret_cast<const u8*>(_text->c_str()));

        const float maxWidth = static_cast<float>(_border.size.x) - 12.f;

        _newline.push_back(str);

        // Skip UTF8 sig
        if (str[0] == 0xEF && str[1] == 0xBB && str[2] == 0xBF)
            str += 3;

        int advance = 0;
        while (*str == '\n')
        {
            _newline.push_back(str);
            str++;
            advance++;
        }

        while (str && *str)
        {
            if (*str == '\n')// || *str == 0x18)
            {
                str++;
            }
           /* if (*str == 0x1B)
            {
                str += 4;
            }*/

            u8 *s = str;
            float line = 0.f;

            while (line < maxWidth && s != nullptr)
            {
                str = s;
                float wordWidth;

                s = _GetWordWidth(s, wordWidth);
                // If word's width is greater than line's width
                if (wordWidth > maxWidth)
                {
                    str = CutWordWidth(str, wordWidth, maxWidth);
                    break;
                }

                line += wordWidth;

                // if we are at the end of the text
                if (s == nullptr)
                {
                    // if line is shorter than the border
                    if (line < maxWidth)
                    {
                        goto exit;
                    }
                    _newline.push_back(str);
                    goto exit;
                }

                // If we have a new line symbol
                if (*s == '\n' && line < maxWidth)
                {
                    str = s;
                    break;
                }

                // If the space char is out of border, skip it and go to a new line
                if (line >= maxWidth)
                {
                    break;
                }
            }
            _newline.push_back(str);
        }
    exit:
        if (!_newline.empty() && _newline.back() != nullptr)
            _newline.push_back(nullptr);
    }
}
