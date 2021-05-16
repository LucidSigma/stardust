#pragma once
#ifndef STARDUST_CIRCLE_H
#define STARDUST_CIRCLE_H

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/math/Math.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
    namespace physics
    {
        class Circle
        {
        private:
            b2CircleShape m_circle{ };

        public:
            Circle() = default;
            Circle(const b2CircleShape& shapeHandle);
            explicit Circle(const f32 radius, const Vec2& position = Vec2Zero);
            ~Circle() noexcept = default;

            [[nodiscard]] Vec2 GetPosition() const noexcept;
            void SetPosition(const Vec2& position) noexcept;
            
            inline f32 GetRadius() const noexcept { return m_circle.m_radius; }
            inline void SetRadius(const f32 radius) noexcept { m_circle.m_radius = radius; }

            [[nodiscard]] bool TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const;
            [[nodiscard]] AABB ComputeAABB(const Vec2& worldPosition, const f32 rotation) const;
            [[nodiscard]] MassData ComputeMassData(const f32 density) const;

            inline operator ObserverPtr<b2Shape>() noexcept { return &m_circle; }
            inline operator ObserverPtr<const b2Shape>() const noexcept { return &m_circle; }
            inline operator ObserverPtr<b2CircleShape>() noexcept { return &m_circle; }
            inline operator ObserverPtr<const b2CircleShape>() const noexcept { return &m_circle; }
        };
    }
}

#endif