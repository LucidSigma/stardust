#pragma once
#ifndef STARDUST_CURSOR_H
#define STARDUST_CURSOR_H

#include <type_traits>

#include <SDL2/SDL.h>

namespace stardust
{
    namespace cursor
    {
        enum class Type
            : std::underlying_type_t<SDL_SystemCursor>
        {
            Arrow = SDL_SYSTEM_CURSOR_ARROW,
            IBeam = SDL_SYSTEM_CURSOR_IBEAM,
            Wait = SDL_SYSTEM_CURSOR_WAIT,
            Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
            WaitArrow = SDL_SYSTEM_CURSOR_WAITARROW,
            ResizeBackwardDiagonal = SDL_SYSTEM_CURSOR_SIZENWSE,
            ResizeForwardDiagonal = SDL_SYSTEM_CURSOR_SIZENESW,
            ResizeHorizontal = SDL_SYSTEM_CURSOR_SIZEWE,
            ResizeVertical = SDL_SYSTEM_CURSOR_SIZENS,
            ResizeAll = SDL_SYSTEM_CURSOR_SIZEALL,
            No = SDL_SYSTEM_CURSOR_NO,
            Hand = SDL_SYSTEM_CURSOR_HAND,
        };

        extern auto SetType(const Type type) -> void;
        extern auto Reset() -> void;

        [[nodiscard]] extern auto IsCaptureEnabled() -> bool;
        extern auto EnableCapture(const bool enable) -> void;

        extern auto Show() -> void;
        extern auto Hide() -> void;
        [[nodiscard]] extern auto IsShown() -> bool;
        [[nodiscard]] extern auto IsHidden() -> bool;
    }
}

#endif
