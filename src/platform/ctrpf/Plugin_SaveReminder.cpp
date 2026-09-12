#include "core/Config.hpp"
#include "core/infrastructure/Language.hpp"
#include "core/game_api/Game.hpp"
#include "core/HUD.hpp"
#include "Color.h"

namespace CTRPluginFramework {
    namespace {
        static bool s_saveReminderEnabled = true;
        static int s_saveReminderInterval = 25;
        static Clock s_reminderClock;
    }

    void SaveReminderCallback(void) {
        if(!s_saveReminderEnabled) {
            return;
        }

        if(Game::GetGameMode() != Game::OFFLINE) {
            return;
        }

        if(!s_reminderClock.HasTimePassed(Seconds(s_saveReminderInterval * 60))) {
            return;
        }

        s_reminderClock.Restart();
        HUD::Notify(Language::getInstance()->get(TextID::SAVE_REMINDER_MESSAGE), Color(0xADFF2FFF));
    }

    void InitSaveReminder(void) {
        Config::GetSaveReminderEnabled(s_saveReminderEnabled);
        Config::GetSaveReminderInterval(s_saveReminderInterval);
        s_reminderClock.Restart();
    }

    void SaveReminderSettings(void) {
        while(true) {
            Language *lang = Language::getInstance();

            std::vector<std::string> options = {
                (s_saveReminderEnabled ? Color(pGreen) : Color(pRed))
                    << lang->get(s_saveReminderEnabled ? TextID::VECTOR_ENABLED : TextID::VECTOR_DISABLED),
                Utils::Format(lang->get(TextID::SAVE_REMINDER_INTERVAL).c_str(), s_saveReminderInterval),
            };

            Keyboard keyboard(lang->get(TextID::SAVE_REMINDER_SETTINGS), options);
            int choice = keyboard.Open();
            if(choice < 0) {
                return;
            }

            switch(choice) {
                case 0:
                    s_saveReminderEnabled = !s_saveReminderEnabled;
                    Config::SetSaveReminderEnabled(s_saveReminderEnabled);
                    if(s_saveReminderEnabled) {
                        s_reminderClock.Restart();
                    }
                    break;
                case 1: {
                    Keyboard intervalKb(lang->get(TextID::SAVE_REMINDER_INTERVAL_PROMPT));
                    intervalKb.IsHexadecimal(false);
                    intervalKb.SetSlider(5, 120, 5);
                    u32 newInterval = s_saveReminderInterval;
                    if(intervalKb.Open(newInterval, newInterval) == 0) {
                        s_saveReminderInterval = newInterval;
                        Config::SetSaveReminderInterval(newInterval);
                        s_reminderClock.Restart();
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}
