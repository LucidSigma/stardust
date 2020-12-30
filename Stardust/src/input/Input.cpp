#include "stardust/input/Input.h"

#include <algorithm>
#include <utility>

#include "stardust/debug/logging/Log.h"
#include "stardust/debug/message_box/MessageBox.h"
#include "stardust/math/Math.h"

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

	u32 Input::GetMaxGameControllers() noexcept
	{
		return s_maxGameControllers;
	}

	void Input::SetMaxGameControllers(const u32 maxGameControllers) noexcept
	{
		s_maxGameControllers = std::max(1u, maxGameControllers);
	}

	u32 Input::GetGameControllerDeadzone() noexcept
	{
		return s_gameControllerDeadzone;
	}

	void Input::SetGameControllerDeadzone(const u32 gameControllerDeadzone) noexcept
	{
		s_gameControllerDeadzone = gameControllerDeadzone;
	}

	ObserverPtr<GameController> Input::AddGameController(const i32 id, const Locale& locale)
	{
		if (GetGameControllerCount() == s_maxGameControllers)
		{
			return nullptr;
		}

		GameController gameController(id);
		const SDL_JoystickID instanceID = SDL_JoystickInstanceID(gameController.GetRawJoystickHandle());

		s_gameControllers.emplace(instanceID, std::move(gameController));

		if (s_gameControllers.at(instanceID).GetRawHandle() == nullptr)
		{
			message_box::Show(
				locale["engine"]["warnings"]["titles"]["controller"],
				locale["engine"]["warnings"]["bodies"]["controller"],
				message_box::Type::Warning
			);
			Log::EngineWarn("Could not add new game controller: {}.", SDL_GetError());

			s_gameControllers.erase(instanceID);

			return nullptr;
		}
		else
		{
			Log::EngineTrace("Controller {} added (Instance ID {}).", id, instanceID);

			return &s_gameControllers.at(instanceID);
		}
	}

	void Input::RemoveGameController(const SDL_JoystickID instanceID)
	{
		if (s_gameControllers.contains(instanceID))
		{
			s_gameControllers.erase(instanceID);
			Log::EngineTrace("Controller removed: {}", instanceID);
		}
	}

	void Input::RemoveAllGameControllers()
	{
		s_gameControllers.clear();
	}

	usize Input::GetGameControllerCount()
	{
		return s_gameControllers.size();
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

	void Input::ResetScrollState() noexcept
	{
		s_mouseState.m_yScrollAmount = 0;
	}

	void Input::UpdateScrollState(const i32 scrollAmount) noexcept
	{
		s_mouseState.m_yScrollAmount = scrollAmount;
	}

	void Input::UpdateGameControllers()
	{
		for (auto& [id, gameController] : s_gameControllers)
		{
			gameController.UpdateButtons();
			gameController.UpdateAxes();
			gameController.UpdateTouchpadFingers();
			gameController.UpdateSensors();
		}
	}

	const Keyboard& Input::GetKeyboardState()
	{
		return s_keyboardState;
	}

	const Mouse& Input::GetMouseState()
	{
		return s_mouseState;
	}

	[[nodiscard]] bool Input::DoesGameControllerExist(const SDL_JoystickID instanceID)
	{
		return s_gameControllers.contains(instanceID);
	}

	[[nodiscard]] ObserverPtr<GameController> Input::GetGameController(const SDL_JoystickID instanceID)
	{
		return DoesGameControllerExist(instanceID) ? &s_gameControllers.at(instanceID) : nullptr;
	}

	[[nodiscard]] ObserverPtr<GameController> Input::GetGameControllerByPlayerIndex(const u32 playerIndex)
	{
		for (auto& [id, gameController] : s_gameControllers)
		{
			if (gameController.m_playerIndex == playerIndex)
			{
				return &gameController;
			}
		}

		return nullptr;
	}

	[[nodiscard]] const HashMap<SDL_JoystickID, GameController>& Input::GetGameControllers()
	{
		return s_gameControllers;
	}
}