#include "stardust/input/joystick/JoystickLobby.h"

#include <utility>

#include "stardust/input/InputController.h"

namespace stardust
{
    auto JoystickLobby::Update(const InputController& inputController) -> void
    {
        for (auto& [instanceID, joystick] : m_joysticks)
        {
            joystick.Update(inputController);
        }
    }

    auto JoystickLobby::AddJoystick(const Joystick::ID id) -> ObserverPointer<Joystick>
    {
        Joystick joystick(id);
        const Joystick::InstanceID instanceID = joystick.GetInstanceID();

        m_joysticks.emplace(instanceID, std::move(joystick));

        if (!m_joysticks.at(instanceID).IsValid())
        {
            m_joysticks.erase(instanceID);

            return nullptr;
        }

        return &m_joysticks.at(instanceID);
    }

    auto JoystickLobby::RemoveJoystick(const Joystick::InstanceID instanceID) -> void
    {
        if (m_joysticks.contains(instanceID))
        {
            m_joysticks.erase(instanceID);
        }
    }

    auto JoystickLobby::RemoveAllJoysticks() -> void
    {
        m_joysticks.clear();
    }

    [[nodiscard]] auto JoystickLobby::DoesJoystickExist(const Joystick::InstanceID instanceID) -> bool
    {
        return m_joysticks.contains(instanceID);;
    }

    [[nodiscard]] auto JoystickLobby::GetJoystick(const Joystick::InstanceID instanceID) -> ObserverPointer<Joystick>
    {
        if (m_joysticks.contains(instanceID))
        {
            return &m_joysticks.at(instanceID);
        }

        return nullptr;
    }

    [[nodiscard]] auto JoystickLobby::GetJoystick(const Joystick::InstanceID instanceID) const -> ObserverPointer<const Joystick>
    {
        if (m_joysticks.contains(instanceID))
        {
            return &m_joysticks.at(instanceID);
        }

        return nullptr;
    }

    [[nodiscard]] auto JoystickLobby::GetJoystickByPlayerIndex(const Joystick::PlayerIndex playerIndex) -> ObserverPointer<Joystick>
    {
        for (const auto& [instanceID, joystick] : m_joysticks)
        {
            if (joystick.GetPlayerIndex() == playerIndex)
            {
                return &m_joysticks.at(instanceID);
            }
        }

        return nullptr;
    }

    [[nodiscard]] auto JoystickLobby::GetJoystickByPlayerIndex(const Joystick::PlayerIndex playerIndex) const -> ObserverPointer<const Joystick>
    {
        for (const auto& [instanceID, joystick] : m_joysticks)
        {
            if (joystick.GetPlayerIndex() == playerIndex)
            {
                return &m_joysticks.at(instanceID);
            }
        }

        return nullptr;
    }

    [[nodiscard]] auto JoystickLobby::GetJoystickList() -> List<ObserverPointer<Joystick>>
    {
        List<ObserverPointer<Joystick>> joysticks{ };
        joysticks.reserve(m_joysticks.size());

        for (const auto& [instanceID, joystick] : m_joysticks)
        {
            joysticks.push_back(&m_joysticks.at(instanceID));
        }

        return joysticks;
    }

    [[nodiscard]] auto JoystickLobby::GetJoystickList() const -> List<ObserverPointer<const Joystick>>
    {
        List<ObserverPointer<const Joystick>> joysticks{ };
        joysticks.reserve(m_joysticks.size());

        for (const auto& [instanceID, joystick] : m_joysticks)
        {
            joysticks.push_back(&m_joysticks.at(instanceID));
        }

        return joysticks;
    }
}
