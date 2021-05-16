#include "stardust/audio/SoundSystem.h"

namespace stardust
{
    SoundSystem::SoundSystem()
        : m_soLoudHandle(std::make_unique<SoLoud::Soloud>()), m_listener(*this)
    {
        Initialise();
    }

    SoundSystem::~SoundSystem() noexcept
    {
        Destroy();
    }

    void SoundSystem::Update() const
    {
        m_soLoudHandle->update3dAudio();
    }

    SoundSource SoundSystem::PlaySound(Sound& sound, const bool startPaused)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->play(sound.GetRawHandle(), -1.0f, 0.0f, startPaused);
    
        return SoundSource(soundHandle, *this);
    }
    
    SoundSource SoundSystem::PlaySound(SoundStream& soundStream, const bool startPaused)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->play(soundStream.GetRawHandle(), -1.0f, 0.0f, startPaused);
    
        return SoundSource(soundHandle, *this);
    }
    
    SoundSource SoundSystem::Play3DSound(Sound& sound, const Vec3& position, const Vec3& velocity, const bool startPaused)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->play3d(
            sound.GetRawHandle(),
            position.x, position.y, position.z,
            velocity.x, velocity.y, velocity.z,
            1.0f, startPaused
        );
    
        return SoundSource(soundHandle, *this, position, velocity);
    }
    
    SoundSource SoundSystem::Play3DSound(SoundStream& soundStream, const Vec3& position, const Vec3& velocity, const bool startPaused)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->play3d(
            soundStream.GetRawHandle(),
            position.x, position.y, position.z,
            velocity.x, velocity.y, velocity.z,
            1.0f, startPaused
        );
    
        return SoundSource(soundHandle, *this, position, velocity);
    }
    
    SoundSource SoundSystem::PlaySoundWithDelay(Sound& sound, const f32 secondsDelay)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->playClocked(secondsDelay, sound.GetRawHandle());
    
        return SoundSource(soundHandle, *this);
    }
    
    SoundSource SoundSystem::PlaySoundWithDelay(SoundStream& soundStream, const f32 secondsDelay)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->playClocked(secondsDelay, soundStream.GetRawHandle());
    
        return SoundSource(soundHandle, *this);
    }
    
    SoundSource SoundSystem::Play3DSoundWithDelay(Sound& sound, const f32 secondsDelay, const Vec3& position, const Vec3& velocity)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->play3dClocked(
            secondsDelay, sound.GetRawHandle(),
            position.x, position.y, position.z,
            velocity.x, velocity.y, velocity.z,
            1.0f
        );
    
        return SoundSource(soundHandle, *this, position, velocity);
    }
    
    SoundSource SoundSystem::Play3DSoundWithDelay(SoundStream& soundStream, const f32 secondsDelay, const Vec3& position, const Vec3& velocity)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->play3dClocked(
            secondsDelay, soundStream.GetRawHandle(),
            position.x, position.y, position.z,
            velocity.x, velocity.y, velocity.z,
            1.0f
        );
    
        return SoundSource(soundHandle, *this, position, velocity);
    }
    
    SoundSource SoundSystem::PlaySoundInBackground(Sound& sound, const bool startPaused)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->playBackground(sound.GetRawHandle(), -1.0f, 0.0f, startPaused);
    
        return SoundSource(soundHandle, *this);
    }
    
    SoundSource SoundSystem::PlaySoundInBackground(SoundStream& soundStream, const bool startPaused)
    {
        const SoLoud::handle soundHandle = m_soLoudHandle->playBackground(soundStream.GetRawHandle(), -1.0f, 0.0f, startPaused);
    
        return SoundSource(soundHandle, *this);
    }

    void SoundSystem::StopAllSounds() const
    {
        m_soLoudHandle->stopAll();
    }

    void SoundSystem::PauseAllSounds() const
    {
        m_soLoudHandle->setPauseAll(true);
    }

    void SoundSystem::ResumeAllSounds() const
    {
        m_soLoudHandle->setPauseAll(false);
    }

    void SoundSystem::FadeVolumeGlobal(const f32 volumeToFadeTo, const f32 seconds) const
    {
        m_soLoudHandle->fadeGlobalVolume(volumeToFadeTo, seconds);
    }

    void SoundSystem::OscillateGlobalVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) const
    {
        m_soLoudHandle->oscillateGlobalVolume(fromVolume, toVolume, frequency);
    }

    void SoundSystem::ResetGlobalVolumeOscillation() const
    {
        m_soLoudHandle->oscillateGlobalVolume(1.0f, 1.0f, 0.0);
    }

    unsigned int SoundSystem::GetPlayingSoundCount() const
    {
        return m_soLoudHandle->getActiveVoiceCount();
    }

    bool SoundSystem::IsSoundPlaying() const
    {
        return m_soLoudHandle->getActiveVoiceCount() > 0u;
    }

    [[nodiscard]] f32 SoundSystem::GetGlobalVolume() const noexcept
    {
        return m_soLoudHandle->getGlobalVolume();
    }

    void SoundSystem::SetGlobalVolume(const f32 globalVolume) const noexcept
    {
        m_soLoudHandle->setGlobalVolume(globalVolume);
    }

    [[nodiscard]] f32 SoundSystem::GetPostClipScaler() const noexcept
    {
        return m_soLoudHandle->getPostClipScaler();
    }

    void SoundSystem::SetPostClipScaler(const f32 postClipScaler) const noexcept
    {
        m_soLoudHandle->setPostClipScaler(postClipScaler);
    }

    [[nodiscard]] f32 SoundSystem::GetSpeedOfSound() const noexcept
    {
        return m_soLoudHandle->get3dSoundSpeed();
    }

    void SoundSystem::SetSpeedOfSound(const f32 speedOfSound) const noexcept
    {
        m_soLoudHandle->set3dSoundSpeed(speedOfSound);
    }

    void SoundSystem::Initialise()
    {
        const SoLoud::result initialiseStatus = m_soLoudHandle->init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::LEFT_HANDED_3D, SoLoud::Soloud::SDL2);
        m_didInitialiseSuccessfully = initialiseStatus == 0u;
    }

    void SoundSystem::Destroy()
    {
        if (m_soLoudHandle != nullptr)
        {
            m_soLoudHandle->deinit();
            m_soLoudHandle = nullptr;

            m_didInitialiseSuccessfully = false;
        }
    }
}