#pragma once
#ifndef STARDUST_GAME_TREE_H
#define STARDUST_GAME_TREE_H

#include <concepts>
#include <type_traits>
#include <utility>

#include "stardust/ai/game_tree/GameState.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
    namespace ai
    {
        class GameTree
        {
        private:
            UniquePtr<GameState> m_rootState;

        public:
            template <std::derived_from<GameState> T, typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
            GameTree(Args&&... args)
                : m_rootState(std::make_unique<T>(std::forward<Args>(args)...))
            { }

            ~GameTree() noexcept = default;

            [[nodiscard]] UniquePtr<GameState>& GetRootState() noexcept { return m_rootState; }
            [[nodiscard]] const UniquePtr<GameState>& GetRootState() const noexcept { return m_rootState; }
        };
    }
}

#endif