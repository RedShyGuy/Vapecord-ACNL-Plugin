#ifndef CTRPLUGINFRAMEWORKIMPL_FLOATINGBUTTON_HPP
#define CTRPLUGINFRAMEWORKIMPL_FLOATINGBUTTON_HPP

#include "Drawable.hpp"
#include "CTRPluginFramework/System/Rect.hpp"

namespace CTRPluginFramework
{
    class FloatingButton : Drawable
    {
        using IconCallback = int(*)(int, int);
    public:
        FloatingButton(const IntRect &pos, const IconCallback icon);
        ~FloatingButton(void) override;

        void    Draw(void) override;
        bool    operator()(void) override;
        void    Update(const bool isTouchDown, const IntVector &touchPos) override;

    private:
        IconCallback    _icon;
        IntRect     _box;
        IntVector   _lastTouch;
        bool        _pressed;
        bool        _enabled;
        bool        _moving;
    };
}

#endif