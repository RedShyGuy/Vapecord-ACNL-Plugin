#ifndef CTRPLUGINFRAMEWORK_UTILS_UTILS_HPP
#define CTRPLUGINFRAMEWORK_UTILS_UTILS_HPP

#include "types.h"

#include "CTRPluginFramework/System/Process.hpp"

#include <string>
#include <vector>
#include <cstring>

namespace CTRPluginFramework
{
    enum class HexEditorView
    {
        Byte = 0,
        Integer = 1,
        Disassembler = 2
    };

    using string16 = std::basic_string<uint16_t>;

    class Color;
    class Utils
    {
    public:
        /**
         * \brief Get a string formatted with format specifier from printf
         * \param fmt String to be formatted
         * \param ... Additional arguments
         * \return The formatted std::string
         */
        static std::string  Format(const char *fmt, ...);

        /**
         * \brief Get the hexadecimal representation of a value
         * \param value The value to convert
         * \return An std::string with the the hexadecimal representation of value
         */
        static std::string  ToHex(u32 value);

        /**
         * \brief Get the string representation of a float value
         * \param fpval The value to convert
         * \param precision The number of digit after the .
         * \return An std::string with the representation of the value\n
         * Values between -999999.f && 999999.f will be a fixed representation \n
         * while anything lower / higher will be a scientific representation
         */
        static std::string  ToString(float fpval, int precision = 2);

        /**
         * \brief Get a random number
         * \return A random number
         */
        static u32          Random(void);

        /**
         * \brief Get a random number
         * \param min Minimum value for the random number
         * \param max Maximum value for the random number
         * \return A random number between min & max
         */
        static u32          Random(u32 min, u32 max);

        /**
         * \brief Get the size of an utf8 std::string (max size 0x100)
         * \param str The string to count
         * \return The count of utf8 chars in the str
         */
        static u32          GetSize(const std::string &str);


        /**
         * \brief Open a menu which allow to browse the SD card to select a file
         * \param out The absolute path of the selected file
         * \param filter If the files must be filtered (can be an extension or a pattern that must be present in the filename)
         * \return -1 if the user aborted the operation, 0 on success
         */
        static int          FilePicker(std::string &out, const std::string &filter = "");

        /**
         * \brief Open a menu which allow to browse the SD card to select a Directory
         * \param out The absolute path of the selected directory
         * \return -1 if the user aborted the operation, 0 on success
         */
        static int          DirectoryPicker(std::string &out);

        /**
         * \brief Open the HexEditor menu at a specified address within the specified view
         * \param address The address to jump to in the HexEditor
         * \param view The type of view (can be changed when open)
         * \return The address where the cursor was when the HexEditor is closed
         */
        static u32          OpenInHexEditor(u32 address, HexEditorView view = HexEditorView::Byte);

        /**
         * \brief Remove the last char of an utf8 string (max size 0x100)
         * \param str The string to remove the char from
         * \return The codepoint value of the char removed
         */
        static u32          RemoveLastChar(std::string &str);

        /**
         * @brief Converts an UTF8 string into an UTF16 string16 (use the .c_str() method to obtain the raw characters).
         *
         * @param out Output string16 where the UTF16 characters will be appended.
         * @param utf8str Input UTF8 C string.
         */
        static void         ConvertUTF8ToUTF16(string16& out, const char* utf8str);

        /**
         * @brief Converts an UTF8 string into an UTF16 string16 (use the .c_str() method to obtain the raw characters).
         *
         * @param out Output string16 where the UTF16 characters will be appended.
         * @param utf8str Input UTF8 string.
         */
        static void         ConvertUTF8ToUTF16(string16& out, const std::string &utf8str) {ConvertUTF8ToUTF16(out, utf8str.c_str());}

        /**
         * @brief Converts an UTF16 string into an UTF8 string (use the .c_str() method to obtain the raw characters).
         *
         * @param out Output string where the UTF8 characters will be appended.
         * @param utf8str Input UTF16 C string.
         */
        static void         ConvertUTF16ToUTF8(std::string& out, const u16* utf16str);

        /**
         * @brief Converts an UTF16 string into an UTF8 string (use the .c_str() method to obtain the raw characters).
         *
         * @param out Output string where the UTF8 characters will be appended.
         * @param utf8str Input UTF16 string16.
         */
        static void         ConvertUTF16ToUTF8(std::string& out, const string16 &utf16str) {ConvertUTF16ToUTF8(out, utf16str.c_str());}

        template <typename T>
        static u32     Search(const u32 start, const u32 size, const std::vector<T> &pattern)
        {
            if (!start || !size || pattern.empty())
                return (0);

            const u32   patternSize = pattern.size() * sizeof(T);
            const u8    *patternc = reinterpret_cast<const u8 *>(pattern.data());
            const u8    *startPos = reinterpret_cast<const u8 *>(start);

            u32     table[256];

            for (u32 i = 0; i < 256; i++)
                table[i] = patternSize;

            for (u32 i = 0; i < patternSize - 1; i++)
                table[patternc[i]] = patternSize - i - 1;

            u32 j = 0;
            while (j <= size - patternSize)
            {
                const u8 c = startPos[j + patternSize - 1];
                if (patternc[patternSize - 1] == c && std::memcmp(patternc, startPos + j, patternSize - 1) == 0)
                    return (reinterpret_cast<u32>(startPos + j));
                j += table[c];
            }
            return (0);
        }

        enum class ConvertResult {
            SUCCESS = 0, // Conversion succeeded.
            PARTIAL = 1, // Conversion succeeded partially, up to the first invalid character in the string.
            INVALID = 2, // Conversion failed, the string starts with invalid characters.
            OUT_OF_RANGE = 3 // Conversion failed, the value cannot be represented in the given output type.
        };

        /**
         * @brief Converts a string to an integer.
         *
         * @tparam T
         * @param str String to convert, may be surrounded by whitespaces.
         * @param out Variable to store the result to. Can be int, unsigned int, float or double
         * @param mode Conversion base to use. Set to 0 for automatic base.
         * @return Result of the conversion. See ConvertResult.
         */
        template <typename T>
        static ConvertResult ToInteger(const std::string& str, T& out, int base = 0) {
            return ToInteger(str.c_str(), out, base);
        }

        /**
         * @brief Converts a string to an integer.
         *
         * @tparam T
         * @param str String to convert, may be surrounded by whitespaces.
         * @param out Variable to store the result to. Can be int, unsigned int, float or double
         * @param mode Conversion base to use. Set to 0 for automatic base.
         * @return Result of the conversion. See ConvertResult.
         */
        template <typename T>
        static ConvertResult ToInteger(const char* str, T& out, int base = 0) {
            char* parsed = nullptr;
            errno = 0;
            ToIntegerImpl(str, &parsed, base, out);
            if (errno == ERANGE) {return ConvertResult::OUT_OF_RANGE;}
            if (parsed == str) {return ConvertResult::INVALID;}
            while (*parsed) {if (!std::isspace(*parsed++)) return ConvertResult::PARTIAL;}
            return ConvertResult::SUCCESS;
        }

        template <typename T>
        static u32     Rsearch(const u32 start, const u32 size, const std::vector<T> &pattern)
        {
            if (!start || !size || pattern.empty())
                return (0);

            const u32   patternSize = pattern.size() * sizeof(T);
            const u8    *patternc = reinterpret_cast<const u8 *>(pattern.data());
            const u8    *startPos = reinterpret_cast<const u8 *>(start);

            u32     table[256];

            for (u32 i = 0; i < 256; i++)
                table[i] = patternSize;

            for (u32 i = 0; i < patternSize - 1; i++)
                table[patternc[i]] = patternSize - i - 1;

            u32 j = 0;
            u32 last = 0;
            while (j <= size - patternSize)
            {
                const u8 c = startPos[j + patternSize - 1];
                if (patternc[patternSize - 1] == c && std::memcmp(patternc, startPos + j, patternSize - 1) == 0)
                    last = reinterpret_cast<u32>(startPos + j);
                j += table[c];
            }
            return (last);
        }
    private:
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, int& out) {out = std::strtol(cstr, parsed, base);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, int32_t& out) {out = std::strtol(cstr, parsed, base);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, unsigned int& out) {out = std::strtoul(cstr, parsed, base);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, uint32_t& out) {out = std::strtoul(cstr, parsed, base);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, int64_t& out) {out = std::strtoll(cstr, parsed, base);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, uint64_t& out) {out = std::strtoull(cstr, parsed, base);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, float& out) {out = std::strtof(cstr, parsed);}
        static void ToIntegerImpl(const char* cstr, char** parsed, int base, double& out) {out = std::strtod(cstr, parsed);}
    };
}

#endif