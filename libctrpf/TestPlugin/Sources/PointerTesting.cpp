#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework
{
    template <typename T>
    T   *GetArg(MenuEntry *entry, T defaultValue)
    {
        T   *arg = reinterpret_cast<T *>(entry->GetArg());

        if (arg == nullptr)
        {
            arg = new T(defaultValue);
            entry->SetArg(arg);
        }

        return (arg);
    }

    enum
    {
        U8,
        U16,
        U32,
        Float
    };

    u32         g_mode = U32;

    void    UpdateEntries(void);

    MenuFolder  *g_pointerFolder = new MenuFolder("Pointers testing", "",
    {
        new MenuEntry("Change mode", nullptr, [](MenuEntry *entry)
        {
            Keyboard kb(std::vector<std::string>({ "u8 ", "u16", "u32", "Float" }));

            kb.CanAbort(false);
            g_mode = kb.Open();
            UpdateEntries();
        })
    });

    std::string FormatAddress(u32 addr, u32 off)
    {
        std::string address = Utils::Format("%08X", addr == 0 ? off : addr);
        std::string offset = Utils::Format("%X", off);

        if (addr == 0)
        {
            return ("[" << Color::Yellow << address << ResetColor() << "] = ");
        }

        return ("[" << Color::Yellow << address << ResetColor()
            << " + " << Color::Orange << offset << ResetColor() << "] = ");
    }

    std::string FormatValue(u32 &address)
    {
        union
        {
            u32 U32;
            float Float;
            u8 U8;
            u16 U16;
        }val = { 0 };

        std::string ret;

        if (!Process::Read32(address, address))
            address = 0;

        val.U32 = address;

        switch (g_mode)
        {
        case U8:
            ret = Color::SkyBlue << Utils::Format("%08X", address)
                << ResetColor() << " => "
                << Color::Green << Utils::Format("%02X", val.U8);
            break;
        case U16:
            ret = Color::SkyBlue << Utils::Format("%08X", address)
                << ResetColor() << " => "
                << Color::Green << Utils::Format("%04X", val.U16);
            break;
        case U32:
            ret = Color::Green << Utils::Format("%08X", val.U32);
            break;
        case Float:
            ret = Color::SkyBlue << Utils::Format("%08X", address)
                << ResetColor() << " => "
                << Color::Green << Utils::Format("%.02f", val.Float);
            break;
        }

        return (ret);
    }

    void    UpdateEntries(void)
    {
        std::vector<MenuEntry *> &&list = g_pointerFolder->GetEntryList();

        u32 address = 0;

        for (int i = 1; i < 11; i++)
        {
            MenuEntry *entry = list[i];
            std::string &name = entry->Name();

            u32 val = *GetArg<u32>(entry, 0);

            name = FormatAddress(address, val);

            address += val;

            name += FormatValue(address);
        }
    }

    void    ChangeValue(MenuEntry *entry)
    {
        u32 *val = GetArg<u32>(entry, 0);

        Keyboard kb;

        kb.CanAbort(false);
        kb.Open(*val);
        UpdateEntries();
    }

    void    InitPointerChecker(PluginMenu &menu)
    {
        menu += g_pointerFolder;

        for (int i = 0; i < 10; i++)
            *g_pointerFolder += new MenuEntry("", nullptr, ChangeValue);

        menu += []
        {
            static Clock timer;

            if (timer.HasTimePassed(Seconds(1.f)))
            {
                UpdateEntries();
                timer.Restart();
            }
        };

        UpdateEntries();
    }
}