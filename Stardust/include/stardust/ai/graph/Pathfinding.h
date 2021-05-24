#pragma once
#ifndef STARDUST_PATHFINDING_H
#define STARDUST_PATHFINDING_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        template <typename T>
        [[nodiscard]] Vector<T> DepthFirstTraversal(const Graph<T>& graph, const T& root, const Optional<T>& goalNode = NullOpt)
        {
            if (!graph.HasNode(root))
            {
                return { };
            }

            Vector<T> traversalOrder{ };
            HashSet<T> visitedNodes{ };

            Stack<T> nodeStack{ };
            nodeStack.push(root);

            while (!nodeStack.empty())
            {
                const T currentNode = nodeStack.top();
                nodeStack.pop();

                if (!visitedNodes.contains(currentNode))
                {
                    visitedNodes.insert(currentNode);
                    traversalOrder.push_back(currentNode);

                    if (goalNode.has_value() && currentNode == goalNode.value())
                    {
                        break;
                    }

                    const auto adjacentNodes = graph.GetAdjacentNodes(currentNode);

                    for (const auto& [node, weight] : adjacentNodes)
                    {
                        nodeStack.push(node);
                    }
                }
            }

            return traversalOrder;
        }

        template <typename T>
        [[nodiscard]] Vector<T> BreadthFirstTraversal(const Graph<T>& graph, const T& root, const Optional<T>& goalNode = NullOpt)
        {
            if (!graph.HasNode(root))
            {
                return { };
            }

            Vector<T> traversalOrder{ };
            HashSet<T> visitedNodes{ };
            visitedNodes.insert(root);

            Queue<T> nodeQueue{ };
            nodeQueue.push(root);

            while (!nodeQueue.empty())
            {
                const T currentNode = nodeQueue.front();
                nodeQueue.pop();
                
                traversalOrder.push_back(currentNode);

                if (goalNode.has_value() && currentNode == goalNode.value())
                {
                    break;
                }

                const auto adjacentNodes = graph.GetAdjacentNodes(currentNode);

                for (const auto& [node, weight] : adjacentNodes)
                {
                    if (!visitedNodes.contains(node))
                    {
                        visitedNodes.insert(node);
                        nodeQueue.push(node);
                    }
                }
            }

            return traversalOrder;
        }

        template <typename T>
        [[nodiscard]] Vector<T> BestFirstTraversal(const Graph<T>& graph, const T& root, const Optional<T>& goalNode = NullOpt)
        {
            if (!graph.HasNode(root))
            {
                return { };
            }

            struct NodeWeightSorter
            {
                [[nodiscard]] inline bool operator ()(const Pair<T, f32>& lhs, const Pair<T, f32>& rhs) const noexcept
                {
                    return lhs.second > rhs.second;
                }
            };

            Vector<T> traversalOrder{ };
            HashSet<T> visitedNodes{ };
            visitedNodes.insert(root);

            PriorityQueue<Pair<T, f32>, NodeWeightSorter> nodeQueue{ };
            nodeQueue.push({ root, 0.0f });

            while (!nodeQueue.empty())
            {
                const auto [currentNode, currentWeight] = nodeQueue.top();
                nodeQueue.pop();

                traversalOrder.push_back(currentNode);

                if (goalNode.has_value() && currentNode == goalNode.value())
                {
                    break;
                }

                const auto adjacentNodes = graph.GetEfficientAdjacentNodes(currentNode);

                for (const auto& [node, weight] : adjacentNodes)
                {
                    if (!visitedNodes.contains(node))
                    {
                        visitedNodes.insert(node);
                        nodeQueue.push({ node, weight });
                    }
                }
            }

            return traversalOrder;
        }
    }
}

#endif