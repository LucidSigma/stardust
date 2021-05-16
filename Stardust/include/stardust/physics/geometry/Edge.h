#pragma once
#ifndef STARDUST_EDGE_H
#define STARDUST_EDGE_H

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
    namespace physics
    {
        class Edge
        {
        private:
            b2EdgeShape m_edge{ };

        public:
            Edge() = default;
            Edge(const b2EdgeShape& shapeHandle);
            Edge(const Vec2& vertexA, const Vec2& vertexB);
            Edge(const Vec2& ghostVertexA, const Vec2& vertexA, const Vec2& vertexB, const Vec2& ghostVertexB);
            ~Edge() noexcept = default;

            void SetOneSided(const Vec2& ghostVertexA, const Vec2& vertexA, const Vec2& vertexB, const Vec2& ghostVertexB);
            void SetTwoSided(const Vec2& vertexA, const Vec2& vertexB);

            [[nodiscard]] Pair<Vec2, Vec2> GetVertices() const;
            [[nodiscard]] Pair<Vec2, Vec2> GetGhostVertices() const;

            inline bool IsOneSided() const noexcept { return m_edge.m_oneSided; }

            [[nodiscard]] bool TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const;
            [[nodiscard]] AABB ComputeAABB(const Vec2& worldPosition, const f32 rotation) const;
            [[nodiscard]] MassData ComputeMassData(const f32 density) const;

            inline operator ObserverPtr<b2Shape>() noexcept { return &m_edge; }
            inline operator ObserverPtr<const b2Shape>() const noexcept { return &m_edge; }
            inline operator ObserverPtr<b2EdgeShape>() noexcept { return &m_edge; }
            inline operator ObserverPtr<const b2EdgeShape>() const noexcept { return &m_edge; }
        };
    }
}

#endif