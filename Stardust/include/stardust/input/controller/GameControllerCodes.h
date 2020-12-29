#pragma once
#ifndef STARDUST_GAME_CONTROLLER_CODES_H
#define STARDUST_GAME_CONTROLLER_CODES_H

#include <type_traits>

#include <magic_enum/magic_enum.hpp>
#include <SDL2/SDL.h>

#include "stardust/data/Types.h"

namespace stardust
{
	enum class GameControllerButton
		: std::underlying_type_t<SDL_GameControllerButton>
	{
		DPadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
		DPadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		DPadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		DPadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,

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

		Paddle1 = SDL_CONTROLLER_BUTTON_PADDLE1,
		Paddle2 = SDL_CONTROLLER_BUTTON_PADDLE2,
		Paddle3 = SDL_CONTROLLER_BUTTON_PADDLE3,
		Paddle4 = SDL_CONTROLLER_BUTTON_PADDLE4,

		Misc = SDL_CONTROLLER_BUTTON_MISC1,
		TouchPad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
	};

	constexpr usize GameControllerButtonCount = magic_enum::enum_count<GameControllerButton>();
}

#endif