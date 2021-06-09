#pragma once
#ifndef STARDUST_TASK_STATS_H
#define STARDUST_TASK_STATS_H

#include <chrono>

#include <psched/psched.h>

#include "stardust/data/Types.h"

namespace stardust
{
    class TaskStats
    {
    private:
        psched::TaskStats m_stats{ };

    public:
        TaskStats(const psched::TaskStats& taskStats);
        ~TaskStats() noexcept = default;

        [[nodiscard]] inline const std::chrono::steady_clock::time_point& GetArrivalTimePoint() const noexcept { return m_stats.arrival_time; }
        [[nodiscard]] inline const std::chrono::steady_clock::time_point& GetStartTimePoint() const noexcept { return m_stats.start_time; }
        [[nodiscard]] inline const std::chrono::steady_clock::time_point& GetFinishTimePoint() const noexcept { return m_stats.end_time; }

        template <typename Duration = std::chrono::milliseconds>
            requires psched::is_chrono_duration<Duration>::value
        [[nodiscard]] inline u64 GetWaitingTime() const noexcept
        {
            return static_cast<u64>(m_stats.waiting_time());
        }

        template <typename Duration = std::chrono::milliseconds>
            requires psched::is_chrono_duration<Duration>::value
        [[nodiscard]] inline u64 GetBurstTime() const noexcept
        {
            return static_cast<u64>(m_stats.burst_time());
        }

        template <typename Duration = std::chrono::milliseconds>
            requires psched::is_chrono_duration<Duration>::value
        [[nodiscard]] inline u64 GetTurnaroundTime() const noexcept
        {
            return static_cast<u64>(m_stats.turnaround_time());
        }
    };
}

#endif