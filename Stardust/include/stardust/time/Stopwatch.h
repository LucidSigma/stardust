#pragma once
#ifndef STARDUST_STOPWATCH_H
#define STARDUST_STOPWATCH_H

#include <chrono>
#include <ratio>

#include "stardust/data/Types.h"

namespace stardust
{
    class Stopwatch
    {
    private:
        using Clock = std::chrono::high_resolution_clock;

        static constexpr u64 s_Precision = static_cast<u64>(Clock::period::den);

        static constexpr bool s_CanCountMicroseconds = std::ratio_greater_equal_v<std::micro, Clock::period>;
        static constexpr bool s_CanCountNanoseconds = std::ratio_greater_equal_v<std::nano, Clock::period>;

        bool m_isRunning = false;
        bool m_isPaused = false;

        u64 m_startTicks = 0u;
        u64 m_pausedTickCount = 0u;

    public:
        static constexpr u64 GetPrecision() noexcept { return s_Precision; }

        static constexpr bool CanCountMicroseconds() noexcept { return s_CanCountMicroseconds; }
        static constexpr bool CanCountNanoseconds() noexcept { return s_CanCountNanoseconds; }

        explicit Stopwatch(const bool startTicking = false);
        ~Stopwatch() noexcept = default;

        void Start();
        void Stop();
        void Restart();

        void Pause();
        void Resume();

        inline bool IsRunning() const noexcept { return m_isRunning && !m_isPaused; }
        inline bool IsStopped() const noexcept { return !m_isRunning; }
        inline bool IsPaused() const noexcept { return m_isRunning && m_isPaused; }

        [[nodiscard]] u32 GetElapsedSeconds() const;
        [[nodiscard]] u64 GetElapsedMilliseconds() const;
        [[nodiscard]] u64 GetElapsedMicroseconds() const;
        [[nodiscard]] u64 GetElapsedNanoseconds() const;
        
        [[nodiscard]] inline bool HasPassedSeconds(const u32 seconds) const { return GetElapsedSeconds() >= seconds; }
        [[nodiscard]] inline bool HasPassedMilliSeconds(const u64 milliseconds) const { return GetElapsedMilliseconds() >= milliseconds; }
        [[nodiscard]] inline bool HasPassedMicroSeconds(const u64 microseconds) const { return GetElapsedMicroseconds() >= microseconds; }
        [[nodiscard]] inline bool HasPassedNanoSeconds(const u64 nanoseconds) const { return GetElapsedNanoseconds() >= nanoseconds; }

    private:
        [[nodiscard]] u64 GetTimeSinceEpoch() const;
        [[nodiscard]] u64 GetElapsedTime(const u64 magnitude) const;
    };
}

#endif