#include "stardust/text/clipboard/Clipboard.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace clipboard
    {
        [[nodiscard]] bool HasText()
        {
            return SDL_HasClipboardText();
        }

        [[nodiscard]] String GetText()
        {
            char* clipboardTextPointer = SDL_GetClipboardText();
            const String clipboardText(clipboardTextPointer);

            SDL_free(clipboardTextPointer);

            return clipboardText;
        }

        void SetText(const String& text)
        {
            SDL_SetClipboardText(text.c_str());
        }
    }
}