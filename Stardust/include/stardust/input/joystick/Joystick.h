#pragma once
#ifndef STARDUST_JOYSTICK_H
#define STARDUST_JOYSTICK_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <limits>
#include <type_traits>

#include <SDL2/SDL.h>

#include "stardust/graphics/colour/Colour.h"
#include "stardust/input/joystick/JoystickCodes.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Joystick final
        : private INoncopyable
    {
    public:
        using ID = i32;
        using InstanceID = SDL_JoystickID;
        using PlayerIndex = u32;

        using ButtonID = usize;
        using AxisID = usize;
        using HatSwitchID = usize;
        using TrackballID = usize;

        static constexpr PlayerIndex InvalidPlayerIndex = std::numeric_limits<PlayerIndex>::max();

        enum class Type
            : std::underlying_type_t<SDL_JoystickType>
        {
            Unknown = SDL_JOYSTICK_TYPE_UNKNOWN,
            GameController = SDL_JOYSTICK_TYPE_GAMECONTROLLER,
            Wheel = SDL_JOYSTICK_TYPE_WHEEL,
            ArcadeStick = SDL_JOYSTICK_TYPE_ARCADE_STICK,
            FlightStick = SDL_JOYSTICK_TYPE_FLIGHT_STICK,
            DancePad = SDL_JOYSTICK_TYPE_DANCE_PAD,
            Guitar = SDL_JOYSTICK_TYPE_GUITAR,
            DrumKit = SDL_JOYSTICK_TYPE_DRUM_KIT,
            ArcadePad = SDL_JOYSTICK_TYPE_ARCADE_PAD,
            Throttle = SDL_JOYSTICK_TYPE_THROTTLE,
        };

        struct Button final
        {
            bool previousState = false;
            bool currentState = false;
        };

        struct Trackball final
        {
            f32 deltaX = 0.0f;
            f32 deltaY = 0.0f;
        };

    private:
        ID m_id = 0;
        PlayerIndex m_playerIndex = InvalidPlayerIndex;

        String m_name;
        String m_guid;

        SDL_Joystick* m_handle = nullptr;
        bool m_isOwning = false;

        List<bool> m_currentButtonStates{ };
        List<bool> m_previousButtonStates{ };

        List<f32> m_currentAxes{ };
        List<f32> m_previousAxes{ };

        List<JoystickHatSwitchDirection> m_currentHatSwitches{ };
        List<JoystickHatSwitchDirection> m_previousHatSwitches{ };

        List<Trackball> m_trackballs{ };

    public:
        explicit Joystick(const ID id);
        explicit Joystick(const class GameController& gameController);

        Joystick(Joystick&& other) noexcept;
        auto operator =(Joystick&& other) noexcept -> Joystick&;

        ~Joystick() noexcept;

        [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_handle != nullptr; }

        auto Update(const class InputController& inputController) -> void;

        [[nodiscard]] inline auto GetButtonCount() const noexcept -> u32 { return static_cast<u32>(m_currentButtonStates.size()); }
        [[nodiscard]] inline auto GetAxisCount() const noexcept -> u32 { return static_cast<u32>(m_currentAxes.size()); }
        [[nodiscard]] inline auto GetHatSwitchCount() const noexcept -> u32 { return static_cast<u32>(m_currentHatSwitches.size()); }
        [[nodiscard]] inline auto GetTrackballCount() const noexcept -> u32 { return static_cast<u32>(m_trackballs.size()); }

        [[nodiscard]] inline auto HasButton(const ButtonID button) const noexcept -> bool { return button < GetButtonCount(); }
        [[nodiscard]] inline auto HasAxis(const AxisID axis) const noexcept -> bool { return axis < GetAxisCount(); }
        [[nodiscard]] inline auto HasHatSwitch(const HatSwitchID hatSwitch) const noexcept -> bool { return hatSwitch < GetHatSwitchCount(); }
        [[nodiscard]] inline auto HasTrackball(const TrackballID trackball) const noexcept -> bool { return trackball < GetTrackballCount(); }

        [[nodiscard]] auto IsButtonDown(const ButtonID button) const -> bool;
        [[nodiscard]] auto IsButtonPressed(const ButtonID button) const -> bool;
        [[nodiscard]] auto IsButtonUp(const ButtonID button) const -> bool;

        [[nodiscard]] auto IsAnyButtonDown(const List<ButtonID>& buttons) const -> bool;
        [[nodiscard]] auto IsAnyButtonPressed(const List<ButtonID>& buttons) const -> bool;
        [[nodiscard]] auto IsAnyButtonUp(const List<ButtonID>& buttons) const -> bool;

        [[nodiscard]] auto AreAllButtonsDown(const List<ButtonID>& buttons) const -> bool;
        [[nodiscard]] auto AreAllButtonsPressed(const List<ButtonID>& buttons) const -> bool;
        [[nodiscard]] auto AreAllButtonsUp(const List<ButtonID>& buttons) const -> bool;

        [[nodiscard]] auto AreNoButtonsDown(const List<ButtonID>& buttons) const -> bool;
        [[nodiscard]] auto AreNoButtonsPressed(const List<ButtonID>& buttons) const -> bool;
        [[nodiscard]] auto AreNoButtonsUp(const List<ButtonID>& buttons) const -> bool;

        [[nodiscard]] auto IsAxisDown(const AxisID axis) const -> bool;
        [[nodiscard]] auto IsAxisPressed(const AxisID axis) const -> bool;
        [[nodiscard]] auto IsAxisUp(const AxisID axis) const -> bool;

        [[nodiscard]] auto WasHatSwitchMoved(const HatSwitchID hatSwitch, const Optional<JoystickHatSwitchDirection>& direction = None) const -> bool;
        [[nodiscard]] auto IsHatSwitchMoved(const HatSwitchID hatSwitch, const Optional<JoystickHatSwitchDirection>& direction = None) const -> bool;
        [[nodiscard]] auto WasHatSwitchReturned(const HatSwitchID hatSwitch, const Optional<JoystickHatSwitchDirection>& direction = None) const -> bool;

        [[nodiscard]] inline auto GetAxes() const noexcept -> const List<f32>& { return m_currentAxes; }
        [[nodiscard]] inline auto GetHatSwitches() const noexcept -> const List<JoystickHatSwitchDirection>& { return m_currentHatSwitches; }
        [[nodiscard]] inline auto GetTrackball() const noexcept -> const List<Trackball>& { return m_trackballs; }

        [[nodiscard]] inline auto GetAxis(const AxisID axis) const -> f32 { return m_currentAxes[axis]; }
        [[nodiscard]] inline auto GetHatSwitch(const HatSwitchID hatSwitch) const -> JoystickHatSwitchDirection { return m_currentHatSwitches[hatSwitch]; }
        [[nodiscard]] inline auto GetTrackball(const TrackballID trackball) const -> Trackball { return m_trackballs[trackball]; }

        [[nodiscard]] auto CanRumble() const -> bool;
        auto Rumble(const f32 lowFrequency, const f32 highFrequency, const u32 milliseconds) const -> void;
        auto StopRumbling() const -> void;

        [[nodiscard]] auto CanRumbleTriggers() const -> bool;
        auto RumbleTriggers(const f32 leftIntensity, const f32 rightIntensity, const u32 milliseconds) const -> void;
        auto StopRumblingTriggers() const -> void;

        [[nodiscard]] auto HasLED() const -> bool;
        auto SetLED(const Colour& colour) const -> void;

        [[nodiscard]] inline auto GetID() const noexcept -> ID { return m_id; }
        [[nodiscard]] auto GetInstanceID() const -> InstanceID;

        [[nodiscard]] inline auto GetPlayerIndex() const noexcept -> PlayerIndex { return m_playerIndex; }
        [[nodiscard]] inline auto SetPlayerIndex(const PlayerIndex playerIndex) noexcept -> void { m_playerIndex = playerIndex; }

        [[nodiscard]] inline auto GetName() const noexcept -> const String& { return m_name; }
        [[nodiscard]] inline auto GetGUID() const noexcept -> const String& { return m_guid; }
        [[nodiscard]] auto GetType() const noexcept -> Type;

        [[nodiscard]] inline auto GetRawHandle() const noexcept -> SDL_Joystick* { return m_handle; }

    private:
        auto Initialise() -> void;
        auto Destroy() noexcept -> void;

        auto UpdateButtons() -> void;
        auto UpdateAxes(const class InputController& inputController) -> void;
        auto UpdateHatSwitches() -> void;
        auto UpdateTrackballs() -> void;
    };
}

#endif
