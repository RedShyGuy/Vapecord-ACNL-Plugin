#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUNDENGINE_IMPL_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUNDENGINE_IMPL_HPP

#include "CTRPluginFramework/Sound.hpp"
#include "cwav.h"
#include <vector>

namespace CTRPluginFramework
{
    class SoundEngineImpl
    {
    public:
        static void Initializelibcwav();

        static void NotifyAptEvent(APT_HookType event);

        static void SetVaToPaConvFunction(vaToPaCallback_t function);

        static bool RegisterMenuSoundEvent(SoundEngine::Event eventType, Sound& sound);

        static Sound& GetMenuSoundEvent(SoundEngine::Event eventType);

        static Sound::CWAVStatus PlayMenuSound(SoundEngine::Event eventType);

        static void StopMenuSound(SoundEngine::Event eventType);

        static void DeRegisterMenuSoundEvent(SoundEngine::Event eventType);

        static void InitializeMenuSounds();

        static void ClearMenuSounds();

    private:
        static std::vector<Sound> menuSounds;
        static Sound fallbackSound;
    };
}

#endif