#pragma once
#ifndef STARDUST_STOPWATCH_H
#define STARDUST_STOPWATCH_H

#include <chrono>
#include <ratio>

#include "stardust/types/Primitives.h"

namespace stardust
{
    class Stopwatch final
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
        [[nodiscard]] static constexpr auto GetPrecision() noexcept -> u64 { return s_Precision; }

        [[nodiscard]] static constexpr auto CanCountMicroseconds() noexcept -> bool { return s_CanCountMicroseconds; }
        [[nodiscard]] static constexpr auto CanCountNanoseconds() noexcept -> bool { return s_CanCountNanoseconds; }

        explicit Stopwatch(const bool startTicking = false);

        auto Start() -> void;
        auto Stop() -> void;
        auto Restart() -> void;

        auto Pause() -> void;
        auto Resume() -> void;

        [[nodiscard]] inline auto IsRunning() const noexcept -> bool { return m_isRunning && !m_isPaused; }
        [[nodiscard]] inline auto IsStopped() const noexcept -> bool { return !m_isRunning; }
        [[nodiscard]] inline auto IsPaused() const noexcept -> bool { return m_isRunning && m_isPaused; }

        [[nodiscard]] auto GetElapsedSeconds() const -> u32;
        [[nodiscard]] auto GetElapsedMilliseconds() const -> u64;
        [[nodiscard]] auto GetElapsedMicroseconds() const -> u64;
        [[nodiscard]] auto GetElapsedNanoseconds() const -> u64;

        [[nodiscard]] inline auto HasPassedSeconds(const u32 seconds) const -> bool { return GetElapsedSeconds() >= seconds; }
        [[nodiscard]] inline auto HasPassedMilliSeconds(const u64 milliseconds) const -> bool { return GetElapsedMilliseconds() >= milliseconds; }
        [[nodiscard]] inline auto HasPassedMicroSeconds(const u64 microseconds) const -> bool { return GetElapsedMicroseconds() >= microseconds; }
        [[nodiscard]] inline auto HasPassedNanoSeconds(const u64 nanoseconds) const -> bool { return GetElapsedNanoseconds() >= nanoseconds; }

    private:
        [[nodiscard]] auto GetTimeSinceEpoch() const -> u64;
        [[nodiscard]] auto GetElapsedTime(const u64 magnitude) const -> u64;
    };
}

#endif
