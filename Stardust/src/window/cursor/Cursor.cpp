#include "stardust/window/cursor/Cursor.h"

namespace stardust
{
    namespace
    {
        SDL_Cursor* cursor = nullptr;
    }

    void SetCursor(const CursorType cursorType)
    {
        if (cursor != nullptr)
        {
            ResetCursor();
        }

        cursor = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(cursorType));
        SDL_SetCursor(cursor);
    }

    void ResetCursor()
    {
        SDL_FreeCursor(cursor);
        cursor = nullptr;
    }
}