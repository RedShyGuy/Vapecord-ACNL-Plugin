#ifndef CTRPPLUGINFRAMEWORK_PREFERENCES_HPP
#define CTRPPLUGINFRAMEWORK_PREFERENCES_HPP

#include "types.h"
#include "3ds.h"
#include "CTRPluginFramework/System/FwkSettings.hpp"
#include "CTRPluginFrameworkImpl/Graphics/BMPImage.hpp"
#include <vector>

namespace CTRPluginFramework
{
    class Preferences
    {
        #define SETTINGS_VERSION1 SYSTEM_VERSION(1, 0, 0)
        #define SETTINGS_VERSION11 SYSTEM_VERSION(1, 1, 0)
        #define SETTINGS_VERSION12 SYSTEM_VERSION(1, 2, 0)

        #define SETTINGS_VERSION SETTINGS_VERSION11
    public:
        enum
        {
            // Settings
            UseFloatingBtn      = 1,
            AutoSaveCheats      = 1 << 2,
            AutoSaveFavorites   = 1 << 3,
            AutoLoadCheats      = 1 << 4,
            AutoLoadFavorites   = 1 << 5,

            // Misc
            DisplayLoadedFiles  = 1 << 16,
            WriteLoadedFiles    = 1 << 17,
            DrawTouchCursor     = 1 << 18,
            DrawTouchPosition   = 1 << 19,
            ShowTopFps          = 1 << 20,
            ShowBottomFps       = 1 << 21,
        };

        /*struct HeaderV1
        {
            u8      sig[8];
            u32     version;
            u32     pluginVersion;
            u64     size;
            u64     flags;
            u32     hotkeys;
            u32     freeCheatsCount;
            u64     freeCheatsOffset;
            u32     enabledCheatsCount;
            u64     enabledCheatsOffset;
            u32     favoritesCount;
            u64     favoritesOffset;
            u32     hotkeysCount;
            u64     hotkeysOffset;
        } PACKED; */

        struct HeaderV11
        {
            u8      sig[8];
            u32     version;
            u32     pluginVersion;
            u64     size;
            u64     flags;
            u32     hotkeys;
            u64     lcdbacklights;
            u32     enabledCheatsCount;
            u64     enabledCheatsOffset;
            u32     favoritesCount;
            u64     favoritesOffset;
            u32     hotkeysCount;
            u64     hotkeysOffset;
            u32     reserved[100];
        } PACKED;

        using Header = HeaderV11;

        struct EnabledCheats
        {
            u32     uid;
        };

        struct Favorites
        {
            u32     uid;
        };

        struct HotkeysInfos
        {
            u32     uid;        ///< owner of the hotkeys
            u32     count;
            std::vector<u32>   hotkeys;
        };

        struct LCDBacklight
        {
            u16     isEnabled{0};
            u16     value{0};
        };

        static bool         IsEnabled(u64 setting)
        {
            return (Flags & setting) > 0ULL;
        }

        static void         Set(u64 setting)
        {
            Flags |= setting;
        }

        static void         Clear(u64 setting)
        {
            Flags &= ~setting;
        }

        static void         Toggle(u64 setting)
        {
            Flags ^= setting;
        }

        static BMPImage     *topBackgroundImage;
        static BMPImage     *bottomBackgroundImage;

        static u32          MenuHotkeys;
        static u64          Flags;
        static LCDBacklight Backlights[2];
        static FwkSettings  Settings;

        static std::string  CheatsFile;
        static std::string  ScreenshotPath;
        static std::string  ScreenshotPrefix;

        static int          OpenConfigFile(File &file, Header &header);
        static void         LoadSettings(void);
        static void         LoadSavedEnabledCheats(void);
        static void         LoadSavedFavorites(void);
        static void         LoadHotkeysFromFile(void);
        static void         LoadBackgrounds(void);
        static void         UnloadBackgrounds(void);
        static void         WriteSettings(void);
        static void         ApplyBacklight(void);

    private:
        static bool         _cheatsAlreadyLoaded;
        static bool         _favoritesAlreadyLoaded;
        static bool         _bmpCanBeLoaded;

        friend class PluginMenuImpl;

        static void     Initialize(void);
    };
}

#endif
