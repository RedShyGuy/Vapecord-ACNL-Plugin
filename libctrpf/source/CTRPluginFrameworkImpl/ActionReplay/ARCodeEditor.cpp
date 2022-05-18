#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCodeEditor.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Menu/HexEditor.hpp"
#include "Unicode.h"
#include "CTRPluginFrameworkImpl/Disassembler/arm_disasm.h"

#define PATCH_COLOR Color::Gray
#define TYPE_COLOR Color::Brown
#define IMMEDIATE_COLOR Color::Blue
#define MASK_COLOR Color::Orange
#define OFFSET_COLOR Color::ForestGreen
#define CONTROL_COLOR Color::Red
#define UNUSED_COLOR Color::Black

namespace CTRPluginFramework
{
    enum CondMode
    {
        ImmAgainstVal,  // YYYYYYYY > [XXXXXXX + offset]
        DataAgainstVal, // data > [XXXXXXX + offset]
        ImmAgainstData,  // YYYYYYYY > data
        ImmAgainstStorage, // YYYYYYYY > storage
        DataAgainstStorage, // storage > data
    };
    static const char       *__emptyCode = "00000000 00000000";
    static ARCodeEditor     *__arCodeEditor = nullptr;

    static CondMode g_condAgainstData = CondMode::ImmAgainstVal;
    static bool g_newCondDataToggle = false;

#define IsEmpty (flags & ARCodeEditor::CodeLine::Empty)
#define IsError (flags & ARCodeEditor::CodeLine::Error)
#define IsModified (flags & ARCodeEditor::CodeLine::Modified)
#define IsPatch (flags & ARCodeEditor::CodeLine::PatchData)
#define IsPattern (flags & ARCodeEditor::CodeLine::PatternData)
#define IsAsm (flags & ARCodeEditor::CodeLine::Asm)
#define IsData (flags & (u32)(ARCodeEditor::CodeLine::PatchData | ARCodeEditor::CodeLine::PatternData | ARCodeEditor::CodeLine::Asm))
#define BClear(bit) (flags &= ~bit)
#define BSet(bit) (flags |= bit)

    static std::string      ColorToString(const Color &color)
    {
        char  strColor[5] = { 0 };

        strColor[0] = 0x1B;
        strColor[1] = std::max((u8)1, color.r);
        strColor[2] = std::max((u8)1, color.g);
        strColor[3] = std::max((u8)1, color.b);

        return (strColor);
    }

#define ARROW_UP "\xFF"
#define ARROW_DOWN "\x19"
#define ARROW_LEFT "\xFE"
#define ARROW_RIGHT "\x1A"

    static std::string  KeysToString(u32 keys)
    {
        static const char * keysText[] =
        {
            "A", "B", "Select", "Start", ARROW_RIGHT, ARROW_LEFT, ARROW_UP, ARROW_DOWN,
            "R", "L", "X", "Y", "", "", "ZL", "ZR", "", "", "", "", "Touch", "", "", "",
            "SR", "SL", "SU", "SD", "CR", "CL", "CU", "CD"
        };

        std::string ret;
        bool plus = false;

        for (u32 i = 0; i < 32; ++i)
        {
            if (keys & (1u << i))
            {
                std::string key = keysText[i];
                if (key.empty())
                    continue;
                if (plus)
                    ret += '+';
                ret += key;
                plus = true;
            }
        }

        return ret;
    }

    static std::string      ColorCodeLine(const ARCode &code)
    {
        std::string ret = code.Text;

        switch (code.Type)
        {
        case 0x00: ///< Write32
        case 0x30: ///< GT 32Bits
        case 0x40: ///< LT 32Bits
        case 0x50: ///< EQ 32Bits
        case 0x60: ///< NE 32Bits
        case 0xE0: ///< Patch code
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(5 /* 1 + Color */, ColorToString(OFFSET_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            break;
        case 0x10: ///< Write16
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(5 /* 1 + Color */, ColorToString(OFFSET_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(UNUSED_COLOR));
            ret.insert(25 /* 13 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            break;
        case 0x20: ///< Write8
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(5 /* 1 + Color */, ColorToString(OFFSET_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(UNUSED_COLOR));
            ret.insert(27 /* 15 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            break;

        case 0x70: ///< GT 16Bits
        case 0x80: ///< LT 16Bits
        case 0x90: ///< EQ 16Bits
        case 0xA0: ///< NE 16Bits
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(5 /* 1 + Color */, ColorToString(OFFSET_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(MASK_COLOR));
            ret.insert(25 /* 13 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            break;

        case 0xB0: ///< Read & Set Offset
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(5 /* 1 + Color */, ColorToString(OFFSET_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(UNUSED_COLOR));
            break;

        case 0xC0: ///< Set loop count
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(5 /* 1 + Color */, ColorToString(UNUSED_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            break;

        case 0xC1:
        case 0xC2:
        case 0xDC: ///< Add to offset
        case 0xD4: ///< Add to register
        case 0xD6: ///< Set data
        case 0xD7:
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDD: ///< Check input
        case 0xF4: ///< MUL
        case 0xF5: ///< DIV
        case 0xF6: ///< AND
        case 0xF7: ///< OR
        case 0xF8: ///< XOR
        case 0xFA: ///< LSL
        case 0xFB: ///< LSR
        case 0xFC: ///< Copy
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(UNUSED_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            break;

        case 0xD0: ///< Terminator
        case 0xD2: ///< Full terminator
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(UNUSED_COLOR));
            if (code.Right)
                ret.insert(24 /* 16 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            break;

        case 0xD1: ///< Loop execute
        case 0xF9: ///< NOT operation
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(UNUSED_COLOR));
            break;

        case 0xD3: ///< Set offset
        case 0xD5: ///< Set data register
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(UNUSED_COLOR));

            if (code.Left == 0)
                ret.insert(17 /* 9 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            else
            {
                ret.insert(15 /* 7 + Color * 2 */, ColorToString(Color::Red));
                ret.insert(21 /* 9 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            }
            break;

        case 0xDE: ///< Check touchpad:
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(UNUSED_COLOR));
            ret.insert(15 /* 7 + Color * 2 */, ColorToString(Color::Red));
            ret.insert(21 /* 9 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            ret.insert(29 /* 13 + Color * 4 */, ColorToString(Color::Green));
            break;

        case 0xDF: ///< Register
            if (code.Left == 0xFFFFFF) ///< Cond mod toggle
            {
                ret.insert(0, ColorToString(TYPE_COLOR));
                ret.insert(6 /* 2 + Color */, ColorToString(MASK_COLOR));
                ret.insert(17 /* 9 + Color * 2 */, ColorToString(UNUSED_COLOR));
                ret.insert(28 /* 16 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
                break;
            }
            if (code.Left == 0xFFFFFE) ///< VFP state
            {
                ret.insert(0, ColorToString(TYPE_COLOR));
                ret.insert(6 /* 2 + Color */, ColorToString(MASK_COLOR));
                ret.insert(17 /* 9 + Color * 2 */, ColorToString(UNUSED_COLOR));
                ret.insert(27 /* 15 + Color * 3 */, ColorToString(Color::Red));
                ret.insert(32 /* 16 + Color * 4 */, ColorToString(IMMEDIATE_COLOR));
                break;
            }
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(Color::Gray));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(MASK_COLOR));
            ret.insert(25 /* 13 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            break;
        case 0xF0:
            if (code.Left == 1)
            {
                ret.insert(0, ColorToString(TYPE_COLOR));
                ret.insert(6 /* 2 + Color */, ColorToString(MASK_COLOR));
                ret.insert(17 /* 9 + Color * 2 */, ColorToString(UNUSED_COLOR));
                ret.insert(28 /* 16 + Color * 3 */, ColorToString(IMMEDIATE_COLOR));
            }
            else if (code.Left == 0x00F00000)
            {
                ret.insert(0, ColorToString(TYPE_COLOR));
                ret.insert(6 /* 2 + Color */, ColorToString(MASK_COLOR));
                ret.insert(17 /* 9 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            }
            else
                ret.insert(0, ColorToString(Color::Red));
            break;
        case 0xF1: ///< Custom Codes
        case 0xF2:
        case 0xF3:
        case 0xFE: ///< Memsearch
        case 0xFF: ///< Random
            ret.insert(0, ColorToString(TYPE_COLOR));
            ret.insert(6 /* 2 + Color */, ColorToString(OFFSET_COLOR));
            ret.insert(17 /* 9 + Color * 2 */, ColorToString(IMMEDIATE_COLOR));
            break;

        default:
            ret.insert(0, ColorToString(Color::Red));
            break;
        }
        return (ret);
    }

    static std::string      GetCond32Str(u32 left, u32 right, const char *ope)
    {
        if (g_condAgainstData == CondMode::ImmAgainstVal)
            return Utils::Format("if %08X %s [%07X+off]:", right, ope, left);
        else if (g_condAgainstData == CondMode::DataAgainstVal)
            return Utils::Format("if data %s [%07X+offs]:", ope, left);
        else if (g_condAgainstData == CondMode::ImmAgainstStorage)
            return Utils::Format("if %08X %s storage:", right, ope);
        else if (g_condAgainstData == CondMode::DataAgainstStorage)
            return Utils::Format("if data %s storage:", ope);
        else // CondMode::ImmAgainstData
            return Utils::Format("if %08X %s data:", right, ope);
    }

    static std::string      GetCond16Str(u32 left, u32 right, const char *ope)
    {
        u32 mask = right >> 16;
        u32 value = right & 0xFFFF;

        if (mask) mask = (~mask) & 0xFFFF;

        if (g_condAgainstData == CondMode::ImmAgainstVal)
        {
            if (mask)
                return Utils::Format("if %04X%s[%07X+off] & %04X:", value, ope, left, mask);
            else
                return Utils::Format("if %04X %s [%07X+offs]:", value, ope, left);
        }
        else if (g_condAgainstData == CondMode::DataAgainstVal)
        {
            if (mask)
                return Utils::Format("if data%s[%07X+of] & %04X:", ope, left, mask);
            else
                return Utils::Format("if data %s [%07X+offs]:", ope, left);
        }
        else if (g_condAgainstData == CondMode::ImmAgainstStorage)
        {
            if (mask)
                return Utils::Format("if %04X %s stor. & %04X:", value, ope, mask);
            else
                return Utils::Format("if %04X %s storage:", value, ope);
        }
        else if (g_condAgainstData == CondMode::DataAgainstStorage)
        {
                return Utils::Format("if data %s storage:", ope);
        }
        else // CondMode::ImmAgainstData
        {
            if (mask)
                return Utils::Format("if %04X %s data & %04X:", value, ope, mask);
            else
                return Utils::Format("if %04X %s data:", value, ope);
        }
    }

    static std::string      CommentCodeLine(const ARCode &code)
    {
        const char  *reg;
        std::string ret;

        static const char *_data[3] = {"data", "data#1", "data#2"};

        switch (code.Type)
        {
        case 0x00: ///< Write32
            ret = Utils::Format("[%08X + offs] = %08X", code.Left, code.Right);
            break;
        case 0x10: ///< Write16
            ret = Utils::Format("[%08X + offs] = %04X", code.Left, (code.Right & 0xFFFF));
            break;
        case 0x20: ///< Write8
            ret = Utils::Format("[%08X + offs] = %02X", code.Left, (code.Right & 0xFF));
            break;

        case 0x30: ///< GT 32Bits
            ret = GetCond32Str(code.Left, code.Right, ">");
            break;
        case 0x40: ///< LT 32Bits
            ret = GetCond32Str(code.Left, code.Right, "<");
            break;
        case 0x50: ///< EQ 32Bits
            ret = GetCond32Str(code.Left, code.Right, "==");
            break;
        case 0x60: ///< NE 32Bits
            ret = GetCond32Str(code.Left, code.Right, "!=");
            break;

        case 0x70: ///< GT 16Bits
            ret = GetCond16Str(code.Left, code.Right, ">");
            break;

        case 0x80: ///< LT 16Bits
            ret = GetCond16Str(code.Left, code.Right, "<");
            break;

        case 0x90: ///< EQ 16Bits
            ret = GetCond16Str(code.Left, code.Right, "==");
            break;
        case 0xA0: ///< NE 16Bits
            ret = GetCond16Str(code.Left, code.Right, "!=");
            break;

        case 0xB0: ///< Read and Set Offset
            ret = Utils::Format("offset = [%08X + offset]", code.Left);
            break;
        case 0xD3: ///< Set offset to immediate
            reg = code.Left & 1 ? "Offset#2" : "Offset#1";
            ret = Utils::Format("%s = %08X", reg, code.Right);
            break;
        case 0xDC: ///< Add to offset
            ret = Utils::Format("offset += %08X", code.Right);
            break;
        case 0xC0: ///< Loop
            ret = Utils::Format("Loop %X times:", code.Right);
            break;
        case 0xC1: ///< Loop
            ret = "loop [data#1] times";
            break;
        case 0xC2: ///< Loop
            ret = "loop [data#2] times";
            break;

        case 0xD2: ///< Full terminator
            if (code.Right & 1)
                ret = "exit code immediately";
            else
                ret = "end all if,start loop,clr reg";
            break;

        case 0xD1: ///< Loop execute
            ret = "start loop";
            break;
        case 0xD0: ///< Terminator
            if (code.Right == 0)
                ret = "end if";
            else
                ret = "exit loop";
            break;

        case 0xD4: ///< Add to register
            if (code.Left == 0)
                ret = Utils::Format("data += %08X", code.Right);
            else if (code.Left == 1)
                ret = Utils::Format("data#1 += data#2 + %08X", code.Right);
            else if (code.Left == 2)
                ret = Utils::Format("data#2 += data#1 + %08X", code.Right);
            break;
        case 0xD5: ///< Set register
            reg = _data[code.Left & 3];
            ret = Utils::Format("%s = %08X", reg, code.Right);
            break;
        case 0xD6: ///< Write32
            reg = _data[code.Left & 3];
            ret = Utils::Format("[%08X+off] = %s, off+=4", code.Right, reg);
            break;
        case 0xD7: ///< Write16
            reg = _data[code.Left & 3];
            ret = Utils::Format("[%08X+off] = %s, off+=2", code.Right, reg);
            break;
        case 0xD8: ///< Write8
            reg = _data[code.Left & 3];
            ret = Utils::Format("[%08X+off] = %s, off+=1", code.Right, reg);
            break;

        case 0xD9: ///< Set data 32
            reg = _data[code.Left & 3];
            ret = Utils::Format("%s = [%08X+offs]", reg, code.Right);
            break;
        case 0xDA: ///< Set data 16
            reg = _data[code.Left & 3];
            ret = Utils::Format("%s = [%08X+offs] & FFFF", reg, code.Right);
            break;
        case 0xDB: ///< Set data 8
            reg = _data[code.Left & 3];
            ret = Utils::Format("%s = [%08X+offs] & FF", reg, code.Right);
            break;

        case 0xE0: ///< E code
            if (code.Right)
                ret = Utils::Format("[%08X+offs] = patch data", code.Left);
            else
                ret = "!! error !!";
            break;

        case 0xDD: ///< Check input
            ret = "if ";
            ret += KeysToString(code.Right);
            ret += ":";
            break;

        case 0xDE: ///< Check touch:
        {
            if (code.Left == 0)
            {
                u32 minX = code.Right >> 16;
                u32 maxX = (code.Right << 16) >> 16;
                ret = Utils::Format("if %04X <= touch.X <= %04X:", minX, maxX);
            }
            else
            {
                u32 minY = code.Right >> 16;
                u32 maxY = (code.Right << 16) >> 16;
                ret = Utils::Format("if %04X <= touch.Y <= %04X:", minY, maxY);
            }
            break;
        }
        case 0xDF: ///< Registers:
        {
            // VFP Toggle
            if (code.Left == 0xFFFFFE)
            {
                bool conversion = (code.Right & 0x10) > 0;
                reg = (code.Right & 1) > 0 ? "enabled" : "disabled";
                ret = Utils::Format("data vfp state %s", reg);
                if (conversion)
                    ret += ", cvt";
                break;
            }
            // Condition toggle
            if (code.Left == 0xFFFFFF)
            {
                static const char * condstr[5] =
                {
                    "cond: immediate against value",
                    "cond. data against value",
                    "cond. immediate against data",
                    "cond. immediate against storage",
                    "cond. data against storage"
                };
                ret = condstr[code.Right & 7];
                break;
            }

            u32 operation = code.Right >> 16;
            u32 control = code.Right & 0xFFFF;

            static const char *offsets[2] = {"offset#1", "offset#2"};
            static const char *data[2] = {"data#1", "data#2"};
            static const char *storage[2] = {"storage#1", "storage#2"};

            const char *from = nullptr;
            const char *to = nullptr;
            switch (operation)
            {
            case 0: ///< Toggle active register
            {
                if (code.Left == 0) ///< change active offset
                    reg = offsets[control > 0];
                else if (code.Left == 1) ///< change active data
                    reg = data[control > 0];
                else if (code.Left == 2) ///< change active storage
                    reg = storage[control > 0];
                else break;
                ret = Utils::Format("Set %s as active", reg);
                break;
            }
            case 1: ///< Copy to other index
            {
                if (code.Left == 0)
                {
                    from = offsets[control > 0];
                    to = offsets[control == 0];
                }
                else if (code.Left == 1)
                {
                    from = data[control > 0];
                    to = data[control == 0];
                }
                else if (code.Left == 2)
                {
                    from = storage[control > 0];
                    to = data[control > 0];
                }

                break;
            }
            case 2: ///< Copy to other register
            {
                if (code.Left == 0)
                {
                    from = offsets[control > 0];
                    to = data[control > 0];
                }
                else if (code.Left == 1)
                {
                    from = data[control > 0];
                    to = offsets[control > 0];
                }
                else if (code.Left == 2)
                {
                    from = data[control > 0];
                    to = storage[control > 0];
                }

                break;
            }
            default:
                break;
            }
            if (to && from)
                ret = Utils::Format("%s = %s", to, from);
            break;
        }
        case 0xF0:
            if (code.Left == 1) ///< VFP Toggle
            {
                ret = (code.Right & 1) > 0 ? "Enable " : "Disable ";
                ret += "vfp mode for F1,F2,F3";
            }
            if (code.Left == 0x00F00000) ///< Custom ASM routine
            {
                ret = "ASM code";
            }
            break;

        case 0xF1: ///< Add to addr
            ret = Utils::Format("[%08X+offs] += %08X", code.Left, code.Right);
            break;
        case 0xF2: ///< Mul to addr
            ret = Utils::Format("[%08X+offs] *= %08X", code.Left, code.Right);
            break;
        case 0xF3: ///< Div to addr
            ret = Utils::Format("[%08X+offs] /= %08X", code.Left, code.Right);
            break;
        case 0xF4: /// Mul data
            ret = Utils::Format("data *= %08X", code.Right);
            break;
        case 0xF5: ///< Div data
            ret = Utils::Format("data /= %08X", code.Right);
            break;
        case 0xF6: ///< AND data
            ret = Utils::Format("data &= %08X", code.Right);
            break;
        case 0xF7: ///< OR data
            ret = Utils::Format("data |= %08X", code.Right);
            break;
        case 0xF8: ///< XOR data
            ret = Utils::Format("data ^= %08X", code.Right);
            break;
        case 0xF9: ///< NOT data
            ret = "data = ~data";
            break;
        case 0xFA: ///< LSL data
            ret = Utils::Format("data <<= %08X", code.Right);
            break;
        case 0xFB: ///< LSR data
            ret = Utils::Format("data >>= %08X", code.Right);
            break;
        case 0xFC: ///< Copy
            ret = Utils::Format("Copy %08X, off#2 => off#1", code.Right);
            break;
        case 0xFD: ///< Hook
            ret = "hook at offset";
            break;
        case 0xFE: ///< Memsearch
            ret = Utils::Format("if pattern in [off:%08X]:", code.Right);
            break;
        case 0xFF: ///< Random
            if (!code.Left && !code.Right)
                ret = "data = random number";
            else
                ret = Utils::Format("data=rand[%08X:%08X]", code.Left, code.Right);
            break;
        default:
            ret = "!! error !!";
            break;
        }

        if (ret.size() > 29)
            ret.erase(30);
        if (ret.empty())
            ret = "!! error !!";
        return (ret);
    }

    ARCodeEditor::CodeLine::CodeLine(ARCode &code) :
        base(code), parent(nullptr), flags(0), index(0)
    {
        if (code.Text.empty())
            code.Text = code.ToString();
        BSet(Modified);
    }

    ARCodeEditor::CodeLine::CodeLine(const CodeLine &right) :
        base(right.base)
    {
        parent = right.parent;
        flags = right.flags;
        index = right.index;
        BSet(Modified);
    }

    ARCodeEditor::CodeLine::CodeLine(CodeLine &&right) :
        base(right.base)
    {
        parent = right.parent;
        flags = right.flags;
        index = right.index;
        BSet(Modified);
    }

    #define IsInRange(val, min, max) (val >= min && val <= max)

    ARCodeEditor::CodeLine &ARCodeEditor::CodeLine::operator=(CodeLine &&right)
    {
        base = std::move(right.base);
        parent = right.parent;
        flags = right.flags;
        index = right.index;
        BSet(Modified);
        return *this;
    }

    void    ARCodeEditor::CodeLine::Edit(u32 cursor, u32 value)
    {
        if (IsData)
        {
            if (!parent)
                return;

            u32 &dst = cursor >= 8 ? parent->Data[index + 1] : parent->Data[index];

            if (cursor >= 8)
                cursor -= 9;

            u32 shift = (7 - cursor) * 4;
            value <<= shift;
            u32 mask = ~(0xF << shift);
            dst &= mask;
            dst |= value;
        }
        else
        {
            value &= 0xFF;
            if (IsInRange(value, 0, 9))
                value += '0';
            else if (IsInRange(value, 10, 15))
                value = (value - 10) + 'A';
            else
                return;

            // If we write the same char, do nothing
            if (base.Text[cursor] == value)
                return;

            std::string     currentCodeType = base.Text.substr(0, 2);
            std::string     nextCodeType = currentCodeType;

            if (cursor < 2)
                nextCodeType[cursor] = value;

            if (currentCodeType[0] == 'E') currentCodeType.pop_back();
            if (nextCodeType[0] == 'E') nextCodeType.pop_back();

            bool    ctDiffer = nextCodeType != currentCodeType;

            // Handle F0F00000 special case
            {
                static const char *asmcode = "F0F00000 00000000";

                if (ctDiffer && nextCodeType == "F0" && base.Left == 0x00F00000)
                {
                    base.Text = asmcode;
                    BSet(Modified);
                    return;
                }

                if (!ctDiffer && currentCodeType == "F0" && cursor < 8)
                {
                    std::string left = base.Text.substr(0, 8);

                    left[cursor] = value;

                    if (left == std::string(asmcode).substr(0, 8))
                    {
                        base.Text = asmcode;
                        BSet(Modified);
                        return;
                    }
                }

                if (std::string(asmcode).substr(0, 8) == base.Text.substr(0, 8))
                {
                    if (cursor > 8)
                        goto _changeDataSize;
                    else
                        goto _changeCodeType;
                }
            }

            // If the codetype change for E or FE or FD
            if (ctDiffer && (nextCodeType == "E" || nextCodeType == "FE" || nextCodeType == "FD"))
            {
                // Be sure that the whole line is empty
                base.Text = __emptyCode;

                // Set the code type
                base.Text[0] = nextCodeType[0];
                if (nextCodeType.size() > 1)
                    base.Text[1] = nextCodeType[1];
            }
            // If the code type changes from E or FE to something else, ask before pursue
            else if (ctDiffer && !base.Data.empty() && (currentCodeType == "E" || currentCodeType == "FE" || currentCodeType == "FD"))
            {
            _changeCodeType:
                if (!MessageBox(Color::Orange << "Warning", "You're about to delete a code with all it's data, continue ?", DialogType::DialogYesNo)())
                    return;

                // Be sure that the whole line is empty
                base.Text = __emptyCode;
                base.Data.clear();
                base.Update(base.Text);
                base.Text[0] = nextCodeType[0];
                if (nextCodeType.size() > 1)
                    base.Text[1] = nextCodeType[1];
                __arCodeEditor->_ReloadCodeLines();
                return;
            }
            // If what changes is the size to patch for E or FE code
            else if ((cursor >= 8 && (base.Type == 0xE0 || base.Type == 0xFD)) || (IsInRange(cursor, 2, 7) && base.Type == 0xFE))
            {
            _changeDataSize:
                u32             size = base.Type == 0xFE ? base.Left : base.Right;
                bool            error;
                int             currentLines = size / 8 + (size % 8 > 0 ? 1 : 0);
                int             newLines = 0;
                int             diff = 0;
                std::string     sizestr = Utils::Format("%08X", size);

                sizestr[(cursor > 8 ? cursor - 9 : cursor)] = value;

                newLines = ActionReplayPriv::Str2U32(sizestr, error);
                if (!error)
                    newLines = newLines / 8 + (newLines % 8 > 0 ? 1 : 0);
                else
                    newLines = 0;

                diff = currentLines - newLines;

                if (diff > 0)
                {
                    std::string body = Utils::Format("You're about to delete %d line(s) of data, continue ?", diff);
                    if (!MessageBox(Color::Orange << "Warning", body, DialogType::DialogYesNo)())
                        return;
                }

                base.Text[cursor] = value;
                if (!base.Update(base.Text))
                {
                    size = base.Type == 0xFE ? base.Left : base.Right;
                    if (size > 0)
                        base.Data.resize((size / 8 + ((size % 8) > 0 ? 1 : 0)) * 2);
                    else
                        base.Data.clear();
                    __arCodeEditor->_ReloadCodeLines();
                }
            }

            // Change value
            else
                base.Text[cursor] = value;
        }

        BSet(Modified);
    }

    void    ARCodeEditor::CodeLine::Update(void)
    {
        // If current line is E code data
        if (IsData && IsModified)
        {
            u32 left = parent->Data[index];
            u32 right = parent->Data[index + 1];

            display = ColorToString(PATCH_COLOR) + Utils::Format("%08X %08X", left, right);
            if (IsPattern)
                comment = "pattern data";
            else if (IsPatch)
                comment = "patch data";
            else
                comment = "asm data";

            BClear(Modified);
            return;
        }

        if (g_newCondDataToggle && IsInRange(base.Type, 0x30, 0xA0))
        {
            BSet(Modified);
        }

        #define IsCondModeToggle(code) (code.Type == 0xDF && code.Left == 0x00FFFFFF)

        if (IsModified)
        {
            bool    isCondModeToggle = IsCondModeToggle(base);
            bool    error = base.Update(base.Text);

            if (isCondModeToggle || IsCondModeToggle(base))
                g_newCondDataToggle = true;

            if (error)
            {
                display = Color::Red << base.Text;
                comment = "!! error !!";
            }
            else
            {
                display = ColorCodeLine(base);
                comment = CommentCodeLine(base);
            }

            BClear(Modified);
        }

        if (IsCondModeToggle(base))
        {
            g_condAgainstData = static_cast<CondMode>(base.Right & 7);
        }
    }

    static void ShowHelp(void)
    {
        std::string body = "Controls:\n" \
            "    - " FONT_B ": Exit editor (changes are applied directly)\n" \
            "    - " FONT_Y ": Delete current code\n" \
            "    - " FONT_L ": Insert a new code before current code\n" \
            "    - " FONT_R ": Insert a new code after current code\n";
        if (System::IsNew3DS())
            body += "    - " FONT_ZL ": Copy current code to clipboard\n" \
            "    - " FONT_ZR ": Clear clipboard\n";
        body += "    - \uE006: Navigate in the code";

        MessageBox(Color::LimeGreen << "Action Replay Code Editor Help",  body)();

        ScreenImpl::Top->Clear(true);
    }

    /*
     * Editor
     */
    ARCodeEditor::ARCodeEditor(void) :
        _submenu{ { "Copy to clipboard", "Clear clipboard", "Delete all codes", "Converter", "Hex Editor", "Help" } }
    {
        _exit = false;
        _index =  _line = 0;
        _keyboard.SetLayout(Layout::HEXADECIMAL);
        _keyboard._Hexadecimal();
        _keyboard._showCursor = false;
        __arCodeEditor = this;
        _clipboard = nullptr;
    }

    extern HexEditor *__g_hexEditor;

    bool    ARCodeEditor::operator()(EventList &eventList)
    {
        static bool isInHexEditor = false;

        if (isInHexEditor)
        {
            if (!__g_hexEditor)
                isInHexEditor = false;
            else
            {
                HexEditor &hexeditor = *__g_hexEditor;

                isInHexEditor = !hexeditor(eventList);
            }
            return false;
        }

        // Process event
        bool isSubMenuOpen = _submenu.IsOpen();
        for (Event &event : eventList)
        {
            _submenu.ProcessEvent(event);
            if (!isSubMenuOpen)
                _ProcessEvent(event);
        }

        int out; ///< Used for keyboard's input

        if (_submenu.IsOpen())
        {
            switch (_submenu())
            {
            case 0: ///< Copy to clipboard
            {
                if (_codes.empty() || !_context)
                    break;

                CodeLine &code = _codes[_line];

                // If we're in the middle of data, don't remove anything
                if (code.flags & (u32)(CodeLine::PatchData | CodeLine::PatternData))
                    break;

                if (_clipboard)
                    delete _clipboard;

                _clipboard = new ARCode(code.base);
                break;
            }
            case 1: ///< Clear clipboard
            {
                if (_clipboard)
                {
                    delete _clipboard;
                    _clipboard = nullptr;
                }
                break;
            }
            case 2: ///< Clear all codes
            {
                if (!(MessageBox(Color::Orange << "Warning", "Do you really want to delete all codes ?", DialogType::DialogYesNo)()))
                    break;

                _context->codes.clear();

                _ReloadCodeLines();

                break;
            }
            case 3: ///< Converter
                _converter();
                break;
            case 4: ///< Hex Editor
                if (static_cast<size_t>(_line) < _codes.size() && _codes[_line].base.Type == 0xD3)
                {
                    u32 address = _codes[_line].base.Right;
                    if (address && __g_hexEditor)
                        __g_hexEditor->Goto(address, true);
                }
                isInHexEditor = true;
                break;
            case 5: ///< Show help
                ShowHelp();
                break;
            default:
                break;
            }
        }
        else if (_keyboard(out) && !_codes.empty())
        {
            _codes[_line].Edit(_index, out);
            if (_index < 16)
                _index++;
            if (_index == 8) _index++;
        }

        // Update states
        _Update();

        // Draw top screen
        _RenderTop();

        // Draw bottom screen
        _RenderBottom();

        return _exit;
    }

    void    ARCodeEditor::Edit(ARCodeContext &ctx)
    {
        if (!__arCodeEditor)
            return;

        ARCodeEditor &editor = *__arCodeEditor;

        editor._context = &ctx;
        editor._exit = false;
        editor._index = editor._line = 0;
        editor._codes.clear();

        editor._ReloadCodeLines();

        Event           event;
        EventList       eventList;
        EventManager    manager(EventManager::EventGroups::GROUP_KEYS);
        bool            exit = false;

        do
        {
            eventList.clear();
            // Fetch all events
            while (manager.PollEvent(event))
                eventList.push_back(event);

            // Execute Editor's loop
            exit = editor(eventList);

            // Swap screens
            Renderer::EndFrame();

        } while (!exit);
    }

    void    ARCodeEditor::_ProcessEvent(Event &event)
    {
        if (event.type == Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case Key::B:
            {
                _exit = true;
                break;
            }
            // Insert Before
            case Key::L:
            {
                bool error;
                ARCodeVector &codes = _context->codes;

                // If code is empty, simply push something
                if (_codes.empty())
                {
                    _line = 0;
                    if (!_clipboard)
                        codes.push_back(ARCode(__emptyCode, error));
                    else
                        codes.push_back(ARCode(*_clipboard));
                    _ReloadCodeLines();
                    break;
                }

                CodeLine &code = _codes[_line];

                // If we're in the middle of data, don't remove anything
                if (code.flags & (u32)(CodeLine::PatchData | CodeLine::PatternData))
                    break;

                int line = code.index;

                if (!_clipboard)
                    codes.insert(codes.begin() + line, ARCode(__emptyCode, error));
                else
                    codes.insert(codes.begin() + line, ARCode(*_clipboard));
                _ReloadCodeLines();
                break;
            }
            // Insert After
            case Key::R:
            {
                bool error;
                ARCodeVector &codes = _context->codes;

                // If code is empty or we're at the end, simply push something
                if (_codes.empty())
                {
                    if (!_clipboard)
                        codes.push_back(ARCode(__emptyCode, error));
                    else
                        codes.push_back(ARCode(*_clipboard));
                    _ReloadCodeLines();
                    break;
                }

                CodeLine &code = _codes[_line];

                // If we're in the middle of data, don't remove anything
                if (code.flags & (u32)(CodeLine::PatchData | CodeLine::PatternData))
                    break;

                int line = code.index + 1;

                if (!_clipboard)
                    codes.insert(codes.begin() + line, ARCode(__emptyCode, error));
                else
                    codes.insert(codes.begin() + line, ARCode(*_clipboard));
                _ReloadCodeLines();
                break;
            }
            // Copy code to clipboard
            case Key::ZL:
            {
                if (_codes.empty() || !_context)
                    break;

                CodeLine &code = _codes[_line];

                // If we're in the middle of data, don't remove anything
                if (code.flags & (u32)(CodeLine::PatchData | CodeLine::PatternData))
                    break;

                if (_clipboard)
                    delete _clipboard;

                _clipboard = new ARCode(code.base);
                break;
            }
            // Clear clipboard
            case Key::ZR:
            {
                if (_clipboard)
                {
                    delete _clipboard;
                    _clipboard = nullptr;
                }
                break;
            }
            // Delete code
            case Key::Y:
            {
                if (static_cast<size_t>(_line) >= _codes.size()) break;

                CodeLine &code = _codes[_line];
                // If we're in the middle of data, don't remove anything
                if (code.flags & (u32)(CodeLine::PatchData | CodeLine::PatternData))
                    break;

                if (!(MessageBox(Color::Orange << "Warning", "Do you really want to delete this line ?", DialogType::DialogYesNo))())
                    break;

                _context->codes.erase(_context->codes.begin() + code.index);
                _ReloadCodeLines();
                break;
            }
            default:
                break;
            }///< End switch event.key.code
        }///< End if Event::KeyPressed

        if (event.type == Event::KeyDown && _inputClock.HasTimePassed(Milliseconds(150)))
        {
            switch (event.key.code)
            {
            case Key::DPadUp:
                _line = std::max((int)0, (int)_line - 1);
                break;

            case Key::DPadDown:
                _line = std::min(_line + 1, (int)(_codes.size() - 1));
                break;

            case Key::DPadLeft:
                _index = std::max(_index - 1, 0);
                if (_index == 8)
                    _index--;
                break;

            case Key::DPadRight:
                _index = std::min(_index + 1, 16);
                if (_index == 8)
                    _index++;
                break;
            default:
                break;
            }///< End switch event.key.code
            _inputClock.Restart();
        }///< End if Event::KeyHold
    }

    void    ARCodeEditor::_RenderTop(void)
    {
        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw("Code Editor");

        // Column headers

        // First column: 31px : 3*7 + 10 => Line
        // Second column: 112px : (17* 6) + 10 => Code
        // Last column: 185px : 175 + 10 => Comment
        // Total: 312px :  31 + 1 + 112 + 1 + 185
        // Margin left / right: 5px

        int   posY = 61;
        // Line header
        Renderer::DrawRect(35, posY, 31, 20, Color::SkyBlue);

        // Code header
        Renderer::DrawRect(67, posY, 112, 20, Color::DeepSkyBlue);

        // Comment header
        Renderer::DrawRect(180, posY, 185, 20, Color::DeepSkyBlue);

        posY += 21;

        // Line body
        Renderer::DrawRect(35, posY, 31, 112, Color::DeepSkyBlue);
        // Code body
        Renderer::DrawRect(67, posY, 112, 112, Color::White);
        // Comment body
        Renderer::DrawRect(180, posY, 185, 112, Color::SkyBlue);

        // If there's no code, exit here
        if (_codes.empty())
        {
            _submenu.Draw();
            return;
        }

        // Draw cursor
        Renderer::DrawRect(_cursorPosX, _cursorPosY, 7, 10, Color::SkyBlue);

        // Draw codes
        int posX = 72;
        int posXline = 40;
        int posXComment = 185;
        posY += 2;
        int posYline = posY;
        int posYComment = posY;
        {
            int i = std::max(0, _line - 10);
            int max = std::min(i + 11, (int)_codes.size());

            for (; i < max; ++i)
            {
                // Draw line
                Renderer::DrawString(Utils::Format("%3d", i + 1).c_str(), posXline, posYline, Color::Black);
                // Draw code
                Renderer::DrawString(_codes[i].display.c_str(), posX, posY, Color::Black);
                // Draw comment
                Renderer::DrawString(_codes[i].comment.c_str(), posXComment, posYComment, Color::DimGrey);
            }
        }

        const Color    &textcolor = Preferences::Settings.MainTextColor;
        posY = 203;
        Renderer::DrawString((char *)"Options:", 260, posY, textcolor);
        posY -= 14;
        Renderer::DrawSysString((char *)"\uE002", 320, posY, 380, textcolor);
        _submenu.Draw();
    }

    void    ARCodeEditor::_RenderBottom(void)
    {
        //Renderer::SetTarget(BOTTOM);
        _keyboard._RenderBottom();
    }

    void    ARCodeEditor::_Update(void)
    {
        if (_submenu.IsOpen())
            return;

        // Update cursor pos
        _cursorPosX = 71 + _index * 6;
        int start = std::max(_line - 10, 0);
        _cursorPosY = 83 + (_line - start) * 10;

        g_condAgainstData = CondMode::ImmAgainstVal;
        for (CodeLine &code : _codes)
            code.Update();
        g_newCondDataToggle = false;
    }

    void    ARCodeEditor::_ReloadCodeLines(void)
    {
        static std::vector<ARCode> tempar;

        _codes.clear();
        tempar.clear();

        ARCodeVector &arcodes = _context->codes;

        u16 index = 0;
        for (ARCode &code : arcodes)
        {
            _codes.push_back(CodeLine(code));
            _codes.back().index = index++;

            // If the code is E or FD, FE type, add all its data
            if (code.IsCodeWithData() && !code.Data.empty())
            {
                u32 flags = code.Type == 0xFE ? CodeLine::PatternData :
                            (code.Type == 0xF0 && code.Left == 0x00F00000 ? CodeLine::Asm : CodeLine::PatchData);


                for (size_t i = 0; i < code.Data.size() - 1; i += 2)
                {
                    bool error;
                    std::string data = Utils::Format("%08X %08X", code.Data[i], code.Data[i + 1]);
                    ARCode temp(data, error);
                    temp.Left = code.Data[i];
                    temp.Right = code.Data[i + 1];
                    tempar.push_back(temp);

                    _codes.push_back(CodeLine(tempar.back()));
                    _codes.back().parent = &code;
                    _codes.back().flags |= flags;
                    _codes.back().index = i;
                }
            }
        }

        while (static_cast<size_t>(_line) >= _codes.size() && _line > 0)
            _line--;
        if (_line < 0)
            _line = 0;
    }
}
