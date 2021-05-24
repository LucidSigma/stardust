#pragma once
#ifndef STARDUST_TRAVERSAL_H
#define STARDUST_TRAVERSAL_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        template <typename T>
        [[nodiscard]] Vector<T> DepthFirstTraversal(const Graph<T>& graph, const T& rootNode, const Optional<T>& goalNode = NullOpt)
        {
            if (!graph.HasNode(rootNode))
            {
                return { };
            }

            Vector<T> traversalOrder{ };
            HashSet<T> visitedNodes{ };

            Stack<T> nodeStack{ };
            nodeStack.push(rootNode);

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

                    for (const auto& [adjacentNode, weight] : adjacentNodes)
                    {
                        nodeStack.push(adjacentNode);
                    }
                }
            }

            return traversalOrder;
        }

        template <typename T>
        [[nodiscard]] Vector<T> BreadthFirstTraversal(const Graph<T>& graph, const T& rootNode, const Optional<T>& goalNode = NullOpt)
        {
            if (!graph.HasNode(rootNode))
            {
                return { };
            }

            Vector<T> traversalOrder{ };
            HashSet<T> visitedNodes{ };
            visitedNodes.insert(rootNode);

            Queue<T> nodeQueue{ };
            nodeQueue.push(rootNode);

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

                for (const auto& [adjacentNode, weight] : adjacentNodes)
                {
                    if (!visitedNodes.contains(adjacentNode))
                    {
                        visitedNodes.insert(adjacentNode);
                        nodeQueue.push(adjacentNode);
                    }
                }
            }

            return traversalOrder;
        }

        template <typename T>
        [[nodiscard]] Vector<T> BestFirstTraversal(const Graph<T>& graph, const T& rootNode, const Optional<T>& goalNode = NullOpt)
        {
            if (!graph.HasNode(rootNode))
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
            visitedNodes.insert(rootNode);

            PriorityQueue<Pair<T, f32>, NodeWeightSorter> nodeQueue{ };
            nodeQueue.push({ rootNode, 0.0f });

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

                for (const auto& [adjacentNode, weight] : adjacentNodes)
                {
                    if (!visitedNodes.contains(adjacentNode))
                    {
                        visitedNodes.insert(adjacentNode);
                        nodeQueue.push({ adjacentNode, weight });
                    }
                }
            }

            return traversalOrder;
        }
    }
}

#endif