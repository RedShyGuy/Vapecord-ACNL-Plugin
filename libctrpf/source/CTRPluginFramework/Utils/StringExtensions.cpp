#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFramework/Graphics/Render.hpp"
#include "CTRPluginFramework/Utils/LineReader.hpp"
#include <algorithm>
#include <string>
#include <sstream>

namespace CTRPluginFramework
{
    std::string     ResetColor(void)
    {
        return ("\x18");
    }

    std::string     ToggleDrawMode(u32 flags)
    {
        char code[4];
        code[0] = '\x11';
        *(u16*)&code[1] = 0x4080 | (flags & 0x7F);
        code[3] = '\0';
        return (code);
    }

    std::string     SkipToPixel(u16 px)
    {
        char code[4];
        if (px == 0)
        {
            code[0] = '\0';
        } else
        {
            code[0] = '\x11';
            *(u16*)&code[1] = 0x8000 | (px & 0x1FF);
            code[3] = '\0';
        }
        return (code);
    }

    std::string     SetShake(bool horizontalDirection, bool verticalDirection, u8 amount) {
        char code[4];
        if (amount >= 63)
        {
            code[0] = '\0';
        }
        else
        {
            code[0] = '\x11';
            *(u16*)&code[1] = 0x2100 | (horizontalDirection ? 0x80 : 0) | (verticalDirection ? 0x40 : 0) | ((amount + 1) & 0x3F);
            code[3] = '\0';
        }
        return (code);
    }

    std::string     SetRandomText(int randomSetID) {
        char code[4];
        if (randomSetID < 0) {
            code[0] = '\x11';
            *(u16*)&code[1] = 0x2280;
            code[3] = '\0';
        }
        else
        {
            if (randomSetID >= 63)
            {
                code[0] = '\0';
            }
            else
            {
                code[0] = '\x11';
                *(u16*)&code[1] = 0x2200 | ((randomSetID + 1) & 0x3F);
                code[3] = '\0';
            }
        }
        return (code);
    }

    static size_t     SafeStringHalve(const std::string& str) {
        size_t middle = str.length()/2;
        const char* strC = str.c_str();
        for (u32 i = 0; i < 4 && middle >= i; i++)
        {
            char c = strC[middle - i];
            if (c == 0x11 || c == 0x1B) { // Special characters, cannot cut, max length special character is 4.
                middle = middle - i;
                i = 0;
                if (middle == 0)
                    break;
            }
        }
        return middle;
    }

    static std::string     CenterAlignImpl(const std::string& str, u16 leftBorder, u16 rightBorder, int rec)
    {
        if (rec >= 10) // Hacky and lazy way to prevent infinite recursion.
            return "";

        std::string line;
        std::string out;
        StringLineReader reader(str);
        while(reader(line))
        {
            u32 size = (u32)Render::GetTextWidth(line);
            if (size > (u32)rightBorder - (u32)leftBorder)
            {
                size_t middle = SafeStringHalve(line);
                std::string left = line.substr(0, middle);
                std::string right = line.substr(middle);
                out += CenterAlignImpl(left, leftBorder, rightBorder, rec+1) + "\n" + CenterAlignImpl(right, leftBorder, rightBorder, rec+1) + "\n";
            } else {
                u16 offset = (((rightBorder - leftBorder) - size) / 2) + leftBorder;
                out += SkipToPixel(offset) + line + "\n";
            }
        }
        if (!out.empty() && !str.empty() && str[str.length() -1] != '\n')
            out.pop_back(); // Remove last \n
        return out;
    }

    std::string     CenterAlign(const std::string& str, u16 leftBorder, u16 rightBorder)
    {
        return CenterAlignImpl(str, leftBorder, rightBorder, 0);
    }

    std::string     RightAlignImpl(const std::string& str, u16 leftBorder, u16 rightBorder, int rec)
    {
        if (rec >= 10) // Hacky and lazy way to prevent infinite recursion.
            return "";

        std::string line;
        std::string out;
        StringLineReader reader(str);
        while(reader(line))
        {
            u32 size = (u32)Render::GetTextWidth(line);
            if ((size + 1) > (u32)rightBorder - (u32)leftBorder)
            {
                size_t middle = SafeStringHalve(line);
                std::string left = line.substr(0, middle);
                std::string right = line.substr(middle);
                out += RightAlignImpl(left, leftBorder, rightBorder, rec+1) + "\n" + RightAlignImpl(right, leftBorder, rightBorder, rec+1) + "\n";
            } else {
                u16 offset = rightBorder - (size + 1);
                out += SkipToPixel(offset) + line + "\n";
            }
        }
        if (!out.empty() && !str.empty() && str[str.length() -1] != '\n')
            out.pop_back(); // Remove last \n
        return out;
    }

    std::string     RightAlign(const std::string& str, u16 leftBorder, u16 rightBorder)
    {
        return RightAlignImpl(str, leftBorder, rightBorder, 0);
    }

    std::string     HorizontalSeparator()
    {
        return "\n" << ToggleDrawMode(Render::FontDrawMode::STRIKETHROUGH) << " " << RightAlign(" ", 35, 366) << ToggleDrawMode(Render::FontDrawMode::STRIKETHROUGH) << "\n";
    }

    std::string operator<<(const std::string &left, const char* right)
    {
        return (left + right);
    }

    std::string operator <<(const std::string &left, const std::string &right)
    {
        return (left + right);
    }

    /*std::string operator <<(const std::string &left, const Color &color)
    {
        char  strColor[5] = { 0 };

        strColor[0] = 0x1B;
        strColor[1] = std::max((u8)1, color.r);
        strColor[2] = std::max((u8)1, color.g);
        strColor[3] = std::max((u8)1, color.b);

        return (left + strColor);
    }

    std::string operator <<(const Color &color, const std::string &right)
    {
        char  strColor[5] = { 0 };

        strColor[0] = 0x1B;
        strColor[1] = std::max((u8)1, color.r);
        strColor[2] = std::max((u8)1, color.g);
        strColor[3] = std::max((u8)1, color.b);

        return (strColor + right);
    } */
}
