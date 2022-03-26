#pragma once
#ifndef STARDUST_EDGE_H
#define STARDUST_EDGE_H

#include <box2d/box2d.h>

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
        class Edge final
        {
        private:
            b2EdgeShape m_edge{ };

        public:
            Edge() = default;
            Edge(const b2EdgeShape& shapeHandle);
            Edge(const Vector2 vertexA, const Vector2 vertexB);
            Edge(const Vector2 ghostVertexA, const Vector2 vertexA, const Vector2 vertexB, const Vector2 ghostVertexB);

            auto SetOneSided(const Vector2 ghostVertexA, const Vector2 vertexA, const Vector2 vertexB, const Vector2 ghostVertexB) -> void;
            auto SetTwoSided(const Vector2 vertexA, const Vector2 vertexB) -> void;

            [[nodiscard]] auto GetVertices() const -> Pair<Vector2, Vector2>;
            [[nodiscard]] auto GetGhostVertices() const -> Pair<Vector2, Vector2>;

            [[nodiscard]] inline auto IsOneSided() const noexcept -> bool { return m_edge.m_oneSided; }

            [[nodiscard]] auto ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB;
            [[nodiscard]] auto ComputeMassData(const f32 density) const -> MassData;

            [[nodiscard]] inline operator ObserverPointer<b2Shape>() noexcept { return &m_edge; }
            [[nodiscard]] inline operator ObserverPointer<const b2Shape>() const noexcept { return &m_edge; }
            [[nodiscard]] inline operator ObserverPointer<b2EdgeShape>() noexcept { return &m_edge; }
            [[nodiscard]] inline operator ObserverPointer<const b2EdgeShape>() const noexcept { return &m_edge; }
        };
    }
}

#endif
