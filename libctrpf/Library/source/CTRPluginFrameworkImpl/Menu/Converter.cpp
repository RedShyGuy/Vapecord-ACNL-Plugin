#include "CTRPluginFrameworkImpl/Menu/Converter.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "memory.h"

namespace CTRPluginFramework
{
    static Converter *__converter = nullptr;

    Converter::Converter():
        _decimalTB(30, 100, 125, 15),
        _hexadecimalTB(30, 140, 125, 15),
        _floatTB(160, 100, 125, 15),
        _hexfloatTB(160, 140, 125, 15)
    {
        u32 val = 0;
        _decimalTB.SetValue(val);
        _decimalTB.UseHexadecimal(false);
        _hexadecimalTB.SetValue(val);
        _hexadecimalTB.UseHexadecimal(true);
        _floatTB.SetValue(0.f);
        _hexfloatTB.UseHexadecimal(true);
        _hexfloatTB.SetValue(val);

        __converter = this;
    }

    Converter::~Converter()
    {
    }

    void    Converter::operator()(u32 v)
    {
        Event   event;
        EventManager eventMgr(EventManager::EventGroups::GROUP_KEYS);

        if (v != 0)
        {
            _hexadecimalTB.SetValue(v);
            _decimalTB.SetValue(v);
            float vf;
            memcpy(&vf, &v, sizeof(float)); // Optimized away
            _floatTB.SetValue(vf);
            _hexfloatTB.SetValue(v);
        }

        do
        {
            // Update events
            while (eventMgr.PollEvent(event))
            {
                if (event.type == Event::KeyPressed && event.key.code == Key::B)
                    return;
            }

            bool isTouchDown = Touch::IsDown();
            IntVector touchPos(Touch::GetPosition());

            // Update controls
            _decimalTB.Update(isTouchDown, touchPos);
            _hexadecimalTB.Update(isTouchDown, touchPos);
            _floatTB.Update(isTouchDown, touchPos);
            _hexfloatTB.Update(isTouchDown, touchPos);
            Window::BottomWindow.Update(isTouchDown, touchPos);

            // Check textbox and do conversion
            if (_decimalTB())
            {
                int dec = static_cast<int>(_decimalTB.Bits32);
                float val = static_cast<float>(dec);

                _hexadecimalTB.SetValue(_decimalTB.Bits32);
                _floatTB.SetValue(val);
                _hexfloatTB.SetValue(_floatTB.Bits32);
            }

            if (_hexadecimalTB())
            {
                int hex = static_cast<int>(_hexadecimalTB.Bits32);
                float val = static_cast<float>(hex);

                _decimalTB.SetValue(_hexadecimalTB.Bits32);
                _floatTB.SetValue(val);
                _hexfloatTB.SetValue(_floatTB.Bits32);
            }

            if (_floatTB())
            {
                float val = _floatTB.Float;
                int dec = static_cast<int>(val);

                _decimalTB.SetValue((u32)dec);
                _hexadecimalTB.SetValue(_decimalTB.Bits32);
                _hexfloatTB.SetValue(_floatTB.Bits32);
            }

            if (_hexfloatTB())
            {
                _floatTB.SetValue(_hexfloatTB.Float);
                int val = static_cast<int>(_hexfloatTB.Float);
                _decimalTB.SetValue((u32)val);
                _hexadecimalTB.SetValue(_decimalTB.Bits32);
            }

            // Draw controls
            _Draw();

        } while (!Window::BottomWindow.MustClose());
    }

    Converter * Converter::Instance(void)
    {
        return __converter;
    }

    void    Converter::_Draw()
    {
        Renderer::SetTarget(BOTTOM);

        Window::BottomWindow.Draw("Converter");

        _decimalTB.Draw();
        _hexadecimalTB.Draw();
        _floatTB.Draw();
        _hexfloatTB.Draw();

        int posX = 30;
        int posY = 90;
        const Color &textcolor = Preferences::Settings.MainTextColor;

        Renderer::DrawString("Decimal:", posX, posY, textcolor);
        posY = 130;
        Renderer::DrawString("Decimal (hex):", posX, posY, textcolor);

        posX = 160;
        posY = 90;
        Renderer::DrawString("Float: ", posX, posY, textcolor);
        posY = 130;
        Renderer::DrawString("Float (hex):", posX, posY, textcolor);
        Renderer::EndFrame();
    }
}
