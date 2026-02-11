#include <CTRPluginFramework.hpp>
#include "LibCtrpfExtras/OSDExtras.hpp"

namespace CTRPluginFramework {
    RecursiveLock OSDExtras::RecLock;
    std::vector<OSDExtras::OSDMessage*> OSDExtras::Notifications;

    void OSDExtras::Lock(void) {
        RecursiveLock_Lock(&RecLock);
    }

    void OSDExtras::Unlock(void) {
        RecursiveLock_Unlock(&RecLock);
    }


    bool OSDExtras::TryLock(void) {
        return (RecursiveLock_TryLock(&RecLock));
    }

    void OSDExtras::Update(void) {
        if (TryLock()) {
            return;
        }

        if (Notifications.size() && Notifications.front()->drawn)
            Notifications.erase(std::remove_if(Notifications.begin(), Notifications.end(),
                [](OSDMessage *message) {
                    bool remove = message->drawn && message->time.HasTimePassed(Seconds(5.f));
                    if (remove) {
                        delete message;
                    }
                    return remove;
                }),
                Notifications.end());

        Unlock();
    }

    bool OSDExtras::Draw(const Screen &screen) {
        if (Notifications.empty()) {
            return false;
        }

        if (!screen.IsTop) {
            return false;
        }

        constexpr int lineHeight = 16;
        constexpr int spacingY = 3;
        constexpr int spacingX = 2;
        constexpr int bottomY = 233;
        constexpr int endX = 390;

        int posX;
        int posY = bottomY - lineHeight;

        int count = 0;

        for (OSDMessage *message : Notifications) {
            posX = endX - message->width;

            screen.DrawRect(posX - spacingX, posY, message->width + (spacingX * 2), lineHeight, message->background, true);
            screen.DrawSysfont(message->text.c_str(), posX, posY, message->foreground);
            posY -= lineHeight + spacingY;

            if (!message->drawn) {
                message->time.Restart();
            }

            message->drawn = true;

            count++;
            if (count >= 15) {
                break;
            }
        }

        return true;
    }

    int OSDExtras::Notify(const TextID &textID, const Color &foreground, const Color &background) {
        return Notify(Language::getInstance()->get(textID), foreground, background);
    }

    std::vector<std::string> SplitText(const std::string& text, int maxWidth) {
        std::vector<std::string> lines;
        std::string remaining = text;

        while (!remaining.empty()) {
            std::string current = remaining;

            bool isFirstLine = lines.empty();
            std::string prefix = isFirstLine ? "" : "...";
            std::string suffix = "";

            //Always expect a new line
            suffix = "...";

            while (!current.empty()) {
                std::string candidate = prefix + current + suffix;

                if (ScreenExtras::SystemFontSize(candidate.c_str()) <= maxWidth) {
                    break;
                }

                current.pop_back();
            }

            //If everything fits, no suffix needed
            if (current.size() == remaining.size()) {
                suffix = "";
            }

            lines.push_back(prefix + current + suffix);

            remaining.erase(0, current.size());
        }

        return lines;
    }

    int OSDExtras::Notify(const std::string &text, const Color &foreground, const Color &background) {
        auto lines = SplitText(text, 375);

        Lock();

        if (Notifications.size() + lines.size() > 50) {
            Unlock();
            return -1;
        }

        for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
            Notifications.push_back(new OSDMessage(*it, foreground, background));
        }

        Unlock();
        return 0;
    }

    bool OSDExtras::UpdateOSDSystem(const Screen &screen) {
        Update();
        return Draw(screen);
    }
}