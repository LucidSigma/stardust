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
			gameController.m_previousButtons = gameController.m_currentButtons;

			gameController.m_currentButtons.dPad.up = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_UP) == 1u;
			gameController.m_currentButtons.dPad.down = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1u;
			gameController.m_currentButtons.dPad.left = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1u;
			gameController.m_currentButtons.dPad.right = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1u;

			gameController.m_currentButtons.a = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_A) == 1u;
			gameController.m_currentButtons.b = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_B) == 1u;
			gameController.m_currentButtons.x = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_X) == 1u;
			gameController.m_currentButtons.y = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_Y) == 1u;

			gameController.m_currentButtons.back = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_BACK) == 1u;
			gameController.m_currentButtons.guide = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_GUIDE) == 1u;
			gameController.m_currentButtons.start = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_START) == 1u;

			gameController.m_currentButtons.leftStick = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1u;
			gameController.m_currentButtons.rightStick = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1u;
			gameController.m_currentButtons.leftShoulder = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1u;
			gameController.m_currentButtons.rightShoulder = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1u;

			gameController.m_currentButtons.misc = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_MISC1) == 1u;
			gameController.m_currentButtons.touchPad = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_TOUCHPAD) == 1u;

			gameController.m_currentButtons.paddles[0] = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE1) == 1u;
			gameController.m_currentButtons.paddles[1] = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE2) == 1u;
			gameController.m_currentButtons.paddles[2] = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE3) == 1u;
			gameController.m_currentButtons.paddles[3] = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE4) == 1u;

			gameController.m_axes.left.x = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTX);
			gameController.m_axes.left.y = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTY);
			gameController.m_axes.right.x = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTX);
			gameController.m_axes.right.y = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTY);

			gameController.m_axes.leftTrigger = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			gameController.m_axes.rightTrigger = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

			const Vector<ReferenceWrapper<i16>> axes{
				std::ref(gameController.m_axes.left.x),
				std::ref(gameController.m_axes.left.y),
				std::ref(gameController.m_axes.right.x),
				std::ref(gameController.m_axes.right.y),
				std::ref(gameController.m_axes.leftTrigger),
				std::ref(gameController.m_axes.rightTrigger),
			};

			for (auto& axis : axes)
			{
				if (static_cast<u16>(std::abs(axis.get())) < s_gameControllerDeadzone)
				{
					axis.get() = 0;
				}
			}
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