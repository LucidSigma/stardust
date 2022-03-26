#pragma once
#ifndef STARDUST_CHAIN_H
#define STARDUST_CHAIN_H

#include <box2d/box2d.h>

#include "stardust/physics/geometry/Edge.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        class Chain final
        {
        private:
            b2ChainShape m_chain{ };

        public:
            Chain() = default;
            Chain(const b2ChainShape& shapeHandle);
            explicit Chain(const List<Vector2>& vertices);
            Chain(const List<Vector2>& vertices, const Vector2 ghostVertexA, const Vector2 ghostVertexB);

            auto CreateLoop(const List<Vector2>& vertices) -> void;
            auto CreateChain(const List<Vector2>& vertices, const Vector2 ghostVertexA, const Vector2 ghostVertexB) -> void;

            auto Clear() -> void;

            [[nodiscard]] auto GetVertices() const -> List<Vector2>;
            [[nodiscard]] inline auto GetVertexCount() const noexcept -> u32 { return static_cast<u32>(m_chain.m_count); }
            [[nodiscard]] auto GetGhostVertices() const -> Pair<Vector2, Vector2>;

            [[nodiscard]] auto GetChildEdge(const u32 childEdgeIndex) const -> Edge;
            [[nodiscard]] inline auto GetChildEdgeCount() const noexcept -> u32 { return static_cast<u32>(m_chain.GetChildCount()); }

            [[nodiscard]] auto ComputeEdgeAABB(const Vector2 worldPosition, const f32 rotation, const u32 childEdgeIndex) const -> AABB;
            [[nodiscard]] auto ComputeMassData(const f32 density) const -> MassData;

            [[nodiscard]] inline operator ObserverPointer<b2Shape>() noexcept { return &m_chain; }
            [[nodiscard]] inline operator ObserverPointer<const b2Shape>() const noexcept { return &m_chain; }
            [[nodiscard]] inline operator ObserverPointer<b2ChainShape>() noexcept { return &m_chain; }
            [[nodiscard]] inline operator ObserverPointer<const b2ChainShape>() const noexcept { return &m_chain; }
        };
    }
}

#endif
