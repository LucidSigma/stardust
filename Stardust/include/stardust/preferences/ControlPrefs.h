#pragma once
#ifndef STARDUST_CONTROL_PREFS_H
#define STARDUST_CONTROL_PREFS_H

#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/joystick/Joystick.h"
#include "stardust/input/joystick/JoystickCodes.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/types/Containers.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class ControlPrefs final
    {
    public:
        struct AxisData final
        {
            struct KeyboardData final
            {
                List<KeyCode> positiveKeys{ };
                List<KeyCode> negativeKeys{ };
            };

            struct MouseData final
            {
                List<MouseButton> positiveButtons{ };
                List<MouseButton> negativeButtons{ };

                List<MouseAxis> axes{ };
            };

            struct GameControllerData final
            {
                List<GameControllerButton> positiveButtons{ };
                List<GameControllerButton> negativeButtons{ };

                List<GameControllerTrigger> positiveTriggers{ };
                List<GameControllerTrigger> negativeTriggers{ };

                List<GameControllerAxis> axes{ };
            };

            struct JoystickData final
            {
                List<Joystick::ButtonID> positiveButtons{ };
                List<Joystick::ButtonID> negativeButtons{ };

                List<Joystick::AxisID> positiveTriggers{ };
                List<Joystick::AxisID> negativeTriggers{ };

                List<Joystick::AxisID> axes{ };

                List<Joystick::HatSwitchID> xHatSwitches{ };
                List<Joystick::HatSwitchID> yHatSwitches{ };

                List<Joystick::TrackballID> xTrackballs{ };
                List<Joystick::TrackballID> yTrackballs{ };
            };

            KeyboardData keyboard{ };
            MouseData mouse{ };
            GameControllerData gameController{ };
            JoystickData joystick{ };
        };

        struct ButtonData final
        {
            struct GameControllerData final
            {
                List<GameControllerButton> buttons{ };
                List<GameControllerTrigger> triggers{ };
            };

            struct JoystickData final
            {
                List<Joystick::ButtonID> buttons{ };
                List<Joystick::AxisID> triggers{ };
                HashMap<Joystick::HatSwitchID, List<JoystickHatSwitchDirection>> hatSwitches{ };
            };

            List<KeyCode> keys{ };
            List<MouseButton> mouseButtons{ };
            GameControllerData gameController{ };
            JoystickData joystick{ };
        };

    private:
        HashMap<String, AxisData> m_axes{ };
        HashMap<String, ButtonData> m_buttons{ };

        String m_filepath{ };

    public:
        auto Initialise(const StringView filepath) -> void;

        [[nodiscard]] auto MergeControlsFromJSONFile(const StringView filepath, const bool useVFS = true) -> Status;
        [[nodiscard]] auto MergeControlsFromMessagePackFile(const StringView filepath, const bool useVFS = true) -> Status;
        auto Save() -> Status;

        auto ClearAllAxes() -> void;
        auto ClearAllButtons() -> void;

        [[nodiscard]] auto HasAxis(const String& name) const -> bool;
        [[nodiscard]] auto HasButton(const String& name) const -> bool;
        [[nodiscard]] inline auto GetAxes() const noexcept -> HashMap<String, AxisData> { return m_axes; }
        [[nodiscard]] inline auto GetButtons() const noexcept -> HashMap<String, ButtonData> { return m_buttons; }

    private:
        [[nodiscard]] auto MergeControlsFromJSONObject(const JSON& controlsData) -> Status;

        [[nodiscard]] auto GetAxes(const JSON& controlsData) -> Status;
        auto GetKeyboardDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void;
        auto GetMouseDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void;
        auto GetGameControllerDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void;
        auto GetJoystickDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void;

        [[nodiscard]] auto GetButtons(const JSON& controlsData) -> Status;
        auto GetKeyboardDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void;
        auto GetMouseDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void;
        auto GetGameControllerDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void;
        auto GetJoystickDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void;

        auto WriteAxesToJSON(JSON& json) const -> void;
        auto WriteButtonsToJSON(JSON& json) const -> void;
    };
}

#endif
