#include "stardust/audio/mixing/MixingBus.h"

#include <utility>

#include "stardust/audio/source/PositionalSoundSource.h"
#include "stardust/audio/source/SoundSource.h"
#include "stardust/audio/SoundSystem.h"

namespace stardust
{
    namespace audio
    {
        MixingBus::MixingBus(SoundSystem& soundSystem)
        {
            Initialise(soundSystem);
        }

        MixingBus::MixingBus(MixingBus&& other) noexcept
        {
            Destroy();

            m_handle = std::move(other.m_handle);
            std::swap(m_soundSystem, other.m_soundSystem);
            std::swap(m_voiceHandle, other.m_voiceHandle);

            std::swap(m_bassBoostFilter, other.m_bassBoostFilter);
            std::swap(m_biquadResonantFilter, other.m_biquadResonantFilter);
            std::swap(m_echoFilter, other.m_echoFilter);
            std::swap(m_flangerFilter, other.m_flangerFilter);
            std::swap(m_reverbFilter, other.m_reverbFilter);
            std::swap(m_loFiFilter, other.m_loFiFilter);
        }

        auto MixingBus::operator =(MixingBus&& other) noexcept -> MixingBus&
        {
            Destroy();

            m_handle = std::move(other.m_handle);
            std::swap(m_soundSystem, other.m_soundSystem);
            std::swap(m_voiceHandle, other.m_voiceHandle);
            std::swap(m_defaultVolume, other.m_defaultVolume);

            std::swap(m_bassBoostFilter, other.m_bassBoostFilter);
            std::swap(m_biquadResonantFilter, other.m_biquadResonantFilter);
            std::swap(m_echoFilter, other.m_echoFilter);
            std::swap(m_flangerFilter, other.m_flangerFilter);
            std::swap(m_reverbFilter, other.m_reverbFilter);
            std::swap(m_loFiFilter, other.m_loFiFilter);

            return *this;
        }

        MixingBus::~MixingBus() noexcept
        {
            Destroy();
        }

        auto MixingBus::Initialise(SoundSystem& soundSystem) -> void
        {
            m_soundSystem = &soundSystem;
        }

        auto MixingBus::Destroy() noexcept -> void
        {
            if (m_soundSystem != nullptr)
            {
                if (IsPlaying())
                {
                    ClearAllFilters();
                    Stop();
                }

                m_bassBoostFilter = None;
                m_biquadResonantFilter = None;
                m_echoFilter = None;
                m_flangerFilter = None;
                m_reverbFilter = None;
                m_loFiFilter = None;

                m_soundSystem = nullptr;
                m_defaultVolume = 1.0f;
            }
        }

        auto MixingBus::Play() -> void
        {
            m_voiceHandle = m_soundSystem->GetRawHandle().play(m_handle, m_defaultVolume);
        }

        auto MixingBus::Stop() noexcept -> void
        {
            if (m_voiceHandle != InvalidVoiceHandle)
            {
                m_handle.stop();
                m_voiceHandle = InvalidVoiceHandle;
            }
        }

        auto MixingBus::PlaySound(Sound& sound, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_handle.play(sound.GetRawHandle(), soundPlayData.volume, soundPlayData.pan, soundPlayData.startPaused);
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *m_soundSystem, this);
        }

        auto MixingBus::PlaySound(SoundStream& soundStream, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_handle.play(soundStream.GetRawHandle(), soundPlayData.volume, soundPlayData.pan);
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *m_soundSystem, this);
        }

        auto MixingBus::PlaySoundClocked(Sound& sound, const f64 fixedTimestep, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_handle.playClocked(fixedTimestep, sound.GetRawHandle(), soundPlayData.volume, soundPlayData.pan);
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *m_soundSystem, this);
        }

        auto MixingBus::PlaySoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const SoundPlayData& soundPlayData) -> SoundSource
        {
            const VoiceHandle voiceHandle = m_handle.playClocked(fixedTimestep, soundStream.GetRawHandle(), soundPlayData.volume, soundPlayData.pan);
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return SoundSource(voiceHandle, *m_soundSystem, this);
        }

        auto MixingBus::PlayPositionalSound(Sound& sound, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_handle.play3d(
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

            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *m_soundSystem, this, soundPlayData.position, soundPlayData.velocity);
        }

        auto MixingBus::PlayPositionalSound(SoundStream& soundStream, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_handle.play3d(
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
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *m_soundSystem, this, soundPlayData.position, soundPlayData.velocity);
        }

        auto MixingBus::PlayPositionalSoundClocked(Sound& sound, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_handle.play3dClocked(
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
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *m_soundSystem, this, soundPlayData.position, soundPlayData.velocity);
        }

        auto MixingBus::PlayPositionalSoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData) -> PositionalSoundSource
        {
            const VoiceHandle voiceHandle = m_handle.play3dClocked(
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
            m_soundSystem->SetVoiceHandleParameters(voiceHandle, soundPlayData);

            return PositionalSoundSource(voiceHandle, *m_soundSystem, this, soundPlayData.position, soundPlayData.velocity);
        }

        [[nodiscard]] auto MixingBus::GetPlayingSoundCount() -> u32
        {
            return m_handle.getActiveVoiceCount();
        }

        [[nodiscard]] auto MixingBus::IsAnySoundPlaying() -> bool
        {
            return m_handle.getActiveVoiceCount() > 0u;
        }

        auto MixingBus::SetDefaultVolume(const f32 defaultVolume) -> void
        {
            m_defaultVolume = defaultVolume;
            m_handle.setVolume(defaultVolume);
        }

        auto MixingBus::SetBassBoostFilter(const f32 boostAmount) -> void
        {
            if (!m_bassBoostFilter.has_value())
            {
                m_bassBoostFilter = SoLoud::BassboostFilter();
            }

            m_bassBoostFilter.value().setParams(boostAmount);
            m_handle.setFilter(static_cast<u32>(FilterSlot::BassBoost), &m_bassBoostFilter.value());
        }

        auto MixingBus::ClearBassBoostFilter() -> void
        {
            if (m_bassBoostFilter.has_value())
            {
                m_handle.setFilter(static_cast<u32>(FilterSlot::BassBoost), nullptr);
                m_bassBoostFilter = None;
            }
        }

        auto MixingBus::SetBiquadResonantFilter(const BiquadResonantFilterType type, const f32 frequency, const f32 resonance) -> void
        {
            if (!m_biquadResonantFilter.has_value())
            {
                m_biquadResonantFilter = SoLoud::BiquadResonantFilter();
            }

            m_biquadResonantFilter.value().setParams(static_cast<i32>(type), frequency, resonance);
            m_handle.setFilter(static_cast<u32>(FilterSlot::BiquadResonant), &m_biquadResonantFilter.value());
        }

        auto MixingBus::ClearBiquadResonantFilter() -> void
        {
            if (m_biquadResonantFilter.has_value())
            {
                m_handle.setFilter(static_cast<u32>(FilterSlot::BiquadResonant), nullptr);
                m_biquadResonantFilter = None;
            }
        }

        auto MixingBus::SetEchoFilter(const f32 delay, const f32 decay) -> void
        {
            if (!m_echoFilter.has_value())
            {
                m_echoFilter = SoLoud::EchoFilter();
            }

            m_echoFilter.value().setParams(delay, decay);
            m_handle.setFilter(static_cast<u32>(FilterSlot::Echo), &m_echoFilter.value());
        }

        auto MixingBus::ClearEchoFilter() -> void
        {
            if (m_echoFilter.has_value())
            {
                m_handle.setFilter(static_cast<u32>(FilterSlot::Echo), nullptr);
                m_echoFilter = None;
            }
        }

        auto MixingBus::SetFlangerFilter(const f32 delay, const f32 frequency) -> void
        {
            if (!m_flangerFilter.has_value())
            {
                m_flangerFilter = SoLoud::FlangerFilter();
            }

            m_flangerFilter.value().setParams(delay, frequency);
            m_handle.setFilter(static_cast<u32>(FilterSlot::Flanger), &m_flangerFilter.value());
        }

        auto MixingBus::ClearFlangerFilter() -> void
        {
            if (m_flangerFilter.has_value())
            {
                m_handle.setFilter(static_cast<u32>(FilterSlot::Flanger), nullptr);
                m_flangerFilter = None;
            }
        }

        auto MixingBus::SetReverbFilter(const f32 roomSize, const f32 damping, const f32 width) -> void
        {
            if (!m_reverbFilter.has_value())
            {
                m_reverbFilter = SoLoud::FreeverbFilter();
            }

            m_reverbFilter.value().setParams(0.0f, roomSize, damping, width);
            m_handle.setFilter(static_cast<u32>(FilterSlot::Reverb), &m_reverbFilter.value());
        }

        auto MixingBus::ClearReverbFilter() -> void
        {
            if (m_reverbFilter.has_value())
            {
                m_handle.setFilter(static_cast<u32>(FilterSlot::Reverb), nullptr);
                m_reverbFilter = None;
            }
        }

        auto MixingBus::SetLoFiFilter(const f32 sampleRate, const f32 bitDepth) -> void
        {
            if (!m_loFiFilter.has_value())
            {
                m_loFiFilter = SoLoud::LofiFilter();
            }

            m_loFiFilter.value().setParams(sampleRate, bitDepth);
            m_handle.setFilter(static_cast<u32>(FilterSlot::LoFi), &m_loFiFilter.value());
        }

        auto MixingBus::ClearLoFiFilter() -> void
        {
            if (m_loFiFilter.has_value())
            {
                m_handle.setFilter(static_cast<u32>(FilterSlot::LoFi), nullptr);
                m_loFiFilter = None;
            }
        }

        auto MixingBus::ClearAllFilters() -> void
        {
            ClearBassBoostFilter();
            ClearBiquadResonantFilter();
            ClearEchoFilter();
            ClearFlangerFilter();
            ClearReverbFilter();
            ClearLoFiFilter();
        }
    }
}
