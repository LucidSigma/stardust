#include "stardust/text/text_input/TextInput.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace text_input
    {
        auto Start() -> void
        {
            if (!IsActive())
            {
                SDL_StartTextInput();
            }
        }
        
        auto Stop() -> void
        {
            if (IsActive())
            {
                SDL_StopTextInput();
            }
        }
        
        [[nodiscard]] auto IsActive() -> bool
        {
            return SDL_IsTextInputActive() == SDL_TRUE;
        }
    }
}
