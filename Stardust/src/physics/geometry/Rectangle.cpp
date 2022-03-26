#include "stardust/physics/geometry/Rectangle.h"

#include "stardust/math/Math.h"

namespace stardust
{
    namespace physics
    {
        Rectangle::Rectangle(const f32 width)
        {
            SetAsSquare(width);
        }

        Rectangle::Rectangle(const f32 width, const f32 height)
        {
            Set(width, height);
        }

        auto Rectangle::SetAsSquare(const f32 width) -> void
        {
            m_polygon.SetAsBox(width / 2.0f, width / 2.0f);
            m_width = width;
            m_height = width;
        }

        auto Rectangle::SetAsSquare(const f32 width, const Vector2 pivot, const f32 rotation) -> void
        {
            m_polygon.SetAsBox(width / 2.0f, width / 2.0f, b2Vec2{ pivot.x, pivot.y }, -glm::radians(rotation));
            m_width = width;
            m_height = width;
        }

        auto Rectangle::Set(const f32 width, const f32 height) -> void
        {
            m_polygon.SetAsBox(width / 2.0f, height / 2.0f);
            m_width = width;
            m_height = height;
        }

        auto Rectangle::Set(const f32 width, const f32 height, const Vector2 pivot, const f32 rotation) -> void
        {
            m_polygon.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2{ pivot.x, pivot.y }, -glm::radians(rotation));
            m_width = width;
            m_height = height;
        }

        [[nodiscard]] auto Rectangle::ContainsPoint(const Vector2 worldPosition, const f32 rotation, const Vector2 point) const -> bool
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            return m_polygon.TestPoint(transform, b2Vec2{ point.x, point.y });
        }

        [[nodiscard]] auto Rectangle::ComputeAABB(const Vector2 worldPosition, const f32 rotation) const -> AABB
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            b2AABB aabb{ };
            m_polygon.ComputeAABB(&aabb, transform, 0);

            return aabb;
        }

        [[nodiscard]] auto Rectangle::ComputeMassData(const f32 density) const -> MassData
        {
            b2MassData massData{ };
            m_polygon.ComputeMass(&massData, density);

            return MassData{
                .mass = massData.mass,
                .centre = Vector2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }
    }
}
