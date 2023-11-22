#ifndef CTRPLUGINFRAMEWORK_RENDER_HPP
#define CTRPLUGINFRAMEWORK_RENDER_HPP

#include "string"
#include "unordered_map"
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

    private:
        static std::unordered_map<int, string16> _randomSets;
    };

}

#endif