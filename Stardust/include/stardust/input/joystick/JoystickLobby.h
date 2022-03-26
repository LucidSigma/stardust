#pragma once
#ifndef STARDUST_JOYSTICK_LOBBY_H
#define STARDUST_JOYSTICK_LOBBY_H

#include <SDL2/SDL.h>

#include "stardust/input/joystick/Joystick.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class JoystickLobby final
    {
    private:
        HashMap<Joystick::InstanceID, Joystick> m_joysticks{ };

    public:
        auto Update(const class InputController& inputController) -> void;

        auto AddJoystick(const Joystick::ID id) -> ObserverPointer<Joystick>;
        auto RemoveJoystick(const Joystick::InstanceID instanceID) -> void;
        auto RemoveAllJoysticks() -> void;
        [[nodiscard]] inline auto GetJoystickCount() const noexcept -> u32 { return static_cast<u32>(m_joysticks.size()); }

        [[nodiscard]] auto DoesJoystickExist(const Joystick::InstanceID instanceID) -> bool;

        [[nodiscard]] auto GetJoystick(const Joystick::InstanceID instanceID) -> ObserverPointer<Joystick>;
        [[nodiscard]] auto GetJoystick(const Joystick::InstanceID instanceID) const -> ObserverPointer<const Joystick>;
        [[nodiscard]] auto GetJoystickByPlayerIndex(const Joystick::PlayerIndex playerIndex) -> ObserverPointer<Joystick>;
        [[nodiscard]] auto GetJoystickByPlayerIndex(const Joystick::PlayerIndex playerIndex) const -> ObserverPointer<const Joystick>;

        [[nodiscard]] inline auto GetJoystickss() noexcept -> HashMap<Joystick::InstanceID, Joystick>& { return m_joysticks; }
        [[nodiscard]] inline auto GetJoystickss() const noexcept -> const HashMap<Joystick::InstanceID, Joystick>& { return m_joysticks; }

        [[nodiscard]] auto GetJoystickList() -> List<ObserverPointer<Joystick>>;
        [[nodiscard]] auto GetJoystickList() const -> List<ObserverPointer<const Joystick>>;
    };
}

#endif
