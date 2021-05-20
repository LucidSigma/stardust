#pragma once
#ifndef STARDUST_CPU_H
#define STARDUST_CPU_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace system
    {
        struct CPUInfo
        {
            struct CacheInfo
            {
                u32 level;
                usize size;

                u32 lineCount;
                usize lineSize;
            };

            String name;
            String vendor;

            u32 coreCount;

            u32 family;
            u32 model;
            u32 steppingLevel;

            Vector<CacheInfo> caches;
        };

        [[nodiscard]] extern const CPUInfo& GetCPUInfo();
    }
}

#endif