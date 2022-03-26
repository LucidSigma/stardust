
#pragma once
#ifndef STARDUST_SOUND_SOURCE_H
#define STARDUST_SOUND_SOURCE_H

#include <soloud/soloud.h>
#undef min
#undef max

#include "stardust/audio/Audio.h"
#include "stardust/math/Math.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace audio
    {
        class SoundSource final
        {
        private:
            ObserverPointer<class SoundSystem> m_soundSystem = nullptr;
            VoiceHandle m_handle = InvalidVoiceHandle;

            ObserverPointer<class MixingBus> m_mixingBus = nullptr;

        public:
            SoundSource() = default;
            SoundSource(const VoiceHandle handle, class SoundSystem& soundSystem, const ObserverPointer<class MixingBus> mixingBus = nullptr);
            ~SoundSource() noexcept = default;

            auto Pause() const -> void;
            auto Resume() const -> void;
            auto Stop() const noexcept -> void;

            [[nodiscard]] auto IsPlaying() const -> bool;
            [[nodiscard]] auto IsPaused() const -> bool;
            [[nodiscard]] auto IsStopped() const -> bool;

            auto SchedulePause(const f32 secondsDelay) -> void;
            auto ScheduleStop(const f32 secondsDelay) -> void;

            [[nodiscard]] auto IsLooping() const -> bool;
            auto SetLooping(const bool isLooping) -> void;
            [[nodiscard]] auto GetLoopPoint() const ->f64;
            auto SetLoopPoint(const f64 loopPoint) -> void;
            [[nodiscard]] auto GetLoopCount() const -> u32;

            [[nodiscard]] auto GetStreamTime() const -> f64;
            auto Seek(const f64 seconds) -> void;
            auto Rewind() -> void;

            [[nodiscard]] auto GetVolume() const -> f32;
            auto SetVolume(const f32 volume) -> void;

            [[nodiscard]] auto GetPan() const -> f32;
            auto SetPan(const f32 pan) -> void;
            auto SetPanAbsolute(const f32 leftVolume, const f32 rightVolume) -> void;

            [[nodiscard]] auto GetRelativePlaySpeed() const -> f32;
            auto SetRelativePlaySpeed(const f32 relativePlaySpeed) -> void;
            [[nodiscard]] auto GetSampleRate() const -> f32;
            auto SetSampleRate(const f32 sampleRate) -> void;

            auto FadeVolume(const f32 toVolume, const f32 fadeTime) -> void;
            auto FadePan(const f32 toPan, const f32 fadeTime) -> void;
            auto FadeRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fadeTime) -> void;

            auto OscillateVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency) -> void;
            auto ResetVolumeOscillation() -> void;
            auto OscillatePan(const f32 toPan, const f32 fromPan, const f32 frequency) -> void;
            auto ResetPanOscillation() -> void;
            auto OscillateRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fromRelativePlaySpeed, const f32 frequency) -> void;
            auto ResetRelativePlaySpeedOscillation() -> void;

            [[nodiscard]] auto IsProtected() const -> bool;
            auto EnableProtection(const bool isProtected) -> void;

            [[nodiscard]] inline auto GetMixingBus() noexcept -> class MixingBus& { return *m_mixingBus; }
            [[nodiscard]] inline auto GetMixingBus() const noexcept -> const class MixingBus& { return *m_mixingBus; }
            [[nodiscard]] inline auto HasMixingBus() const noexcept -> bool { return m_mixingBus != nullptr; }

            [[nodiscard]] inline auto GetRawHandle() const noexcept -> VoiceHandle { return m_handle; }
        };
    }
}

#endif
