#pragma once
#ifndef STARDUST_POLYGON_H
#define STARDUST_POLYGON_H

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
        class Polygon final
        {
        private:
            b2PolygonShape m_polygon{ };

        public:
            [[nodiscard]] static constexpr auto GetMaxVertices() noexcept -> u32 { return b2_maxPolygonVertices; }

            Polygon() = default;
            Polygon(const b2PolygonShape& shapeHandle);
            explicit Polygon(const List<Vector2>& points);

            auto Set(const List<Vector2>& points) -> void;
            auto SetAsBox(const f32 width, const f32 height) -> void;
            auto SetAsBox(const f32 width, const f32 height, const Vector2 pivot, const f32 rotation) -> void;

            [[nodiscard]] auto GetCentroid() const noexcept -> Vector2;
            [[nodiscard]] auto GetVertices() const -> List<Vector2>;
            [[nodiscard]] auto GetNormals() const -> List<Vector2>;
            [[nodiscard]] inline auto GetVertexCount() const noexcept -> u32 { return static_cast<u32>(m_polygon.m_count); }

            [[nodiscard]] auto ContainsPoint(const Vector2 worldPosition, const f32 rotation, const Vector2 point) const -> bool;
            [[nodiscard]] auto ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB;
            [[nodiscard]] auto ComputeMassData(const f32 density) const -> MassData;

            [[nodiscard]] auto IsConvex() const -> bool;

            [[nodiscard]] inline operator ObserverPointer<b2Shape>() noexcept { return &m_polygon; }
            [[nodiscard]] inline operator ObserverPointer<const b2Shape>() const noexcept { return &m_polygon; }
            [[nodiscard]] inline operator ObserverPointer<b2PolygonShape>() noexcept { return &m_polygon; }
            [[nodiscard]] inline operator ObserverPointer<const b2PolygonShape>() const noexcept { return &m_polygon; }
        };
    }
}

#endif
