#pragma once
#ifndef STARDUST_GAME_CONTROLLER_BUTTONS_H
#define STARDUST_GAME_CONTROLLER_BUTTONS_H

#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    enum class GameControllerButton
        : std::underlying_type_t<SDL_GameControllerButton>
    {
        Unknown = SDL_CONTROLLER_BUTTON_INVALID,

        A = SDL_CONTROLLER_BUTTON_A,
        B = SDL_CONTROLLER_BUTTON_B,
        X = SDL_CONTROLLER_BUTTON_X,
        Y = SDL_CONTROLLER_BUTTON_Y,

        Back = SDL_CONTROLLER_BUTTON_BACK,
        Guide = SDL_CONTROLLER_BUTTON_GUIDE,
        Start = SDL_CONTROLLER_BUTTON_START,

        LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,

        DPadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
        DPadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        DPadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        DPadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,

        Misc = SDL_CONTROLLER_BUTTON_MISC1,

        Paddle1 = SDL_CONTROLLER_BUTTON_PADDLE1,
        Paddle2 = SDL_CONTROLLER_BUTTON_PADDLE2,
        Paddle3 = SDL_CONTROLLER_BUTTON_PADDLE3,
        Paddle4 = SDL_CONTROLLER_BUTTON_PADDLE4,

        Touchpad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
    };

    enum class GameControllerTrigger
        : std::underlying_type_t<SDL_GameControllerAxis>
    {
        Unknown = SDL_CONTROLLER_AXIS_INVALID,

        Left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        Right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    };

    enum class GameControllerAxis
        : std::underlying_type_t<SDL_GameControllerAxis>
    {
        Unknown = SDL_CONTROLLER_AXIS_INVALID,

        LeftX = SDL_CONTROLLER_AXIS_LEFTX,
        LeftY = SDL_CONTROLLER_AXIS_LEFTY,

        RightX = SDL_CONTROLLER_AXIS_RIGHTX,
        RightY = SDL_CONTROLLER_AXIS_RIGHTY,

        TriggerLeft = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        TriggerRight = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
    };

    [[nodiscard]] extern auto GetInternalGameControllerButtonName(const GameControllerButton button) -> String;
    [[nodiscard]] extern auto GetInternalGameControllerTriggerName(const GameControllerTrigger trigger) -> String;
    [[nodiscard]] extern auto GetInternalGameControllerAxisName(const GameControllerAxis axis) -> String;
}

#endif
