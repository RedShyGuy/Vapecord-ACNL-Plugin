#ifndef CTRPLUGINFRAMEWORK_RENDER_HPP
#define CTRPLUGINFRAMEWORK_RENDER_HPP

#include "string"
#include "unordered_map"
#include "CTRPluginFramework/Graphics/CustomIcon.hpp"
#include "CTRPluginFramework/System/Rect.hpp"
#include "CTRPluginFramework/System/Vector.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"

namespace CTRPluginFramework
{
    class Render
    {
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

        /**
         * @brief Creates a set of characters that will be used with the random text draw mode (see StringExtensions.hpp).
         *
         * @param set A string that contains all the characters that will be randomly selected
         * @return The ID of the random set. Use this ID to delete the random set with EraseRandomSet
         */
        static int CreateRandomSet(const std::string& set);

        /**
         * @brief Erases the specified random set.
         *
         * @param setID Set ID to delete, returned by CreateRandomSet.
         */
        static void EraseRandomSet(int setID);

        /**
         * @brief Pulls a random character from the specified setID.
         *
         * @param setID Set ID to pull the random character from.
         * @return std::string
         */
        static std::string PullRandomCharacter(int setID);

        class Interface {
        public:
            /**
             * @brief Draws a pixel
             *
             * @param posX X position of the pixel
             * @param posY Y position of the pixel
             * @param color Color of the pixel
             */
            void     DrawPixel(int posX, int posY, const Color &color);

            /**
             * @brief Reads a pixel
             *
             * @param posX X position of the pixel
             * @param posY Y position of the pixel
             * @param color Output color of the pixel
             */
            void     ReadPixel(int posX, int posY, Color &outColor);

            /**
             * @brief Draws an horizontal line.
             *
             * @param posX X position of the line
             * @param posY Y position of the line
             * @param length Length of the line
             * @param color Color of the line
             * @param thickness Thickness of the line
             * @param stippled The line is stippled
             */
            void     DrawHLine(int posX, int posY, int length, const Color &color, int thickness = 1, bool stippled = false);

            /**
             * @brief Draws a vertical line.
             *
             * @param posX X position of the line
             * @param posY Y position of the line
             * @param length Length of the line
             * @param color Color of the line
             * @param thickness Thickness of the line
             * @param stippled The line is stippled
             */
            void     DrawVLine(int posX, int posY, int length, const Color &color, int thickness = 1, bool stippled = false);

            /**
             * @brief Draws a line
             *
             * @param start Start position of the line
             * @param end End postion of the line
             * @param color Color of the line
             */
            void     DrawLine(const IntVector &start, const IntVector &end, const Color &color);

            /**
             * @brief Draws a rectangle
             *
             * @param rect Rectangle
             * @param color Color of the rectangle
             * @param fill Fill the rectangle
             * @param thickness Thickness of the rectangle
             */
            void     DrawRect(const IntRect& rect, const Color& color, bool fill = true, int thickness = 1);

            /**
             * @brief Draws an string with the system font
             *
             * @param str String to draw
             * @param posX Starting X position of the string
             * @param posY Starting Y position of the string
             * @param color Color of the string
             * @param xLimit Limit X coordinate. If autoWrap is enabled, the text will be wrapped to the next line
             * @param yLimit Limit Y coordinate. Drawing will stop after reaching this Y coordinate
             * @param autoWrap Automatically wrap text to the next line
             */
            void     DrawSysString(const std::string& str, int posX, int posY, Color color, int xLimit = 320, int yLimit = 240, bool autoWrap = false);

            /**
             * @brief Draws a custom icon
             *
             * @param icon Icon to draw
             * @param posX X position
             * @param posY Y position
             */
            void     DrawCustomIcon(const CustomIcon& icon, int posX, int posY);

        private:
            friend class Renderer;
            Interface() {}
        };

    private:
        static std::unordered_map<int, string16> _randomSets;
    };

}

#endif