#include "stardust/window/cursor/Cursor.h"

namespace stardust
{
    namespace cursor
    {
        namespace
        {
            SDL_Cursor* cursor = nullptr;
            bool isCursorCaptured = false;
        }

        auto SetType(const Type type) -> void
        {
            Reset();

            cursor = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(type));
            SDL_SetCursor(cursor);
        }

        auto Reset() -> void
        {
            if (cursor != nullptr)
            {
                SDL_FreeCursor(cursor);
                cursor = nullptr;
            }
        }

        [[nodiscard]] auto IsCaptureEnabled() -> bool
        {
            return isCursorCaptured;
        }

        auto EnableCapture(const bool enable) -> void
        {
            if (SDL_CaptureMouse(enable ? SDL_TRUE : SDL_FALSE) != 0)
            {
                isCursorCaptured = enable;
            }
            else
            {
                isCursorCaptured = false;
            }
        }

        auto Show() -> void
        {
            SDL_ShowCursor(SDL_ENABLE);
        }

        auto Hide() -> void
        {
            SDL_ShowCursor(SDL_DISABLE);
        }

        [[nodiscard]] auto IsShown() -> bool
        {
            return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
        }

        [[nodiscard]] auto IsHidden() -> bool
        {
            return SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE;
        }
    }
}
