#include "stardust/debug/Debug.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace debug
    {
        void TriggerBreakpoint()
        {
            SDL_TriggerBreakpoint();
        }
    }
}