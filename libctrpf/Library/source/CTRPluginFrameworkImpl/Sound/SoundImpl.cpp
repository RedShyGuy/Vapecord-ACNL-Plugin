
#include "CTRPluginFrameworkImpl/Sound/SoundImpl.hpp"
#include "CTRPluginFramework/System/File.hpp"
#include <cstring>

namespace CTRPluginFramework
{
    SoundImpl::SoundImpl(const std::string& bcwavFile, int maxSimultPlays)
    {
        File cwavFile(bcwavFile, File::READ);
        std::memset(&_cwav, 0, sizeof(CWAV));

        if (!cwavFile.IsOpen())
        {
            _cwav.loadStatus = CWAV_FILE_OPEN_FAILED;
            return;
        }

        u32 fileSize = cwavFile.GetSize();

        _cwav.dataBuffer = static_cast<void *>(::operator new(fileSize, std::nothrow));
        if (_cwav.dataBuffer == nullptr)
        {
            _cwav.loadStatus = CWAV_FILE_READ_FAILED;
            return;
        }

        cwavFile.Seek(0);
        cwavFile.Read(_cwav.dataBuffer, fileSize);

        cwavLoad(&_cwav, _cwav.dataBuffer, maxSimultPlays);
    }

    SoundImpl::SoundImpl(const u8* bcwavBuffer, int maxSimultPlays)
    {
        _cwav.dataBuffer = nullptr;
        cwavLoad(&_cwav, bcwavBuffer, maxSimultPlays);
    }

    cwavStatus_t SoundImpl::GetLoadStatus()
    {
        return _cwav.loadStatus;
    }

    void SoundImpl::SetVolume(float volume)
    {
        _cwav.volume = volume;
    }

    float SoundImpl::GetVolume()
    {
        return _cwav.volume;
    }

    void SoundImpl::SetPan(float pan)
    {
        _cwav.monoPan = pan;
    }

    float SoundImpl::GetPan()
    {
        return _cwav.monoPan;
    }

    u32 SoundImpl::GetChannelAmount()
    {
        return _cwav.numChannels;
    }

    bool SoundImpl::IsLooped()
    {
        return _cwav.isLooped;
    }

    cwavStatus_t SoundImpl::Play(int leftEarChannel, int rightEarChannel)
    {
        return cwavPlay(&_cwav, leftEarChannel, rightEarChannel).playStatus;
    }

    bool SoundImpl::PlayDirectly(int leftEarChannel, int rightEarChannel, u32 directSoundChannel, u32 priority, DirectSoundModifiers& modifiers)
    {
        ncsndDirectSound dirSound;
        ncsndInitializeDirectSound(&dirSound);
        dirSound.soundModifiers.speedMultiplier = modifiers.speedMultiplier;
        dirSound.soundModifiers.channelVolumes[0] *= modifiers.leftChannelVolume;
        dirSound.soundModifiers.channelVolumes[1] *= modifiers.rightChannelVolume;
        dirSound.soundModifiers.ignoreVolumeSlider = modifiers.ignoreVolumeSlider ? 1 : 0;
        dirSound.soundModifiers.forceSpeakerOutput = modifiers.forceSpeakerOutput ? 1 : 0;
        dirSound.soundModifiers.playOnSleep = modifiers.playOnSleep ? 1 : 0;

        return cwavPlayAsDirectSound(&_cwav, leftEarChannel, rightEarChannel, directSoundChannel, priority, &dirSound.soundModifiers);
    }

    void SoundImpl::Stop(int leftEarChannel, int rightEarChannel)
    {
        cwavStop(&_cwav, leftEarChannel, rightEarChannel);
    }

    SoundImpl::~SoundImpl()
    {
        cwavFree(&_cwav);
        if (_cwav.dataBuffer)
            ::operator delete(_cwav.dataBuffer);
    }
}