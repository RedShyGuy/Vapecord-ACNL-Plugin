#include "CTRPluginFramework/Sound.hpp"
#include "CTRPluginFrameworkImpl/Sound/SoundEngineImpl.hpp"

namespace CTRPluginFramework
{
    bool SoundEngine::RegisterMenuSoundEvent(Event eventType, Sound& sound)
    {
        return SoundEngineImpl::RegisterMenuSoundEvent(eventType, sound);
    }

    Sound& SoundEngine::GetMenuSoundEvent(Event eventType)
    {
        return SoundEngineImpl::GetMenuSoundEvent(eventType);
    }

    Sound::CWAVStatus SoundEngine::PlayMenuSound(Event eventType)
    {
        return SoundEngineImpl::PlayMenuSound(eventType);
    }

    void SoundEngine::StopMenuSound(Event eventType)
    {
        SoundEngineImpl::StopMenuSound(eventType);
    }

    void SoundEngine::DeRegisterMenuSoundEvent(Event eventType)
    {
        SoundEngineImpl::DeRegisterMenuSoundEvent(eventType);
    }
}