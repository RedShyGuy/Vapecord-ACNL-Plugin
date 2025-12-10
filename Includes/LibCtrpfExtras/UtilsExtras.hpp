#pragma once

#include <CTRPluginFramework.hpp>
#include <cstdarg>

namespace CTRPluginFramework {
    class UtilsExtras : public Utils {
    public:
        /**
        * \brief Converts a string to lowcase
        * \param str String to cast to lowcase
        * \return Lowercased string
        */
        static void ConvertToLowcase(std::string& str) {
            for(char& c : str)
                c = std::tolower(c);
        }

        static inline std::string& Ltrim(std::string& str) {
            auto it = std::find_if(str.begin(), str.end(), [](char ch) { return (!std::isspace(ch)); });
            str.erase(str.begin(), it);
            return str;
        }

        static inline std::string& Rtrim(std::string& str) {
            auto it = std::find_if(str.rbegin(), str.rend(), [](char ch) { return (!std::isspace(ch)); });
            str.erase(it.base(), str.end());
            return str;
        }

        static std::string& Trim(std::string& str) {
            return (Ltrim(Rtrim(str)));
        }
    };
}