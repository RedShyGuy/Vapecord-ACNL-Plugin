#include "CTRPluginFrameworkImpl/Menu/PluginMenuActionReplay.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCode.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/MenuEntryActionReplay.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "CTRPluginFrameworkImpl/ActionReplay/ARCodeEditor.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"

namespace CTRPluginFramework
{
    static inline const char *FindFolder(const char *path)
    {
        while (path && *path && (*path != '\\' && *path != '/'))
            ++path;

        if (!path || !*path)
            return nullptr;

        return path;
    }

    static void     ProcessPathString(u8 *out)
    {
        std::string &path = Preferences::CheatsFile;
        const char  *cpath = path.c_str();
        const u32   size = Utils::GetSize(path);

        if (size < 36)
        {
            *out++ = '.';
            if (*cpath != '/')
                *out++ = '/';
            while (*cpath)
                *out++ = *cpath++;

            *out = 0;
            return;
        }

        // Try to cut in folders
        u32 ssize = size;
        const char *cut = cpath;

        while (ssize >= 35)
        {
            // Skip the first '/' character
            ++cut;

            cut = FindFolder(cut);

            if (cut == nullptr)
                break;

            ssize = Utils::GetSize(cut);
        }

        // If we have a cut path which fits
        if (cut != nullptr)
        {
            *out++ = '.';
            *out++ = '/';
            *out++ = '.';
            *out++ = '.';
            while (*cut)
                *out++ = *cut++;
            *out = 0;
            return;
        }

        // Else, just display the end of the filename
        cut = cpath + path.size() - 2 - 34;

        while (*cut)
            *out++ = *cut++;
        *out = 0;
    }

    static PluginMenuActionReplay *__pmARinstance = nullptr;
    PluginMenuActionReplay::PluginMenuActionReplay() :
        _topMenu{ "ActionReplay" },
        _noteBtn(Button::Icon | Button::Toggle, IntRect(90, 30, 25, 25), Icon::DrawInfo),
        _editorBtn(Button::Icon, IntRect(130, 30, 25, 25), Icon::DrawEdit),
        _newBtn(Button::Icon, IntRect(165, 30, 25, 25), Icon::DrawPlus),
        _cutBtn(Button::Icon, IntRect(200, 30, 25, 25), Icon::DrawCut),
        _pasteBtn(Button::Icon, IntRect(200, 30, 25, 25), Icon::DrawClipboard),
        _duplicateBtn(Button::Icon, IntRect(235, 30, 25, 25), Icon::DrawDuplicate),
        _trashBtn(Button::Icon, IntRect(50, 30, 25, 25), Icon::DrawTrash),
        _openFileBtn(0, "Open", IntRect(30, 195, 34, 15)),

        _clipboard{ nullptr },
        _path{0}
    {
        __pmARinstance = this;
        ProcessPathString(_path);

        // Disable buttons
        _editorBtn.Disable();
        _cutBtn.Disable();
        _pasteBtn.Disable();
        _duplicateBtn.Disable();
        _trashBtn.Disable();
    }

    PluginMenuActionReplay::~PluginMenuActionReplay()
    {
        if (_clipboard)
            delete _clipboard;
    }

    void    PluginMenuActionReplay::Initialize(void)
    {
        Task    task([](void *arg)
        {
            ActionReplay_LoadCodes(__pmARinstance->_topMenu.GetFolder());
            return (s32)0;
        });

        // Async job
        task.Start();
    }

    bool    PluginMenuActionReplay::operator()(EventList &eventList, const Time &delta)
    {
        static Task     top([](void *arg)
        {
            static_cast<PluginMenuActionReplay *>(arg)->_topMenu.Draw();

            return static_cast<s32>(0);
        }, this, Task::AppCores);

        // Process events
        _ProcessEvent(eventList);

        // Update components
        _Update(delta);

        // Check if note state must be changed
        if (_noteBtn())
        {
            if (!_topMenu.IsNoteOpen())
            {
                if (!_topMenu.ShowNote())
                    _noteBtn.Disable();
            }
            else
                _topMenu.CloseNote();
        }

        // Check buttons
        if (_editorBtn()) _EditorBtn_OnClick();
        if (_newBtn()) _NewBtn_OnClick();
        if (_cutBtn()) _CutBtn_OnClick();
        if (_pasteBtn()) _PasteBtn_OnClick();
        if (_duplicateBtn()) _DuplicateBtn_OnClick();
        if (_trashBtn()) _TrashBtn_OnClick();
        if (_openFileBtn()) _OpenFileBtn_OnClick();

        // Draw menu on top screen
        top.Start();

        // Draw bottom screen
        _DrawBottom();

        // Wait until rendering on Top Screen is finished
        top.Wait();

        // Return if user want to close the window
        return (Window::BottomWindow.MustClose());
    }

    void    PluginMenuActionReplay::_DrawBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        _noteBtn.Draw();
        _editorBtn.Draw();
        _newBtn.Draw();
        _cutBtn.Draw();
        _pasteBtn.Draw();
        _duplicateBtn.Draw();
        _trashBtn.Draw();

        _openFileBtn.Draw();

        int posX = 30 + 34 + 5;
        int posY = 195;

        Renderer::DrawRect(posX, posY, 220, 15, Color::Gray);
        posY += 3;
        Renderer::DrawString((const char *)_path, posX + 2, posY, Color::Black);
    }

    void    PluginMenuActionReplay::_ProcessEvent(EventList &eventList)
    {
        for (Event &event : eventList)
        {
            // Process top menu's event
            MenuItem *item = nullptr;
            int action = _topMenu.ProcessEvent(event, &item);

            // If user want to exit the current menu
            if (action == MenuEvent::MenuClose)
                Window::BottomWindow.Close();
        }
    }

    void    PluginMenuActionReplay::_Update(const Time &delta)
    {
        _topMenu.Update(delta);
        Window::BottomWindow.Update(Touch::IsDown(), IntVector(Touch::GetPosition()));

        bool        touchIsDown = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());
        MenuItem    *item = _topMenu.GetSelectedItem();

        _noteBtn.Enable(item && !item->note.empty());
        _noteBtn.SetState(_topMenu.IsNoteOpen());
        _noteBtn.Update(touchIsDown, touchPos);

        _editorBtn.Enable(item != nullptr);
        _editorBtn.Update(touchIsDown, touchPos);

        _cutBtn.Enable(item != nullptr && _clipboard == nullptr);
        _pasteBtn.Enable(_clipboard != nullptr);
        _duplicateBtn.Enable(item != nullptr && !item->IsFolder());
        _trashBtn.Enable(item != nullptr);

        _newBtn.Update(touchIsDown, touchPos);
        _cutBtn.Update(touchIsDown, touchPos);
        _pasteBtn.Update(touchIsDown, touchPos);
        _duplicateBtn.Update(touchIsDown, touchPos);
        _trashBtn.Update(touchIsDown, touchPos);
        _openFileBtn.Update(touchIsDown, touchPos);
    }

    static bool ActionReplay_GetInput(std::string &ret)
    {
        Keyboard    keyboard;

        /*keyboard.SetCompareCallback([](const void *in, std::string &error)
        {
            std::string &input = *(std::string *)(in);
            if (input.empty())
                return false;
            ActionReplay_ProcessString(input, false);
            return true;
        });*/

        return keyboard.Open(ret, ret) != -1;
    }

    void    PluginMenuActionReplay::_EditorBtn_OnClick(void)
    {
        MenuItem    *item = _topMenu.GetSelectedItem();
        Keyboard   optKbd;
        std::vector<std::string>    options = { "Name", "Note" };

        if (!item)
            return;
        if (!item->IsFolder())
            options.push_back("Code");

        optKbd.Populate(options);
        int choice = optKbd.Open();
        if (choice >= 0)
        {
            // Name edition
            if (choice == 0)
            {
                ActionReplay_GetInput(item->name);
            }
            // Note edition
            else if (choice == 1)
            {
                ActionReplay_GetInput(item->note);
                ActionReplay_ProcessString(item->note);
            }
            // Code edition
            else if (choice == 2)
            {
                MenuEntryActionReplay *e = reinterpret_cast<MenuEntryActionReplay *>(item);

                // Disable the hard way :p
                PluginMenuExecuteLoop::RemoveAR(e);

                // Edit code
                ARCodeEditor::Edit(e->context);
                e->context.Update();
            }
        }
    }

    void    PluginMenuActionReplay::_NewBtn_OnClick(void)
    {
        Keyboard        kbd("", {"Code", "Folder"});
        MenuFolderImpl  *f = __pmARinstance->_topMenu.GetRootFolder();

        if (f == nullptr)
            return;

        int choice = kbd.Open();

        if (choice == -1)
            return;

        std::string name;

        if (!ActionReplay_GetInput(name))
            return;

        // Create a new code
        if (choice == 0)
        {
            MenuEntryActionReplay *entry = new MenuEntryActionReplay(name);
            f->Append(entry);
        }

        // Create a new folder
        if (choice == 1)
        {
            MenuFolderImpl *folder = new MenuFolderImpl(name);
            f->Append(folder);
        }
    }

    void    PluginMenuActionReplay::_CutBtn_OnClick(void)
    {
        // If clipboard already exists, abort
        if (_clipboard)
            return;
        _clipboard = _topMenu.Pop();
    }

    void    PluginMenuActionReplay::_PasteBtn_OnClick(void)
    {
        if (!_clipboard)
            return;
        _topMenu.Insert(_clipboard);
        _clipboard = nullptr;
    }

    void    PluginMenuActionReplay::_DuplicateBtn_OnClick(void)
    {
        MenuItem *current = _topMenu.GetSelectedItem();

        if (!current || current->IsFolder())
            return;

        MenuEntryActionReplay *old = reinterpret_cast<MenuEntryActionReplay *>(current);
        MenuEntryActionReplay *dup = new MenuEntryActionReplay(current->name, current->note);

        dup->context = old->context;
        _topMenu.Insert(dup);
    }

    void    PluginMenuActionReplay::_TrashBtn_OnClick(void)
    {
        MenuItem *item = _topMenu.GetSelectedItem();

        if (item == nullptr)
            return;

        if (!(MessageBox(Color::Orange << "Warning", "Do you really want to delete: " << item->name, DialogType::DialogYesNo )()))
            return;

        item = _topMenu.Pop();

        delete item;
    }

    void    PluginMenuActionReplay::_OpenFileBtn_OnClick(void)
    {
        if (!__pmARinstance)
            return;

        // Backup current codes
        if (MessageBox(Color::Orange << "Warning", "Do you want to save all changes to current file ?", DialogType::DialogYesNo)())
            SaveCodes();

        std::string newPath;

        if (Utils::FilePicker(newPath, ".txt") == -1)
            return;

        // Set path
        Preferences::CheatsFile = newPath;

        MenuFolderImpl *root = __pmARinstance->_topMenu.GetRootFolder();

        if (root->ItemsCount() > 0)
        {
            if (MessageBox(Color::Orange << "Warning", "Do you want to clear current code list ?", DialogType::DialogYesNo)())
            {
                // Ensure we're at the root of the menu
                __pmARinstance->_topMenu.CloseAll();

                // Delete all codes
                root->Clear();
            }
        }

        // Open file and load codes
        ActionReplay_LoadCodes(root);

        // Update path
        ProcessPathString(_path);
    }

    void    PluginMenuActionReplay::SaveCodes(void)
    {
        if (!__pmARinstance)
            return;

        MenuFolderImpl *folder = __pmARinstance->_topMenu.GetRootFolder();

        // If nothing to save, abort
        if (!folder || folder->ItemsCount() == 0) return;

        // Open a temporary file
        File        file("AR.temp", File::RWC | File::TRUNCATE);
        LineWriter  writer(file);

        if (!file.IsOpen())
            return;

        // Write all codes to the temporary file
        MenuFolderImpl &f = *folder;
        for (u32 i = 0; i < f.ItemsCount(); i++)
            ActionReplay_WriteToFile(writer, f[i]);

        writer.Close();

        // If we're here, it means that everything went fine, so delete old file and rename the temporary file
        ActionReplay_OpenCheatsFile(file, true);

        std::string path = file.GetFullName();
        file.Close();

        File::Remove(path);
        File::Rename("AR.temp", path);
    }

    void    PluginMenuActionReplay::NewARCode(u8 type, u32 address, u32 value)
    {
        if (!__pmARinstance)
            return;

        std::string name;

        if (!ActionReplay_GetInput(name))
            return;

        MenuEntryActionReplay *ar = new MenuEntryActionReplay(name);

        u32 offset = address & 0xFF000000;
        address &= 0xFFFFFF;
        ar->context.codes.push_back(ARCode(0xD3, 0, offset));
        ar->context.codes.push_back(ARCode(type, address, value));
        ar->context.codes.push_back(ARCode(0xD2, 0, 0));

        ar->context.Update();
        MenuFolderImpl *f = __pmARinstance->_topMenu.GetRootFolder();

        if (f)
            f->Append(ar);
        ARCodeEditor::Edit(ar->context);
        ar->context.Update();
    }
}
