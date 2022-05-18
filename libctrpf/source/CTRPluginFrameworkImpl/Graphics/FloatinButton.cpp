#include "CTRPluginFrameworkImpl/Graphics/FloatingButton.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    FloatingButton::FloatingButton(const IntRect &pos, const IconCallback icon) :
        _icon(icon), _box(pos), _lastTouch{ 0,0 },
        _pressed(false), _enabled(false), _moving(false)
    {        
    }

    FloatingButton::~FloatingButton()
    {
    }

    void    FloatingButton::Draw(void)
    {
        if (_icon != nullptr)
        {
            _icon(_box.leftTop.x, _box.leftTop.y);
        }
    }

    bool    FloatingButton::operator()()
    {
        bool ret = _enabled;

        _enabled = false;
        return (ret);
    }
#define ABS(x) ((x) >= 0 ? (x) : (-x))

    static inline bool     IsDifferentByAtLeast(int old, int newv, int val)
    {
        int v = newv - old;
        return (old != newv && ABS(v) >= val);
    }

    void    FloatingButton::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        IntVector &currentPos = _box.leftTop;
        const IntVector &size = _box.size;

        if (isTouchDown)
        {
            // If the user is moving
            if (_moving)
            {
                // Adjust the position
                currentPos.x = std::min(std::max(touchPos.x - size.x / 2, 0), 319 - size.x);
                currentPos.y = std::min(std::max(touchPos.y - size.y / 2, 0), 239 - size.y);
            }
            // Else if the position is within the button's area
            else if (_box.Contains(touchPos))
            {
                // If user just pressed the button, save the current state
                if (!_pressed)
                {
                    _lastTouch = touchPos;
                    _pressed = true;
                }

                // Else check if user initiate a movement to move the icon
                else if (!_moving)
                {
                    if (IsDifferentByAtLeast(_lastTouch.x, touchPos.x, 5)
                        || IsDifferentByAtLeast(_lastTouch.y, touchPos.y, 5))
                    {
                        _moving = true;
                    }
                }
            }
            // Touchpos is not in the area so reset the state
            else
            {
                _pressed = false;
                _moving = false;
            }
            return;
        }

        _enabled = _pressed && !_moving;
        _pressed = false;
        _moving = false;
    }
}
