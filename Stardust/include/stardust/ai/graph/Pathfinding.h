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
        [[nodiscard]] Vector<T> DepthFirstTraversal(const Graph<T>& graph, const T& root)
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
        [[nodiscard]] Vector<T> BreadthFirstTraversal(const Graph<T>& graph, const T& root)
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
    }
}

#endif