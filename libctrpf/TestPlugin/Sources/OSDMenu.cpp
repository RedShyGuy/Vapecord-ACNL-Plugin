#include "OSDMenu.hpp"
#include "CTRPluginFramework.hpp"
#include <3ds.h>
#include <algorithm>

#define AtomicRead(ptr) __atomic_load_n((u32 *)(ptr), __ATOMIC_SEQ_CST)

namespace CTRPluginFramework
{
    OSDMenu     OSDMenu::_instance;

    OSDMenu & OSDMenu::GetInstance(void)
    {
        return _instance;
    }

    bool    OSDMenu::IsBusy(void)
    {
        return AtomicRead(&_isBusy);
    }

    void    OSDMenu_UpdateMenu(void);
    bool    OSDMenu_Draw(const Screen &screen);
    int     GetMenuInput(void)
    {
        Process::Pause();

        const Screen &topScreen = OSD::GetTopScreen();
        OSDMenu &menu = OSDMenu::GetInstance();

        while (menu.IsBusy())
        {
            Controller::Update();
            OSDMenu_Draw(topScreen);
            OSDMenu_UpdateMenu();
            OSD::SwapBuffers();
        }

        Process::Play();

        return menu.GetSelectionIndex();
    }

    void    OSDMenu::Open(void)
    {
        PluginMenu *menu = PluginMenu::GetRunningInstance();

        if (IsBusy() || menu == nullptr)
            return;

        AtomicIncrement(&_isBusy);
        _selector = 0;
        _selection = -1;
        //*menu += OSDMenu_UpdateMenu;
        //OSD::Run(OSDMenu_Draw);
    }




    void    OSDMenu::Close(void)
    {
        PluginMenu *menu = PluginMenu::GetRunningInstance();

        if (!IsBusy() || menu == nullptr)
            return;

        *menu -= OSDMenu_UpdateMenu;
        OSD::Stop(OSDMenu_Draw);
        AtomicDecrement(&_isBusy);
    }

    void    OSDMenu::Clear(void)
    {
        if (IsBusy())
            return;

        _entries.clear();
        _title.clear();
    }

    int     OSDMenu::GetSelectionIndex(void)
    {
        return _selection;
    }

    void    OSDMenu::SetTitle(const std::string &title)
    {
        _title = title;
    }

    void    OSDMenu::operator+=(const std::string &entry)
    {
        if (IsBusy())
            return;

        _entries.push_back(entry);
    }

    void    OSDMenu::operator+=(const std::vector<std::string> &entries)
    {
        if (IsBusy())
            return;

        for (const std::string &str : entries)
            _entries.push_back(str);
    }

    OSDMenu::OSDMenu(void)
    {
        _isBusy = false;
        _selection = -1;
        _selector = 0;
    }

    void     OSDMenu_UpdateMenu(void)
    {
        static Clock    clock;
        u32         keys = Controller::GetKeysDown();
        OSDMenu     &menu = OSDMenu::GetInstance();

        if (!clock.HasTimePassed(Milliseconds(100)))
            return;

        if (keys & Key::A) menu._selection = menu._selector;
        // else if (keys & Key::B) menu.Close(); ///< Uncomment to enable the abort
        else if (keys & Key::Up) menu._selector = std::max((int)0, menu._selector - 1);
        else if (keys & Key::Down) menu._selector = std::min((int)menu._entries.size() - 1, menu._selector + 1);

        clock.Restart();

        if (menu._selection != -1)
            menu.Close();
    }

    bool     OSDMenu_Draw(const Screen &screen)
    {
        u32     posY = 22;
        u32     posX = 35;
        OSDMenu &menu = OSDMenu::GetInstance();

        if (!screen.IsTop || menu._entries.empty()) return false;

        // Background
        screen.DrawRect(30, 20, 340, 200, Color::Black, true);

        // Draw title if any
        if (!menu._title.empty())
        {
            screen.Draw(menu._title, posX, posY, Color::LimeGreen);
            screen.DrawRect(posX, posY + 10, (Utils::GetSize(menu._title) + 3) * 6, 1, Color::LimeGreen, false);
        }

        posY += 13;
        // Parse and draw entries
        {
            std::vector<std::string>    &entries = menu._entries;
            u32     entryCount = entries.size();
            u32     start = std::max((int)0, menu._selector - 13);
            u32     end = std::min(entryCount, start + 18);

            for (; start < end; ++start, posY += 10)
            {
                screen.Draw(entries[start], posX, posY, start == menu._selector ? Color::DeepSkyBlue : Color::White);
            }
        }

        return true;
    }
}
