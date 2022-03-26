#pragma once
#ifndef STARDUST_TIMESTEP_CONTROLLER_H
#define STARDUST_TIMESTEP_CONTROLLER_H

#include <chrono>

#include "stardust/types/Primitives.h"

namespace stardust
{
    class TimestepController final
    {
    public:
        enum class FrameSkipType
            : u32
        {
            Never = 0u,
            Rarely = 4u,
            Sometimes = 2u,
            Always = 1u,
        };

    private:
        using Clock = std::chrono::steady_clock;

        static constexpr f64 s_MaxDeltaTime = 0.25;

        f64 m_fixedTimestep = 0.0;
        f64 m_fixedTimeAccumulator = 0.0;
        f64 m_deltaTime = 0.0;

        f32 m_fixedTimeInterpolation = 0.0f;

        std::chrono::time_point<Clock> m_tickCount{ };

        bool m_isFrameRateTargettingEnabled = false;
        f64 m_targetFrameRate = 0.0;
        f64 m_minFrameTime = 0.0;

        bool m_isSynchronisingToTargetFrameRate = false;
        f64 m_targetFrameRateSynchronisationEnd = 0.0f;

        FrameSkipType m_frameSkipType = FrameSkipType::Never;
        u32 m_frameSkipCounter = 0u;
        bool m_skipNextFrame = false;

    public:
        auto Start() -> void;
        auto Update(f64& elapsedTime) -> void;

        [[nodiscard]] inline auto HasFixedTimeAccumulated() const noexcept -> bool { return m_fixedTimeAccumulator >= m_fixedTimestep; }
        inline auto UpdateFixedTime() noexcept -> void { m_fixedTimeAccumulator -= m_fixedTimestep; }

        [[nodiscard]] inline auto GetFixedTimestep() const noexcept -> f64 { return m_fixedTimestep; }
        inline auto SetFixedTimestep(const f64 fixedTimestep) noexcept -> void { m_fixedTimestep = fixedTimestep; }

        [[nodiscard]] inline auto GetDeltaTime() const noexcept -> f64 { return m_deltaTime; }

        auto UpdateFixedTimeInterpolation() noexcept -> void;
        [[nodiscard]] inline auto GetFixedTimeInterpolation() const noexcept -> f32 { return m_fixedTimeInterpolation; }

        [[nodiscard]] inline auto IsFrameRateTargetting() const noexcept -> bool { return m_isFrameRateTargettingEnabled; }
        auto EnableTargettingFrameRate(const f64 frameRate) noexcept -> void;
        auto DisableTargettingFrameRate() noexcept -> void;
        [[nodiscard]] auto IsSynchronisingToTargetFrameRate() const noexcept -> bool { return m_isSynchronisingToTargetFrameRate; }

        [[nodiscard]] inline auto GetTargetFrameRate() const noexcept -> f64 { return m_targetFrameRate; }
        auto SetTargetFrameRate(const f64 frameRate) noexcept -> void;

        [[nodiscard]] auto GetFrameSkipType() const noexcept -> FrameSkipType { return m_frameSkipType; }
        auto SetFrameSkipType(const FrameSkipType frameSkipType) noexcept -> void { m_frameSkipType = frameSkipType; }
        [[nodiscard]] inline auto SkipNextFrame() const noexcept -> bool { return m_skipNextFrame; }
    };
}

#endif
