#pragma once
#ifndef STARDUST_SOUND_SOURCE_H
#define STARDUST_SOUND_SOURCE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <limits>

#include <soloud/soloud.h>
#undef min
#undef max

#include "stardust/audio/sounds/SoundEnums.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace audio
    {
        class SoundSource
            : private INoncopyable
        {
        private:
            class SoundSystem* m_soundSystem = nullptr;
            SoLoud::handle m_handle = std::numeric_limits<SoLoud::handle>::max();

            Vec3 m_position = Vec3Zero;
            Vec3 m_velocity = Vec3Zero;

            f32 m_minDistance = 1.0f;
            f32 m_maxDistance = 1'000'000.0f;

            InaudibleBehaviour m_inaudibleBehaviour = InaudibleBehaviour::Pause;

        public:
            SoundSource() = default;
            SoundSource(const SoLoud::handle handle, class SoundSystem& soundSystem, const Vec3& position = Vec3Zero, const Vec3& velocity = Vec3Zero);
            SoundSource(SoundSource&& other) noexcept;
            SoundSource& operator =(SoundSource&& other) noexcept;
            ~SoundSource() noexcept = default;

            [[nodiscard]] inline bool HasValidHandle() const noexcept { return m_soundSystem != nullptr; }

            void Pause() const;
            void Resume() const;
            void Stop() const;

            [[nodiscard]] bool IsPlaying() const;
            [[nodiscard]] bool IsPaused() const;
            [[nodiscard]] bool IsStopped() const;

            void SchedulePause(const f32 secondsDelay);
            void ScheduleStop(const f32 secondsDelay);

            [[nodiscard]] bool IsLooping() const;
            void SetLooping(const bool isLooping);
            [[nodiscard]] f32 GetLoopPoint() const;
            void SetLoopPoint(const f32 loopPoint);
            [[nodiscard]] unsigned int GetLoopCount() const;

            [[nodiscard]] f64 GetStreamTime() const;
            void Seek(const f64 seconds);
            void Rewind();

            [[nodiscard]] f32 GetVolume() const;
            [[nodiscard]] f32 GetOverallVolume() const;
            void SetVolume(const f32 volume);

            [[nodiscard]] f32 GetPan() const;
            void SetPan(const f32 pan);
            void SetPanAbsolute(const f32 leftVolume, const f32 rightVolume);

            [[nodiscard]] f32 GetRelativePlaySpeed() const;
            void SetRelativePlaySpeed(const f32 relativePlaySpeed);
            [[nodiscard]] f32 GetSampleRate() const;
            void SetSampleRate(const f32 sampleRate);

            void FadeVolume(const f32 toVolume, const f32 fadeTime);
            void FadePan(const f32 toPan, const f32 fadeTime);
            void FadeRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fadeTime);

            void OscillateVolume(const f32 toVolume, const f32 fromVolume, const f32 frequency);
            void ResetVolumeOscillation();
            void OscillatePan(const f32 toPan, const f32 fromPan, const f32 frequency);
            void ResetPanOscillation();
            void OscillateRelativePlaySpeed(const f32 toRelativePlaySpeed, const f32 fromRelativePlaySpeed, const f32 frequency);
            void ResetRelativePlaySpeedOscillation();

            [[nodiscard]] inline const Vec3& GetPosition() const noexcept { return m_position; }
            void SetPosition(const Vec3& position);
            [[nodiscard]] inline const Vec3& GetVelocity() const noexcept { return m_velocity; }
            void SetVelocity(const Vec3& velocity);

            [[nodiscard]] inline f32 GetMinDistance() const noexcept { return m_minDistance; }
            void SetMinDistance(const f32 minDistance);
            [[nodiscard]] inline f32 GetMaxDistance() const noexcept { return m_maxDistance; }
            void SetMaxDistance(const f32 maxDistance);

            [[nodiscard]] inline InaudibleBehaviour GetInaudibleBehaviour() const noexcept { return m_inaudibleBehaviour; }
            void SetInaudibleBehaviour(const InaudibleBehaviour inaudibleBehaviour);

            [[nodiscard]] bool IsProtected() const;
            void SetProtection(const bool isProtected);

            [[nodiscard]] inline SoLoud::handle GetRawHandle() const noexcept { return m_handle; }
        };
    }
}

#endif