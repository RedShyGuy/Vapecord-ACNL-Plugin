#ifndef CTRPLUGINFRAMEWORKIMPL_UICONTAINER_HPP
#define CTRPLUGINFRAMEWORKIMPL_UICONTAINER_HPP

#include "CTRPluginFrameworkImpl/Graphics/Drawable.hpp"
#include <vector>

namespace CTRPluginFramework
{
    class UIContainer
    {
    public:
        UIContainer(void);
        ~UIContainer(void);

        void    operator+=(Drawable *uiControl);
        void    Draw(void);
        void    Update(const bool isTouchDown, const IntVector &touchPos);
        void    ExecuteAll(void);
        
    private:
        std::vector<Drawable*> _uiControls;
    };
}

#endif
