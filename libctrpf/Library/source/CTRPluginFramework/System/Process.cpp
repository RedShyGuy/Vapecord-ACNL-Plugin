#include <3ds.h>
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFrameworkImpl/Graphics/OSDImpl.hpp"

#include "csvc.h"

extern Handle gspThreadEventHandle;

namespace CTRPluginFramework
{
    Process::ExceptionCallback Process::exceptionCallback = nullptr;
    Process::OnPauseResumeCallback Process::OnPauseResume = nullptr;
    bool Process::ThrowOldExceptionOnCallbackException = false;

    Handle Process::GetHandle(void)
    {
        return ProcessImpl::ProcessHandle;
    }

    u32 Process::GetProcessID(void)
    {
        return ProcessImpl::ProcessId;
    }

    u64 Process::GetTitleID(void)
    {
        return ProcessImpl::TitleId;
    }

    void Process::GetTitleID(std::string &output)
    {
        char tid[17] = {0};

        sprintf(tid, "%016llX", ProcessImpl::TitleId);

        for (int i = 0; i < 16;)
            output += tid[i++];
    }

    void Process::GetName(std::string &output)
    {
        for (int i = 0; i < 8;)
        {
            char c = ProcessImpl::CodeSet.processName[i++];
            if (c)
                output += c;
        }
    }

    u16 Process::GetVersion(void)
    {
        AM_TitleEntry entry = {0};
        AM_TitleEntry entryUpd = {0};
        AM_TitleEntry entryCard = {0};

        u64 tid = Process::GetTitleID();
        u64 tidupdate = tid | 0x000000E00000000;

        AM_GetTitleInfo(MEDIATYPE_SD, 1, &tid, &entry);
        AM_GetTitleInfo(MEDIATYPE_SD, 1, &tidupdate, &entryUpd);

        if (R_SUCCEEDED(AM_GetTitleInfo(MEDIATYPE_GAME_CARD, 1, &tid, &entryCard)))
            return (std::max(entryUpd.version, entryCard.version));

        return std::max(entryUpd.version, entry.version);
    }

    u32 Process::GetTextSize(void)
    {
        return ProcessImpl::CodeSet.textPages * 0x1000;
    }

    u32 Process::GetRoDataSize(void)
    {
        return ProcessImpl::CodeSet.rodataPages * 0x1000;
    }

    u32 Process::GetRwDataSize(void)
    {
        return ProcessImpl::CodeSet.rwPages * 0x1000;
    }

    bool Process::IsPaused(void)
    {
        return ProcessImpl::IsPaused > 0;
    }

    void Process::Pause()
    {
        ProcessImpl::Pause(false);
    }

    void Process::Play(const u32 frames)
    {
        if (frames)
        {
            OSDImpl::ResumeFrame(frames);
        }
        else
            ProcessImpl::Play(false);
    }

    void    Process::WaitForExit()
    {
        ProcessImpl::WaitForExit();
    }

    bool Process::Patch(u32 addr, void *patch, u32 length, void *original)
    {
        return ProcessImpl::PatchProcess(addr, static_cast<u8 *>(patch), length, static_cast<u8 *>(original));
    }

    bool Process::Patch(u32 addr, u32 patch, void *original)
    {
        return ProcessImpl::PatchProcess(addr, reinterpret_cast<u8 *>(&patch), 4, static_cast<u8 *>(original));
    }

    bool Process::ProtectMemory(u32 addr, u32 size, int perm)
    {
        if (addr & 0xFFF)
        {
            addr &= ~0xFFF;
            size += 0x1000;
            size &= ~0xFFF;
        }

        //if (R_FAILED(svcControlProcessMemory(ProcessImpl::ProcessHandle, addr, addr, size, 6, perm)))
        //	return false;
        svcControlProcess(ProcessImpl::ProcessHandle, PROCESSOP_SET_MMU_TO_RWX, 0, 0);
        return true;
    }

    bool Process::ProtectRegion(u32 addr, int perm)
    {
        MemInfo minfo;
        PageInfo pinfo;

        if (R_FAILED(svcQueryProcessMemory(&minfo, &pinfo, ProcessImpl::ProcessHandle, addr)))
            goto error;
        if (minfo.state == MEMSTATE_FREE)
            goto error;
        if (addr < minfo.base_addr || addr > minfo.base_addr + minfo.size)
            goto error;

        return ProtectMemory(minfo.base_addr, minfo.size, perm);
    error:
        return false;
    }

    void Process::ProtectRegionInRange(u32 startAddress, u32 endAddress, int perm)
    {
        // This should be removed in a future update
    }

    bool Process::CopyMemory(void *dst, const void *src, u32 size)
    {
        u32 src32 = (u32)src;
        u32 dst32 = (u32)dst;

        if (!CheckAddress(src32))
            goto error;
        if (!CheckAddress(dst32))
            goto error;
        if (!CheckAddress(dst32 + size))
            goto error;
        if (!CheckAddress(src32 + size))
            goto error;

        svcFlushProcessDataCache(ProcessImpl::ProcessHandle, src32, size);
        svcInvalidateProcessDataCache(ProcessImpl::ProcessHandle, dst32, size);

        std::memcpy(dst, src, size);

        svcFlushProcessDataCache(ProcessImpl::ProcessHandle, dst32, size);

        return true;
    error:
        return false;
    }

    bool Process::CheckAddress(u32 address, u32 perm)
    {
        return ProcessImpl::IsValidAddress(address);
    }

    bool Process::CheckRegion(u32 address, u32 &size, u32 perm)
    {
        //Result         res;
        //PageInfo       pInfo = { 0 };
        MemInfo mInfo = ProcessImpl::GetMemRegion(address);

        size = mInfo.size;
        return mInfo != ProcessImpl::InvalidRegion;
    }

    u32  Process::GetFreeMemoryRegion(u32 size, u32 startAddress)
    {
        return ProcessImpl::GetFreeMemRegion(size, startAddress);
    }

    bool Process::Write64(u32 address, u64 value)
    {
        if (CheckAddress(address, MEMPERM_WRITE))
        {
            *reinterpret_cast<u64 *>(address) = value;
            return (true);
        }
        return (false);
    }

    bool Process::Write32(u32 address, u32 value)
    {
        if (CheckAddress(address, MEMPERM_WRITE))
        {
            *reinterpret_cast<u32 *>(address) = value;
            return (true);
        }
        return (false);
    }

    bool Process::Write16(u32 address, u16 value)
    {
        if (CheckAddress(address, MEMPERM_WRITE))
        {
            *reinterpret_cast<u16 *>(address) = value;
            return (true);
        }
        return (false);
    }

    bool Process::Write8(u32 address, u8 value)
    {
        if (CheckAddress(address, MEMPERM_WRITE))
        {
            *reinterpret_cast<u8 *>(address) = value;
            return (true);
        }
        return (false);
    }

    bool Process::WriteFloat(u32 address, float value)
    {
        if (CheckAddress(address, MEMPERM_WRITE))
        {
            *reinterpret_cast<float *>(address) = value;
            return (true);
        }
        return (false);
    }

    bool Process::WriteDouble(u32 address, double value)
    {
        if (CheckAddress(address, MEMPERM_WRITE))
        {
            *reinterpret_cast<double *>(address) = value;
            return (true);
        }
        return (false);
    }

    bool Process::Read64(u32 address, u64 &value)
    {
        if (CheckAddress(address, MEMPERM_READ))
        {
            value = *reinterpret_cast<u64 *>(address);
            return (true);
        }
        return (false);
    }

    bool Process::Read32(u32 address, u32 &value)
    {
        if (CheckAddress(address, MEMPERM_READ))
        {
            value = *reinterpret_cast<vu32 *>(address);
            return (true);
        }
        return (false);
    }

    bool Process::Read16(u32 address, u16 &value)
    {
        if (CheckAddress(address, MEMPERM_READ))
        {
            value = *reinterpret_cast<vu16 *>(address);
            return (true);
        }
        return (false);
    }

    bool Process::Read8(u32 address, u8 &value)
    {
        if (CheckAddress(address, MEMPERM_READ))
        {
            value = *reinterpret_cast<vu8 *>(address);
            return (true);
        }
        return (false);
    }

    bool Process::ReadFloat(u32 address, float &value)
    {
        if (CheckAddress(address, MEMPERM_READ))
        {
            value = *reinterpret_cast<volatile float *>(address);
            return (true);
        }
        return (false);
    }

    bool Process::ReadDouble(u32 address, double &value)
    {
        if (CheckAddress(address, MEMPERM_READ))
        {
            value = *reinterpret_cast<volatile double *>(address);
            return (true);
        }
        return (false);
    }

    static bool ConvertString(void *output, const u8 *input, u32 size, StringFormat outfmt)
    {
        if (outfmt == StringFormat::Utf16)
        {
            u16 buffer[0x10];
            u16 *out = reinterpret_cast<u16 *>(output);
            u16 *buf;
            u32 code;
            int units;

            size >>= 1;
            size <<= 1;
            do
            {
                buf = buffer;
                units = decode_utf8(&code, input);
                if (units == -1)
                    return (false);

                input += units;
                units = encode_utf16(buf, code);
                size -= units;
                if (!size)
                    *out = 0;
                else
                    while (units--)
                        *out++ = *buf++;
            } while (size && code > 0);
        }
        else
        {
            u32 *out = reinterpret_cast<u32 *>(output);
            u32 code;
            int units;

            size >>= 2;
            size <<= 2;
            do
            {
                units = decode_utf8(&code, input);
                if (units == -1)
                    return (false);

                input += units;
                size -= 4;
                if (!size)
                    *out = 0;
                else
                    while (units--)
                        *out++ = code;

            } while (size && code > 0);
        }

        return (true);
    }

    bool Process::ReadString(u32 address, std::string &output, u32 size, StringFormat format)
    {
        if (!CheckAddress(address, MEMPERM_READ))
            return (false);

        u8 buffer[0x10];

        if (format == StringFormat::Utf8)
        {
            u32 code = 0;
            u8 *p = reinterpret_cast<u8 *>(address);
            int unit = 0;

            do
            {
                unit = decode_utf8(&code, p);

                if (unit == -1)
                    return (false);

                size -= unit;

                if (code > 0)
                    while (unit--)
                        output += *p++;
            } while (code > 0 && size);
        }
        else if (format == StringFormat::Utf16)
        {
            u32 code = 0;
            u16 *p = reinterpret_cast<u16 *>(address);
            u8 *buf;
            int unit = 0;

            size >>= 1;
            size <<= 1;
            do
            {
                buf = buffer;

                unit = decode_utf16(&code, p);
                if (unit == -1)
                    return (false);

                p += unit;
                size -= unit * 2;
                unit = encode_utf8(buf, code);
                if (unit == -1)
                    return (false);

                if (code > 0)
                    while (unit--)
                        output += *buf++;
            } while (code > 0 && size);
        }
        else
        {
            u32 code = 0;
            u32 *p = reinterpret_cast<u32 *>(address);
            u8 *buf;
            int unit = 0;

            size >>= 2;
            size <<= 2;
            do
            {
                buf = buffer;
                code = *p++;
                size -= 4;

                unit = encode_utf8(buf, code);
                if (unit == -1)
                    return (false);

                if (code > 0)
                    while (unit--)
                        output += *buf++;
            } while (code > 0 && size);
        }

        return (true);
    }

    bool Process::WriteString(u32 address, const std::string &input, StringFormat outFmt)
    {
        if (!CheckAddress(address, MEMPERM_READ | MEMPERM_WRITE) || input.empty())
            return (false);

        if (outFmt == StringFormat::Utf8)
        {
            u8 *p = reinterpret_cast<u8 *>(address);

            for (char c : input)
            {
                *p++ = c;
            }
            return (true);
        }
        else if (outFmt == StringFormat::Utf16)
        {
            u32 size = (input.size() + 1) * 2;
            const u8 *in = reinterpret_cast<const u8 *>(input.c_str());
            u16 *out = reinterpret_cast<u16 *>(address);

            return (ConvertString(out, in, size, outFmt));
        }

        {
            u32 size = (input.size() + 1) * 4;
            const u8 *in = reinterpret_cast<const u8 *>(input.c_str());
            u32 *out = reinterpret_cast<u32 *>(address);

            return (ConvertString(out, in, size, outFmt));
        }
    }

    bool Process::WriteString(u32 address, const std::string &input, u32 size, StringFormat outFmt)
    {
        if (!CheckAddress(address, MEMPERM_READ | MEMPERM_WRITE) || input.empty())
            return (false);

        if (outFmt == StringFormat::Utf8)
        {
            u8 *p = reinterpret_cast<u8 *>(address);

            for (char c : input)
            {
                size--;
                if (!size)
                {
                    *p = 0;
                    break;
                }
                *p++ = c;
            }
            return (true);
        }
        else if (outFmt == StringFormat::Utf16)
        {
            const u8 *in = reinterpret_cast<const u8 *>(input.c_str());
            u16 *out = reinterpret_cast<u16 *>(address);

            return (ConvertString(out, in, size, outFmt));
        }

        {
            const u8 *in = reinterpret_cast<const u8 *>(input.c_str());
            u32 *out = reinterpret_cast<u32 *>(address);

            return (ConvertString(out, in, size, outFmt));
        }
    }

    void Process::ReturnToHomeMenu(void)
    {
        APT_PrepareToCloseApplication(true);
        APT_CloseApplication(NULL, 0, 0);
        ProcessImpl::UnlockGameThreads();
        svcExitProcess();
        for (;;)
            ;
    }

    u32 Process::GetMainThreadTLS()
    {
        return ProcessImpl::MainThreadTls;
    }

    void Process::SetProcessEventCallback(ProcessEventCallback callback) {
        ProcessImpl::processEventCallback = callback;
    }
} // namespace CTRPluginFramework
