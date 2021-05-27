#pragma once
#ifndef STARDUST_GAME_STATE_H
#define STARDUST_GAME_STATE_H

#include <functional>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        template <typename T>
        struct GameState
        {
            T state;

            std::function<f32(const T&)> getScoreCallback;
            std::function<bool(const T&)> isTerminalCallback;
            std::function<Vector<GameState<T>>(const T&)> getNextStatesCallback;

            [[nodiscard]] inline f32 GetScore() const { return getScoreCallback(this->state); }
            [[nodiscard]] inline bool IsTerminal() const { return isTerminalCallback(this->state); }
            [[nodiscard]] inline Vector<GameState<T>> GetNextStates() const { return getNextStatesCallback(this->state); }
        };
    }
}

#endif 