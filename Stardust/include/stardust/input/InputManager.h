#pragma once
#ifndef STARDUST_INPUT_MANAGER_H
#define STARDUST_INPUT_MANAGER_H

#include "stardust/input/controller/GameController.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/joystick/Joystick.h"
#include "stardust/input/joystick/JoystickCodes.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/input/InputController.h"
#include "stardust/preferences/ControlPrefs.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class InputManager final
    {
    private:
        enum class JoystickAxisSign
            : i32
        {
            Negative = -1,
            Both = 0,
            Positive = 1,
        };

        struct JoystickAxisInfo final
        {
            JoystickAxisSign sign = JoystickAxisSign::Both;
            bool isInverted = true;
        };

        struct ButtonInput final
        {
            HashSet<KeyCode> keys{ };

            HashSet<MouseButton> mouseButtons{ };

            HashSet<GameControllerButton> gameControllerButtons{ };
            HashSet<GameControllerTrigger> gameControllerTriggers{ };

            HashSet<Joystick::ButtonID> joystickButtons{ };
            HashSet<Joystick::AxisID> joystickAxes{ };
            HashMap<Joystick::HatSwitchID, HashSet<JoystickHatSwitchDirection>> joystickHatSwitchDirections{ };
        };

        struct AxisInput final
        {
            HashSet<KeyCode> positiveKeys{ };
            HashSet<KeyCode> negativeKeys{ };

            HashSet<MouseButton> positiveMouseButtons{ };
            HashSet<MouseButton> negativeMouseButtons{ };
            HashSet<MouseAxis> mouseAxes{ };

            HashSet<GameControllerButton> positiveGameControllerButtons{ };
            HashSet<GameControllerButton> negativeGameControllerButtons{ };
            HashSet<GameControllerTrigger> positiveGameControllerTriggers{ };
            HashSet<GameControllerTrigger> negativeGameControllerTriggers{ };
            HashMap<GameControllerAxis, bool> gameControllerAxes{ };

            HashSet<Joystick::ButtonID> positiveJoystickButtons{ };
            HashSet<Joystick::ButtonID> negativeJoystickButtons{ };
            HashMap<Joystick::AxisID, JoystickAxisInfo> joystickAxes{ };
            HashSet<Joystick::HatSwitchID> joystickXHatSwitches{ };
            HashSet<Joystick::HatSwitchID> joystickYHatSwitches{ };
            HashSet<Joystick::TrackballID> joystickXTrackballs{ };
            HashSet<Joystick::TrackballID> joystickYTrackballs{ };
        };

        HashMap<String, ButtonInput> m_buttons{ };
        HashMap<String, AxisInput> m_axes{ };

        ObserverPointer<const InputController> m_inputController = nullptr;

    public:
        InputManager() = default;
        InputManager(const InputController& inputController);

        auto Initialise(const InputController& inputController) -> void;

        auto AddKeyToButton(const String& buttonName, const KeyCode key) -> void;
        auto AddMouseButtonToButton(const String& buttonName, const MouseButton button) -> void;
        auto AddGameControllerButtonToButton(const String& buttonName, const GameControllerButton button) -> void;
        auto AddGameControllerTriggerToButton(const String& buttonName, const GameControllerTrigger trigger) -> void;
        auto AddJoystickButtonToButton(const String& buttonName, const Joystick::ButtonID button) -> void;
        auto AddJoystickAxisToButton(const String& buttonName, const Joystick::AxisID axis) -> void;
        auto AddJoystickHatSwitchToButton(const String& buttonName, const Joystick::HatSwitchID hatSwitch, const JoystickHatSwitchDirection direction) -> void;

        auto RemoveKeyFromButton(const String& buttonName, const KeyCode key) -> void;
        auto RemoveMouseButtonFromButton(const String& buttonName, const MouseButton button) -> void;
        auto RemoveGameControllerButtonFromButton(const String& buttonName, const GameControllerButton button) -> void;
        auto RemoveGameControllerTriggerFromButton(const String& buttonName, const GameControllerTrigger trigger) -> void;
        auto RemoveJoystickButtonFromButton(const String& buttonName, const Joystick::ButtonID button) -> void;
        auto RemoveJoystickAxisFromButton(const String& buttonName, const Joystick::AxisID axis) -> void;
        auto RemoveJoystickHatSwitchDirectionFromButton(const String& buttonName, const Joystick::HatSwitchID hatSwitch, const JoystickHatSwitchDirection direction) -> void;
        auto RemoveJoystickHatSwitchFromButton(const String& buttonName, const Joystick::HatSwitchID hatSwitch) -> void;

        auto RemoveButton(const String& buttonName) -> void;
        auto ClearAllButtons() -> void;

        [[nodiscard]] auto HasButton(const String& buttonName) -> bool;

        [[nodiscard]] auto IsButtonDown(const String& buttonName, const List<ObserverPointer<const GameController>>& gameControllers = { }, const List<ObserverPointer<const Joystick>>& joysticks = { }) const -> bool;
        [[nodiscard]] auto IsButtonPressed(const String& buttonName, const List<ObserverPointer<const GameController>>& gameControllers = { }, const List<ObserverPointer<const Joystick>>& joysticks = { }) const -> bool;
        [[nodiscard]] auto IsButtonUp(const String& buttonName, const List<ObserverPointer<const GameController>>& gameControllers = { }, const List<ObserverPointer<const Joystick>>& joysticks = { }) const -> bool;

        auto AddKeyToPositiveAxis(const String& axisName, const KeyCode key) -> void;
        auto AddKeyToNegativeAxis(const String& axisName, const KeyCode key) -> void;

        auto AddMouseButtonToPositiveAxis(const String& axisName, const MouseButton button) -> void;
        auto AddMouseButtonToNegativeAxis(const String& axisName, const MouseButton button) -> void;
        auto AddMouseAxisToAxis(const String& axisName, const MouseAxis axis) -> void;

        auto AddGameControllerButtonToPositiveAxis(const String& axisName, const GameControllerButton button) -> void;
        auto AddGameControllerButtonToNegativeAxis(const String& axisName, const GameControllerButton button) -> void;
        auto AddGameControllerTriggerToPositiveAxis(const String& axisName, const GameControllerTrigger trigger) -> void;
        auto AddGameControllerTriggerToNegativeAxis(const String& axisName, const GameControllerTrigger trigger) -> void;
        auto AddGameControllerAxisToAxis(const String& axisName, const GameControllerAxis axis, const bool isInverted = false) -> void;

        auto AddJoystickButtonToPositiveAxis(const String& axisName, const Joystick::ButtonID button) -> void;
        auto AddJoystickButtonToNegativeAxis(const String& axisName, const Joystick::ButtonID button) -> void;
        auto AddJoystickAxisToPositiveAxis(const String& axisName, const Joystick::AxisID axis) -> void;
        auto AddJoystickAxisToNegativeAxis(const String& axisName, const Joystick::AxisID axis) -> void;
        auto AddJoystickAxisToAxis(const String& axisName, const Joystick::AxisID axis, const bool isInverted = false) -> void;
        auto AddJoystickHatSwitchXToAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void;
        auto AddJoystickHatSwitchYToAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void;
        auto AddJoystickTrackballXToAxis(const String& axisName, const Joystick::TrackballID trackball) -> void;
        auto AddJoystickTrackballYToAxis(const String& axisName, const Joystick::TrackballID trackball) -> void;

        auto RemoveKeyFromPositiveAxis(const String& axisName, const KeyCode key) -> void;
        auto RemoveKeyFromNegativeAxis(const String& axisName, const KeyCode key) -> void;

        auto RemoveMouseButtonFromPositiveAxis(const String& axisName, const MouseButton button) -> void;
        auto RemoveMouseButtonFromNegativeAxis(const String& axisName, const MouseButton button) -> void;
        auto RemoveMouseAxisFromAxis(const String& axisName, const MouseAxis axis) -> void;

        auto RemoveGameControllerButtonFromPositiveAxis(const String& axisName, const GameControllerButton button) -> void;
        auto RemoveGameControllerButtonFromNegativeAxis(const String& axisName, const GameControllerButton button) -> void;
        auto RemoveGameControllerTriggerFromPositiveAxis(const String& axisName, const GameControllerTrigger trigger) -> void;
        auto RemoveGameControllerTriggerFromNegativeAxis(const String& axisName, const GameControllerTrigger trigger) -> void;
        auto RemoveGameControllerAxisFromAxis(const String& axisName, const GameControllerAxis axis) -> void;

        auto RemoveJoystickButtonFromPositiveAxis(const String& axisName, const Joystick::ButtonID button) -> void;
        auto RemoveJoystickButtonFromNegativeAxis(const String& axisName, const Joystick::ButtonID button) -> void;
        auto RemoveJoystickAxisFromAxis(const String& axisName, const Joystick::AxisID axis) -> void;
        auto RemoveJoystickHatSwitchXFromAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void;
        auto RemoveJoystickHatSwitchYFromAxis(const String& axisName, const Joystick::HatSwitchID hatSwitch) -> void;
        auto RemoveJoystickTrackballXFromAxis(const String& axisName, const Joystick::TrackballID trackball) -> void;
        auto RemoveJoystickTrackballYFromAxis(const String& axisName, const Joystick::TrackballID trackball) -> void;

        auto RemoveAxis(const String& axisName) -> void;
        auto ClearAllAxes() -> void;

        [[nodiscard]] auto IsAxisInverted(const String& axisName, const GameControllerAxis gameControllerAxis) const -> bool;
        [[nodiscard]] auto IsAxisInverted(const String& axisName, const Joystick::AxisID joystickAxis) const -> bool;
        auto InvertAxis(const String& axisName, const GameControllerAxis gameControllerAxis, const bool invertAxis) -> void;
        auto InvertAxis(const String& axisName, const Joystick::AxisID joystickAxis, const bool invertAxis) -> void;

        [[nodiscard]] auto HasAxis(const String& axisName) -> bool;

        [[nodiscard]] auto GetAxis(const String& axisName, const List<ObserverPointer<const GameController>>& gameControllers = { }, const List<ObserverPointer<const Joystick>>& joysticks = { }) const -> i32;
        [[nodiscard]] auto GetAxisRaw(const String& axisName, const List<ObserverPointer<const GameController>>& gameControllers = { }, const List<ObserverPointer<const Joystick>>& joysticks = { }) const -> f32;
        [[nodiscard]] auto GetClampedAxisRaw(const String& axisName, const List<ObserverPointer<const GameController>>& gameControllers = { }, const List<ObserverPointer<const Joystick>>& joysticks = { }) const -> f32;

        auto LoadFromControlPrefs(const ControlPrefs& controlPrefs) -> void;

    private:
        [[nodiscard]] auto GetAxisValueFromKeyboard(const AxisInput& axisData) const -> f32;
        [[nodiscard]] auto GetAxisValueFromMouse(const AxisInput& axisData) const -> f32;
        [[nodiscard]] auto GetAxisValueFromGameControllers(const AxisInput& axisData, const List<ObserverPointer<const GameController>>& gameControllers) const -> f32;
        [[nodiscard]] auto GetAxisValueFromJoysticks(const AxisInput& axisData, const List<ObserverPointer<const Joystick>>& joysticks) const -> f32;

        auto LoadAxesFromControlPrefs(const ControlPrefs& controlPrefs) -> void;
        auto LoadButtonsFromControlPrefs(const ControlPrefs& controlPrefs) -> void;
    };
}

#endif
