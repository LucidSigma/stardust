#pragma once
#ifndef STARDUST_CIRCLE_H
#define STARDUST_CIRCLE_H

#include <box2d/box2d.h>

#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/math/Math.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
    namespace physics
    {
        class Circle final
        {
        private:
            b2CircleShape m_circle{ };

        public:
            Circle() = default;
            Circle(const b2CircleShape& shapeHandle);
            explicit Circle(const f32 radius, const Vector2 position = Vector2Zero);

            [[nodiscard]] auto GetPosition() const noexcept -> Vector2;
            auto SetPosition(const Vector2 position) noexcept -> void;

            [[nodiscard]] inline auto GetRadius() const noexcept -> f32 { return m_circle.m_radius; }
            [[nodiscard]] inline auto SetRadius(const f32 radius) noexcept -> void { m_circle.m_radius = radius; }

            [[nodiscard]] auto ContainsPoint(const Vector2 worldPosition, const f32 rotation, const Vector2 point) const -> bool;
            [[nodiscard]] auto ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB;
            [[nodiscard]] auto ComputeMassData(const f32 density) const -> MassData;

            [[nodiscard]] inline operator ObserverPointer<b2Shape>() noexcept { return &m_circle; }
            [[nodiscard]] inline operator ObserverPointer<const b2Shape>() const noexcept { return &m_circle; }
            [[nodiscard]] inline operator ObserverPointer<b2CircleShape>() noexcept { return &m_circle; }
            [[nodiscard]] inline operator ObserverPointer<const b2CircleShape>() const noexcept { return &m_circle; }
        };
    }
}

#endif
