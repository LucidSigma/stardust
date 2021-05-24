#pragma once
#ifndef STARDUST_GRAPH_H
#define STARDUST_GRAPH_H

#include <algorithm>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace ai
    {
        enum class EdgeDirection
        {
            Unidirectional,
            Bidirectional,
        };

        template <typename T>
        class Graph
        {
        private:
            struct EdgeData
            {
                T node;

                f32 weight;
                EdgeDirection direction;

                bool isBidirectionalDuplicate;
            };

            HashMap<T, Vector<EdgeData>> m_edges{ };

        public:
            Graph() = default;
            ~Graph() noexcept = default;

            void AddEdge(const T& source, const T& destination, const f32 weight = 1.0f, const EdgeDirection direction = EdgeDirection::Bidirectional)
            {
                if (!m_edges.contains(source))
                {
                    m_edges[source] = { };
                }

                if (!m_edges.contains(destination))
                {
                    m_edges[destination] = { };
                }

                m_edges[source].push_back(EdgeData{
                    .node = destination,
                    .weight = weight,
                    .direction = direction,
                    .isBidirectionalDuplicate = false,
                });

                if (direction == EdgeDirection::Bidirectional)
                {
                    m_edges[destination].push_back(EdgeData{
                        .node = source,
                        .weight = weight,
                        .direction = direction,
                        .isBidirectionalDuplicate = true,
                    });
                }
            }
            
            void AddEdgelessNode(const T& node)
            {
                if (!m_edges.contains(node))
                {
                    m_edges[node] = { };
                }
            }

            [[nodiscard]] bool DoNodesConnect(const T& source, const T& destination) const
            {
                if (const auto sourceLocation = m_edges.find(source);
                    sourceLocation != std::cend(m_edges))
                {
                    for (const auto& edgeData : sourceLocation->second)
                    {
                        if (edgeData.node == destination)
                        {
                            return true;
                        }
                    }
                }

                return false;
            }

            [[nodiscard]] HashSet<T> GetNodes() const
            {
                HashSet<T> nodes;
                nodes.reserve(m_edges.size());

                for (const auto& [node, edgeData] : m_edges)
                {
                    nodes.insert(node);
                }

                return nodes;
            }

            [[nodiscard]] MultiMap<T, f32> GetAdjacentNodes(const T& node) const
            {
                MultiMap<T, f32> adjacentNodes{ };

                if (const auto nodeLocation = m_edges.find(node);
                    nodeLocation != std::cend(m_edges))
                {
                    for (const auto& edgeData : nodeLocation->second)
                    {
                        adjacentNodes.insert({ edgeData.node, edgeData.weight });
                    }
                }

                return adjacentNodes;
            }

            void RemoveNode(const T& node)
            {
                m_edges.erase(node);
            
                for (auto& [sourceNode, edges] : m_edges)
                {
                    edges.erase(
                        std::remove_if(std::begin(edges), std::end(edges), [&node](const EdgeData& edgeData) -> bool
                        {
                            return edgeData.node == node;
                        }),
                        std::end(edges)
                    );
                }
            }

            [[nodiscard]] inline bool HasNode(const T& node) const noexcept { return m_edges.contains(node); }

            [[nodiscard]] inline usize GetOrder() const noexcept { return m_edges.size(); }

            [[nodiscard]] usize GetSize() const noexcept
            {
                usize edgeCount = 0u;

                for (const auto& [node, edges] : m_edges)
                {
                    for (const auto& edgeData : edges)
                    {
                        if (!edgeData.isBidirectionalDuplicate)
                        {
                            ++edgeCount;
                        }
                    }
                }

                return edgeCount;
            }

            [[nodiscard]] usize GetDegree(const T& node) const noexcept
            {
                if (const auto nodeLocation = m_edges.find(node);
                    nodeLocation != std::cend(m_edges))
                {
                    return nodeLocation->second.size();
                }

                return 0u;
            }
        };
    }
}

#endif