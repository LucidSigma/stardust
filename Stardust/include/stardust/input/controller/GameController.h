#pragma once
#ifndef STARDUST_GAME_CONTROLLER_H
#define STARDUST_GAME_CONTROLLER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/graphics/Colour.h"
#include "stardust/math/Math.h"

namespace stardust
{
    class GameController
        : private INoncopyable

    {
    public:
        struct ButtonState
        {
            struct DPad
            {
                bool up;
                bool down;
                bool left;
                bool right;
            } dPad;

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

        struct Axes
        {
            struct Thumbstick
            {
                i16 x;
                i16 y;
            };

            Thumbstick left;
            Thumbstick right;

            i16 leftTrigger;
            i16 rightTrigger;
        };

        struct TouchpadFingerInfo
        {
            Vec2 position;
            f32 pressure;

            bool isTouching;
        };

    private:
        struct GameControllerDestroyer
        {
            void operator ()(SDL_GameController* const gameController) const noexcept;
        };

        i32 m_id = 0;
        u32 m_playerIndex = 0u;

        UniquePtr<SDL_GameController, GameControllerDestroyer> m_handle = nullptr;

        ButtonState m_currentButtons;
        ButtonState m_previousButtons;
        Axes m_axes;
        Vector<TouchpadFingerInfo> m_touchpadFingers{ };
        Vec3 m_accelerometerState = Vec3Zero;
        Vec3 m_gyroscopeState = Vec3Zero;

        bool m_hasLED = false;
        bool m_hasTouchpad = false;

        bool m_canRumble = false;
        bool m_canRumbleTriggers = false;

        bool m_hasAccelerometer = false;
        bool m_hasGyroscope = false;

    public:
        friend class Input;

        explicit GameController(const i32 id);

        GameController(GameController&& other) noexcept;
        GameController& operator =(GameController&& other) noexcept;

        ~GameController() noexcept = default;

        [[nodiscard]] bool IsButtonDown(const GameControllerButton button) const;
        [[nodiscard]] bool IsButtonPressed(const GameControllerButton button) const;
        [[nodiscard]] bool IsButtonUp(const GameControllerButton button) const;

        [[nodiscard]] bool IsAnyButtonDown(const Vector<GameControllerButton>& buttons) const;
        [[nodiscard]] bool IsAnyButtonPressed(const Vector<GameControllerButton>& buttons) const;
        [[nodiscard]] bool IsAnyButtonUp(const Vector<GameControllerButton>& buttons) const;

        [[nodiscard]] bool AreAllButtonsDown(const Vector<GameControllerButton>& buttons) const;
        [[nodiscard]] bool AreAllButtonsPressed(const Vector<GameControllerButton>& buttons) const;
        [[nodiscard]] bool AreAllButtonsUp(const Vector<GameControllerButton>& buttons) const;

        inline bool CanRumble() const noexcept { return m_canRumble; }
        void Rumble(const f32 lowFrequency, const f32 highFrequency, const u32 milliseconds) const;
        void StopRumbling() const;

        inline bool CanRumbleTriggers() const noexcept { return m_canRumbleTriggers; }
        void RumbleTriggers(const f32 leftIntensity, const f32 rightIntensity, const u32 milliseconds) const;
        void StopRumblingTriggers() const;

        inline bool HasLED() const noexcept { return m_hasLED; }
        void SetLED(const Colour& colour) const;

        inline bool HasTouchpad() const noexcept { return m_hasTouchpad; }
        [[nodiscard]] u32 GetSupportedTouchpadFingerCount() const;

        inline bool HasAccelerometer() const noexcept { return m_hasAccelerometer; }
        inline const Vec3& GetAccelerometerData() const noexcept { return m_accelerometerState; }
        inline bool HasGyroscope() const noexcept { return m_hasGyroscope; }
        inline const Vec3& GetGyroscopeData() const noexcept { return m_gyroscopeState; }

        inline i32 GetID() const noexcept { return m_id; }
        inline const Axes& GetAxes() const noexcept { return m_axes; }
        inline const Vector<TouchpadFingerInfo>& GetTouchpadFingers() const noexcept { return m_touchpadFingers; }

        inline u32 GetPlayerIndex() const noexcept { return m_playerIndex; }
        inline void SetPlayerIndex(const u32 playerIndex) noexcept { m_playerIndex = playerIndex; }

        [[nodiscard]] String GetName() const;

        [[nodiscard]] inline SDL_GameController* const GetRawHandle() const noexcept { return m_handle.get(); }
        [[nodiscard]] SDL_Joystick* const GetRawJoystickHandle() const;

    private:
        static bool GetButtonState(const GameControllerButton button, const ButtonState& buttonState) noexcept;
    
        void UpdateButtons();
        void UpdateAxes();
        void UpdateTouchpadFingers();
        void UpdateSensors();
    };
}

#endif