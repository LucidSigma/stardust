#include "stardust/task/TaskStats.h"

namespace stardust
{
    TaskStats::TaskStats(const psched::TaskStats& taskStats)
        : m_stats(taskStats)
    { }
}