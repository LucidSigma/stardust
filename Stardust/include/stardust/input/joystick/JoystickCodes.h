#pragma once
#ifndef STARDUST_JOYSTICK_CODES_H
#define STARDUST_JOYSTICK_CODES_H

#include <type_traits>

#include <SDL2/SDL.h>

namespace stardust
{
    enum class JoystickHatSwitchDirection
        : decltype(SDL_HAT_CENTERED)
    {
        Centred = SDL_HAT_CENTERED,
        Up = SDL_HAT_UP,
        Right = SDL_HAT_RIGHT,
        Down = SDL_HAT_DOWN,
        Left = SDL_HAT_LEFT,
        RightUp = SDL_HAT_RIGHTUP,
        RightDown = SDL_HAT_RIGHTDOWN,
        LeftUp = SDL_HAT_LEFTUP,
        LeftDown = SDL_HAT_LEFTDOWN,
    };
}

#endif
