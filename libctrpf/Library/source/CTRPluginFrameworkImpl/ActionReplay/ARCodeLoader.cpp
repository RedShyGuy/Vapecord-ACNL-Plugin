#include "types.h"
#include "3ds.h"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/System/File.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCode.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/MenuEntryActionReplay.hpp"
#include <algorithm>
#include <stack>
#include <locale>
#include <cctype>
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#undef DEBUG
#undef TRACE
#undef XTRACE

#define DEBUG 0

#if DEBUG
#define TRACE  { OSDManager["trace"].SetScreen(true).SetPos(10,50) = std::string(__FUNCTION__) << ":" << __LINE__; Sleep(Seconds(0.04f)); }
#define XTRACE(str, ...) { OSDManager["trace1"].SetScreen(true).SetPos(0,30) = std::string(__FUNCTION__) << ":" << __LINE__ << Utils::Format(" " str, ##__VA_ARGS__); Sleep(Seconds(0.10f)); }
#define XTRACE2(str, ...) { OSDManager["trace2"].SetScreen(true).SetPos(0,40) = std::string(__FUNCTION__) << ":" << __LINE__ << Utils::Format(" " str, ##__VA_ARGS__); Sleep(Seconds(0.10f)); }
#define XTRACE3(str, ...) { OSDManager["trace3"].SetScreen(true).SetPos(0,50) = std::string(__FUNCTION__) << ":" << __LINE__ << Utils::Format(" " str, ##__VA_ARGS__); Sleep(Seconds(0.10f)); }
#define XTRACE4(str, ...) { OSDManager["trace4"].SetScreen(true).SetPos(0,60) = std::string(__FUNCTION__) << ":" << __LINE__ << Utils::Format(" " str, ##__VA_ARGS__); Sleep(Seconds(0.10f)); }

#else
#define TRACE
#define XTRACE(str, ...)
#define XTRACE2(str, ...)
#define XTRACE3(str, ...)
#define XTRACE4(str, ...)
#endif

namespace CTRPluginFramework
{
    void    ActionReplay_OpenCheatsFile(File &output, bool create)
    {
        u32 flags = File::RW | File::SYNC;

        if (create)
            flags |= File::TRUNCATE | File::CREATE;

        File::Open(output, Preferences::CheatsFile, flags);
    }

    bool    ActionReplay_CheckCodeTypeValidity(u32 left)
    {
        u8 ct = left >> 24;
        for (const u8 code : g_codeTypes)
        {
            if ((code & 0xF) == 0)
            {
                if ((code >> 4) == (ct >> 4))
                    return true;
            }
            else
                if (code == ct)
                    return true;
        }

        return false;
    }

    bool    ActionReplay_IsHexCode(const std::string &line)
    {
        u32 index = 0;
        for (const char c : line)
        {
            if (index != 8 && !std::isxdigit(c))
                return (false);
            ++index;
            if (index >= 16)
                break;
        }

        return (true);
    }

    bool    ActionReplay_IsValidCode(const std::string &line)
    {
        // Check that the line has a valid hex pattern
        if (!ActionReplay_IsHexCode(line))
            return false;

        // If so check that the code type is valid
        bool error = false;
        u32 left = ActionReplayPriv::Str2U32(line, error);

        return !(error || !ActionReplay_CheckCodeTypeValidity(left)); ///< The code is valid
    }

    static bool     LineEndWith(const char c, std::string &line)
    {
        return (line.size() && line[line.size() - 1] == c);
    }

    static inline std::string &Ltrim(std::string &str)
    {
        auto it = std::find_if(str.begin(), str.end(), [](char ch) { return (!std::isspace(ch)); });
        str.erase(str.begin(), it);
        return (str);
    }

    static inline std::string &Rtrim(std::string &str)
    {
        auto it = std::find_if(str.rbegin(), str.rend(), [](char ch) { return (!std::isspace(ch)); });
        str.erase(it.base(), str.end());
        return (str);
    }

    static inline std::string &Trim(std::string &str)
    {
        return (Ltrim(Rtrim(str)));
    }

    static std::string ConvertToUTF8(const std::string &str, bool &error)
    {
        u32     code = 0;
        char    buffer[10] = { 0 };

        if (str[0] != '\\' && str[1] != 'u')
        {
            error = true;
            return (buffer);
        }

        error = false;
        code = ActionReplayPriv::Str2U32(str.substr(2, 4), error);
        if (error)
            return (buffer);
        encode_utf8(reinterpret_cast<u8 *>(buffer), code);
        return (buffer);
    }

    void    ActionReplay_ProcessString(std::string &str, bool canNewLine)
    {
        const u32 strSize = str.size();

        if (strSize <= 1)
            return;

        // Process our string
        for (u32 i = 0; i < strSize - 2; i++)
        {
            const char c = str[i];

            if (c == '\n' && !canNewLine)
            {
                str.erase(i, 1);
                str.insert(i, "\\n");
                continue;
            }
            if (c == '\\')
            {
                // Check the symbol \n
                if (str[i + 1] == 'n' && canNewLine)
                {
                    str[i] = '\n';
                    str = str.erase(i + 1, 1);
                }
                // Check any unicode attempt
                else if (str[i + 1] == 'u')
                {
                    bool error = false;
                    std::string utf8 = ConvertToUTF8(str.substr(i, 6), error);

                    if (utf8.empty() || error) continue;

                    str.erase(i, 6);
                    str.insert(i, utf8);
                }
            }
            // Check if we found a color pattern
            else if (c == '~')
            {
                char        strColor[5] = { 0 };
                const char  *cstr = str.c_str() + i + 1;
                const int   leftSize = strSize - i -1; ///< size left of the current string from the '~' + 1 character


                // Check if it's a hex color
                if (*cstr == '#' && (leftSize > 7) && *(cstr + 7) == '~')
                {
                    bool        error = false;
                    std::string &&hexpattern = str.substr(i + 2, 6);
                    u32         hex = ActionReplayPriv::Str2U32(hexpattern, error);

                    // If it's a valid hex pattern
                    if (!error)
                    {
                        Color color(hex << 8); ///< Ctor expect 0xRGBA we have 0x0RGB

                        str.erase(i, 9); /// ~#RRGGBB~ => 9 chars
                        strColor[0] = 0x1B;
                        strColor[1] = std::max((u8)1, color.r);
                        strColor[2] = std::max((u8)1, color.g);
                        strColor[3] = std::max((u8)1, color.b);
                        str.insert(i, strColor);
                        i += 3;
                    }
                    continue;
                }

                // Check if it's a predefined color
                struct PColor
                {
                    PColor(const char *n, const Color &col) :
                        name(n), color(col)
                    {
                    }
                    const char *name;
                    const Color &color;
                };

                static const std::vector<PColor> colors =
                {
                    PColor("white", Color::White), PColor("black", Color::Black),
                    PColor("red", Color::Red), PColor("green", Color::Green), PColor("blue", Color::Blue),
                    PColor("cyan", Color::Cyan), PColor("magenta", Color::Magenta), PColor("yellow", Color::Yellow), PColor("gray", Color::Gray)
                };

                for (const PColor &color : colors)
                {
                    u32 colorsize = std::strlen(color.name);
                    if (!std::strncmp(cstr, color.name, colorsize) && *(cstr + colorsize) == '~')
                    {
                        str.erase(i, colorsize + 2);
                        strColor[0] = 0x1B;
                        strColor[1] = std::max((u8)1, color.color.r);
                        strColor[2] = std::max((u8)1, color.color.g);
                        strColor[3] = std::max((u8)1, color.color.b);
                        str.insert(i, strColor);
                        i += 3;
                        break;
                    }
                }
            }
        }
    }

    static bool    ActionReplay_GetData(std::string &line, ARCodeContext *codectx, int &index)
    {
        if (!codectx)
            return (false);

        bool            error = false;
        std::vector<u32> &data = codectx->codes.back().Data;
        std::string     &&leftstr = line.substr(0, 8);
        std::string     &&rightstr = line.substr(9, 8);

        data[index++] = ActionReplayPriv::Str2U32(leftstr, error);
        if (error) goto exit;
        data[index++] = ActionReplayPriv::Str2U32(rightstr, error);
        if (error) goto exit;

    exit:
        codectx->hasError = error;
        return (error);
    }

    using FolderStack = std::stack<MenuFolderImpl*>;
    void    ActionReplay_LoadCodes(MenuFolderImpl *dst)
    {
        File            file;
        LineReader      reader(file);
        MenuEntryActionReplay   *entry = nullptr;
        FolderStack     folders;
        std::string     line;
        std::string     name;
        std::string     note;
        bool            error = false;
        bool            ecode = false;
        int             count = 0;
        int             index = 0;

        ActionReplay_OpenCheatsFile(file, false);
        if (!file.IsOpen())
            return;

        folders.push(dst);

        // While there's lines in the file
        while (reader(line))
        {
            XTRACE("New line, free space: %08X", getMemFree());
            // Remove spaces in the left
            Ltrim(line);

            // If line is empty
            if (line.empty())
            {
                if (entry)
                    folders.top()->Append(entry->Update());
                entry = nullptr;
                name.clear();
                error = ecode = false;
                continue;
            }
            XTRACE2("%s", line.c_str());

            // If line is not a code
            if (!ActionReplay_IsHexCode(line) || error)
            {
                XTRACE3("Not a code");
                // If we found a pattern
                if (line[0] == '[')
                {
                    if (entry)
                        folders.top()->Append(entry->Update());
                    entry = nullptr;

                    Rtrim(line);
                    // If line is a folder opening pattern
                    if (line.substr(0, 3) == "[++")
                    {
                        name = line.substr(3, line.size() - 6);
                        Trim(name);
                        ActionReplay_ProcessString(name, false);
                        folders.push(new MenuFolderImpl(name));
                        continue;
                    }

                    // If line is a folder close pattern
                    if (line == "[--]")
                    {
                        if (folders.size() > 1)
                        {
                            MenuFolderImpl *f = folders.top();
                            folders.pop();
                            folders.top()->Append(f);
                        }

                        continue;
                    }

                    // Else it's a new code pattern
                    name = line.substr(1, line.size() - 2);
                    Trim(name);
                    ActionReplay_ProcessString(name, false);
                    entry = new MenuEntryActionReplay(name);
                    error = false;
                    continue;
                }
                // If we found a note pattern
                if (entry && line[0] == ('{'))
                {
                    XTRACE("Note");
                    Rtrim(line);
                    note = line.substr(1);

                    while (!LineEndWith('}', line))
                    {
                        XTRACE3("Wait for end: %s", line.c_str());
                        if (!reader(line))
                            break;
                        note += Rtrim(line);
                    }
                    note.pop_back();
                    Trim(note);
                    ActionReplay_ProcessString(note);
                    entry->note = note;
                    note.clear();
                    continue;
                }

                // Else consider the line as error
                //if (entry)
                //    entry->context.data += line + "\r\n";
                //error = true;

                // Else skip the line, if we're in the middle of a code,
                // End it
                if (entry)
                    folders.top()->Append(entry->Update());
                entry = nullptr;
                name.clear();
                error = ecode = false;
                continue;
            }

            // If there's no entry at this point, skip the line
            if (!entry)
                continue;

            // Add current line to ctx in case of error later
            //entry->context.data += line + "\r\n";

            // If we're in E or FE, FD mode
            if (ecode)
            {
                if (!ActionReplay_GetData(line, &entry->context, index))
                {
                    --count;
                    ecode = count > 0;
                }
                else
                {
                    entry->context.codes.back().HasError = error = true;
                }
                continue;
            }

            // Get ARCode object from line
            ARCode code(line, error);
            if (error)
                XTRACE4("Error: %s", line.c_str());
            // If the code is a E code (data)
            ecode = !error && code.IsCodeWithData();
            if (ecode)
            {
                u32 bytes = code.Type == 0xFE ? code.Left : code.Right;

                if (bytes)
                {
                    count = bytes / 8 + (bytes % 8 > 0 ? 1 : 0);
                    code.Data.resize(count * 2);
                    index = 0;
                }
                else
                {
                    ecode = false;
                }
            }

            // Add ARCode to context
            entry->context.codes.push_back(code);
        }

        // Add the last code
        if (entry)
            folders.top()->Append(entry->Update());
    }
}
