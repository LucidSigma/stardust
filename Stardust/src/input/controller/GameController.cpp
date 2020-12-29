#include "stardust/input/controller/GameController.h"

#include <algorithm>
#include <utility>

namespace stardust
{
	void GameController::GameControllerDestroyer::operator ()(SDL_GameController* const gameController) const noexcept
	{
		SDL_GameControllerClose(gameController);
	}

	GameController::GameController(const i32 id)
	{
		m_id = id;
		m_currentButtons = ButtonState{ };
		m_previousButtons = ButtonState{ };
		m_axes = Axes{ };

		m_handle = UniquePtr<SDL_GameController, GameControllerDestroyer>(SDL_GameControllerOpen(id));
	}

	GameController::GameController(GameController&& other) noexcept
		: m_id(0), m_currentButtons(ButtonState{ }), m_previousButtons(ButtonState{ }), m_axes(Axes{ }), m_handle(nullptr)
	{
		std::swap(m_id, other.m_id);

		std::swap(m_currentButtons, other.m_currentButtons);
		std::swap(m_previousButtons, other.m_previousButtons);
		std::swap(m_axes, other.m_axes);

		std::swap(m_handle, other.m_handle);
	}

	GameController& GameController::operator =(GameController&& other) noexcept
	{
		m_id = std::exchange(other.m_id, 0);

		m_currentButtons = std::exchange(other.m_currentButtons, ButtonState{ });
		m_previousButtons = std::exchange(other.m_previousButtons, ButtonState{ });
		m_axes = std::exchange(other.m_axes, Axes{ });

		m_handle = std::exchange(other.m_handle, nullptr);

		return *this;
	}

	[[nodiscard]] bool GameController::IsButtonDown(const GameControllerButton button) const
	{
		return GetButtonState(button, m_currentButtons) && !GetButtonState(button, m_previousButtons);
	}

	[[nodiscard]] bool GameController::IsButtonPressed(const GameControllerButton button) const
	{
		return GetButtonState(button, m_currentButtons);
	}

	[[nodiscard]] bool GameController::IsButtonUp(const GameControllerButton button) const
	{
		return !GetButtonState(button, m_currentButtons) && GetButtonState(button, m_previousButtons);
	}

	[[nodiscard]] bool GameController::IsAnyButtonDown(const Vector<GameControllerButton>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const GameControllerButton button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool GameController::IsAnyButtonPressed(const Vector<GameControllerButton>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const GameControllerButton button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool GameController::IsAnyButtonUp(const Vector<GameControllerButton>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const GameControllerButton button)
		{
			return IsButtonUp(button);
		});
	}

	[[nodiscard]] bool GameController::AreAllButtonsDown(const Vector<GameControllerButton>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const GameControllerButton button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool GameController::AreAllButtonsPressed(const Vector<GameControllerButton>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const GameControllerButton button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool GameController::AreAllButtonsUp(const Vector<GameControllerButton>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const GameControllerButton button)
		{
			return IsButtonUp(button);
		});
	}

	[[nodiscard]] SDL_Joystick* const GameController::GetRawJoystickHandle() const
	{
		return SDL_GameControllerGetJoystick(GetRawHandle());
	}

	bool GameController::GetButtonState(const GameControllerButton button, const ButtonState& buttonState) noexcept
	{
		switch (button)
		{
		case GameControllerButton::DPadUp:
			return buttonState.dPad.up;

		case GameControllerButton::DPadDown:
			return buttonState.dPad.down;

		case GameControllerButton::DPadLeft:
			return buttonState.dPad.left;

		case GameControllerButton::DPadRight:
			return buttonState.dPad.right;

		case GameControllerButton::A:
			return buttonState.a;

		case GameControllerButton::B:
			return buttonState.b;

		case GameControllerButton::X:
			return buttonState.x;

		case GameControllerButton::Y:
			return buttonState.y;

		case GameControllerButton::Back:
			return buttonState.back;

		case GameControllerButton::Guide:
			return buttonState.guide;

		case GameControllerButton::Start:
			return buttonState.start;

		case GameControllerButton::LeftStick:
			return buttonState.leftStick;

		case GameControllerButton::RightStick:
			return buttonState.rightStick;

		case GameControllerButton::LeftShoulder:
			return buttonState.leftShoulder;

		case GameControllerButton::RightShoulder:
			return buttonState.rightShoulder;

		case GameControllerButton::Paddle1:
			return buttonState.paddles[0];

		case GameControllerButton::Paddle2:
			return buttonState.paddles[1];

		case GameControllerButton::Paddle3:
			return buttonState.paddles[2];

		case GameControllerButton::Paddle4:
			return buttonState.paddles[3];

		case GameControllerButton::Misc:
			return buttonState.misc;

		case GameControllerButton::TouchPad:
			return buttonState.touchPad;

		default:
			return false;
		}
	}
}