#ifndef CTRPLUGINFRAMEWORK_BUTTON_H
#define CTRPLUGINFRAMEWORK_BUTTON_H

#include "types.h"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFramework/System/Touch.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Drawable.hpp"
#include "CTRPluginFramework/Sound.hpp"

#include <string>

namespace CTRPluginFramework
{
    class Button : public Drawable
    {
        using IconCallbackState = int (*)(int, int, bool);
        using IconCallback = int (*)(int, int);
    public:

        enum
        {
            Normal = 0,
            Toggle = 1 << 1,
            Icon = 1 << 3,
            Sysfont = 1 << 4,
            Rounded = 1 << 5
        };

        Button(u32 type, const std::string &content, const IntRect &bounds, IconCallback icon);
        Button(u32 type, const std::string &content, const IntRect &bounds, IconCallbackState icon = nullptr);
        Button(u32 type, const IntRect &bounds, IconCallback icon);
        Button(u32 type, const IntRect &bounds, IconCallbackState icon = nullptr);
        ~Button();

        bool    operator()(void) override;
        void    Draw(void) override;
        void    Update(const bool isTouchDown, const IntVector &touchPos) override;
        void    Enable(bool isEnabled);
        void    Enable(void);
        void    Disable(void);
        void    Lock(void);
        void    Unlock(void);
        void    Execute(void);
        void    SetState(bool state);
        bool    GetState(void) const;
        void    SetAcceptSoundEvent(SoundEngine::Event event);

    private:

        u16                     _posX;
        u16                     _posY;
        u16                     _limit;
        void  *                 _icon{nullptr};
        std::string             _content;
        IntRect                 _bounds;
        Clock                   _clock;
        SoundEngine::Event      _acceptSoundEvent;

        union
        {
            u32         _bitfield{0};
            struct
            {
                bool    _isIconBtn : 1;
                bool    _isToggleBtn : 1;
                bool    _useSysfont : 1;
                bool    _isRounded : 1;

                bool    _isEnabled : 1;
                bool    _isLocked : 1;
                bool    _state : 1;
                bool    _isPressed : 1;
                bool    _wasPressed : 1;
                bool    _execute : 1;
            };
        };
    };
}

#endif
