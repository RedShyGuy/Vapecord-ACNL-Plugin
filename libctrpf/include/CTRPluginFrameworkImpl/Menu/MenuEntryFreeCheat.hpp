#ifndef CTRPLUGINFRAMEWORKIMPL_MENUENTRYFREECHEAT_HPP
#define CTRPLUGINFRAMEWORKIMPL_MENUENTRYFREECHEAT_HPP

#define FREECHEATS 0

#if FREECHEATS

#include "MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#include <string>


namespace CTRPluginFramework
{
    enum class Type_e
    {
        Bits8 = 1,
        Bits16 = 1 << 1,
        Bits32 = 1 << 2,
       // Bits64 = 1 << 3,
        Float = 1 << 4,
       // Double = 1 << 5
    };

    class MenuEntryFreeCheat : public MenuEntryImpl
    {
        using FuncPointer = void (*)(MenuEntryFreeCheat *);
    public:
        MenuEntryFreeCheat(const std::string &text, u32 addr, u8 value);
        MenuEntryFreeCheat(const std::string &text, u32 addr, u16 value);
        MenuEntryFreeCheat(const std::string &text, u32 addr, u32 value);
       // MenuEntryFreeCheat(const std::string &text, u32 addr, u64 value);
        MenuEntryFreeCheat(const std::string &text, u32 addr, float value);
       // MenuEntryFreeCheat(const std::string &text, u32 addr, double value);
        MenuEntryFreeCheat(const MenuEntryFreeCheat &freeCheat);
        explicit MenuEntryFreeCheat(const Preferences::SavedCheats &savedCheats);
        ~MenuEntryFreeCheat() {}

        bool    TriggerState(void);
        void    Enable(void) override;
        void    Disable(void) override;
        void    SetType(Type_e type);
        void    ToSavedSearch(Preferences::SavedCheats &savedCheats);

        FuncPointer     Func;
        u32             Address;
        Type_e          Type;

        union
        {
            u8      Bits8;
            u16     Bits16;
            u32     Bits32;
           // u64     Bits64;
            float   Float;
           // double  Double;
        }               Value;
    };
}

#endif
#endif
