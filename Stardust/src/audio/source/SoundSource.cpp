#include "stardust/audio/source/SoundSource.h"

#include "stardust/audio/mixing/MixingBus.h"
#include "stardust/audio/SoundSystem.h"

namespace stardust
{
    namespace audio
    {
        SoundSource::SoundSource(const VoiceHandle handle, SoundSystem& soundSystem, const ObserverPointer<MixingBus> mixingBus)
            : m_soundSystem(&soundSystem), m_handle(handle), m_mixingBus(mixingBus)
        { }

        auto SoundSource::Pause() const -> void
        {
            m_soundSystem->GetRawHandle().setPause(m_handle, true);
        }

        auto SoundSource::Resume() const -> void
        {
            m_soundSystem->GetRawHandle().setPause(m_handle, false);
        }

        auto SoundSource::Stop() const noexcept -> void
        {
            m_soundSystem->GetRawHandle().stop(m_handle);
        }

        [[nodiscard]] auto SoundSource::IsPlaying() const -> bool
        {
            return m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle) && !IsPaused();
        }

        [[nodiscard]] auto SoundSource::IsPaused() const -> bool
        {
            return m_soundSystem->GetRawHandle().getPause(m_handle);
        }

        [[nodiscard]] auto SoundSource::IsStopped() const -> bool
        {
            return !m_soundSystem->GetRawHandle().isValidVoiceHandle(m_handle);
        }

        auto SoundSource::SchedulePause(const f32 secondsDelay) -> void
        {
            m_soundSystem->GetRawHandle().schedulePause(m_handle, static_cast<f32>(secondsDelay));
        }

        auto SoundSource::ScheduleStop(const f32 secondsDelay) -> void
        {
            m_soundSystem->GetRawHandle().scheduleStop(m_handle, static_cast<f32>(secondsDelay));
        }

        [[nodiscard]] auto SoundSource::IsLooping() const -> bool
        {
            return m_soundSystem->GetRawHandle().getLooping(m_handle);
        }

        auto SoundSource::SetLooping(const bool isLooping) -> void
        {
            m_soundSystem->GetRawHandle().setLooping(m_handle, isLooping);
        }

        [[nodiscard]] auto SoundSource::GetLoopPoint() const -> f64
        {
            return static_cast<f32>(m_soundSystem->GetRawHandle().getLoopPoint(m_handle));
        }

        auto SoundSource::SetLoopPoint(const f64 loopPoint) -> void
        {
            m_soundSystem->GetRawHandle().setLoopPoint(m_handle, loopPoint);
        }

        [[nodiscard]] auto SoundSource::GetLoopCount() const -> u32
        {
            return m_soundSystem->GetRawHandle().getLoopCount(m_handle);
        }

        [[nodiscard]] auto SoundSource::GetStreamTime() const -> f64
        {
            return m_soundSystem->GetRawHandle().getStreamTime(m_handle);
        }

        auto SoundSource::Seek(const f64 seconds) -> void
        {
            m_soundSystem->GetRawHandle().seek(m_handle, seconds);
        }

        auto SoundSource::Rewind() -> void
        {
            m_soundSystem->GetRawHandle().seek(m_handle, 0.0);
        }

        [[nodiscard]] auto SoundSource::GetVolume() const -> f32
        {
            return m_soundSystem->GetRawHandle().getVolume(m_handle);
        }

        auto SoundSource::SetVolume(const f32 volume) -> void
        {
            m_soundSystem->GetRawHandle().setVolume(m_handle, volume);
        }

        auto SoundSource::FadeVolume(const f32 toVolume, const f32 fadeTime) -> void
        {
            m_soundSystem->GetRawHandle().fadeVolume(m_handle, toVolume, fadeTime);
        }

        auto SoundSource::FadeRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fadeTime) -> void
        {
            m_soundSystem->GetRawHandle().fadeRelativePlaySpeed(m_handle, toRelativePlaySpeed, fadeTime);
        }

        auto SoundSource::FadePan(const f32 toPan, const f32 fadeTime) -> void
        {
            m_soundSystem->GetRawHandle().fadePan(m_handle, glm::clamp(toPan, -1.0f, 1.0f), fadeTime);
        }

        [[nodiscard]] auto SoundSource::GetPan() const -> f32
        {
            return m_soundSystem->GetRawHandle().getPan(m_handle);
        }

        auto SoundSource::SetPan(const f32 pan) -> void
        {
            m_soundSystem->GetRawHandle().setPan(m_handle, glm::clamp(pan, -1.0f, 1.0f));
        }

        auto SoundSource::SetPanAbsolute(const f32 leftVolume, const f32 rightVolume) -> void
        {
            m_soundSystem->GetRawHandle().setPanAbsolute(m_handle, leftVolume, rightVolume);
        }

        [[nodiscard]] auto SoundSource::GetRelativePlaySpeed() const -> f32
        {
            return m_soundSystem->GetRawHandle().getRelativePlaySpeed(m_handle);
        }

        auto SoundSource::SetRelativePlaySpeed(const f32 relativePlaySpeed) -> void
        {
            m_soundSystem->GetRawHandle().setRelativePlaySpeed(m_handle, relativePlaySpeed);
        }

        [[nodiscard]] auto SoundSource::GetSampleRate() const -> f32
        {
            return m_soundSystem->GetRawHandle().getSamplerate(m_handle);
        }

        auto SoundSource::SetSampleRate(const f32 sampleRate) -> void
        {
            m_soundSystem->GetRawHandle().setSamplerate(m_handle, sampleRate);
        }

        auto SoundSource::OscillateVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) -> void
        {
            m_soundSystem->GetRawHandle().oscillateVolume(m_handle, fromVolume, toVolume, frequency);
        }

        auto SoundSource::ResetVolumeOscillation() -> void
        {
            m_soundSystem->GetRawHandle().oscillateVolume(m_handle, 1.0f, 1.0f, 0.0);
        }

        auto SoundSource::OscillatePan(const f32 toPan, const f32 fromPan, const f32 frequency) -> void
        {
            m_soundSystem->GetRawHandle().oscillatePan(m_handle, fromPan, toPan, frequency);
        }

        auto SoundSource::ResetPanOscillation() -> void
        {
            m_soundSystem->GetRawHandle().oscillatePan(m_handle, 0.0f, 0.0f, 0.0);
        }

        auto SoundSource::OscillateRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fromRelativePlaySpeed, const f32 frequency) -> void
        {
            m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, fromRelativePlaySpeed, toRelativePlaySpeed, frequency);
        }

        auto SoundSource::ResetRelativePlaySpeedOscillation() -> void
        {
            m_soundSystem->GetRawHandle().oscillateRelativePlaySpeed(m_handle, 1.0f, 1.0f, 0.0);
        }

        [[nodiscard]] auto SoundSource::IsProtected() const -> bool
        {
            return m_soundSystem->GetRawHandle().getProtectVoice(m_handle);
        }

        auto SoundSource::EnableProtection(const bool isProtected) -> void
        {
            m_soundSystem->GetRawHandle().setProtectVoice(m_handle, isProtected);
        }
    }
}
