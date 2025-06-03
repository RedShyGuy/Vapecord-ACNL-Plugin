#include "CTRPluginFrameworkImpl/Graphics/UIContainer.hpp"

namespace CTRPluginFramework
{
    UIContainer::UIContainer()
    {
        
    }

    UIContainer::~UIContainer()
    {
        
    }

    void    UIContainer::operator+=(Drawable* uiControl)
    {
        if (uiControl != nullptr)
            _uiControls.push_back(uiControl);
    }

    void    UIContainer::Draw(void)
    {
        for (Drawable *uiControl : _uiControls)
            uiControl->Draw();
    }

    void    UIContainer::Update(const bool isTouchDown, const IntVector &touchPos)
    {
        for (Drawable *uiControl : _uiControls)
            uiControl->Update(isTouchDown, touchPos);
    }

    void    UIContainer::ExecuteAll(void)
    {
        for (Drawable *uiControl : _uiControls)
            (*uiControl)();
    }
}
