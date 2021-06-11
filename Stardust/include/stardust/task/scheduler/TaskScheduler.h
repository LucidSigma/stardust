#pragma once
#ifndef STARDUST_TASK_SCHEDULER_H
#define STARDUST_TASK_SCHEDULER_H

#include <functional>

#include <psched/psched.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/task/TaskStats.h"

namespace stardust
{
    constexpr usize NoStarvation = 0u;

    template <usize Threads = 3u, usize Queues = 3u, usize TaskStarvationMilliseconds = NoStarvation, usize StarvationPriorityIncrease = 1u>
    class TaskScheduler
    {
    private:
        psched::PriorityScheduler<
            psched::threads<Threads>,
            psched::queues<Queues>,
            psched::aging_policy<
                psched::task_starvation_after<std::chrono::milliseconds, TaskStarvationMilliseconds>,
                psched::increment_priority_by<StarvationPriorityIncrease>
            >
        > m_scheduler{ };

    public:
        TaskScheduler() = default;
        ~TaskScheduler() noexcept = default;

        template <usize Priority>
        void ScheduleTask(const std::function<void()>& task, const std::function<void(const TaskStats&)>& endCallback = [](const TaskStats&) { }, const std::function<void(const String&)>& errorCallback = [](const String&) { })
        {
            const auto convertedEndCallback = [endCallback](const psched::TaskStats& taskStats) { endCallback(TaskStats(taskStats)); };
            const auto convertedErrorCallback = [errorCallback](const char* errorMessage) { errorCallback(errorMessage); };

            psched::Task taskObject(task, convertedEndCallback, convertedErrorCallback);

            m_scheduler.schedule<psched::priority<Priority>>(taskObject);
        }

        void Stop()
        {
            m_scheduler.stop();
        }
    };
}

#endif