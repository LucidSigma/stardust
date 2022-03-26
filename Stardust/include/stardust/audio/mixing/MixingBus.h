#pragma once
#ifndef STARDUST_MIXING_BUS_H
#define STARDUST_MIXING_BUS_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <soloud/soloud.h>
#include <soloud/soloud_bassboostfilter.h>
#include <soloud/soloud_biquadresonantfilter.h>
#include <soloud/soloud_echofilter.h>
#include <soloud/soloud_flangerfilter.h>
#include <soloud/soloud_freeverbfilter.h>
#include <soloud/soloud_lofifilter.h>
#undef min
#undef max

#include "stardust/audio/sounds/Sounds.h"
#include "stardust/audio/source/PositionalSoundSource.h"
#include "stardust/audio/source/SoundSource.h"
#include "stardust/audio/Audio.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        class MixingBus final
            : private INoncopyable
        {
        public:
            enum class BiquadResonantFilterType
                : std::underlying_type_t<SoLoud::BiquadResonantFilter::FILTERTYPE>
            {
                LowPass = SoLoud::BiquadResonantFilter::LOWPASS,
                HighPass = SoLoud::BiquadResonantFilter::HIGHPASS,
                BandPass = SoLoud::BiquadResonantFilter::BANDPASS,
            };

        private:
            enum class FilterSlot
                : u32
            {
                BassBoost = 0u,
                BiquadResonant = 1u,
                Echo = 2u,
                Flanger = 3u,
                Reverb = 4u,
                LoFi = 5u,
            };

            static_assert(FILTERS_PER_STREAM <= 8);

            SoLoud::Bus m_handle;
            ObserverPointer<class SoundSystem> m_soundSystem = nullptr;

            f32 m_defaultVolume = 1.0f;
            VoiceHandle m_voiceHandle = InvalidVoiceHandle;

            Optional<SoLoud::BassboostFilter> m_bassBoostFilter = None;
            Optional<SoLoud::BiquadResonantFilter> m_biquadResonantFilter = None;
            Optional<SoLoud::EchoFilter> m_echoFilter = None;
            Optional<SoLoud::FlangerFilter> m_flangerFilter = None;
            Optional<SoLoud::FreeverbFilter> m_reverbFilter = None;
            Optional<SoLoud::LofiFilter> m_loFiFilter = None;

        public:
            MixingBus() = default;
            explicit MixingBus(class SoundSystem& soundSystem);
            MixingBus(MixingBus&& other) noexcept;
            auto operator =(MixingBus&& other) noexcept -> MixingBus&;
            ~MixingBus() noexcept;

            auto Initialise(class SoundSystem& soundSystem) -> void;
            auto Destroy() noexcept -> void;

            auto Play() -> void;
            auto Stop() noexcept -> void;
            [[nodiscard]] inline auto IsPlaying() const noexcept -> bool { return m_voiceHandle != InvalidVoiceHandle; }

            auto PlaySound(Sound& sound, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;
            auto PlaySound(SoundStream& soundStream, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;
            auto PlaySoundClocked(Sound& sound, const f64 fixedTimestep, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;
            auto PlaySoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const SoundPlayData& soundPlayData = SoundPlayData{ }) -> SoundSource;

            auto PlayPositionalSound(Sound& sound, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;
            auto PlayPositionalSound(SoundStream& soundStream, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;
            auto PlayPositionalSoundClocked(Sound& sound, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;
            auto PlayPositionalSoundClocked(SoundStream& soundStream, const f64 fixedTimestep, const PositionalSoundPlayData& soundPlayData = PositionalSoundPlayData{ }) -> PositionalSoundSource;

            [[nodiscard]] auto GetPlayingSoundCount() -> u32;
            [[nodiscard]] auto IsAnySoundPlaying() -> bool;

            [[nodiscard]] inline auto GetDefaultVolume() const noexcept -> f32 { return m_defaultVolume; }
            auto SetDefaultVolume(const f32 defaultVolume) -> void;

            auto SetBassBoostFilter(const f32 boostAmount) -> void;
            auto ClearBassBoostFilter() -> void;

            auto SetBiquadResonantFilter(const BiquadResonantFilterType type, const f32 frequency, const f32 resonance) -> void;
            auto ClearBiquadResonantFilter() -> void;

            auto SetEchoFilter(const f32 delay, const f32 decay) -> void;
            auto ClearEchoFilter() -> void;

            auto SetFlangerFilter(const f32 delay, const f32 frequency) -> void;
            auto ClearFlangerFilter() -> void;

            auto SetReverbFilter(const f32 roomSize, const f32 damping, const f32 width) -> void;
            auto ClearReverbFilter() -> void;

            auto SetLoFiFilter(const f32 sampleRate, const f32 bitDepth) -> void;
            auto ClearLoFiFilter() -> void;

            auto ClearAllFilters() -> void;

            [[nodiscard]] inline auto GetRawHandle() noexcept -> SoLoud::Bus& { return m_handle; }
            [[nodiscard]] inline auto GetRawHandle() const noexcept -> const SoLoud::Bus& { return m_handle; }
        };
    }
}

#endif
