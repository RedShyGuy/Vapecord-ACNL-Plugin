#ifndef CTRPLUGINFRAMEWORKIMPL_DRAWABLE_HPP
#define CTRPLUGINFRAMEWORKIMPL_DRAWABLE_HPP
#include "CTRPluginFramework/System/Vector.hpp"

namespace CTRPluginFramework
{
    class Drawable
    {
    public:
        virtual ~Drawable(){}

        virtual void Draw(void) = 0;
        virtual bool operator()(void) { return (false); }
        virtual void Update(const bool isTouchDown, const IntVector &touchPos) = 0;
    };
}

#endif