#ifndef CTRPLUGINFRAMEWORKIMPL_WINDOW_HPP
#define CTRPLUGINFRAMEWORKIMPL_WINDOW_HPP

#include "CTRPluginFramework/System/Vector.hpp"
#include "CTRPluginFramework/System/Rect.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Button.hpp"
#include <string>

namespace CTRPluginFramework
{
    class BMPImage;
    class Window
    {
    public:

        Window(u32 posX, u32 posY, u32 width, u32 height, bool closeBtn, BMPImage *image);
        ~Window(void);

        void    Draw(void) const;
        void    Draw(const std::string &title) const;
        void    DrawButton(void) const;
        void    Update(const bool isTouched, const IntVector &touchPos) const;
        bool    MustClose(void) const;
        void    Close(void) const;
        const IntRect &GetRect(void) const;
		
		//vapecord infobar
        static void    DrawTopInfoBar(void);
        static void    DrawBottomInfoBar(void);

        static Window   TopWindow;
        static Window   TopWindow_TopBar;
        static Window   TopWindow_BottomBar;

        static Window   BottomWindow;
        static Window   BottomWindow_TopBar;
        static Window   BottomWindow_BottomBar;

        static void     UpdateBackgrounds(void);

    private:

        IntRect     _rect;
        IntRect     _border;
        Button    * _closeBtn;
        BMPImage  * _image;
    };
}

#endif
