#pragma once
#ifndef STARDUST_INPUT_CONTROLLER_H
#define STARDUST_INPUT_CONTROLLER_H

#include "stardust/input/controller/GameControllerLobby.h"
#include "stardust/input/joystick/JoystickLobby.h"
#include "stardust/input/keyboard/KeyboardState.h"
#include "stardust/input/mouse/MouseState.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class InputController final
    {
    private:
        KeyboardState m_keyboardState;
        MouseState m_mouseState;
        GameControllerLobby m_gameControllerLobby;
        JoystickLobby m_joystickLobby;

        f32 m_gameControllerDeadzone = 0.0f;
        f32 m_joystickDeadzone = 0.0f;

    public:
        [[nodiscard]] static auto LoadGameControllerDatabase(const StringView gameControllerDatabasePath) -> Status;

        [[nodiscard]] static auto GenerateTouchEventsFromMouseEvents(const bool generateTouchEvents) -> void;
        [[nodiscard]] static auto GenerateMouseEventsFromTouchEvents(const bool generateMouseEvents) -> void;

        auto Update() -> void;

        [[nodiscard]] inline auto GetKeyboardState() -> KeyboardState& { return m_keyboardState; }
        [[nodiscard]] inline auto GetKeyboardState() const -> const KeyboardState& { return m_keyboardState; }
        [[nodiscard]] inline auto GetMouseState() -> MouseState& { return m_mouseState; }
        [[nodiscard]] inline auto GetMouseState() const -> const MouseState& { return m_mouseState; }
        [[nodiscard]] inline auto GetGameControllerLobby() -> GameControllerLobby& { return m_gameControllerLobby; }
        [[nodiscard]] inline auto GetGameControllerLobby() const -> const GameControllerLobby& { return m_gameControllerLobby; }
        [[nodiscard]] inline auto GetJoystickLobby() -> JoystickLobby& { return m_joystickLobby; }
        [[nodiscard]] inline auto GetJoystickLobby() const -> const JoystickLobby& { return m_joystickLobby; }

        [[nodiscard]] inline auto GetGameControllerDeadzone() const noexcept -> f32 { return m_gameControllerDeadzone; }
        inline auto SetGameControllerDeadzone(const f32 gameControllerDeadzone) noexcept -> void { m_gameControllerDeadzone = glm::clamp(gameControllerDeadzone, 0.0f, 1.0f); }

        [[nodiscard]] inline auto GetJoystickDeadzone() const noexcept -> f32 { return m_joystickDeadzone; }
        inline auto SetJoystickDeadzone(const f32 joystickDeadzone) noexcept -> void { m_joystickDeadzone = glm::clamp(joystickDeadzone, 0.0f, 1.0f); }
    };
}

#endif
