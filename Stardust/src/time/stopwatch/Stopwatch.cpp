#include "stardust/time/stopwatch/Stopwatch.h"

namespace stardust
{
    Stopwatch::Stopwatch(const bool startTicking)
    {
        if (startTicking)
        {
            Start();
        }
    }

    auto Stopwatch::Start() -> void
    {
        m_startTicks = GetTimeSinceEpoch();
        m_pausedTickCount = 0u;

        m_isRunning = true;
        m_isPaused = false;
    }

    auto Stopwatch::Stop() -> void
    {
        m_isRunning = false;
        m_isPaused = false;

        m_startTicks = 0u;
        m_pausedTickCount = 0u;
    }

    auto Stopwatch::Restart() -> void
    {
        Stop();
        Start();
    }

    auto Stopwatch::Pause() -> void
    {
        if (m_isRunning && !m_isPaused)
        {
            m_pausedTickCount = GetTimeSinceEpoch() - m_startTicks;
            m_startTicks = 0u;

            m_isPaused = true;
        }
    }

    auto Stopwatch::Resume() -> void
    {
        if (m_isRunning && m_isPaused)
        {
            m_startTicks = GetTimeSinceEpoch() - m_pausedTickCount;
            m_pausedTickCount = 0u;

            m_isPaused = false;
        }
    }

    [[nodiscard]] auto Stopwatch::GetElapsedSeconds() const -> u32
    {
        return static_cast<u32>(GetElapsedTime(1u));
    }

    [[nodiscard]] auto Stopwatch::GetElapsedMilliseconds() const -> u64
    {
        return GetElapsedTime(static_cast<u64>(std::milli::den));
    }

    [[nodiscard]] auto Stopwatch::GetElapsedMicroseconds() const -> u64
    {
        if constexpr (s_CanCountMicroseconds)
        {
            return GetElapsedTime(static_cast<u64>(std::micro::den));
        }
        else
        {
            return GetElapsedMilliseconds() * 1'000u;
        }
    }

    [[nodiscard]] auto Stopwatch::GetElapsedNanoseconds() const -> u64
    {
        if constexpr (s_CanCountNanoseconds)
        {
            return GetElapsedTime(static_cast<u64>(std::nano::den));
        }
        else
        {
            return GetElapsedMicroseconds() * 1'000u;
        }
    }

    [[nodiscard]] auto Stopwatch::GetTimeSinceEpoch() const -> u64
    {
        return static_cast<u64>(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now().time_since_epoch()).count());
    }

    [[nodiscard]] auto Stopwatch::GetElapsedTime(const u64 magnitude) const -> u64
    {
        if (m_isRunning)
        {
            if (m_isPaused)
            {
                return static_cast<u64>(m_pausedTickCount / (s_Precision / magnitude));
            }
            else
            {
                return static_cast<u64>((GetTimeSinceEpoch() - m_startTicks) / (s_Precision / magnitude));
            }
        }
        else
        {
            return 0u;
        }
    }
}
