#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFramework/System/Directory.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryImpl.hpp"
#include "CTRPluginFrameworkImpl/Menu/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/SubMenu.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFrameworkImpl/Menu/HexEditor.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <random>
#include "3ds.h"
#include "CTRPluginFramework/Menu/Keyboard.hpp"
#include "Unicode.h"


namespace CTRPluginFramework
{
    std::string Utils::Format(const char* fmt, ...)
    {
        char        buffer[0x100] = { 0 };
        va_list     argList;

        va_start(argList, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, argList);
        va_end(argList);

        return (std::string(buffer));
    }

    std::string Utils::ToHex(u32 x)
    {
        char buf[9] = { 0 };

        sprintf(buf, "%08X", static_cast<unsigned int>(x));

        return (buf);
    }

    std::string     Utils::ToString(float fpval, int precision)
    {
        return (fpval > 999999.f || fpval < -999999.f ? Format(Format("%%.%de", precision).c_str(), fpval) : Format(Format("%%.%df", precision).c_str(), fpval));
    }

    static std::mt19937    g_rng; ///< Engine

    void    InitializeRandomEngine(void)
    {
        // Init the engine with a random seed
        g_rng.seed(svcGetSystemTick());
    }

    u32     Utils::Random(void)
    {
        return (Random(0, 0));
    }

    u32     Utils::Random(u32 min, u32 max)
    {
        if (max == 0)
            max = UINT32_MAX;
        std::uniform_int_distribution<u32>  uniform(min, max);

        return (uniform(g_rng));
    }

    u32     Utils::GetSize(const std::string &str)
    {
        u32     size = str.length();
        u8      buffer[0x100] = { 0 };
        u8      *s = buffer;

        if (!size) return (0);

        std::memcpy(buffer, str.data(), size);

        // Skip UTF8 sig
        if (s[0] == 0xEF && s[1] == 0xBB && s[2] == 0xBF)
            s += 3;

        size = 0;
        while (*s)
        {
            if (*s == 0x18)
            {
                s++;
                continue;
            }

            if (*s == 0x1B)
            {
                s += 4;
                continue;
            }

            u32 code;
            int units = decode_utf8(&code, s);

            if (units == -1)
                break;

            s += units;
            size++;
        }
        return (size);
    }

    static void     ListFolders(MenuFolderImpl &folder, const std::string &filter, bool listFiles)
    {
        std::string     path = folder.note;
        Directory       dir;
        std::vector<std::string>    list;

        // Clear folder if necessary
        if (folder.ItemsCount() > 0)
            folder.Clear();

        int res = Directory::Open(dir, path);

        if (res != 0)
            MessageBox("Error", Utils::Format("%08X - %s", res, path.c_str()))();

        if (path.size() > 1 && path[path.size() - 1] != '/')
            path.append("/");

        if (dir.ListDirectories(list) > 0)
        {
            for (std::string &name : list)
            {
                folder.Append(new MenuFolderImpl(name, path + name));
            }
        }

        if (!listFiles)
            return;

        list.clear();

        if (dir.ListFiles(list, filter) > 0)
        {
            for (std::string &name : list)
            {
                folder.Append(new MenuEntryImpl(name));
            }
        }
    }

    static bool     GetName(std::string &out)
    {
        Keyboard kb;

        return kb.Open(out, out) != -1;
    }

    static int     SDExplorerInternal(std::string &out, const std::string &filter, bool selDirectory)
    {
        const char *    opendir =   FONT_A ": Open folder";
        const char *    selFile =   " / Select file\n";
        const char *    selDir =    "\nStart : Select the current folder\n";
        const char *    commands =  FONT_B ": Close folder\n" \
                                    FONT_X ": Open options";

        Menu            menu("/", "", Icon::DrawFile);
        MenuFolderImpl  &root = *menu.GetRootFolder();
        std::string     footer(opendir);
        u8  *           footer_cstr;

        footer += selDirectory ? selDir : selFile;
        footer += commands;
        footer_cstr = (u8 *)footer.c_str();

        // Ensure the process is paused
        Process::Pause();

        // Use note to store current path
        root.note = "/";

        // List root's items
        ListFolders(root, filter, !selDirectory);

        // Open menu
        int             menuEvent;
        Event           event;
        EventManager    eventManager(EventManager::EventGroups::GROUP_KEYS);
        MenuItem        *item;
        Clock           clock;
        SubMenu         submenu({ "Create folder", "Create file", "Rename", "Delete", "Cancel" });

        do
        {
            menuEvent = Nothing;

            // Process events
            while (eventManager.PollEvent(event))
            {
                if (!submenu.IsOpen())
                    if ((menuEvent = menu.ProcessEvent(event, &item)) != Nothing)
                        break;
                submenu.ProcessEvent(event);

                // Check for start
                if (selDirectory
                    && event.type == Event::KeyPressed
                    && event.key.code == Key::Start)
                {
                    menuEvent = EntrySelected;
                    break;
                }
            }

            // If submenu is closed
            if (!submenu.IsOpen())
            {
                // Check if folder changed
                if (menuEvent == FolderChanged)
                {
                    MenuFolderImpl *folder = reinterpret_cast<MenuFolderImpl *>(item);

                    if (!folder->ItemsCount())
                    {
                        ListFolders(*folder, filter, !selDirectory);
                    }
                }
            }
            // Else check submenu
            else
            {
                // Get menu's action
                menuEvent = submenu();

                MenuFolderImpl *folder = menu.GetFolder();
                std::string     path;
                std::string     name;

                if (menuEvent >= 0)
                {
                    // Get current path
                    path = folder->note;
                    if (path != "/")
                        path += "/";

                    if (menuEvent == 0) // Create folder
                    {
                        if (GetName(name))
                        {
                            Directory::Create(path + name);

                            // Relist folder
                            ListFolders(*folder, filter, !selDirectory);
                        }
                    }
                    if (menuEvent == 1) // Create file
                    {
                        if (GetName(name))
                        {
                            File::Create(path + name);

                            // Relist folder
                            ListFolders(*folder, filter, !selDirectory);
                        }
                    }
                    if (menuEvent == 2) // Rename
                    {
                        item = menu.GetSelectedItem();

                        name = item->name;
                        if (GetName(name))
                        {
                            name.insert(0, path); ///< New path
                            path.append(name); ///< Old path

                            if (item->IsEntry())
                                File::Rename(path, name);
                            else
                                Directory::Rename(path, name);

                            // Relist folder
                            ListFolders(*folder, filter, !selDirectory);
                        }

                    }
                    if (menuEvent == 3) // Delete
                    {
                        item = menu.GetSelectedItem();

                        // Display a little warning
                        {
                            std::string body = "Do you really want to delete the ";

                            body.append(item->IsEntry() ? "file" : "folder");
                            body.append(" : ").append(item->name).append(" ?");

                            MessageBox  msgBox(Color::Orange << "Warning", body, DialogType::DialogYesNo);

                            if (msgBox())
                            {
                                int     res;

                                path += item->name;

                                if (item->IsEntry())
                                    res = File::Remove(path);
                                else
                                    res = Directory::Remove(path);

                                if (R_SUCCEEDED(res))
                                {
                                    MessageBox(Color::LimeGreen << "Info", "Operation succeeded")();

                                    // Relist folder
                                    ListFolders(*folder, filter, !selDirectory);
                                }
                                else
                                    MessageBox(Color::Red << "Error", Utils::Format("Operation failed: %08X", res))();
                            }
                        }
                    }
                    if (menuEvent == 4) menuEvent = MenuClose;// Cancel
                }
            }

            // Render menu
            menu.Update(clock.Restart());
            menu.Draw();

            // Render submenu
            submenu.Draw();

            // Render bottom screen

            Renderer::SetTarget(BOTTOM);

            Window::BottomWindow.Draw();
            int posY = 55;
            Renderer::DrawSysStringReturn(footer_cstr, 50, posY, 300, Preferences::Settings.MainTextColor);

            // Swap buffers
            Renderer::EndFrame();

        } while (menuEvent != EntrySelected && menuEvent != MenuClose);

        if (menuEvent == MenuClose)
        {
            // Release the process
            Process::Play();
            return -1;
        }

        out = menu.GetFolder()->note;

        if (!selDirectory)
        {
            if (out != "/")
                out.append("/");
            MenuItem *entry = menu.GetSelectedItem();
            if (entry)
                out.append(entry->name);
        }

        // Release the process
        Process::Play();
        return 0;
    }

    int     Utils::FilePicker(std::string &out, const std::string &filter)
    {
        return SDExplorerInternal(out, filter, false);
    }

    int     Utils::DirectoryPicker(std::string &out)
    {
        return SDExplorerInternal(out, "", true);
    }

    u32     Utils::OpenInHexEditor(u32 address, HexEditorView view)
    {
        extern HexEditor *__g_hexEditor;

        Event           event;
        EventManager    manager(EventManager::EventGroups::GROUP_KEYS);
        std::vector<Event>  eventList;
        HexEditor& hexEditor = *__g_hexEditor;

        ProcessImpl::Pause(false);
        if (!hexEditor.Goto(address, true))
        {
            Process::Play();
            return address; ///< Invalid address
        }

        hexEditor.SetView(static_cast<u32>(view));

        while (true)
        {
            if (SystemImpl::WantsToSleep())
                break;
            eventList.clear();
            while (manager.PollEvent(event))
                eventList.push_back(event);

            bool exit = hexEditor(eventList);
            Renderer::EndFrame(false);

            if (exit)
                break;
        }

        ProcessImpl::Play(false);
        if (SystemImpl::WantsToSleep())
            SystemImpl::ReadyToSleep();

        return hexEditor.GetCursorAddress();
    }

    u32     Utils::RemoveLastChar(std::string &str)
    {
        u32     size = str.length();
        u8      buffer[0x100] = { 0 };
        u8      *s = buffer;

        if (!size) return (0);

        std::memcpy(buffer, str.data(), size);

        // Skip UTF8 sig
        if (s[0] == 0xEF && s[1] == 0xBB && s[2] == 0xBF)
            s += 3;

        u32 code = 0;
        int units = 0;

        while (*s)
        {
            // End color pattern
            if (*s == 0x18)
            {
                // if it's the last char, remove it along with previous char
                if (!*(s + 1))
                {
                    if (units)
                    {
                        // go back to previous char
                        s -= units;
                    }

                    *s = 0;
                    str = reinterpret_cast<char *>(buffer);
                    return (code);
                }
                // Else just skip it
                s++;
                units++;
                continue;
            }

            // Start color pattern
            if(*s == 0x1B)
            {
                // If it's the last char
                if (*(s + 1) && *(s + 2) && *(s + 3) && !*(s + 4))
                {
                    // remove it along with previous char
                    if (units)
                    {
                        // go back to previous char
                        s -= units;
                    }

                    *s = 0;
                    str = reinterpret_cast<char *>(buffer);
                    return (code);
                }
                // Else skip it
                s += 4;
                units += 4;
                continue;
            }

            units = decode_utf8(&code, s);

            if (units == -1)
                break;
            if (*(s + units))
                s += units;
            else
            {
                *s = 0;
                str = reinterpret_cast<char *>(buffer);
                return (code);
            }
        }
        return (0);
    }

    void Utils::ConvertUTF8ToUTF16(string16& out, const char* utf8str)
    {
        u32 utf32;
        u16 utf16[2+1]; // Size + NULL
        out.reserve(out.size() + strlen(utf8str)); // Output size will be approximately the amount of chars of input
        while (*utf8str)
        {
            int consumed = decode_utf8(&utf32, reinterpret_cast<const uint8_t*>(utf8str));
            if (consumed < 0)
                break;
            utf8str += consumed;
            int created = encode_utf16(utf16, utf32);
            if (created < 0)
                break;
            utf16[created] = 0;
            out.append(utf16);
        }
        out.shrink_to_fit();
    }

    static int __strwlen(const u16* in) {
        int count = 0;
        while(*in++) count++;
        return count;
    }

    void Utils::ConvertUTF16ToUTF8(std::string& out, const u16* utf16str)
    {
        u32 utf32;
        char utf8[4+1]; // Size + NULL
        out.reserve(out.size() + __strwlen(utf16str)); // Output size will be approximately the amount of chars of input
        while (*utf16str)
        {
            int consumed = decode_utf16(&utf32, utf16str);
            if (consumed < 0)
                break;
            utf16str += consumed;
            int created = encode_utf8(reinterpret_cast<uint8_t*>(utf8), utf32);
            if (created < 0)
                break;
            utf8[created] = 0;
            out.append(utf8);
        }
        out.shrink_to_fit();
    }
}
