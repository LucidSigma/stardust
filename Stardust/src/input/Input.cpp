#include "stardust/input/Input.h"

#include <utility>

namespace stardust
{
	bool Input::IsMouseCaptured()
	{
		return s_isMouseCaptured;
	}

	void Input::CaptureMouse(const bool captureMouse)
	{
		s_isMouseInRelativeMode = SDL_CaptureMouse(static_cast<SDL_bool>(captureMouse)) == 0;
	}

	bool Input::IsMouseInRelativeMode()
	{
		return s_isMouseInRelativeMode;
	}

	void Input::SetRelativeMouseMode(const bool isMouseRelative)
	{
		s_isMouseInRelativeMode = SDL_SetRelativeMouseMode(static_cast<SDL_bool>(isMouseRelative)) == 0;

		if (s_isMouseInRelativeMode)
		{
			ClearRelativeMouseState();
		}
	}

	void Input::ClearRelativeMouseState()
	{
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}

	bool Input::IsCursorShown()
	{
		return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
	}

	void Input::ShowCursor(const bool showCursor)
	{
		SDL_ShowCursor(showCursor ? SDL_ENABLE : SDL_DISABLE);
	}

	void Input::WarpMouse(const Window& window, const i32 x, const i32 y)
	{
		SDL_WarpMouseInWindow(window.GetRawHandle(), x, y);
	}

	void Input::UpdateKeyboardState()
	{
		s_previousKeys = std::move(s_currentKeys);
		s_currentKeys = std::move(Vector<u8>(SDL_GetKeyboardState(nullptr), SDL_GetKeyboardState(nullptr) + SDL_NUM_SCANCODES));

		s_keyboardState = Keyboard(s_currentKeys.data(), s_previousKeys.data());
	}

	void Input::UpdateMouseState()
	{
		i32 x = 0;
		i32 y = 0;
		s_previousMouseButtonStates = s_currentMouseButtonStates;
		s_currentMouseButtonStates = SDL_GetMouseState(&x, &y);

		i32 relativeX = 0;
		i32 relativeY = 0;

		if (s_isMouseInRelativeMode)
		{
			SDL_GetRelativeMouseState(&relativeX, &relativeY);
		}

		s_mouseState.m_currentButtonStates = s_currentMouseButtonStates;
		s_mouseState.m_previousButtonStates = s_previousMouseButtonStates;
		s_mouseState.m_x = static_cast<f32>(x);
		s_mouseState.m_y = static_cast<f32>(y);
		s_mouseState.m_relativeX = static_cast<f32>(relativeX);
		s_mouseState.m_relativeY = static_cast<f32>(relativeY);
	}

	const Keyboard& Input::GetKeyboardState()
	{
		return s_keyboardState;
	}

	const Mouse& Input::GetMouseState()
	{
		return s_mouseState;
	}
}