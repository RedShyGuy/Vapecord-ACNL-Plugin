#include "CTRPluginFrameworkImpl/Menu/HotkeysModifier.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuExecuteLoop.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"

#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Menu/PluginMenu.hpp"
#include "CTRPluginFramework/System/Directory.hpp"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/System/Sleep.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/Utils/StringExtensions.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"

#include <3ds.h>

#include <ctime>
#include <cstring>
#include <cstdio>

#define ALPHA 0

#define TAG_VERSION CTRPF_VERSION_MAJOR "." CTRPF_VERSION_MINOR "." CTRPF_VERSION_BUILD

// Maybe add branch as metadata ?
#if ALPHA
#define VersionStr "CTRPluginFramework Alpha V.0.6.0"
#else
#define VersionStr "CTRPluginFramework Beta V" TAG_VERSION
#endif

namespace CTRPluginFramework
{
    enum Mode
    {
        NORMAL = 0,
        ABOUT,
        HEXEDITOR,
        GWRAMDUMP,
        SCREENSHOT,
        MISCELLANEOUS,
        SETTINGS,
    };

    static int  g_mode = NORMAL;

    PluginMenuTools::PluginMenuTools(std::string &about, HexEditor &hexEditor) :
        _about(about),
        _mainMenu("Tools"),
        _miscellaneousMenu("Miscellaneous"),
        _screenshotMenu("Screenshots"),
        _settingsMenu("Settings"),
        _hexEditorEntry(nullptr),
        _hexEditor(hexEditor),
        _menu(&_mainMenu, nullptr),
        _abouttb("About", _about, IntRect(30, 20, 340, 200)),
        _exit(false)
    {
        InitMenu();
    }

    static void    MenuHotkeyModifier(void)
    {
        u32 keys = Preferences::MenuHotkeys;

        (HotkeysModifier(keys, "Select the hotkeys you'd like to use to open the menu."))();

        if (keys != 0)
            Preferences::MenuHotkeys = keys;
    }

    void    PluginMenuTools::UpdateSettings(void)
    {
        //using MenuItemIter =  MenuFolderImpl::MenuItemIter;

        // Settings
        auto item = _settingsMenu.begin() + 2;

        if (Preferences::IsEnabled(Preferences::UseFloatingBtn)) (*item++)->AsMenuEntryImpl().Enable();
        else (*item++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::AutoSaveCheats)) (*item++)->AsMenuEntryImpl().Enable();
        else (*item++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::AutoSaveFavorites)) (*item++)->AsMenuEntryImpl().Enable();
        else (*item++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::AutoLoadCheats)) (*item++)->AsMenuEntryImpl().Enable();
        else (*item++)->AsMenuEntryImpl().Disable();

        if (Preferences::IsEnabled(Preferences::AutoLoadFavorites)) (*item)->AsMenuEntryImpl().Enable();
        else (*item)->AsMenuEntryImpl().Disable();

        item = _miscellaneousMenu.begin();

        // Misc.


        if (Preferences::IsEnabled(Preferences::DisplayLoadedFiles)) (*item++)->AsMenuEntryTools().Enable();
        else (*item++)->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::WriteLoadedFiles)) (*item++)->AsMenuEntryTools().Enable();
        else (*item++)->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::DrawTouchCursor)) (*item++)->AsMenuEntryTools().Enable();
        else (*item++)->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::DrawTouchPosition)) (*item++)->AsMenuEntryTools().Enable();
        else (*item++)->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::ShowTopFps)) (*item++)->AsMenuEntryTools().Enable();
        else (*item++)->AsMenuEntryTools().Disable();

        if (Preferences::IsEnabled(Preferences::ShowBottomFps)) (*item)->AsMenuEntryTools().Enable();
        else (*item)->AsMenuEntryTools().Disable();
    }

    using   FsTryOpenFileType = u32(*)(u32, u16*, u32);

    enum HookFilesMode
    {
        NONE = 0,
        OSD = 1,
        FILE = 2
    };
    static Hook         g_FsTryOpenFileHook;
    static u32          g_HookMode = NONE;
    //static u32          g_returncode[4];
    static File         g_hookExportFile;
    u32                 g_FsTryOpenFileAddress = 0;
    static LightLock    g_OpenFileLock;

    static u32      FindNearestSTMFD(u32 addr)
    {
        for (u32 i = 0; i < 1024; i++)
        {
            addr -= 4;
            if (*(u16 *)(addr + 2) == 0xE92D)
                return addr;
        }
        return (0);
    }

    static void      FindFunction(u32 &FsTryOpenFile)
    {
        const u8 tryOpenFilePat1[] = { 0x0D, 0x10, 0xA0, 0xE1, 0x00, 0xC0, 0x90, 0xE5, 0x04, 0x00, 0xA0, 0xE1, 0x3C, 0xFF, 0x2F, 0xE1 };
        const u8 tryOpenFilePat2[] = { 0x10, 0x10, 0x8D, 0xE2, 0x00, 0xC0, 0x90, 0xE5, 0x05, 0x00, 0xA0, 0xE1, 0x3C, 0xFF, 0x2F, 0xE1 };

        u32    *addr = (u32 *)0x00100000;
        u32    *maxAddress = (u32 *)(Process::GetTextSize() + 0x00100000);

        while (addr < maxAddress)
        {
            if (!memcmp(addr, tryOpenFilePat1, sizeof(tryOpenFilePat1)) || !memcmp(addr, tryOpenFilePat2, sizeof(tryOpenFilePat2)))
            {
                FsTryOpenFile = FindNearestSTMFD((u32)addr);
                break;
            }
            addr++;
        }
    }

    // TODO: clean this whole code
    static u32      FsTryOpenFileCallback(u32 a1, u16 *fileName, u32 mode);
    static bool     InitFsTryOpenFileHook(void)
    {
        static bool isInitialized = false;

        if (isInitialized)
            return isInitialized;

        /*auto  createReturncode = [](u32 address, u32 *buf)
        {
            Process::CopyMemory(buf, (void *)address, 8);
            buf[2] = 0xE51FF004;
            buf[3] = address + 8;
        };*/

        // Hook on OpenFile
        u32     FsTryOpenFileAddress = 0;

        FindFunction(FsTryOpenFileAddress);

        // Check that we found the function
        if (FsTryOpenFileAddress != 0)
        {
            // Create lock
            LightLock_Init(&g_OpenFileLock);

            // Initialize the return code
            //createReturncode(FsTryOpenFileAddress, g_returncode);

            // Initialize the hook
            g_FsTryOpenFileHook.InitializeForMitm(FsTryOpenFileAddress, (u32)FsTryOpenFileCallback);
            g_FsTryOpenFileAddress = FsTryOpenFileAddress;
            isInitialized = true;
        }
        else
        {
            OSD::Notify("Error: couldn't find OpenFile function");
            // Disable the option
            Preferences::Clear(Preferences::DisplayLoadedFiles);
        }

        return isInitialized;
    }

    static u32      FsTryOpenFileCallback(u32 a1, u16 *fileName, u32 mode)
    {
        u8      buffer[256] = {0};
        std::string str;
        int     units;

        LightLock_Lock(&g_OpenFileLock);

        if (g_HookMode & OSD)
        {
            // Convert utf16 to utf8
            units = utf16_to_utf8(buffer, fileName, 255);

            if (units > 0)
            {
                str = (char *)buffer;
                OSD::Notify(str);
            }
        }

        if (g_HookMode & FILE)
        {
            // Convert utf16 to utf8 if necessary
            if (str.empty())
            {
                units = utf16_to_utf8(buffer, fileName, 255);
                if (units > 0)
                    str = (char *)buffer;
            }

            // If string isn't empty, write to file
            if (!str.empty())
            {
                g_hookExportFile.WriteLine(str);
            }
        }

        LightLock_Unlock(&g_OpenFileLock);

        return HookContext::GetCurrent().OriginalFunction<u32>(a1, fileName, mode);
    }

    static void    _DisplayLoadedFiles(MenuEntryTools *entry)
    {
        // If we must enable the hook
        if (entry->WasJustActivated())
        {
            // Initialize hook
            if (!InitFsTryOpenFileHook())
                entry->Disable(); ///< Hook failed

            // Enable the hook
            Preferences::Set(Preferences::DisplayLoadedFiles);
            g_HookMode |= OSD;
            g_FsTryOpenFileHook.Enable();
            return;
        }
        if (!entry->IsActivated())
        {
            // Disable OSD
            Preferences::Clear(Preferences::DisplayLoadedFiles);
            g_HookMode &= ~OSD;

            // If there's no task to do on the hook, disable it
            if (g_HookMode == 0)
                g_FsTryOpenFileHook.Disable();
        }
    }

    static void    _WriteLoadedFiles(MenuEntryTools *entry)
    {
        // If we must enable the hook
        if (entry->WasJustActivated())
        {
            // Initialize hook
            if (!InitFsTryOpenFileHook())
                entry->Disable(); ///< Hook failed

            // Open the file
            int     mode = File::READ | File::WRITE | File::CREATE | File::APPEND;
            std::string filename = Utils::Format("[%016llX] - LoadedFiles.txt", Process::GetTitleID());

            if (File::Open(g_hookExportFile, filename, mode) != 0)
            {
                OSD::Notify(std::string("Error: couldn't open \"").append(filename).append("\""), Color::Red, Color::White);
                entry->Disable(); ///< Disable the entry
                return;
            }

            static bool firstActivation = true;

            if (firstActivation)
            {
                g_hookExportFile.WriteLine("\r\n\r\n### New log ###\r\n");
                firstActivation = false;
            }

            // Enable the hook
            g_HookMode |= FILE;
            g_FsTryOpenFileHook.Enable();

            return;
        }

        if (!entry->IsActivated())
        {
            // Disable File exporting
            g_HookMode &= ~FILE;
            g_hookExportFile.Flush();
            g_hookExportFile.Close();

            // If there's no task to do on the hook, disable it
            if (g_HookMode == 0)
                g_FsTryOpenFileHook.Disable();
        }
    }

    static bool     ConfirmBeforeProceed(const std::string &task)
    {
        std::string msg = Color::Yellow << "Warning\n\n" << ResetColor() << "Do you really want to " + task + " ?";
        MessageBox  msgBox(msg, DialogType::DialogYesNo);

        return (msgBox());
    }

    static void     Shutdown(void)
    {
        if (ConfirmBeforeProceed("shutdown"))
        {
            srvPublishToSubscriber(0x203, 0);
            ProcessImpl::UnlockGameThreads();
            svcExitThread();
        }
    }

    static void     Reboot(void)
    {
        if (ConfirmBeforeProceed("reboot"))
        {
            svcKernelSetState(7);
            svcExitThread();
        }
    }

    static MenuEntryTools    *g_screenshotEntry;

    static void      GetScreenShotMode(void)
    {
        Keyboard    kb(Color::LimeGreen << "Screenshot settings\x18\n\n Which screen(s) would you like to capture ?", {"Top screen", "Bottom screen", "Both screens"});

        int mode = kb.Open();
        if (mode != -1)
            Screenshot::Screens = mode + 1;
    }

    std::string     KeysToString(u32 keys);
    bool            stou32(std::string &input, u32 &res);

    static bool g_manualScreenshotTrigger = false;
    static void Screenshot_Enabler(MenuEntryTools *entry)
    {
        if (!g_manualScreenshotTrigger)
            Screenshot::IsEnabled = !Screenshot::IsEnabled;
    }

    static void     UpdateScreenshotText(void) {
        // Update entry
        const char *screens[3] = {"Top screen", "Bottom screen", "Both screens"};

        g_screenshotEntry->name = ("Screenshot: " << Color::LimeGreen).append(KeysToString(Screenshot::Hotkeys));
        g_screenshotEntry->name += "\x18, " << Color::Orange;
        g_screenshotEntry->name.append(screens[(Screenshot::Screens & SCREENSHOT_BOTH) - 1]);

        u32 time = static_cast<u32>(Screenshot::Timer.AsSeconds());

        if (time)
        {
            g_screenshotEntry->name += ("\x18, " << Color::DeepSkyBlue << time);
            Screenshot::Screens |= 4; ///< TIMED flags
        }
        g_manualScreenshotTrigger = true;

        if (Screenshot::IsEnabled)
            g_screenshotEntry->Enable();
        else
            g_screenshotEntry->Disable();

        g_manualScreenshotTrigger = false;
    }

    static void     ScreenshotMenuCallback(void)
    {
        Keyboard    kb(Color::LimeGreen << "Screenshot settings\x18\n\nWhat do you want to change ?", { "Screens", "Hotkeys", "Timer", "Name", "Directory" });

        int choice;

        do
        {
            choice = kb.Open();

            switch (choice)
            {
            case 0: ///< Screens
            {
                GetScreenShotMode();
                break;
            }

            case 1: ///< Hotkeys
            {
                u32 keys = Screenshot::Hotkeys;

                (HotkeysModifier(keys, "Select the hotkeys you'd like to use to take a new screenshot."))();

                if (keys != 0)
                    Screenshot::Hotkeys = keys;
                break;
            }

            case 2: ///< Timer
            {
                u32         current = static_cast<u32>(Screenshot::Timer.AsSeconds());
                Keyboard    keyboard(Color::LimeGreen << "Screenshot timer\x18\n\n Enter the amount of seconds you would like\n to continuously take screenshots.\n\n Enter 0 to disable the timer.\n\n"
                            << Color::Orange << " Note that during that time, you can't\n access the menu.");

                keyboard.IsHexadecimal(false);
                keyboard.OnKeyboardEvent([](Keyboard &kb, KeyboardEvent &event)
                {
                    if (event.type == KeyboardEvent::CharacterAdded)
                    {
                        std::string &input = kb.GetInput();
                        u32  value;
                        stou32(input, value);

                        if (value > 120)
                            input = "120";
                    }
                });

                if (keyboard.Open(current, current) != -1)
                    Screenshot::Timer = Seconds(static_cast<float>(current));

                break;
            }

            case 3: ///< Name
            {
                Keyboard    nameKb(Color::LimeGreen << "Screenshot name\x18\n\n Which name would you like for the files ?");
                std::string out;

                if (nameKb.Open(out, Screenshot::Prefix) != -1)
                     Screenshot::Prefix = out;
                break;
            }

            case 4: ///< Directory
            {
                std::string out;

                if (Utils::DirectoryPicker(out) == -1)
                    break;

                Screenshot::Path = std::move(out);
                if (Screenshot::Path[Screenshot::Path.size() - 1] != '/')
                    Screenshot::Path += '/';
                break;
            }

            default:
                break;
            }
        } while (choice != -1);

        UpdateScreenshotText();

        // Update file count
        Screenshot::UpdateFileCount();
    }

    static void EditBacklight(MenuEntryTools *entry)
    {
        using LCDBacklight = Preferences::LCDBacklight;

        int             userchoice;
        u16             backlight;
        ScreenImpl *    screen;
        Keyboard        kb;
        std::string     triggerTop;
        std::string     triggerBottom;
        std::string&    message = kb.GetMessage();

        kb.DisplayTopScreen = true;
        while (true)
        {
            triggerTop = Preferences::Backlights[0].isEnabled ?
                            Color::Red << "Disable for Top"
                          : Color::LimeGreen << "Enable for Top";
            triggerBottom = Preferences::Backlights[1].isEnabled ?
                            Color::Red << "Disable for Bottom"
                          : Color::LimeGreen << "Enable for Bottom";

            message = "Backlight Setter\n\n" \
                      " Select which screen to configure\n\n" \
                      "  - Top screen: " + std::to_string(Preferences::Backlights[0].value) + ", " + (Preferences::Backlights[0].isEnabled ?
                            (std::string)Color::LimeGreen + "Enabled\n"
                          : (std::string)Color::Red + "Disabled\n") + ResetColor();

            message += "  - Bottom screen: " + std::to_string(Preferences::Backlights[1].value) + ", " + (Preferences::Backlights[1].isEnabled ?
                            (std::string)Color::LimeGreen + "Enabled\n"
                          : (std::string)Color::Red + "Disabled\n") + ResetColor();
            message += "\n\n Remember to put the 'Auto-Brightness'\n on OFF in the menu Home.";

            kb.Populate({triggerTop, "Top screen", triggerBottom, "Bottom screen"});
            userchoice = kb.Open();
            if (userchoice == -1)
                return;

            if (userchoice == 0 || userchoice == 2)
            {
                LCDBacklight *  backlights = Preferences::Backlights;

                backlights += userchoice == 2;
                backlights->isEnabled = !backlights->isEnabled;
                continue;
            }

            screen = userchoice == 3 ? ScreenImpl::Bottom : ScreenImpl::Top;
            backlight = screen->GetBacklight();
            kb.IsHexadecimal(false);
            message = "Backlight Setter\n\nSet a value between 2 - 1023\n\nCurrent value: ";
            message += Color::LimeGreen << backlight;

            if (kb.Open(backlight, backlight) != -1)
            {
                backlight = std::max(backlight, static_cast<u16>(2));
                backlight = std::min(backlight, static_cast<u16>(0x3FF));
                Preferences::Backlights[userchoice == 3].value = backlight;
            }
        }
    }

    void    PluginMenuTools::InitMenu(void)
    {
        // Main menu
        _mainMenu.Append(new MenuEntryTools("About", [] { g_mode = ABOUT; }, Icon::DrawAbout));

        _hexEditorEntry = new MenuEntryTools("Hex Editor", [] { g_mode = HEXEDITOR; }, Icon::DrawGrid);
        _mainMenu.Append(_hexEditorEntry);
        _mainMenu.Append(new MenuEntryTools("Gateway RAM Dumper", [] { g_mode = GWRAMDUMP; }, Icon::DrawRAM));
        _mainMenu.Append(new MenuEntryTools("Screenshots", nullptr, Icon::DrawUnsplash, new u32(SCREENSHOT)));
        _mainMenu.Append(new MenuEntryTools("Miscellaneous", nullptr, Icon::DrawMore, new u32(MISCELLANEOUS)));
        _mainMenu.Append(new MenuEntryTools("Settings", nullptr, Icon::DrawSettings, this));
        _mainMenu.Append(new MenuEntryTools("Shutdown the 3DS", Shutdown, Icon::DrawShutdown));
        _mainMenu.Append(new MenuEntryTools("Reboot the 3DS", Reboot, Icon::DrawRestart));

        // Screenshots menu
        _screenshotMenu.Append(new MenuEntryTools("Change screenshot settings", ScreenshotMenuCallback, Icon::DrawSettings));
        _screenshotMenu.Append((g_screenshotEntry = new MenuEntryTools( "Screenshot: " << Color::LimeGreen << KeysToString(Screenshot::Hotkeys)
                                << "\x18, " << Color::Orange << "Both screens", Screenshot_Enabler, true)));

        // Miscellaneous menu
        _miscellaneousMenu.Append(new MenuEntryTools("Display loaded files", _DisplayLoadedFiles, true));
        _miscellaneousMenu.Append(new MenuEntryTools("Write loaded files to file", _WriteLoadedFiles, true));
        _miscellaneousMenu.Append(new MenuEntryTools("Display touch cursor", [] { Preferences::Toggle(Preferences::DrawTouchCursor); }, true, Preferences::IsEnabled(Preferences::DrawTouchCursor)));
        _miscellaneousMenu.Append(new MenuEntryTools("Display touch coordinates", [] { Preferences::Toggle(Preferences::DrawTouchPosition); }, true, Preferences::IsEnabled(Preferences::DrawTouchPosition)));
        _miscellaneousMenu.Append(new MenuEntryTools("Display top screen's fps", [] { Preferences::Toggle(Preferences::ShowTopFps); }, true, Preferences::IsEnabled(Preferences::ShowTopFps)));
        _miscellaneousMenu.Append(new MenuEntryTools("Display bottom screen's fps", [] { Preferences::Toggle(Preferences::ShowBottomFps); }, true, Preferences::IsEnabled(Preferences::ShowBottomFps)));

        // Settings menu
        _settingsMenu.Append(new MenuEntryTools("Change menu hotkeys", MenuHotkeyModifier, Icon::DrawGameController));

        _settingsMenu.Append(new MenuEntryTools("Set backlight (Experimental)", EditBacklight, false, false));
        _settingsMenu.Append(new MenuEntryTools("Use floating button", [] { Preferences::Toggle(Preferences::UseFloatingBtn); }, true, Preferences::IsEnabled(Preferences::UseFloatingBtn)));

        _settingsMenu.Append(new MenuEntryTools("Auto save enabled cheats", [] { Preferences::Toggle(Preferences::AutoSaveCheats); }, true, Preferences::IsEnabled(Preferences::AutoSaveCheats)));
        _settingsMenu.Append(new MenuEntryTools("Auto save favorites", [] { Preferences::Toggle(Preferences::AutoSaveFavorites); }, true, Preferences::IsEnabled(Preferences::AutoSaveFavorites)));
        _settingsMenu.Append(new MenuEntryTools("Auto load enabled cheats at starts", [] { Preferences::Toggle(Preferences::AutoLoadCheats); }, true, Preferences::IsEnabled(Preferences::AutoLoadCheats)));
        _settingsMenu.Append(new MenuEntryTools("Auto load favorites at starts", [] { Preferences::Toggle(Preferences::AutoLoadFavorites); }, true, Preferences::IsEnabled(Preferences::AutoSaveFavorites)));
        _settingsMenu.Append(new MenuEntryTools("Load enabled cheats now", [] { Preferences::LoadSavedEnabledCheats(); }, nullptr));
        _settingsMenu.Append(new MenuEntryTools("Load favorites now", [] { Preferences::LoadSavedFavorites(); }, nullptr));
    }

    bool    PluginMenuTools::operator()(EventList &eventList, Time &delta)
    {
        if (g_mode == HEXEDITOR)
        {
            if (_hexEditor(eventList))
                g_mode = NORMAL;
            return (false);
        }
        //u32 i = Preferences::IsEnabled(Preferences::UseFloatingBtn);
        if (g_mode == ABOUT)
        {
            if (!_abouttb.IsOpen())
                _abouttb.Open();
            else
                g_mode = NORMAL;
        }

        if (g_mode == GWRAMDUMP)
        {
            _gatewayRamDumper();
            g_mode = NORMAL;
            return (false);
        }

        // Process Event
        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        // Update
        _Update(delta);

        static Task task([](void *arg) -> s32
        {
            static_cast<PluginMenuTools *>(arg)->_RenderTop();
            return 0;
        }, static_cast<void *>(this), Task::AppCores);

        // Render Top
        //_RenderTop();
        task.Start();

        // Render Bottom
        _RenderBottom();

        task.Wait();

        // Check buttons
        bool exit = _exit || Window::BottomWindow.MustClose();
        _exit = false;
        return (exit);
    }

    void PluginMenuTools::TriggerHexEditor(bool isEnabled) const
    {
        if (!isEnabled)
        {
            _hexEditorEntry->Hide();
        }
        else
            _hexEditorEntry->Show();
    }

    /*
    ** Process Event
    *****************/
    void    PluginMenuTools::_ProcessEvent(Event &event)
    {
        if (_abouttb.IsOpen())
        {
            _abouttb.ProcessEvent(event);
            if (!_abouttb.IsOpen())
                SoundEngine::PlayMenuSound(SoundEngine::Event::CANCEL);
            return;
        }

        MenuItem    *item = nullptr;
        static int  selector = -1;

        int ret = _menu.ProcessEvent(event, &item);

        if (ret == EntrySelected && item != nullptr)
        {
            void *arg = ((MenuEntryTools *)item)->GetArg();

            if (arg == this)
            {
                selector = _menu._selector;
                _menu.Open(&_settingsMenu);
            }
            else if (arg != nullptr && *(u32 *)arg == MISCELLANEOUS)
            {
                selector = _menu._selector;
                _menu.Open(&_miscellaneousMenu);
            }
            else if (arg != nullptr &&  *(u32 *)arg == SCREENSHOT)
            {
                selector = _menu._selector;
                UpdateScreenshotText();
                _menu.Open(&_screenshotMenu);
            }
        }

        if (ret == MenuClose)
        {
            //MenuFolderImpl *cur = _menu.GetFolder();

            if (_menu.GetFolder() == &_mainMenu)
            {
                _exit = true;
                _menu.Open(&_mainMenu, selector);
            }
            else
                _menu.Open(&_mainMenu, selector);
        }
    }

    void PluginMenuTools::_RenderTopMenu(void)
    {

    }

    /*
    ** Render Top
    **************/

    void    PluginMenuTools::_RenderTop(void)
    {
        // Enable renderer
        Renderer::SetTarget(TOP);

        if(_abouttb.IsOpen())
        {
            _abouttb.Draw();
            return;
        }

      /*  // Window
        Window::TopWindow.Draw();

        // Title
        int posY = 25;
        int xx = Renderer::DrawSysString("Tools", 40, posY, 330, Color::Blank);
        Renderer::DrawLine(40, posY, xx, Color::Blank);*/

        _menu.Draw();
    }

    /*
    ** Render Bottom
    *****************/
    void    PluginMenuTools::_RenderBottom(void)
    {
        //const Color    &black = Color::Black;
        const Color    &blank = Color::White;
        //const Color    &dimGrey = Color::BlackGrey;

        // Enable renderer
        Renderer::SetTarget(BOTTOM);

        // Window
        Window::BottomWindow.Draw();

        // Draw Framework version
        {
            //static const char *version = VersionStr;
            static const char *tagVersion = TAG_VERSION;
            static const char *commit = COMMIT_HASH;
            static const char *compilationDate = COMPILE_DATE;

            int posY = 30, posYY = 50;
            Renderer::DrawString("CTRPF Build:",  30, posY, blank);
            Renderer::DrawLine(30, posY, 12 * 6, blank); posY += 10;
            Renderer::DrawString("Version: ",  30, posY, blank);    Renderer::DrawString(tagVersion,  100, posYY, blank);
            Renderer::DrawString("Commit: ",  30, posY, blank);     Renderer::DrawString(commit,  100, posYY, blank);
            Renderer::DrawString("Compiled: ",  30, posY, blank);   Renderer::DrawString(compilationDate,  100, posYY, blank);


            /*
            static const u32 xpos = (320 - Renderer::LinuxFontSize(version)) / 2;

            int posY = 205;
            Renderer::DrawString(version, xpos, posY, blank);*/
        }
    }

    /*
    ** Update
    ************/
    void    PluginMenuTools::_Update(Time delta)
    {
        /*
        ** Buttons
        *************/
        bool        isTouched = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
