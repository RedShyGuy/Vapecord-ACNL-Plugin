#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFramework/System/Lock.hpp"

namespace CTRPluginFramework
{
    Window  Window::BottomWindow = Window(20, 20, 280, 200, true, nullptr);
    Window  Window::TopWindow = Window(30, 20, 340, 200, false, nullptr);

    Window::Window(u32 posX, u32 posY, u32 width, u32 height, bool closeBtn, BMPImage *image) :
    _rect(posX, posY, width, height),
    _border(posX + 2, posY + 2, width - 4, height - 4),
    _image(image)
    {
        if (closeBtn)
        {
            _closeBtn = new Button(Button::Icon, IntRect(posX + width - 25, posY + 4, 20, 20), Icon::DrawClose);
            _closeBtn->SetAcceptSoundEvent(SoundEngine::Event::NUM_EVENTS);
        }

        else
            _closeBtn = nullptr;
    }

    Window::~Window(void)
    {
        delete _closeBtn;
        _closeBtn = nullptr;
    }

    void    Window::Draw(void) const
    {
        // Background
        if (_image != nullptr)
            _image->Draw(_rect.leftTop);
        else
        {
            Renderer::DrawRect2(_rect, Preferences::Settings.BackgroundMainColor, Preferences::Settings.BackgroundSecondaryColor);
            Renderer::DrawRect(_border, Preferences::Settings.BackgroundBorderColor, false);
        }

        // Close button
        if (_closeBtn != nullptr)
            _closeBtn->Draw();
    }

    void    Window::Draw(const std::string& title) const
    {
        // Background
        if (_image != nullptr)
            _image->Draw(_rect.leftTop);
        else
        {
            Renderer::DrawRect2(_rect, Preferences::Settings.BackgroundMainColor, Preferences::Settings.BackgroundSecondaryColor);
            Renderer::DrawRect(_border, Preferences::Settings.BackgroundBorderColor, false);
        }

        // Title
        Color &color = Preferences::Settings.WindowTitleColor;
        int posY = _rect.leftTop.y + 5;
        int posX = _rect.leftTop.x + 10;
        int xx = Renderer::DrawSysString(title.c_str(),posX, posY, 330, color);
        Renderer::DrawLine(posX, posY, xx, color);

        // Close button
        if (_closeBtn != nullptr)
            _closeBtn->Draw();
    }

    void    Window::DrawButton(void) const
    {
        // Close button
        if (_closeBtn != nullptr)
            _closeBtn->Draw();
    }

    void    Window::Update(const bool isTouched, const IntVector &touchPos) const
    {
        if (_closeBtn != nullptr)
            _closeBtn->Update(isTouched, touchPos);
    }

    bool    Window::MustClose(void) const
    {
        if (_closeBtn == nullptr)
            return (false);

        return ((*_closeBtn)());
    }

    void    Window::Close(void) const
    {
        if (_closeBtn)
        {
            _closeBtn->SetState(true);
            _closeBtn->Execute();
        }
    }

    const   IntRect &Window::GetRect(void) const
    {
        return (_rect);
    }

    void    Window::UpdateBackgrounds(void)
    {
        BottomWindow._image = Preferences::bottomBackgroundImage;
        TopWindow._image = Preferences::topBackgroundImage;
    }
}
