#ifndef OSDMENU_HPP
#define OSDMENU_HPP

#include <3ds.h>
#include <string>
#include <vector>

namespace CTRPluginFramework
{
    class Screen;
    class OSDMenu
    {
    public:

        static OSDMenu &GetInstance(void);

        bool    IsBusy(void);
        void    Open(void);
        void    Close(void);
        void    Clear(void);
        int     GetSelectionIndex(void);

        void    SetTitle(const std::string &title);
        void    operator+=(const std::string &entry);
        void    operator+=(const std::vector<std::string> &entries);

    private:

        friend void     OSDMenu_UpdateMenu(void);
        friend bool     OSDMenu_Draw(const Screen &screen);
        static OSDMenu     _instance;

        OSDMenu(void);

        u32     _isBusy;
        int     _selection;
        int     _selector;
        std::string     _title;
        std::vector<std::string> _entries;
    };
}

#endif
