#pragma once
#ifndef STARDUST_RECTANGLE_H
#define STARDUST_RECTANGLE_H

#include <box2d/box2d.h>

#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/physics/AABB.h"
#include "stardust/physics/Physics.h"

namespace stardust
{
    namespace physics
    {
        class Rectangle
        {
        private:
            b2PolygonShape m_polygon{ };

            f32 m_width = 0.0f;
            f32 m_height = 0.0f;

        public:
            Rectangle() = default;
            explicit Rectangle(const f32 width);
            Rectangle(const f32 width, const f32 height);
            ~Rectangle() noexcept = default;

            void SetAsSquare(const f32 width);
            void SetAsSquare(const f32 width, const Vec2& pivot, const f32 rotation);
            void Set(const f32 width, const f32 height);
            void Set(const f32 width, const f32 height, const Vec2& pivot, const f32 rotation);

            inline f32 GetWidth() const noexcept { return m_width; }
            inline f32 GetHeight() const noexcept { return m_height; }

            [[nodiscard]] bool TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const;
            [[nodiscard]] AABB ComputeAABB(const Vec2& worldPosition, const f32 rotation) const;
            [[nodiscard]] MassData ComputeMassData(const f32 density) const;

            inline operator ObserverPtr<b2Shape>() noexcept { return &m_polygon; }
            inline operator ObserverPtr<const b2Shape>() const noexcept { return &m_polygon; }
            inline operator ObserverPtr<b2PolygonShape>() noexcept { return &m_polygon; }
            inline operator ObserverPtr<const b2PolygonShape>() const noexcept { return &m_polygon; }
        };
    }
}

#endif