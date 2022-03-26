#include "stardust/audio/SoundSystem.h"

#include <memory>
#include <utility>

namespace stardust
{
    namespace audio
    {
        SoundSystem::SoundSystem(SoundSystem&& other) noexcept
        {
            Destroy();

            std::swap(m_soLoudHandle, other.m_soLoudHandle);
            m_listener = std::move(other.m_listener);

            std::swap(m_didInitialiseSuccessfully, other.m_didInitialiseSuccessfully);
            std::swap(m_errorString, other.m_errorString);
        }

        auto SoundSystem::operator =(SoundSystem&& other) noexcept -> SoundSystem&
        {
            Destroy();

            std::swap(m_soLoudHandle, other.m_soLoudHandle);
            m_listener = std::move(other.m_listener);

            std::swap(m_didInitialiseSuccessfully, other.m_didInitialiseSuccessfully);
            std::swap(m_errorString, other.m_errorString);

            return *this;
        }

        SoundSystem::~SoundSystem() noexcept
        {
            Destroy();
        }

        auto SoundSystem::Initialise() -> void
        {
            m_soLoudHandle = std::make_unique<SoLoud::Soloud>();

            const SoLoud::result initialiseStatus = m_soLoudHandle->init(SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::LEFT_HANDED_3D, SoLoud::Soloud::SDL2);
            m_didInitialiseSuccessfully = initialiseStatus == 0u;

            if (!m_didInitialiseSuccessfully)
            {
                m_errorString = m_soLoudHandle->getErrorString(initialiseStatus);
            }

            m_listener.Initialise(*this);
        }

        auto SoundSystem::Destroy() noexcept -> void
        {
            if (m_soLoudHandle != nullptr)
            {
                m_soLoudHandle->deinit();
                m_soLoudHandle = nullptr;

                m_didInitialiseSuccessfully = false;
            }
        }

        auto SoundSystem::Update() const -> void
        {
            m_soLoudHandle->update3dAudio();
        }

        auto SoundSystem::PlaySound(Sound& sound, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->play(sound.GetRawHandle(), soundPlayData.volume, soundPlayData.pan, soundPlayData.startPaused);
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *this);
        }

        auto SoundSystem::PlaySound(SoundStream& soundStream, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->play(soundStream.GetRawHandle(), soundPlayData.volume, soundPlayData.pan);
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *this);
        }

        auto SoundSystem::PlaySoundClocked(Sound& sound, const f64 fixedTimestep, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->playClocked(fixedTimestep, sound.GetRawHandle(), soundPlayData.volume, soundPlayData.pan);
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *this);
        }

        auto SoundSystem::PlaySoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->playClocked(fixedTimestep, soundStream.GetRawHandle(), soundPlayData.volume, soundPlayData.pan);
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *this);
        }

        auto SoundSystem::PlayPositionalSound(Sound& sound, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->play3d(
                sound.GetRawHandle(),
                soundPlayData.position.x,
                soundPlayData.position.y,
                0.0f,
                soundPlayData.velocity.x,
                soundPlayData.velocity.y,
                0.0f,
                soundPlayData.volume,
                soundPlayData.startPaused
            );

            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *this, nullptr, soundPlayData.position, soundPlayData.velocity);
        }

        auto SoundSystem::PlayPositionalSound(SoundStream& soundStream, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->play3d(
                soundStream.GetRawHandle(),
                soundPlayData.position.x,
                soundPlayData.position.y,
                0.0f,
                soundPlayData.velocity.x,
                soundPlayData.velocity.y,
                0.0f,
                soundPlayData.volume,
                soundPlayData.startPaused
            );
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *this, nullptr, soundPlayData.position, soundPlayData.velocity);
        }

        auto SoundSystem::PlayPositionalSoundClocked(Sound& sound, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->play3dClocked(
                fixedTimestep,
                sound.GetRawHandle(),
                soundPlayData.position.x,
                soundPlayData.position.y,
                0.0f,
                soundPlayData.velocity.x,
                soundPlayData.velocity.y,
                0.0f,
                soundPlayData.volume
            );
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *this, nullptr, soundPlayData.position, soundPlayData.velocity);
        }

        auto SoundSystem::PlayPositionalSoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_soLoudHandle->play3dClocked(
                fixedTimestep,
                soundStream.GetRawHandle(),
                soundPlayData.position.x,
                soundPlayData.position.y,
                0.0f,
                soundPlayData.velocity.x,
                soundPlayData.velocity.y,
                0.0f,
                soundPlayData.volume
            );
            SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *this, nullptr, soundPlayData.position, soundPlayData.velocity);
        }

        auto SoundSystem::PauseAllSounds() const -> void
        {
            m_soLoudHandle->setPauseAll(true);
        }

        auto SoundSystem::ResumeAllSounds() const -> void
        {
            m_soLoudHandle->setPauseAll(true);
        }

        auto SoundSystem::StopAllSounds() const noexcept -> void
        {
            m_soLoudHandle->stopAll();
        }

        auto SoundSystem::FadeGlobalVolume(const f32 targetVolume, const f32 seconds) const -> void
        {
            m_soLoudHandle->fadeGlobalVolume(targetVolume, seconds);
        }

        auto SoundSystem::OscillateGlobalVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) const -> void
        {
            m_soLoudHandle->oscillateGlobalVolume(fromVolume, toVolume, frequency);
        }

        auto SoundSystem::ResetGlobalVolumeOscillation() const -> void
        {
            m_soLoudHandle->oscillateGlobalVolume(1.0f, 1.0f, 0.0);
        }

        [[nodiscard]] auto SoundSystem::GetPlayingSoundCount() const -> u32
        {
            return m_soLoudHandle->getActiveVoiceCount();
        }

        [[nodiscard]] auto SoundSystem::IsAnySoundPlaying() const -> bool
        {
            return m_soLoudHandle->getActiveVoiceCount() > 0u;
        }

        [[nodiscard]] auto SoundSystem::GetGlobalVolume() const noexcept -> f32
        {
            return m_soLoudHandle->getGlobalVolume();
        }

        auto SoundSystem::SetGlobalVolume(const f32 globalVolume) const noexcept -> void
        {
            m_soLoudHandle->setGlobalVolume(globalVolume);
        }

        [[nodiscard]] auto SoundSystem::GetPostClipScaler() const noexcept -> f32
        {
            return m_soLoudHandle->getPostClipScaler();
        }

        auto SoundSystem::SetPostClipScaler(const f32 postClipScaler) const noexcept -> void
        {
            m_soLoudHandle->setPostClipScaler(postClipScaler);
        }

        [[nodiscard]] auto SoundSystem::GetSpeedOfSound() const noexcept -> f32
        {
            return m_soLoudHandle->get3dSoundSpeed();
        }

        void SoundSystem::SetSpeedOfSound(const f32 speedOfSound) const noexcept
        {
        m_soLoudHandle->set3dSoundSpeed(speedOfSound);
        }

        auto SoundSystem::SetVoiceHandleParameters(const VoiceHandle voiceHandle, const SoundPlayData& soundPlayData) const -> void
        {
            if (soundPlayData.protect)
            {
                m_soLoudHandle->setProtectVoice(voiceHandle, true);
            }

            if (soundPlayData.relativePlaySpeed != 1.0f)
            {
                m_soLoudHandle->setRelativePlaySpeed(voiceHandle, soundPlayData.relativePlaySpeed);
            }

            if (soundPlayData.loop)
            {
                m_soLoudHandle->setLooping(voiceHandle, true);
            }
        }

        auto SoundSystem::SetVoiceHandleParameters(const VoiceHandle voiceHandle, const PositionalSoundPlayData& soundPlayData) const -> void
        {
            if (soundPlayData.protect)
            {
                m_soLoudHandle->setProtectVoice(voiceHandle, true);
            }

            if (soundPlayData.relativePlaySpeed != 1.0f)
            {
                m_soLoudHandle->setRelativePlaySpeed(voiceHandle, soundPlayData.relativePlaySpeed);
            }

            if (soundPlayData.loop)
            {
                m_soLoudHandle->setLooping(voiceHandle, true);
            }

            m_soLoudHandle->set3dSourceAttenuation(voiceHandle, static_cast<u32>(soundPlayData.attenuation.model), soundPlayData.attenuation.rolloffFactor);
            m_soLoudHandle->set3dSourceMinMaxDistance(voiceHandle, soundPlayData.attenuation.minDistance, soundPlayData.attenuation.maxDistance);

            switch (soundPlayData.inaudibleBehaviour)
            {
            case InaudibleBehaviour::KeepTicking:
                m_soLoudHandle->setInaudibleBehavior(voiceHandle, true, false);

                break;

            case InaudibleBehaviour::Pause:
                m_soLoudHandle->setInaudibleBehavior(voiceHandle, false, false);

                break;

            case InaudibleBehaviour::Kill:
                m_soLoudHandle->setInaudibleBehavior(voiceHandle, false, true);

                break;
            }
        }
    }
}
