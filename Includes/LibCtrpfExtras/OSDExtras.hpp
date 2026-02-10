#pragma once

#include <CTRPluginFramework.hpp>
#include "TextID.hpp"
#include "Language.hpp"
#include "LibCtrpfExtras/ScreenExtras.hpp"

namespace CTRPluginFramework {
    class OSDExtras : public OSD {
    public:
        struct  OSDMessage {
            std::string     text;
            int             width;
            bool			drawn;
            Color           foreground;
            Color           background;
            Clock           time;

            OSDMessage(const std::string &str, const Color &fg, const Color &bg) {
                text = str;
                width = ScreenExtras::SystemFontSize(text.c_str());
                drawn = false;
                foreground = fg;
                background = bg;
                time = Clock();
            }
        };

        static RecursiveLock RecLock;
        static std::vector<OSDMessage*> Notifications;

        static int     Notify(const TextID &textID, const Color &foreground = Color::White, const Color &background = Color::Black);
        static int     Notify(const std::string &text, const Color &foreground = Color::White, const Color &background = Color::Black);

        static void Lock(void);
        static void Unlock(void);
        static bool TryLock(void);
        static void Update(void);
        static bool Draw(const Screen &screen);

        static bool UpdateOSDSystem(const Screen &screen);
    };
}