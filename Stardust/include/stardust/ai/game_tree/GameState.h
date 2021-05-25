#pragma once
#ifndef STARDUST_GAME_STATE_H
#define STARDUST_GAME_STATE_H

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        class GameState
        {
        public:
            GameState() = default;
            virtual ~GameState() noexcept = default;

            [[nodiscard]] virtual f32 GetScore() const = 0;
            [[nodiscard]] virtual bool IsTerminal() const = 0;

            [[nodiscard]] virtual Vector<UniquePtr<GameState>> GetPossibleMoves() const = 0;
        };
    }
}

#endif