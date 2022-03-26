#include "stardust/physics/geometry/Circle.h"

namespace stardust
{
    namespace physics
    {
        Circle::Circle(const b2CircleShape& shapeHandle)
            : m_circle(shapeHandle)
        { }

        Circle::Circle(const f32 radius, const Vector2 position)
        {
            m_circle.m_radius = radius;
            m_circle.m_p = b2Vec2{ position.x, position.y };
        }

        [[nodiscard]] auto Circle::GetPosition() const noexcept -> Vector2
        {
            return Vector2{ m_circle.m_p.x, m_circle.m_p.y };
        }

        auto Circle::SetPosition(const Vector2 position) noexcept -> void
        {
            m_circle.m_p = b2Vec2{ position.x, position.y };
        }

        [[nodiscard]] auto Circle::ContainsPoint(const Vector2 worldPosition, const f32 rotation, const Vector2 point) const -> bool
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            return m_circle.TestPoint(transform, b2Vec2{ point.x, point.y });
        }

        [[nodiscard]] auto Circle::ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            b2AABB aabb{ };
            m_circle.ComputeAABB(&aabb, transform, 0);

            return aabb;
        }

        [[nodiscard]] auto Circle::ComputeMassData(const f32 density) const -> MassData
        {
            b2MassData massData{ };
            m_circle.ComputeMass(&massData, density);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }
    }
}
