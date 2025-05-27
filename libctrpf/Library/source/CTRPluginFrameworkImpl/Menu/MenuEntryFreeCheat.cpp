
#include "CTRPluginFrameworkImpl/Menu/MenuEntryFreeCheat.hpp"

#if FREECHEATS

#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuFreeCheats.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"

#include <cstring>

namespace CTRPluginFramework
{
    static void Write8(MenuEntryFreeCheat *entry)
    {
        Process::Write8(entry->Address, entry->Value.Bits8);
    }

    static void Write16(MenuEntryFreeCheat *entry)
    {
        Process::Write16(entry->Address, entry->Value.Bits16);
    }

    static void Write32(MenuEntryFreeCheat *entry)
    {
        Process::Write32(entry->Address, entry->Value.Bits32);
    }
    /*
    static void Write64(MenuEntryFreeCheat *entry)
    {
        Process::Write64(entry->Address, entry->Value.Bits64);
    }*/

    static void WriteFloat(MenuEntryFreeCheat *entry)
    {
        Process::WriteFloat(entry->Address, entry->Value.Float);
    }
    /*
    static void WriteDouble(MenuEntryFreeCheat *entry)
    {
        Process::WriteDouble(entry->Address, entry->Value.Double);
    }*/

    MenuEntryFreeCheat::MenuEntryFreeCheat(const std::string& text, u32 addr, u8 value) :
        MenuEntryImpl(text),
        Func(Write8),
        Address(addr)
    {
        _type = MenuType::FreeCheat;
        Type = Type_e::Bits8;
        Value.Bits32 = value;
    }

    MenuEntryFreeCheat::MenuEntryFreeCheat(const std::string& text, u32 addr, u16 value) :
        MenuEntryImpl(text),
        Func(Write16),
        Address(addr)
    {
        _type = MenuType::FreeCheat;
        Type = Type_e::Bits16;
        Value.Bits32 = value;
    }

    MenuEntryFreeCheat::MenuEntryFreeCheat(const std::string& text, u32 addr, u32 value) :
        MenuEntryImpl(text),
        Func(Write32),
        Address(addr)
    {
        _type = MenuType::FreeCheat;
        Type = Type_e::Bits32;
        Value.Bits32 = value;
    }

    /*MenuEntryFreeCheat::MenuEntryFreeCheat(const std::string& text, u32 addr, u64 value) :
        MenuEntryImpl(text),
        Func(Write64),
        Address(addr)
    {
        _type = MenuType::FreeCheat;
        Type = Type_e::Bits64;
        Value.Bits64 = value;
    }*/

    MenuEntryFreeCheat::MenuEntryFreeCheat(const std::string& text, u32 addr, float value) :
        MenuEntryImpl(text),
        Func(WriteFloat),
        Address(addr)
    {
        _type = MenuType::FreeCheat;
        Type = Type_e::Bits32;
        Value.Float = value;
    }

    /*MenuEntryFreeCheat::MenuEntryFreeCheat(const std::string& text, u32 addr, double value) :
        MenuEntryImpl(text),
        Func(WriteDouble),
        Address(addr)
    {
        _type = MenuType::FreeCheat;
        Type = Type_e::Double;
        Value.Double = value;
    }*/

    MenuEntryFreeCheat::MenuEntryFreeCheat(const MenuEntryFreeCheat& freeCheat) :
        MenuEntryImpl(freeCheat.name),
        Func(freeCheat.Func),
        Address(freeCheat.Address)
    {
        _type = MenuType::FreeCheat;
        Type = freeCheat.Type;
        Value.Bits32 = freeCheat.Value.Bits32;
    }

    MenuEntryFreeCheat::MenuEntryFreeCheat(const Preferences::SavedCheats& savedCheats) :
        MenuEntryImpl(savedCheats.name),
        Address(savedCheats.address)
    {
        _type = MenuType::FreeCheat;
        Type = (Type_e)(savedCheats.flags & 0xFF);
        Value.Bits32 = savedCheats.value;

        if (savedCheats.flags >> 8 && Preferences::AutoLoadCheats)
            Enable();

        Func = nullptr;

        if (Type == Type_e::Bits8) Func = Write8;
        if (Type == Type_e::Bits16) Func = Write16;
        if (Type == Type_e::Bits32) Func = Write32;
        //if (Type == Type_e::Bits64) Func = Write64;
        if (Type == Type_e::Float) Func = WriteFloat;
        //if (Type == Type_e::Double) Func = WriteDouble;
    }

    bool    MenuEntryFreeCheat::TriggerState(void)
    {
        return (_TriggerState());
    }

    void    MenuEntryFreeCheat::Enable(void)
    {
        if (IsActivated())
            return;

        _TriggerState();
        PluginMenuExecuteLoop::Add(this);
    }

    void    MenuEntryFreeCheat::Disable(void)
    {
        if (!IsActivated())
            return;

        _TriggerState();
        PluginMenuExecuteLoop::Remove(this);
    }

    void    MenuEntryFreeCheat::SetType(Type_e type)
    {
        Type = type;
        if (Type == Type_e::Bits8) Func = Write8;
        if (Type == Type_e::Bits16) Func = Write16;
        if (Type == Type_e::Bits32) Func = Write32;
      //  if (Type == Type_e::Bits64) Func = Write64;
        if (Type == Type_e::Float) Func = WriteFloat;
      //  if (Type == Type_e::Double) Func = WriteDouble;
    }

    void    MenuEntryFreeCheat::ToSavedSearch(Preferences::SavedCheats &savedCheats)
    {
        savedCheats.flags = ((u8)Type) | (IsActivated() << 8);
        savedCheats.address = Address;
        savedCheats.value = Value.Bits32;

        std::memset(savedCheats.name, 0, 50);
        std::strncpy(savedCheats.name, name.c_str(), 49);
    }
}

#endif
