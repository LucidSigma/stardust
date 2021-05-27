#pragma once
#ifndef STARDUST_MINIMAX_H
#define STARDUST_MINIMAX_H

#include <algorithm>
#include <functional>
#include <limits>

#include "stardust/ai/game_tree/GameState.h"
#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        template <typename T>
        [[nodiscard]] GameState<T> EvaluateMinimax(const GameState<T>& state, const u32 depth, const bool startMaximising = true)
        {
            using MinimaxHelperFunction = std::function<Pair<f32, GameState<T>>(const GameState<T>&, const u32, const bool)>;

            static const MinimaxHelperFunction minimaxHelper = [](const GameState<T>& currentState, const u32 currentDepth, const bool isMaximising) -> Pair<f32, GameState<T>>
            {
                if (currentDepth == 0u || currentState.IsTerminal())
                {
                    return { currentState.GetScore(), currentState };
                }

                if (isMaximising)
                {
                    f32 maxEvaluation = -std::numeric_limits<f32>::infinity();
                    GameState<T> bestChildState{ };

                    const auto childStates = currentState.GetNextStates();

                    for (const auto& childState : childStates)
                    {
                        const auto [currentEvaluation, bestGrandchildState] = minimaxHelper(childState, currentDepth - 1u, false);

                        if (currentEvaluation > maxEvaluation)
                        {
                            maxEvaluation = currentEvaluation;
                            bestChildState = childState;
                        }
                    }

                    return { maxEvaluation, bestChildState };
                }
                else
                {
                    f32 minEvaluation = std::numeric_limits<f32>::infinity();
                    GameState<T> bestChildState{ };

                    const auto childStates = currentState.GetNextStates();

                    for (const auto& childState : childStates)
                    {
                        const auto [currentEvaluation, bestGrandchildState] = minimaxHelper(childState, currentDepth - 1u, true);

                        if (currentEvaluation < minEvaluation)
                        {
                            minEvaluation = currentEvaluation;
                            bestChildState = childState;
                        }
                    }

                    return { minEvaluation, bestChildState };
                }
            };

            return minimaxHelper(state, depth, startMaximising).second;
        }

        template <typename T>
        [[nodiscard]] GameState<T> EvaluateAlphaBetaMinimax(const GameState<T>& state, const u32 depth, const bool startMaximising = true)
        {
            using AlphaBetaHelperFunction = std::function<Pair<f32, GameState<T>>(const GameState<T>&, const u32, const bool, f32, f32)>;

            static const AlphaBetaHelperFunction alphaBetaHelper = [](const GameState<T>& currentState, const u32 currentDepth, const bool isMaximising, f32 alpha, f32 beta) -> Pair<f32, GameState<T>>
            {
                if (currentDepth == 0u || currentState.IsTerminal())
                {
                    return { currentState.GetScore(), currentState };
                }

                if (isMaximising)
                {
                    f32 maxEvaluation = -std::numeric_limits<f32>::infinity();
                    GameState<T> bestChildState{ };

                    const auto childStates = currentState.GetNextStates();

                    for (const auto& childState : childStates)
                    {
                        const auto [currentEvaluation, bestGrandchildState] = alphaBetaHelper(childState, currentDepth - 1u, false, alpha, beta);

                        if (currentEvaluation > maxEvaluation)
                        {
                            maxEvaluation = currentEvaluation;
                            bestChildState = childState;
                        }

                        alpha = std::max(alpha, currentEvaluation);

                        if (beta <= alpha)
                        {
                            break;
                        }
                    }

                    return { maxEvaluation, bestChildState };
                }
                else
                {
                    f32 minEvaluation = std::numeric_limits<f32>::infinity();
                    GameState<T> bestChildState{ };

                    const auto childStates = currentState.GetNextStates();

                    for (const auto& childState : childStates)
                    {
                        const auto [currentEvaluation, bestGrandchildState] = alphaBetaHelper(childState, currentDepth - 1u, true, alpha, beta);

                        if (currentEvaluation < minEvaluation)
                        {
                            minEvaluation = currentEvaluation;
                            bestChildState = childState;
                        }

                        beta = std::min(beta, currentEvaluation);

                        if (beta <= alpha)
                        {
                            break;
                        }
                    }

                    return { minEvaluation, bestChildState };
                }
            };

            return alphaBetaHelper(state, depth, startMaximising, -std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity()).second;
        }
    }
}

#endif