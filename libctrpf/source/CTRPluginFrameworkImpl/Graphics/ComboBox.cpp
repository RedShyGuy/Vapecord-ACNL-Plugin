#include "CTRPluginFrameworkImpl/Graphics/ComboBox.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"

namespace CTRPluginFramework
{
    ComboBox::ComboBox(int posX, int posY, int width, int height) :
    IsEnabled(true),
    IsVisible(true),
    SelectedItem(-1),
    _execute(false),
    _isTouched(false),
    _rectPos(IntRect(posX, posY, width, height))
    {

    }

    void    ComboBox::Add(const std::string &item)
    {
        _items.push_back(item);

        if (SelectedItem == -1)
            SelectedItem = 0;
    }

    void    ComboBox::Clear(void)
    {
        _items.clear();
        SelectedItem = -1;
    }

    void    ComboBox::Draw(void)
    {
       // static Color    blank = Color(255, 255, 255);
        const Color    &black = Color::Black;
        const Color    &gainsboro = Color::Gainsboro;
        const Color    &grey = Color::Gray;

        if (!IsVisible)
            return;

        // Draw background
        Renderer::DrawRect(_rectPos, IsEnabled ? gainsboro : grey);


        if (SelectedItem == -1)
            return;

        // Draw text
        int posX = _rectPos.leftTop.x + 5;
        int posY = _rectPos.leftTop.y;

        posY += (_rectPos.size.y - 10) / 2;

        Renderer::DrawString((char *)_items[SelectedItem].c_str(), posX, posY, black);
    }

    void    ComboBox::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        if (!_items.size() || !IsEnabled)
            return;
        if (!_isTouched && isTouchDown && _rectPos.Contains(touchPos))
            _isTouched = true;

        if (_isTouched && isTouchDown && !_rectPos.Contains(touchPos))
            _isTouched = false;

        if (_isTouched && !isTouchDown)
        {
            _execute = true;
            _isTouched = false;
        }
    }

    bool    ComboBox::operator()(void)
    {
        if (_items.size() && IsEnabled && _execute)
        {
            Keyboard  keyboard;

            keyboard.Populate(_items);

            keyboard.DisplayTopScreen = false;

            int  out = keyboard.Open();

            if (out != -1)
            {
                SelectedItem = out;
            }

            _execute = false;
            return (true);
        }
        return (false);
    }
}
