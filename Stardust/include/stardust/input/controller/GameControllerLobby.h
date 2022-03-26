#pragma once
#ifndef STARDUST_GAME_CONTROLLER_LOBBY_H
#define STARDUST_GAME_CONTROLLER_LOBBY_H

#include <SDL2/SDL.h>

#include "stardust/input/controller/GameController.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class GameControllerLobby final
    {
    private:
        HashMap<GameController::InstanceID, GameController> m_gameControllers{ };

    public:
        auto Update(const class InputController& inputController) -> void;

        auto AddGameController(const GameController::ID id) -> ObserverPointer<GameController>;
        auto RemoveGameController(const GameController::InstanceID instanceID) -> void;
        auto RemoveAllGameControllers() -> void;
        [[nodiscard]] inline auto GetGameControllerCount() const noexcept -> u32 { return static_cast<u32>(m_gameControllers.size()); }

        [[nodiscard]] auto DoesGameControllerExist(const GameController::InstanceID instanceID) -> bool;

        [[nodiscard]] auto GetGameController(const GameController::InstanceID instanceID) -> ObserverPointer<GameController>;
        [[nodiscard]] auto GetGameController(const GameController::InstanceID instanceID) const -> ObserverPointer<const GameController>;
        [[nodiscard]] auto GetGameControllerByPlayerIndex(const GameController::PlayerIndex playerIndex) -> ObserverPointer<GameController>;
        [[nodiscard]] auto GetGameControllerByPlayerIndex(const GameController::PlayerIndex playerIndex) const -> ObserverPointer<const GameController>;

        [[nodiscard]] inline auto GetGameControllers() noexcept -> HashMap<GameController::InstanceID, GameController>& { return m_gameControllers; }
        [[nodiscard]] inline auto GetGameControllers() const noexcept -> const HashMap<GameController::InstanceID, GameController>& { return m_gameControllers; }

        [[nodiscard]] auto GetGameControllerList() -> List<ObserverPointer<GameController>>;
        [[nodiscard]] auto GetGameControllerList() const -> List<ObserverPointer<const GameController>>;
    };
}

#endif
