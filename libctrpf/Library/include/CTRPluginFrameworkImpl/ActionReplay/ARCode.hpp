#ifndef CTRPLUGINFRAMEWORKIMPL_ACTIONREPLAY_ARCODE_HPP
#define CTRPLUGINFRAMEWORKIMPL_ACTIONREPLAY_ARCODE_HPP

#include "types.h"
#include "CTRPluginFramework/System/File.hpp"
#include "CTRPluginFramework/System/Hook.hpp"

#include <string>
#include <vector>

namespace CTRPluginFramework
{
    namespace ActionReplayPriv
    {
        u32     Str2U32(const std::string &str, bool &error);
    }

    class File;
    class MenuItem;
    class MenuFolderImpl;
    class LineWriter;

    class ARCode
    {
    public:
        bool    HasError;
        u8      Type;
        u32     Left;
        u32     Right; ///< Also size for E code
        std::vector<u32> Data; ///< Data for E code
        std::string Text; ///< Original data when there's an error detected

        ARCode(const ARCode &code);
        ARCode(u8 type, u32 left, u32 right);
        ARCode(const std::string &line, bool &error);
        //virtual ~ARCode(void) = default;
        std::string ToString(void) const;

        bool    IsCodeWithData(void) const;
        bool    Update(const std::string &line);
        bool    Update(void);
    };

    using ARCodeVector = std::vector<ARCode>;
    using HookVector = std::vector<Hook>;

    struct ARCodeContext
    {
        ARCodeContext();
        bool            hasError;   ///< True if any of the codes has an unrecognized char
        u32             storage[2]; ///< Storage for this code (persistent register)
        ARCodeVector    codes;      ///< List of all codes
        HookVector      *hooks;     ///< List of all hooks

        bool            Update(void);
        void            Clear(void);
    };

    extern const std::vector<u8> g_codeTypes;

    void    ActionReplay_OpenCheatsFile(File &output, bool create);
    /**
     * \brief Check if the code type is supported by this handler
     * \param left Left code value
     * \return true if the code type is valid
     */
    bool    ActionReplay_CheckCodeTypeValidity(u32 left);
    /**
     * \brief Check if the line has a valid code pattern (hex) 00000000 00000000
     * \param line The line to check
     * \return true if the line has correct hex values
     */
    bool    ActionReplay_IsHexCode(const std::string &line);
    /**
    * \brief Check if the line passed is a valid code
    * \param line The string to check
    * \return true if the line is a valid code
    */
    bool    ActionReplay_IsValidCode(const std::string &line);
    void    ActionReplay_ProcessString(std::string &str, bool canNewLine = true);
    void    ActionReplay_LoadCodes(MenuFolderImpl *dst);
    bool    ActionReplay_WriteToFile(LineWriter &file, MenuItem *item);
}

#endif
