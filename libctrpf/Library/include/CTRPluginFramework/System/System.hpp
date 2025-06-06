#ifndef CTRPLUGINFRAMEWORK_SYSTEM_SYSTEM_HPP
#define CTRPLUGINFRAMEWORK_SYSTEM_SYSTEM_HPP

#include "types.h"

namespace CTRPluginFramework
{
    enum class LanguageId
    {
        Japanese = 0,
        English,
        French,
        German,
        Italian,
        Spanish,
        ChineseSimplified,
        Korean,
        Dutch,
        Portugese,
        Russian,
        ChineseTraditional
    };

    class System
    {
    public:

		//vapecord infobar
        /**
         * \brief Gets local friend code
         * \return The local friend code
         */
        static u64 GetFriendCode(void);

        /**
         * \brief Gets current battery percentage
         * \return The batter percentage
         */
        static float GetBatteryPercentage(void);

        /**
         * \brief Check if the current console is a New3DS
         * \return true if the current console is a New3DS,\n false otherwise
         */
        static bool         IsNew3DS(void);

        /**
         * @brief Check if plugin is running on citra emulator
         * \return true if the plugin is running on citra emulator
         */
        static bool         IsCitra(void);

        /**
         * \brief Get the system's language (user defined)
         * \return The language of the system
         */
        static LanguageId   GetSystemLanguage(void);

        /**
         * \brief Get if the 3DS is connected to Internet
         * \return true if connected \n false if not connected
         */
        static bool         IsConnectedToInternet(void);

        /**
         * \brief Check if the cfw is Luma3DS or not\n
         * A version number can be passed and if it's the case the function will return whether the version of the cfw is the same or higher (true) or inferior (false)
         * \param major An optional version
         * \param minor An optional version
         * \param revision An optional version
         * \return If the cfw is Luma3DS or not
         */
        static bool         CfwIsLuma3DS(u8 major = 0, u8 minor = 0, u8 revision = 0);

        /**
         * \brief A callback that will be called in std::abort if set
         */
        static void         (*OnAbort)(void);

        /**
        * \brief Causes a system hardware reboot. Does not return.
        */
        static void     Reboot(void) NORETURN;
    };
}

#endif
