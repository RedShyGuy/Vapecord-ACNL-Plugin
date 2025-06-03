#include "CTRPluginFrameworkImpl/Sound.hpp"
#include "CTRPluginFramework/System/FwkSettings.hpp"

namespace CTRPluginFramework
{
    std::vector<Sound> SoundEngineImpl::menuSounds;
    Sound SoundEngineImpl::fallbackSound;

    static const char* defaultSoundFiles[(u32)SoundEngine::Event::NUM_EVENTS] =
    {
        "snd/sound_cursor.bcwav",
        "snd/sound_accept.bcwav",
        "snd/sound_cancel.bcwav",
        "snd/sound_select.bcwav",
        "snd/sound_deselect.bcwav"
    };

    void SoundEngineImpl::Initializelibcwav()
    {
        cwavUseEnvironment(CWAV_ENV_CSND);
    }

    void SoundEngineImpl::NotifyAptEvent(APT_HookType event)
    {
        ncsndNotifyAptEvent(event);
    }

    void SoundEngineImpl::SetVaToPaConvFunction(vaToPaCallback_t function)
    {
        cwavSetVAToPACallback(function);
    }

    bool SoundEngineImpl::RegisterMenuSoundEvent(SoundEngine::Event eventType, Sound& sound)
    {
        if (sound.GetLoadStatus() == Sound::CWAVStatus::SUCCESS && eventType < SoundEngine::Event::NUM_EVENTS)
        {
            menuSounds[(u32)eventType] = sound;
            return true;
        }
        return false;
    }

    Sound& SoundEngineImpl::GetMenuSoundEvent(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            return  menuSounds[(u32)eventType];
        else
            return fallbackSound;
    }

    Sound::CWAVStatus SoundEngineImpl::PlayMenuSound(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            return menuSounds[(u32)eventType].Play();
        return Sound::CWAVStatus::INVALID_ARGUMENT;
    }

    void SoundEngineImpl::StopMenuSound(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            menuSounds[(u32)eventType].Stop();
    }

    void SoundEngineImpl::DeRegisterMenuSoundEvent(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            menuSounds[(u32)eventType] = Sound();
    }

    void SoundEngineImpl::InitializeMenuSounds()
    {
        FwkSettings &settings = FwkSettings::Get();

        menuSounds.resize((u32)SoundEngine::Event::NUM_EVENTS);

        if (!settings.TryLoadSDSounds)
            return;

        std::string source;
        if (FwkSettings::Header->isDefaultPlugin)
                source = "/luma/plugins/ActionReplay/";

        for (u32 i = 0; i < (u32)SoundEngine::Event::NUM_EVENTS; i++)
        {
            Sound curr(source + defaultSoundFiles[i], 3);
            if (curr.GetLoadStatus() == Sound::CWAVStatus::SUCCESS)
                RegisterMenuSoundEvent((SoundEngine::Event)i, curr);
        }
    }

    void SoundEngineImpl::ClearMenuSounds()
    {
        for (u32 i = 0; i < (u32)SoundEngine::Event::NUM_EVENTS; i++)
            menuSounds[i] = Sound();
    }
}