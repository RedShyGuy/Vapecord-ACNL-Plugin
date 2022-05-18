#ifndef CTRPLUGINFRAMEWORK_RENDER_HPP
#define CTRPLUGINFRAMEWORK_RENDER_HPP

#include "string"

namespace CTRPluginFramework
{
    class Render
    {
    private:

    public:
        enum FontDrawMode {
            BOLD = (1 << 0),
            ITALIC = (1 << 1),
            UNDERLINE = (1 << 2),
            STRIKETHROUGH = (1 << 3),
            LINEDOTTED = (1 << 4)
        };
        /**
        * \brief Gets the amount of pixels the text takes using the sysfont.
        * \param text Input text to calculate its width
        * \return Value of pixels the text takes.
        */
        static float GetTextWidth(const std::string& text);
    };

}

#endif