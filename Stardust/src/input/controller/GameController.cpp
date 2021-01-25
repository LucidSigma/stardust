#include "stardust/input/controller/GameController.h"

#include <algorithm>
#include <limits>
#include <utility>

#include "stardust/input/Input.h"

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

		if (m_handle != nullptr)
		{
			m_hasLED = SDL_GameControllerHasLED(GetRawHandle());
			m_hasTouchpad = SDL_GameControllerGetNumTouchpads(GetRawHandle()) > 0;
			
			if (m_hasTouchpad)
			{
				m_touchpadFingers.resize(GetSupportedTouchpadFingerCount());
			}

			m_canRumble = SDL_GameControllerRumble(GetRawHandle(), 1u, 1u, 0u) == 0;
			m_canRumbleTriggers = SDL_GameControllerRumbleTriggers(GetRawHandle(), 1u, 1u, 0u) == 0;

			m_hasAccelerometer = SDL_GameControllerHasSensor(GetRawHandle(), SDL_SENSOR_ACCEL);

			if (m_hasAccelerometer)
			{
				SDL_GameControllerSetSensorEnabled(GetRawHandle(), SDL_SENSOR_ACCEL, SDL_TRUE);
			}

			m_hasGyroscope = SDL_GameControllerHasSensor(GetRawHandle(), SDL_SENSOR_GYRO);

			if (m_hasGyroscope)
			{
				SDL_GameControllerSetSensorEnabled(GetRawHandle(), SDL_SENSOR_GYRO, SDL_TRUE);
			}
		}
	}

	GameController::GameController(GameController&& other) noexcept
		: m_id(0), m_playerIndex(0u), m_handle(nullptr),
		  m_currentButtons(ButtonState{ }), m_previousButtons(ButtonState{ }), m_axes(Axes{ }), m_touchpadFingers({ }), m_accelerometerState(Vec3Zero), m_gyroscopeState(Vec3Zero),
		  m_hasLED(false), m_hasTouchpad(false), m_canRumble(false), m_canRumbleTriggers(false), m_hasAccelerometer(false), m_hasGyroscope(false)
	{
		std::swap(m_id, other.m_id);
		std::swap(m_playerIndex, other.m_playerIndex);
		std::swap(m_handle, other.m_handle);

		std::swap(m_currentButtons, other.m_currentButtons);
		std::swap(m_previousButtons, other.m_previousButtons);
		std::swap(m_axes, other.m_axes);
		std::swap(m_touchpadFingers, other.m_touchpadFingers);
		std::swap(m_accelerometerState, other.m_accelerometerState);
		std::swap(m_gyroscopeState, other.m_gyroscopeState);

		std::swap(m_hasLED, other.m_hasLED);
		std::swap(m_hasTouchpad, other.m_hasTouchpad);
		std::swap(m_canRumble, other.m_canRumble);
		std::swap(m_canRumbleTriggers, other.m_canRumbleTriggers);
		std::swap(m_hasAccelerometer, other.m_hasAccelerometer);
		std::swap(m_hasGyroscope, other.m_hasGyroscope);
	}

	GameController& GameController::operator =(GameController&& other) noexcept
	{
		m_id = std::exchange(other.m_id, 0);
		m_playerIndex = std::exchange(other.m_playerIndex, 0u);
		m_handle = std::exchange(other.m_handle, nullptr);

		m_currentButtons = std::exchange(other.m_currentButtons, ButtonState{ });
		m_previousButtons = std::exchange(other.m_previousButtons, ButtonState{ });
		m_axes = std::exchange(other.m_axes, Axes{ });
		m_touchpadFingers = std::exchange(other.m_touchpadFingers, { });
		m_accelerometerState = std::exchange(other.m_accelerometerState, Vec3Zero);
		m_gyroscopeState = std::exchange(other.m_gyroscopeState, Vec3Zero);

		m_hasLED = std::exchange(other.m_hasLED, false);
		m_hasTouchpad = std::exchange(other.m_hasTouchpad, false);
		m_canRumble = std::exchange(other.m_canRumble, false);
		m_canRumbleTriggers = std::exchange(other.m_canRumbleTriggers, false);
		m_hasAccelerometer = std::exchange(other.m_hasAccelerometer, false);
		m_hasGyroscope = std::exchange(other.m_hasGyroscope, false);

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

	void GameController::Rumble(const f32 lowFrequency, const f32 highFrequency, const u32 milliseconds) const
	{
		const u16 convertedLowFrequency = static_cast<u16>(lowFrequency * static_cast<f32>(std::numeric_limits<u16>::max()));
		const u16 convertedHighFrequency = static_cast<u16>(highFrequency * static_cast<f32>(std::numeric_limits<u16>::max()));

		SDL_GameControllerRumble(GetRawHandle(), convertedLowFrequency, convertedHighFrequency, milliseconds);
	}

	void GameController::StopRumbling() const
	{
		SDL_GameControllerRumble(GetRawHandle(), 0u, 0u, 0u);
	}

	void GameController::RumbleTriggers(const f32 leftIntensity, const f32 rightIntensity, const u32 milliseconds) const
	{
		const u16 convertedLeftIntensity = static_cast<u16>(leftIntensity * static_cast<f32>(std::numeric_limits<u16>::max()));
		const u16 convertedRightIntensity = static_cast<u16>(rightIntensity * static_cast<f32>(std::numeric_limits<u16>::max()));

		SDL_GameControllerRumbleTriggers(GetRawHandle(), convertedLeftIntensity, convertedRightIntensity, milliseconds);
	}

	void GameController::StopRumblingTriggers() const
	{
		SDL_GameControllerRumbleTriggers(GetRawHandle(), 0u, 0u, 0u);
	}

	void GameController::SetLED(const Colour& colour) const
	{
		SDL_GameControllerSetLED(GetRawHandle(), colour.red, colour.green, colour.blue);
	}

	[[nodiscard]] u32 GameController::GetSupportedTouchpadFingerCount() const
	{
		return static_cast<u32>(SDL_GameControllerGetNumTouchpadFingers(GetRawHandle(), 0));
	}

	[[nodiscard]] String GameController::GetName() const
	{
		return SDL_GameControllerName(GetRawHandle());
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

		case GameControllerButton::Misc:
			return buttonState.misc;

		case GameControllerButton::Touchpad:
			return buttonState.touchpad;

		case GameControllerButton::Paddle1:
			return buttonState.paddles[0];

		case GameControllerButton::Paddle2:
			return buttonState.paddles[1];

		case GameControllerButton::Paddle3:
			return buttonState.paddles[2];

		case GameControllerButton::Paddle4:
			return buttonState.paddles[3];

		default:
			return false;
		}
	}

	void GameController::UpdateButtons()
	{
		m_previousButtons = m_currentButtons;

		m_currentButtons.dPad.up = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_UP) == 1u;
		m_currentButtons.dPad.down = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1u;
		m_currentButtons.dPad.left = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1u;
		m_currentButtons.dPad.right = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1u;

		m_currentButtons.a = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_A) == 1u;
		m_currentButtons.b = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_B) == 1u;
		m_currentButtons.x = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_X) == 1u;
		m_currentButtons.y = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_Y) == 1u;

		m_currentButtons.back = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_BACK) == 1u;
		m_currentButtons.guide = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_GUIDE) == 1u;
		m_currentButtons.start = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_START) == 1u;

		m_currentButtons.leftStick = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1u;
		m_currentButtons.rightStick = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1u;
		m_currentButtons.leftShoulder = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1u;
		m_currentButtons.rightShoulder = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1u;

		m_currentButtons.misc = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_MISC1) == 1u;
		m_currentButtons.touchpad = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_TOUCHPAD) == 1u;

		m_currentButtons.paddles[0] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE1) == 1u;
		m_currentButtons.paddles[1] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE2) == 1u;
		m_currentButtons.paddles[2] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE3) == 1u;
		m_currentButtons.paddles[3] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE4) == 1u;
	}

	void GameController::UpdateAxes()
	{
		m_axes.left.x = SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTX);
		m_axes.left.y = SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTY);
		m_axes.right.x = SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTX);
		m_axes.right.y = SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTY);

		m_axes.leftTrigger = SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		m_axes.rightTrigger = SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

		const Vector<ReferenceWrapper<i16>> axes{
			std::ref(m_axes.left.x),
			std::ref(m_axes.left.y),
			std::ref(m_axes.right.x),
			std::ref(m_axes.right.y),
			std::ref(m_axes.leftTrigger),
			std::ref(m_axes.rightTrigger),
		};

		for (auto& axis : axes)
		{
			if (static_cast<u16>(std::abs(axis.get())) < Input::GetGameControllerDeadzone())
			{
				axis.get() = 0;
			}
		}
	}

	void GameController::UpdateTouchpadFingers()
	{
		if (m_hasTouchpad)
		{
			for (u32 i = 0u; i < m_touchpadFingers.size(); ++i)
			{
				u8 fingerState = 0u;;

				SDL_GameControllerGetTouchpadFinger(
					GetRawHandle(), 0, i,
					&fingerState, &m_touchpadFingers[i].position.x, &m_touchpadFingers[i].position.y, &m_touchpadFingers[i].pressure
				);

				m_touchpadFingers[i].isTouching = fingerState != 0u;
			}
		}
	}

	void GameController::UpdateSensors()
	{
		Array<f32, 3u> sensorData{ 0.0f, 0.0f, 0.0f };

		if (m_hasAccelerometer)
		{
			SDL_GameControllerGetSensorData(GetRawHandle(), SDL_SENSOR_ACCEL, sensorData.data(), 3);
			m_accelerometerState.x = sensorData[0];
			m_accelerometerState.y = sensorData[1];
			m_accelerometerState.z = sensorData[2];
		}

		if (m_hasGyroscope)
		{
			SDL_GameControllerGetSensorData(GetRawHandle(), SDL_SENSOR_GYRO, sensorData.data(), 3);
			m_gyroscopeState.x = sensorData[0];
			m_gyroscopeState.y = sensorData[1];
			m_gyroscopeState.z = sensorData[2];
		}
	}
}