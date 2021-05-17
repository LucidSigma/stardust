#include "stardust/input/InputManager.h"

#include <algorithm>

#include "stardust/input/Input.h"
#include "stardust/math/Math.h"

namespace stardust
{
    void InputManager::AddToButton(const String& buttonName, const KeyCode key)
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].keys= { };
        }

        m_buttons[buttonName].keys.insert(key);
    }

    void InputManager::AddToButton(const String& buttonName, const Vector<KeyCode>& keys)
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        for (const auto key : keys)
        {
            m_buttons[buttonName].keys.insert(key);
        }
    }

    void InputManager::AddToButton(const String& buttonName, const MouseButton button)
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].mouseButtons.insert(button);
    }

    void InputManager::AddToButton(const String& buttonName, const Vector<MouseButton>& buttons)
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        for (const auto button : buttons)
        {
            m_buttons[buttonName].mouseButtons.insert(button);
        }
    }

    void InputManager::AddToButton(const String& buttonName, const GameControllerButton button)
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].controllerButtons.insert(button);
    }

    void InputManager::AddToButton(const String& buttonName, const Vector<GameControllerButton>& buttons)
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        for (const auto button : buttons)
        {
            m_buttons[buttonName].controllerButtons.insert(button);
        }
    }

    void InputManager::RemoveButton(const String& buttonName)
    {
        m_buttons.erase(buttonName);
    }

    void InputManager::RemoveFromButton(const String& buttonName, const KeyCode key)
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].keys.erase(key);
        }
    }

    void InputManager::RemoveFromButton(const String& buttonName, const MouseButton button)
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].mouseButtons.erase(button);
        }
    }

    void InputManager::RemoveFromButton(const String& buttonName, const GameControllerButton button)
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].controllerButtons.erase(button);
        }
    }

    [[nodiscard]] bool InputManager::IsButtonDown(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
    {
        if (m_buttons.contains(buttonName))
        {
            const auto& buttons = m_buttons.at(buttonName);

            for (const auto key : buttons.keys)
            {
                if (Input::GetKeyboardState().IsKeyDown(key))
                {
                    return true;
                }
            }

            for (const auto mouseButton : buttons.mouseButtons)
            {
                if (Input::GetMouseState().IsButtonDown(mouseButton))
                {
                    return true;
                }
            }

            for (const auto& gameController : gameControllers)
            {
                for (const auto controllerButton : buttons.controllerButtons)
                {
                    if (gameController != nullptr && gameController->IsButtonDown(controllerButton))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    [[nodiscard]] bool InputManager::IsButtonPressed(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
    {
        if (m_buttons.contains(buttonName))
        {
            const auto& buttons = m_buttons.at(buttonName);

            for (const auto key : buttons.keys)
            {
                if (Input::GetKeyboardState().IsKeyPressed(key))
                {
                    return true;
                }
            }

            for (const auto mouseButton : buttons.mouseButtons)
            {
                if (Input::GetMouseState().IsButtonPressed(mouseButton))
                {
                    return true;
                }
            }

            for (const auto& gameController : gameControllers)
            {
                for (const auto controllerButton : buttons.controllerButtons)
                {
                    if (gameController != nullptr && gameController->IsButtonPressed(controllerButton))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    [[nodiscard]] bool InputManager::IsButtonUp(const String& buttonName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
    {
        if (m_buttons.contains(buttonName))
        {
            const auto& buttons = m_buttons.at(buttonName);

            for (const auto key : buttons.keys)
            {
                if (Input::GetKeyboardState().IsKeyUp(key))
                {
                    return true;
                }
            }

            for (const auto mouseButton : buttons.mouseButtons)
            {
                if (Input::GetMouseState().IsButtonUp(mouseButton))
                {
                    return true;
                }
            }

            for (const auto& gameController : gameControllers)
            {
                for (const auto controllerButton : buttons.controllerButtons)
                {
                    if (gameController != nullptr && gameController->IsButtonUp(controllerButton))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void InputManager::AddToAxis(const String& axisName, const AxisType axisType, const bool inverted)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].axes.insert({ axisType, inverted });
    }

    void InputManager::AddToPositiveAxis(const String& axisName, const KeyCode key)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveKeys.insert(key);
    }

    void InputManager::AddToPositiveAxis(const String& axisName, const Vector<KeyCode>& keys)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        for (const auto key : keys)
        {
            m_axes[axisName].positiveKeys.insert(key);
        }
    }

    void InputManager::AddToPositiveAxis(const String& axisName, const MouseButton button)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveMouseButtons.insert(button);
    }

    void InputManager::AddToPositiveAxis(const String& axisName, const Vector<MouseButton>& buttons)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        for (const auto button : buttons)
        {
            m_axes[axisName].positiveMouseButtons.insert(button);
        }
    }

    void InputManager::AddToPositiveAxis(const String& axisName, const GameControllerButton button)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveControllerButtons.insert(button);
    }

    void InputManager::AddToPositiveAxis(const String& axisName, const Vector<GameControllerButton>& buttons)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        for (const auto button : buttons)
        {
            m_axes[axisName].positiveControllerButtons.insert(button);
        }
    }

    void InputManager::AddToNegativeAxis(const String& axisName, const KeyCode key)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeKeys.insert(key);
    }

    void InputManager::AddToNegativeAxis(const String& axisName, const Vector<KeyCode>& keys)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        for (const auto key : keys)
        {
            m_axes[axisName].negativeKeys.insert(key);
        }
    }

    void InputManager::AddToNegativeAxis(const String& axisName, const MouseButton button)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeMouseButtons.insert(button);
    }

    void InputManager::AddToNegativeAxis(const String& axisName, const Vector<MouseButton>& buttons)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        for (const auto button : buttons)
        {
            m_axes[axisName].negativeMouseButtons.insert(button);
        }
    }

    void InputManager::AddToNegativeAxis(const String& axisName, const GameControllerButton button)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeControllerButtons.insert(button);
    }

    void InputManager::AddToNegativeAxis(const String& axisName, const Vector<GameControllerButton>& buttons)
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        for (const auto button : buttons)
        {
            m_axes[axisName].negativeControllerButtons.insert(button);
        }
    }

    void InputManager::InvertAxis(const String& axisName, const AxisType axisType, const bool isInverted)
    {
        if (m_axes.contains(axisName) && m_axes.at(axisName).axes.contains(axisType))
        {
            m_axes[axisName].axes[axisType] = isInverted;
        }
    }

    void InputManager::RemoveAxis(const String& axisName)
    {
        m_axes.erase(axisName);
    }

    void InputManager::RemoveFromAxis(const String& axisName, const AxisType axisType)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].axes.erase(axisType);
        }
    }

    void InputManager::RemoveFromPositiveAxis(const String& axisName, const KeyCode key)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveKeys.erase(key);
        }
    }

    void InputManager::RemoveFromPositiveAxis(const String& axisName, const MouseButton button)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveMouseButtons.erase(button);
        }
    }

    void InputManager::RemoveFromPositiveAxis(const String& axisName, const GameControllerButton button)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveControllerButtons.erase(button);
        }
    }

    void InputManager::RemoveFromNegativeAxis(const String& axisName, const KeyCode key)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeKeys.erase(key);
        }
    }

    void InputManager::RemoveFromNegativeAxis(const String& axisName, const MouseButton button)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeMouseButtons.erase(button);
        }
    }

    void InputManager::RemoveFromNegativeAxis(const String& axisName, const GameControllerButton button)
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeControllerButtons.erase(button);
        }
    }
    
    [[nodiscard]] i32 InputManager::GetAxis(const String& axisName, const Vector<ObserverPtr<const GameController>>& gameControllers) const
    {
        if (!m_axes.contains(axisName))
        {
            return 0;
        }

        const auto& axisData = m_axes.at(axisName);
        i32 axisResult = 0;

        for (auto&& [axisType, isInverted] : axisData.axes)
        {
            axisResult += GetAxisValueFromType(axisType, isInverted, gameControllers);
        }

        axisResult = std::clamp(axisResult, -1, 1);
        axisResult += GetAxisValueFromButtons(axisData, gameControllers);
        axisResult = std::clamp(axisResult, -1, 1);

        return axisResult;
    }

    [[nodiscard]] i32 InputManager::GetAxisValueFromType(const AxisType axisType, const bool isInverted, const Vector<ObserverPtr<const GameController>>& gameControllers) const
    {
        f32 axisValue = 0.0f;

        switch (axisType)
        {
        case AxisType::WSKeys:
            axisValue += Input::GetKeyboardState().IsKeyPressed(KeyCode::W) ? 1.0f : 0.0f;
            axisValue -= Input::GetKeyboardState().IsKeyPressed(KeyCode::S) ? 1.0f : 0.0f;

            break;

        case AxisType::ADKeys:
            axisValue += Input::GetKeyboardState().IsKeyPressed(KeyCode::D) ? 1.0f : 0.0f;
            axisValue -= Input::GetKeyboardState().IsKeyPressed(KeyCode::A) ? 1.0f : 0.0f;

            break;

        case AxisType::UpDownKeys:
            axisValue += Input::GetKeyboardState().IsKeyPressed(KeyCode::Up) ? 1.0f : 0.0f;
            axisValue -= Input::GetKeyboardState().IsKeyPressed(KeyCode::Down) ? 1.0f : 0.0f;

            break;

        case AxisType::LeftRightKeys:
            axisValue += Input::GetKeyboardState().IsKeyPressed(KeyCode::Right) ? 1.0f : 0.0f;
            axisValue -= Input::GetKeyboardState().IsKeyPressed(KeyCode::Left) ? 1.0f : 0.0f;

            break;

        case AxisType::MouseX:
            axisValue = Input::IsMouseInRelativeMode() ? Input::GetMouseState().GetRelativeCoordinates().x : 0.0f;

            break;

        case AxisType::MouseY:
            axisValue = Input::IsMouseInRelativeMode() ? Input::GetMouseState().GetRelativeCoordinates().y : 0.0f;

            break;

        case AxisType::MouseScroll:
            axisValue = static_cast<f32>(Input::GetMouseState().GetScrollAmount());

            break;

        case AxisType::ControllerDPadX:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->IsButtonPressed(GameControllerButton::DPadRight)) : 0.0f;
                axisValue -= controller != nullptr ? static_cast<f32>(controller->IsButtonPressed(GameControllerButton::DPadLeft)) : 0.0f;
            }

            break;

        case AxisType::ControllerDPadY:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->IsButtonPressed(GameControllerButton::DPadUp)) : 0.0f;
                axisValue -= controller != nullptr ? static_cast<f32>(controller->IsButtonPressed(GameControllerButton::DPadDown)) : 0.0f;
            }

            break;

        case AxisType::ControllerLeftX:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->GetAxes().left.x) : 0.0f;
            }

            break;

        case AxisType::ControllerLeftY:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->GetAxes().left.y) : 0.0f;
            }

            break;

        case AxisType::ControllerRightX:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->GetAxes().right.x) : 0.0f;
            }

            break;

        case AxisType::ControllerRightY:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->GetAxes().right.y) : 0.0f;
            }

            break;

        case AxisType::ControllerLeftTrigger:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->GetAxes().leftTrigger) : 0.0f;
            }

            break;

        case AxisType::ControllerRightTrigger:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr ? static_cast<f32>(controller->GetAxes().rightTrigger) : 0.0f;
            }

            break;

        case AxisType::ControllerGyroX:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr && controller->HasGyroscope()
                    ? controller->GetGyroscopeData().x
                    : 0.0f;
            }

            break;

        case AxisType::ControllerGyroY:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr && controller->HasGyroscope()
                    ? controller->GetGyroscopeData().y
                    : 0.0f;
            }

            break;

        case AxisType::ControllerGyroZ:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr && controller->HasGyroscope()
                    ? controller->GetGyroscopeData().z
                    : 0.0f;
            }

            break;

        case AxisType::ControllerAccelX:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr && controller->HasAccelerometer()
                    ? controller->GetAccelerometerData().x
                    : 0.0f;
            }

            break;

        case AxisType::ControllerAccelY:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr && controller->HasAccelerometer()
                    ? controller->GetAccelerometerData().y
                    : 0.0f;
            }

            break;

        case AxisType::ControllerAccelZ:
            for (const auto& controller : gameControllers)
            {
                axisValue += controller != nullptr && controller->HasAccelerometer()
                    ? controller->GetAccelerometerData().z
                    : 0.0f;
            }

            break;

        default:
            break;
        }

        i32 axisResult = static_cast<i32>(glm::sign(axisValue));

        if (isInverted)
        {
            axisResult *= -1;
        }

        return axisResult;
    }

    [[nodiscard]] i32 InputManager::GetAxisValueFromButtons(const AxisInput& axisData, const Vector<ObserverPtr<const GameController>>& gameControllers) const
    {
        i32 axisResult = 0;

        for (const auto key : axisData.positiveKeys)
        {
            if (Input::GetKeyboardState().IsKeyPressed(key))
            {
                ++axisResult;

                break;
            }
        }

        for (const auto key : axisData.negativeKeys)
        {
            if (Input::GetKeyboardState().IsKeyPressed(key))
            {
                --axisResult;

                break;
            }
        }

        for (const auto button : axisData.positiveMouseButtons)
        {
            if (Input::GetMouseState().IsButtonPressed(button))
            {
                ++axisResult;

                break;
            }
        }

        for (const auto button : axisData.negativeMouseButtons)
        {
            if (Input::GetMouseState().IsButtonPressed(button))
            {
                --axisResult;

                break;
            }
        }

        for (const auto& gameController : gameControllers)
        {
            if (gameController != nullptr)
            {
                for (const auto button : axisData.positiveControllerButtons)
                {
                    if (gameController->IsButtonPressed(button))
                    {
                        ++axisResult;

                        break;
                    }
                }

                for (const auto button : axisData.negativeControllerButtons)
                {
                    if (gameController->IsButtonPressed(button))
                    {
                        --axisResult;

                        break;
                    }
                }
            }
        }

        return axisResult;
    }
}