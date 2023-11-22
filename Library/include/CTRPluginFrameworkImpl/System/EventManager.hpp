#ifndef CTRPLUGINFRAMEWORK_EVENTMANAGER_HPP
#define CTRPLUGINFRAMEWORK_EVENTMANAGER_HPP

#include "3ds.h"
#include <queue>
#include "CTRPluginFramework/System/Clock.hpp"

namespace CTRPluginFramework
{
    class Event;
    class EventManager
    {
    public:
        enum EventGroups {
            GROUP_KEYS = (1 << 0),
            GROUP_CPAD = (1 << 1),
            GROUP_CSTICK = (1 << 2),
            GROUP_TOUCH = (1 << 3),
            GROUP_TOUCH_AND_SWIPE = (1 << 4),

            GROUP_MISC = (1 << 31)
        };

        EventManager(u32 captureEventGroups);

        bool PollEvent(Event &event);
        bool WaitEvent(Event &event);
        void Clear();

    private:

        bool PopEvent(Event &event, bool isBlocking);
        void PushEvent(const Event &event);
        void ProcessEvents(void);

        std::queue<Event>   _eventsQueue;
        touchPosition       _lastTouch;
        float               _slider3D;
        u32                 _keysHeld;
        u32                 _captureMask;
        Clock               _startTime;

        static bool                _refresh;
        static bool                _isTouching;
        static touchPosition       _firstTouch;

    };
}

#endif