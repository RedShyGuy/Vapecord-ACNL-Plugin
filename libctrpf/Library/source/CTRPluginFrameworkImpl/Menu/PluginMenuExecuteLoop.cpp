#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryFreeCheat.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/MenuEntryActionReplay.hpp"

#include <queue>
#include "CTRPluginFramework/System/Lock.hpp"

namespace CTRPluginFramework
{
    using ExecuteIterator = std::vector<MenuEntryImpl *>::iterator;

    PluginMenuExecuteLoop *PluginMenuExecuteLoop::_firstInstance = nullptr;


    PluginMenuExecuteLoop::PluginMenuExecuteLoop(void)
    {
        _firstInstance = this;
    }

    void PluginMenuExecuteLoop::WriteEnabledCheatsToFile(Preferences::Header& header, File& file)
    {
        if (_firstInstance == nullptr)
            return;

        Lock    lock(_firstInstance->_mutex);

        std::vector<u32>    uids;
        std::vector<MenuEntryImpl *>  &items = _firstInstance->_builtinEnabledList;
        u32     count = items.size();
        u32     index = 0;
        u32     written = 0;
        u64     offset = file.Tell();

        while (count)
        {
            uids.clear();
            u32 nb = count > 1000 ? 1000 : count;

            for (; index < items.size(); ++index)
            {
                MenuEntryImpl *e = items[index];

                if (e != nullptr && e->IsEntry() && e->IsActivated())
                    uids.push_back(e->Uid);
            }

            if (file.Write(uids.data(), sizeof(u32) * uids.size()) != 0)
                goto error;

            written += uids.size();
            count -= nb;
        }
        header.enabledCheatsCount = written;
        header.enabledCheatsOffset = offset;

        error:
            return;
    }

    static int g_radioId; ///< Global for lambda access
    void    PluginMenuExecuteLoop::Add(MenuEntryImpl *entry)
    {
        if (_firstInstance == nullptr || entry == nullptr)
            return;

        Lock    lock(_firstInstance->_mutex);

        std::vector<MenuEntryImpl*> &vector = _firstInstance->_builtinEnabledList;

        int     id = entry->_radioId;

        // If entry already in the vector, abort
        for (MenuEntryImpl *e : vector)
            if (e == entry)
                return;

        // If it's a radio entry remove any entry with the same radio id
        if (entry->_flags.isRadio && id != -1)
        {
            g_radioId = id;
            vector.erase(std::remove_if(vector.begin(), vector.end(),
                [](MenuEntryImpl *e)
                {
                    if (e->_flags.state && e->_flags.isRadio && e->_radioId == g_radioId)
                    {
                        // If the entry was just added to the list
                        if (e->_flags.justChanged)
                        {
                            e->_flags.state = false;
                            e->_flags.justChanged = false;
                            return true;
                        }
                        else
                            e->_TriggerState(); ///< Disable and shedule it for future remove
                    }
                    return false;
                }), vector.end());
        }

        // Add the entry to the list
        vector.push_back(entry);
    }


    void    PluginMenuExecuteLoop::Remove(MenuEntryImpl *entry)
    {
        if (_firstInstance == nullptr || entry == nullptr)
            return;

        Lock    lock(_firstInstance->_mutex);

        if (_firstInstance->_builtinEnabledList.empty())
            return;


        std::vector<MenuEntryImpl *>    &vector = _firstInstance->_builtinEnabledList;

        // Update entry's state
        entry->_flags.state = false;
        entry->_flags.justChanged = false;

        // Remove the entry from list
        vector.erase(std::remove(vector.begin(), vector.end(), entry), vector.end());
    }

    void    PluginMenuExecuteLoop::ExecuteBuiltin(void)
    {
        if (_firstInstance == nullptr)
            return;

        (*_firstInstance)();
    }

    void    PluginMenuExecuteLoop::AddAR(MenuEntryActionReplay *entry)
    {
        if (_firstInstance == nullptr || entry == nullptr)
            return;

        Lock    lock(_firstInstance->_mutex);

        std::vector<MenuEntryActionReplay *> &list = _firstInstance->_arEnabledList;

        // If the list already has the same code abort
        for (MenuEntryActionReplay *ar : list)
            if (ar == entry)
                return;

        list.push_back(entry);
    }

    void    PluginMenuExecuteLoop::RemoveAR(MenuEntryActionReplay *entry)
    {
        if (_firstInstance == nullptr || entry == nullptr)
            return;

        Lock    lock(_firstInstance->_mutex);

        std::vector<MenuEntryActionReplay *> &list = _firstInstance->_arEnabledList;

        // Update entry's state
        entry->_flags.state = false;
        entry->_flags.justChanged = false;

        // Disable all hooks
        if (entry->context.hooks != nullptr)
        {
            for (Hook &hook : *entry->context.hooks)
                hook.Disable();
            entry->context.hooks->clear();
        }

        // Remove entry from list
        list.erase(std::remove(list.begin(), list.end(), entry), list.end());
    }

    void    PluginMenuExecuteLoop::ExecuteAR(void)
    {
        if (_firstInstance == nullptr)
            return;

        Lock    lock(_firstInstance->_mutex);

        for (MenuEntryActionReplay *ar : _firstInstance->_arEnabledList)
        {
            if (ar->GameFunc != nullptr)
                ar->GameFunc((MenuEntry *)(ar)); ///< cast is just for the warning, properly handled on the callback
        }
    }

    bool    PluginMenuExecuteLoop::operator()(void)
    {
        // Double condition pattern, removed the need of locking the mutex
        if (_builtinEnabledList.empty())
            return false;

        _mutex.Lock();

        if (_builtinEnabledList.empty())
        {
            _mutex.Unlock();
            return false;
        }

        bool needToRemove = false;
        for (MenuEntryImpl *entry : _builtinEnabledList)
        {
            // Execute MenuEntryImpl
            if (entry->IsEntry() && entry->_Execute()) ///< _Execute returns true if the entry have to be removed from enabled list
            {
                needToRemove = true;
            }
            // Execute FreeCheat
            /*else if (entry->IsFreeCheat())
            {
                MenuEntryFreeCheat *fc = reinterpret_cast<MenuEntryFreeCheat *>(entry);

                if (fc->Func != nullptr)
                    fc->Func(fc);
            }*/
            /*else if (entry->_type == ActionReplay)
            {
                MenuEntryActionReplay *ar = reinterpret_cast<MenuEntryActionReplay *>(entry);

                if (ar->GameFunc != nullptr)
                    ar->GameFunc((MenuEntry *)ar); ///< cast only to silence a warning
            } */
        }

        if (needToRemove)
        {
            _builtinEnabledList.erase(
                std::remove_if(_builtinEnabledList.begin(), _builtinEnabledList.end(),
                    [](MenuEntryImpl *e)
                    {
                        return e->_MustBeRemoved();
                    }),
                _builtinEnabledList.end());
        }

        _mutex.Unlock();
        return (false);
    }
}
