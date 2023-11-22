#include "types.h"

#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"

namespace CTRPluginFramework
{
    MenuEntryImpl::MenuEntryImpl(const std::string &name, const std::string &note, MenuEntry *owner) : MenuItem(MenuType::Entry)
    {
        this->name = name;
        this->firstName = name;
        this->note = note;
        this->GameFunc = nullptr;
        this->MenuFunc = nullptr;
        this->_arg = nullptr;
        this->_executeIndex = -1;
        this->_flags.state = false;
        this->_flags.justChanged = false;
        this->_flags.isRadio = false;
        this->_flags.isUnselectable = false;
        this->_radioId = -1;
        this->_owner = owner;
    }

    MenuEntryImpl::MenuEntryImpl(const std::string &name, FuncPointer func, const std::string &note, MenuEntry *owner) : MenuItem(MenuType::Entry)
    {
        this->name = name;
        this->firstName = name;
        this->note = note;
        this->GameFunc = func;
        this->MenuFunc = nullptr;
        this->_arg = nullptr;
        this->_executeIndex = -1;
        this->_flags.state = false;
        this->_flags.justChanged = false;
        this->_flags.isRadio = false;
        this->_flags.isUnselectable = false;
        this->_radioId = -1;
        this->_owner = owner;
    }

    MenuEntryImpl::~MenuEntryImpl()
    {
    }

    void    MenuEntryImpl::Enable(void)
    {
        if (IsActivated())
            return;

        // Change the state
        _TriggerState();

        // If the entry has a valid funcpointer
        if (GameFunc != nullptr)
        {
            PluginMenuExecuteLoop::Add(this);
        }
    }

    void    MenuEntryImpl::Disable(void)
    {
        _flags.state = 0;
        _flags.justChanged = 1;
    }

    void    MenuEntryImpl::SetRadio(int id)
    {
        _flags.isRadio = true;
        _radioId = id;
    }

    void    MenuEntryImpl::SetArg(void *arg)
    {
        _arg = arg;
    }

    void    *MenuEntryImpl::GetArg(void) const
    {
        return (_arg);
    }

    bool    MenuEntryImpl::WasJustActivated(void) const
    {
        return (_flags.state && _flags.justChanged);
    }

    bool    MenuEntryImpl::IsActivated(void) const
    {
        return (_flags.state);
    }

    MenuEntry *MenuEntryImpl::AsMenuEntry(void) const
    {
        return (_owner);
    }

    //##############################################################

    std::string &MenuEntryImpl::GetNote()
    {
        if (_owner == nullptr || _owner->Hotkeys.Count() == 0)
            return (note);

        if (Note2.empty() || HasNoteChanged())
            Note2 = note + "\n\n" + _owner->Hotkeys.ToString();
        return (Note2);
    }

    bool    MenuEntryImpl::_TriggerState(void)
    {
        // Disable if currently enabled
        if (_flags.state)
        {
            _flags.state = false;
            _flags.justChanged = false;
            return (false);
        }
        // Else enable
        _flags.state = true;
        _flags.justChanged = true;
        return (true);
    }

    bool    MenuEntryImpl::_MustBeRemoved(void) const
    {
        if (_flags.state)
            return (false);
        if (_flags.justChanged)
            return (false);
        return (true);
    }

    bool    MenuEntryImpl::_Execute(void)
    {
        EntryImplFlags fl = _flags;

        if (GameFunc != nullptr)
            GameFunc(_owner);

        if (_flags.state && _flags.justChanged)
            _flags.justChanged = false;
        else if (!_flags.state && fl.justChanged && !fl.state)
            _flags.justChanged = false;

        return (_MustBeRemoved());
    }
}
