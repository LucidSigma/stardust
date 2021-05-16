#include "stardust/time/Stopwatch.h"

namespace stardust
{
    Stopwatch::Stopwatch(const bool startTicking)
    {
        if (startTicking)
        {
            Start();
        }
    }

    void Stopwatch::Start()
    {
        m_startTicks = GetTimeSinceEpoch();
        m_pausedTickCount = 0u;

        m_isRunning = true;
        m_isPaused = false;
    }

    void Stopwatch::Stop()
    {
        m_isRunning = false;
        m_isPaused = false;

        m_startTicks = 0u;
        m_pausedTickCount = 0u;
    }

    void Stopwatch::Restart()
    {
        Stop();
        Start();
    }

    void Stopwatch::Pause()
    {
        if (m_isRunning && !m_isPaused)
        {
            m_pausedTickCount = GetTimeSinceEpoch() - m_startTicks;
            m_startTicks = 0u;

            m_isPaused = true;
        }
    }

    void Stopwatch::Resume()
    {
        if (m_isRunning && m_isPaused)
        {
            m_startTicks = GetTimeSinceEpoch() - m_pausedTickCount;
            m_pausedTickCount = 0u;

            m_isPaused = false;
        }
    }

    [[nodiscard]] u32 Stopwatch::GetElapsedSeconds() const
    {
        return static_cast<u32>(GetElapsedTime(1u));
    }

    [[nodiscard]] u64 Stopwatch::GetElapsedMilliseconds() const
    {
        return GetElapsedTime(static_cast<u64>(std::milli::den));
    }

    [[nodiscard]] u64 Stopwatch::GetElapsedMicroseconds() const
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

    [[nodiscard]] u64 Stopwatch::GetElapsedNanoseconds() const
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
    
    [[nodiscard]] u64 Stopwatch::GetTimeSinceEpoch() const
    {
        return static_cast<u64>(std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now().time_since_epoch()).count());
    }

    [[nodiscard]] u64 Stopwatch::GetElapsedTime(const u64 magnitude) const
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