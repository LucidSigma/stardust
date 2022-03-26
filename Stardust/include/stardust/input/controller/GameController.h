#pragma once
#ifndef STARDUST_GAME_CONTROLLER_H
#define STARDUST_GAME_CONTROLLER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <limits>
#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/graphics/colour/Colour.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/joystick/Joystick.h"
#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class GameController final
        : private INoncopyable
    {
    public:
        using ID = i32;
        using InstanceID = SDL_JoystickID;
        using PlayerIndex = u32;

        static constexpr PlayerIndex InvalidPlayerIndex = std::numeric_limits<PlayerIndex>::max();

        struct ButtonState final
        {
            struct DPad final
            {
                bool up;
                bool down;
                bool left;
                bool right;
            };

            DPad dPad;

            bool a;
            bool b;
            bool x;
            bool y;

            bool back;
            bool guide;
            bool start;

            bool leftStick;
            bool rightStick;

            bool leftShoulder;
            bool rightShoulder;

            bool misc;
            bool touchpad;

            Array<bool, 4u> paddles;
        };

        struct Axes final
        {
            struct Thumbstick final
            {
                f32 x;
                f32 y;
            };

            Thumbstick left;
            Thumbstick right;

            f32 leftTrigger;
            f32 rightTrigger;
        };

        struct TouchpadFingerInfo final
        {
            Vector2 position;
            f32 pressure;

            bool isTouching;
        };

        enum class ThumbstickInputType
        {
            Orthogonal,
            Radial,
        };

        enum class Type
            : std::underlying_type_t<SDL_GameControllerType>
        {
            Xbox360 = SDL_CONTROLLER_TYPE_XBOX360,
            XboxOne = SDL_CONTROLLER_TYPE_XBOXONE,
            PS3 = SDL_CONTROLLER_TYPE_PS3,
            PS4 = SDL_CONTROLLER_TYPE_PS4,
            PS5 = SDL_CONTROLLER_TYPE_PS5,
            NintendoSwitchPro = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO,
            AmazonLuna = SDL_CONTROLLER_TYPE_AMAZON_LUNA,
            GoogleStadia = SDL_CONTROLLER_TYPE_GOOGLE_STADIA,
            Virtual = SDL_CONTROLLER_TYPE_VIRTUAL,
            Other = SDL_CONTROLLER_TYPE_UNKNOWN,
        };

    private:
        struct GameControllerDestroyer final
        {
            void operator ()(SDL_GameController* const gameController) const noexcept;
        };

        inline static ThumbstickInputType s_leftThumbstickInputType = ThumbstickInputType::Orthogonal;
        inline static ThumbstickInputType s_rightThumbstickInputType = ThumbstickInputType::Orthogonal;

        ID m_id = 0;
        PlayerIndex m_playerIndex = InvalidPlayerIndex;

        String m_name;
        String m_guid;

        UniquePointer<SDL_GameController, GameControllerDestroyer> m_handle = nullptr;

        ButtonState m_currentButtonsState{ };
        ButtonState m_previousButtonsState{ };

        Axes m_axes{ };
        f32 m_previousLeftTriggerState = 0.0f;
        f32 m_previousRightTriggerState = 0.0f;

        List<TouchpadFingerInfo> m_touchpadFingers{ };
        Vector3 m_accelerometerState = Vector3Zero;
        Vector3 m_gyroscopeState = Vector3Zero;

        bool m_hasTouchpad = false;

        bool m_hasAccelerometer = false;
        bool m_hasGyroscope = false;

    public:
        [[nodiscard]] inline static auto GetLeftThumbstickInputType() noexcept -> ThumbstickInputType { return s_leftThumbstickInputType; }
        [[nodiscard]] inline static auto GetRightThumbstickInputType() noexcept -> ThumbstickInputType { return s_rightThumbstickInputType; }
        inline static auto SetLeftThumbstickInputType(const ThumbstickInputType thumbstickInputType) noexcept -> void { s_leftThumbstickInputType = thumbstickInputType; }
        inline static auto SetRightThumbstickInputType(const ThumbstickInputType thumbstickInputType) noexcept -> void { s_rightThumbstickInputType = thumbstickInputType; }

        explicit GameController(const ID id);

        GameController(GameController&& other) noexcept;
        auto operator =(GameController&& other) noexcept -> GameController&;

        ~GameController() noexcept = default;

        [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }

        auto Update(const class InputController& inputController) -> void;

        [[nodiscard]] auto IsButtonDown(const GameControllerButton button) const -> bool;
        [[nodiscard]] auto IsButtonPressed(const GameControllerButton button) const -> bool;
        [[nodiscard]] auto IsButtonUp(const GameControllerButton button) const -> bool;

        [[nodiscard]] auto IsAnyButtonDown(const List<GameControllerButton>& buttons) const -> bool;
        [[nodiscard]] auto IsAnyButtonPressed(const List<GameControllerButton>& buttons) const -> bool;
        [[nodiscard]] auto IsAnyButtonUp(const List<GameControllerButton>& buttons) const -> bool;

        [[nodiscard]] auto AreAllButtonsDown(const List<GameControllerButton>& buttons) const -> bool;
        [[nodiscard]] auto AreAllButtonsPressed(const List<GameControllerButton>& buttons) const -> bool;
        [[nodiscard]] auto AreAllButtonsUp(const List<GameControllerButton>& buttons) const -> bool;

        [[nodiscard]] auto AreNoButtonsDown(const List<GameControllerButton>& buttons) const -> bool;
        [[nodiscard]] auto AreNoButtonsPressed(const List<GameControllerButton>& buttons) const -> bool;
        [[nodiscard]] auto AreNoButtonsUp(const List<GameControllerButton>& buttons) const -> bool;

        [[nodiscard]] auto IsTriggerDown(const GameControllerTrigger trigger) const -> bool;
        [[nodiscard]] auto IsTriggerPressed(const GameControllerTrigger trigger) const -> bool;
        [[nodiscard]] auto IsTriggerUp(const GameControllerTrigger trigger) const -> bool;

        [[nodiscard]] auto CanRumble() const -> bool;
        auto Rumble(const f32 lowFrequency, const f32 highFrequency, const u32 milliseconds) const -> void;
        auto StopRumbling() const -> void;

        [[nodiscard]] auto CanRumbleTriggers() const -> bool;
        auto RumbleTriggers(const f32 leftIntensity, const f32 rightIntensity, const u32 milliseconds) const -> void;
        auto StopRumblingTriggers() const -> void;

        [[nodiscard]] auto HasLED() const -> bool;
        auto SetLED(const Colour& colour) const -> void;

        [[nodiscard]] inline auto HasTouchpad() const noexcept -> bool { return m_hasTouchpad; }
        [[nodiscard]] auto GetSupportedTouchpadFingerCount() const -> u32;

        [[nodiscard]] inline auto HasAccelerometer() const noexcept -> bool { return m_hasAccelerometer; }
        [[nodiscard]] inline auto GetAccelerometerData() const noexcept -> const Vector3 { return m_accelerometerState; }
        [[nodiscard]] inline auto HasGyroscope() const noexcept -> bool { return m_hasGyroscope; }
        [[nodiscard]] inline auto GetGyroscopeData() const noexcept -> const Vector3 { return m_gyroscopeState; }

        [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
        [[nodiscard]] auto GetInstanceID() const -> InstanceID;

        [[nodiscard]] inline auto GetAxes() const noexcept -> const Axes& { return m_axes; }
        [[nodiscard]] inline auto GetTouchpadFingers() const noexcept -> const List<TouchpadFingerInfo>& { return m_touchpadFingers; }

        [[nodiscard]] inline auto GetPlayerIndex() const noexcept -> PlayerIndex { return m_playerIndex; }
        [[nodiscard]] inline auto SetPlayerIndex(const PlayerIndex playerIndex) noexcept -> void { m_playerIndex = playerIndex; }

        [[nodiscard]] inline auto GetName() const noexcept -> const String& { return m_name; }
        [[nodiscard]] inline auto GetGUID() const noexcept -> const String& { return m_guid; }
        [[nodiscard]] auto GetType() const noexcept -> Type;

        [[nodiscard]] inline auto GetRawHandle() const noexcept -> SDL_GameController* { return m_handle.get(); }
        [[nodiscard]] auto GetRawJoystickHandle() const -> SDL_Joystick*;

        [[nodiscard]] explicit operator Joystick() const;

    private:
        [[nodiscard]] static auto GetButtonState(const GameControllerButton button, const ButtonState& buttonState) noexcept -> bool;

        auto UpdateButtons() -> void;
        auto UpdateAxes(const class InputController& inputController) -> void;
        [[nodiscard]] auto GetNormalisedThumbstickValue(const Vector2 rawAxisData, const f32 deadzone, const ThumbstickInputType thumbstickInputType) -> Axes::Thumbstick;
        [[nodiscard]] auto GetNormalisedTriggerValue(const i16 rawAxisData, const f32 deadzone) -> f32;
        auto UpdateTouchpadFingers() -> void;
        auto UpdateSensors() -> void;
    };
}

#endif
