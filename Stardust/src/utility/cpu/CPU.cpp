#include "stardust/utility/cpu/CPU.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace cpu
    {
        [[nodiscard]] u32 GetL1CacheLineSize()
        {
            return static_cast<u32>(SDL_GetCPUCacheLineSize());
        }

        [[nodiscard]] u32 GetCPUCount()
        {
            return static_cast<u32>(SDL_GetCPUCount());
        }

        [[nodiscard]] u32 GetSystemRAMCount()
        {
            return static_cast<u32>(SDL_GetSystemRAM());
        }
    }
}