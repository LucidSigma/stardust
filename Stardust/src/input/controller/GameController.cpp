#include "stardust/input/controller/GameController.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <utility>

#include "stardust/input/InputController.h"

namespace stardust
{
    auto GameController::GameControllerDestroyer::operator ()(SDL_GameController* const gameController) const noexcept -> void
    {
        SDL_GameControllerClose(gameController);
    }

    GameController::GameController(const ID id)
        : m_id(id), m_handle(UniquePointer<SDL_GameController, GameControllerDestroyer>(SDL_GameControllerOpen(id)))
    {
        if (m_handle == nullptr)
        {
            return;
        }

        m_name = SDL_GameControllerName(GetRawHandle());
        m_guid = [this]() -> String
        {
            const SDL_JoystickGUID guid = SDL_JoystickGetGUID(GetRawJoystickHandle());

            String guidString(64u, '\0');
            SDL_JoystickGetGUIDString(guid, guidString.data(), static_cast<i32>(guidString.length()));

            if (const usize firstNullTerminatorLocation = guidString.find_first_of('\0');
                firstNullTerminatorLocation != String::npos)
            {
                guidString.erase(firstNullTerminatorLocation);
            }

            return guidString;
        }();

        m_hasTouchpad = SDL_GameControllerGetNumTouchpads(GetRawHandle()) > 0;

        if (m_hasTouchpad)
        {
            m_touchpadFingers.resize(GetSupportedTouchpadFingerCount());
        }

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

    GameController::GameController(GameController&& other) noexcept
        : m_id(0), m_playerIndex(0u), m_name(""), m_guid(""), m_handle(nullptr),
          m_currentButtonsState(ButtonState{ }), m_previousButtonsState(ButtonState{ }), m_axes(Axes{ }), m_previousLeftTriggerState(0), m_previousRightTriggerState(0),
          m_touchpadFingers({ }), m_accelerometerState(Vector3Zero), m_gyroscopeState(Vector3Zero),
          m_hasTouchpad(false), m_hasAccelerometer(false), m_hasGyroscope(false)
    {
        std::swap(m_id, other.m_id);
        std::swap(m_playerIndex, other.m_playerIndex);
        std::swap(m_name, other.m_name);
        std::swap(m_guid, other.m_guid);
        std::swap(m_handle, other.m_handle);

        std::swap(m_currentButtonsState, other.m_currentButtonsState);
        std::swap(m_previousButtonsState, other.m_previousButtonsState);
        std::swap(m_axes, other.m_axes);
        std::swap(m_previousLeftTriggerState, other.m_previousLeftTriggerState);
        std::swap(m_previousRightTriggerState, other.m_previousRightTriggerState);

        std::swap(m_touchpadFingers, other.m_touchpadFingers);
        std::swap(m_accelerometerState, other.m_accelerometerState);
        std::swap(m_gyroscopeState, other.m_gyroscopeState);

        std::swap(m_hasTouchpad, other.m_hasTouchpad);
        std::swap(m_hasAccelerometer, other.m_hasAccelerometer);
        std::swap(m_hasGyroscope, other.m_hasGyroscope);
    }

    auto GameController::operator =(GameController&& other) noexcept -> GameController&
    {
        m_id = std::exchange(other.m_id, 0);
        m_playerIndex = std::exchange(other.m_playerIndex, 0u);
        m_name = std::exchange(other.m_name, "");
        m_guid = std::exchange(other.m_guid, "");
        m_handle = std::exchange(other.m_handle, nullptr);

        m_currentButtonsState = std::exchange(other.m_currentButtonsState, ButtonState{ });
        m_previousButtonsState = std::exchange(other.m_previousButtonsState, ButtonState{ });
        m_axes = std::exchange(other.m_axes, Axes{ });
        m_previousLeftTriggerState = std::exchange(other.m_previousLeftTriggerState, 0.0f);
        m_previousRightTriggerState = std::exchange(other.m_previousRightTriggerState, 0.0f);

        m_touchpadFingers = std::exchange(other.m_touchpadFingers, { });
        m_accelerometerState = std::exchange(other.m_accelerometerState, Vector3Zero);
        m_gyroscopeState = std::exchange(other.m_gyroscopeState, Vector3Zero);

        m_hasTouchpad = std::exchange(other.m_hasTouchpad, false);
        m_hasAccelerometer = std::exchange(other.m_hasAccelerometer, false);
        m_hasGyroscope = std::exchange(other.m_hasGyroscope, false);

        return *this;
    }

    auto GameController::Update(const InputController& inputController) -> void
    {
        UpdateButtons();
        UpdateAxes(inputController);
        UpdateTouchpadFingers();
        UpdateSensors();
    }

    [[nodiscard]] auto GameController::IsButtonDown(const GameControllerButton button) const -> bool
    {
        return GetButtonState(button, m_currentButtonsState) && !GetButtonState(button, m_previousButtonsState);
    }

    [[nodiscard]] auto GameController::IsButtonPressed(const GameControllerButton button) const -> bool
    {
        return GetButtonState(button, m_currentButtonsState);
    }

    [[nodiscard]] auto GameController::IsButtonUp(const GameControllerButton button) const -> bool
    {
        return !GetButtonState(button, m_currentButtonsState) && GetButtonState(button, m_previousButtonsState);
    }

    [[nodiscard]] auto GameController::IsAnyButtonDown(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto GameController::IsAnyButtonPressed(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto GameController::IsAnyButtonUp(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::any_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto GameController::AreAllButtonsDown(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto GameController::AreAllButtonsPressed(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto GameController::AreAllButtonsUp(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::all_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto GameController::AreNoButtonsDown(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonDown(button);
        });
    }

    [[nodiscard]] auto GameController::AreNoButtonsPressed(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonPressed(button);
        });
    }

    [[nodiscard]] auto GameController::AreNoButtonsUp(const List<GameControllerButton>& buttons) const -> bool
    {
        return std::ranges::none_of(buttons, [this](const GameControllerButton button)
        {
            return IsButtonUp(button);
        });
    }

    [[nodiscard]] auto GameController::IsTriggerDown(const GameControllerTrigger trigger) const -> bool
    {
        if (trigger == GameControllerTrigger::Left)
        {
            return m_axes.leftTrigger != 0.0f && m_previousLeftTriggerState == 0.0f;
        }
        else
        {
            return m_axes.rightTrigger != 0.0f && m_previousRightTriggerState == 0.0f;
        }
    }

    [[nodiscard]] auto GameController::IsTriggerPressed(const GameControllerTrigger trigger) const -> bool
    {
        if (trigger == GameControllerTrigger::Left)
        {
            return m_axes.leftTrigger != 0.0f;
        }
        else
        {
            return m_axes.rightTrigger != 0.0f;
        }
    }

    [[nodiscard]] auto GameController::IsTriggerUp(const GameControllerTrigger trigger) const -> bool
    {
        if (trigger == GameControllerTrigger::Left)
        {
            return m_axes.leftTrigger == 0.0f && m_previousLeftTriggerState != 0.0f;
        }
        else
        {
            return m_axes.rightTrigger == 0.0f && m_previousRightTriggerState != 0.0f;
        }
    }

    [[nodiscard]] auto GameController::CanRumble() const -> bool
    {
        return SDL_GameControllerHasRumble(GetRawHandle()) == SDL_TRUE;
    }

    auto GameController::Rumble(const f32 lowFrequency, const f32 highFrequency, const u32 milliseconds) const -> void
    {
        const u16 convertedLowFrequency = static_cast<u16>(lowFrequency * static_cast<f32>(std::numeric_limits<u16>::max()));
        const u16 convertedHighFrequency = static_cast<u16>(highFrequency * static_cast<f32>(std::numeric_limits<u16>::max()));

        SDL_GameControllerRumble(GetRawHandle(), convertedLowFrequency, convertedHighFrequency, milliseconds);
    }

    auto GameController::StopRumbling() const -> void
    {
        SDL_GameControllerRumble(GetRawHandle(), 0u, 0u, 0u);
    }

    [[nodiscard]] auto GameController::CanRumbleTriggers() const -> bool
    {
        return SDL_GameControllerHasRumbleTriggers(GetRawHandle()) == SDL_TRUE;
    }

    auto GameController::RumbleTriggers(const f32 leftIntensity, const f32 rightIntensity, const u32 milliseconds) const -> void
    {
        const u16 convertedLeftIntensity = static_cast<u16>(leftIntensity * static_cast<f32>(std::numeric_limits<u16>::max()));
        const u16 convertedRightIntensity = static_cast<u16>(rightIntensity * static_cast<f32>(std::numeric_limits<u16>::max()));

        SDL_GameControllerRumbleTriggers(GetRawHandle(), convertedLeftIntensity, convertedRightIntensity, milliseconds);
    }

    auto GameController::StopRumblingTriggers() const -> void
    {
        SDL_GameControllerRumbleTriggers(GetRawHandle(), 0u, 0u, 0u);
    }

    [[nodiscard]] auto GameController::HasLED() const -> bool
    {
        return SDL_GameControllerHasLED(GetRawHandle()) == SDL_TRUE;
    }

    auto GameController::SetLED(const Colour& colour) const -> void
    {
        SDL_GameControllerSetLED(GetRawHandle(), colour.red, colour.green, colour.blue);
    }

    [[nodiscard]] auto GameController::GetSupportedTouchpadFingerCount() const -> u32
    {
        return static_cast<u32>(SDL_GameControllerGetNumTouchpadFingers(GetRawHandle(), 0));
    }

    [[nodiscard]] auto GameController::GetInstanceID() const -> InstanceID
    {
        return SDL_JoystickInstanceID(GetRawJoystickHandle());
    }

    [[nodiscard]] auto GameController::GetType() const noexcept -> Type
    {
        return static_cast<Type>(SDL_GameControllerGetType(GetRawHandle()));
    }

    [[nodiscard]] auto GameController::GetRawJoystickHandle() const -> SDL_Joystick*
    {
        return SDL_GameControllerGetJoystick(GetRawHandle());
    }

    [[nodiscard]] GameController::operator Joystick() const
    {
        return Joystick(*this);
    }

    [[nodiscard]] auto GameController::GetButtonState(const GameControllerButton button, const ButtonState& buttonState) noexcept -> bool
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

        [[unlikely]] default:
            return false;
        }
    }

    auto GameController::UpdateButtons() -> void
    {
        m_previousButtonsState = m_currentButtonsState;

        m_currentButtonsState.dPad.up = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_UP) == 1u;
        m_currentButtonsState.dPad.down = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1u;
        m_currentButtonsState.dPad.left = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1u;
        m_currentButtonsState.dPad.right = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1u;

        m_currentButtonsState.a = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_A) == 1u;
        m_currentButtonsState.b = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_B) == 1u;
        m_currentButtonsState.x = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_X) == 1u;
        m_currentButtonsState.y = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_Y) == 1u;

        m_currentButtonsState.back = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_BACK) == 1u;
        m_currentButtonsState.guide = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_GUIDE) == 1u;
        m_currentButtonsState.start = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_START) == 1u;

        m_currentButtonsState.leftStick = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1u;
        m_currentButtonsState.rightStick = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1u;
        m_currentButtonsState.leftShoulder = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1u;
        m_currentButtonsState.rightShoulder = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1u;

        m_currentButtonsState.misc = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_MISC1) == 1u;
        m_currentButtonsState.touchpad = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_TOUCHPAD) == 1u;

        m_currentButtonsState.paddles[0] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE1) == 1u;
        m_currentButtonsState.paddles[1] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE2) == 1u;
        m_currentButtonsState.paddles[2] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE3) == 1u;
        m_currentButtonsState.paddles[3] = SDL_GameControllerGetButton(GetRawHandle(), SDL_CONTROLLER_BUTTON_PADDLE4) == 1u;
    }

    auto GameController::UpdateAxes(const InputController& inputController) -> void
    {
        const f32 deadzone = inputController.GetGameControllerDeadzone();

        const Vector2 leftAxisData{
            static_cast<f32>(SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTX)),
            static_cast<f32>(SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTY)),
        };

        const Vector2 rightAxisData{
            static_cast<f32>(SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTX)),
            static_cast<f32>(SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTY)),
        };

        m_axes.left = GetNormalisedThumbstickValue(leftAxisData, deadzone, s_leftThumbstickInputType);
        m_axes.right = GetNormalisedThumbstickValue(rightAxisData, deadzone, s_rightThumbstickInputType);

        m_previousLeftTriggerState = m_axes.leftTrigger;
        m_previousRightTriggerState = m_axes.rightTrigger;

        m_axes.leftTrigger = GetNormalisedTriggerValue(
            SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERLEFT),
            deadzone
        );

        m_axes.rightTrigger = GetNormalisedTriggerValue(
            SDL_GameControllerGetAxis(GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERRIGHT),
            deadzone
        );
    }

    [[nodiscard]] auto GameController::GetNormalisedThumbstickValue(const Vector2 rawAxisData, const f32 deadzone, const ThumbstickInputType thumbstickInputType) -> Axes::Thumbstick
    {
        constexpr f32 MaxAxisValue = static_cast<f32>(std::numeric_limits<i16>::max());
        const f32 unnormalisedDeadzone = deadzone * MaxAxisValue;

        switch (thumbstickInputType)
        {
        case ThumbstickInputType::Orthogonal:
        {
            Axes::Thumbstick thumbstick{
                .x = 0.0f,
                .y = 0.0f,
            };

            if (glm::abs(rawAxisData.x) >= unnormalisedDeadzone)
            {
                const f32 normalisedX = glm::sign(rawAxisData.x) * ((glm::abs(rawAxisData.x) - unnormalisedDeadzone) / (MaxAxisValue - unnormalisedDeadzone));
                thumbstick.x = glm::clamp(normalisedX, -1.0f, 1.0f);
            }

            if (glm::abs(rawAxisData.y) >= unnormalisedDeadzone)
            {
                const f32 normalisedY = glm::sign(rawAxisData.y) * ((glm::abs(rawAxisData.y) - unnormalisedDeadzone) / (MaxAxisValue - unnormalisedDeadzone));
                thumbstick.y = glm::clamp(normalisedY, -1.0f, 1.0f);
            }

            return thumbstick;
        }

        case ThumbstickInputType::Radial:
        [[unlikely]] default:
        {
            const f32 axisLength = glm::abs(glm::length(rawAxisData));

            if (axisLength < unnormalisedDeadzone)
            {
                return Axes::Thumbstick{
                    .x = 0.0f,
                    .y = 0.0f,
                };
            }

            const f32 axisPercentage = (axisLength - unnormalisedDeadzone) / (MaxAxisValue - unnormalisedDeadzone);
            const Vector2 normalisedAxisData = glm::normalize(rawAxisData) * axisPercentage;

            return Axes::Thumbstick{
                .x = glm::clamp(normalisedAxisData.x, -1.0f, 1.0f),
                .y = glm::clamp(normalisedAxisData.y, -1.0f, 1.0f),
            };
        }
        }
    }

    [[nodiscard]] auto GameController::GetNormalisedTriggerValue(const i16 rawAxisData, const f32 deadzone) -> f32
    {
        constexpr f32 MaxAxisValue = static_cast<f32>(std::numeric_limits<i16>::max());
        const f32 unnormalisedDeadzone = deadzone * MaxAxisValue;

        if (glm::abs(static_cast<f32>(rawAxisData)) < unnormalisedDeadzone)
        {
            return 0.0f;
        }

        const f32 normalisedTriggerData = glm::sign(static_cast<f32>(rawAxisData)) *
            ((glm::abs(static_cast<f32>(rawAxisData)) - unnormalisedDeadzone) / (MaxAxisValue - unnormalisedDeadzone));

        return glm::clamp(normalisedTriggerData, -1.0f, 1.0f);
    }

    auto GameController::UpdateTouchpadFingers() -> void
    {
        if (m_hasTouchpad)
        {
            for (u32 i = 0u; i < m_touchpadFingers.size(); ++i)
            {
                u8 fingerState = 0u;

                SDL_GameControllerGetTouchpadFinger(
                    GetRawHandle(),
                    0,
                    i,
                    &fingerState,
                    &m_touchpadFingers[i].position.x,
                    &m_touchpadFingers[i].position.y,
                    &m_touchpadFingers[i].pressure
                );

                m_touchpadFingers[i].isTouching = fingerState != 0u;
            }
        }
    }

    auto GameController::UpdateSensors() -> void
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
