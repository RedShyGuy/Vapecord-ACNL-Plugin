#ifndef CTRPLUGINFRAMEWORKIMPL_MENUITEMIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_MENUITEMIMPL_HPP

#include "CTRPluginFramework/Graphics/Color.hpp"
#include "types.h"
#include <string>

namespace CTRPluginFramework
{
    enum MenuType
    {
        Folder,
        Entry,
        EntryTools,
        ActionReplay,
        FreeCheat
    };

    struct ItemFlags
    {
        bool    useSeparatorBefore : 1;
        bool    useSeparatorAfter : 1;
        bool    useStippledLineForBefore : 1;
        bool    useStippledLineForAfter : 1;
        bool    isVisible : 1;
        bool    isStarred : 1;
        bool    noteChanged : 1;
    };

    class MenuEntryImpl;
    class MenuEntryTools;
    class MenuFolderImpl;
    class MenuItem
    {
    public:
        virtual ~MenuItem() = default;

        MenuItem(MenuType type) :
        Uid(++_uidCounter),
        _type(type), _container(nullptr), _index(0)
        {
            Flags.useSeparatorBefore = false;
            Flags.useSeparatorAfter = false;
            Flags.useStippledLineForBefore = false;
            Flags.useStippledLineForAfter = false;
            Flags.isVisible = true;
            Flags.isStarred = false;
            Flags.noteChanged = false;
        }

        std::string     name;
        std::string     firstName;
        std::string     note;
        //vapecord
        std::string     firstNote;
        Color           color;
        s8              cogID;
        s16             cheatID;

        void    Hide(void);
        void    Show(void);
        bool    IsVisible(void) const
        {
            return (Flags.isVisible);
        }

        bool    IsEntry(void) const
        {
            return (_type == Entry);
        }

        bool    IsFolder(void) const
        {
            return (_type == Folder);
        }

        bool    IsEntryTools(void) const
        {
            return (_type == EntryTools);
        }

        bool    IsFreeCheat(void) const
        {
            return (_type == FreeCheat);
        }

        MenuEntryImpl &AsMenuEntryImpl(void)
        {
            return (*reinterpret_cast<MenuEntryImpl *>(this));
        }

        MenuEntryTools &AsMenuEntryTools(void)
        {
            return (*reinterpret_cast<MenuEntryTools *>(this));
        }

        MenuFolderImpl &AsMenuFolderImpl(void)
        {
            return (*reinterpret_cast<MenuFolderImpl *>(this));
        }

        virtual std::string &GetNote(void)
        {
            return (note);
        }

        void    NoteChanged(void);
        bool    HasNoteChanged(void) const;
        void    HandledNoteChanges(void);

        const u32   Uid;

        ItemFlags   Flags;
    protected:
        friend class MenuFolderImpl;
        friend class PluginMenuImpl;
        friend class PluginMenuHome;
        friend class Menu;

        static void     _DisableFolder(MenuFolderImpl *folder);
        static void     _EnableFolder(MenuFolderImpl* folder);

        bool        _IsStarred(void) const
        {
            return (Flags.isStarred);
        }

        bool        _TriggerStar(void)
        {
            Flags.isStarred = !Flags.isStarred;
            return (Flags.isStarred);
        }

        MenuType    _type;
        MenuItem    *_container; /* MenuFolderImpl */
        int         _index;


        static u32  _uidCounter;
    };
}

#endif
