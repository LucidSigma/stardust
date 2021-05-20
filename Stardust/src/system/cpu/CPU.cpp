#include "stardust/system/cpu/CPU.h"

#include <utility>

#include <cpu_features/cpuinfo_x86.h>
#include <SDL2/SDL.h>

namespace stardust
{
    namespace system
    {
        namespace
        {
            CPUInfo cpuInfo{ };
            bool hasCPUInfoBeenQueried = false;
        }

        [[nodiscard]] u32 GetCPUCount()
        {
            return static_cast<u32>(SDL_GetCPUCount());
        }
        
        [[nodiscard]] const CPUInfo& GetCPUInfo()
        {
        #ifndef CPU_FEATURES_ARCH_X86
            #error "Non-x86 architecture CPU detected."
        #endif

            if (!hasCPUInfoBeenQueried)
            {
                String cpuBrandString(49u, ' ');
                cpu_features::FillX86BrandString(cpuBrandString.data());
                cpuInfo.name = std::move(cpuBrandString);

                const cpu_features::X86Info queriedCPUInfo = cpu_features::GetX86Info();

                cpuInfo.vendor = queriedCPUInfo.vendor;
                cpuInfo.family = static_cast<u32>(queriedCPUInfo.family);
                cpuInfo.model = static_cast<u32>(queriedCPUInfo.model);
                cpuInfo.steppingLevel = static_cast<u32>(queriedCPUInfo.stepping);

                const cpu_features::CacheInfo queriedCPUCacheInfo = cpu_features::GetX86CacheInfo();
                HashSet<i32> visitedCacheLevels{ };

                for (usize i = 0u; i < static_cast<usize>(queriedCPUCacheInfo.size); ++i)
                {
                    const cpu_features::CacheLevelInfo currentCacheLevelInfo = queriedCPUCacheInfo.levels[i];

                    if (visitedCacheLevels.contains(currentCacheLevelInfo.level))
                    {
                        continue;
                    }

                    cpuInfo.caches.push_back(CPUInfo::CacheInfo{
                        .level = static_cast<u32>(currentCacheLevelInfo.level),
                        .size = static_cast<u32>(currentCacheLevelInfo.cache_size),
                        .lineCount = static_cast<u32>(currentCacheLevelInfo.partitioning),
                        .lineSize = static_cast<u32>(currentCacheLevelInfo.line_size),
                    });

                    visitedCacheLevels.insert(currentCacheLevelInfo.level);
                }

                hasCPUInfoBeenQueried = true;
            }

            return cpuInfo;
        }
    }
}