#include "types.h"

#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"

namespace CTRPluginFramework
{
    static s8 _MenuCount = 0;

    MenuEntry::MenuEntry(const std::string &name, const std::string &note) :
        Hotkeys(this),
        _item(new MenuEntryImpl(name, note, this))
    {
    }

    MenuEntry::MenuEntry(const std::string &name, FuncPointer func, const std::string &note) :
        Hotkeys(this),
        _item(new MenuEntryImpl(name, func, note, this))
    {

    }

    MenuEntry::MenuEntry(const std::string &name, FuncPointer GameFunc, FuncPointer MenuFunc, const std::string &note) :
        Hotkeys(this),
        _item(new MenuEntryImpl(name, GameFunc, note, this))
    {
        _item->MenuFunc = MenuFunc;
        _item->cogID = _MenuCount++;
    }

    MenuEntry::MenuEntry(int radioId, const std::string &name, FuncPointer func, const std::string &note) :
        Hotkeys(this),
        _item(new MenuEntryImpl(name, func, note, this))
    {
        _item->SetRadio(radioId);
    }

    MenuEntry::MenuEntry(int radioGroup, const std::string &name, FuncPointer GameFunc, FuncPointer MenuFunc, const std::string &note) :
        Hotkeys(this),
        _item(new MenuEntryImpl(name, GameFunc, note, this))
    {
        _item->SetRadio(radioGroup);
        _item->MenuFunc = MenuFunc;
        _item->cogID = _MenuCount++;
    }

    MenuEntry::~MenuEntry()
    {
    }

    void    MenuEntry::Enable(void) const
    {
        _item->Enable();
    }

    void    MenuEntry::Disable(void) const
    {
        _item->Disable();
    }

    void    MenuEntry::Hide(void) const
    {
        _item->Hide();
    }

    void    MenuEntry::Show(void) const
    {
        _item->Show();
    }

    void    MenuEntry::SetRadio(int id) const
    {
        _item->SetRadio(id);
    }

    void    MenuEntry::SetArg(void *arg) const
    {
        _item->SetArg(arg);
    }

    void    *MenuEntry::GetArg(void) const
    {
        return (_item->GetArg());
    }

    bool    MenuEntry::WasJustActivated(void) const
    {
        return (_item->WasJustActivated());
    }

    bool    MenuEntry::IsActivated(void) const
    {
        return (_item->IsActivated());
    }

    bool    MenuEntry::IsVisible() const
    {
        return (_item->IsVisible());
    }

    void    MenuEntry::UseTopSeparator(Separator type) const
    {
        _item->Flags.useSeparatorBefore = type != Separator::None;
        _item->Flags.useStippledLineForBefore = type == Separator::Stippled;
    }

    void    MenuEntry::UseBottomSeparator(Separator type) const
    {
        _item->Flags.useSeparatorAfter = type != Separator::None;
        _item->Flags.useStippledLineForAfter = type == Separator::Stippled;
    }

    void    MenuEntry::CanBeSelected(bool canBeSelected) const
    {
        _item->_flags.isUnselectable = !canBeSelected;
        if (!canBeSelected && _item->IsActivated())
            _item->Disable();
    }

    void    MenuEntry::SetGameFunc(FuncPointer func) const
    {
        _item->GameFunc = func;
    }

    void    MenuEntry::SetMenuFunc(FuncPointer func) const
    {
        _item->MenuFunc = func;
    }

    FuncPointer MenuEntry::GetGameFunc() const
    {
        return _item->GameFunc;
    }

    FuncPointer MenuEntry::GetMenuFunc() const
    {
        return _item->MenuFunc;
    }

    void MenuEntry::RefreshNote() const
    {
        _item->NoteChanged();
    }

    std::string &MenuEntry::Name(void) const
    {
        return (_item->name);
    }

    std::string &MenuEntry::Note(void) const
    {
        return (_item->note);
    }

    Color   &MenuEntry::NameColor(void) const
    {
        return (_item->color);
    }

    s8      MenuEntry::CogID(void) const
    {
        return (_item->cogID);
    }
}
