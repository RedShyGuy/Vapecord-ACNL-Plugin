#ifndef CTRPLUGINFRAMEWORK_OSD_HPP
#define CTRPLUGINFRAMEWORK_OSD_HPP

#include "CTRPluginFramework/Graphics/Color.hpp"
#include <3ds.h>
#include <string>

namespace CTRPluginFramework
{
    using GSPFormat = GSPGPU_FramebufferFormat;

    class Screen
    {
    public:
        bool      IsTop;
        bool      Is3DEnabled;
        u32       LeftFramebuffer;
        u32       RightFramebuffer;
        u32       Stride;
        u32       BytesPerPixel;
        GSPFormat Format;

        /**
         * \brief Get a pointer to the screen's framebuffer
         * \param posX The position on the screen to point to
         * \param posY The position on the screen to point to
         * \param useRightFb If the function must return a pointer to the right framebuffer
         * \return A pointer to the screen's framebuffer at the desired position
         */
        u8 *    GetFramebuffer(u32 posX, u32 posY, bool useRightFb = false) const;

        /**
         * \brief Draw a string using Linux's font
         * \param str The string to draw
         * \param posX The position on screen to draw the string to
         * \param posY The position on screen to draw the string to
         * \param foreground The color of the characters
         * \param background The color of the background. Set alpha to 0 to disable.
         * \return posY + 10 (line feed)
         */
        u32     Draw(const std::string &str, u32 posX, u32 posY, const Color &foreground = Color::White, const Color &background = Color::Black) const;

        /**
         * \brief Draw a string using system font (support utf8 strings with special chars & unicode)
         * \param str The string to draw
         * \param posX The position on screen to draw the string to
         * \param posY The position on screen to draw the string to
         * \param foreground The color of the characters
         * \return posY + 16 (line feed)
         */
        u32     DrawSysfont(const std::string &str, u32 posX, u32 posY, const Color &foreground = Color::White) const;

        /**
         * \brief Draw a rectangle
         * \param posX The position on screen to draw the rectangle
         * \param posY The position on screen to draw the rectangle
         * \param width The width of the rectangle
         * \param height The height of the rectangle
         * \param color The color of the rectangle
         * \param filled If the rectangle is filled or if the function should only draw the border
         */
        void    DrawRect(u32 posX, u32 posY, u32 width, u32 height, const Color &color, bool filled = true) const;
        /**
         * \brief Change the color of a pixel at the desired X, Y position
         * \param posX The X position of the pixel to change
         * \param posY The Y position of the pixel to change
         * \param color The new color of the pixel
         */
        void    DrawPixel(u32 posX, u32 posY, const Color &color) const;

        /**
         * \brief Get the current color of a pixel at X, Y position
         * \param posX The X position of the pixel to read
         * \param posY The Y position of the pixel to read
         * \param pixel The output color of the pixel
         * \param fromRightFb If the pixel must be read from the right framebuffer
         */
        void    ReadPixel(u32 posX, u32 posY, Color &pixel, bool fromRightFb = false) const;

        /**
         * \brief Fades the screen to white the specified amount.
         * \param fadeAmount Amount to fade, range [0, 1]
         */
        void    Fade(const float fadeAmount);

    private:
        friend class OSDImpl;
        Screen() {};
    };

    using OSDCallback = bool(*)(const Screen &);
    class OSD
    {
    public:

        /**
         * \brief Send a notification on the top screen, a notification duration is 5 seconds\n
         * A maximum of 50 notifications can be queued
         * \param str Text of the notification
         * \param foreground The color of the text (Default: blank)
         * \param background The color of the background (Default: black)
         * \return 0 if success, -1 if the notification couldn't be added
         */
        static int      Notify(const std::string &str, const Color &foreground = Color::White, const Color &background = Color::Black);

        /**
         * \brief Add a callback to the OSD system which will be called at each game's frame
         * \param cb The callback to add
         */
        static void     Run(OSDCallback cb);

        /**
         * \brief Remove a callback from the OSD system
         * \param cb The callback to remove
         */
        static void     Stop(OSDCallback cb);

        /**
         * \brief Returns the width in pixels of the specified string
         * \param sysfont Which font will be used to compute the width
         * \param text
         * \return The width in pixels
         */
        static float    GetTextWidth(bool sysfont, const std::string& text);

        /* Those are to be used only when the process is paused */
        static const Screen&    GetTopScreen(void);
        static const Screen&    GetBottomScreen(void);
        static void             SwapBuffers(void);

        static void     Lock(void);
        static bool     TryLock(void); //false success, true failure
        static void     Unlock(void);
    };
}

#endif
