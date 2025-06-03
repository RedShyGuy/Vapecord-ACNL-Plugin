#ifndef CTRPLUGINFRAMEWORK_KEYBOARD_HPP
#define CTRPLUGINFRAMEWORK_KEYBOARD_HPP

#include "CTRPluginFramework/Graphics/CustomIcon.hpp"
#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFramework/Graphics/Render.hpp"
//vapecord
#include "CTRPluginFramework/System/Time.hpp"

#include "CTRPluginFramework/Sound.hpp"
#include "types.h"
#include <string>
#include <vector>
#include <memory>

namespace CTRPluginFramework
{
    class KeyboardEvent
    {
    public:
        enum EventType
        {
            CharacterAdded,
            CharacterRemoved,
            InputWasCleared,
            SelectionChanged,
            KeyPressed,
            KeyDown,
            KeyReleased,
            FrameTop,
            FrameBottom
        };

        EventType   type{};       ///< Type of the event
        u32         codepoint{0};  ///< The codepoint of the character that thrown the event (used for CharacterAdded and CharacterRemoved, 0 otherwise)
        s32         selectedIndex{-1}; ///< The entry index in a custom keyboard being selected (used for SelectionChanged, -1 otherwise)
        Key         affectedKey{(Key)0}; ///< Button affected not mapped to any keyboard feature (used for ButtonPressed, ButtonHold and ButtonReleased, 0 otherwise)
        Render::Interface* renderInterface{nullptr}; ///< Interface to render to the screen (used for FrameTop and FrameBottom, nullptr otherwise)
    };

    class KeyboardImpl;
    class Keyboard
    {
        /**
         * \brief The signature of the callback called to check the input
         * \param input     A void pointer to the input, must be casted to the correct type (as passed to Keyboard::Open)
         * \param error     A reference to the string that hold the error message. You can edit it according to what you want to display
         * \return A boolean \n Return true if the input is valid, false if it's not
         */
        using   CompareCallback = bool (*)(const void *, std::string&);

        /**
         * \brief The signature of the callback called when the input change (user enter / delete a character)
         * \param keyboard  A reference to the Keyboard object that called the callback
         * \event event     The event that caused the input to change
         */
        using   OnEventCallback = void(*)(Keyboard&, KeyboardEvent &event);
        
        //vapecord
        using   FrameCallback = void (*)(Time);
        
    public:
        /**
         * @brief List of pages that the QWERTY keyboard can show.
         */
        enum class QwertyKeyboardPage {
            LETTERS_LOWER, ///< Letters lowercase
            LETTERS_UPPER, ///< Letters uppercase
            SYMBOLS_PAGE1, ///< Numbers and punctuation page 1
            SYMBOLS_PAGE2, ///< Numbers and punctuation page 2
            NINTENDO_PAGE1, ///< Nintendo only symbols page 1
            NINTENDO_PAGE2, ///< Nintendo only symbols page 2
        };

        /**
         * @brief The special code points used by control keys.
         * Useful with SetBlockedKeys/SetAllowedKeys.
         */
        struct SpecialCodePoints {
            enum {
                KEY_ENTER = 0xA,            ///< Enter key
                KEY_BACKSPACE = 0x8,        ///< Backspace key
                KEY_SYMBOLS = -2,           ///< Symbols and numbers switch
                KEY_CAPS = -3,              ///< Caps switch
                KEY_SMILEY = -4,            ///< Nintendo symbols switch
                KEY_SPACE = -5,             ///< Space key
                KEY_SYMBOLS_PAGE = -6,      ///< Symbols page change switch
                KEY_NINTENDO_PAGE = -7,     ///< Nintendo symbols change switch
                KEY_PLUS_MINUS = -8,        ///< Plus-minus key
            };
        };

        /**
         * \brief Keyboard constructor
         * \param text  The message to display on the top screen if displayed
         */
        Keyboard(const std::string &text = "");
        Keyboard(const std::string &text, const std::vector<std::string> &options);
        Keyboard(const std::vector<std::string> &options);
        ~Keyboard(void);

        /**
         * \brief Set if the user can abort the keybord by pressing B.
         * The keyboard can still abort if a sleep event happens.
         * \param canAbort  Whether the user can press B to close the keyboard and abort the current operation
         */
        void    CanAbort(bool canAbort) const;

        /**
         * \brief Define if the input must be hexadecimal or not \n
         * Have no effect for float, double, string
         * \param isHex  If the input must be hexadecimal
         */
        void    IsHexadecimal(bool isHex);

        /**
         * \brief Define a maximum input length for qwerty keyboard
         * \param maxValue The maximum count of characters that the user can type
         */
        void    SetMaxLength(u32 maxValue) const;

        /**
         * \brief Define a callback to check the input \n
         * The callback is called each time the input is changed \n
         * See CompareCallback's description for more infos
         * \param callback The callback that must be called
         */
        void    SetCompareCallback(CompareCallback callback) const;

        /**
         * \brief Define a callback that will be called when the user change the input \n
         * Note that if a CompareCallback is set, CompareCallback is called before OnKeyboardEvent \n
         * See OnEventCallback's description for more infos
         * \param callback
         */
        void    OnKeyboardEvent(OnEventCallback callback) const;
        
        //vapecord
        /**
         * \brief The callback set will be called at each frame rendered while ANY keyboard is open
         * The function will receive the Time elapsed since last frame
         */
        static void    OnNewFrame(FrameCallback callback);

        /**
         * \brief Set the error flag and an error message \n
         * When the error flag is set, the user can't valid the input
         * \param error The error message that must be displayed
         */
        void    SetError(std::string error) const;

        /**
         * @brief Sets the starting page when using the QWERTY keyboard.
         * @see QwertyKeyboardPage
         *
         * @param page The page to start when calling Open.
         */
        void    SetStartingPage(QwertyKeyboardPage page);

        /**
         * @brief Sets the blocked keys.
         *
         * @param blockedKeys The keys to be blocked in the keyboard
         * @example kbd.SetBlockedKeys({'0', '1', 'A', 'a'})
         */
        void    SetBlockedKeys(const std::vector<int>& blockedKeys);

        /**
         * @brief Sets the allowed keys. Providing an empty vector enables all the keys
         *
         * @param allowedKeys The allowed keys to be used in the keyboard
         * @note Control keys, such as enter or backspace are not allowed by default and need to be provided in the vector.
         * @example kbd.SetAllowedKeys({'a', 'b', 'c', SpecialCodePoins::KEY_ENTER})
         */
        void    SetAllowedKeys(const std::vector<int>& allowedKeys);

        /**
         * \brief Selects and scroll to the specified entry, must be called after Populate
         * \param entry Entry index to select, -1 to deselect all.
         */
        void	ChangeSelectedEntry(int entry);

        /**
         * @brief Gets the last selected entry before the keyboard was closed
         *
         * @return Last selected entry, or -1 if none was selected.
         */
        int     GetLastSelectedEntry();

        /**
         * \brief Sets the sound event to play when the entry is activated
         * \param entry Entry index to change the sound event
         * \param soundEvent Event to set for the given entry.
         */
        void	ChangeEntrySound(int entry, SoundEngine::Event soundEvent);

        /**
         * \brief Populate a keyboard with the strings contained in an std::vector
         * \param input  A std::vector that contain a list of strings
         * \param resetScroll  Set to true to reset the scroll position if the list is the same size
         */
        void    Populate(const std::vector<std::string> &input, bool resetScroll = false);

        /**
         * \brief Populate a keyboard with the CustomIcons contained in an std::vector
         * \param input A std::vector that contains a list of CustomIcons (icon size must be 30x30 pixels, otherwise a red cross will be displayed)
         * \param resetScroll  Set to true to reset the scroll position if the list is the same size
         */
        void    Populate(const std::vector<CustomIcon>& input, bool resetScroll = false);

        /**
         * \brief Open a keyboard which is populated with strings
         * \return -1 : user abort / not populated \n
         * -2 : closed by sleep event \n
         * >= 0 : index of the user choice in the vector
         */
        int     Open(void) const;

        /**
         * \brief Open the keyboard and wait for user input
         * \param output Where to place the user's input
         * \return -1 : user abort / error \n
         * -2 : closed by sleep event \n
         * 0 : Success
         */
        int     Open(u8 &output) const;

        /**
         * \brief Open the keyboard and wait for user input
         * \param output Where to place the user's input
         * \param start The keyboard will start with this value as input
         * \return -1 : user abort / error \n
         * -2 : closed by sleep event \n
         * 0 : Success
         */
        int     Open(u8 &output, u8 start) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(u16 &output) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \param start The keyboard will start with this value as input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(u16 &output, u16 start) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(u32 &output) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \param start The keyboard will start with this value as input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(u32 &output, u32 start) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(u64 &output) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \param start The keyboard will start with this value as input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(u64 &output, u64 start) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(float &output) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \param start The keyboard will start with this value as input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(float &output, float start) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(double &output) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \param start The keyboard will start with this value as input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(double &output, double start) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(std::string &output) const;

        /**
        * \brief Open the keyboard and wait for user input
        * \param output Where to place the user's input
        * \param start The keyboard will start with this value as input
        * \return -1 : user abort / error \n
        * -2 : closed by sleep event \n
        * 0 : Success
        */
        int     Open(std::string &output, const std::string &start) const;

        /**
         * \brief Forcefully close the keyboard without any regard to the error flag \n
         * (This can only be called from an OnKeyboardEvent callback)
         */
        void    Close(void) const;

        /**
         * \brief Get a reference to the Keyboard's input string
         * \return A reference to the Keyboard's input string
         */
        std::string     &GetInput(void) const;

        /**
        * \brief Get a reference to the top screen's message string
        * \return A reference to the top screen's message string
        */
        std::string     &GetMessage(void) const;

        /**
         * \brief This property define if the top screen must be displayed or not \n
         * Note that when disabled, errors messages can't be displayed
         */
        bool    DisplayTopScreen;

    private:
        std::unique_ptr<KeyboardImpl>   _keyboard;
        bool                            _hexadecimal;
        mutable bool                    _isPopulated;
    };
}

#endif
