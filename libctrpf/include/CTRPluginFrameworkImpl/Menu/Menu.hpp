#ifndef CTRPLUGINFRAMEWORKIMPL_MENUIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_MENUIMPL_HPP

#include "types.h"
#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Events.hpp"
#include "CTRPluginFramework/System/Time.hpp"

namespace CTRPluginFramework
{
    enum MenuEvent
    {
        Error = -1,
        EntrySelected = -2,
        FolderChanged = -3,
        MenuClose = -4,
        SelectorChanged = -5,
        Nothing = -6
    };

    class Menu
    {
        using IconCallback = int(*)(int, int);
    public:

        Menu(const std::string &title, const std::string &footer = "", IconCallback iconCallback = nullptr);
        Menu(MenuFolderImpl *folder, IconCallback iconCallback = nullptr);
        ~Menu(void);

        void            Append(MenuItem *item) const;
        void            Remove(MenuItem *item);
        MenuFolderImpl  *GetFolder(void) const;
        MenuFolderImpl  *GetRootFolder(void) const;
        MenuItem        *GetSelectedItem(void);

        void    Draw(void) const; ///< Stupid I know
        MenuFolderImpl    *Open(MenuFolderImpl *folder, int selector = 0);
        void    CloseAll(void);
        void    Update(const Time &delta);
        bool    ShowNote(void);
        void    CloseNote(void);
        bool    IsNoteOpen(void);
        // Insert item at current index
        void    Insert(MenuItem *item);
        // Remove selected item from folder, and return it
        MenuItem *Pop(void);

        /*
        ** Return value:
        ** -1 : error or menu is empty
        ** -2 : user pressed B to exit the menu
        ** >= 0 : user choice (irrelevant on menu using folders, so prefer using an overload returning the object)
        *******************************************/
        // This return a menuEvent value
        int     ProcessEvent(Event &event, MenuItem **userchoice);

        bool    drawFooter;
    private:
        friend class PluginMenuTools;
        MenuFolderImpl  *_root;
        MenuFolderImpl  *_folder;
        MenuItem        *_lastSelectedItem;
        MenuItem        *_lastNoteItem;

        Clock           _input;
        Clock           _scrollClock;

        TextBox         _noteTB;
        IconCallback    _iconCallback;
        int             _selector;
        u32             _selectedNameSize;
        u32             _reverseFlow;
        float           _scrollOffset;
        float           _maxScrollOffset;

        void            _ScrollUp(int step);
        void            _ScrollDown(int step);
    };
}

#endif
