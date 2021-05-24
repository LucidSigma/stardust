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
    }
}

#endif