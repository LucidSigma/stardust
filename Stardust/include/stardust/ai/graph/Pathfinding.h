#pragma once
#ifndef STARDUST_PATHFINDING_H
#define STARDUST_PATHFINDING_H

#include <algorithm>
#include <limits>

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

                    for (const auto& [node, weight] : adjacentNodes)
                    {
                        nodeStack.push(node);
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

        template <typename T>
        [[nodiscard]] Vector<T> DijkstrasAlgorithm(const Graph<T>& graph, const T& rootNode, const T& goalNode)
        {
            if (!graph.HasNode(rootNode) || !graph.HasNode(goalNode))
            {
                return { };
            }

            HashMap<T, f32> nodeDistances{ };
            nodeDistances[rootNode] = 0.0f;

            constexpr auto NodeDistanceSorter = [](const Pair<T, f32>& lhs, const Pair<T, f32>& rhs) -> bool
            {
                return lhs.second > rhs.second;
            };

            HashMap<T, Optional<T>> predecessorNodes{ };
            Vector<Pair<T, f32>> nodeQueue{ };
            const auto allNodes = graph.GetNodes();

            for (const auto& node : allNodes)
            {
                if (node != rootNode)
                {
                    nodeDistances[node] = std::numeric_limits<f32>::max();
                    predecessorNodes[node] = NullOpt;
                }

                nodeQueue.push_back({ node, nodeDistances[node] });
                std::ranges::sort(nodeQueue, NodeDistanceSorter);
            }

            bool wasNodeFound = false;

            while (!nodeQueue.empty())
            {
                const auto [currentNode, minDistance] = nodeQueue.back();
                nodeQueue.pop_back();

                const auto adjacentNodes = graph.GetEfficientAdjacentNodes(currentNode);

                for (const auto& [node, distance] : adjacentNodes)
                {
                    const f32 candidateDistance = nodeDistances[currentNode] + distance;

                    if (candidateDistance < nodeDistances[node])
                    {
                        nodeDistances[node] = candidateDistance;
                        predecessorNodes[node] = currentNode;

                        std::ranges::find_if(nodeQueue, [node](const auto& nodePair) -> bool { return nodePair.first == node; })->second = candidateDistance;
                        std::ranges::sort(nodeQueue, NodeDistanceSorter);
                    }
                }

                if (currentNode == goalNode && minDistance != std::numeric_limits<f32>::max())
                {
                    wasNodeFound = true;

                    break;
                }
            }

            if (!wasNodeFound)
            {
                return { };
            }

            Vector<T> traversedNodes{ };
            Optional<T> currentNode = goalNode;

            while (currentNode.has_value())
            {
                traversedNodes.push_back(currentNode.value());
                currentNode = predecessorNodes[currentNode.value()];
            }

            std::ranges::reverse(traversedNodes);

            return traversedNodes;
        }
    }
}

#endif