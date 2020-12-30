#pragma once
#ifndef STARDUST_TASK_H
#define STARDUST_TASK_H

#include <chrono>

#include <psched/psched.h>

namespace stardust
{
	template <
		typename Threads = psched::threads<3u>,
		typename Queues = psched::queues<3u, psched::maintain_size<100u, psched::discard::oldest_task>>,
		typename AgingPolicy = psched::aging_policy<psched::task_starvation_after<std::chrono::milliseconds, 250u>, psched::increment_priority_by<1>>
	>
	using Scheduler = psched::PriorityScheduler<Threads, Queues, AgingPolicy>;

	using Task = psched::Task;
	using TaskStats = psched::TaskStats;
}

#endif