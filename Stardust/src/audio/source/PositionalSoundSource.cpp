#include "stardust/audio/source/PositionalSoundSource.h"

#include "stardust/audio/mixing/MixingBus.h"
#include "stardust/audio/SoundSystem.h"

namespace stardust
{
    namespace audio
    {
        PositionalSoundSource::PositionalSoundSource(const VoiceHandle handle, SoundSystem& soundSystem, const ObserverPointer<MixingBus> mixingBus, const Vector2 position, const Vector2 velocity)
            : m_handle(handle), m_soundSystem(&soundSystem), m_mixingBus(mixingBus), m_position(position), m_velocity(velocity)
        { }

        auto PositionalSoundSource::Pause() const -> void
        {
            m_soundSystem->GetRawHandle().setPause(m_handle, true);
        }

        auto PositionalSoundSource::Resume() const -> void
        {
            m_soundSystem->GetRawHandle().setPause(m_handle, false);
        }

        auto PositionalSoundSource::Stop() const noexcept -> void
        {
            m_soundSystem->GetRawHandle().stop(m_handle);
        }

        [[nodiscard]] auto PositionalSoundSource::IsPlaying() const -> bool
        {
            return m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle) && !IsPaused();
        }

        [[nodiscard]] auto PositionalSoundSource::IsPaused() const -> bool
        {
            return m_soundSystem->GetRawHandle().getPause(m_handle);
        }

        [[nodiscard]] auto PositionalSoundSource::IsStopped() const -> bool
        {
            return !m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle);
        }

        auto PositionalSoundSource::SchedulePause(const f32 secondsDelay) -> void
        {
            m_soundSystem->GetRawHandle().schedulePause(m_handle, static_cast<f32>(secondsDelay));
        }

        auto PositionalSoundSource::ScheduleStop(const f32 secondsDelay) -> void
        {
            m_soundSystem->GetRawHandle().scheduleStop(m_handle, static_cast<f32>(secondsDelay));
        }

        [[nodiscard]] auto PositionalSoundSource::IsLooping() const -> bool
        {
            return m_soundSystem->GetRawHandle().getLooping(m_handle);
        }

        auto PositionalSoundSource::SetLooping(const bool isLooping) -> void
        {
            m_soundSystem->GetRawHandle().setLooping(m_handle, isLooping);
        }

        [[nodiscard]] auto PositionalSoundSource::GetLoopPoint() const -> f64
        {
            return static_cast<f32>(m_soundSystem->GetRawHandle().getLoopPoint(m_handle));
        }

        auto PositionalSoundSource::SetLoopPoint(const f64 loopPoint) -> void
        {
            m_soundSystem->GetRawHandle().setLoopPoint(m_handle, loopPoint);
        }

        [[nodiscard]] auto PositionalSoundSource::GetLoopCount() const -> u32
        {
            return m_soundSystem->GetRawHandle().getLoopCount(m_handle);
        }

        [[nodiscard]] auto PositionalSoundSource::GetStreamTime() const -> f64
        {
            return m_soundSystem->GetRawHandle().getStreamTime(m_handle);
        }

        auto PositionalSoundSource::Seek(const f64 seconds) -> void
        {
            m_soundSystem->GetRawHandle().seek(m_handle, seconds);
        }

        auto PositionalSoundSource::Rewind() -> void
        {
            m_soundSystem->GetRawHandle().seek(m_handle, 0.0);
        }

        [[nodiscard]] auto PositionalSoundSource::GetVolume() const -> f32
        {
            return m_soundSystem->GetRawHandle().getVolume(m_handle);
        }

        [[nodiscard]] auto PositionalSoundSource::GetOverallVolume() const -> f32
        {
            return m_soundSystem->GetRawHandle().getOverallVolume(m_handle);
        }

        auto PositionalSoundSource::SetVolume(const f32 volume) -> void
        {
            m_soundSystem->GetRawHandle().setVolume(m_handle, volume);
        }

        auto PositionalSoundSource::FadeVolume(const f32 toVolume, const f32 fadeTime) -> void
        {
            m_soundSystem->GetRawHandle().fadeVolume(m_handle, toVolume, fadeTime);
        }

        auto PositionalSoundSource::FadeRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fadeTime) -> void
        {
            m_soundSystem->GetRawHandle().fadeRelativePlaySpeed(m_handle, toRelativePlaySpeed, fadeTime);
        }

        [[nodiscard]] auto PositionalSoundSource::GetPan() const -> f32
        {
            return m_soundSystem->GetRawHandle().getPan(m_handle);
        }

        [[nodiscard]] auto PositionalSoundSource::GetRelativePlaySpeed() const -> f32
        {
            return m_soundSystem->GetRawHandle().getRelativePlaySpeed(m_handle);
        }

        auto PositionalSoundSource::SetRelativePlaySpeed(const f32 relativePlaySpeed) -> void
        {
            m_soundSystem->GetRawHandle().setRelativePlaySpeed(m_handle, relativePlaySpeed);
        }

        [[nodiscard]] auto PositionalSoundSource::GetSampleRate() const -> f32
        {
            return m_soundSystem->GetRawHandle().getSamplerate(m_handle);
        }

        auto PositionalSoundSource::SetSampleRate(const f32 sampleRate) -> void
        {
            m_soundSystem->GetRawHandle().setSamplerate(m_handle, sampleRate);
        }

        auto PositionalSoundSource::OscillateVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) -> void
        {
            m_soundSystem->GetRawHandle().oscillateVolume(m_handle, fromVolume, toVolume, frequency);
        }

        auto PositionalSoundSource::ResetVolumeOscillation() -> void
        {
            m_soundSystem->GetRawHandle().oscillateVolume(m_handle, 1.0f, 1.0f, 0.0);
        }

        auto PositionalSoundSource::OscillateRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fromRelativePlaySpeed, const f32 frequency) -> void
        {
            m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, fromRelativePlaySpeed, toRelativePlaySpeed, frequency);
        }

        auto PositionalSoundSource::ResetRelativePlaySpeedOscillation() -> void
        {
            m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, 1.0f, 1.0f, 0.0);
        }

        auto PositionalSoundSource::SetPosition(const Vector2 position) -> void
        {
            m_soundSystem->GetRawHandle().set3dSourcePosition(m_handle, position.x, position.y, 0.0f);
            m_position = position;
        }

        auto PositionalSoundSource::SetVelocity(const Vector2 velocity) -> void
        {
            m_soundSystem->GetRawHandle().set3dSourceVelocity(m_handle, velocity.x, velocity.y, 0.0f);
            m_velocity = velocity;
        }

        auto PositionalSoundSource::SetAttenuationModel(const AttenuationModel attenuationModel) -> void
        {
            m_soundSystem->GetRawHandle().set3dSourceAttenuation(m_handle, static_cast<u32>(attenuationModel), m_attenuationInfo.rolloffFactor);
            m_attenuationInfo.model = attenuationModel;
        }

        auto PositionalSoundSource::SetAttenuationRolloffFactor(const f32 attenuationRolloffFactor) -> void
        {
            m_soundSystem->GetRawHandle().set3dSourceAttenuation(m_handle, static_cast<u32>(m_attenuationInfo.model), attenuationRolloffFactor);
            m_attenuationInfo.rolloffFactor = attenuationRolloffFactor;
        }

        auto PositionalSoundSource::SetMinAttenuationDistance(const f32 minAttenuationDistance) -> void
        {
            m_soundSystem->GetRawHandle().set3dSourceMinMaxDistance(m_handle, minAttenuationDistance, m_attenuationInfo.maxDistance);
            m_attenuationInfo.minDistance = minAttenuationDistance;
        }

        auto PositionalSoundSource::SetMaxAttenuationDistance(const f32 maxAttenuationDistance) -> void
        {
            m_soundSystem->GetRawHandle().set3dSourceMinMaxDistance(m_handle, m_attenuationInfo.minDistance, maxAttenuationDistance);
            m_attenuationInfo.maxDistance = maxAttenuationDistance;
        }

        auto PositionalSoundSource::SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour) -> void
        {
            switch (inaudibleBehaviour)
            {
            case InaudibleBehaviour::KeepTicking:
                m_soundSystem->GetRawHandle().setInaudibleBehavior(m_handle, true, false);

                break;

            case InaudibleBehaviour::Pause:
                m_soundSystem->GetRawHandle().setInaudibleBehavior(m_handle,false, false);

                break;

            case InaudibleBehaviour::Kill:
                m_soundSystem->GetRawHandle().setInaudibleBehavior(m_handle, false, true);

                break;
            }

            m_inaudibleBehaviour = inaudibleBehaviour;
        }

        [[nodiscard]] auto PositionalSoundSource::IsProtected() const -> bool
        {
            return m_soundSystem->GetRawHandle().getProtectVoice(m_handle);
        }

        auto PositionalSoundSource::EnableProtection(const bool isProtected) -> void
        {
            m_soundSystem->GetRawHandle().setProtectVoice(m_handle, isProtected);
        }
    }
}
