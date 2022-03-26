#include "stardust/input/InputManager.h"

#include "stardust/math/Math.h"

namespace stardust
{
    InputManager::InputManager(const InputController& inputController)
    {
        Initialise(inputController);
    }

    auto InputManager::Initialise(const InputController& inputController) -> void
    {
        m_inputController = &inputController;
    }

    auto InputManager::AddKeyToButton(const String& buttonName, const KeyCode key) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].keys = { };
        }

        m_buttons[buttonName].keys.insert(key);
    }

    auto InputManager::AddMouseButtonToButton(const String& buttonName, const MouseButton button) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].mouseButtons.insert(button);
    }

    auto InputManager::AddGameControllerButtonToButton(const String& buttonName, const GameControllerButton button) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].gameControllerButtons.insert(button);
    }

    auto InputManager::AddGameControllerTriggerToButton(const String& buttonName, const GameControllerTrigger trigger) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].gameControllerTriggers.insert(trigger);
    }

    auto InputManager::AddJoystickButtonToButton(const String& buttonName, const Joystick::ButtonID button) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].joystickButtons.insert(button);
    }

    auto InputManager::AddJoystickAxisToButton(const String& buttonName, const Joystick::AxisID axis) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        m_buttons[buttonName].joystickAxes.insert(axis);
    }

    auto InputManager::AddJoystickHatSwitchToButton(const String& buttonName, const Joystick::HatSwitchID hatSwitch, const JoystickHatSwitchDirection direction) -> void
    {
        if (!m_buttons.contains(buttonName))
        {
            m_buttons[buttonName] = { };
        }

        if (!m_buttons[buttonName].joystickHatSwitchDirections.contains(hatSwitch))
        {
            m_buttons[buttonName].joystickHatSwitchDirections[hatSwitch] = { };
        }

        m_buttons[buttonName].joystickHatSwitchDirections[hatSwitch].insert(direction);
    }

    auto InputManager::RemoveKeyFromButton(const String& buttonName, const KeyCode key) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].keys.erase(key);
        }
    }

    auto InputManager::RemoveMouseButtonFromButton(const String& buttonName, const MouseButton button) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].mouseButtons.erase(button);
        }
    }

    auto InputManager::RemoveGameControllerButtonFromButton(const String& buttonName, const GameControllerButton button) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].gameControllerButtons.erase(button);
        }
    }

    auto InputManager::RemoveGameControllerTriggerFromButton(const String& buttonName, const GameControllerTrigger trigger) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].gameControllerTriggers.erase(trigger);
        }
    }

    auto InputManager::RemoveJoystickButtonFromButton(const String& buttonName, const Joystick::ButtonID button) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].joystickButtons.erase(button);
        }
    }

    auto InputManager::RemoveJoystickAxisFromButton(const String& buttonName, const Joystick::AxisID axis) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].joystickAxes.erase(axis);
        }
    }

    auto InputManager::RemoveJoystickHatSwitchDirectionFromButton(const String& buttonName, const Joystick::HatSwitchID hatSwitch, const JoystickHatSwitchDirection direction) -> void
    {
        if (m_buttons.contains(buttonName) && m_buttons[buttonName].joystickHatSwitchDirections.contains(hatSwitch))
        {
            m_buttons[buttonName].joystickHatSwitchDirections[hatSwitch].erase(direction);
        }
    }

    auto InputManager::RemoveJoystickHatSwitchFromButton(const String& buttonName, const Joystick::HatSwitchID hatSwitch) -> void
    {
        if (m_buttons.contains(buttonName))
        {
            m_buttons[buttonName].joystickHatSwitchDirections.erase(hatSwitch);
        }
    }

    auto InputManager::RemoveButton(const String& buttonName) -> void
    {
        m_buttons.erase(buttonName);
    }

    auto InputManager::ClearAllButtons() -> void
    {
        m_buttons.clear();
    }

    [[nodiscard]] auto InputManager::HasButton(const String& buttonName) -> bool
    {
        return m_buttons.contains(buttonName);
    }

    [[nodiscard]] auto InputManager::IsButtonDown(const String& buttonName, const List<ObserverPointer<const GameController>>& gameControllers, const List<ObserverPointer<const Joystick>>& joysticks) const -> bool
    {
        if (m_buttons.contains(buttonName))
        {
            const auto& buttons = m_buttons.at(buttonName);

            for (const auto key : buttons.keys)
            {
                if (m_inputController->GetKeyboardState().IsKeyDown(key))
                {
                    return true;
                }
            }

            for (const auto mouseButton : buttons.mouseButtons)
            {
                if (m_inputController->GetMouseState().IsButtonDown(mouseButton))
                {
                    return true;
                }
            }

            for (const auto& gameController : gameControllers)
            {
                if (gameController == nullptr) [[unlikely]]
                {
                    continue;
                }

                for (const auto gameControllerButton : buttons.gameControllerButtons)
                {
                    if (gameController->IsButtonDown(gameControllerButton))
                    {
                        return true;
                    }
                }

                for (const auto gameControllerTrigger : buttons.gameControllerTriggers)
                {
                    if (gameController->IsTriggerDown(gameControllerTrigger))
                    {
                        return true;
                    }
                }
            }

            for (const auto& joystick : joysticks)
            {
                if (joystick == nullptr) [[unlikely]]
                {
                    continue;
                }

                for (const auto joystickButton : buttons.joystickButtons)
                {
                    if (joystick->HasButton(joystickButton) && joystick->IsButtonDown(joystickButton))
                    {
                        return true;
                    }
                }

                for (const auto joystickAxis : buttons.joystickAxes)
                {
                    if (joystick->HasAxis(joystickAxis) && joystick->IsAxisDown(joystickAxis))
                    {
                        return true;
                    }
                }

                for (const auto& [joystickHatSwitch, hatSwitchDirections] : buttons.joystickHatSwitchDirections)
                {
                    if (!joystick->HasHatSwitch(joystickHatSwitch))
                    {
                        continue;
                    }

                    for (const auto hatSwitchDirection : hatSwitchDirections)
                    {
                        if (joystick->WasHatSwitchMoved(joystickHatSwitch, hatSwitchDirection))
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    [[nodiscard]] auto InputManager::IsButtonPressed(const String& buttonName, const List<ObserverPointer<const GameController>>& gameControllers, const List<ObserverPointer<const Joystick>>& joysticks) const -> bool
    {
        if (m_buttons.contains(buttonName))
        {
            const auto& buttons = m_buttons.at(buttonName);

            for (const auto key : buttons.keys)
            {
                if (m_inputController->GetKeyboardState().IsKeyPressed(key))
                {
                    return true;
                }
            }

            for (const auto mouseButton : buttons.mouseButtons)
            {
                if (m_inputController->GetMouseState().IsButtonPressed(mouseButton))
                {
                    return true;
                }
            }

            for (const auto& gameController : gameControllers)
            {
                if (gameController == nullptr) [[unlikely]]
                {
                    continue;
                }

                for (const auto gameControllerButton : buttons.gameControllerButtons)
                {
                    if (gameController->IsButtonPressed(gameControllerButton))
                    {
                        return true;
                    }
                }

                for (const auto gameControllerTrigger : buttons.gameControllerTriggers)
                {
                    if (gameController->IsTriggerPressed(gameControllerTrigger))
                    {
                        return true;
                    }
                }
            }

            for (const auto& joystick : joysticks)
            {
                if (joystick == nullptr) [[unlikely]]
                {
                    continue;
                }

                for (const auto joystickButton : buttons.joystickButtons)
                {
                    if (joystick->HasButton(joystickButton) && joystick->IsButtonPressed(joystickButton))
                    {
                        return true;
                    }
                }

                for (const auto joystickAxis : buttons.joystickAxes)
                {
                    if (joystick->HasAxis(joystickAxis) && joystick->IsAxisPressed(joystickAxis))
                    {
                        return true;
                    }
                }

                for (const auto& [joystickHatSwitch, hatSwitchDirections] : buttons.joystickHatSwitchDirections)
                {
                    if (!joystick->HasHatSwitch(joystickHatSwitch))
                    {
                        continue;
                    }

                    for (const auto hatSwitchDirection : hatSwitchDirections)
                    {
                        if (joystick->IsHatSwitchMoved(joystickHatSwitch, hatSwitchDirection))
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    [[nodiscard]] auto InputManager::IsButtonUp(const String& buttonName, const List<ObserverPointer<const GameController>>& gameControllers, const List<ObserverPointer<const Joystick>>& joysticks) const -> bool
    {
        if (m_buttons.contains(buttonName))
        {
            const auto& buttons = m_buttons.at(buttonName);

            for (const auto key : buttons.keys)
            {
                if (m_inputController->GetKeyboardState().IsKeyUp(key))
                {
                    return true;
                }
            }

            for (const auto mouseButton : buttons.mouseButtons)
            {
                if (m_inputController->GetMouseState().IsButtonUp(mouseButton))
                {
                    return true;
                }
            }

            for (const auto& gameController : gameControllers)
            {
                if (gameController == nullptr) [[unlikely]]
                {
                    continue;
                }

                for (const auto gameControllerButton : buttons.gameControllerButtons)
                {
                    if (gameController->IsButtonUp(gameControllerButton))
                    {
                        return true;
                    }
                }

                for (const auto gameControllerTrigger : buttons.gameControllerTriggers)
                {
                    if (gameController->IsTriggerUp(gameControllerTrigger))
                    {
                        return true;
                    }
                }
            }

            for (const auto& joystick : joysticks)
            {
                if (joystick == nullptr) [[unlikely]]
                {
                    continue;
                }

                for (const auto joystickButton : buttons.joystickButtons)
                {
                    if (joystick->HasButton(joystickButton) && joystick->IsButtonUp(joystickButton))
                    {
                        return true;
                    }
                }

                for (const auto joystickAxis : buttons.joystickAxes)
                {
                    if (joystick->HasAxis(joystickAxis) && joystick->IsAxisUp(joystickAxis))
                    {
                        return true;
                    }
                }

                for (const auto& [joystickHatSwitch, hatSwitchDirections] : buttons.joystickHatSwitchDirections)
                {
                    if (!joystick->HasHatSwitch(joystickHatSwitch))
                    {
                        continue;
                    }

                    for (const auto hatSwitchDirection : hatSwitchDirections)
                    {
                        if (joystick->WasHatSwitchReturned(joystickHatSwitch, hatSwitchDirection))
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    auto InputManager::AddKeyToPositiveAxis(const String& axisName, const KeyCode key) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveKeys.insert(key);
    }

    auto InputManager::AddKeyToNegativeAxis(const String& axisName, const KeyCode key) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeKeys.insert(key);
    }

    auto InputManager::AddMouseButtonToPositiveAxis(const String& axisName, const MouseButton button) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveMouseButtons.insert(button);
    }

    auto InputManager::AddMouseButtonToNegativeAxis(const String& axisName, const MouseButton button) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeMouseButtons.insert(button);
    }

    auto InputManager::AddMouseAxisToAxis(const String& axisName, const MouseAxis axis) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].mouseAxes.insert(axis);
    }

    auto InputManager::AddGameControllerButtonToPositiveAxis(const String& axisName, const GameControllerButton button) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveGameControllerButtons.insert(button);
    }

    auto InputManager::AddGameControllerButtonToNegativeAxis(const String& axisName, const GameControllerButton button) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeGameControllerButtons.insert(button);
    }

    auto InputManager::AddGameControllerTriggerToPositiveAxis(const String& axisName, const GameControllerTrigger trigger) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveGameControllerTriggers.insert(trigger);
    }

    auto InputManager::AddGameControllerTriggerToNegativeAxis(const String& axisName, const GameControllerTrigger trigger) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeGameControllerTriggers.insert(trigger);
    }

    auto InputManager::AddGameControllerAxisToAxis(const String& axisName, const GameControllerAxis axis, const bool isInverted) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].gameControllerAxes[axis] = isInverted;
    }

    auto InputManager::AddJoystickButtonToPositiveAxis(const String& axisName, const Joystick::ButtonID button) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].positiveJoystickButtons.insert(button);
    }

    auto InputManager::AddJoystickButtonToNegativeAxis(const String& axisName, const Joystick::ButtonID button) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].negativeJoystickButtons.insert(button);
    }

    auto InputManager::AddJoystickAxisToPositiveAxis(const String& axisName, const Joystick::AxisID axis) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickAxes[axis] = JoystickAxisInfo{
            .sign = JoystickAxisSign::Positive,
            .isInverted = false,
        };
    }

    auto InputManager::AddJoystickAxisToNegativeAxis(const String& axisName, const Joystick::AxisID axis) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickAxes[axis] = JoystickAxisInfo{
            .sign = JoystickAxisSign::Negative,
            .isInverted = false,
        };
    }

    auto InputManager::AddJoystickAxisToAxis(const String& axisName, const Joystick::AxisID axis, const bool isInverted) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickAxes[axis] = JoystickAxisInfo{
            .sign = JoystickAxisSign::Both,
            .isInverted = isInverted,
        };
    }

    auto InputManager::AddJoystickHatSwitchXToAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickXHatSwitches.insert(hatSwitch);
    }

    auto InputManager::AddJoystickHatSwitchYToAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickYHatSwitches.insert(hatSwitch);
    }

    auto InputManager::AddJoystickTrackballXToAxis(const String& axisName, const Joystick::TrackballID trackball) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickXTrackballs.insert(trackball);
    }

    auto InputManager::AddJoystickTrackballYToAxis(const String& axisName, const Joystick::TrackballID trackball) -> void
    {
        if (!m_axes.contains(axisName))
        {
            m_axes[axisName] = { };
        }

        m_axes[axisName].joystickYTrackballs.insert(trackball);
    }

    auto InputManager::RemoveKeyFromPositiveAxis(const String& axisName, const KeyCode key) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveKeys.erase(key);
        }
    }

    auto InputManager::RemoveKeyFromNegativeAxis(const String& axisName, const KeyCode key) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeKeys.erase(key);
        }
    }

    auto InputManager::RemoveMouseButtonFromPositiveAxis(const String& axisName, const MouseButton button) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveMouseButtons.erase(button);
        }
    }

    auto InputManager::RemoveMouseButtonFromNegativeAxis(const String& axisName, const MouseButton button) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeMouseButtons.erase(button);
        }
    }

    auto InputManager::RemoveMouseAxisFromAxis(const String& axisName, const MouseAxis axis) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].mouseAxes.erase(axis);
        }
    }

    auto InputManager::RemoveGameControllerButtonFromPositiveAxis(const String& axisName, const GameControllerButton button) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveGameControllerButtons.erase(button);
        }
    }

    auto InputManager::RemoveGameControllerButtonFromNegativeAxis(const String& axisName, const GameControllerButton button) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeGameControllerButtons.erase(button);
        }
    }

    auto InputManager::RemoveGameControllerTriggerFromPositiveAxis(const String& axisName, const GameControllerTrigger trigger) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveGameControllerTriggers.erase(trigger);
        }
    }

    auto InputManager::RemoveGameControllerTriggerFromNegativeAxis(const String& axisName, const GameControllerTrigger trigger) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeGameControllerTriggers.erase(trigger);
        }
    }

    auto InputManager::RemoveGameControllerAxisFromAxis(const String& axisName, const GameControllerAxis axis) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].gameControllerAxes.erase(axis);
        }
    }

    auto InputManager::RemoveJoystickButtonFromPositiveAxis(const String& axisName, const Joystick::ButtonID button) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].positiveJoystickButtons.erase(button);
        }
    }

    auto InputManager::RemoveJoystickButtonFromNegativeAxis(const String& axisName, const Joystick::ButtonID button) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].negativeJoystickButtons.erase(button);
        }
    }

    auto InputManager::RemoveJoystickAxisFromAxis(const String& axisName, const Joystick::AxisID axis) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].joystickAxes.erase(axis);
        }
    }

    auto InputManager::RemoveJoystickHatSwitchXFromAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].joystickXHatSwitches.erase(hatSwitch);
        }
    }

    auto InputManager::RemoveJoystickHatSwitchYFromAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].joystickYHatSwitches.erase(hatSwitch);
        }
    }

    auto InputManager::RemoveJoystickTrackballXFromAxis(const String& axisName, const Joystick::TrackballID trackball) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].joystickXTrackballs.erase(trackball);
        }
    }

    auto InputManager::RemoveJoystickTrackballYFromAxis(const String& axisName, const Joystick::TrackballID trackball) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].joystickYTrackballs.erase(trackball);
        }
    }

    auto InputManager::RemoveAxis(const String& axisName) -> void
    {
        m_axes.erase(axisName);
    }

    auto InputManager::ClearAllAxes() -> void
    {
        m_axes.clear();
    }

    [[nodiscard]] auto InputManager::IsAxisInverted(const String& axisName, const GameControllerAxis gameControllerAxis) const -> bool
    {
        if (!m_axes.contains(axisName))
        {
            return false;
        }

        return m_axes.at(axisName).gameControllerAxes.at(gameControllerAxis);
    }

    [[nodiscard]] auto InputManager::IsAxisInverted(const String& axisName, const Joystick::AxisID joystickAxis) const -> bool
    {
        if (!m_axes.contains(axisName))
        {
            return false;
        }

        return m_axes.at(axisName).joystickAxes.at(joystickAxis).isInverted;
    }

    auto InputManager::InvertAxis(const String& axisName, const GameControllerAxis gameControllerAxis, const bool invertAxis) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].gameControllerAxes[gameControllerAxis] = invertAxis;
        }
    }

    auto InputManager::InvertAxis(const String& axisName, const Joystick::AxisID joystickAxis, const bool invertAxis) -> void
    {
        if (m_axes.contains(axisName))
        {
            m_axes[axisName].joystickAxes[joystickAxis].isInverted = invertAxis;
        }
    }

    [[nodiscard]] auto InputManager::HasAxis(const String& axisName) -> bool
    {
        return m_axes.contains(axisName);
    }

    [[nodiscard]] auto InputManager::GetAxis(const String& axisName, const List<ObserverPointer<const GameController>>& gameControllers, const List<ObserverPointer<const Joystick>>& joysticks) const -> i32
    {
        if (!m_axes.contains(axisName))
        {
            return 0;
        }

        return static_cast<i32>(glm::sign(GetAxisRaw(axisName, gameControllers, joysticks)));
    }

    [[nodiscard]] auto InputManager::GetAxisRaw(const String& axisName, const List<ObserverPointer<const GameController>>& gameControllers, const List<ObserverPointer<const Joystick>>& joysticks) const -> f32
    {
        if (!m_axes.contains(axisName))
        {
            return 0.0f;
        }

        const auto& axisData = m_axes.at(axisName);
        f32 axisResult = 0.0f;

        axisResult += GetAxisValueFromKeyboard(axisData);
        axisResult += GetAxisValueFromMouse(axisData);
        axisResult += GetAxisValueFromGameControllers(axisData, gameControllers);
        axisResult += GetAxisValueFromJoysticks(axisData, joysticks);

        return axisResult;
    }

    [[nodiscard]] auto InputManager::GetClampedAxisRaw(const String& axisName, const List<ObserverPointer<const GameController>>& gameControllers, const List<ObserverPointer<const Joystick>>& joysticks) const -> f32
    {
        return glm::clamp(GetAxisRaw(axisName, gameControllers, joysticks), -1.0f, 1.0f);
    }

    auto InputManager::LoadFromControlPrefs(const ControlPrefs& controlPrefs) -> void
    {
        LoadAxesFromControlPrefs(controlPrefs);
        LoadButtonsFromControlPrefs(controlPrefs);
    }

    [[nodiscard]] auto InputManager::GetAxisValueFromKeyboard(const AxisInput& axisData) const -> f32
    {
        f32 axisResult = 0.0f;

        for (const auto key : axisData.positiveKeys)
        {
            if (m_inputController->GetKeyboardState().IsKeyPressed(key))
            {
                axisResult += 1.0f;

                break;
            }
        }

        for (const auto key : axisData.negativeKeys)
        {
            if (m_inputController->GetKeyboardState().IsKeyPressed(key))
            {
                axisResult -= 1.0f;

                break;
            }
        }

        return axisResult;
    }

    [[nodiscard]] auto InputManager::GetAxisValueFromMouse(const AxisInput& axisData) const -> f32
    {
        f32 axisResult = 0.0f;

        for (const auto button : axisData.positiveMouseButtons)
        {
            if (m_inputController->GetMouseState().IsButtonPressed(button))
            {
                axisResult += 1.0f;

                break;
            }
        }

        for (const auto button : axisData.negativeMouseButtons)
        {
            if (m_inputController->GetMouseState().IsButtonPressed(button))
            {
                axisResult -= 1.0f;

                break;
            }
        }

        for (const auto axis : axisData.mouseAxes)
        {
            switch (axis)
            {
            case MouseAxis::X:
                axisResult += glm::sign(static_cast<f32>(m_inputController->GetMouseState().GetRelativeCoordinates().x));

                break;

            case MouseAxis::Y:
                axisResult += glm::sign(static_cast<f32>(m_inputController->GetMouseState().GetRelativeCoordinates().x));

                break;

            case MouseAxis::Scroll:
                axisResult += glm::sign(static_cast<f32>(m_inputController->GetMouseState().GetScrollAmount()));

                break;
            }
        }

        return axisResult;
    }

    [[nodiscard]] auto InputManager::GetAxisValueFromGameControllers(const AxisInput& axisData, const List<ObserverPointer<const GameController>>& gameControllers) const -> f32
    {
        f32 axisResult = 0.0f;

        for (const auto gameController : gameControllers)
        {
            if (gameController == nullptr) [[unlikely]]
            {
                continue;
            }

            for (const auto button : axisData.positiveGameControllerButtons)
            {
                if (gameController->IsButtonPressed(button))
                {
                    axisResult += 1.0f;

                    break;
                }
            }

            for (const auto button : axisData.negativeGameControllerButtons)
            {
                if (gameController->IsButtonPressed(button))
                {
                    axisResult -= 1.0f;

                    break;
                }
            }

            for (const auto trigger : axisData.positiveGameControllerTriggers)
            {
                if (gameController->IsTriggerPressed(trigger))
                {
                    axisResult += 1.0f;

                    break;
                }
            }

            for (const auto trigger : axisData.negativeGameControllerTriggers)
            {
                if (gameController->IsTriggerPressed(trigger))
                {
                    axisResult -= 1.0f;

                    break;
                }
            }

            for (const auto [axis, isInverted] : axisData.gameControllerAxes)
            {
                f32 currentAxisValue = 0.0f;

                switch (axis)
                {
                case GameControllerAxis::LeftX:
                    currentAxisValue = gameController->GetAxes().left.x;

                    break;

                case GameControllerAxis::LeftY:
                    currentAxisValue = gameController->GetAxes().left.y;

                    break;

                case GameControllerAxis::RightX:
                    currentAxisValue = gameController->GetAxes().right.x;

                    break;

                case GameControllerAxis::RightY:
                    currentAxisValue = gameController->GetAxes().right.y;

                    break;

                case GameControllerAxis::TriggerLeft:
                    currentAxisValue = gameController->GetAxes().leftTrigger;

                    break;

                case GameControllerAxis::TriggerRight:
                    currentAxisValue = gameController->GetAxes().rightTrigger;

                    break;
                }

                if (isInverted)
                {
                    currentAxisValue *= -1.0f;
                }

                axisResult += currentAxisValue;
            }
        }

        return axisResult;
    }

    [[nodiscard]] auto InputManager::GetAxisValueFromJoysticks(const AxisInput& axisData, const List<ObserverPointer<const Joystick>>& joysticks) const -> f32
    {
        f32 axisResult = 0.0f;

        for (const auto joystick : joysticks)
        {
            if (joystick == nullptr) [[unlikely]]
            {
                continue;
            }

            for (const auto button : axisData.positiveJoystickButtons)
            {
                if (joystick->HasButton(button) && joystick->IsButtonPressed(button))
                {
                    axisResult += 1.0f;

                    break;
                }
            }

            for (const auto button : axisData.negativeJoystickButtons)
            {
                if (joystick->HasButton(button) && joystick->IsButtonPressed(button))
                {
                    axisResult -= 1.0f;

                    break;
                }
            }

            for (const auto& [axis, axisInfo] : axisData.joystickAxes)
            {
                if (!joystick->HasAxis(axis))
                {
                    continue;
                }

                const auto [axisSign, isInverted] = axisInfo;

                switch (axisSign)
                {
                case JoystickAxisSign::Both:
                    axisResult += joystick->GetAxis(axis);

                    break;

                case JoystickAxisSign::Positive:
                    if (joystick->IsAxisPressed(axis))
                    {
                        axisResult += 1.0f;
                    }

                    break;

                case JoystickAxisSign::Negative:
                    if (joystick->IsAxisPressed(axis))
                    {
                        axisResult -= 1.0f;
                    }

                    break;
                }
            }

            for (const auto hatSwitch : axisData.joystickXHatSwitches)
            {
                if (!joystick->HasHatSwitch(hatSwitch))
                {
                    continue;
                }

                const JoystickHatSwitchDirection hatSwitchDirection = joystick->GetHatSwitch(hatSwitch);

                if (hatSwitchDirection == JoystickHatSwitchDirection::RightUp ||
                    hatSwitchDirection == JoystickHatSwitchDirection::Right ||
                    hatSwitchDirection == JoystickHatSwitchDirection::RightDown)
                {
                    axisResult += 1.0f;
                }
                else if (hatSwitchDirection == JoystickHatSwitchDirection::LeftUp ||
                    hatSwitchDirection == JoystickHatSwitchDirection::Left ||
                    hatSwitchDirection == JoystickHatSwitchDirection::LeftDown)
                {
                    axisResult -= 1.0f;
                }
            }

            for (const auto hatSwitch : axisData.joystickYHatSwitches)
            {
                if (!joystick->HasHatSwitch(hatSwitch))
                {
                    continue;
                }

                const JoystickHatSwitchDirection hatSwitchDirection = joystick->GetHatSwitch(hatSwitch);

                if (hatSwitchDirection == JoystickHatSwitchDirection::LeftUp ||
                    hatSwitchDirection == JoystickHatSwitchDirection::Up ||
                    hatSwitchDirection == JoystickHatSwitchDirection::RightUp)
                {
                    axisResult += 1.0f;
                }
                else if (hatSwitchDirection == JoystickHatSwitchDirection::LeftDown ||
                    hatSwitchDirection == JoystickHatSwitchDirection::Down ||
                    hatSwitchDirection == JoystickHatSwitchDirection::RightDown)
                {
                    axisResult -= 1.0f;
                }
            }

            for (const auto trackball : axisData.joystickXTrackballs)
            {
                if (joystick->HasTrackball(trackball))
                {
                    axisResult += glm::sign(joystick->GetTrackball(trackball).deltaX);
                }
            }

            for (const auto trackball : axisData.joystickYTrackballs)
            {
                if (joystick->HasTrackball(trackball))
                {
                    axisResult += glm::sign(joystick->GetTrackball(trackball).deltaY);
                }
            }
        }

        return axisResult;
    }

    auto InputManager::LoadAxesFromControlPrefs(const ControlPrefs& controlPrefs) -> void
    {
        for (const auto& [axisName, axisData] : controlPrefs.GetAxes())
        {
            for (const auto keyCode : axisData.keyboard.positiveKeys)
            {
                AddKeyToPositiveAxis(axisName, keyCode);
            }

            for (const auto keyCode : axisData.keyboard.negativeKeys)
            {
                AddKeyToNegativeAxis(axisName, keyCode);
            }

            for (const auto mouseButton : axisData.mouse.positiveButtons)
            {
                AddMouseButtonToPositiveAxis(axisName, mouseButton);
            }

            for (const auto mouseButton : axisData.mouse.negativeButtons)
            {
                AddMouseButtonToNegativeAxis(axisName, mouseButton);
            }

            for (const auto mouseAxis : axisData.mouse.axes)
            {
                AddMouseAxisToAxis(axisName, mouseAxis);
            }

            for (const auto gameControllerButton : axisData.gameController.positiveButtons)
            {
                AddGameControllerButtonToPositiveAxis(axisName, gameControllerButton);
            }

            for (const auto gameControllerButton : axisData.gameController.negativeButtons)
            {
                AddGameControllerButtonToNegativeAxis(axisName, gameControllerButton);
            }

            for (const auto gameControllerTrigger : axisData.gameController.positiveTriggers)
            {
                AddGameControllerTriggerToPositiveAxis(axisName, gameControllerTrigger);
            }

            for (const auto gameControllerTrigger : axisData.gameController.negativeTriggers)
            {
                AddGameControllerTriggerToNegativeAxis(axisName, gameControllerTrigger);
            }

            for (const auto gameControllerAxis : axisData.gameController.axes)
            {
                AddGameControllerAxisToAxis(axisName, gameControllerAxis);
            }

            for (const auto joystickButton : axisData.joystick.positiveButtons)
            {
                AddJoystickButtonToPositiveAxis(axisName, joystickButton);
            }

            for (const auto joystickButton : axisData.joystick.negativeButtons)
            {
                AddJoystickButtonToNegativeAxis(axisName, joystickButton);
            }

            for (const auto joystickTrigger : axisData.joystick.positiveTriggers)
            {
                AddJoystickAxisToPositiveAxis(axisName, joystickTrigger);
            }

            for (const auto joystickTrigger : axisData.joystick.negativeTriggers)
            {
                AddJoystickAxisToNegativeAxis(axisName, joystickTrigger);
            }

            for (const auto joystickAxis : axisData.joystick.axes)
            {
                AddJoystickAxisToAxis(axisName, joystickAxis);
            }

            for (const auto joystickHatSwitch : axisData.joystick.xHatSwitches)
            {
                AddJoystickHatSwitchXToAxis(axisName, joystickHatSwitch);
            }

            for (const auto joystickHatSwitch : axisData.joystick.yHatSwitches)
            {
                AddJoystickHatSwitchYToAxis(axisName, joystickHatSwitch);
            }

            for (const auto joystickTrackball : axisData.joystick.xTrackballs)
            {
                AddJoystickTrackballXToAxis(axisName, joystickTrackball);
            }

            for (const auto joystickTrackball : axisData.joystick.yTrackballs)
            {
                AddJoystickTrackballYToAxis(axisName, joystickTrackball);
            }
        }
    }

    auto InputManager::LoadButtonsFromControlPrefs(const ControlPrefs& controlPrefs) -> void
    {
        for (const auto& [buttonName, buttonData] : controlPrefs.GetButtons())
        {
            for (const auto keyCode : buttonData.keys)
            {
                AddKeyToButton(buttonName, keyCode);
            }

            for (const auto mouseButton : buttonData.mouseButtons)
            {
                AddMouseButtonToButton(buttonName, mouseButton);
            }

            for (const auto gameControllerButton : buttonData.gameController.buttons)
            {
                AddGameControllerButtonToButton(buttonName, gameControllerButton);
            }

            for (const auto gameControllerTrigger : buttonData.gameController.triggers)
            {
                AddGameControllerTriggerToButton(buttonName, gameControllerTrigger);
            }

            for (const auto joystickButton : buttonData.joystick.buttons)
            {
                AddJoystickButtonToButton(buttonName, joystickButton);
            }

            for (const auto joystickTrigger : buttonData.joystick.triggers)
            {
                AddJoystickAxisToButton(buttonName, joystickTrigger);
            }

            for (const auto& [joystickHatSwitch, hatSwitchDirections] : buttonData.joystick.hatSwitches)
            {
                for (const auto hatSwitchDirection : hatSwitchDirections)
                {
                    AddJoystickHatSwitchToButton(buttonName, joystickHatSwitch, hatSwitchDirection);
                }
            }
        }
    }
}
