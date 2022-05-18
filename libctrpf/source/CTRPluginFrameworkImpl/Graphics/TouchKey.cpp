#include "CTRPluginFrameworkImpl/Graphics/TouchKey.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    TouchKey::TouchKey(TouchKey &&tk)
    {
        _character = tk._character;
        _glyph = tk._glyph;
        _content = tk._content;
        _icon = tk._icon;
        _uiProperties = tk._uiProperties;
        _posX = tk._posX;
        _posY = tk._posY;
        _isPressed = tk._isPressed;
        _enabled = tk._enabled;
        _execute = tk._execute;
        _acceptSoundEvent = tk._acceptSoundEvent;

        tk._content = nullptr;
    }
    TouchKey::TouchKey(int character, IntRect ui, bool isEnabled)
    {
        _character = character;
        _content = nullptr;
        _icon = nullptr;
        _uiProperties = ui;
        _enabled = isEnabled;
        _acceptSoundEvent = SoundEngine::Event::DESELECT;

        _isPressed = false;
        _execute = false;

        // Character drawing infos
        _glyph = Font::GetGlyph((char)character);

        if (_glyph == nullptr)
            return;

        _posX = ((ui.size.x - static_cast<int>(_glyph->Width())) >> 1) + ui.leftTop.x;
        _posY = ((ui.size.y - 16) >> 1) + ui.leftTop.y;
    }

    TouchKey::TouchKey(const std::string &str, IntRect ui, int value, bool isEnabled)
    {
        _character = value;
        _content = new KeyContent(str);
        _icon = nullptr;
        _uiProperties = ui;
        _enabled = isEnabled;
        _acceptSoundEvent = SoundEngine::Event::DESELECT;

        _isPressed = false;
        _execute = false;

        // String drawing infos
        _posX = ((ui.size.x - static_cast<int>(_content->width)) >> 1) + ui.leftTop.x;
        _posY = ((ui.size.y - 16) >> 1) + ui.leftTop.y;
    }

    TouchKey::TouchKey(int value, IconCallback icon, IntRect ui, bool isEnabled)
    {
        _character = value;
        _content = nullptr;
        _icon = icon;
        _uiProperties = ui;
        _enabled = isEnabled;
        _acceptSoundEvent = SoundEngine::Event::DESELECT;

        _isPressed = false;
        _execute = false;

        // Icon drawing infos
        _posX = ((_uiProperties.size.x - 15) >> 1) + _uiProperties.leftTop.x;
        _posY = ((_uiProperties.size.y - 15) >> 1) + _uiProperties.leftTop.y;
    }

    TouchKey::~TouchKey()
    {
        Clear();
    }

    void TouchKey::Clear(void)
    {
        if (_content != nullptr)
        {
            delete _content;
            _content = nullptr;
        }
    }

    void    TouchKey::Enable(bool isEnabled)
    {
        _enabled = isEnabled;
    }

    void    TouchKey::DrawCharacter(const Color &color)
    {
        // If not a string
        if (_content == nullptr)
        {
            if (_glyph == nullptr)
                return;

            Renderer::DrawGlyph(_glyph, _posX, _posY, color);
        }
        // String
        else
        {
            int posX = _posX;

            for (Glyph *glyph : _content->glyphs)
            {
                posX = Renderer::DrawGlyph(glyph, posX, _posY, color);
            }
        }
    }

    void    TouchKey::Draw(void)
    {
        const auto      &theme = Preferences::Settings.Keyboard;
        const Color     &background = _isPressed ? theme.KeyBackgroundPressed : theme.KeyBackground;
        const Color     &text = _isPressed ? theme.KeyTextPressed : theme.KeyText;

        // Background
        Renderer::DrawRect(_uiProperties, background);

        // Icon
        if (_icon != nullptr)
        {
            _icon(_posX, _posY, false);
        }
        // Character
        else
            DrawCharacter(_enabled ? text : theme.KeyTextDisabled);
    }

    void    TouchKey::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        if (!_enabled)
            return;

        bool    isTouched = _uiProperties.Contains(touchPos);

        if (_isPressed && !isTouchDown)
        {
            _isPressed = false;
            _wasPressed = _isPressed;
            _execute = true;
            return;
        }

        if (_isPressed != _wasPressed)
        {
            if (_isPressed)
                SoundEngine::PlayMenuSound(SoundEngine::Event::SELECT);
            else if (isTouchDown)
                SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
        }
        _wasPressed = _isPressed;

        _isPressed = isTouchDown && isTouched;
    }

    int      TouchKey::operator()(std::string &str)
    {
        // Hacky code for BACKSPACE key holding
        if (_enabled && _character == 0x8)
        {
            if (_isPressed)
                return (_character);
            if (_execute)
            {
                SoundEngine::PlayMenuSound(_acceptSoundEvent);
                _execute = false;
                return (~_character);
            }
        }
        else
        {
            if (_enabled && _execute)
            {
                _execute = false;
                SoundEngine::PlayMenuSound(_acceptSoundEvent);
                if (_content != nullptr && _character == 0x12345678)
                {
                    str += _content->text;
                }
                return (_character);
            }
        }

        return (-1);
    }

    void TouchKey::SetAcceptSoundEvent(SoundEngine::Event event)
    {
        _acceptSoundEvent = event;
    }
}
