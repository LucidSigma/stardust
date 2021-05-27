#pragma once
#ifndef STARDUST_GAME_STATE_H
#define STARDUST_GAME_STATE_H

#include <functional>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        template <typename T>
        struct GameState
        {
            T state;

            std::function<f32(const T&)> getScore;
            std::function<bool(const T&)> isTerminal;

            std::function<Vector<GameState<T>>(const T&)> getNextScores;
        };
    }
}

#endif 