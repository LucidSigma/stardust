#pragma once
#ifndef STARDUST_AABB_H
#define STARDUST_AABB_H

#include <box2d/box2d.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace physics
    {
        class AABB
        {
        private:
            b2AABB m_aabb{ };

        public:
            AABB() = default;
            AABB(const b2AABB& aabb);
            explicit AABB(const Pair<Vec2, Vec2>& bounds);
            AABB(const Vec2& centre, const Vec2& halfSize);
            ~AABB() noexcept = default;

            void Combine(const AABB& other);
            void Combine(const AABB& first, const AABB& second);

            void Expand(f32 amount);
            void Expand(Vec2 amount);

            [[nodiscard]] bool Contains(const Vec2& point) const;
            [[nodiscard]] bool Contains(const AABB& aabb) const;

            [[nodiscard]] bool IsValid() const;

            [[nodiscard]] Vec2 GetLowerBound() const;
            void SetLowerBound(const Vec2& lowerBound);

            [[nodiscard]] Vec2 GetUpperBound() const;
            void SetUpperBound(const Vec2& upperBound);

            [[nodiscard]] f32 GetWidth() const;
            [[nodiscard]] f32 GetHeight() const;
            [[nodiscard]] f32 GetHalfWidth() const;
            [[nodiscard]] f32 GetHalfHeight() const;

            [[nodiscard]] Vec2 GetCentre() const;
            void SetCentre(const Vec2& centre);

            [[nodiscard]] Vec2 GetExtents() const;
            [[nodiscard]] Vec2 GetSize() const;

            [[nodiscard]] f32 GetPerimeter() const;

            [[nodiscard]] inline operator b2AABB() const noexcept { return m_aabb; }
        };
    }
}

#endif