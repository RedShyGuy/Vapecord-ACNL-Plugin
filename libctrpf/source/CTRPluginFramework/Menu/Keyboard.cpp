#include "CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"

#include <string>
#include <climits>
#include <cstdlib>
#include "CTRPluginFramework/Utils/Utils.hpp"

namespace CTRPluginFramework
{
    /*
    ** Convert
    ************/

    bool stou32(std::string &input, u32 &res)
    {
        const char *str = input.c_str();
        bool neg = false;

        if (*str == '-')
        {
            str++;
            neg = true;
        }

        res = 0;

        while (*str)
        {
            int val = *str - '0';

            // Check that the mutiplication won't overflow
            if (res > UINT32_MAX / 10)
                goto overflow;

            res *= 10;

            // Check that the addition won't overflow
            if ((res > 0) && (static_cast<u32>(val) > UINT32_MAX - res))
                goto overflow;

            res += val;
            str++;
        }

        if (neg)
        {
            s32 sign = -static_cast<s32>(res);

            res = sign;
        }
        return (true);

    overflow:
        res = UINT32_MAX;
        input = "4294967295";
        return (false);
    }

    bool stou64(std::string &input, u64 &res)
    {
        const char *str = input.c_str();
        bool neg = false;

        if (*str == '-')
        {
            str++;
            neg = true;
        }

        res = 0;

        while (*str)
        {
            int val = *str - '0';

            // Check that the mutiplication won't overflow
            if (res > U64_MAX / 10)
                goto overflow;

            res *= 10;

            // Check that the addition won't overflow
            if ((res > 0) && (static_cast<u64>(val) > U64_MAX - res))
                goto overflow;

            res += val;
            str++;
        }

        if (neg)
        {
            s64 sign = -static_cast<s64>(res);

            res = sign;
        }

        return (true);

    overflow:
        res = U64_MAX;
        input = "18446744073709551615";
        return (false);
    }

    void *ConvertToU8(std::string &input, bool isHex)
    {
        static u8 temp = 0;

        if (input.length() > 0 && !(input.length() == 1 && input[0] == '-'))
        {
            if (!isHex)
            {
                u32 n;
                stou32(input, n);
                temp = static_cast<u8>(n);
            }
            else
            {
                const char *cstr = input.c_str();
                char *cstro = nullptr;
                temp = static_cast<u8>(strtoul(cstr, &cstro, 16));
                if (cstro == cstr)
                    temp = UINT8_MAX;
            }
        }
        else
            temp = 0;

        return (static_cast<void *>(&temp));
    }

    void *ConvertToU16(std::string &input, bool isHex)
    {
        static u16 temp = 0;

        if (input.length() > 0 && !(input.length() == 1 && input[0] == '-'))
        {
            if (!isHex)
            {
                u32 n;
                stou32(input, n);
                temp = static_cast<u16>(n);
            }
            else
            {
                const char *cstr = input.c_str();
                char *cstro = nullptr;
                temp = static_cast<u16>(strtoul(cstr, &cstro, 16));
                if (cstro == cstr)
                    temp = UINT16_MAX;
            }
        }
        else
            temp = 0;

        return (static_cast<void *>(&temp));
    }

    void *ConvertToU32(std::string &input, bool isHex)
    {
        static u32 temp = 0;

        if (input.length() > 0 && !(input.length() == 1 && input[0] == '-'))
        {
            if (!isHex)
            {
                stou32(input, temp);
            }
            else
            {
                const char *cstr = input.c_str();
                char *cstro = nullptr;
                temp = static_cast<u32>(strtoul(cstr, &cstro, 16));
                if (cstro == cstr)
                    temp = UINT32_MAX;
            }
        }
        else
            temp = 0;

        return (static_cast<void *>(&temp));
    }

    void *ConvertToU64(std::string &input, bool isHex)
    {
        static u64 temp = 0;

        if (input.length() > 0 && !(input.length() == 1 && input[0] == '-'))
        {
            if (!isHex)
            {
                stou64(input, temp);
            }
            else
            {
                const char *cstr = input.c_str();
                char *cstro = nullptr;
                temp = static_cast<u64>(strtoul(cstr, &cstro, 16));
                if (cstro == cstr)
                    temp = UINT64_MAX;
            }
        }
        else
            temp = 0;

        return (static_cast<void *>(&temp));
    }

    void *ConvertToFloat(std::string &input, bool isHex)
    {
        static float temp = 0;

        if (input.length() > 0 && !(input.length() == 1 && input[0] == '-'))
            temp = static_cast<float>(std::stof(input, nullptr));
        else
            temp = 0.f;
        return ((void *)&temp);
    }

    void *ConvertToDouble(std::string &input, bool isHex)
    {
        static double temp = 0;

        if (input.length() > 0 && !(input.length() == 1 && input[0] == '-'))
            temp = static_cast<double>(std::stod(input, nullptr));
        else
            temp = 0;
        return ((void *)&temp);
    }

    /*
    ** Compare
    ***********/

    Keyboard::Keyboard(const std::string &text) : _keyboard(new KeyboardImpl(this, text))
    {
        _hexadecimal = true;
        _isPopulated = false;
        DisplayTopScreen = !text.empty();
    }

    Keyboard::Keyboard(const std::string &text, const std::vector<std::string> &options) : _keyboard(new KeyboardImpl(this, text))
    {
        _hexadecimal = false;
        DisplayTopScreen = !text.empty();
        _keyboard->Populate(options, true);
        _isPopulated = !options.empty();
    }

    Keyboard::Keyboard(const std::vector<std::string> &options) : _keyboard(new KeyboardImpl(this))
    {
        _hexadecimal = false;
        DisplayTopScreen = false;
        _keyboard->Populate(options, true);
        _isPopulated = !options.empty();
    }

    Keyboard::~Keyboard(void)
    {
    }

    void Keyboard::IsHexadecimal(bool isHex)
    {
        _hexadecimal = isHex;
    }

    void Keyboard::SetMaxLength(u32 maxValue) const
    {
        _keyboard->SetMaxInput(maxValue);
    }

    void Keyboard::SetCompareCallback(CompareCallback callback) const
    {
        _keyboard->SetCompareCallback(callback);
    }

    void Keyboard::ChangeSelectedEntry(int entry)
    {
        _keyboard->ChangeSelectedEntry(entry);
    }

    int Keyboard::GetLastSelectedEntry()
    {
        return _keyboard->GetSelectedEntry();
    }

    void Keyboard::ChangeEntrySound(int entry, SoundEngine::Event soundEvent)
    {
        _keyboard->ChangeEntrySound(entry, soundEvent);
    }

    void Keyboard::Populate(const std::vector<std::string> &input, bool resetScroll)
    {
        _keyboard->Populate(input, resetScroll);
        _isPopulated = true;
    }

    void Keyboard::Populate(const std::vector<CustomIcon> &input, bool resetScroll)
    {
        _keyboard->Populate(input, resetScroll);
        _isPopulated = true;
    }

    int Keyboard::Open(void) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (!_isPopulated)
            return (-1);

        _keyboard->DisplayTopScreen = DisplayTopScreen;

        return (_keyboard->Run());
    }

    /*
    ** U8
    ******/

    int Keyboard::Open(u8 &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(2);
        }
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU8);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            std::string &input = _keyboard->GetInput();
            output = *(static_cast<u8 *>(ConvertToU8(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    int Keyboard::Open(u8 &output, u8 start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        std::string &input = _keyboard->GetInput();
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(2);
            input = Utils::Format("%X", start);
        }
        else
            input = Utils::Format("%d", start);
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU8);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = *(static_cast<u8 *>(ConvertToU8(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    /*
    ** U16
    ******/

    int Keyboard::Open(u16 &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(4);
        }
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU16);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            std::string &input = _keyboard->GetInput();
            output = *(static_cast<u16 *>(ConvertToU16(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    int Keyboard::Open(u16 &output, u16 start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        std::string &input = _keyboard->GetInput();
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(4);
            input = Utils::Format("%X", start);
        }
        else
            input = Utils::Format("%d", start);
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU16);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = *(static_cast<u16 *>(ConvertToU16(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    /*
    ** U32
    ******/

    int Keyboard::Open(u32 &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(8);
        }
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU32);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            std::string &input = _keyboard->GetInput();
            output = *(static_cast<u32 *>(ConvertToU32(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    int Keyboard::Open(u32 &output, u32 start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        std::string &input = _keyboard->GetInput();
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(8);
            input = Utils::Format("%X", start);
        }
        else
            input = Utils::Format("%d", start);
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU32);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = *(static_cast<u32 *>(ConvertToU32(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    /*
    ** U64
    ******/

    int Keyboard::Open(u64 &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(16);
        }
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU64);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            std::string &input = _keyboard->GetInput();
            output = *(static_cast<u64 *>(ConvertToU64(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    int Keyboard::Open(u64 &output, u64 start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(_hexadecimal ? Layout::HEXADECIMAL : Layout::DECIMAL);
        std::string &input = _keyboard->GetInput();
        if (_hexadecimal)
        {
            _keyboard->SetMaxInput(16);
            input = Utils::Format("%X", start);
        }
        else
            input = Utils::Format("%d", start);
        _keyboard->CanChangeLayout(true);
        _keyboard->SetConvertCallback(ConvertToU64);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = *(static_cast<u64 *>(ConvertToU64(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    /*
    ** float
    ******/

    int Keyboard::Open(float &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(DECIMAL);
        _keyboard->SetConvertCallback(ConvertToFloat);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            std::string &input = _keyboard->GetInput();
            output = *(static_cast<float *>(ConvertToFloat(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    int Keyboard::Open(float &output, float start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(DECIMAL);
        _keyboard->SetConvertCallback(ConvertToFloat);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        std::string &input = _keyboard->GetInput();
        input = Utils::ToString(start, 4);

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = *(static_cast<float *>(ConvertToFloat(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    /*
    ** double
    ******/

    int Keyboard::Open(double &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }
        _keyboard->SetLayout(DECIMAL);
        _keyboard->SetConvertCallback(ConvertToDouble);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            std::string &input = _keyboard->GetInput();
            output = *(static_cast<double *>(ConvertToDouble(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    int Keyboard::Open(double &output, double start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(DECIMAL);
        _keyboard->SetConvertCallback(ConvertToDouble);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        std::string &input = _keyboard->GetInput();
        input = Utils::Format("%.4lf", start);

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = *(static_cast<double *>(ConvertToDouble(input, _keyboard->IsHexadecimal())));
        }
        return (ret);
    }

    /*
    ** string
    **********/

    int Keyboard::Open(std::string &output) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(QWERTY);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = _keyboard->GetInput();
        }
        return (ret);
    }

    int Keyboard::Open(std::string &output, const std::string &start) const
    {
        if (SystemImpl::IsSleeping())
            return (-2);

        if (_isPopulated)
        {
            _keyboard->Clear();
            _isPopulated = false;
        }

        _keyboard->SetLayout(QWERTY);
        _keyboard->DisplayTopScreen = DisplayTopScreen;

        _keyboard->GetInput() = start;

        int ret = _keyboard->Run();

        if (ret >= 0)
        {
            output = _keyboard->GetInput();
        }
        return (ret);
    }

    std::string &Keyboard::GetInput(void) const
    {
        return (_keyboard->GetInput());
    }

    std::string &Keyboard::GetMessage(void) const
    {
        return (_keyboard->GetMessage());
    }

    void Keyboard::SetError(std::string error) const
    {
        _keyboard->SetError(error);
    }

    void Keyboard::CanAbort(bool canAbort) const
    {
        _keyboard->CanAbort(canAbort);
    }

    void Keyboard::Close(void) const
    {
        _keyboard->Close();
    }

    void Keyboard::OnKeyboardEvent(OnEventCallback callback) const
    {
        _keyboard->OnKeyboardEvent(callback);
    }
} // namespace CTRPluginFramework
