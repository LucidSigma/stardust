#pragma once
#ifndef STARDUST_MOUSE_BUTTON_CODES_H
#define STARDUST_MOUSE_BUTTON_CODES_H

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    enum class MouseButton
        : decltype(SDL_BUTTON_LEFT)
    {
        Unknown = -1,

        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT,
        Thumb1 = SDL_BUTTON_X1,
        Thumb2 = SDL_BUTTON_X2,
    };

    enum class MouseAxis
        : i32
    {
        Unknown = -1,

        X = 0,
        Y = 1,

        Scroll = 2,
    };

    [[nodiscard]] extern auto GetInternalMouseButtonName(const MouseButton button) -> String;
    [[nodiscard]] extern auto GetInternalMouseAxisName(const MouseAxis axis) -> String;
}

#endif
