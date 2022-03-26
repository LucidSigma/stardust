#pragma once
#ifndef STARDUST_BEZIER_H
#define STARDUST_BEZIER_H

#include "stardust/math/Math.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Bezier final
    {
    public:
        struct Points final
        {
            Vector2 start = Vector2Zero;
            Vector2 controlA = Vector2Zero;
            Vector2 controlB = Vector2Zero;
            Vector2 end = Vector2Zero;
        };

    private:
        Points m_points{ };

    public:
        Bezier() = default;
        Bezier(const Points& points);
        Bezier(const Vector2 start, const Vector2 controlA, const Vector2 controlB, const Vector2 end);

        [[nodiscard]] auto GetValue(const f32 t) const noexcept -> Vector2;

        [[nodiscard]] inline auto GetPoints() const noexcept -> const Points& { return m_points; }
        [[nodiscard]] inline auto GetStartPoint() const noexcept -> Vector2 { return m_points.start; }
        [[nodiscard]] inline auto GetFirstControlPoint() const noexcept -> Vector2 { return m_points.controlA; }
        [[nodiscard]] inline auto GetSecondControlPoint() const noexcept -> Vector2 { return m_points.controlB; }
        [[nodiscard]] inline auto GetEndPoint() const noexcept -> Vector2 { return m_points.end; }

        inline auto SetPoints(const Points& points) noexcept -> void { m_points = points; }
        inline auto SetStartPoint(const Vector2 point) noexcept -> void { m_points.start = point; }
        inline auto SetFirstControlPoint(const Vector2 point) noexcept -> void { m_points.controlA = point; }
        inline auto SetSecondControlPoint(const Vector2 point) noexcept -> void { m_points.controlB = point; }
        inline auto SetEndPoint(const Vector2 point) noexcept -> void { m_points.end = point; }

        [[nodiscard]] auto GetTangent(const f32 t) const noexcept -> Vector2;
        [[nodiscard]] auto GetNormal(const f32 t) const noexcept -> Vector2;
        [[nodiscard]] auto GetTangentAndNormal(const f32 t) const noexcept -> Pair<Vector2, Vector2>;

        [[nodiscard]] auto GetVelocity(const f32 t) const noexcept -> Vector2;
        [[nodiscard]] auto GetAcceleration(const f32 t) const noexcept -> Vector2;
        [[nodiscard]] auto GetJerk() const noexcept -> Vector2;

        [[nodiscard]] auto GetCurvature(const f32 t) const noexcept -> f32;

        [[nodiscard]] auto GetArcLength(const u32 segmentCount = 20u) const noexcept -> f32;
    };
}

#endif
