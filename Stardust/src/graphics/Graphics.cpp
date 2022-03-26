#include "stardust/graphics/Graphics.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace graphics
    {
        auto SetPresentationMode(const PresentationMode presentationMode) -> void
        {
            SDL_GL_SetSwapInterval(static_cast<i32>(presentationMode));
        }
    }
}
