#ifndef CTRPLUGINFRAMEWORK_COLOR_HPP
#define CTRPLUGINFRAMEWORK_COLOR_HPP

#include "types.h"
#include <algorithm>
#include <string>

namespace CTRPluginFramework
{
    class Color
    {
    public:

        enum class BlendMode
        {
            Alpha,
            Add,
            Sub,
            Mul,
            None
        };

        constexpr Color(void) : a(255), b(0), g(0), r(0) {}
        constexpr Color(u32 color) : raw(color) {}
        constexpr Color(u8 red, u8 green, u8 blue, u8 alpha = 255) : a(alpha), b(blue), g(green), r(red) {}

        inline u32 ToU32(void) const { return raw; };
        Color   &Fade(float fading);
        Color   Blend(const Color &color, BlendMode mode) const;

        /**
        * \brief Removes the color of a string (only if the color is at the beginning)
        * \param str String to remove color from
        * \return String with color removed
        */
        static std::string RemoveColor(const std::string &str);

        /**
        * \brief Printf for Colors (add %c to string)
        * \param fmt String to be formatted
        * \param ... Additional arguments
        * \return Formatted string
        */
        static std::string Format(const char *fmt, ...);

        /**
        * \brief Get the color of the given string (only if the color is at the beginning)
        * \param str String to get color from
        * \return The color of the string
        */
        static Color GetColor(const std::string &str);

        /**
        * @brief Get a randomized rgba value 
        * @return rgba value
        */
        static Color Rainbow(void);

        inline bool    operator == (const Color &right) const {return raw == right.raw;}
        inline bool    operator != (const Color &right) const {return raw != right.raw;}
        bool    operator < (const Color &right) const;
        bool    operator <= (const Color &right) const;
        bool    operator > (const Color &right) const;
        bool    operator >= (const Color &right) const;
        Color   operator + (const Color &right) const;
        Color   operator - (const Color &right) const;
        Color   operator * (const Color &right) const;
        Color   &operator += (const Color &right);
        Color   &operator -= (const Color &right);
        Color   &operator *= (const Color &right);

        operator std::string() const
        {
            char  strColor[5] = { 0 };

            strColor[0] = 0x1B;
            strColor[1] = std::max((u8)1, r);
            strColor[2] = std::max((u8)1, g);
            strColor[3] = std::max((u8)1, b);

            return strColor;
        }

        union
        {
            u32     raw;
            struct // Match raw byte order
            {
                u8      a;
                u8      b;
                u8      g;
                u8      r;
            };
        };

        // All those colors are from https://www.rapidtables.com/web/color/RGB_Color.html
        // Some basic colors constants
        static const Color  Black;
        static const Color  White;
        static const Color  Red;
        static const Color  Lime;
        static const Color  Blue;
        static const Color  Yellow;
        static const Color  Cyan;
        static const Color  Magenta;
        static const Color  Silver;
        static const Color  Gray;
        static const Color  Maroon;
        static const Color  Olive;
        static const Color  Green;
        static const Color  Purple;
        static const Color  Teal;
        static const Color  Navy;

        static const Color  BlackGrey;
        static const Color  Brown;
        static const Color  DarkGrey;
        static const Color  DeepSkyBlue;
        static const Color  DimGrey;
        static const Color  DodgerBlue;
        static const Color  Gainsboro;
        static const Color  ForestGreen;
        static const Color  LimeGreen;
        static const Color  Orange;
        static const Color  SkyBlue;
        static const Color  Turquoise;
    };
}

#endif