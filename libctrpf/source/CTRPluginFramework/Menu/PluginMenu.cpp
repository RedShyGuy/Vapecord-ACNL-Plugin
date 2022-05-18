#include "CTRPluginFramework/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"

namespace CTRPluginFramework
{
    static PluginMenu   *g_runningInstance = nullptr;

    PluginMenu::PluginMenu(std::string name, std::string about, u32 menuType) :
        OnFirstOpening{ nullptr }, OnOpening{ nullptr }, OnNewFrame{ nullptr }, _menu(new PluginMenuImpl(name, about, menuType))
    {
    }

    PluginMenu::PluginMenu(std::string name, void *about, DecipherPointer func, u32 menuType) :
        OnFirstOpening{ nullptr }, OnOpening{ nullptr }, OnNewFrame{ nullptr }
    {
        std::string aboutStr = "";
        func(aboutStr, about);

        _menu = std::unique_ptr<PluginMenuImpl>(new PluginMenuImpl(name, aboutStr, menuType));
    }

    PluginMenu::PluginMenu(std::string name, u32 major, u32 minor, u32 revision, std::string about, u32 menuType) :
        OnFirstOpening{ nullptr }, OnOpening{ nullptr }, OnNewFrame{ nullptr }, _menu(new PluginMenuImpl(name, about, menuType))
    {
        u32 version = (major & 0xFF) | ((minor & 0xFF) << 8) | ((revision & 0xFF) << 16);

        _menu->AddPluginVersion(version);
    }

    PluginMenu::~PluginMenu(void)
    {
    }

    void    PluginMenu::Append(MenuEntry *item) const
    {
        if (item == nullptr)
            return;

        MenuEntryImpl *entry = item->_item.get();
        _menu->Append(entry);
    }

    void    PluginMenu::Append(MenuFolder *item) const
    {
        if (item == nullptr)
            return;

        MenuFolderImpl *folder = item->_item.get();
        _menu->Append(folder);
    }

    void    PluginMenu::operator+=(const MenuEntry *entry) const
    {
        if (entry == nullptr)
            return;

        MenuEntryImpl *e = entry->_item.get();
        _menu->Append(e);
    }

    void    PluginMenu::operator+=(const MenuFolder *folder) const
    {
        if (folder == nullptr)
            return;

        MenuFolderImpl *f = folder->_item.get();
        _menu->Append(f);
    }

    void    PluginMenu::operator+=(CallbackPointer callback) const
    {
        if (callback != nullptr)
            _menu->Callback(callback);
    }

    void    PluginMenu::operator-=(CallbackPointer callback) const
    {
        if (callback != nullptr)
            _menu->RemoveCallback(callback);
    }

    void    PluginMenu::Callback(CallbackPointer callback) const
    {
        if (callback != nullptr)
            _menu->Callback(callback);
    }

    int    PluginMenu::Run(void) const
    {
        g_runningInstance = (PluginMenu *)this;

        _menu->OnFirstOpening = OnFirstOpening;
        _menu->OnOpening = OnOpening;
        _menu->OnFrame = OnNewFrame;

        int ret = _menu->Run();

        g_runningInstance = nullptr;

        return (ret);
    }

    std::vector<MenuEntry*> PluginMenu::GetEntryList() const
    {
        return (_menu->GetRoot()->GetEntryList());
    }

    std::vector<MenuFolder*> PluginMenu::GetFolderList() const
    {
        return (_menu->GetRoot()->GetFolderList());
    }

    void    PluginMenu::SetHexEditorState(bool isEnabled) const
    {
        _menu->SetHexEditorState(isEnabled);
    }

	void    PluginMenu::ShowWelcomeMessage(bool showMsg) const
	{
		_menu->ShowWelcomeMessage(showMsg);
	}

    bool    PluginMenu::IsOpen(void)
    {
        return (_menu->IsOpen());
    }

    bool    PluginMenu::WasOpened(void)
    {
        return (_menu->WasOpened());
    }

    PluginMenu  *PluginMenu::GetRunningInstance(void)
    {
        return (g_runningInstance);//(const_cast<PluginMenu &>(*g_runningInstance));
    }

	void PluginMenu::ForceOpen(void)
	{
		PluginMenuImpl::ForceOpen();
	}

    void PluginMenu::ForceClose(void)
    {
        PluginMenuImpl::ForceClose();
    }

    void    PluginMenu::SynchronizeWithFrame(const bool useSync)
    {
        _menu->SyncOnFrame = useSync;
    }

    std::string &   PluginMenu::Title(void)
    {
        return _menu->_home->_root->name;
    }
    std::string &       PluginMenu::ScreenshotPath(void)
    {
        return Screenshot::Path;
    }
    std::string &       PluginMenu::ScreenshotFilePrefix(void)
    {
        return Screenshot::Prefix;
    }

    void                PluginMenu::ScreenshotUpdatePaths(void)
    {
        Screenshot::UpdateFileCount();
    }

    void                PluginMenu::ScreenshotSettings(bool** enabled, u32** hotkey)
    {
        *enabled = &Screenshot::IsEnabled;
        *hotkey = &Screenshot::Hotkeys;
    }

    void                PluginMenu::ScreenshotSetcallback(OnScreenshotCallback callback)
    {
        Screenshot::ScreenshotCallback = callback;
    }

    bool                PluginMenu::IsBackgroundLoaded(bool isTopScreen)
    {
        return isTopScreen ? (Preferences::topBackgroundImage != nullptr) : (Preferences::bottomBackgroundImage != nullptr);
    }

    void                PluginMenu::Reload(std::vector<MenuEntry *> quickmenu) 
    {
        _menu->GetRoot()->Reload(quickmenu);
    }
}
