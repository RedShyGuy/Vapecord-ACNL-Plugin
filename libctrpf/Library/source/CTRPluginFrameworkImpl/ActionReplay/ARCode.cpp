#include "CTRPluginFrameworkImpl/ActionReplay/ARCode.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include <string>

namespace CTRPluginFramework
{
    // A list of all supported code types
    const std::vector<u8> g_codeTypes =
    {
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0,
        0xB0, 0xD3, 0xDC, 0xC0, 0xD2, 0xD1, 0xD0, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xE0, 0xDD, 0xDE, 0xDF, 0xF0, 0xF1, 0xF2,
        0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD,
        0xFE, 0xFF
    };

    namespace ActionReplayPriv
    {
        u32     Str2U32(const std::string &str, bool &error)
        {
            u32 val = 0;
            u32 index = 0;
            const u8 *hex = (const u8 *)str.c_str();

            error = false;
            if (str.empty())
                return (val);

            while (*hex && ++index < 9)
            {
                u8 byte = (u8)*hex++;

                if (byte >= '0' && byte <= '9') byte = byte - '0';
                else if (byte >= 'a' && byte <= 'f') byte = byte - 'a' + 10;
                else if (byte >= 'A' && byte <= 'F') byte = byte - 'A' + 10;
                else ///< Incorrect char
                {
                    error = true;
                    return (0);
                }

                val = (val << 4) | (byte & 0xF);
            }
            return (val);
        }
    }

    ARCode::ARCode(const std::string &line, bool &error)
    {
        Text = line;
        if (line.size() < 17 || !ActionReplay_IsValidCode(line))
        {
            error = HasError = true;
            return; // An error occured
        }

        std::string lStr = line.substr(0, 8);
        std::string rStr = line.substr(9, 8);


        Left = ActionReplayPriv::Str2U32(lStr, HasError);
        Right = ActionReplayPriv::Str2U32(rStr, error);

        error |= HasError;
        if (error)
        {
            HasError = error;
            return;
        }

        Type = Left >> 24;

        u8 type = Type >> 4;
        if (type == 0xC || type == 0xD || type == 0xF)
            Left &= 0x00FFFFFF;
        else
        {
            Type &= 0xF0;
            Left &= 0x0FFFFFFF;
        }
        error = HasError = false;
    }

    bool ARCode::IsCodeWithData(void) const
    {
        if (Type == 0xE0 || Type == 0xFD || Type == 0xFE
            || (Type == 0xF0 && Left == 0x00F00000))
            return true;
        return false;
    }

    bool    ARCode::Update(const std::string &line)
    {
        if (line.size() < 17)
        {
            Text = line;
            HasError = true;
            return HasError;
        }

        std::string lStr = line.substr(0, 8);
        std::string rStr = line.substr(9, 8);

        bool error = false;
        Left = ActionReplayPriv::Str2U32(lStr, HasError);
        Right = ActionReplayPriv::Str2U32(rStr, error);
        HasError |= error;

        if (HasError)
        {
            Text = line;
            return HasError;
        }

        Type = Left >> 24;

        u8 type = Type >> 4;
        if (type == 0xC || type == 0xD || type == 0xF)
            Left &= 0x00FFFFFF;
        else
        {
            Type &= 0xF0;
            Left &= 0x0FFFFFFF;
        }

        HasError = false;
        return HasError;
    }

    bool    ARCode::Update(void)
    {
        if (Type == 0xE0 || Type == 0xFE || Type == 0xFD)
            return HasError;

        if (HasError && !Text.empty())
            Update(Text);

        if (!HasError && !Text.empty())
            Text.clear();

        return HasError;
    }

    ARCodeContext::ARCodeContext() :
        hasError{ false }, hooks{ nullptr }
    {
        storage[0] = 0;
        storage[1] = 0;
    }

    std::string     ARCode::ToString(void) const
    {
        return (Utils::Format("%08X %08X", Type << 24 | Left, Right));
    }

    ARCode::ARCode(const ARCode &code) :
        HasError(code.HasError), Type(code.Type), Left(code.Left), Right(code.Right),
        Data(code.Data), Text(code.Text)
    {
    }

    ARCode::ARCode(u8 type, u32 left, u32 right) :
        HasError(false), Type(type), Left(left), Right(right)
    {
    }

    bool    ARCodeContext::Update(void)
    {
        bool error = false;

        for (ARCode &code : codes)
            error |= code.Update();
        hasError = error;
        return hasError;
    }

    void    ARCodeContext::Clear(void)
    {
        hasError = false;
        //data.clear();
        storage[0] = storage[1] = 0;
        codes.clear();

        if (hooks != nullptr)
        {
            for (Hook &hook : *hooks)
                hook.Disable();
            hooks->clear();
        }
    }
}
