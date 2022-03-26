#include "stardust/preferences/ControlPrefs.h"

#include <utility>

#include <nlohmann/json.hpp>

#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/filesystem/Filesystem.h"

namespace stardust
{
    namespace
    {
        static constexpr const char* ControlsKey = "controls";

        static constexpr const char* AxesKey = "axes";
        static constexpr const char* ButtonsKey = "buttons";

        static constexpr const char* KeyboardKey = "keyboard";
        static constexpr const char* MouseKey = "mouse";
        static constexpr const char* ControllerKey = "controller";
        static constexpr const char* JoystickKey = "joystick";

        static constexpr const char* KeysKey = "keys";
        static constexpr const char* PositiveKeysKey = "positive-keys";
        static constexpr const char* NegativeKeysKey = "negative-keys";
        static constexpr const char* MouseButtonsKey = "mouse-buttons";
        static constexpr const char* PositiveButtonsKey = "positive-buttons";
        static constexpr const char* NegativeButtonsKey = "negative-buttons";
        static constexpr const char* TriggersKey = "triggers";
        static constexpr const char* PositiveTriggersKey = "positive-triggers";
        static constexpr const char* NegativeTriggersKey = "negative-triggers";
        static constexpr const char* HatSwitchesKey = "hat-switches";
        static constexpr const char* TrackballsKey = "trackballs";

        static constexpr const char* XKey = "x";
        static constexpr const char* YKey = "y";

        static constexpr const char* IDKey = "id";
        static constexpr const char* DirectionsKey = "directions";

        template <typename From, typename To>
        concept StaticallyConvertible = requires(From a)
        {
            { static_cast<To>(a) };
        };

        template <typename T>
            requires StaticallyConvertible<T, i32>
        [[nodiscard]] auto ConvertToIntegerList(const List<T>& data) -> List<i32>
        {
            List<i32> convertedData{ };
            convertedData.reserve(data.size());

            for (const auto item : data)
            {
                convertedData.push_back(static_cast<i32>(item));
            }

            return convertedData;
        }
    }

    auto ControlPrefs::Initialise(const StringView filepath) -> void
    {
        m_filepath = filepath;
    }

    [[nodiscard]] auto ControlPrefs::MergeControlsFromJSONFile(const StringView filepath, const bool useVFS) -> Status
    {
        JSON controlsData{ };

        if (useVFS)
        {
            auto controlsFileResult = vfs::ReadJSON(filepath);

            if (controlsFileResult.is_err())
            {
                return Status::Fail;
            }

            controlsData = std::move(controlsFileResult).unwrap();
        }
        else
        {
            auto controlsFileResult = filesystem::ReadJSON(filepath);

            if (controlsFileResult.is_err())
            {
                return Status::Fail;
            }

            controlsData = std::move(controlsFileResult).unwrap();
        }

        return MergeControlsFromJSONObject(controlsData);
    }

    auto ControlPrefs::MergeControlsFromMessagePackFile(const StringView filepath, const bool useVFS) -> Status
    {
        JSON controlsData{ };

        if (useVFS)
        {
            auto controlsFileResult = vfs::ReadMessagePack(filepath);

            if (controlsFileResult.is_err())
            {
                return Status::Fail;
            }

            controlsData = std::move(controlsFileResult).unwrap();
        }
        else
        {
            auto controlsFileResult = filesystem::ReadMessagePack(filepath);

            if (controlsFileResult.is_err())
            {
                return Status::Fail;
            }

            controlsData = std::move(controlsFileResult).unwrap();
        }

        return MergeControlsFromJSONObject(controlsData);
    }

    auto ControlPrefs::Save() -> Status
    {
        try
        {
            JSON controlsJSON;
            controlsJSON[ControlsKey] = JSON();
            controlsJSON[ControlsKey][AxesKey] = JSON();
            controlsJSON[ControlsKey][ButtonsKey] = JSON();

            WriteAxesToJSON(controlsJSON);
            WriteButtonsToJSON(controlsJSON);

            return filesystem::WriteMessagePackToFile(m_filepath, controlsJSON);
        }
        catch (const nlohmann::json::parse_error& error)
        {
            Log::EngineError("JSON error when saving control preferences (parsing error) [{}].", error.what());

            return Status::Fail;
        }
        catch (const nlohmann::json::type_error& error)
        {
            Log::EngineError("JSON error when saving control preferences (type error) [{}].", error.what());

            return Status::Fail;
        }
    }

    auto ControlPrefs::ClearAllAxes() -> void
    {
        m_axes.clear();
    }

    auto ControlPrefs::ClearAllButtons() -> void
    {
        m_buttons.clear();
    }

    [[nodiscard]] auto ControlPrefs::HasAxis(const String& name) const -> bool
    {
        return m_axes.contains(name);
    }

    [[nodiscard]] auto ControlPrefs::HasButton(const String& name) const -> bool
    {
        return m_buttons.contains(name);
    }

    [[nodiscard]] auto ControlPrefs::MergeControlsFromJSONObject(const JSON& controlsData) -> Status
    {
        if (!controlsData.contains(ControlsKey) || !controlsData[ControlsKey].is_object())
        {
            return Status::Fail;
        }

        if (GetAxes(controlsData[ControlsKey]) != Status::Success)
        {
            return Status::Fail;
        }

        if (GetButtons(controlsData[ControlsKey]) != Status::Success)
        {
            return Status::Fail;
        }

        return Status::Success;
    }

    [[nodiscard]] auto ControlPrefs::GetAxes(const JSON& controlsData) -> Status
    {
        if (!controlsData.contains(AxesKey))
        {
            return Status::Success;
        }

        try
        {
            const auto& axes = controlsData[AxesKey];

            for (const auto& [axisName, axisData] : axes.items())
            {
                if (!m_axes.contains(axisName))
                {
                    m_axes[axisName] = AxisData{ };
                }

                AxisData& currentAxis = m_axes[axisName];

                GetKeyboardDataForAxis(axisData, currentAxis);
                GetMouseDataForAxis(axisData, currentAxis);
                GetGameControllerDataForAxis(axisData, currentAxis);
                GetJoystickDataForAxis(axisData, currentAxis);
            }
        }
        catch (const nlohmann::json::parse_error&)
        {
            Log::EngineError("Invalid JSON when merging control preferences (parsing error).");

            return Status::Fail;
        }
        catch (const nlohmann::json::type_error&)
        {
            Log::EngineError("Invalid JSON when merging control preferences (type error).");

            return Status::Fail;
        }

        return Status::Success;
    }

    auto ControlPrefs::GetKeyboardDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void
    {
        if (axisData.contains(KeyboardKey))
        {
            const auto& keyboardData = axisData[KeyboardKey];

            if (keyboardData.contains(PositiveKeysKey))
            {
                currentAxis.keyboard.positiveKeys.clear();

                for (const auto& keyCode : keyboardData[PositiveKeysKey])
                {
                    currentAxis.keyboard.positiveKeys.push_back(static_cast<KeyCode>(keyCode));
                }
            }

            if (keyboardData.contains(NegativeKeysKey))
            {
                currentAxis.keyboard.negativeKeys.clear();

                for (const auto& keyCode : keyboardData[NegativeKeysKey])
                {
                    currentAxis.keyboard.negativeKeys.push_back(static_cast<KeyCode>(keyCode));
                }
            }
        }
    }

    auto ControlPrefs::GetMouseDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void
    {
        if (axisData.contains(MouseKey))
        {
            const auto& mouseData = axisData[MouseKey];

            if (mouseData.contains(PositiveButtonsKey))
            {
                currentAxis.mouse.positiveButtons.clear();

                for (const auto& mouseButton : mouseData[PositiveButtonsKey])
                {
                    currentAxis.mouse.positiveButtons.push_back(static_cast<MouseButton>(mouseButton));
                }
            }

            if (mouseData.contains(NegativeButtonsKey))
            {
                currentAxis.mouse.negativeButtons.clear();

                for (const auto& mouseButton : mouseData[NegativeButtonsKey])
                {
                    currentAxis.mouse.negativeButtons.push_back(static_cast<MouseButton>(mouseButton));
                }
            }

            if (mouseData.contains(AxesKey))
            {
                currentAxis.mouse.axes.clear();

                for (const auto& mouseAxis : mouseData[AxesKey])
                {
                    currentAxis.mouse.axes.push_back(static_cast<MouseAxis>(mouseAxis));
                }
            }
        }
    }

    auto ControlPrefs::GetGameControllerDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void
    {
        if (axisData.contains(ControllerKey))
        {
            const auto& controllerData = axisData[ControllerKey];

            if (controllerData.contains(PositiveButtonsKey))
            {
                currentAxis.gameController.positiveButtons.clear();

                for (const auto& gameControllerButton : controllerData[PositiveButtonsKey])
                {
                    currentAxis.gameController.positiveButtons.push_back(static_cast<GameControllerButton>(gameControllerButton));
                }
            }

            if (controllerData.contains(NegativeButtonsKey))
            {
                currentAxis.gameController.negativeButtons.clear();

                for (const auto& gameControllerButton : controllerData[NegativeButtonsKey])
                {
                    currentAxis.gameController.negativeButtons.push_back(static_cast<GameControllerButton>(gameControllerButton));
                }
            }

            if (controllerData.contains(PositiveTriggersKey))
            {
                currentAxis.gameController.positiveTriggers.clear();

                for (const auto& gameControllerTrigger : controllerData[PositiveTriggersKey])
                {
                    currentAxis.gameController.positiveTriggers.push_back(static_cast<GameControllerTrigger>(gameControllerTrigger));
                }
            }

            if (controllerData.contains(NegativeTriggersKey))
            {
                currentAxis.gameController.negativeTriggers.clear();

                for (const auto& gameControllerTrigger : controllerData[NegativeTriggersKey])
                {
                    currentAxis.gameController.negativeTriggers.push_back(static_cast<GameControllerTrigger>(gameControllerTrigger));
                }
            }

            if (controllerData.contains(AxesKey))
            {
                currentAxis.gameController.axes.clear();

                for (const auto& gameControllerAxis : controllerData[AxesKey])
                {
                    currentAxis.gameController.axes.push_back(static_cast<GameControllerAxis>(gameControllerAxis));
                }
            }
        }
    }

    auto ControlPrefs::GetJoystickDataForAxis(const JSON& axisData, AxisData& currentAxis) const -> void
    {
        if (axisData.contains(JoystickKey))
        {
            const auto& joystickData = axisData[JoystickKey];

            if (joystickData.contains(PositiveButtonsKey))
            {
                currentAxis.joystick.positiveButtons.clear();

                for (const auto& joystickButton : joystickData[PositiveButtonsKey])
                {
                    currentAxis.joystick.positiveButtons.push_back(static_cast<Joystick::ButtonID>(joystickButton));
                }
            }

            if (joystickData.contains(NegativeButtonsKey))
            {
                currentAxis.joystick.negativeButtons.clear();

                for (const auto& joystickButton : joystickData[NegativeButtonsKey])
                {
                    currentAxis.joystick.negativeButtons.push_back(static_cast<Joystick::ButtonID>(joystickButton));
                }
            }

            if (joystickData.contains(PositiveTriggersKey))
            {
                currentAxis.joystick.positiveTriggers.clear();

                for (const auto& joystickTrigger : joystickData[PositiveTriggersKey])
                {
                    currentAxis.joystick.positiveTriggers.push_back(static_cast<Joystick::AxisID>(joystickTrigger));
                }
            }

            if (joystickData.contains(NegativeTriggersKey))
            {
                currentAxis.joystick.negativeTriggers.clear();

                for (const auto& joystickTrigger : joystickData[NegativeTriggersKey])
                {
                    currentAxis.joystick.negativeTriggers.push_back(static_cast<Joystick::AxisID>(joystickTrigger));
                }
            }

            if (joystickData.contains(AxesKey))
            {
                currentAxis.joystick.axes.clear();

                for (const auto& joystickAxis : joystickData[AxesKey])
                {
                    currentAxis.joystick.axes.push_back(static_cast<Joystick::AxisID>(joystickAxis));
                }
            }

            if (joystickData.contains(HatSwitchesKey))
            {
                const auto& hatSwitchesData = joystickData[HatSwitchesKey];

                if (hatSwitchesData.contains(XKey))
                {
                    currentAxis.joystick.xHatSwitches.clear();

                    for (const auto& joystickHatSwitch : hatSwitchesData[XKey])
                    {
                        currentAxis.joystick.xHatSwitches.push_back(static_cast<Joystick::HatSwitchID>(joystickHatSwitch));
                    }
                }

                if (hatSwitchesData.contains(YKey))
                {
                    currentAxis.joystick.yHatSwitches.clear();

                    for (const auto& joystickHatSwitch : hatSwitchesData[YKey])
                    {
                        currentAxis.joystick.yHatSwitches.push_back(static_cast<Joystick::HatSwitchID>(joystickHatSwitch));
                    }
                }
            }

            if (joystickData.contains(TrackballsKey))
            {
                const auto& trackballsData = joystickData[TrackballsKey];

                if (trackballsData.contains(XKey))
                {
                    currentAxis.joystick.xTrackballs.clear();

                    for (const auto& joystickTrackball : trackballsData[XKey])
                    {
                        currentAxis.joystick.xTrackballs.push_back(static_cast<Joystick::TrackballID>(joystickTrackball));
                    }
                }

                if (trackballsData.contains(YKey))
                {
                    currentAxis.joystick.yTrackballs.clear();

                    for (const auto& joystickTrackball : trackballsData[YKey])
                    {
                        currentAxis.joystick.yTrackballs.push_back(static_cast<Joystick::TrackballID>(joystickTrackball));
                    }
                }
            }
        }
    }

    [[nodiscard]] auto ControlPrefs::GetButtons(const JSON& controlsData) -> Status
    {
        if (!controlsData.contains(ButtonsKey))
        {
            return Status::Success;
        }

        try
        {
            const auto& buttons = controlsData[ButtonsKey];

            for (const auto& [buttonName, buttonData] : buttons.items())
            {
                if (!m_buttons.contains(buttonName))
                {
                    m_buttons[buttonName] = ButtonData{ };
                }

                ButtonData& currentButton = m_buttons[buttonName];

                GetKeyboardDataForButton(buttonData, currentButton);
                GetMouseDataForButton(buttonData, currentButton);
                GetGameControllerDataForButton(buttonData, currentButton);
                GetJoystickDataForButton(buttonData, currentButton);
            }
        }
        catch (const nlohmann::json::parse_error&)
        {
            Log::EngineError("Invalid JSON when merging control preferences (parsing error).");

            return Status::Fail;
        }
        catch (const nlohmann::json::type_error&)
        {
            Log::EngineError("Invalid JSON when merging control preferences (type error).");

            return Status::Fail;
        }

        return Status::Success;
    }

    auto ControlPrefs::GetKeyboardDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void
    {
        if (buttonData.contains(KeysKey))
        {
            currentButton.keys.clear();

            for (const auto& keyCode : buttonData[KeysKey])
            {
                currentButton.keys.push_back(static_cast<KeyCode>(keyCode));
            }
        }
    }

    auto ControlPrefs::GetMouseDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void
    {
        if (buttonData.contains(MouseButtonsKey))
        {
            currentButton.mouseButtons.clear();

            for (const auto& mouseButton : buttonData[MouseButtonsKey])
            {
                currentButton.mouseButtons.push_back(static_cast<MouseButton>(mouseButton));
            }
        }
    }

    auto ControlPrefs::GetGameControllerDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void
    {
        if (buttonData.contains(ControllerKey))
        {
            const auto& controllerData = buttonData[ControllerKey];

            if (controllerData.contains(ButtonsKey))
            {
                currentButton.gameController.buttons.clear();

                for (const auto& gameControllerButton : controllerData[ButtonsKey])
                {
                    currentButton.gameController.buttons.push_back(static_cast<GameControllerButton>(gameControllerButton));
                }
            }

            if (controllerData.contains(TriggersKey))
            {
                currentButton.gameController.triggers.clear();

                for (const auto& gameControllerTrigger : controllerData[TriggersKey])
                {
                    currentButton.gameController.triggers.push_back(static_cast<GameControllerTrigger>(gameControllerTrigger));
                }
            }
        }
    }

    auto ControlPrefs::GetJoystickDataForButton(const JSON& buttonData, ButtonData& currentButton) const -> void
    {
        if (buttonData.contains(JoystickKey))
        {
            const auto& joystickData = buttonData[JoystickKey];

            if (joystickData.contains(ButtonsKey))
            {
                currentButton.joystick.buttons.clear();

                for (const auto& joystickButton : joystickData[ButtonsKey])
                {
                    currentButton.joystick.buttons.push_back(static_cast<Joystick::ButtonID>(joystickButton));
                }
            }

            if (joystickData.contains(TriggersKey))
            {
                currentButton.joystick.triggers.clear();

                for (const auto& joystickTrigger : joystickData[TriggersKey])
                {
                    currentButton.joystick.triggers.push_back(static_cast<Joystick::AxisID>(joystickTrigger));
                }
            }

            if (joystickData.contains(HatSwitchesKey))
            {
                for (const auto& joystickHatSwitch : joystickData[HatSwitchesKey])
                {
                    if (!joystickHatSwitch.contains(IDKey))
                    {
                        continue;
                    }

                    const Joystick::HatSwitchID hatSwitchID = static_cast<Joystick::HatSwitchID>(joystickHatSwitch[IDKey]);
                    currentButton.joystick.hatSwitches[hatSwitchID] = { };

                    if (joystickHatSwitch.contains(DirectionsKey))
                    {
                        for (const auto& hatSwitchDirection : joystickHatSwitch[DirectionsKey])
                        {
                            currentButton.joystick.hatSwitches[hatSwitchID].push_back(static_cast<JoystickHatSwitchDirection>(hatSwitchDirection));
                        }
                    }
                }
            }
        }
    }

    auto ControlPrefs::WriteAxesToJSON(JSON& json) const -> void
    {
        for (const auto& [axisName, axisData] : m_axes)
        {
            json[ControlsKey][AxesKey][axisName] = JSON();
            auto& currentAxis = json[ControlsKey][AxesKey][axisName];

            if (!axisData.keyboard.positiveKeys.empty())
            {
                currentAxis[KeyboardKey][PositiveKeysKey] = ConvertToIntegerList(axisData.keyboard.positiveKeys);
            }

            if (!axisData.keyboard.negativeKeys.empty())
            {
                currentAxis[KeyboardKey][NegativeKeysKey] = ConvertToIntegerList(axisData.keyboard.negativeKeys);
            }

            if (!axisData.mouse.positiveButtons.empty())
            {
                currentAxis[MouseKey][PositiveButtonsKey] = ConvertToIntegerList(axisData.mouse.positiveButtons);
            }

            if (!axisData.mouse.negativeButtons.empty())
            {
                currentAxis[MouseKey][NegativeButtonsKey] = ConvertToIntegerList(axisData.mouse.negativeButtons);
            }

            if (!axisData.mouse.axes.empty())
            {
                currentAxis[MouseKey][AxesKey] = ConvertToIntegerList(axisData.mouse.axes);
            }

            if (!axisData.gameController.positiveButtons.empty())
            {
                currentAxis[ControllerKey][PositiveButtonsKey] = ConvertToIntegerList(axisData.gameController.positiveButtons);
            }

            if (!axisData.gameController.negativeButtons.empty())
            {
                currentAxis[ControllerKey][NegativeButtonsKey] = ConvertToIntegerList(axisData.gameController.negativeButtons);
            }

            if (!axisData.gameController.positiveTriggers.empty())
            {
                currentAxis[ControllerKey][PositiveTriggersKey] = ConvertToIntegerList(axisData.gameController.positiveTriggers);
            }

            if (!axisData.gameController.negativeTriggers.empty())
            {
                currentAxis[ControllerKey][NegativeTriggersKey] = ConvertToIntegerList(axisData.gameController.negativeTriggers);
            }

            if (!axisData.gameController.axes.empty())
            {
                currentAxis[ControllerKey][AxesKey] = ConvertToIntegerList(axisData.gameController.axes);
            }

            if (!axisData.joystick.positiveButtons.empty())
            {
                currentAxis[JoystickKey][PositiveButtonsKey] = ConvertToIntegerList(axisData.joystick.positiveButtons);
            }

            if (!axisData.joystick.negativeButtons.empty())
            {
                currentAxis[JoystickKey][NegativeButtonsKey] = ConvertToIntegerList(axisData.joystick.negativeButtons);
            }

            if (!axisData.joystick.positiveTriggers.empty())
            {
                currentAxis[JoystickKey][PositiveTriggersKey] = ConvertToIntegerList(axisData.joystick.positiveTriggers);
            }

            if (!axisData.joystick.negativeTriggers.empty())
            {
                currentAxis[JoystickKey][NegativeTriggersKey] = ConvertToIntegerList(axisData.joystick.negativeTriggers);
            }

            if (!axisData.joystick.axes.empty())
            {
                currentAxis[JoystickKey][AxesKey] = ConvertToIntegerList(axisData.joystick.axes);
            }

            if (!axisData.joystick.xHatSwitches.empty())
            {
                currentAxis[JoystickKey][HatSwitchesKey][XKey] = ConvertToIntegerList(axisData.joystick.xHatSwitches);
            }

            if (!axisData.joystick.yHatSwitches.empty())
            {
                currentAxis[JoystickKey][HatSwitchesKey][YKey] = ConvertToIntegerList(axisData.joystick.yHatSwitches);
            }

            if (!axisData.joystick.xTrackballs.empty())
            {
                currentAxis[JoystickKey][TrackballsKey][XKey] = ConvertToIntegerList(axisData.joystick.xTrackballs);
            }

            if (!axisData.joystick.yTrackballs.empty())
            {
                currentAxis[JoystickKey][TrackballsKey][YKey] = ConvertToIntegerList(axisData.joystick.yTrackballs);
            }
        }
    }

    auto ControlPrefs::WriteButtonsToJSON(JSON& json) const -> void
    {
        for (const auto& [buttonName, buttonData] : m_buttons)
        {
            json[ControlsKey][ButtonsKey][buttonName] = JSON();
            auto& currentButton = json[ControlsKey][ButtonsKey][buttonName];

            if (!buttonData.keys.empty())
            {
                currentButton[KeysKey] = ConvertToIntegerList(buttonData.keys);
            }

            if (!buttonData.mouseButtons.empty())
            {
                currentButton[MouseButtonsKey] = ConvertToIntegerList(buttonData.mouseButtons);
            }

            if (!buttonData.gameController.buttons.empty())
            {
                currentButton[ControllerKey][ButtonsKey] = ConvertToIntegerList(buttonData.gameController.buttons);
            }

            if (!buttonData.gameController.triggers.empty())
            {
                currentButton[ControllerKey][TriggersKey] = ConvertToIntegerList(buttonData.gameController.triggers);
            }

            if (!buttonData.joystick.buttons.empty())
            {
                currentButton[JoystickKey][ButtonsKey] = ConvertToIntegerList(buttonData.joystick.buttons);
            }

            if (!buttonData.joystick.triggers.empty())
            {
                currentButton[JoystickKey][TriggersKey] = ConvertToIntegerList(buttonData.joystick.triggers);
            }

            if (!buttonData.joystick.hatSwitches.empty())
            {
                List<JSON> hatSwitches{ };
                hatSwitches.reserve(buttonData.joystick.hatSwitches.size());

                for (const auto& [hatSwitch, hatSwitchDirections] : buttonData.joystick.hatSwitches)
                {
                    JSON currentHatSwitch{ };
                    currentHatSwitch[IDKey] = static_cast<i32>(hatSwitch);
                    currentHatSwitch[DirectionsKey] = ConvertToIntegerList(hatSwitchDirections);

                    hatSwitches.push_back(std::move(currentHatSwitch));
                }

                currentButton[JoystickKey][HatSwitchesKey] = hatSwitches;
            }
        }
    }
}
