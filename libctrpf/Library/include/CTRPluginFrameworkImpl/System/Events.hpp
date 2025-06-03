#ifndef CTRPLUGINFRAMEWORK_EVENTS_HPP
#define CTRPLUGINFRAMEWORK_EVENTS_HPP

#include "CTRPluginFramework/System/Controller.hpp"

namespace CTRPluginFramework
{
    class Event
    {
    public:

        enum EventType
        {
            KeyPressed,
            KeyDown,
            KeyReleased,
            CPadMoved,
            CStickMoved,
            TouchBegan,
            TouchMoved,
            TouchEnded,
            TouchSwipped,
            Slider3DChanged,

            Count
        };

        enum SwipDirection
        {
            None = 0,
            Left = 1,
            Right = 1 << 1,
            Up = 1 << 2,
            Down = 1 << 3,
            LeftUp = Left | Up,
            RightUp = Right | Up,
            LeftDown = Left | Down,
            RightDown = Right | Down
        };

        struct KeyEvent
        {
            Key code;
        };

        struct StickEvent
        {
            int x;
            int y;
        };

        struct TouchEvent
        {
            int x;
            int y;
        };

        struct SwipEvent
        {
            SwipDirection direction;
        };

        struct SliderEvent
        {
            float value;
        };

        EventType   type;

        union
        {
            KeyEvent        key;
            StickEvent      stick;
            TouchEvent      touch;
            SliderEvent     slider;
            SwipEvent       swip;
        };
    };
}

#endif