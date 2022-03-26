#pragma once
#ifndef STARDUST_AABB_H
#define STARDUST_AABB_H

#include <box2d/box2d.h>

#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace physics
    {
        class AABB final
        {
        private:
            b2AABB m_aabb{ };

        public:
            AABB() = default;
            AABB(const b2AABB& aabb);
            explicit AABB(const Pair<Vector2, Vector2>& bounds);
            AABB(const Vector2 centre, const Vector2 halfSize);

            auto Combine(const AABB& other) -> void;
            auto Combine(const AABB& first, const AABB& second) -> void;

            auto Expand(Vector2 amount) -> void;
            auto Expand(f32 amount) -> void;
            auto Expand(f32 xAmount, f32 yAmount) -> void;
            auto Shrink(const Vector2 amount) -> void;
            auto Shrink(const f32 amount) -> void;
            auto Shrink(const f32 xAmount, const f32 yAmount) -> void;

            [[nodiscard]] auto Contains(const Vector2 point) const -> bool;
            [[nodiscard]] auto Contains(const AABB& aabb) const -> bool;

            [[nodiscard]] auto IsValid() const -> bool;

            [[nodiscard]] auto GetLowerBound() const -> Vector2;
            auto SetLowerBound(const Vector2 lowerBound) -> void;

            [[nodiscard]] auto GetUpperBound() const -> Vector2;
            auto SetUpperBound(const Vector2 upperBound) -> void;

            [[nodiscard]] auto GetWidth() const -> f32;
            [[nodiscard]] auto GetHeight() const -> f32;
            [[nodiscard]] auto GetHalfWidth() const -> f32;
            [[nodiscard]] auto GetHalfHeight() const -> f32;

            [[nodiscard]] auto GetCentre() const -> Vector2;
            auto SetCentre(const Vector2 centre) -> void;
            auto ShiftCentre(const Vector2 offset) -> void;
            auto ShiftCentre(const f32 xOffset, const f32 yOffset) -> void;

            [[nodiscard]] auto GetHalfSize() const -> Vector2;
            [[nodiscard]] auto GetSize() const -> Vector2;
            auto SetSize(const Vector2 size) -> void;

            [[nodiscard]] auto GetArea() const -> f32;
            [[nodiscard]] auto GetPerimeter() const->f32;

            [[nodiscard]] inline operator b2AABB() const noexcept { return m_aabb; }
        };
    }
}

#endif
