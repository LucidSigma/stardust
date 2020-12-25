#include "stardust/input/Input.h"

namespace stardust
{
	void Input::UpdateKeyboardState()
	{
		s_previousKeys = std::move(s_currentKeys);
		s_currentKeys = std::move(std::vector<std::uint8_t>(SDL_GetKeyboardState(nullptr), SDL_GetKeyboardState(nullptr) + SDL_NUM_SCANCODES));

		s_keyboardState = Keyboard(s_currentKeys.data(), s_previousKeys.data());
	}

	const Keyboard& Input::GetKeyboardState()
	{
		return s_keyboardState;
	}
}