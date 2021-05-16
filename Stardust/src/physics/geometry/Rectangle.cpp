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

        void Rectangle::SetAsSquare(const f32 width)
        {
            m_polygon.SetAsBox(width / 2.0f, width / 2.0f);
            m_width = width;
            m_height = width;
        }

        void Rectangle::SetAsSquare(const f32 width, const Vec2& pivot, const f32 rotation)
        {
            m_polygon.SetAsBox(width / 2.0f, width / 2.0f, b2Vec2{ pivot.x, pivot.y }, -glm::radians(rotation));
            m_width = width;
            m_height = width;
        }

        void Rectangle::Set(const f32 width, const f32 height)
        {
            m_polygon.SetAsBox(width / 2.0f, height / 2.0f);
            m_width = width;
            m_height = height;
        }

        void Rectangle::Set(const f32 width, const f32 height, const Vec2& pivot, const f32 rotation)
        {
            m_polygon.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2{ pivot.x, pivot.y }, -glm::radians(rotation));
            m_width = width;
            m_height = height;
        }

        [[nodiscard]] bool Rectangle::TestPoint(const Vec2& worldPosition, const f32 rotation, const Vec2& point) const
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            return m_polygon.TestPoint(transform, b2Vec2{ point.x, point.y });
        }

        [[nodiscard]] AABB Rectangle::ComputeAABB(const Vec2& worldPosition, const f32 rotation) const
        {
            b2Transform transform{ };
            transform.Set(b2Vec2{ worldPosition.x, worldPosition.y }, -glm::radians(rotation));

            b2AABB aabb{ };
            m_polygon.ComputeAABB(&aabb, transform, 0);

            return aabb;
        }

        [[nodiscard]] MassData Rectangle::ComputeMassData(const f32 density) const
        {
            b2MassData massData{ };
            m_polygon.ComputeMass(&massData, density);

            return MassData{
                .mass = massData.mass,
                .centre = Vec2{ massData.center.x, massData.center.y },
                .momentOfInertia = massData.I,
            };
        }
    }
}