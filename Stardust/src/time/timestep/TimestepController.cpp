#include "stardust/time/timestep/TimestepController.h"

#include <thread>

namespace stardust
{
    auto TimestepController::Start() -> void
    {
        m_tickCount = Clock::now();
        m_fixedTimeAccumulator = 0.0;
    }

    auto TimestepController::Update(f64& elapsedTime) -> void
    {
        m_skipNextFrame = false;

        const auto newTickCount = Clock::now();
        const auto frameTicks = newTickCount - m_tickCount;
        m_tickCount = newTickCount;

        m_deltaTime = static_cast<f64>(frameTicks.count()) /
            static_cast<f64>(Clock::period::den);

        elapsedTime += m_deltaTime;

        m_deltaTime = std::min(m_deltaTime, s_MaxDeltaTime);
        m_fixedTimeAccumulator += m_deltaTime;

        if (m_isSynchronisingToTargetFrameRate)
        {
            m_isSynchronisingToTargetFrameRate = elapsedTime < m_targetFrameRateSynchronisationEnd;
        }
        else if (m_isFrameRateTargettingEnabled)
        {
            if (m_deltaTime < m_minFrameTime)
            {
                m_isSynchronisingToTargetFrameRate = true;
                m_targetFrameRateSynchronisationEnd = elapsedTime + (m_minFrameTime - m_deltaTime);
            }
            else if (m_deltaTime > m_minFrameTime && m_frameSkipType != FrameSkipType::Never)
            {
                switch (m_frameSkipType)
                {
                case FrameSkipType::Rarely:
                case FrameSkipType::Sometimes:
                    ++m_frameSkipCounter;

                    if (m_frameSkipCounter != static_cast<u32>(m_frameSkipType))
                    {
                        break;
                    }

                    [[fallthrough]];

                case FrameSkipType::Always:
                    m_skipNextFrame = true;
                    m_frameSkipCounter = 0u;

                    break;
                }
            }
        }
    }

    auto TimestepController::UpdateFixedTimeInterpolation() noexcept -> void
    {
        m_fixedTimeInterpolation = static_cast<f32>(m_fixedTimeAccumulator / m_fixedTimestep);
    }

    auto TimestepController::EnableTargettingFrameRate(const f64 frameRate) noexcept -> void
    {
        m_isFrameRateTargettingEnabled = true;
        SetTargetFrameRate(frameRate);
    }

    auto TimestepController::DisableTargettingFrameRate() noexcept -> void
    {
        m_isFrameRateTargettingEnabled = false;
        m_isSynchronisingToTargetFrameRate = false;
    }

    auto TimestepController::SetTargetFrameRate(const f64 frameRate) noexcept -> void
    {
        m_targetFrameRate = frameRate;
        m_minFrameTime = 1.0 / m_targetFrameRate;
    }
}
