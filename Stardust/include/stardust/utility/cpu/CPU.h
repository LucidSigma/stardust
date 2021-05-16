#pragma once
#ifndef STARDUST_CPU_H
#define STARDUST_CPU_H

#include "stardust/data/Types.h"

namespace stardust
{
    namespace cpu
    {
        [[nodiscard]] extern u32 GetL1CacheLineSize();
        [[nodiscard]] extern u32 GetCPUCount();
        [[nodiscard]] extern u32 GetSystemRAMCount();
    }
}

#endif