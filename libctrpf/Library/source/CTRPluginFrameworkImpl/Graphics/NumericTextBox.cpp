#include "CTRPluginFrameworkImpl/Graphics/NumericTextBox.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"

#include <cstdio>
#include "CTRPluginFramework/Utils/Utils.hpp"

namespace CTRPluginFramework
{
    NumericTextBox::NumericTextBox(int posX, int posY, int width, int height) :

    IsEnabled(true),
    IsVisible(true),
    ValueType(Type::Bits32),
    Bits32(0),
    _execute(false),
    _isTouched(false),
	_isHexadecimal(true),
    _rectPos(IntRect(posX, posY, width, height))
    {
        Clear();
    }

    void    NumericTextBox::_UpdateVal(void)
    {
		/*if (_isHexadecimal)
		{
			switch (ValueType)
			{
			case Type::Bits8: sprintf(buffer, "%08X", Bits8); break;
			case Type::Bits16: sprintf(buffer, "%08X", Bits16); break;
			case Type::Bits32: sprintf(buffer, "%08X", Bits32); break;
			case Type::Bits64: sprintf(buffer, "%016llX", Bits64); break;
			case Type::Float: sprintf(buffer, "%.4f", Single); break;
			case Type::Double: sprintf(buffer, "%.4lf", Double); break;
			}
		}
		else
		{
			switch (ValueType)
			{
			case Type::Bits8: sprintf(buffer, "%u", Bits8); break;
			case Type::Bits16: sprintf(buffer, "%u", Bits16); break;
			case Type::Bits32: sprintf(buffer, "%u", Bits32); break;
			case Type::Bits64: sprintf(buffer, "%ll", Bits64); break;
			case Type::Float: sprintf(buffer, "%.4f", Single); break;
			case Type::Double: sprintf(buffer, "%.4lf", Double); break;
			}
		} */

        if (ValueType == Type::Float)
            _text = Utils::ToString(Float, 4);
        else
            _text = Utils::Format(_isHexadecimal ? "%X" : "%d", Bits32);
    }

    void    NumericTextBox::SetValue(u8 val)
    {
        ValueType = Type::Bits8;
        Bits32 = val;

        _UpdateVal();
    }

    void    NumericTextBox::SetValue(u16 val)
    {
        ValueType = Type::Bits16;
        Bits32 = val;

        _UpdateVal();
    }

    void    NumericTextBox::SetValue(u32 val)
    {
        ValueType = Type::Bits32;
        Bits32 = val;

        _UpdateVal();
    }
    /*
    void    NumericTextBox::SetValue(u64 val)
    {
        ValueType = Type::Bits64;
        Bits64 = val;

        _UpdateVal();
    }*/

    void    NumericTextBox::SetValue(float val)
    {
        ValueType = Type::Float;
        Float = val;

        _UpdateVal();
    }
    /*
    void    NumericTextBox::SetValue(double val)
    {
        ValueType = Type::Double;
        Double = val;

        _UpdateVal();
    }*/

	void	NumericTextBox::UseHexadecimal(bool useHex)
	{
		_isHexadecimal = useHex;
		_UpdateVal();
	}

    void    NumericTextBox::Clear(void)
    {
        Bits32 = 0;

        _UpdateVal();
    }

    void    NumericTextBox::Draw(void)
    {
       // static Color    blank = Color(255, 255, 255);
        const Color    &black = Color::Black;
        const Color    &gainsboro = Color::Gainsboro;
        const Color    &grey = Color::Gray;

        if (!IsVisible)
            return;

        // Draw background
        Renderer::DrawRect(_rectPos, IsEnabled ? gainsboro : grey);

        // Draw text
        int posX = _rectPos.leftTop.x + 5;
        int posY = _rectPos.leftTop.y;

        posY += (_rectPos.size.y - 10) / 2;

        Renderer::DrawString((char *)_text.c_str(), posX, posY, black);
    }

    void    NumericTextBox::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        if (!IsEnabled)
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

    bool    NumericTextBox::operator()(void)
    {
        if (IsEnabled && _execute)
        {
            Keyboard  keyboard;

            keyboard.DisplayTopScreen = false;
			keyboard.IsHexadecimal(_isHexadecimal);

           int  out = -1;

           switch (ValueType)
            {
                case Type::Bits8: out = keyboard.Open(Bits8); break;
                case Type::Bits16: out = keyboard.Open(Bits16); break;
                case Type::Bits32: out = keyboard.Open(Bits32); break;
             //   case Type::Bits64: out = keyboard.Open(Bits64, Bits64); break;
                case Type::Float: out = keyboard.Open(Float); break;
             //   case Type::Double: out = keyboard.Open(Double, Double); break;
            }

            if (out != -1)
				_UpdateVal();

            _execute = false;
            return (true);
        }
        return (false);
    }
}
