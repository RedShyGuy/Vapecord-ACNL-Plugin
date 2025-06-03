#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"

namespace CTRPluginFramework
{
    u32     MenuItem::_uidCounter = 0;

    using   MenuIter = std::vector<MenuItem*>::iterator;

    void    MenuItem::NoteChanged(void)
    {
        Flags.noteChanged = true;
    }

    bool    MenuItem::HasNoteChanged(void) const
    {
        return (Flags.noteChanged);
    }

    void    MenuItem::HandledNoteChanges(void)
    {
        Flags.noteChanged = false;
    }

    void    MenuItem::_DisableFolder(MenuFolderImpl *folder)
    {
        if (folder == nullptr)
            return;

       MenuIter iter = folder->_items.begin();

        for (; iter != folder->_items.end(); ++iter)
        {
            MenuItem *item = *iter;

            if (!item)
                break;

            // If the item is starred
            if (item->_IsStarred())
                PluginMenuImpl::UnStar(item);

            // If it's a folder, call this function again
            if (item->_type == MenuType::Folder)
            {
                MenuFolderImpl *f = reinterpret_cast<MenuFolderImpl*>(item);

                // Set Hidden flag to the folder
                f->Flags.isVisible = false;
                _DisableFolder(f);
            }

            // Else disable the entry
            else
                reinterpret_cast<MenuEntryImpl *>(item)->Disable();
        }
    }

    void    MenuItem::Hide(void)
    {
        if (Flags.isVisible == false)
            return;

        Flags.isVisible = false;

        // If the item is starred
        if (Flags.isStarred)
            PluginMenuImpl::UnStar(this);

        // If it's an entry, disable it
        if (_type == MenuType::Entry)
        {
            MenuEntryImpl *e = reinterpret_cast<MenuEntryImpl*>(this);

            if (e != nullptr)
                e->Disable();
        }
        // Else refresh the menu to be sure that we're not currently in a hidden folder
        else if (_type == MenuType::Folder)
        {
            // Now forcefully disable every sub entries
            _DisableFolder(reinterpret_cast<MenuFolderImpl *>(this));

            PluginMenuImpl::Refresh();
        }

        // Remove the item from it's container
        if (_container != nullptr)
        {
            MenuFolderImpl *container = reinterpret_cast<MenuFolderImpl *>(_container);
            std::vector<MenuItem *> &items = container->_items;

            items.erase(std::remove(items.begin(), items.end(), this), items.end());
        }
    }

    void    MenuItem::_EnableFolder(MenuFolderImpl *folder)
    {
        if (folder == nullptr)
            return;

        MenuIter iter = folder->_items.begin();

        for (; iter != folder->_items.end(); ++iter)
        {
            MenuItem *item = *iter;

            if (item->_type == MenuType::Folder)
            {
                // Set visible flag
                MenuFolderImpl  *f = reinterpret_cast<MenuFolderImpl *>(item);

                f->Flags.isVisible = true;
                _EnableFolder(f);
            }
        }
    }

    void    MenuItem::Show(void)
    {
        if (Flags.isVisible == true)
            return;

        Flags.isVisible = true;

        // If it's a folder
        if (_type == MenuType::Folder)
        {
            // Remove hidden flag from all sub folders
            _EnableFolder(reinterpret_cast<MenuFolderImpl *>(this));
        }

        // Reinsert it to the _container
        if (_container != nullptr)
        {
            MenuFolderImpl *container = reinterpret_cast<MenuFolderImpl *>(_container);
            std::vector<MenuItem *> &items = container->_items;
            std::vector<MenuItem *>::iterator iter = items.begin();

            if (_index < static_cast<int>(items.size()))
            {
                std::advance(iter, _index);
                container->_items.insert(iter, this);
            }
            else
            {
                items.push_back(this);
            }
        }
    }
}
