#include "stardust/ai/game_tree/GameTree.h"

#include <limits>

namespace stardust
{
    namespace ai
    {
        [[nodiscard]] ObserverPtr<GameState> GameTree::GetMinimaxResult(const u32 depth, const bool startMaximising) const
        {
            return MinimaxHelper(m_rootState, depth, startMaximising).second;
        }

        [[nodiscard]] Pair<f32, ObserverPtr<GameState>> GameTree::MinimaxHelper(const UniquePtr<GameState>& currentState, const u32 depth, const bool isMaximising) const
        {
            if (depth == 0u || currentState->IsTerminal())
            {
                return { currentState->GetScore(), currentState.get() };
            }

            if (isMaximising)
            {
                f32 maxEvaluation = -std::numeric_limits<f32>::infinity();
                ObserverPtr<GameState> bestChildState = nullptr;

                const auto childStates = currentState->GetNextStates();

                for (const auto& childState : childStates)
                {
                    const auto [currentEvaluation, bestGrandchildState] = MinimaxHelper(childState, depth - 1u, false);

                    if (currentEvaluation > maxEvaluation)
                    {
                        maxEvaluation = currentEvaluation;
                        bestChildState = childState.get();
                    }
                }

                return { maxEvaluation, bestChildState };
            }
            else
            {
                f32 minEvaluation = std::numeric_limits<f32>::infinity();
                ObserverPtr<GameState> bestChildState = nullptr;

                const auto childStates = currentState->GetNextStates();

                for (const auto& childState : childStates)
                {
                    const auto [currentEvaluation, bestGrandchildState] = MinimaxHelper(childState, depth - 1u, true);

                    if (currentEvaluation < minEvaluation)
                    {
                        minEvaluation = currentEvaluation;
                        bestChildState = childState.get();
                    }
                }

                return { minEvaluation, bestChildState };
            }
        }
    }
}