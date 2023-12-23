#ifndef CTRPLUGINFRAMEWORK_MENUENTRYHOTKEYS_HPP
#define CTRPLUGINFRAMEWORK_MENUENTRYHOTKEYS_HPP

#include "types.h"

#include <string>
#include <vector>

namespace CTRPluginFramework
{
    class MenuEntry;
    class Hotkey
    {
    public:

        Hotkey();

        /**
         * \brief Create a new Hotkey
         * \param keys The default keys of this hotkey
         * \param name The name for this hotkey \n
         * Appear on the note and on the Hotkey selector when the user want to edit an hotkey
         */
        Hotkey(u32 keys, const std::string &name);
        ~Hotkey();

        /**
         * \brief Change the keys value of the hotkey
         * \param keys the new keys value of the hotkey
         */
        void    operator=(u32 keys);

        /**
         * \brief Change the name of the hotkey
         * \param name The new name you want to appear on the note / the HotkeyModifier selector
         */
        void    operator=(const std::string &name);

        /**
         * \brief Check if the keys of this Hotkey are currently down
         * \return If the keys are down
         */
        bool    IsDown(void) const;

        /**
         * \brief Check if the keys of this Hotkey are pressed
         * \return If the keys were just being pressed
         */
        bool    IsPressed(void) const;

        /**
         * \brief Display a control for the user to select the keys
         */
        void    AskForKeys(void);

        /**
         * \brief Get the keys value of this Hotkey
         * \return the keys value
         */
        u32     GetKeys(void);

        /**
         * @brief Gets the name of this HotKey
         * \return The hotkey name
         */
        std::string& GetName();

        /**
         * @brief Gets the first name of this HotKey
         * \return The hotkey first name
         */
        std::string& GetFirstName();

        /**
         * \brief Stringify the Hotkey
         * \param withName if the name of the Hotkey must be in the result
         * \return return a string of this Hotkey\n
         * if withName is false then the output will simply be the Key's value: "A + B"
         * if withName is true the output will be: "Hotkey's name : A + B"
         */
        std::string  ToString(bool withName = false) const;

    private:
        friend class HotkeyManager;
        friend class PluginMenuImpl;
        u32 _keys;
        std::string _name;
        std::string _firstName;
    };

    class MenuEntry;
    class HotkeyManager
    {
    public:
        /**
         *  \brief A callback type, the args are:
         *  MenuEntry *: the entry that own the Hotkey
         *  int : the index in the HotkeyManager of the hotkey that changed
         */
        using OnHotkeyChangeClbk = void(*)(MenuEntry*, int);

        HotkeyManager(MenuEntry *owner);
        ~HotkeyManager();

        /**
         * \brief Add an hotkey to the manager
         * \param hotkey The Hotkey to add
         */
        void operator+=(const Hotkey &hotkey);

        /**
         * \brief Return a reference to the Hotkey from the manager
         * \param index The index of the Hotkey you want from the manager
         * \return A reference to the desired Hotkey
         */
        Hotkey &operator[](u32 index);

        /**
         * \brief Return a reference to the Hotkey from the manager
         * \param name The name of the Hotkey you want from the manager
         * \return A reference to the desired Hotkey
         */
        Hotkey &operator[](const std::string& name);

        /**
         * \brief Stringify all Hotkeys
         * \return a string with all the Hotkey as string
         */
        std::string ToString(void);

        /**
         * \brief Display a control that allows the user to select which Hotkey he wants to edit
         */
        void        AskForKeys(void);

        /**
         * \brief Add a callback that be called when an Hotkey will change
         * \param callback The callback to call
         */
        void        OnHotkeyChangeCallback(OnHotkeyChangeClbk callback);

        /**
         * \brief Get how many Hotkeys the manager currently have
         * \return The count of Hotkeys
         */
        u32         Count(void);

        /**
         * \brief Get all hotkeys
         * \return hotkeys
         */
        std::vector<Hotkey> GetHotkeys(void);

    private:
        friend class PluginMenuHome;
        friend class PluginMenuImpl;

        MenuEntry           *_owner;
        OnHotkeyChangeClbk  _callback;
        std::vector<Hotkey> _hotkeys;

    };
}

#endif
