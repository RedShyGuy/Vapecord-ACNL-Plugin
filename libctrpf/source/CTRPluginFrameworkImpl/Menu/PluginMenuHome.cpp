#include "CTRPluginFrameworkImpl/Menu/PluginMenuHome.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFramework/Menu/MenuFolder.hpp"
#include "CTRPluginFramework/Sound.hpp"

#include <cstring>

namespace CTRPluginFramework
{
    static u32 g_textXpos[2] = { 0 };

    // DO NOT REMOVE THIS COPYRIGHT NOTICE
    static const char g_ctrpfText[] = "CTRPluginFramework";
    static const char g_copyrightText[] = "Copyright (c) The Pixellizer Group";

    PluginMenuHome::PluginMenuHome(std::string &name, bool showNoteBottom) :

        _noteTB("", "", showNoteBottom ? IntRect(20, 46, 280, 124) : IntRect(40, 30, 320, 180)),

        _showStarredBtn(Button::Toggle | Button::Sysfont | Button::Rounded, "Favorite", IntRect(30, 70, 120, 30), Icon::DrawFavorite),
        _hidMapperBtn(Button::Sysfont | Button::Rounded, "Mapper", IntRect(165, 70, 120, 30), Icon::DrawController),
        _gameGuideBtn(Button::Sysfont | Button::Rounded, "Game Guide", IntRect(30, 105, 120, 30), Icon::DrawGuide),
        _searchBtn(Button::Sysfont | Button::Rounded, "Search", IntRect(165, 105, 120, 30), Icon::DrawSearch),
        _arBtn(Button::Sysfont | Button::Rounded, "ActionReplay", IntRect(30, 140, 120, 30)),
        _toolsBtn(Button::Sysfont | Button::Rounded, "Tools", showNoteBottom ? IntRect(99, 172, 120, 30) : IntRect(165, 140, 120, 30), Icon::DrawTools),

       // _closeBtn(*this, nullptr, IntRect(275, 24, 20, 20), Icon::DrawClose),
        _keyboardBtn(Button::Icon, IntRect(130, 30, 25, 25), Icon::DrawKeyboard),
        _controllerBtn(Button::Icon, IntRect(170, 30, 25, 25), Icon::DrawGameController25),

        _AddFavoriteBtn(Button::Icon | Button::Toggle, IntRect(50, 30, 25, 25), Icon::DrawAddFavorite),
        _InfoBtn(Button::Icon | Button::Toggle, IntRect(90, 30, 25, 25), Icon::DrawInfo)
    {
        _root = _folder = new MenuFolderImpl(name);
        _starredConst = _starred = new MenuFolderImpl("Favorites");

        _starMode = false;
        _selector = 0;
        _selectedTextSize = 0;
        _scrollOffset = 0.f;
        _maxScrollOffset = 0.f;
        _reverseFlow = false;
        _showVersion = false;
        _versionPosX = 0;

        ShowNoteBottom = showNoteBottom;

        _mode = 0;

        // Disable Toggle buttons
        _AddFavoriteBtn.Disable();
        _InfoBtn.Disable();
        _keyboardBtn.Disable();
        _controllerBtn.Disable();


        // Temporary disable unused buttons
        _hidMapperBtn.Lock();

        // Get strings x position
        g_textXpos[0] = (320 - Renderer::LinuxFontSize(g_ctrpfText)) / 2;
        g_textXpos[1] = (320 - Renderer::LinuxFontSize(g_copyrightText)) / 2;

        // Are the buttons locked ?
        if (!Preferences::Settings.AllowActionReplay)
            _arBtn.Lock();
        if (!Preferences::Settings.AllowSearchEngine)
            _searchBtn.Lock();
    }

    bool PluginMenuHome::operator()(EventList& eventList, int& mode, Time& delta)
    {
        static Task top([](void *arg)
        {
            PluginMenuHome *home = reinterpret_cast<PluginMenuHome *>(arg);
            if (home->ShowNoteBottom) {
                Renderer::SetTarget(TOP);
                home->_RenderTop();
            }
            else {
                Renderer::SetTarget(TOP);
                if (home->_noteTB.IsOpen())
                    home->_noteTB.Draw();
                else
                    home->_RenderTop();
            }

            return (s32)0;

        }, this, Task::AppCores);

        _mode = mode;

        // Process events
        if (_noteTB.IsOpen() && !ShowNoteBottom)
        {
            for (size_t i = 0; i < eventList.size(); i++)
                if (_noteTB.ProcessEvent(eventList[i]) == false)
                {
                    _InfoBtn.SetState(false);
                    break;
                }
        }
        else
        {
            for (size_t i = 0; i < eventList.size(); i++)
                _ProcessEvent(eventList[i]);
        }

        if (_toolsBtn()) _toolsBtn_OnClick();

        if (!ShowNoteBottom) {
            // Check all buttons
            if (_showStarredBtn()) _showStarredBtn_OnClick();
            // _hidMapperBtn();
            if (_gameGuideBtn()) _gameGuideBtn_OnClick();
            if (_searchBtn()) _searchBtn_OnClick();
            if (_arBtn()) _actionReplayBtn_OnClick();
            if (_AddFavoriteBtn()) _StarItem();
            if (_InfoBtn()) _InfoBtn_OnClick();
            if (_keyboardBtn()) _keyboardBtn_OnClick();
            if (_controllerBtn()) _controllerBtn_OnClick();
        }

        // Update UI
        _Update(delta);

        // Render top
        top.Start();

        // RenderBottom
        _RenderBottom();

        top.Wait();

        mode = _mode;

        return (Window::BottomWindow.MustClose());
    }

    void PluginMenuHome::Append(MenuItem* item) const
    {
        _folder->Append(item);
    }

    void PluginMenuHome::Refresh(void)
    {
        // If the currently selected folder is root
        // Nothing to do
        if (_folder->_container != nullptr)
        {
            // If current folder is hidden, close it
            while (!_folder->Flags.isVisible)
            {
                MenuFolderImpl *p = _folder->_Close(_selector);

                if (p)
                {
                    _folder = p;
                    if (_selector >= 1)
                        _selector--;
                }
                else
                    break;
            }
        }

        // Starred folder
        do
        {
            // If the currently selected folder is root
            // Nothing to do
            if (_starred->_container == nullptr)
                break;

            // If current folder is hidden, close it
            while (!_starred->Flags.isVisible)
            {
                MenuFolderImpl *p = _starred->_Close(_selector, true);

                if (p)
                {
                    _starred = p;
                    if (_selector >= 1)
                        _selector--;
                }
                else
                    break;
            }
        } while (true);

        // Check for the validity of _selector range
        MenuFolderImpl *folder = _starMode ? _starred : _folder;

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            _selector = 0;

    }

#define IsUnselectableEntry(item) (item->IsEntry() && item->AsMenuEntryImpl()._flags.isUnselectable)
    static u32  SelectableEntryCount(MenuFolderImpl &folder)
    {
        u32 count = 0;

        for (u32 i = 0; i < folder.ItemsCount(); i++)
        {
            MenuItem *item = folder[i];

            if (item->IsEntry() && item->AsMenuEntryImpl()._flags.isUnselectable)
                continue;
            else
                count++;
        }
        return (count);
    }

    static void ScrollUp(int &selector, MenuFolderImpl &folder, int step)
    {
        // If there's no selectable entry in the folder, return
        if (!SelectableEntryCount(folder))
            return;

        // We're already at the begining
        if (selector == 0)
        {
            // Else select last item
            selector = folder.ItemsCount() - 1;
            // If entry is unselectable scroll again
            if (IsUnselectableEntry(folder[selector]))
                ScrollUp(selector, folder, step);
            else
                SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
            return;
        }
        // Else go up
        selector -= step;
        if (selector < 0)
            selector = 0;
        // If entry is unselectable, scroll again
        if (IsUnselectableEntry(folder[selector]))
        {
            step = step > 1 ? step - 1 : 1;
            ScrollUp(selector, folder, step);
        }
        else
            SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
    }

    static void ScrollDown(int &selector, MenuFolderImpl &folder, int step)
    {
        // If there's no selectable entry in the folder, return
        if (!SelectableEntryCount(folder))
            return;

        // We're already at the end
        if (selector == static_cast<int>(folder.ItemsCount()) - 1)
        {
            // Else select first item
            selector = 0;
            // If entry is unselectable scroll again
            if (IsUnselectableEntry(folder[selector]))
                ScrollDown(selector, folder, step);
            else
                SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
            return;
        }
        // Else go down
        selector += step;
        if (selector >= static_cast<int>(folder.ItemsCount()))
            selector = folder.ItemsCount() - 1;
        // If entry is unselectable, scroll again
        if (IsUnselectableEntry(folder[selector]))
        {
            step = step > 1 ? step - 1 : 1;
            ScrollDown(selector, folder, step);
        }
        else
            SoundEngine::PlayMenuSound(SoundEngine::Event::CURSOR);
    }

    //###########################################
    // Process Event
    //###########################################
    void PluginMenuHome::_ProcessEvent(Event& event)
    {
        static Clock fastScroll;
        static Clock inputClock;
        static MenuItem* last = nullptr;

        MenuFolderImpl* folder = _starMode ? _starred : _folder;
        MenuItem* item;

        switch (event.type)
        {
            case Event::KeyDown:
            {
                if (fastScroll.HasTimePassed(Milliseconds(800)) && inputClock.HasTimePassed(Milliseconds(100)))
                {
                    switch (event.key.code)
                    {
                            /*
                            ** Selector
                            **************/
                        case Key::CPadUp:
                        case Key::DPadUp:
                        {
                            ScrollUp(_selector, *folder, 1);
                            break;
                        }
                        case Key::CPadDown:
                        case Key::DPadDown:
                        {
                            ScrollDown(_selector, *folder, 1);
                            break;
                        }
                        case Key::CPadLeft:
                        case Key::DPadLeft:
                        {
                            ScrollUp(_selector, *folder, 4);
                            break;
                        }
                        case Key::CPadRight:
                        case Key::DPadRight:
                        {
                            ScrollDown(_selector, *folder, 4);
                            break;
                        }
                        default: break;
                    }
                    inputClock.Restart();
                }
                break;
            } // Event::KeyDown
            case Event::KeyPressed:
            {
                switch (event.key.code)
                {
                        /*
                        ** Selector
                        **************/
                    case Key::CPadUp:
                    case Key::DPadUp:
                    {
                        ScrollUp(_selector, *folder, 1);
                        fastScroll.Restart();
                        break;
                    }
                    case Key::CPadDown:
                    case Key::DPadDown:
                    {
                        ScrollDown(_selector, *folder, 1);
                        fastScroll.Restart();
                        break;
                    }
                    case Key::CPadLeft:
                    case Key::DPadLeft:
                    {
                        ScrollUp(_selector, *folder, 4);
                        fastScroll.Restart();
                        break;
                    }
                    case Key::CPadRight:
                    case Key::DPadRight:
                    {
                        ScrollDown(_selector, *folder, 4);
                        fastScroll.Restart();
                        break;
                    }
                        /*
                        ** Trigger entry
                        ** Top Screen
                        ******************/
                    case Key::A:
                    {
                        _TriggerEntry();
                        break;
                    }
                        /*
                        ** Closing Folder
                        ********************/
                    case Key::B:
                    {
                        MenuFolderImpl *newFolder = folder->_Close(_selector, _starMode);

                        // Call the MenuEntry::OnAction callback if there's one
                        if (folder->_owner != nullptr && folder->_owner->OnAction != nullptr)
                            folder->_owner->OnAction(*_folder->_owner, ActionType::Closing);

                        // Switch current folder
                        if (newFolder != nullptr)
                        {
                            SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                            if (_starMode)
                                _starred = newFolder;
                            else
                                _folder = newFolder;
                        }
                        break;
                    }
                    default: break;
                }
                break;
            } // End Key::Pressed event
            default: break;
        } // End switch

        folder = _starMode ? _starred : _folder;

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            _selector = 0;

        /*
        ** Scrolling text variables
        *********************************/
        if (folder->ItemsCount() > 0 && event.key.code != Key::Touchpad && (event.type < Event::TouchBegan || event.type > Event::TouchSwipped))
        {
            item = folder->_items[_selector];
            _selectedTextSize = Renderer::GetTextSize(item->name.c_str());
            _maxScrollOffset = static_cast<float>(_selectedTextSize) - 200.f;
            _scrollClock.Restart();
            _scrollOffset = 0.f;
            _reverseFlow = false;
        }
        else if (folder->ItemsCount() == 0)
        {
            _selectedTextSize = 0;
            _AddFavoriteBtn.SetState(false);
            _AddFavoriteBtn.Enable(false);
            _InfoBtn.Enable(false);
            _InfoBtn.SetState(false);
            _keyboardBtn.Enable(false);
            _keyboardBtn.SetState(false);
        }

        /*
        ** Update icon buttons state
        **************************/
        if (folder->ItemsCount() > 0 && _selector < static_cast<int>(folder->ItemsCount()))
        {
            item = folder->_items[_selector];

            // Update favorite icon
            _AddFavoriteBtn.SetState(item->_IsStarred());

            if (last != item)
            {
                // Toggle the keyboard icon
                if (item->_type == MenuType::Entry)
                {
                    MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl *>(item);
                    _keyboardBtn.Enable(e->MenuFunc != nullptr);
                }
                else
                    _keyboardBtn.Enable(false);

                last = item;

                if (!ShowNoteBottom)
                {
                    // Toggle the info button
                    if (item->GetNote().size() > 0)
                    {
                        _noteTB.Update(item->name, item->GetNote());
                        _InfoBtn.Enable(true);
                    }
                    else
                        _InfoBtn.Enable(false);
                }
                else
                {
                    _noteTB.Update(item->firstName, item->GetNote());
                }
            }
        }
    }

    //###########################################
    // Render Menu
    //###########################################

    void PluginMenuHome::_RenderTop(void)
    {
        const Color &selected = Preferences::Settings.MenuSelectedItemColor;
        const Color &unselected = Preferences::Settings.MenuUnselectedItemColor;
        const Color &maintext = Preferences::Settings.MainTextColor;

        int posY = 25;
        int posX = 40;


        // Draw background
        Window::TopWindow.Draw();

        MenuFolderImpl* folder = _starMode ? _starred : _folder;

        // Draw Title
        int maxWidth = _showVersion ? _versionPosX - 10 : 360;
        int posYbak = posY;
        int width = Renderer::DrawSysString(folder->name.c_str(), posX, posY, maxWidth, maintext);
        Renderer::DrawLine(posX, posY, width, maintext);
        posY += 7;

        if (_showVersion && !_starMode && !folder->HasParent())
            Renderer::DrawSysString(_versionStr.c_str(), _versionPosX, posYbak, 360, maintext);

        // Draw Entry
        u32  drawSelector = SelectableEntryCount(*folder);
        int max = folder->ItemsCount();
        if (max == 0)
            return;
        int i = std::max(0, _selector - 6);
        max = std::min(max, (i + 8));

        for (; i < max; i++)
        {
            MenuItem    *item = folder->_items[i];
            ItemFlags   flags = item->Flags;
            const char  *name = item->name.c_str();
            const Color  &fg = i == _selector ? selected : unselected;
            float       offset = i == _selector ? _scrollOffset : 0.f;

            // Draw separator if needed
            if (flags.useSeparatorBefore)
            {
                if (flags.useStippledLineForBefore)
                    Renderer::DrawStippledLine(posX, posY - 1, 320, unselected, 1);
                else
                    Renderer::DrawLine(posX, posY - 1, 320, unselected, 1);
            }

            // Draw cursor
            if (drawSelector && i == _selector)
                Renderer::MenuSelector(posX - 5, posY - 3, 330, 20);

            // Draw entry
            if (item->_type == MenuType::Entry)
            {
                MenuEntryImpl   *entry = reinterpret_cast<MenuEntryImpl *>(item);

                if (entry->GameFunc != nullptr)
                    Renderer::DrawSysCheckBox(name, posX, posY, 350, fg, entry->IsActivated(), offset);
                else
                {
                    if (entry->MenuFunc != nullptr && !entry->_flags.isUnselectable)
                        Icon::DrawSettings(posX, posY);

                    Renderer::DrawSysString(name, posX + 20, posY, 350, fg, offset);
                    posY += 1;
                }
            }
            // Draw folder
            else
            {
                Renderer::DrawSysFolder(name, posX, posY, 350, fg, offset);
            }

            // Draw separator if needed
            if (flags.useSeparatorAfter)
            {
                if (flags.useStippledLineForAfter)
                    Renderer::DrawStippledLine(posX, posY - 1, 320, unselected, 1);
                else
                    Renderer::DrawLine(posX, posY - 1, 320, unselected, 1);
            }
            posY += 4;
        }
    }

    //###########################################
    // Render Bottom Screen
    //###########################################

    void PluginMenuHome::_RenderBottom(void)
    {
        const Color& blank = Color::White;
        static Clock creditClock;
        static bool framework = true;

        Renderer::SetTarget(BOTTOM);

        Window::BottomWindow.Draw();

        int posY = 205;

        if (framework)
            Renderer::DrawString(g_ctrpfText, g_textXpos[0], posY, blank);
        else
            Renderer::DrawString(g_copyrightText, g_textXpos[1], posY, blank);

        if (creditClock.HasTimePassed(Seconds(5)))
        {
            creditClock.Restart();
            framework = !framework;
        }

        posY = 35;

        // Draw buttons
        if (ShowNoteBottom)
        {
            _noteTB.Draw();
        }
        else
        {
            _showStarredBtn.Draw();
            _hidMapperBtn.Draw();
            _gameGuideBtn.Draw();
            _searchBtn.Draw();
            _arBtn.Draw();
            _AddFavoriteBtn.Draw();
            _InfoBtn.Draw();
            _keyboardBtn.Draw();
            _controllerBtn.Draw();
        }
        _toolsBtn.Draw();
    }

    //###########################################
    // Update menu
    //###########################################
    void PluginMenuHome::_Update(Time delta)
    {
        /*
        ** Scrolling
        *************/

        if (_selectedTextSize >= 280 && _scrollClock.HasTimePassed(Seconds(2)))
        {
            if (!_reverseFlow && _scrollOffset < _maxScrollOffset)
            {
                _scrollOffset += 29.f * delta.AsSeconds();
            }
            else
            {
                _scrollOffset -= 55.f * delta.AsSeconds();
                if (_scrollOffset <= 0.0f)
                {
                    _reverseFlow = false;
                    _scrollOffset = 0.f;
                    _scrollClock.Restart();
                }
                else
                    _reverseFlow = true;
            }
        }

        /*
        ** Buttons
        *************/

        // Buttons visibility

        MenuFolderImpl *folder = _starMode ? _starred : _folder;

        if (folder && (*folder)[_selector])
        {
            // If current folder is empty
            if (folder->ItemsCount() == 0 && !ShowNoteBottom)
            {
                _AddFavoriteBtn.Enable(false);
                _InfoBtn.Enable(false);
                _keyboardBtn.Enable(false);
                _controllerBtn.Enable(false);
            }
            // If selected object is a folder
            else if ((*folder)[_selector]->IsFolder())
            {
                MenuFolderImpl *e = reinterpret_cast<MenuFolderImpl *>((*folder)[_selector]);

                if (!ShowNoteBottom)
                {
                    // A folder will not have a menufunc
                    _keyboardBtn.Enable(false);
                    // Check if folder has a note
                    _InfoBtn.Enable(e->note.size());
                    // Enable AddFavorites icon
                    _AddFavoriteBtn.Enable(true);
                    _AddFavoriteBtn.SetState(e->_IsStarred());
                    _controllerBtn.Enable(false);
                }
                if (e->HasNoteChanged())
                {
                    _noteTB.Update(e->firstName, e->GetNote());
                    e->HandledNoteChanges();
                }
            }

            // If selected object is an entry
            else if ((*folder)[_selector]->IsEntry())
            {
                MenuEntryImpl   *e = reinterpret_cast<MenuEntryImpl *>((*folder)[_selector]);
                std::string     &note = e->GetNote();

                if (!ShowNoteBottom)
                {
                    // Check if entry has a menu func
                    _keyboardBtn.Enable(e->MenuFunc != nullptr);
                    // Check if entry has a note
                    _InfoBtn.Enable(note.size());
                    // Enable AddFavorites icon
                    _AddFavoriteBtn.Enable(true);
                    _AddFavoriteBtn.SetState(e->_IsStarred());
                    // Enable controller icon
                    _controllerBtn.Enable(e->_owner != nullptr && e->_owner->Hotkeys.Count() > 0);
                }
                if (e->HasNoteChanged())
                {
                    _noteTB.Update(e->firstName, note);
                    e->HandledNoteChanges();
                }
            }
            // An error is happening
            else
            {
                _AddFavoriteBtn.Enable(false);
                _InfoBtn.Enable(false);
                _keyboardBtn.Enable(false);
                _controllerBtn.Enable(false);
            }
        }
        // Buttons status
        bool isTouched = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        if (!ShowNoteBottom)
        {
            // Update buttons
            _showStarredBtn.Update(isTouched, touchPos);
            //_hidMapperBtn.Update(isTouched, touchPos);
            _gameGuideBtn.Update(isTouched, touchPos);
            _searchBtn.Update(isTouched, touchPos);
            _arBtn.Update(isTouched, touchPos);
            _AddFavoriteBtn.Update(isTouched, touchPos);
            _InfoBtn.Update(isTouched, touchPos);
            _keyboardBtn.Update(isTouched, touchPos);
            _controllerBtn.Update(isTouched, touchPos);
        }
        _toolsBtn.Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }

    void PluginMenuHome::_TriggerEntry(void)
    {
        MenuFolderImpl* folder = _starMode ? _starred : _folder;


        if (_selector >= static_cast<int>(folder->ItemsCount()))
            return;

        MenuItem* item = folder->_items[_selector];

        /*
        ** MenuEntryImpl
        **************/
        if (item->_type == MenuType::Entry)
        {
            MenuEntryImpl* entry = reinterpret_cast<MenuEntryImpl *>(item);

            if (entry->_flags.isUnselectable)
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                return;
            }

            // If the entry has a valid funcpointer
            if (entry->GameFunc != nullptr)
            {
                // Change the state
                bool just = entry->_flags.justChanged;
                bool state = entry->_TriggerState();

                // If is activated add to executeLoop
                if (state)
                {
                    SoundEngine::PlayMenuSound(SoundEngine::Event::SELECT);
                    PluginMenuExecuteLoop::Add(entry);
                }
                else if (just)
                {
                    SoundEngine::PlayMenuSound(SoundEngine::Event::DESELECT);
                    PluginMenuExecuteLoop::Remove(entry);
                }
            }
            else if (entry->MenuFunc != nullptr)
            {
                SoundEngine::PlayMenuSound(SoundEngine::Event::ACCEPT);
                entry->MenuFunc(entry->_owner);
            }
        }
        /*
        ** MenuFolderImpl
        ****************/
        else
        {
            MenuFolderImpl* p = reinterpret_cast<MenuFolderImpl *>(item);

            // If a MenuFolder exists and has a callback
            if (p->_owner != nullptr && p->_owner->OnAction != nullptr)
            {
                // If the callabck tells us to not open the folder
                if (!(p->_owner->OnAction(*p->_owner, ActionType::Opening)))
                {
                    SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
                    return;
                }
            }
            SoundEngine::PlayMenuSound(SoundEngine::Event::ACCEPT);
            p->_Open(folder, _selector, _starMode);
            if (_starMode)
                _starred = p;
            else
                _folder = p;
            _selector = 0;
        }
    }

    void    PluginMenuHome::_showStarredBtn_OnClick(void)
    {
        static int bak = 0;
        std::swap(bak, _selector);
        _starMode = !_starMode;

        MenuFolderImpl* f = _starMode ? _starred : _folder;
        if (f->ItemsCount() == 0)
        {
            _InfoBtn.Enable(false);
            _AddFavoriteBtn.Enable(false);
            _keyboardBtn.Enable(false);
            _selectedTextSize = 0;
        }
        else
        {
            MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl *>(f->_items[_selector]);
            _InfoBtn.Enable(e->note.size() > 0);
            _keyboardBtn.Enable(e->MenuFunc != nullptr);
            _AddFavoriteBtn.Enable(true);
            _AddFavoriteBtn.SetState(e->_IsStarred());

            // Update entry infos
            _selectedTextSize = Renderer::GetTextSize(e->name.c_str());
            _maxScrollOffset = static_cast<float>(_selectedTextSize) - 200.f;
            _scrollClock.Restart();
            _scrollOffset = 0.f;
            _reverseFlow = false;
        }
    }

    void    PluginMenuHome::_controllerBtn_OnClick(void)
    {
        MenuFolderImpl* f = _starMode ? _starred : _folder;
        MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl *>(f->_items[_selector]);
        MenuEntry *entry = e->_owner;

        if (entry != nullptr)
        {
            if (entry->Hotkeys.Count() == 1)
            {
                entry->Hotkeys[0].AskForKeys();
                if (entry->Hotkeys._callback != nullptr)
                    entry->Hotkeys._callback(entry, 0);
                entry->RefreshNote();
            }
            else if (entry->Hotkeys.Count())
            {
                entry->Hotkeys.AskForKeys();
            }
        }
    }

    void    PluginMenuHome::_keyboardBtn_OnClick(void)
    {
        MenuFolderImpl *f = _starMode ? _starred : _folder;
        MenuEntryImpl *e = reinterpret_cast<MenuEntryImpl *>((*f)[_selector]);

        if (e->MenuFunc != nullptr)
            e->MenuFunc(e->_owner);
    }

    void PluginMenuHome::_actionReplayBtn_OnClick()
    {
        _mode = 4;
    }

    void    PluginMenuHome::_gameGuideBtn_OnClick(void)
    {
        _mode = 2;
    }

    void    PluginMenuHome::_searchBtn_OnClick(void)
    {
        _mode = 3;
    }

    void    PluginMenuHome::_toolsBtn_OnClick(void)
    {
        _mode = 5;
    }

    void    PluginMenuHome::_InfoBtn_OnClick(void)
    {
        if (_noteTB.IsOpen())
            _noteTB.Close();
        else
            _noteTB.Open();
    }

    void PluginMenuHome::_StarItem(void)
    {
        MenuFolderImpl* folder = _starMode ? _starred : _folder;

        if (_selector >= static_cast<int>(folder->ItemsCount()))
            return;

        MenuItem* item = folder->_items[_selector];

        if (item)
        {
            bool star = item->_TriggerStar();

            if (star)
                _starredConst->Append(item, true);
            else
            {
                UnStar(item);
            }
        }
    }

    void PluginMenuHome::UnStar(MenuItem* item)
    {
        MenuFolderImpl* folder = _starMode ? _starred : _folder;

        if (item != nullptr)
        {
            item->Flags.isStarred = false;

            int count = _starredConst->ItemsCount();

            if (count == 1)
            {
                _starredConst->_items.clear();
            }
            else
            {
                for (int i = 0; i < count; i++)
                {
                    MenuItem* it = _starredConst->_items[i];

                    if (it == item)
                    {
                        _starredConst->_items.erase(_starredConst->_items.begin() + i);
                        break;
                    }
                }
            }
            if (_selector >= static_cast<int>(folder->ItemsCount()))
                _selector = std::max((int)folder->ItemsCount() - 1, 0);
        }
    }

    void    PluginMenuHome::Init(void)
    {
        MenuFolderImpl* folder = _starMode ? _starred : _folder;
        MenuItem    *item = folder->ItemsCount() != 0 ? folder->_items[0] : nullptr;

        // Init buttons state
        _AddFavoriteBtn.Enable(folder->ItemsCount() != 0);
        _InfoBtn.Enable(item != nullptr ? !item->GetNote().empty() : false);
    }

    void    PluginMenuHome::AddPluginVersion(u32 version)
    {
        char buffer[100];

        sprintf(buffer, "[%d.%d.%d]", static_cast<int>(version & 0xFF), static_cast<int>((version >> 8) & 0xFF), static_cast<int>(version >> 16));
        _versionStr.clear();
        _versionStr = buffer;

        float width = Renderer::GetTextSize(buffer);

        _versionPosX = 360 - (width + 1);
        _showVersion = true;
    }

    void    PluginMenuHome::Close(MenuFolderImpl *folder)
    {
        if (folder != _root)
        {
            if(_folder == folder)
                _folder = _folder->_Close(_selector, false);
            if (_starred == folder)
                _starred = _starred->_Close(_selector, true);
        }
    }

    void    PluginMenuHome::UpdateNote(void)
    {
        if (!ShowNoteBottom)
            return;

        MenuFolderImpl* folder = _starMode ? _starred : _folder;

        if (!folder || !((*folder)[_selector]))
            return;

        if ((*folder)[_selector]->IsFolder())
        {
            MenuFolderImpl* e = reinterpret_cast<MenuFolderImpl*>((*folder)[_selector]);
            if (e->HasNoteChanged())
            {
                _noteTB.Update(e->firstName, e->GetNote());
                e->HandledNoteChanges();
            }
        }
        else if ((*folder)[_selector]->IsEntry())
        {
            MenuEntryImpl* e = reinterpret_cast<MenuEntryImpl*>((*folder)[_selector]);
            if (e->HasNoteChanged())
            {
                _noteTB.Update(e->firstName, e->GetNote());
                e->HandledNoteChanges();
            }
        }
    }
}
