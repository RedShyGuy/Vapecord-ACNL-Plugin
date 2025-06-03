#include "types.h"

#include "3ds.h"

#include <queue>
#include "CTRPluginFramework/System.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"

namespace CTRPluginFramework
{
    #define ABS(x) (x >= 0 ? x : -x)

    bool                EventManager::_refresh = true;
    bool                EventManager::_isTouching;
    touchPosition       EventManager::_firstTouch;

    EventManager::EventManager(u32 captureEventsGroups) : _captureMask(captureEventsGroups)
    {

    }

    bool    EventManager::PollEvent(Event &event)
    {
        if (PopEvent(event, false))
            return (true);
        return (false);
    }

    bool    EventManager::WaitEvent(Event &event)
    {
        if (PopEvent(event, true))
            return (true);
        return (false);
    }

    void EventManager::Clear()
    {
        while (!_eventsQueue.empty())
            _eventsQueue.pop();

        _startTime.Restart();
    }

    bool    EventManager::PopEvent(Event &event, bool isBlocking)
    {
        if (_refresh && _eventsQueue.empty())
        {
            ProcessEvents();
            if (isBlocking)
            {
                while (_eventsQueue.empty())
                {
                    Sleep(Milliseconds(10));
                    ProcessEvents();
                }
            }
        }
        else if (_eventsQueue.empty())
        {
            _refresh = true;
        }
        if (!_eventsQueue.empty())
        {
            event = _eventsQueue.front();
            _eventsQueue.pop();
            if (_eventsQueue.empty())
                _refresh = false;
            return (true);
        }
        return (false);
    }

    void    EventManager::PushEvent(const Event &event)
    {
        if (_startTime.HasTimePassed(Milliseconds(50)))
            _eventsQueue.push(event);
    }

    void    EventManager::ProcessEvents(void)
    {
        Event   event;

        Controller::Update();

        // Key Event
        if (_captureMask & EventGroups::GROUP_KEYS) {

            for (int i = 0; i < 32; i++)
            {
                Key code = static_cast<Key>(1 << i);
                if (Controller::IsKeyPressed(code))
                {
                    event.type = Event::KeyPressed;
                    event.key.code = code;
                    PushEvent(event);
                }
                if (Controller::IsKeyDown(code))
                {
                    event.type = Event::KeyDown;
                    event.key.code = code;
                    PushEvent(event);
                }
                if (Controller::IsKeyReleased(code))
                {
                    event.type = Event::KeyReleased;
                    event.key.code = code;
                    PushEvent(event);
                }
            }
        }

        // Touch Event
        if ((_captureMask & EventGroups::GROUP_TOUCH) || (_captureMask & EventGroups::GROUP_TOUCH_AND_SWIPE)) {

            touchPosition touchPos;

            if (Controller::IsKeyDown(Key::Touchpad))
            {
                hidTouchRead(&touchPos);
                if (touchPos.px != _lastTouch.px
                || touchPos.py != _lastTouch.py
                || !_isTouching)
                {
                    _lastTouch = touchPos;
                    if (_isTouching)
                    {
                        event.type = Event::TouchMoved;
                    }
                    else
                    {
                        event.type = Event::TouchBegan;
                        _firstTouch = touchPos;
                    }
                    _isTouching = true;
                    event.touch.x = touchPos.px;
                    event.touch.y = touchPos.py;
                    PushEvent(event);
                    _lastTouch = touchPos;
                }
            }
            else if (_isTouching)
            {
                _isTouching = false;
                event.type = Event::TouchEnded;
                event.touch.x = _lastTouch.px;
                event.touch.y = _lastTouch.py;
                PushEvent(event);

                if (_captureMask & EventGroups::GROUP_TOUCH_AND_SWIPE) {
                    int horizontalOffset = _firstTouch.px - _lastTouch.px;
                    int verticalOffset = _firstTouch.py - _lastTouch.py;

                    event.type = Event::TouchSwipped;
                    event.swip.direction = Event::None;
                    if (ABS(horizontalOffset) > 50 || ABS(verticalOffset) > 50)
                    {
                        if (horizontalOffset > 50)
                        {
                            event.swip.direction = Event::Right;
                            PushEvent(event);
                        }
                        if (horizontalOffset < -50)
                        {
                            event.swip.direction = Event::Left;
                            PushEvent(event);
                        }
                        if (verticalOffset < -50)
                        {
                            event.swip.direction = Event::Up;
                            PushEvent(event);
                        }
                        if (verticalOffset > 50)
                        {
                            event.swip.direction = Event::Down;
                            PushEvent(event);
                        }
                        if (horizontalOffset < -50 && verticalOffset < -50)
                        {
                            event.swip.direction = Event::LeftUp;
                            PushEvent(event);
                        }
                        if (horizontalOffset > 50 && verticalOffset < -50)
                        {
                            event.swip.direction = Event::RightUp;
                            PushEvent(event);
                        }
                        if (horizontalOffset < -50 && verticalOffset > 50)
                        {
                            event.swip.direction = Event::LeftDown;
                            PushEvent(event);
                        }
                        if (horizontalOffset > 50 && verticalOffset > 50)
                        {
                            event.swip.direction = Event::RightDown;
                            PushEvent(event);
                        }
                    }
                }
            }
        }
    }
}
