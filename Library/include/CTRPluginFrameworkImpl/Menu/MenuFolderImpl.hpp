#ifndef CTRPLUGINFRAMEWORKIMPL_MENUFOLDERIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_MENUFOLDERIMPL_HPP

#include "types.h"

#include "CTRPluginFrameworkImpl/Menu/MenuItem.hpp"
#include <vector>
#include <string>
#include <memory>

namespace CTRPluginFramework
{
    class Menu;
    class MenuEntry;
    class MenuFolder;
    class MenuFolderImpl : public MenuItem
    {
    public:
        MenuFolderImpl(const std::string &name, const std::string &note = "");
        MenuFolderImpl(MenuFolder *owner, const std::string &name, const std::string &note = "");
        virtual ~MenuFolderImpl();

        void        Append(MenuItem *item, bool isStar = false);
        u32         ItemsCount(void) const;
        MenuItem    *GetItem(u32 uid);
        void        DisableAll(void);

        std::vector<MenuEntry *>    GetEntryList(void) const;
        std::vector<MenuFolder *>   GetFolderList(void) const;
        MenuItem    *operator[](u32 index);
        bool        HasParent(void);
        void        Remove(MenuItem *item);
        void        Remove(u32 start, u32 count = 1, bool destroy = false);
        void        Clear(void);
        void        Reload(std::vector<MenuEntry *> quickMenu);

        using MenuItemIter = std::vector<MenuItem *>::iterator;

        MenuItemIter    begin(void)
        {
            return _items.begin();
        }
        MenuItemIter    end(void)
        {
            return _items.end();
        }

    private:
        friend class MenuItem;
        friend class PluginMenuImpl;
        friend class PluginMenuHome;
        friend class FreeCheats;
        friend class Menu;

        // Private methods
        void            _Open(MenuFolderImpl *parent, int position, bool starMode = false);
        MenuFolderImpl      *_Close(int &position, bool starMode = false);

        // Private members
        MenuFolder                  *_owner;
        MenuFolderImpl              *_parent[2];
        std::vector<MenuItem *>     _items;
        int                         _position[2];

    };
}

#endif
