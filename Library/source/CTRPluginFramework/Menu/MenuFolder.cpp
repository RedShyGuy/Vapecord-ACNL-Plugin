#include "CTRPluginFramework/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu.hpp"


namespace CTRPluginFramework
{
    MenuFolder::MenuFolder(const std::string &name, const std::string &note) :
        OnAction{ nullptr }, _item(new MenuFolderImpl(this, name, note))
    {

    }

    MenuFolder::MenuFolder(const std::string& name, const std::vector<MenuEntry*>& entries) :
        OnAction{ nullptr }, _item(new MenuFolderImpl(this, name))
    {
        for (MenuEntry *entry : entries)
            Append(entry);
    }

    MenuFolder::MenuFolder(const std::string& name, const std::string& note, const std::vector<MenuEntry*>& entries) :
        OnAction{ nullptr }, _item(new MenuFolderImpl(this, name, note))
    {
        for (MenuEntry *entry : entries)
            Append(entry);
    }

    MenuFolder::~MenuFolder()
    {
    }

    void    MenuFolder::Hide(void) const
    {
        _item->Hide();
    }

    void    MenuFolder::Show(void) const
    {
        _item->Show();
    }

    bool    MenuFolder::IsVisible() const
    {
        return(_item->IsVisible());
    }

    void    MenuFolder::UseTopSeparator(Separator type) const
    {
        _item->Flags.useSeparatorBefore = type != Separator::None;
        _item->Flags.useStippledLineForBefore = type == Separator::Stippled;
    }

    void    MenuFolder::UseBottomSeparator(Separator type) const
    {
        _item->Flags.useSeparatorAfter = type != Separator::None;
        _item->Flags.useStippledLineForBefore = type == Separator::Stippled;
    }

    void    MenuFolder::Append(MenuEntry *item) const
    {
        MenuEntryImpl *entry = item->_item.get();

        _item->Append(entry);
    }

    void    MenuFolder::Append(MenuFolder *item) const
    {
        MenuFolderImpl *folder = item->_item.get();

        _item->Append(folder);
    }

    std::vector<MenuEntry *>    MenuFolder::GetEntryList(void) const
    {
        return (_item->GetEntryList());
    }

    std::vector<MenuFolder *>   MenuFolder::GetFolderList(void) const
    {
        return (_item->GetFolderList());
    }

    std::string &MenuFolder::Name(void) const
    {
        return (_item->name);
    }

    std::string &MenuFolder::Note(void) const
    {
        return (_item->note);
    }

    Color   &MenuFolder::NameColor(void) const
    {
        return (_item->color);
    }

    u32    MenuFolder::ItemsCount(void) const
    {
        return (_item->ItemsCount());
    }

    void    MenuFolder::Clear(void) const
    {
        _item->Clear();
    }

    void    MenuFolder::Remove(u32 startIndex, u32 count, bool destroy) const
    {
        _item->Remove(startIndex, count, destroy);
    }

    MenuFolder    *MenuFolder::operator += (const MenuEntry *item)
    {
        MenuEntryImpl *entry = item->_item.get();

        _item->Append(entry);

        return (this);
    }

    MenuFolder *MenuFolder::operator-=(const MenuEntry *entry)
    {
        _item->Remove(entry->_item.get());
        return (this);
    }

    MenuFolder  *MenuFolder::operator+=(const MenuFolder *folder)
    {
        MenuFolderImpl *f = folder->_item.get();

        _item->Append(f);
        return (this);
    }

    MenuFolder *MenuFolder::operator-=(const MenuFolder *folder)
    {
        _item->Remove(folder->_item.get());
        return (this);
    }
}
