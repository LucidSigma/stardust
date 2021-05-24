#pragma once
#ifndef STARDUST_PATHFINDING_H
#define STARDUST_PATHFINDING_H

#include <algorithm>
#include <functional>
#include <limits>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        template <typename T>
        [[nodiscard]] HashMap<T, f32> DijkstrasAlgorithm(const Graph<T>& graph, const T& rootNode)
        {
            if (!graph.HasNode(rootNode))
            {
                return { };
            }

            HashMap<T, f32> nodeDistances{ };
            nodeDistances[rootNode] = 0.0f;

            constexpr auto NodeDistanceSorter = [](const Pair<T, f32>& lhs, const Pair<T, f32>& rhs) -> bool
            {
                return lhs.second > rhs.second;
            };

            Vector<Pair<T, f32>> nodeQueue{ };
            const auto allNodes = graph.GetNodes();

            for (const auto& node : allNodes)
            {
                if (node != rootNode)
                {
                    nodeDistances[node] = std::numeric_limits<f32>::infinity();
                }

                nodeQueue.push_back({ node, nodeDistances[node] });
                std::ranges::sort(nodeQueue, NodeDistanceSorter);
            }

            while (!nodeQueue.empty())
            {
                const auto [currentNode, minDistance] = nodeQueue.back();
                nodeQueue.pop_back();

                const auto adjacentNodes = graph.GetEfficientAdjacentNodes(currentNode);

                for (const auto& [adjacentNode, distance] : adjacentNodes)
                {
                    const f32 candidateDistance = nodeDistances[currentNode] + distance;

                    if (candidateDistance < nodeDistances[adjacentNode])
                    {
                        nodeDistances[adjacentNode] = candidateDistance;

                        std::ranges::find_if(nodeQueue, [adjacentNode](const auto& nodePair) -> bool { return nodePair.first == adjacentNode; })->second = candidateDistance;
                        std::ranges::sort(nodeQueue, NodeDistanceSorter);
                    }
                }
            }

            return nodeDistances;
        }

        template <typename T>
        [[nodiscard]] Vector<T> DijkstrasAlgorithmSearch(const Graph<T>& graph, const T& rootNode, const T& goalNode)
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
                    nodeDistances[node] = std::numeric_limits<f32>::infinity();
                }

                predecessorNodes[node] = NullOpt;

                nodeQueue.push_back({ node, nodeDistances[node] });
                std::ranges::sort(nodeQueue, NodeDistanceSorter);
            }

            bool wasNodeFound = false;

            while (!nodeQueue.empty())
            {
                const auto [currentNode, minDistance] = nodeQueue.back();
                nodeQueue.pop_back();

                const auto adjacentNodes = graph.GetEfficientAdjacentNodes(currentNode);

                for (const auto& [adjacentNode, distance] : adjacentNodes)
                {
                    const f32 candidateDistance = nodeDistances[currentNode] + distance;

                    if (candidateDistance < nodeDistances[adjacentNode])
                    {
                        nodeDistances[adjacentNode] = candidateDistance;
                        predecessorNodes[adjacentNode] = currentNode;

                        std::ranges::find_if(nodeQueue, [adjacentNode](const auto& nodePair) -> bool { return nodePair.first == adjacentNode; })->second = candidateDistance;
                        std::ranges::sort(nodeQueue, NodeDistanceSorter);
                    }
                }

                if (currentNode == goalNode && minDistance != std::numeric_limits<f32>::infinity())
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

        template <typename T>
        using HeuristicFunction = std::function<f32(const T&, const T&)>;

        template <typename T>
        [[nodiscard]] Vector<T> AStarSearch(const Graph<T>& graph, const T& rootNode, const T& goalNode, const HeuristicFunction<T>& heuristicFunction)
        {
            if (!graph.HasNode(rootNode) || !graph.HasNode(goalNode))
            {
                return { };
            }

            struct NodeScoreSorter
            {
                [[nodiscard]] inline bool operator ()(const Pair<T, f32>& lhs, const Pair<T, f32>& rhs) const noexcept
                {
                    return lhs.second > rhs.second;
                }
            };

            PriorityQueue<Pair<T, f32>, NodeScoreSorter> openSet{ };
            HashSet<T> visitedNodes{ };
            HashMap<T, Optional<T>> predecessorNodes{ };

            HashMap<T, f32> fScores{ };
            HashMap<T, f32> gScores{ };
            fScores[rootNode] = heuristicFunction(rootNode, goalNode);
            gScores[rootNode] = 0.0f;
            
            const auto allNodes = graph.GetNodes();

            for (const auto& node : allNodes)
            {
                if (node != rootNode)
                {
                    fScores[node] = std::numeric_limits<f32>::infinity();
                    gScores[node] = std::numeric_limits<f32>::infinity();
                }
                
                predecessorNodes[node] = NullOpt;
            }

            openSet.push({ rootNode, fScores[rootNode] });

            bool wasNodeFound = false;

            while (!openSet.empty())
            {
                const auto [currentNode, currentFScore] = openSet.top();
                openSet.pop();

                visitedNodes.insert(currentNode);

                if (currentNode == goalNode)
                {
                    wasNodeFound = true;

                    break;
                }

                const auto adjacentNodes = graph.GetEfficientAdjacentNodes(currentNode);

                for (const auto& [adjacentNode, distance] : adjacentNodes)
                {
                    const f32 tentativeGScore = gScores[currentNode] + distance;

                    if (tentativeGScore < gScores[adjacentNode])
                    {
                        gScores[adjacentNode] = tentativeGScore;
                        fScores[adjacentNode] = gScores[adjacentNode] + heuristicFunction(adjacentNode, goalNode);

                        predecessorNodes[adjacentNode] = currentNode;

                        if (!visitedNodes.contains(adjacentNode))
                        {
                            openSet.push({ adjacentNode, fScores[adjacentNode] });
                        }
                    }
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