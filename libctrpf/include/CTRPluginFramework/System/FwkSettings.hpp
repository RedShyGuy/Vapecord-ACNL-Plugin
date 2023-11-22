#ifndef CTRPLUGINFRAMEWORK_SYSTEM_FWKSETTINGS_HPP
#define CTRPLUGINFRAMEWORK_SYSTEM_FWKSETTINGS_HPP

#include "types.h"
#include "CTRPluginFramework/System/Time.hpp"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFramework/System/Rect.hpp"

namespace CTRPluginFramework
{
    struct PluginHeader ///< Defined by Luma3DS plugin loader
    {
        u32     magic;
        u32     version;
        u32     heapVA;
        u32     heapSize;
        u32     exeSize; // Include sizeof(PluginHeader) + .text + .rodata + .data + .bss (0x1000 aligned too)
        u32     isDefaultPlugin;
        s32*    plgldrEvent; ///< Used for synchronization
        s32*    plgldrReply; ///< Used for synchronization
        u32     reserved[24];
        u32     config[32];
    };

    struct FwkSettings
    {
        // Plugin init options
        u32     ThreadPriority; ///< Pri ority for the main thread of the plugin must be within 0x3E - 0x18 | Default: 0x30
        bool    AllowActionReplay; ///< Enable the Action Replay's handler, if this is set to off, even if the menu allows to create codes, they won't be executed
        bool    AllowSearchEngine; ///< If false then the search engine won't be available | Default: true
        Time    WaitTimeToBoot; ///< Time to wait for the plugin to starts (from when the game will starts) | Default: 5 seconds
        bool    CachedDrawMode; ///< Set to true if your plugin heavily uses OSD to draw to the screen to improve performance (only change it in PatchProcess). | Default: false

        // Controller options
        bool    UseGameHidMemory; ///< Set to true to prevent using hidInit and use the game hid memory and capabilities (only change it in PatchProcess). | Default: false
        bool    AreN3DSButtonsAvailable; ///< Set to false to signal if the N3DS buttons are available (only change it in PatchProcess). | Default: true

        // Sound engine
        bool    TryLoadSDSounds; ///< Determines if the plugin should automatically try to load the sound files from the SD card. | Default: true

        // UI colors
        Color   MainTextColor;  ///< The color of all texts within the plugin | Default: Blank
        Color   WindowTitleColor; ///< The color of all window's titles | Default: Blank
        Color   MenuSelectedItemColor; ///< The color of the text for the selected item | Default: Blank
        Color   MenuUnselectedItemColor; ///< The color of the text for the items not selected | Default: Silver
        Color   BackgroundMainColor; ///< The color of the background | Default: Black
        Color   BackgroundSecondaryColor; ///< The color of the background 2 | Default: black/grey (RGB: 15,  15, 15)
        Color   BackgroundBorderColor; ///< The color of the border around the window | Default: Blank
        float   CursorFadeValue; ///< The value to be used to draw the cursor (Shade: [-1.0f - 0f], Tint: [0.f - 1.f]) | Default: 0.2f

        // Keyboard colors
        struct
        {
            Color   Background; ///< Color of the window's background | Default: Black
            Color   KeyBackground; ///< Color of the key's background | Default: Black
            Color   KeyBackgroundPressed; ///< Color of the key's background while pressed | Default: Silver
            Color   KeyText; ///< Color of the key's text | Default: Blank
            Color   KeyTextPressed; ///< Color of the key's text while pressed | Default: Blank
            Color   KeyTextDisabled; ///< Color of the key's text when disabled | Default: DimGrey
            Color   Cursor; ///< Color of the cursor | Default: Blank
            Color   Input; ///< Color of the input | Default: Blank
        } Keyboard;

        // Custom Keyboard colors
        struct
        {
            Color   BackgroundMain; ///< The color of the background | Default: Black
            Color   BackgroundSecondary; ///< The color of the background 2 | Default: black/grey (RGB: 15,  15, 15)
            Color   BackgroundBorder; ///< The color of the border around the window | Default: Blank
            Color   KeyBackground;  ///< Color of the background of a key | Default: sort of Grey (51, 51, 51)
            Color   KeyBackgroundPressed; ///< Color of the background of a key when it's being touched | Default: Gainsboro
            Color   KeyText; ///< Color of the text of a key | Default: Blank
            Color   KeyTextPressed; ///< Color of the text of a key when being touched | Default: Black
            Color   ScrollBarBackground; ///< Color of the scrollbar's background | Default: Silver
            Color   ScrollBarThumb; ///< Color of the scrollbar's thumb | Default: DimGrey
        } CustomKeyboard;

        enum Alignment_e
        {
            Left,
            Right,
            Center
        };

        enum Screen_e
        {
            Top,
            Bottom
        };

        struct
        {
            Color           DefaultBackground;  ///< Default: Color::Black
            Color           DefaultForeground;  ///< Default: Color::White
            Screen_e        Screen;             ///< Screen used to display the notifications | Default: Top
            UIntRect        Area;               ///< Area on the screen to display the notifications | Default: 0, 0, 400, 240
            u32             Margin;             ///< Space between each notification | Default: 5px
            Alignment_e     Alignment;          ///< Alignment of the notification in the defined area | Default: Right
            Time            LifeSpan;           ///< Duration of a notification | Default: 5s
        } Notifications;

        /**
         * \brief Returns a reference to the FwkSettings instance used by the framework
         * Allows runtime theme edition
         * \return The instance of FwkSettings
         */
        static FwkSettings& Get(void);
        static PluginHeader *Header;

        /**
         * \brief Reset all colors to their default values
         */
        static void         SetThemeDefault(void);

        /**
         * \brief Set the top screen background image from a bmp file in memory
         * \param bmpData Pointer to the BMP file in memory
         * \return 0 if success
         */
        static Result       SetTopScreenBackground(void *bmpData);

        /**
         * \brief Set the bottom screen background image from a bmp file in memory
         * \param bmpData Pointer to the BMP file in memory
         * \return 0 if success
         */
        static Result       SetBottomScreenBackground(void *bmpData);
    };
}

#endif
