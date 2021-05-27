#pragma once
#ifndef STARDUST_MOUSE_BUTTON_CODES_H
#define STARDUST_MOUSE_BUTTON_CODES_H

#include <type_traits>

#include <magic_enum/magic_enum.hpp>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    enum class MouseButton
        : decltype(SDL_BUTTON_LEFT)
    {
        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT,
        Thumb1 = SDL_BUTTON_X1,
        Thumb2 = SDL_BUTTON_X2,
    };

    constexpr usize MouseButtonCount = magic_enum::enum_count<MouseButton>();

    [[nodiscard]] String MouseButtonToString(const MouseButton mouseButton);
}

#endif