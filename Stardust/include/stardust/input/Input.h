#pragma once
#ifndef STARDUST_INPUT_H
#define STARDUST_INPUT_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <SDL2/SDL.h>

#include "stardust/input/keyboard/Keyboard.h"

namespace stardust
{
	class Input
		: private INoncopyable, private INonmovable
	{
	private:
		inline static Vector<u8> s_currentKeys{ Vector<u8>(SDL_NUM_SCANCODES, SDL_FALSE) };
		inline static Vector<u8> s_previousKeys{ Vector<u8>(SDL_NUM_SCANCODES, SDL_FALSE) };

		inline static Keyboard s_keyboardState{ s_currentKeys.data(), s_previousKeys.data() };

	public:
		static void UpdateKeyboardState();

		static const Keyboard& GetKeyboardState();

		Input() = delete;
		~Input() noexcept = delete;
	};
}

#endif