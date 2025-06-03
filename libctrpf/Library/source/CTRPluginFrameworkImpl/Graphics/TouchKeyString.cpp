#include "CTRPluginFrameworkImpl/Graphics/TouchKeyString.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

namespace CTRPluginFramework
{
    TouchKeyString::TouchKeyString(IntRect ui, bool isEnabled)
    {

        _uiProperties = ui;
        _posY = _uiProperties.leftTop.y;
        _enabled = isEnabled;
        _acceptSoundEvent = SoundEngine::Event::ACCEPT;

        _isPressed = false;
        _wasPressed = false;
        _isForcePressed = false;
        _execute = false;

    }

    TouchKeyString::TouchKeyString(const std::string& content, IntRect ui, bool isEnabled)
        : TouchKeyString(ui, isEnabled)
    {
        _content = content;
        _isIcon = false;

        _contentLength = Renderer::GetTextSize(_content.c_str());

        while ((int)(_contentLength) > ui.size.x)
        {
            if (_content.size() < 1)
                break;
            _content.pop_back();
            _contentLength = Renderer::GetTextSize(_content.c_str());
        }

        _posX = ((_uiProperties.size.x - (int)_contentLength) >> 1) + _uiProperties.leftTop.x;
    }

    TouchKeyString::TouchKeyString(const CustomIcon& icon, IntRect ui, bool isEnabled)
        : TouchKeyString(ui, isEnabled)
    {
        _icon = icon;
        _isIcon = true;
        _posX = _uiProperties.leftTop.x;
    }

    void    TouchKeyString::Enable(bool isEnabled)
    {
        _enabled = isEnabled;
    }

    void    TouchKeyString::ForcePressed(bool force) {
        _isForcePressed = force;
    }

    bool TouchKeyString::CanUse(void)
    {
        return !(!_enabled || (_isIcon && !_icon.isEnabled) || (!_isIcon && _content.empty()));
    }

    void    TouchKeyString::Draw(void)
    {
        // If key is disabled
        if (!CanUse())
            return;

        const auto    &theme = Preferences::Settings.CustomKeyboard;
        const Color &background = (_isPressed || _isForcePressed) ? theme.KeyBackgroundPressed : theme.KeyBackground;
        const Color &text = (_isPressed || _isForcePressed) ? theme.KeyTextPressed : theme.KeyText;

        // Background
        Renderer::DrawRect(_uiProperties, background);

        if (!_isIcon) {
            int     posX = _posX;
            int     posY = ((_uiProperties.size.y - 16) >> 1) + _uiProperties.leftTop.y;
            int     maxX = _uiProperties.leftTop.x + _uiProperties.size.x - 1;

            // Text
            Renderer::DrawSysString(_content.c_str(), posX, posY, maxX, text);
        }
        else {
            // Icon
            Icon::DrawCustomIcon(_icon, _posX, _posY);
        }
    }

    void    TouchKeyString::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        if (!CanUse())
            return;

        bool    isTouched = _uiProperties.Contains(touchPos);

        if (_isPressed && !isTouchDown)
        {
            _isPressed = false;
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

    void    TouchKeyString::Scroll(float amount)
    {
        _posY += amount;

        int &posY = _uiProperties.leftTop.y;

        posY = _posY;
    }

	void TouchKeyString::GetPosition(u16& posX, float& posY)
	{
		posX = _posX;
		posY = _posY;
	}

	void TouchKeyString::SetPosition(u16 posX, float posY)
	{
		if (posX != 0xFFFF)
			_posX = posX;
		_posY = posY;
	}

    int     TouchKeyString::operator()(void)
    {
        if (CanUse() && _execute)
        {
            _execute = false;
            _wasPressed = _isPressed;
            SoundEngine::PlayMenuSound(_acceptSoundEvent);
            return (1);
        }
        return (-1);
    }

    void TouchKeyString::SetAcceptSoundEvent(SoundEngine::Event event)
    {
        _acceptSoundEvent = event;
    }

    SoundEngine::Event TouchKeyString::GetAcceptSoundEvent()
    {
        return _acceptSoundEvent;
    }
}
