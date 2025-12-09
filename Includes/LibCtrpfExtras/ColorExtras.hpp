#pragma once

#include <CTRPluginFramework.hpp>
#include <cstdarg>

namespace CTRPluginFramework {
    class ColorExtras {
    public:
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
    };
}