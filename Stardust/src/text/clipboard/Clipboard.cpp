#include "stardust/text/clipboard/Clipboard.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace clipboard
    {
        [[nodiscard]] auto HasText() -> bool
        {
            return SDL_HasClipboardText() == SDL_TRUE;
        }

        [[nodiscard]] auto GetText() -> String
        {
            char* clipboardTextPointer = SDL_GetClipboardText();
            const String clipboardText(clipboardTextPointer);

            SDL_free(clipboardTextPointer);
            clipboardTextPointer = nullptr;

            return clipboardText;
        }

        auto SetText(const StringView text) -> void
        {
            SDL_SetClipboardText(text.data());
        }
    }
}
