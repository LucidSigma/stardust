#include "stardust/physics/AABB.h"

namespace stardust
{
	namespace physics
	{
		AABB::AABB(const Pair<Vec2, Vec2>& bounds)
		{
			m_aabb.lowerBound = b2Vec2{ bounds.first.x, bounds.first.y };
			m_aabb.upperBound = b2Vec2{ bounds.second.x, bounds.second.y };
		}

		AABB::AABB(const Vec2& centre, const Vec2& halfSize)
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

		void AABB::Combine(const AABB& other)
		{
			m_aabb.Combine(other);
		}

		void AABB::Combine(const AABB& first, const AABB& second)
		{
			m_aabb.Combine(first, second);
		}

		void AABB::Expand(const Vec2& amount)
		{
			m_aabb.lowerBound.x -= amount.x;
			m_aabb.lowerBound.y -= amount.y;

			m_aabb.upperBound.x += amount.x;
			m_aabb.upperBound.y += amount.y;
		}

		[[nodiscard]] bool AABB::Contains(const Vec2& point) const
		{
			return point.x > m_aabb.lowerBound.x && point.x < m_aabb.upperBound.x &&
				point.y > m_aabb.lowerBound.y && point.y < m_aabb.upperBound.y;
		}

		[[nodiscard]] bool AABB::Contains(const AABB& aabb) const
		{
			return m_aabb.Contains(aabb);
		}

		[[nodiscard]] bool AABB::IsValid() const
		{
			return m_aabb.IsValid();
		}

		[[nodiscard]] Vec2 AABB::GetLowerBound() const
		{
			return Vec2{ m_aabb.lowerBound.x, m_aabb.lowerBound.y };
		}

		void AABB::SetLowerBound(const Vec2& lowerBound)
		{
			m_aabb.lowerBound.x = lowerBound.x;
			m_aabb.lowerBound.y = lowerBound.y;
		}

		[[nodiscard]] Vec2 AABB::GetUpperBound() const
		{
			return Vec2{ m_aabb.upperBound.x, m_aabb.upperBound.y };
		}

		void AABB::SetUpperBound(const Vec2& upperBound)
		{
			m_aabb.upperBound.x = upperBound.x;
			m_aabb.upperBound.y = upperBound.y;
		}

		[[nodiscard]] f32 AABB::GetWidth() const
		{
			return m_aabb.upperBound.x - m_aabb.lowerBound.x;
		}

		[[nodiscard]] f32 AABB::GetHeight() const
		{
			return m_aabb.upperBound.y - m_aabb.lowerBound.y;
		}

		[[nodiscard]] f32 AABB::GetHalfWidth() const
		{
			return GetWidth() / 2.0f;
		}

		[[nodiscard]] f32 AABB::GetHalfHeight() const
		{
			return GetHeight() / 2.0f;
		}

		[[nodiscard]] Vec2 AABB::GetCentre() const
		{
			const b2Vec2 centre = m_aabb.GetCenter();

			return Vec2{ centre.x, centre.y };
		}

		void AABB::SetCentre(const Vec2& centre)
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

		[[nodiscard]] Vec2 AABB::GetExtents() const
		{
			const b2Vec2 extents = m_aabb.GetExtents();

			return Vec2{ extents.x, extents.y };
		}

		[[nodiscard]] Vec2 AABB::GetSize() const
		{
			return GetExtents() * 2.0f;
		}

		[[nodiscard]] f32 AABB::GetPerimeter() const
		{
			return m_aabb.GetPerimeter();
		}
	}
}