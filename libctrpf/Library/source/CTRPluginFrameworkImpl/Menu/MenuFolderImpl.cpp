#include "types.h"

#include "CTRPluginFramework/Utils/TextFileParser.hpp"
#include "CTRPluginFrameworkImpl/Menu.hpp"

namespace CTRPluginFramework
{
    MenuFolderImpl::MenuFolderImpl(const std::string &name, const std::string &note) :
        MenuItem(MenuType::Folder), _owner(nullptr)
    {
        /*
        this->name = name;
        this->firstName = name;
        this->note = note;
        this->_position[0] = -1;
        this->_position[1] = -1;
        this->_parent[0] = nullptr;
        this->_parent[1] = nullptr;
        */
        //vapecord
        this->name = name;
        this->firstName = Color::RemoveColor(name);
        this->note = note;
        this->firstNote = note;
        this->color = Color::GetColor(name);
        this->_position[0] = -1;
        this->_position[1] = -1;
        this->_parent[0] = nullptr;
        this->_parent[1] = nullptr;
    }

    MenuFolderImpl::MenuFolderImpl(MenuFolder* owner, const std::string& name, const std::string& note) :
    MenuItem(MenuType::Folder), _owner(owner)
    {
		/*
        this->name = name;
        this->firstName = name;
        this->note = note;
        this->_position[0] = -1;
        this->_position[1] = -1;
        this->_parent[0] = nullptr;
        this->_parent[1] = nullptr;
        */
        this->name = name;
        this->firstName = Color::RemoveColor(name);
        this->note = note;
        this->firstNote = note;
        this->color = Color::GetColor(name);
        this->_position[0] = -1;
        this->_position[1] = -1;
        this->_parent[0] = nullptr;
        this->_parent[1] = nullptr;
    }

    MenuFolderImpl::~MenuFolderImpl()
    {
        for (MenuItem *item : _items)
        {
            // If item has an owner, call the owner's destructor
            if (item->IsEntry() && item->AsMenuEntryImpl().AsMenuEntry())
                delete item->AsMenuEntryImpl().AsMenuEntry();
            else if (item->IsFolder() && item->AsMenuFolderImpl()._owner)
                delete item->AsMenuFolderImpl()._owner;
            else
                delete item;
        }

        // Close this folder if it's the one opened in the menu
        PluginMenuImpl::Close(this);

        _items.clear();
    }

    void    MenuFolderImpl::Append(MenuItem *item, bool isStar)
    {
        if (!isStar)
        {
            item->_container = this;
            item->_index = _items.size();
        }
        _items.push_back(item);

    }

    u32    MenuFolderImpl::ItemsCount(void) const
    {
        return (_items.size());
    }

    MenuItem *MenuFolderImpl::GetItem(u32 uid)
    {
        for (MenuItem *item : _items)
        {
            if (item->Uid == uid)
                return (item);

            if (item->_type == MenuType::Folder)
            {
                MenuItem * i = reinterpret_cast<MenuFolderImpl *>(item)->GetItem(uid);

                if (i != nullptr)
                    return (i);
            }
        }

        return (nullptr);
    }

    void    MenuFolderImpl::DisableAll(void)
    {
        for (MenuItem *item : _items)
        {
            if (item->IsEntry() || item->IsFreeCheat())
                reinterpret_cast<MenuEntryImpl *>(item)->Disable();
            if (item->IsFolder())
                reinterpret_cast<MenuFolderImpl *>(item)->DisableAll();
        }
    }

    std::vector<MenuEntry *> MenuFolderImpl::GetEntryList(void) const
    {
        std::vector<MenuEntry *> entries;

        for (MenuItem *item : _items)
        {
            if (item->IsEntry())
            {
                MenuEntry *entry = item->AsMenuEntryImpl().AsMenuEntry();

                if (entry != nullptr)
                    entries.push_back(entry);
            }
        }
        return (entries);
    }

    std::vector<MenuFolder *> MenuFolderImpl::GetFolderList(void) const
    {
        std::vector<MenuFolder *> folders;

        for (MenuItem *item : _items)
        {
            if (item->IsFolder())
            {
                MenuFolder *folder = item->AsMenuFolderImpl()._owner;

                if (folder != nullptr)
                    folders.push_back(folder);
            }
        }

        return (folders);
    }

    MenuItem* MenuFolderImpl::operator[](u32 index)
    {
        if (index >= _items.size())
            return (nullptr);

        return (_items[index]);
    }

    bool    MenuFolderImpl::HasParent()
    {
        return (_parent[0] != nullptr || _parent[1] != nullptr);
    }

    void    MenuFolderImpl::Remove(MenuItem *item)
    {
        _items.erase(std::remove(_items.begin(), _items.end(), item), _items.end());
    }

    void    MenuFolderImpl::Remove(u32 start, u32 count, bool destroy)
    {
        if (start >= _items.size())
            return;

        if (start + count >= _items.size())
            count = _items.size() - start;

        auto begin = _items.begin() + start;
        auto end = begin + count;

        if (destroy)
        {
            for (auto b = begin, e = end; b != e; ++b)
                delete *b;
        }

        _items.erase(begin, end);
    }

    void    MenuFolderImpl::Clear(void)
    {
        for (MenuItem *item : _items)
        {
            if (item->IsEntry() && item->AsMenuEntryImpl().AsMenuEntry())
                delete item->AsMenuEntryImpl().AsMenuEntry();
            else if (item->IsFolder() && item->AsMenuFolderImpl()._owner)
                delete item->AsMenuFolderImpl()._owner;
            else
                delete item;
        }
        _items.clear();
    }

    //#######################################################################

    void    MenuFolderImpl::_Open(MenuFolderImpl *parent, int position, bool starMode)
    {
        int index = starMode ? 1 : 0;
        _parent[index] = parent;
        _position[index] = position;
    }

    MenuFolderImpl  *MenuFolderImpl::_Close(int &position, bool starMode)
    {
        int index = starMode ? 1 : 0;
        if (_parent[index] != nullptr && _position[index] != -1)
            position = _position[index];
        return (_parent[index]);
    }
    
    //V-settings
    void SetStrings(MenuItem *item, std::vector<MenuEntry *> quickMenu) {
        std::string _strName = Language->Get(item->firstName.c_str());
        std::string _strNote = Language->Get(item->firstNote.c_str());

        item->name = item->color << (_strName == "???" ? item->name : _strName);
        item->note = (_strNote == "???" ? item->note : _strNote);

        if(item->IsEntry()) {
            MenuEntry *entry = item->AsMenuEntryImpl().AsMenuEntry();
            entry->RefreshNote();

            if(entry->Hotkeys.Count() == 1) {
                std::string hotk = entry->Hotkeys[0].ToString();
                item->name = item->color << (_strName == "???" ? (item->name + " " + hotk) : (_strName + " " + hotk));
            }

            for(u32 i = 0; i < entry->Hotkeys.Count(); ++i) {
                std::string _strName = Language->Get(entry->Hotkeys[i].GetFirstName().c_str());
                entry->Hotkeys[i] = (_strName == "???" ? entry->Hotkeys[i].GetName() : _strName);
            }

            if(entry->CogID() != -1) {
                for(auto qdata : quickMenu) {
                //if entry is stored in the quick menu, hide it
                    if(entry->CogID() == qdata->CogID()) {
                        if(entry->IsVisible()) 
                            entry->Hide();
                    }
                }
            }
        }
    }

    void MenuFolderImpl::Reload(std::vector<MenuEntry *> quickMenu) {
        for(MenuItem *item : _items) {
            if(item->IsEntry() || item->IsFolder()) 
                SetStrings(item, quickMenu);

            if(item->IsFolder()) {
                for(MenuItem *In_item : item->AsMenuFolderImpl()._items) {
                    if(In_item->IsEntry() || In_item->IsFolder()) 
                        SetStrings(In_item, quickMenu);

                    if(In_item->IsFolder()) {
                        for(MenuItem *SubIn_item : In_item->AsMenuFolderImpl()._items) 
                            SetStrings(SubIn_item, quickMenu);
                    }
                }
            }
        }
    }
}
