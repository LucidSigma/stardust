#include "stardust/system/System.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace system
    {
        [[nodiscard]] u32 GetSystemRAMCount()
        {
            return static_cast<u32>(SDL_GetSystemRAM());
        }
    }
}