#pragma once
#ifndef STARDUST_RECTANGLE_H
#define STARDUST_RECTANGLE_H

#include <box2d/box2d.h>

#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        class Rectangle final
        {
        private:
            b2PolygonShape m_polygon{ };

            f32 m_width = 0.0f;
            f32 m_height = 0.0f;

        public:
            Rectangle() = default;
            explicit Rectangle(const f32 width);
            Rectangle(const f32 width, const f32 height);

            auto SetAsSquare(const f32 width) -> void;
            auto SetAsSquare(const f32 width, const Vector2 pivot, const f32 rotation) -> void;
            auto Set(const f32 width, const f32 height) -> void;
            auto Set(const f32 width, const f32 height, const Vector2 pivot, const f32 rotation) -> void;

            [[nodiscard]] inline auto GetWidth() const noexcept -> f32 { return m_width; }
            [[nodiscard]] inline auto GetHeight() const noexcept -> f32 { return m_height; }

            [[nodiscard]] auto ContainsPoint(const Vector2 worldPosition, const f32 rotation, const Vector2 point) const -> bool;
            [[nodiscard]] auto ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB;
            [[nodiscard]] auto ComputeMassData(const f32 density) const -> MassData;

            [[nodiscard]] inline operator ObserverPointer<b2Shape>() noexcept { return &m_polygon; }
            [[nodiscard]] inline operator ObserverPointer<const b2Shape>() const noexcept { return &m_polygon; }
            [[nodiscard]] inline operator ObserverPointer<b2PolygonShape>() noexcept { return &m_polygon; }
            [[nodiscard]] inline operator ObserverPointer<const b2PolygonShape>() const noexcept { return &m_polygon; }
        };
    }
}

#endif
