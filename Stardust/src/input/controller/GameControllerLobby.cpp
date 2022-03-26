#include "stardust/input/controller/GameControllerLobby.h"

#include <utility>

#include "stardust/input/InputController.h"

namespace stardust
{
    auto GameControllerLobby::Update(const InputController& inputController) -> void
    {
        for (auto& [instanceID, gameController] : m_gameControllers)
        {
            gameController.Update(inputController);
        }
    }

    auto GameControllerLobby::AddGameController(const GameController::ID id) -> ObserverPointer<GameController>
    {
        GameController gameController(id);
        const GameController::InstanceID instanceID = gameController.GetInstanceID();

        m_gameControllers.emplace(instanceID, std::move(gameController));

        if (!m_gameControllers.at(instanceID).IsValid())
        {
            m_gameControllers.erase(instanceID);

            return nullptr;
        }

        return &m_gameControllers.at(instanceID);
    }

    auto GameControllerLobby::RemoveGameController(const GameController::InstanceID instanceID) -> void
    {
        if (m_gameControllers.contains(instanceID))
        {
            m_gameControllers.erase(instanceID);
        }
    }

    auto GameControllerLobby::RemoveAllGameControllers() -> void
    {
        m_gameControllers.clear();
    }
    
    [[nodiscard]] auto GameControllerLobby::DoesGameControllerExist(const GameController::InstanceID instanceID) -> bool
    {
        return m_gameControllers.contains(instanceID);
    }

    [[nodiscard]] auto GameControllerLobby::GetGameController(const GameController::InstanceID instanceID) -> ObserverPointer<GameController>
    {
        if (m_gameControllers.contains(instanceID))
        {
            return &m_gameControllers.at(instanceID);
        }

        return nullptr;
    }

    [[nodiscard]] auto GameControllerLobby::GetGameController(const GameController::InstanceID instanceID) const -> ObserverPointer<const GameController>
    {
        if (m_gameControllers.contains(instanceID))
        {
            return &m_gameControllers.at(instanceID);
        }

        return nullptr;
    }
    
    [[nodiscard]] auto GameControllerLobby::GetGameControllerByPlayerIndex(const GameController::PlayerIndex playerIndex) -> ObserverPointer<GameController>
    {
        for (const auto& [instanceID, gameController] : m_gameControllers)
        {
            if (gameController.GetPlayerIndex() == playerIndex)
            {
                return &m_gameControllers.at(instanceID);
            }
        }

        return nullptr;
    }

    [[nodiscard]] auto GameControllerLobby::GetGameControllerByPlayerIndex(const GameController::PlayerIndex playerIndex) const -> ObserverPointer<const GameController>
    {
        for (const auto& [instanceID, gameController] : m_gameControllers)
        {
            if (gameController.GetPlayerIndex() == playerIndex)
            {
                return &m_gameControllers.at(instanceID);
            }
        }

        return nullptr;
    }
    
    [[nodiscard]] auto GameControllerLobby::GetGameControllerList() -> List<ObserverPointer<GameController>>
    {
        List<ObserverPointer<GameController>> gameControllers{ };
        gameControllers.reserve(m_gameControllers.size());

        for (const auto& [instanceID, gameController] : m_gameControllers)
        {
            gameControllers.push_back(&m_gameControllers.at(instanceID));
        }

        return gameControllers;
    }

    [[nodiscard]] auto GameControllerLobby::GetGameControllerList() const -> List<ObserverPointer<const GameController>>
    {
        List<ObserverPointer<const GameController>> gameControllers{ };
        gameControllers.reserve(m_gameControllers.size());

        for (const auto& [instanceID, gameController] : m_gameControllers)
        {
            gameControllers.push_back(&m_gameControllers.at(instanceID));
        }

        return gameControllers;
    }
}
