#ifndef CTRPLUGINFRAMEWORKIMPL_TOUCHKEYSTRING_KEYBOARD_HPP
#define CTRPLUGINFRAMEWORKIMPL_TOUCHKEYSTRING_KEYBOARD_HPP

#include "CTRPluginFrameworkImpl/Graphics/Drawable.hpp"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFramework/Graphics/CustomIcon.hpp"

#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFramework/Sound.hpp"

#include <string>

namespace CTRPluginFramework
{
    class TouchKeyString
    {
    public:

        // Key with string
        TouchKeyString(const std::string &content, IntRect ui, bool enabled = true);

        // Key with icon
        TouchKeyString(const CustomIcon& icon, IntRect ui, bool isEnabled);

        ~TouchKeyString(){}

        // Enabler
        void    Enable(bool enable = true);

        // Force key being pressed (graphically only)
        void ForcePressed(bool force);

        // Can be used
        bool    CanUse(void);

        // Draw
        void    Draw(void);

        // Update
        void    Update(const bool touchIsDown, const IntVector &touchPos);

        // Scrolls
        void    Scroll(float amount);

		// Position getter/setter
		void    GetPosition(u16& posX, float& posY);
		void	SetPosition(u16 posX, float posY);

        // Executer
        // Return -1 if not pressed, 1 otherwise
        int    operator()(void);

        void    SetAcceptSoundEvent(SoundEngine::Event event);
        SoundEngine::Event GetAcceptSoundEvent();

    private:
        std::string     _content;
        CustomIcon      _icon;

        IntRect         _uiProperties;
        SoundEngine::Event      _acceptSoundEvent;

        bool            _isPressed;
        bool            _wasPressed;
        bool            _isForcePressed;
        bool            _isIcon;
        bool            _execute;
        bool            _enabled;
        u16             _posX;
        float           _contentLength;
        float           _posY;

        TouchKeyString(IntRect ui, bool isEnabled);
    };
}

#endif
