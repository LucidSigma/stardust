#include "stardust/math/splines/Bezier.h"

namespace stardust
{
    Bezier::Bezier(const Points& points)
        : m_points(points)
    { }

    Bezier::Bezier(const Vector2 start, const Vector2 controlA, const Vector2 controlB, const Vector2 end)
        : Bezier(
            Points{
                .start = start,
                .controlA = controlA,
                .controlB = controlB,
                .end = end,
            }
          )
    { }

    [[nodiscard]] auto Bezier::GetValue(const f32 t) const noexcept -> Vector2
    {
        const f32 tSquared = t * t;
        const f32 tCubed = t * t * t;

        return m_points.start * (-tCubed + 3.0f * tSquared - 3.0f * t + 1.0f) +
            m_points.controlA * (3.0f * tCubed - 6.0f * tSquared + 3.0f * t) +
            m_points.controlB * (-3.0f * tCubed + 3.0f * tSquared) +
            m_points.end * tCubed;
    }

    [[nodiscard]] auto Bezier::GetTangent(const f32 t) const noexcept -> Vector2
    {
        return glm::normalize(GetVelocity(t));
    }

    [[nodiscard]] auto Bezier::GetNormal(const f32 t) const noexcept -> Vector2
    {
        return glm::rotate(GetTangent(t), glm::radians(90.0f));
    }

    [[nodiscard]] auto Bezier::GetTangentAndNormal(const f32 t) const noexcept -> Pair<Vector2, Vector2>
    {
        const Vector2 tangent = GetTangent(t);
        const Vector2 normal = glm::rotate(tangent, glm::radians(90.0f));

        return { tangent, normal };
    }

    [[nodiscard]] auto Bezier::GetVelocity(const f32 t) const noexcept -> Vector2
    {
        const f32 tSquared = t * t;

        return m_points.start * (-3.0f * tSquared + 6.0f * t - 3.0f) +
            m_points.controlA * (9.0f * tSquared - 12.0f * t + 3.0f) +
            m_points.controlB * (-9.0f * tSquared + 6.0f * t) +
            m_points.end * (3.0f * tSquared);
    }

    [[nodiscard]] auto Bezier::GetAcceleration(const f32 t) const noexcept -> Vector2
    {
        return m_points.start * (-6.0f * t + 6.0f) +
            m_points.controlA * (18.0f * t - 12.0f) +
            m_points.controlB * (-18.0f * t + 6.0f) +
            m_points.end * (6.0f * t);
    }

    [[nodiscard]] auto Bezier::GetJerk() const noexcept -> Vector2
    {
        return m_points.start * -6.0f +
            m_points.controlA * 18.0f +
            m_points.controlB * -18.0f +
            m_points.end * 6.0f;
    }

    [[nodiscard]] auto Bezier::GetCurvature(const f32 t) const noexcept -> f32
    {
        const Vector2 velocity = GetVelocity(t);
        const Vector2 acceleration = GetAcceleration(t);
        const Matrix2 combinedVelocityAcceleration(velocity.x, velocity.y, acceleration.x, acceleration.y);

        const f32 determinant = glm::determinant(combinedVelocityAcceleration);
        const f32 speed = glm::length(velocity);

        return determinant / (speed * speed * speed);
    }

    [[nodiscard]] auto Bezier::GetArcLength(const u32 segmentCount) const noexcept -> f32
    {
        f32 arcLength = 0.0f;
        Vector2 previousPoint = Vector2Zero;

        for (u32 i = 0u; i <= segmentCount; ++i)
        {
            const f32 t = static_cast<f32>(i) / static_cast<f32>(segmentCount);
            const Vector2 currentPoint = GetValue(t);

            if (i > 0u)
            {
                const Vector2 pointDifference = currentPoint - previousPoint;
                arcLength += glm::length(pointDifference);
            }

            previousPoint = currentPoint;
        }

        return arcLength;
    }
}
