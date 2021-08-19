#include "cheats.hpp"
#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{
    KeySequence::KeySequence(KeyVector sequence) : 
    _sequence(sequence), _indexInSequence(0)
    {            
    }

    bool KeySequence::operator()(void) {
        if(Controller::IsKeyDown(_sequence[_indexInSequence])) {
            _indexInSequence++;

            if(_indexInSequence >= _sequence.size()) {
                _indexInSequence = 0;
                return true;
            }

            _timer.Restart();
        }

        if(_timer.HasTimePassed(Seconds(1.f))) {
            _indexInSequence = 0;
            _timer.Restart();
        }

        return false;
    }
}