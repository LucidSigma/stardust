#include "stardust/physics/AABB.h"

namespace stardust
{
    namespace physics
    {
        AABB::AABB(const b2AABB& aabb)
            : m_aabb(aabb)
        { }

        AABB::AABB(const Pair<Vector2, Vector2>& bounds)
        {
            m_aabb.lowerBound = b2Vec2{ bounds.first.x, bounds.first.y };
            m_aabb.upperBound = b2Vec2{ bounds.second.x, bounds.second.y };
        }

        AABB::AABB(const Vector2 centre, const Vector2 halfSize)
        {
            m_aabb.lowerBound = b2Vec2{
                centre.x - halfSize.x,
                centre.y - halfSize.y,
            };

            m_aabb.upperBound = b2Vec2{
                centre.x + halfSize.x,
                centre.y + halfSize.y,
            };
        }

        auto AABB::Combine(const AABB& other) -> void
        {
            m_aabb.Combine(other);
        }

        auto AABB::Combine(const AABB& first, const AABB& second) -> void
        {
            m_aabb.Combine(first, second);
        }

        auto AABB::Expand(Vector2 amount) -> void
        {
            amount /= 2.0f;

            m_aabb.lowerBound.x -= amount.x;
            m_aabb.lowerBound.y -= amount.y;

            m_aabb.upperBound.x += amount.x;
            m_aabb.upperBound.y += amount.y;
        }

        auto AABB::Expand(f32 amount) -> void
        {
            amount /= 2.0f;

            m_aabb.lowerBound.x -= amount;
            m_aabb.lowerBound.y -= amount;

            m_aabb.upperBound.x += amount;
            m_aabb.upperBound.y += amount;
        }

        auto AABB::Expand(f32 xAmount, f32 yAmount) -> void
        {
            Expand(Vector2{ xAmount, yAmount });
        }

        auto AABB::Shrink(const Vector2 amount) -> void
        {
            Expand(-amount);
        }

        auto AABB::Shrink(const f32 amount) -> void
        {
            Expand(-amount);
        }

        auto AABB::Shrink(const f32 xAmount, const f32 yAmount) -> void
        {
            Expand(-xAmount, -yAmount);
        }

        [[nodiscard]] auto AABB::Contains(const Vector2 point) const -> bool
        {
            return point.x > m_aabb.lowerBound.x && point.x < m_aabb.upperBound.x &&
                point.y > m_aabb.lowerBound.y && point.y < m_aabb.upperBound.y;
        }

        [[nodiscard]] auto AABB::Contains(const AABB& aabb) const -> bool
        {
            return m_aabb.Contains(aabb);
        }

        [[nodiscard]] auto AABB::IsValid() const -> bool
        {
            return m_aabb.IsValid();
        }

        [[nodiscard]] auto AABB::GetLowerBound() const -> Vector2
        {
            return Vector2{ m_aabb.lowerBound.x, m_aabb.lowerBound.y };
        }

        auto AABB::SetLowerBound(const Vector2 lowerBound) -> void
        {
            m_aabb.lowerBound.x = lowerBound.x;
            m_aabb.lowerBound.y = lowerBound.y;
        }

        [[nodiscard]] auto AABB::GetUpperBound() const -> Vector2
        {
            return Vector2{ m_aabb.upperBound.x, m_aabb.upperBound.y };
        }

        auto AABB::SetUpperBound(const Vector2 upperBound) -> void
        {
            m_aabb.upperBound.x = upperBound.x;
            m_aabb.upperBound.y = upperBound.y;
        }

        [[nodiscard]] auto AABB::GetWidth() const -> f32
        {
            return m_aabb.upperBound.x - m_aabb.lowerBound.x;
        }

        [[nodiscard]] auto AABB::GetHeight() const -> f32
        {
            return m_aabb.upperBound.y - m_aabb.lowerBound.y;
        }

        [[nodiscard]] auto AABB::GetHalfWidth() const -> f32
        {
            return GetWidth() / 2.0f;
        }

        [[nodiscard]] auto AABB::GetHalfHeight() const -> f32
        {
            return GetHeight() / 2.0f;
        }

        [[nodiscard]] auto AABB::GetCentre() const -> Vector2
        {
            const b2Vec2 centre = m_aabb.GetCenter();

            return Vector2{ centre.x, centre.y };
        }

        auto AABB::SetCentre(const Vector2 centre) -> void
        {
            const f32 halfWidth = GetHalfWidth();
            const f32 halfHeight = GetHalfHeight();

            m_aabb.lowerBound = b2Vec2{
                centre.x - halfWidth,
                centre.y - halfHeight,
            };

            m_aabb.upperBound = b2Vec2{
                centre.x + halfWidth,
                centre.y + halfHeight,
            };
        }

        auto AABB::ShiftCentre(const Vector2 offset) -> void
        {
            SetCentre(GetCentre() + offset);
        }

        auto AABB::ShiftCentre(const f32 xOffset, const f32 yOffset) -> void
        {
            ShiftCentre(Vector2{ xOffset, yOffset });
        }

        [[nodiscard]] auto AABB::GetHalfSize() const -> Vector2
        {
            const b2Vec2 extents = m_aabb.GetExtents();

            return Vector2{ extents.x, extents.y };
        }

        [[nodiscard]] auto AABB::GetSize() const -> Vector2
        {
            return GetHalfSize() * 2.0f;
        }

        auto AABB::SetSize(const Vector2 size) -> void
        {
            const Vector2 oldSize = GetSize();
            Expand(size - oldSize);
        }

        [[nodiscard]] auto AABB::GetArea() const -> f32
        {
            return GetWidth() * GetHeight();
        }

        [[nodiscard]] auto AABB::GetPerimeter() const -> f32
        {
            return m_aabb.GetPerimeter();
        }
    }
}
